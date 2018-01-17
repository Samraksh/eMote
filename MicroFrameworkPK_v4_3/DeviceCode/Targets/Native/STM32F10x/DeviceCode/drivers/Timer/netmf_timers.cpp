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
#include <pwr/netmf_pwr.h>
#include "../Timer/Timer16Bit/netmf_timers16Bit.h"
#include "../Timer/advancedtimer/netmf_advancedtimer.h"
#include "../Timer/netmf_rtc/netmf_rtc.h"
#include <intc/stm32.h>


extern const UINT8 ADVTIMER_32BIT;
extern const UINT8 TIMER1_16BIT;
extern const UINT8 TIMER2_16BIT;


extern Timer16Bit_Driver g_Timer16Bit_Driver;
extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
extern STM32F10x_RTC g_STM32F10x_RTC;


//#define HALTIMERDEBUG

//	Returns true of false depending on whether timer creation was successful.
//	Designed to provide an interface to pal level functions to directly timers users include HALTimer,
//  Batched mode adc etc.
//  Can not accept a timer value equal to system timer or an isr input of null.
//BOOL CPU_Timer_Initialize(UINT16 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
BOOL CPU_Timer_Initialize(UINT16 Timer, BOOL IsOneShot, UINT32 Prescaler, HAL_CALLBACK_FPN ISR)
{
    // assumptions about clock settings used in optimizations. TODO: use static assert.
    ASSERT(CLOCK_COMMON_FACTOR == 1000000);
    ASSERT(g_HardwareTimerFrequency[0] == 8000000);
    ASSERT(ONE_MHZ == 1000000);
    ASSERT(SLOW_CLOCKS_PER_SECOND == 48000000);

	// Make sure timer input is not 0 in case macro is not defined or the timer is not trying to
	// re initialize the system  timer

	// Dont allow initializing of timers with NULL as the callback function
	if(ISR == NULL)
		return FALSE;

	////VirtualTimerHandlerFPN = ISR;

#ifdef TIMERDEBUG
		CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, FALSE);
#endif

	// Call timer driver initialize
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		//if(!g_Timer16Bit_Driver.Initialize(Timer, IsOneShot, ClkSource, Prescaler, ISR, NULL))
		if(!g_Timer16Bit_Driver.Initialize(Timer, IsOneShot, Prescaler, ISR, NULL))
			return FALSE;
	}
	else if(Timer == ADVTIMER_32BIT )
	{
		if(g_STM32F10x_AdvancedTimer.Initialize(Prescaler, ISR, ADVTIMER_32BIT) != DS_Success)
			return FALSE;
	}
	else if(Timer == RTC_32BIT )
	{
		if(g_STM32F10x_RTC.Initialize(Prescaler, ISR, RTC_32BIT) != DS_Success)
			return FALSE;
	}

	return TRUE;

}


BOOL CPU_Timer_UnInitialize(UINT16 Timer)
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		if(!g_Timer16Bit_Driver.Uninitialize(Timer))
			return FALSE;
	}
	else if(Timer == ADVTIMER_32BIT )
	{
		if(g_STM32F10x_AdvancedTimer.UnInitialize() != DS_Success)
			return FALSE;
	}
	else if(Timer == RTC_32BIT )
	{
		if(g_STM32F10x_RTC.UnInitialize() != DS_Success)
			return FALSE;
	}

	return TRUE;
}


// Returns true if set compare is successfull else returns false.
// Accepts only Timer values not equal to 0 and system timer.
// Calls the timer driver set compare function.
BOOL CPU_Timer_SetCompare(UINT16 Timer, UINT64 CompareValue)
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		//AnanthAtSamraksh -- added below
		GLOBAL_LOCK(irq);
		//g_Time_Driver.m_nextCompare = CompareValue;
		bool fForceInterrupt = false;
		UINT64 CntrValue = HAL_Time_CurrentTicks();

		if(CompareValue <= CntrValue)
		{
			fForceInterrupt = true;
		}
		else
		{
			UINT32 diff;
			//If compare is greater than total counter value, this is the best we can do
			if((CompareValue - CntrValue) > g_Timer16Bit_Driver.c_MaxTimerValue)
			{
				g_Timer16Bit_Driver.SetCompare ( g_Timer16Bit_Driver.c_SystemTimer, g_Timer16Bit_Driver.c_MaxTimerValue);
				diff = g_Timer16Bit_Driver.c_MaxTimerValue;
			}
			//Else store the difference
			else
			{
				g_Timer16Bit_Driver.SetCompare ( g_Timer16Bit_Driver.c_SystemTimer, CompareValue);
			}
		}

		if(fForceInterrupt)
		{
			// Force interrupt to process this.
			g_Timer16Bit_Driver.ForceInterrupt( g_Timer16Bit_Driver.c_SystemTimer );
		}

	}
	else if(Timer == ADVTIMER_32BIT)
	{
		g_STM32F10x_AdvancedTimer.SetCompare(CompareValue);
	}
	else if(Timer == RTC_32BIT)
	{
		g_STM32F10x_RTC.SetCompare(CompareValue);
	}

	return TRUE;
}

