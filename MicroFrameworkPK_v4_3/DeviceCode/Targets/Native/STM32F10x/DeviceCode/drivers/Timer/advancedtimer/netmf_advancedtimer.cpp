/*
* Author : nived.sivadas
*
* Description :
*
*
*
*/


#include "netmf_advancedtimer.h"
#include <stm32f10x.h>
#include <Samraksh/VirtualTimer.h>
#include <pwr/netmf_pwr.h>
//#include <rcc/stm32f10x_rcc.h>


STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
STM32F10x_Timer_Configuration g_STM32F10x_Timer_Configuration;

void ISR_TIM2(void* Param);
void ISR_TIM1(void* Param);

// Initialize the virtual timer layer
// This is to ensure that users can not remove the hardware timer from underneath the virtual timer
// layer without the knowledge of the virtual timer


// Returns the current 32 bit value of the hardware counter
UINT32 STM32F10x_AdvancedTimer::GetCounter()
{
	GLOBAL_LOCK(irq);
	UINT16 tim1a = TIM1->CNT;
	UINT16 tim2a = TIM2->CNT;
	UINT16 tim1b = TIM1->CNT;

	if(tim1b < tim1a)
	{
		tim2a = TIM2->CNT;
	}

	currentCounterValue = ((tim2a << 16) | tim1b);
	return currentCounterValue;
}


UINT32 STM32F10x_AdvancedTimer::SetCounter(UINT32 counterValue)
{
	currentCounterValue = counterValue;
}


UINT64 STM32F10x_AdvancedTimer::Get64Counter()
{
	UINT32 currentValue = GetCounter();

	m_lastRead &= (0xFFFFFFFF00000000ull);

	if(DidTimerOverflow())
	{
		ClearTimerOverflow();
		m_lastRead += (0x1ull <<32);
		currentValue = GetCounter();
	}

	m_lastRead |= currentValue;

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

// Initialize the advanced timer system. This inolves initializing timer1 as a master timer and tim2 as a slave
// and using timer1 as a prescaler to timer2.
DeviceStatus STM32F10x_AdvancedTimer::Initialize(UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void *ISR_Param)
{

	// Return if already initialized
	if(STM32F10x_AdvancedTimer::initialized)
		return DS_Success;

	m_lastRead = 0;

	STM32F10x_AdvancedTimer::initialized = TRUE;

	// Fix for usart failure on adding the advanced timer in the system
	// Fix breaks the NOR because it shares the remapped pins with the advanced timer
	// Attempting to change the comparator
	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

	// Maintains the last recorded 32 bit counter value
	currentCounterValue = 0;

	// Set the timer overflow flag to false during intialization
	// This flag is set when an over flow happens on timer 2 which happens to represent
	// the most significant 16 bits of our timer system
	// This event has to be recorded to ensure that we keep track of ticks expired since birth
	timerOverflowFlag = FALSE;

	//timerLock.init();

	callBackISR = ISR;
	callBackISR_Param = ISR_Param;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	// Reset
	TIM_DeInit(TIM1);
	TIM_DeInit(TIM2);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

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

    /* Master Mode selection */
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);

   /* Select the Master Slave Mode */
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);


    // Active timer 1 cc interrupt
	if( !CPU_INTC_ActivateInterrupt(TIM1_CC_IRQn, ISR_TIM1, NULL) )
		return DS_Fail;

	// Activate TIM2 interrupt
	if( !CPU_INTC_ActivateInterrupt(TIM2_IRQn, ISR_TIM2, NULL) )
		return DS_Fail;

	//TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
	//TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);

	// Need the update flag for overflow bookkeeping
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	// Enable both the timers, TIM1 because it the LS two bytes
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2,ENABLE);

    // Initialize the timer parameters
    // should this be in the beginning of this function or end ?
    // At this point not sure
    g_STM32F10x_Timer_Configuration.Initialize();

	//===========================================
	// The following wait and re-initialization is needed to get the SetCompare() working
	for(int i=0; i<100000;i++){}

	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    return DS_Success;

}

