#include "netmf_advancedtimer.h"
#include <stm32f10x.h>
#include <Samraksh/VirtualTimer.h>
#include <pwr/netmf_pwr.h>

STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
STM32F10x_Timer_Configuration g_STM32F10x_Timer_Configuration;

void ISR_TIM2(void* Param);
void ISR_TIM1(void* Param);

const UINT64 TIME_CUSHION = 40;  // 15 us
const uint16_t TOO_SHORT_TIM1 = 40;
const UINT64 MAX_ALLOWABLE_WAIT = 0xFFFEFFFF;

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

BOOL STM32F10x_AdvancedTimer::AddTicks(UINT64 ticksToAdd)
{
	UINT64 originalTime =Get64Counter();

	UINT16 ticksToAdd_TIM1 = ticksToAdd & 0xffff;
	UINT16 ticksToAdd_TIM2 = (ticksToAdd & 0xffff0000) >> 16;
	UINT64 ticksToAdd_UPPER32 = (UINT64)((ticksToAdd & 0xffffffff00000000) >> 32);

	GLOBAL_LOCK(irq);
	UINT32 TIM1_new = TIM1->CNT + ticksToAdd_TIM1;
	UINT32 TIM2_new = (UINT32)TIM2->CNT + (UINT32)ticksToAdd_TIM2 + (UINT32)(TIM1_new >> 16);
	UINT64 systemTime_new = (UINT64)(g_STM32F10x_AdvancedTimer.m_systemTime >> 32) + ticksToAdd_UPPER32 + (UINT64)(TIM2_new >> 16);
	TIM1->CNT = TIM1_new;
	TIM2->CNT = TIM2_new;
	g_STM32F10x_AdvancedTimer.m_systemTime = (UINT64)(systemTime_new<<32) + (UINT64)(TIM2->CNT<<16) + (UINT64)TIM1->CNT;

	// it should only take a few thousand ticks to calculate everything...a rollover could happen though so we just do it again
	while ((g_STM32F10x_AdvancedTimer.m_systemTime - (originalTime + ticksToAdd)) > 3000){
		originalTime =Get64Counter();
		TIM1_new = TIM1->CNT + ticksToAdd_TIM1;
		TIM2_new = (UINT32)TIM2->CNT + (UINT32)ticksToAdd_TIM2 + (UINT32)(TIM1_new >> 16);
		systemTime_new = (UINT64)(g_STM32F10x_AdvancedTimer.m_systemTime >> 32) + ticksToAdd_UPPER32 + (UINT64)(TIM2_new >> 16);
		TIM1->CNT = TIM1_new;
		TIM2->CNT = TIM2_new;
		g_STM32F10x_AdvancedTimer.m_systemTime = (UINT64)(systemTime_new<<32) + (UINT64)(TIM2->CNT<<16) + (UINT64)TIM1->CNT;
	}

	//hal_printf("%llx %llx %llx %llx\r\n",g_STM32F10x_AdvancedTimer.m_systemTime, Get64Counter(), (originalTime ), (g_STM32F10x_AdvancedTimer.m_systemTime - (originalTime + ticksToAdd)));
	return 0;
}

UINT32 STM32F10x_AdvancedTimer::SetCounter(UINT32 counterValue)
{
	currentCounterValue = counterValue;
	return currentCounterValue;
}


