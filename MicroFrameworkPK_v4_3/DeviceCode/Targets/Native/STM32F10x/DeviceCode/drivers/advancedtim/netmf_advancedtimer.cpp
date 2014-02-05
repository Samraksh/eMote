/*
 * 	Author 			: 			nived.sivadas
 *
 *  Description     :
 *
 *
 *
 */


#include "netmf_advancedtimer.h"
#include <Samraksh\HALTimer.h>
#include <pwr/netmf_pwr.h>
#include <rcc/stm32f10x_rcc.h>


STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
STM32F10x_Timer_Configuration g_STM32F10x_Timer_Configuration;


extern "C"
{
void ISR_TIM2(void* Param);
void ISR_TIM1(void* Param);
}

extern HALTimerManager gHalTimerManagerObject;
// Initialize the virtual timer layer
// This is to ensure that users can not remove the hardware timer from underneath the virtual timer
// layer without the knowledge of the virtual timer
BOOL HAL_Time_Initialize()
{
	return gHalTimerManagerObject.Initialize();
	//return TRUE;
}

// This function is called the uninitialize the timer system and the virtual timer layer
BOOL HAL_Time_Uninitialize()
{
	return gHalTimerManagerObject.DeInitialize();
}

// This function has been tested using the rollover test for advanced timers - Level_0b
UINT64 HAL_Time_CurrentTicks()
{
	return g_STM32F10x_AdvancedTimer.Get64Counter();
}


INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
	return CPU_TicksToTime( Ticks );
}

// This function has been tested using the rollover test for advanced timers - level_0c
INT64 HAL_Time_CurrentTime()
{
	return CPU_TicksToTime(HAL_Time_CurrentTicks());
}

void HAL_Time_SetCompare( UINT64 CompareValue )
{
	g_STM32F10x_AdvancedTimer.SetCompare(0, CompareValue);
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    //*a = 1;
    //*b = 1;
    //*c = 0;

	// "correct"*A = raw*B + C
	// Correct is defined as 10 MHz
	// Raw is actually 8 MHz, so ratio is 1.25 = 5/4
	// 4*correct = 5*raw + 0
	*a = 4;
    *b = 5;
    *c = 0;
}

UINT64 Time_CurrentTicks()
{
    return HAL_Time_CurrentTicks();
}

// This function is tuned for 8MHz of the emote
// Will not work at other speeds at low uSec values ie ( < 30)
// This function has poor accuracy at less than 10 microsecs
// Coming to the first if condition takes 13.5 us so for values less than 10 this is the best we can do

void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{


	if(uSec <= 10)
	{
	//	UINT32 limit = uSec / 5;
	//	for(volatile UINT32 i = 0; i < limit; i++);
		return;
	}

	GLOBAL_LOCK(irq);

	if(uSec <= 30)
	{
		UINT32 limit = (uSec)/ 5;
		for(volatile UINT32 i = 0; i < limit; i++);
		return;
	}

	UINT32 currentCounterVal = g_STM32F10x_AdvancedTimer.GetCounter();

	UINT32 ticks = CPU_MicrosecondsToTicks(uSec);

	while(g_STM32F10x_AdvancedTimer.GetCounter() - currentCounterVal <= ticks);

	//while((((TIM2->CNT << 16) | TIM1->CNT) - currentCounterVal) <= ticks);


}

// This function is tuned to work when the processor is running at 8 MHz
// When that changes this function no longer works efficiently
void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	UINT32 limit = (uSec / 5);
	for(volatile UINT32 i = 0; i < limit; i++);
}

// Returns the current 32 bit value of the hardware counter
UINT32 STM32F10x_AdvancedTimer::GetCounter()
{
//	timerLock.acquire_and_save();

	// Fix for rollover bug
	UINT16 tim1a = TIM1->CNT;
	UINT16 tim2a = TIM2->CNT;
	UINT16 tim1b = TIM1->CNT;

	if(tim1b < tim1a)
	{
		tim2a = TIM2->CNT;
	}


	currentCounterValue = ((tim2a << 16) | tim1b);

//	timerLock.release_and_restore();

	return currentCounterValue;
}