// Returns current counter value
// Accepts only legal timer values between 0 and c_MaxTimers
// Internally call the Timer_Driver getcounter function
//TODO: not used?
UINT32 CPU_Timer_GetCounter(UINT16 Timer)
{
	// Check to make sure timer is a legal timer
	// Typically should assert here, but returning 0
	////if(Timer < 0 || Timer > g_Timer16Bit_Driver.c_MaxTimers)
	//TODO: AnanthAtSamraksh - not checking if timer is greater than total timers configured.
	//This can be done in the if...else loop
	////if(Timer < 0 || Timer > NUM_VIRTUALTIMER_TIMERS)
	/*if(Timer < 0)
		return 0;*/

	////TODO: AnanthAtSamraksh - change comparison based on hardware timerIDs supported
	UINT32 counterValue = 0;
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		counterValue = (UINT32)g_Timer16Bit_Driver.GetCounter(Timer);
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		counterValue = g_STM32F10x_AdvancedTimer.GetCounter();
	}
	else if(Timer == RTC_32BIT)
	{
		counterValue = g_STM32F10x_RTC.GetCounter();
	}

	return counterValue;
}

//TODO: not used?
UINT32 CPU_Timer_SetCounter(UINT16 Timer, UINT32 Count)
{
	UINT16 counterValue = 0;
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		//counterValue = g_Timer16Bit_Driver.SetCounter(Timer, Count);
		g_Timer16Bit_Driver.SetCounter(Timer, Count);
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		counterValue = g_STM32F10x_AdvancedTimer.SetCounter(Count);
	}
	else if(Timer == RTC_32BIT)
	{
		counterValue = g_STM32F10x_RTC.SetCounter(Count);
	}

	return counterValue;
}

//TODO: AnanthAtSamraksh - to check if this is the right place
UINT64 CPU_Timer_CurrentTicks(UINT16 Timer)
{
	//TODO: AnanthAtSamraksh - not checking if timer is greater than total timers configured. T
	//This can be done in the if...else loop
	////if(Timer < 0 || Timer > NUM_VIRTUALTIMER_TIMERS)
	/*if(Timer < 0)
		return 0;*/

	UINT64 currentTicksValue = 0;
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		//AnanthAtSamraksh -- added below from
		UINT16 value;
		UINT32 m_lastRead = 0;
		GLOBAL_LOCK(irq);
		value = g_Timer16Bit_Driver.GetCounter(g_Timer16Bit_Driver.c_SystemTimer);

		// Nived.Sivadas
		// Workaround for the unusual didtimeroverflow bug, added another check
		UINT16 lastSixteenBits = m_lastRead & 0x0000FFFFull;
		////m_lastRead &= (0xFFFFFFFFFFFF0000ull);
		m_lastRead &= (0xFFFF0000ull);

		if(g_Timer16Bit_Driver.DidTimerOverFlow( g_Timer16Bit_Driver.c_SystemTimer ) || (value < lastSixteenBits))
		{
			g_Timer16Bit_Driver.ClearTimerOverFlow( g_Timer16Bit_Driver.c_SystemTimer );
			m_lastRead += (0x1ull << 16);
		}

		//Or else the value gets added simply
		m_lastRead += value;

		return m_lastRead;

		//TODO: AnanthAtSamraksh - what is the equivalent for 16 bit timer
		//currentTicksValue = g_Timer16Bit_Driver.GetCounter((UINT32)Timer);
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		currentTicksValue = g_STM32F10x_AdvancedTimer.Get64Counter();
	}
	else if(Timer == RTC_32BIT)
	{
		currentTicksValue = g_STM32F10x_RTC.Get64Counter();
	}

	return currentTicksValue;
}

