#include "timer_32bit.h"
#include <max3263x.h>
#include <mxc_sys.h>
#include <Samraksh/VirtualTimer.h>
#include <pwr/netmf_pwr.h>
#include <tmr.h>

Max3263x_timer_32bit g_Timer32Bit_Driver;
#define Max3263x_TIMER MXC_TMR2

void ISR_TIM32(void* Param);

const UINT64 TIME_CUSHION = 40;  // 15 us
const uint16_t TOO_SHORT_TIM1 = 40;
const UINT64 MAX_ALLOWABLE_WAIT = 0xFFFEFFFF;

// Returns the current 32 bit value of the hardware counter
UINT32 Max3263x_timer_32bit::GetCounter()
{
	/*GLOBAL_LOCK(irq);
	UINT16 tim1a = TIM1->CNT;
	UINT16 tim2a = TIM2->CNT;
	UINT16 tim1b = TIM1->CNT;

	if(tim1b < tim1a)
	{
		tim2a = TIM2->CNT;
	}

	currentCounterValue = ((tim2a << 16) | tim1b);*/
	return currentCounterValue;
}

BOOL Max3263x_timer_32bit::AddTicks(UINT64 ticksToAdd)
{
/*	UINT64 originalTime =Get64Counter();

	UINT16 ticksToAdd_TIM1 = ticksToAdd & 0xffff;
	UINT16 ticksToAdd_TIM2 = (ticksToAdd & 0xffff0000) >> 16;
	UINT64 ticksToAdd_UPPER32 = (UINT64)((ticksToAdd & 0xffffffff00000000) >> 32);

	GLOBAL_LOCK(irq);
	UINT32 TIM1_new = TIM1->CNT + ticksToAdd_TIM1;
	UINT32 TIM2_new = (UINT32)TIM2->CNT + (UINT32)ticksToAdd_TIM2 + (UINT32)(TIM1_new >> 16);
	UINT64 systemTime_new = (UINT64)(g_Timer32Bit_Driver.m_systemTime >> 32) + ticksToAdd_UPPER32 + (UINT64)(TIM2_new >> 16);
	TIM1->CNT = TIM1_new;
	TIM2->CNT = TIM2_new;
	g_Timer32Bit_Driver.m_systemTime = (UINT64)(systemTime_new<<32) + (UINT64)(TIM2->CNT<<16) + (UINT64)TIM1->CNT;

	// it should only take a few thousand ticks to calculate everything...a rollover could happen though so we just do it again
	while ((g_Timer32Bit_Driver.m_systemTime - (originalTime + ticksToAdd)) > 3000){
		originalTime =Get64Counter();
		TIM1_new = TIM1->CNT + ticksToAdd_TIM1;
		TIM2_new = (UINT32)TIM2->CNT + (UINT32)ticksToAdd_TIM2 + (UINT32)(TIM1_new >> 16);
		systemTime_new = (UINT64)(g_Timer32Bit_Driver.m_systemTime >> 32) + ticksToAdd_UPPER32 + (UINT64)(TIM2_new >> 16);
		TIM1->CNT = TIM1_new;
		TIM2->CNT = TIM2_new;
		g_Timer32Bit_Driver.m_systemTime = (UINT64)(systemTime_new<<32) + (UINT64)(TIM2->CNT<<16) + (UINT64)TIM1->CNT;
	}*/

	//hal_printf("%llx %llx %llx %llx\r\n",g_Timer32Bit_Driver.m_systemTime, Get64Counter(), (originalTime ), (g_Timer32Bit_Driver.m_systemTime - (originalTime + ticksToAdd)));
	return 0;
}

UINT32 Max3263x_timer_32bit::SetCounter(UINT32 counterValue)
{
//	currentCounterValue = counterValue;
	return currentCounterValue;
}


UINT64 Max3263x_timer_32bit::Get64Counter()
{
	// keeping an interrupt from happening right now causing timer problems where the value can be wrong at times.
/*	GLOBAL_LOCK(irq);
	UINT32 currentValue = GetCounter();

	if(TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		// An overflow just happened, updating variable that holds system time
		g_Timer32Bit_Driver.m_systemTime += (0x1ull <<32);
		// if the timer wrapped and caused an overflow which calls for an increment to systemTime then we need to pull
		// the current counter value again because the counter could be WAY off right now
		currentValue = GetCounter();
	}

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentValue;*/

	return m_systemTime;
}


/*BOOL Max3263x_timer_32bit::DidTimerOverflow()
{
	//return timerOverflowFlag;
	return 0;
}

void Max3263x_timer_32bit::ClearTimerOverflow()
{
	//timerOverflowFlag = FALSE;
}*/