UINT64 STM32F10x_AdvancedTimer::Get64Counter()
{

	UINT32 currentValue = GetCounter();

	//timerLock.acquire_and_save();

	m_lastRead &= (0xFFFFFFFF00000000ull);

	if(DidTimerOverflow())
	{
		ClearTimerOverflow();
		m_lastRead += (0x1ull <<32);
	}

	m_lastRead |= currentValue;
	//timerLock.release_and_restore();
	return m_lastRead;
}


BOOL STM32F10x_AdvancedTimer::DidTimerOverflow()
{
	return timerOverflowFlag;
}

void STM32F10x_AdvancedTimer::ClearTimerOverflow()
{
	timerOverflowFlag = FALSE;
}

//	Initialize the advanced timer system. This inolves initializing timer1 as a master timer and tim2 as a slave
//  and using timer1 as a prescaler to timer2.
DeviceStatus STM32F10x_AdvancedTimer::Initialize(UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void *ISR_Param)
{

	// Return if already initialized
	if(STM32F10x_AdvancedTimer::initialized)
		return DS_Success;

	STM32F10x_AdvancedTimer::initialized = TRUE;

	// Fix for usart failure on adding the advanced timer in the system
	// Fix breaks the NOR because it shares the remapped pins with the advanced timer
	// Attempting to change the comparator
	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

	// Initializes the special deferred function
	TaskletType* tasklet = GetTasklet();

	// Maintains the last recorded 32 bit counter value
	currentCounterValue = 0;

	// Set the timer overflow flag to false during intialization
	// This flag is set when an over flow happens on timer 2 which happens to represent
	// the most significant 16 bits of our timer system
	// This event has to be recorded to ensure that we keep track of ticks expired since birth
	timerOverflowFlag = FALSE;

	timerLock.init();

	// Keep tasklet ready for insertion into bh queue
	tasklet->action = ISR;
	if(!ISR_Param)
		tasklet->data = &currentCounterValue;
	else
		tasklet->data = ISR_Param;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1;

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	/* Slave Mode selection: TIM2 */
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Gated);
	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);

	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	// TIM1 timebase
	if (RCC_Clocks.HCLK_Frequency == RCC_Clocks.PCLK2_Frequency) {
		// No prescaler, so TIM clock == PCLK2
		TIM_TimeBaseStructure.TIM_Prescaler = 0;
	}
	else {
		// Prescaler, so TIM clock = PCLK2 x 2
		TIM_TimeBaseStructure.TIM_Prescaler = 5; // TODO make this more general
	}

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 127;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1;

    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    /* Master Mode selection */
  	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);

  	/* Select the Master Slave Mode */
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);


    // Active timer 1 cc interrupt
	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM1_CC, ISR_TIM1, NULL) )
		return DS_Fail;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_TIM1_CC))
		return DS_Fail;

	// Activate TIM2 interrupt
	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM2, ISR_TIM2, NULL) )
		return DS_Fail;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_TIM2))
		return DS_Fail;

	//TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
	//TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);

	// Need the update flag for overflow bookkeeping
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	// Enable both the timers, TIM1 because it the LS two bytes
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2,ENABLE);

    // Initialize the timer parameters
    // should this be in  the beginning of this function or end ?
    // At this point not sure
    g_STM32F10x_Timer_Configuration.Initialize();

    return DS_Success;

}

