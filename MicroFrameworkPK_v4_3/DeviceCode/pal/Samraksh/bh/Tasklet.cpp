/*
 * 		File Name	:  Tasklet.cpp
 *
 *      Author 		:  nived.sivadas@samrkash.com
 *
 *      Describes   : implements delayed processing using tasklets which are essentially soft irqs implemented
 *                    here using the System call handler since we do not use this handler in MF. At the moment the implementation
 *                    is designed to work only on the emotedotnow and the equivalent of the system call software interrupt
 *                    is needed to work on the other platforms.
 *
 *
 */
#include <Samraksh/Tasklet.h>

#ifdef PLATFORM_ARM_EmoteDotNow
#include <intc/STM32.h>
#endif

static void __SVC()  { __asm volatile ("svc #0x32");}


extern "C"
{
	void ISR_Tasklet_High(void *param);
	void ISR_Tasklet_Low(void *param);
	void Timer_Test_Handler(void *param);
}

Tasklet gTaskletManager;

BOOL Tasklet_Initialize()
{
	return gTaskletManager.initialize();
}

BOOL Tasklet_Schedule(TaskletType* tasklet)
{
	return gTaskletManager.schedule(tasklet);
}

BOOL Tasklet_Schedule_hi(TaskletType* tasklet)
{
	return gTaskletManager.schedule_hi(tasklet);
}

BOOL Tasklet_Run()
{
	return gTaskletManager.causeirq();
}

// This function is used to initialize the tasklet system
// Internally intializes the tasklet lock and the task queues
// both low and high priority
// Initialize the svc interrupt handler
BOOL Tasklet::initialize()
{

	taskletQueueLowPriority.Initialize();
	taskletQueueHighPriority.Initialize();


	if(!CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_Tasklet_High, ISR_Tasklet_High, NULL))
		return FALSE;

	if(!CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_Tasklet_Low, ISR_Tasklet_Low, NULL))
			return FALSE;

	//if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_SVCall, ISR_Tasklet, NULL) )
	//	return FALSE;

	initialized = TRUE;

	return TRUE;

}


BOOL Tasklet::schedule(TaskletType* tasklet)
{
	if(!initialized)
		return FALSE;

	// Protect the queue data structure from simultaneous access
	// may be between two high priority interrupts
	// one consumes and one adds to the queue
	// no relative favouring at this point, producers and consumers are treated alike
	while(!taskletLock.acquire_and_save());

	TaskletType** taskletInQueue = taskletQueueLowPriority.Push();

	if(!(*taskletInQueue))
	{
		*taskletInQueue = tasklet;
	}

		// release the interrupts
	while(!taskletLock.release_and_restore());

	return TRUE;
}

// This function schedules a tasklet in the high priority queue
// This queue should be reserved only for time critical drivers and no one else
// The idea is to provide a simple mechanism to avoid time critical bottom halves to
// be behind a slower device
BOOL Tasklet::schedule_hi(TaskletType* tasklet)
{
	if(!initialized)
		return FALSE;

	// failed to acquire a lock, likely stepped on another thread
	// need a spin lock version of this function
	// at the moment implementing spin lock through busy wait
	while(!taskletLock.acquire_and_save());

	TaskletType** taskletInQueue = taskletQueueHighPriority.Push();

	if(!(*taskletInQueue))
	{
		*taskletInQueue = tasklet;
	}

	while(!taskletLock.release_and_restore());

	return TRUE;

}

BOOL Tasklet::enable()
{
	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_SVCall))
			return FALSE;

	return TRUE;
}

BOOL Tasklet::disable()
{
	if(!CPU_INTC_InterruptDisable(STM32_AITC::c_IRQ_INDEX_SVCall))
		return FALSE;

	return TRUE;
}

BOOL Tasklet::causeirq()
{
	STM32_AITC& AITC = STM32::AITC();

	AITC.SetInterrupt(STM32_AITC::c_IRQ_INDEX_Tasklet_High);

	AITC.SetInterrupt(STM32_AITC::c_IRQ_INDEX_Tasklet_Low);

	//__SVC();

}

// This function needs to be made faster
// At the moment there are multiple calls to check if queues are empty
// In the average case the queues are going to empty most of the time
BOOL Tasklet::runhigh()
{
	TaskletType **tasklet = NULL;
	// Protect against using the tasklet without initializing it
	if(!initialized)
		return FALSE;

	// Attempt to acquire a lock about to access shared data structure
	while(!taskletLock.acquire_and_save());

	if(!taskletQueueHighPriority.IsEmpty())
	{
		tasklet = taskletQueueHighPriority.Pop();

	}

	while(!taskletLock.release_and_restore());

	if(tasklet == NULL)
		return FALSE;

	// call the tasklet function
	if((*tasklet)->action)
	{
		(*tasklet)->action((*tasklet)->data);
	}

	while(!taskletLock.acquire_and_save());

	if(!taskletQueueHighPriority.IsEmpty())
	{
		NVIC->ISPR[STM32_AITC::c_IRQ_INDEX_Tasklet_High >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_Tasklet_High & (UINT8)0x1F);
	}

	while(!taskletLock.release_and_restore());


	return TRUE;
}

BOOL Tasklet::runlow()
{
	TaskletType **tasklet = NULL;
	// Protect against using the tasklet without initializing it
	if(!initialized)
		return FALSE;

	STM32_AITC& AITC = STM32::AITC();

	AITC.ClearInterrupt(STM32_AITC::c_IRQ_INDEX_Tasklet_Low);

	// Attempt to acquire a lock about to access shared data structure
	while(!taskletLock.acquire_and_save());

	if(!taskletQueueLowPriority.IsEmpty())
	{
		tasklet = taskletQueueLowPriority.Pop();

	}

	while(!taskletLock.release_and_restore());

	if(tasklet == NULL)
		return FALSE;

	// call the tasklet function
	if((*tasklet)->action)
	{
		(*tasklet)->action((*tasklet)->data);
	}

	while(!taskletLock.acquire_and_save());

	if(!taskletQueueLowPriority.IsEmpty())
	{
		NVIC->ISPR[STM32_AITC::c_IRQ_INDEX_Tasklet_Low >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_Tasklet_Low & (UINT8)0x1F);
	}

	while(!taskletLock.release_and_restore());

	return TRUE;

}

extern "C"
{
	void ISR_Tasklet_High(void *param)
	{
		gTaskletManager.runhigh();
	}

	void ISR_Tasklet_Low(void *param)
	{
		gTaskletManager.runlow();
	}
}