// Initialize the advanced timer system. This involves initializing timer1 as a master timer and tim2 as a slave
// and using timer1 as a prescaler to timer2.
DeviceStatus Max3263x_timer_32bit::Initialize(HAL_CALLBACK_FPN ISR, UINT32 ISR_Param)
{
	int err;
    int tmrNum;

	tmr = Max3263x_TIMER;

	// Return if already initialized
	if(Max3263x_timer_32bit::initialized)
		return DS_Success;

	m_systemTime = 0;

	Max3263x_timer_32bit::initialized = TRUE;

	// Maintains the last recorded 32 bit counter value
	setCompareRunning = false;

	callBackISR = ISR;
	callBackISR_Param = ISR_Param;

	//get the timer number
    tmrNum = MXC_TMR_GET_IDX(tmr);

    //check for valid pointer
    ASSERT(tmrNum >= 0);

    //steup system GPIO config
    //if((err = SYS_TMR_Init(tmr, sysCfg)) != E_NO_ERROR)
    //    return err;

    //Disable timer and clear settings
    tmr->ctrl = 0;

    //reset all counts to 0
    tmr->count32 = 0;
    tmr->count16_0 = 0;
    tmr->count16_1 = 0;

    // Clear interrupt flag
    tmr->intfl = MXC_F_TMR_INTFL_TIMER0 | MXC_F_TMR_INTFL_TIMER1;

	if( !CPU_INTC_ActivateInterrupt(TMR2_0_IRQn, ISR_TIM32, NULL) )
		return DS_Fail;
	
    return DS_Success;

}

/*
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
}*/

DeviceStatus Max3263x_timer_32bit::UnInitialize()
{
    /*CPU_INTC_DeactivateInterrupt( TIM1_CC_IRQn );
    CPU_INTC_DeactivateInterrupt( TIM2_IRQn );
    callBackISR = NULL;
    TIM_DeInit( TIM1 );
    TIM_DeInit( TIM2 );*/

    return DS_Success;
}

#if defined(DEBUG_EMOTE_ADVTIME)
volatile UINT64 badSetComparesCount = 0;       //!< number of requests set in the past.
volatile UINT64 badSetComparesAvg = 0;         //!< average delay of requests set in the past.
volatile UINT64 badSetComparesMax = 0;         //!< observed worst-case.
#endif
// Set compare happens in two stages, the first stage involves setting the msb on tim2
// the second stage involves lsb on tim1
DeviceStatus Max3263x_timer_32bit::SetCompare(UINT64 compareValue)
{
	
	//stop timer
    TMR32_Stop(tmr);

    //setup timer configuration register
    //clear tmr2x16 (32bit mode), mode and polarity bits
    tmr->ctrl &= ~(MXC_F_TMR_CTRL_TMR2X16 | MXC_F_TMR_CTRL_MODE |
                   MXC_F_TMR_CTRL_POLARITY);

    //set mode and polarity
    tmr->ctrl |= ((TMR32_MODE_ONE_SHOT << MXC_F_TMR_CTRL_MODE_POS) |
                  (TMR_POLARITY_INIT_LOW << MXC_F_TMR_CTRL_POLARITY_POS));

    //setup timer Tick registers
    tmr->term_cnt32 = compareValue;
	

/*	uint16_t tar_upper;
	uint16_t now_upper;
	UINT64 now;
#if defined(DEBUG_EMOTE_ADVTIME)
	// if two timers need to fire at (or very close to) the same time, then the second timer will be passed to this function with a compare value of the current (at the time of setting it) time
	// So by the time we get to this portion of code NowTicks will be greater than compareValue. 
	// we then add on a TIME_CUSHION below and the second timer will fire at the current time plus the TIME_CUSHION
	volatile UINT64 NowTicks = g_Timer32Bit_Driver.Get64Counter();
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
	TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);*/
	return DS_Success;
}

UINT32 Max3263x_timer_32bit::GetMaxTicks()
{
	return (UINT32)0xFFFFFFFF;
}


void ISR_TIM32( void* Param )
{
	/*if(TIM_GetITStatus(TIM1, TIM_IT_CC3))
	{
		TIM_ITConfig(TIM1, TIM_IT_CC3, DISABLE);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);

		g_Timer32Bit_Driver.setCompareRunning = false; // Reset
		g_Timer32Bit_Driver.callBackISR(&g_Timer32Bit_Driver.callBackISR_Param);
	}*/
}