// This function is tuned for 8MHz of the emote in Release mode
void CPU_Timer_Sleep_MicroSeconds( UINT32 uSec, UINT16 Timer)
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{

	}
	else if(Timer == ADVTIMER_32BIT)
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

		UINT32 currentCounterVal = g_STM32F10x_AdvancedTimer.GetCounter();
		UINT32 ticks = CPU_MicrosecondsToTicks(uSec, Timer);
		while(g_STM32F10x_AdvancedTimer.GetCounter() - currentCounterVal <= ticks);
	}
	else
	{
		ASSERT(0);
	}
}

UINT32 CPU_Timer_GetMaxTicks(UINT8 Timer)
{
	UINT32 maxTicks = 0;
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		maxTicks = g_Timer16Bit_Driver.GetMaxTicks();
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		maxTicks = g_STM32F10x_AdvancedTimer.GetMaxTicks();
	}
	else if(Timer == RTC_32BIT)
	{
		maxTicks = g_STM32F10x_RTC.GetMaxTicks();
	}
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

/**
 * Microsoft: Retrieves the speed of the system clock, in Hertz (Hz).
 */
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

/**
 * Microsoft: Retrieves the number of real-time clock ticks per second.
 */
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
// returns common language runtime (CLR) base time, in 100-nanosecond (ns) increments
UINT64 CPU_TicksToTime( UINT64 Ticks, UINT16 Timer )
{
	UINT8 i;
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}

	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Time;
		Time  = Ticks * (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
		Time /=                   (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
		return Time;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		// this reduces to multiplying by 1 if CLOCK_COMMON_FACTOR == 1000000
		if (CLOCK_COMMON_FACTOR != 1000000){
			Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);				 
		}
		if (timerFrequency == 8000000){
			Ticks >> 3;
		} else {
		Ticks /= (timerFrequency/CLOCK_COMMON_FACTOR);
	}
		return Ticks * 10;
	}
	else if(Timer == RTC_32BIT)
	{
		// this reduces to multiplying by 1 if CLOCK_COMMON_FACTOR == 1000000
		if (CLOCK_COMMON_FACTOR != 1000000){
			Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);				 
	}
		Ticks = Ticks * (CLOCK_COMMON_FACTOR / timerFrequency);
		
		return Ticks * 10;
	}
	else
	{
		ASSERT(0);
		return 0;
	}
}

// OK, so the DriftParameters *should* correct this in the PAL layer
// But that begs the question of what this function is really for...
// returns common language runtime (CLR) base time, in 100-nanosecond (ns) increments
UINT64 CPU_TicksToTime( UINT32 Ticks32, UINT16 Timer )
{
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}

	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Time;
		Time  = (UINT64)Ticks32 * (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
		Time /=                   (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
		return Time;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		// this reduces to multiplying by 1 if CLOCK_COMMON_FACTOR == 1000000
		if (CLOCK_COMMON_FACTOR != 1000000){
			Ticks32 *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);				 
		}
		if (timerFrequency == 8000000){
			Ticks32 >> 3;
		} else {
			Ticks32 /= (timerFrequency/CLOCK_COMMON_FACTOR);
		}
		return Ticks32 * 10;
	}
	else if(Timer == RTC_32BIT)
	{
		// this reduces to multiplying by 1 if CLOCK_COMMON_FACTOR == 1000000
		if (CLOCK_COMMON_FACTOR != 1000000){
			Ticks32 *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);				 
		}
		Ticks32 = Ticks32 * (CLOCK_COMMON_FACTOR / timerFrequency);
		
		return Ticks32 * 10;
	}
	else
	{
		ASSERT(0);
		return 0;
	}
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

	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Ticks;
		Ticks = mSec * (SLOW_CLOCKS_PER_SECOND/ 1000);
		return Ticks;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ( mSec * (timerFrequency / 1000) );
	}
	else if(Timer == RTC_32BIT)
	{
		return ( mSec * (timerFrequency / 1000) );
	}
	else
	{
		ASSERT(0);
		return 0;
	}
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

	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Ticks;
		Ticks = mSec * (SLOW_CLOCKS_PER_SECOND/ 1000);
		return Ticks;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ( mSec * (timerFrequency / 1000) );
	}
	else if(Timer == RTC_32BIT)
	{
		return ( mSec * (timerFrequency / 1000) );
	}
	else
	{
		ASSERT(0);
		return 0;
	}

}