UINT64 STM32F10x_AdvancedTimer::Get64Counter()
{
	UINT32 currentValue = GetCounter();

	if(TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		// An overflow just happened, updating variable that holds system time
		g_STM32F10x_AdvancedTimer.m_systemTime += (0x1ull <<32);
	}

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentValue;

	UINT64 firstReading = m_systemTime;

	currentValue = GetCounter();

	if(TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		// An overflow just happened, updating variable that holds system time
		g_STM32F10x_AdvancedTimer.m_systemTime += (0x1ull <<32);
	}

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentValue;

	if ((m_systemTime > (firstReading + 350))|| (m_systemTime < firstReading)){
		//hal_printf("\r\nadv first: %llu cur: %llu\r\n",firstReading,m_systemTime);
		 currentValue = GetCounter();
		 m_systemTime &= (0xFFFFFFFF00000000ull);
		 m_systemTime |= currentValue;
		 //hal_printf("\r\n3rd: %llu\r\n",m_systemTime);
		return currentValue;
	}
	else 
		return m_systemTime;



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
DeviceStatus STM32F10x_AdvancedTimer::Initialize(UINT32 Prescaler, HAL_CALLBACK_FPN ISR, UINT32 ISR_Param)
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
	setCompareRunning = false;

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
	// Accounts for default High Speed (64 MHz).
	// TODO: Make this smarter
		TIM_TimeBaseStructure.TIM_Prescaler = 7; // TODO make this more general
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

// Assumes IRQs locked
static inline void clear_tim2(void) {
	TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	__DSB(); __ISB();
}

// Assumes IRQs locked
static inline void clear_tim1(void) {
	TIM_ITConfig(TIM1, TIM_IT_CC3, DISABLE);
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
	__DSB(); __ISB();
}

// Assumes IRQs locked
static inline void clear_timers(void) {
	TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(TIM1, TIM_IT_CC3, DISABLE);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
	__DSB(); __ISB();
}

DeviceStatus STM32F10x_AdvancedTimer::UnInitialize()
{
    CPU_INTC_DeactivateInterrupt( TIM1_CC_IRQn );
    CPU_INTC_DeactivateInterrupt( TIM2_IRQn );
    callBackISR = NULL;
    TIM_DeInit( TIM1 );
    TIM_DeInit( TIM2 );

    return DS_Success;
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
	UINT64 now;
#if defined(DEBUG_EMOTE_ADVTIME)
	// if two timers need to fire at (or very close to) the same time, then the second timer will be passed to this function with a compare value of the current (at the time of setting it) time
	// So by the time we get to this portion of code NowTicks will be greater than compareValue. 
	// we then add on a TIME_CUSHION below and the second timer will fire at the current time plus the TIME_CUSHION
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
	now = Get64Counter();

	// Clear old timers if already an active request
	if (setCompareRunning == true) {
		clear_timers();
	}
	// making sure we have enough time before the timer fires to exit SetCompare, the VT callback and the timer interrupt
	if (compareValue < (now + TIME_CUSHION)){
		compareValue = (now + TIME_CUSHION);
	}  
	if ( (compareValue - now) > MAX_ALLOWABLE_WAIT ){ // checking to see if our wait time maxes out our upper 16-bit timer		
		// we are to trigger at a value that maxes out our upper 16-bit timer so in order to wait as long as possible and not
		// incorrectly trigger the miss condition below we set the timer for the longest possible value that won't trigger the miss condition
		compareValue = (UINT64)(GetCounter() - 0x00010000); 
	} 
	currentTarget = compareValue;

	setCompareRunning = true;

	tar_upper = (compareValue >> 16) & 0xFFFF;
	now_upper = (now >> 16) & 0xFFFF;

	// if the upper 16-bit counter already matches we don't bother setting it and just set the lower 16-bit below
	if (tar_upper != now_upper){
		TIM_SetCompare1(TIM2, tar_upper);
		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

		// Check for miss
		// This catches cases where upper 16 were set to be for example, 0x0005xxxx at a time of 0x0004 ffff. We roll-over and catch that here
		if (TIM2->CNT != tar_upper || TIM_GetITStatus(TIM2,TIM_IT_CC1) == SET ) {
			// Didn't miss, done for now
			return DS_Success;
		}
		else { // We missed. Back-off.
			clear_tim2();
		}
	}

	// because we added on a TIME_CUSHION earlier, there should be enough time to exit and enable interrupts before TIM1 fires
	tar_lower = compareValue & 0xFFFF;
	TIM_SetCompare3(TIM1, tar_lower);
	TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
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

		// here we check to see if we have enough time to set TIM1 and exit
		volatile UINT64 currentTime = g_STM32F10x_AdvancedTimer.Get64Counter();
		if ( g_STM32F10x_AdvancedTimer.currentTarget < (currentTime + TOO_SHORT_TIM1) ){
			g_STM32F10x_AdvancedTimer.setCompareRunning = false; // Reset
			g_STM32F10x_AdvancedTimer.callBackISR(&g_STM32F10x_AdvancedTimer.callBackISR_Param);
		} else {
			// we shouldn't have to check to see if there is enough time to set TIM1 and exit before the timer compare happens (because of our TOO_SHORT_TIM1 check earlier)
			// but if there are problems (i.e. TIM1 wrap arounds) then we need to add a check here
			TIM_SetCompare3(TIM1, g_STM32F10x_AdvancedTimer.currentTarget  & 0xFFFF);
			TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
		}			
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

		g_STM32F10x_AdvancedTimer.setCompareRunning = false; // Reset
		g_STM32F10x_AdvancedTimer.callBackISR(&g_STM32F10x_AdvancedTimer.callBackISR_Param);
	}
}