#if defined(DEBUG_EMOTE_ADVTIME)
volatile UINT64 badSetComparesCount = 0;    //!< number of requests set in the past.
volatile UINT64 badSetComparesAvg = 11;     //!< average delay of requests set in the past. init to observed value.
volatile UINT64 badSetComparesMax = 0;      //!< observed worst-case.
#endif
// Set compare happens in two stages, the first stage involves setting the msb on tim2
// the second stage involves lsb on tim1
DeviceStatus STM32F10x_AdvancedTimer::SetCompare(UINT64 counterCorrection, UINT64 compareValue, SetCompareType scType)
{
	UINT64 newCompareValue;
	UINT64 init = Get64Counter();

	// making sure we have enough time before the timer fires to exit SetCompare, the VT callback and the timer interrupt
	// TODO: change the 800 to something that is not hardcoded as this could break at other frequencies
	if (compareValue < (init + 800)){
		compareValue += 800;
	}

	if(counterCorrection == 0)
	{
		newCompareValue = compareValue;
	}
	else
	{
		newCompareValue = counterCorrection + compareValue;
	}
	
	if(newCompareValue >> 16)
	{
		if (scType == SET_COMPARE_TIMER){
			TIM_SetCompare1(TIM2, newCompareValue >> 16);
			TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
		} else {
			TIM_SetCompare2(TIM2, newCompareValue >> 16);
			TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
		}
	}
	else
	{
		if (scType == SET_COMPARE_TIMER){
#if defined(DEBUG_EMOTE_ADVTIME)
			volatile UINT64 Now = g_STM32F10x_AdvancedTimer.Get64Counter();
			if(Now > (counterCorrection + compareValue)) {
				UINT64 delta = Now - counterCorrection + compareValue;
				++badSetComparesCount;
				if(badSetComparesMax < delta) {
					badSetComparesMax = delta;
				}
				badSetComparesAvg = (badSetComparesAvg * (badSetComparesCount - 1) + (delta)) / badSetComparesCount;
			}
#else
			if(newCompareValue < 100) {
				newCompareValue += 100;
			}
#endif
				TIM_SetCompare3(TIM1, newCompareValue & 0xffff);
				TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
		} else {
				TIM_SetCompare2(TIM1, newCompareValue & 0xffff);
				TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
		}
	}

	
	currentCompareValue = newCompareValue;
	return DS_Success;
}

UINT32 STM32F10x_AdvancedTimer::GetMaxTicks()
{
	return (UINT32)0xFFFFFFFF;
}

void ISR_TIM2(void* Param)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1))
	{
		TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		// Unsure how there is an extra pending interrupt at this point. This is causing a bug
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);

		// Small values on the lsb are sometimes missed

		UINT16 lsbValue = g_STM32F10x_AdvancedTimer.currentCompareValue & 0xffff;

		//If only 10 ticks remain, might as well trigger the event. Else, set the compare
		//value for interrupt to go off at scheduled time.
		if(TIM1->CNT > lsbValue || (lsbValue - TIM1->CNT) < 10)	
		{
			// Fire now we already missed the counter value
			// Create a software trigger
			////TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
			g_STM32F10x_AdvancedTimer.callBackISR(g_STM32F10x_AdvancedTimer.callBackISR_Param);

		}
		else
		{
			TIM_SetCompare3(TIM1, (g_STM32F10x_AdvancedTimer.currentCompareValue & 0xffff));
			TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
		}
	}
	if(TIM_GetITStatus(TIM2, TIM_IT_CC2))
	{
		TIM_ITConfig(TIM2, TIM_IT_CC2, DISABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

		// Unsure how there is an extra pending interrupt at this point. This is causing a bug
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);

		// Small values on the lsb are sometimes missed

		UINT16 lsbValue = g_STM32F10x_AdvancedTimer.currentCompareValue & 0xffff;

		if(TIM1->CNT > lsbValue || (lsbValue - TIM1->CNT) < 10)
		//if(TIM1->CNT > lsbValue)
		{
			// Fire now we already missed the counter value
			// Create a software trigger
			TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);

		}
		else
		{
			TIM_SetCompare2(TIM1, (g_STM32F10x_AdvancedTimer.currentCompareValue & 0xffff));
			TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
		}
	}
	if(TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		// An overflow just happened, update the 64 bit value
		// maintained in software
		// This is needed because microsoft's timers are polling based and
		// poll this 64 bit number

		g_STM32F10x_AdvancedTimer.timerOverflowFlag = TRUE;
	}
}

void ISR_TIM1( void* Param )
{
	if(TIM_GetITStatus(TIM1, TIM_IT_CC3))
	{
		TIM_ITConfig(TIM1, TIM_IT_CC3, DISABLE);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
		//NVIC->ICPR[STM32_AITC::c_IRQ_INDEX_TIM1_CC >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_TIM1_CC & (UINT8)0x1F);

		//TIM_SetCompare2(TIM1, 500 + (g_STM32F10x_AdvancedTimer.currentCounterValue & 0xffff));


		g_STM32F10x_AdvancedTimer.callBackISR(g_STM32F10x_AdvancedTimer.callBackISR_Param);

	}
	if(TIM_GetITStatus(TIM1, TIM_IT_CC2))
	{
		TIM_ITConfig(TIM1, TIM_IT_CC2, DISABLE);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
		HAL_COMPLETION::DequeueAndExec();
	}
}
