#include "netmf_advancedtimer.h"
#include <stm32f10x.h>
#include <Samraksh/VirtualTimer.h>
#include <pwr/netmf_pwr.h>

STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
STM32F10x_Timer_Configuration g_STM32F10x_Timer_Configuration;

static void set_compare_lower_16(UINT64 target);

void ISR_TIM2(void* Param);
void ISR_TIM1(void* Param);

const uint16_t MISSED_TIMER_DELAY = (uint16_t)(0.000005 * g_HardwareTimerFrequency[0]); // 5us @ 8 MHz 
const uint16_t TIME_CUSHION = (uint16_t)(0.000015 * g_HardwareTimerFrequency[0]); // 15us @ 8 MHz

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
	return currentCounterValue;
}


UINT64 STM32F10x_AdvancedTimer::Get64Counter()
{
	UINT32 currentValue = GetCounter();

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentValue;

	return m_systemTime;
}


BOOL STM32F10x_AdvancedTimer::DidTimerOverflow()
{
	//return timerOverflowFlag;
	return 0;
}

void STM32F10x_AdvancedTimer::ClearTimerOverflow()
{
	//timerOverflowFlag = FALSE;
}

// Initialize the advanced timer system. This involves initializing timer1 as a master timer and tim2 as a slave
// and using timer1 as a prescaler to timer2.
DeviceStatus STM32F10x_AdvancedTimer::Initialize(UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void *ISR_Param)
{

	// Return if already initialized
	if(STM32F10x_AdvancedTimer::initialized)
		return DS_Success;

	m_systemTime = 0;

	STM32F10x_AdvancedTimer::initialized = TRUE;

	// Fix for usart failure on adding the advanced timer in the system
	// Fix breaks the NOR because it shares the remapped pins with the advanced timer
	// Attempting to change the comparator
	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

	// Maintains the last recorded 32 bit counter value
	currentCounterValue = 0;
	currentCompareValue = 0xFFFFFFFFFFFFFFFFull;

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

	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // Active timer 1 cc interrupt
	if( !CPU_INTC_ActivateInterrupt(TIM1_CC_IRQn, ISR_TIM1, NULL) )
		return DS_Fail;

	// Activate TIM2 interrupt
	if( !CPU_INTC_ActivateInterrupt(TIM2_IRQn, ISR_TIM2, NULL) )
		return DS_Fail;

	//TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
	//TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);

	// Need the update flag for overflow bookkeeping
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	// Enable both the timers, TIM1 because it the LS two bytes
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2,ENABLE);

    // Initialize the timer parameters
    // should this be in the beginning of this function or end ?
    // At this point not sure
    g_STM32F10x_Timer_Configuration.Initialize();

	//===========================================

    return DS_Success;

}

// Assumes ONLY lower 16-bit need to be aligned.
// There will be hiccups when lower 16-bits is small, e.g. 0
static void set_compare_lower_16(UINT64 target) {
	uint16_t tar_lower;
	uint16_t now_lower;
	uint32_t now;

	GLOBAL_LOCK(irq);
	now = g_STM32F10x_AdvancedTimer.GetCounter();
	tar_lower = target & 0xFFFF;
	now_lower = now & 0xFFFF;

	// Don't attempt to set a timer for less than 5us.
	if ( tar_lower >= now_lower){
		if ( (tar_lower - now_lower) < MISSED_TIMER_DELAY){
			tar_lower = now_lower + MISSED_TIMER_DELAY;
		}
	} else {
		if ( ( (g_STM32F10x_AdvancedTimer.GetMaxTicks() - now_lower) + tar_lower) < MISSED_TIMER_DELAY) {
			tar_lower = now_lower + MISSED_TIMER_DELAY;
		}
	}

	TIM_SetCompare3(TIM1, tar_lower);
	TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
}

#if defined(DEBUG_EMOTE_ADVTIME)
volatile UINT64 badSetComparesCount = 0;       //!< number of requests set in the past.
volatile UINT64 badSetComparesAvg = 0;         //!< average delay of requests set in the past.
volatile UINT64 badSetComparesMax = 0;         //!< observed worst-case.
#endif
// Set compare happens in two stages, the first stage involves setting the msb on tim2
// the second stage involves lsb on tim1
DeviceStatus STM32F10x_AdvancedTimer::SetCompare(UINT64 compareValue)
{
	uint16_t tar_upper;
	uint16_t now_upper;
	uint32_t now;
#if defined(DEBUG_EMOTE_ADVTIME)
	volatile UINT64 NowTicks = g_STM32F10x_AdvancedTimer.Get64Counter();
	if(NowTicks > compareValue) {
		UINT64 delta = NowTicks - compareValue;
		++badSetComparesCount;
		if(badSetComparesMax < delta) {
			badSetComparesMax = delta;
		}
		badSetComparesAvg = (badSetComparesAvg * (badSetComparesCount - 1) + (delta)) / badSetComparesCount;
	}
#endif
	GLOBAL_LOCK(irq);
	now = g_STM32F10x_AdvancedTimer.Get64Counter();

	// Ignore bogus requests. There are several types. More to be added.
	// 1. Ignore requests for times that not before the current open request.
	if ( compareValue >= currentCompareValue ) {
		return DS_Success;
	}

	// making sure we have enough time before the timer fires to exit SetCompare, the VT callback and the timer interrupt
	// TODO: change the 800 to something that is not hardcoded as this could break at other frequencies
	if (compareValue < (now + TIME_CUSHION)){
		compareValue += TIME_CUSHION;
	}

	tar_upper = (compareValue >> 16) & 0xFFFF;
	now_upper = (now >> 16) & 0xFFFF;

	// This is either a bad input or a 32-bit roll-over.
	// Assume the latter, doesn't hurt us... I think --NPS
	TIM_SetCompare1(TIM2, tar_upper);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

	// Check for miss
	if (TIM2->CNT != tar_upper || TIM_GetITStatus(TIM2,TIM_IT_CC1) == SET ) {
		currentCompareValue = compareValue;
		// Didn't miss, done for now
		return DS_Success;
	}
	else { // We missed. Back-off.
		TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		__DSB(); __ISB();
	}

	set_compare_lower_16(compareValue);
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

		set_compare_lower_16( g_STM32F10x_AdvancedTimer.currentCompareValue );
	}

	if(TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		// An overflow just happened, updating variable that holds system time
		g_STM32F10x_AdvancedTimer.m_systemTime += (0x1ull <<32);
	}
}

void ISR_TIM1( void* Param )
{
	if(TIM_GetITStatus(TIM1, TIM_IT_CC3))
	{
		TIM_ITConfig(TIM1, TIM_IT_CC3, DISABLE);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);

		// Do we really want to run callback in ISR context? Shouldn't this be a continuation except for RT? --NPS
		g_STM32F10x_AdvancedTimer.currentCompareValue = 0xFFFFFFFFFFFFFFFFull; // Reset
		g_STM32F10x_AdvancedTimer.callBackISR(g_STM32F10x_AdvancedTimer.callBackISR_Param);
	}
}