// Set compare happens in two stages, the first stage involves setting the msb on tim2
// the second stage involves lsb on tim1
DeviceStatus STM32F10x_AdvancedTimer::SetCompare(UINT32 counterCorrection, UINT32 compareValue)
{

	UINT32 newCompareValue;

	if(counterCorrection == 0)
	{
		newCompareValue = GetCounter() + compareValue;
#if 0
		if(newCompareValue <  GetCounter())
		{
			 TaskletType* tasklet = g_STM32F10x_AdvancedTimer.GetTasklet();

			// Schedule bottom half processing on arrival of interrupt
			Tasklet_Schedule_hi(tasklet);

			return DS_Success;
		}
#endif
		if(compareValue >> 16)
		{
			//TIM_SetCompare1(TIM2, TIM2->CNT + (compareValue >> 16));
			TIM_SetCompare1(TIM2, newCompareValue >> 16);

			TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
		}
		else
		{
			TIM_SetCompare3(TIM1, newCompareValue & 0xffff);

			TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
		}
	}
	else
	{
		newCompareValue = counterCorrection +  compareValue;
#if 0
		// We have gone past the counter value so set interrupt now
		if(newCompareValue <  GetCounter())
		{
		    TaskletType* tasklet = g_STM32F10x_AdvancedTimer.GetTasklet();

			// Schedule bottom half processing on arrival of interrupt
			Tasklet_Schedule_hi(tasklet);

			return DS_Success;
		}
#endif
		if(compareValue >> 16)
		{
			TIM_SetCompare1(TIM2, newCompareValue >> 16);

			TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
		}
		else
		{
			TIM_SetCompare3(TIM1, newCompareValue & 0xffff);

			TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
		}

	}

	currentCompareValue = newCompareValue;

	return DS_Success;
}

extern "C"
{

void ISR_TIM2(void* Param)
{

	if(TIM_GetFlagStatus(TIM2, TIM_IT_CC1))
	{
		TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		//hal_printf("Current Compare Value %d", g_STM32F10x_AdvancedTimer.currentCompareValue);
		// Set the lsb of the 32 bit timer
		//TIM_SetCompare2(TIM1, TIM1->CNT + (g_STM32F10x_AdvancedTimer.currentCompareValue & 0xffff));
		//TIM_SetCompare2(TIM1, (g_STM32F10x_AdvancedTimer.currentCompareValue & 0xffff));

		// Unsure how there is an extra pending interrupt at this point. This is causing a bug
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);

		// Small values on the lsb are sometimes missed

		UINT16 lsbValue = g_STM32F10x_AdvancedTimer.currentCompareValue & 0xffff;

		if(TIM1->CNT > lsbValue || (lsbValue - TIM1->CNT) < 750)
		//if(TIM1->CNT > lsbValue)
		{
			// Fire now we already missed the counter value
			// Create a software trigger
			TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
			g_STM32F10x_AdvancedTimer.TriggerSoftwareInterrupt();

			//TaskletType* tasklet = g_STM32F10x_AdvancedTimer.GetTasklet();

			// Schedule bottom half processing on arrival of interrupt
			//Tasklet_Schedule_hi(tasklet);
		}
		else
		{
			TIM_SetCompare3(TIM1, (g_STM32F10x_AdvancedTimer.currentCompareValue & 0xffff));
			TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
		}



	}
	if(TIM_GetFlagStatus(TIM2, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		// An overflow just happened, update the 64 bit value
		// maintained in software
		// This is needed because microsoft's timers are polling based and
		// poll this 64 bit number

		g_STM32F10x_AdvancedTimer.timerOverflowFlag = TRUE;

		g_STM32F10x_AdvancedTimer.Get64Counter();
	}

}

void ISR_TIM1( void* Param )
{
	// calculate the current 32 bit value of the counters to account for
	// time spent in the handler when the next compare interrupt is set
	// This variable is pointed to by the tasklet
	// So changing the contents of this variable should be done with extreme caution
	// Update the 64 bit counter value
	g_STM32F10x_AdvancedTimer.Get64Counter();


	if(TIM_GetFlagStatus(TIM1, TIM_IT_CC3))
	{
		TIM_ITConfig(TIM1, TIM_IT_CC3, DISABLE);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
		//NVIC->ICPR[STM32_AITC::c_IRQ_INDEX_TIM1_CC >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_TIM1_CC & (UINT8)0x1F);

		 //TIM_SetCompare2(TIM1, 500 + (g_STM32F10x_AdvancedTimer.currentCounterValue & 0xffff));


		TaskletType* tasklet = g_STM32F10x_AdvancedTimer.GetTasklet();

		 // Schedule bottom half processing on arrival of interrupt
     	Tasklet_Schedule_hi(tasklet);
		//CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
		//CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);


	}

}

}

