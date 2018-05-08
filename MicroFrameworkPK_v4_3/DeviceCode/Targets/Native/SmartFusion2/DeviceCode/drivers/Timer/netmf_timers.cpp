/*
 * Samraksh Timer Driver
 * Initial Create - Kartik Natarajan
 * 04/20/2011
 * Modification for Virtual timer porting - Mukundan Sridharan; Ananth Muralidharan
 * Date: 06/13/2014
 * Notes -
 * 1) Driver uses NVIC directly, rewrite parts with INTC
 * 2) The delay between initializing timer and enabling interrupts
 */

#include <tinyhal.h>
#include "mss_timer.h"
#include <CMSIS/system_m2sxxx.h>
#include <CMSIS/m2sxxx.h>

/*#include "../Timer/Timer16Bit/netmf_timers16Bit.h"
#include "../Timer/advancedtimer/netmf_advancedtimer.h"
#include "../Timer/netmf_rtc/netmf_rtc.h"*/
//#include <intc/stm32.h>

//extern Timer16Bit_Driver g_Timer16Bit_Driver;
//extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
//extern STM32F10x_RTC g_STM32F10x_RTC;

UINT64 m_systemTime = 0;
const UINT64 TIME_CUSHION = 40;  // 15 us

void Timer1_IRQHandler(void* Param)
{
    m_systemTime += (0x1ull <<32);
    // Clear interrupt 
    MSS_TIM1_clear_irq();
}

void Timer2_IRQHandler(void* Param)
{
	//UINT64 getCount = CPU_Timer_CurrentTicks(0);
	//hal_printf("%llu\r\n",getCount);
    //CPU_GPIO_SetPinState(0, TRUE);
	//CPU_GPIO_SetPinState(0, FALSE);
    // Clear interrupt 
    MSS_TIM2_clear_irq();
}

BOOL CPU_Timer_Initialize(UINT16 Timer, BOOL IsOneShot, UINT32 Prescaler, HAL_CALLBACK_FPN ISR)
{
	// TIM1 will keep system time
	// TIM2 will be for VT
	uint32_t tim_load_value;
	SystemCoreClockUpdate();
	MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);
	tim_load_value = g_FrequencyPCLK0;
	MSS_TIM1_load_immediate(0xFFFFFFFF);
    MSS_TIM2_load_immediate(tim_load_value);
	if( !CPU_INTC_ActivateInterrupt(Timer1_IRQn, Timer1_IRQHandler, NULL) )
		return DS_Fail;

	//if( !CPU_INTC_ActivateInterrupt(Timer2_IRQn, ISR, NULL) )
	if( !CPU_INTC_ActivateInterrupt(Timer2_IRQn, Timer2_IRQHandler, NULL) )
		return DS_Fail;
	//CPU_GPIO_EnableOutputPin( 0, FALSE );
	MSS_TIM1_start();
	MSS_TIM2_start();
    MSS_TIM1_enable_irq();
    MSS_TIM2_enable_irq();
	return TRUE;

}


BOOL CPU_Timer_UnInitialize(UINT16 Timer)
{
//		if(g_STM32F10x_AdvancedTimer.UnInitialize() != DS_Success)
//			return FALSE;
	return TRUE;
}


// Returns true if set compare is successfull else returns false.
// Accepts only Timer values not equal to 0 and system timer.
// Calls the timer driver set compare function.
BOOL CPU_Timer_SetCompare(UINT16 Timer, UINT64 CompareValue)
{
	uint64_t currentValue = CompareValue - CPU_Timer_CurrentTicks(0);
	
	if (currentValue > TIME_CUSHION){
		MSS_TIM2_load_immediate(currentValue&0xFFFFFFFF);
		return TRUE;
	} else {
		return FALSE;
	}
}

// Returns current counter value
// Accepts only legal timer values between 0 and c_MaxTimers
// Internally call the Timer_Driver getcounter function
//TODO: not used?
UINT32 CPU_Timer_GetCounter(UINT16 Timer)
{
UINT32 counterValue = (UINT32)(0xFFFFFFFF - MSS_TIM1_get_current_value());

	return counterValue;
}

//TODO: not used?
UINT32 CPU_Timer_SetCounter(UINT16 Timer, UINT32 Count)
{
	UINT16 counterValue = 0;

	//	counterValue = g_STM32F10x_AdvancedTimer.SetCounter(Count);

	return counterValue;
}

//TODO: AnanthAtSamraksh - to check if this is the right place
UINT64 CPU_Timer_CurrentTicks(UINT16 Timer)
{
	uint32_t currentValue = (0xFFFFFFFF - MSS_TIM1_get_current_value());
	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentValue;

	return (UINT64)m_systemTime;
}

