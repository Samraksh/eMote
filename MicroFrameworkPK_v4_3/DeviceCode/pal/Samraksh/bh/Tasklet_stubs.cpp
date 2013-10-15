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
	return TRUE;
}

BOOL Tasklet_Schedule(TaskletType* tasklet)
{
	return TRUE;
}

BOOL Tasklet_Schedule_hi(TaskletType* tasklet)
{
	return TRUE;
}

BOOL Tasklet_Run()
{
	return TRUE;
}

// This function is used to initialize the tasklet system
// Internally intializes the tasklet lock and the task queues
// both low and high priority
// Initialize the svc interrupt handler
BOOL Tasklet::initialize()
{
	return TRUE;
}


BOOL Tasklet::schedule(TaskletType* tasklet)
{
	return TRUE;
}

// This function schedules a tasklet in the high priority queue
// This queue should be reserved only for time critical drivers and no one else
// The idea is to provide a simple mechanism to avoid time critical bottom halves to
// be behind a slower device
BOOL Tasklet::schedule_hi(TaskletType* tasklet)
{
	return TRUE;
}

BOOL Tasklet::enable()
{
	return TRUE;
}

BOOL Tasklet::disable()
{
	return TRUE;
}

BOOL Tasklet::causeirq()
{
	return TRUE;
}

// This function needs to be made faster
// At the moment there are multiple calls to check if queues are empty
// In the average case the queues are going to empty most of the time
BOOL Tasklet::runhigh()
{
	return TRUE;
}

BOOL Tasklet::runlow()
{
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