//TODO: profile minimum value
//TODO: not used?
UINT64 CPU_TicksToMicroseconds( UINT64 ticks, UINT16 Timer )
{
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}

	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		if (SLOW_CLOCKS_PER_SECOND == 48000000){
			return (ticks >> 4) / 3; //(ticks * (ONE_MHZ / SLOW_CLOCKS_PER_SECOND));
		} else {
			return (ticks * (ONE_MHZ / SLOW_CLOCKS_PER_SECOND));
		}
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		if (timerFrequency == 8000000){
			return ticks >> 3;
		} else {
			return ((ticks * CLOCK_COMMON_FACTOR) / timerFrequency);
		}		 
	}
	else if(Timer == RTC_32BIT)
	{
		return ((ticks * CLOCK_COMMON_FACTOR) / timerFrequency);
	}
	else
	{
		ASSERT(0);
		return 0;
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

	switch(Timer)
	{
	case ADVTIMER_32BIT:
		if (timerFrequency == 8000000)
		{
			ret = ticks >> 3;
		} else {
			ret = ((ticks * CLOCK_COMMON_FACTOR) / timerFrequency);
		}
		break;
	case RTC_32BIT:
		ret = ((ticks * CLOCK_COMMON_FACTOR) / timerFrequency);
		break;
	case TIMER1_16BIT:
		// fall through to TIMER2_16BIT
	case TIMER2_16BIT:
		ret = ticks / 48;  // ticks * ONE_MHZ / SLOW_CLOCKS_PER_SECOND; //TODO: ensure compiler emits division (single-cycle on STM32F103xG)
		break;
	default:
		ASSERT(FALSE);
		ret = 0;
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
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
#ifdef DEBUG_PRINT
	UINT64 value;
	debug_printf("CPU_MicrosecondsToTicks, Ticks %lld\n", uSec);
	value = uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
	debug_printf("Return Value is, %lld\n", value);
#endif
	return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicrosecondsToTicks(UINT64/#else), Ticks %u\n", uSec);
#endif
	return uSec * 48;
#endif
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ( uSec * (timerFrequency / CLOCK_COMMON_FACTOR) );
	}
	else if(Timer == RTC_32BIT)
	{
		return (UINT32)( (UINT64)uSec * (UINT64)timerFrequency / (UINT64)CLOCK_COMMON_FACTOR );
	}
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
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
#ifdef DEBUG_PRINT
	UINT32 value32;
	debug_printf("In CPU_MicrosecondsToTicks(UINT32), Ticks %u\n", uSec);
	value32 = uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
	debug_printf("Return Value is, %u\n", value32);
#endif
	return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicrosecondsToTicks(UINT32/#else), Ticks %u\n", uSec);
#endif
	//return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
	return uSec * 48;
#endif
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		//return ( uSec * (g_HardwareTimerFrequency[0] / 1000000) );
		return ( uSec * (timerFrequency / CLOCK_COMMON_FACTOR) );
	}
	else if(Timer == RTC_32BIT)
	{
		//UINT64 answer = (UINT64)((UINT64)uSec * (UINT64)timerFrequency);
		//UINT32 answer1b = (UINT32)(answer / (UINT64)CLOCK_COMMON_FACTOR);
		return (UINT32)( (UINT64)uSec * (UINT64)timerFrequency / (UINT64)CLOCK_COMMON_FACTOR );
	}
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
	UINT32 timerFrequency = SYSTEM_CLOCK_HZ;
	UINT64 ticksToAdd = 0;
	UINT8 i;

	for (i=0; i<g_CountOfHardwareTimers; i++){
		if (Timer == g_HardwareTimerIDs[i]){
			timerFrequency = g_HardwareTimerFrequency[i];
		}
	}
	
	if(Timer == ADVTIMER_32BIT)
	{
		if(timeToAdd == 0)
		{
			return;
		}

		// ticksToAdd = timeToAdd * (0.0000001 s) * 8000000 ticks/s (@ 8MHz)
		ticksToAdd = timeToAdd * 0.0000001 * timerFrequency;
		g_STM32F10x_AdvancedTimer.AddTicks(ticksToAdd);
	}
	else
	{
		ASSERT(0);
	}
}