// This function is tuned for 8MHz of the emote in Release mode
void CPU_Timer_Sleep_MicroSeconds( UINT32 uSec, UINT16 Timer)
{

		if(uSec <= 1)
		{
			return;
		}

		GLOBAL_LOCK(irq);

		if(uSec <= 5)
		{
			UINT32 limit = (uSec << 1);
			for(volatile UINT32 i = 0; i < limit; i++);
			return;
		}

		// Adjustment for 5us of processing overhead
		uSec = uSec - 5;

		UINT32 currentCounterVal =  CPU_Timer_GetCounter(DEFAULT_TIMER);
		UINT32 ticks = CPU_MicrosecondsToTicks(uSec, Timer);
		while( CPU_Timer_GetCounter(DEFAULT_TIMER) - currentCounterVal <= ticks);

}

UINT32 CPU_Timer_GetMaxTicks(UINT8 Timer)
{
	UINT32 maxTicks = 0;

	//	maxTicks = g_STM32F10x_AdvancedTimer.GetMaxTicks();

	return maxTicks;
}

void CPU_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
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

UINT32 CPU_SystemClock(UINT16 Timer)
{
	UINT32 retVal = 0;
	UINT8 i;
	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			return g_HardwareTimerFrequency[i];
		}
	}

	ASSERT(0);
	retVal = SYSTEM_CLOCK_HZ;

	return retVal;
}

UINT32 CPU_TicksPerSecond(UINT16 Timer)
{
	UINT32 retVal = 0;
	UINT8 i;
	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			return g_HardwareTimerFrequency[i];
		}
	}
	
	ASSERT(0);
	retVal = SLOW_CLOCKS_PER_SECOND;

	return retVal;
}

// OK, so the DriftParameters *should* correct this in the PAL layer
// But that begs the question of what this function is really for...
UINT64 CPU_TicksToTime( UINT64 Ticks, UINT16 Timer )
{
	UINT8 i;
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}


		Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
		Ticks /= (timerFrequency/CLOCK_COMMON_FACTOR);

	return Ticks;
}

// OK, so the DriftParameters *should* correct this in the PAL layer
// But that begs the question of what this function is really for...
//TODO: not used?
UINT64 CPU_TicksToTime( UINT32 Ticks32, UINT16 Timer )
{
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}


		// this reduces to multiplying by 1 if CLOCK_COMMON_FACTOR == 1000000
		if (CLOCK_COMMON_FACTOR != 1000000){
			Ticks32 *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);				 
		}
		if (timerFrequency == 8000000){
			Ticks32 >> 3;
		} else {
			Ticks32 /= (timerFrequency/CLOCK_COMMON_FACTOR);
		}
		return Ticks32;

}

UINT64 CPU_MillisecondsToTicks( UINT64 mSec, UINT16 Timer )
{
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}


		return ( mSec * (timerFrequency / 1000) );

}


UINT64 CPU_MillisecondsToTicks( UINT32 mSec, UINT16 Timer )
{
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}


		return ( mSec * (timerFrequency / 1000) );

}


UINT64 CPU_TicksToMicroseconds( UINT64 ticks, UINT16 Timer )
{
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}


		if (timerFrequency == 8000000){
			return ticks >> 3;
		} else {
			return ((ticks * CLOCK_COMMON_FACTOR) / timerFrequency);
		}		 

}

UINT32 CPU_TicksToMicroseconds( UINT32 ticks, UINT16 Timer )
{
	UINT32 ret;
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}

		if (timerFrequency == 8000000)
		{
			ret = ticks >> 3;
		} else {
			ret = ((ticks * CLOCK_COMMON_FACTOR) / timerFrequency);
		}

	return ret;
}

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec, UINT16 Timer )
{
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}

		return ( uSec * (timerFrequency / CLOCK_COMMON_FACTOR) );

}


UINT32 CPU_MicrosecondsToTicks( UINT32 uSec, UINT16 Timer )
{
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}

		return ( uSec * (timerFrequency / CLOCK_COMMON_FACTOR) );

}



UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec ){
	//return uSec * (SystemTimerClock/1000000);
	uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
	uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
	return uSec;
}

int CPU_MicrosecondsToSystemClocks( int uSec ) {
	//return uSec * (SystemTimerClock/1000000);
	uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
	uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
	return uSec;
}

int CPU_SystemClocksToMicroseconds( int Ticks ) {
	//return Ticks * 1000000 / SystemTimerClock ;
	Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
	Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
	return Ticks;
}

// timeToAdd is in 100-nanosecond (ns) increments. This is a Microsoft thing.
void CPU_AddClockTime(UINT16 Timer, UINT64 timeToAdd)
{
/*	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT64 ticksToAdd = 0;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}

		if(timeToAdd == 0)
		{
			return;
		}

		// ticksToAdd = timeToAdd * (0.0000001 s) * 8000000 ticks/s (@ 8MHz)
		ticksToAdd = timeToAdd * 0.0000001 * timerFrequency;
		g_STM32F10x_AdvancedTimer.AddTicks(ticksToAdd);*/

}
