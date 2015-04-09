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
//#include "../time/netmf_time.h"
#include <intc/stm32.h>


//AnanthAtSamraksh - defining the list of timers supported by the drivers. This has to match the IDs defined in "g_HardwareTimerIDs[]" in platform_selector.h
//#define 32BIT_ADVTIMER		1
//#define 16BIT_TIMER1		2
//#define 16BIT_TIMER2		3
extern const UINT8 ADVTIMER_32BIT;
extern const UINT8 TIMER1_16BIT;
extern const UINT8 TIMER2_16BIT;


extern Timer16Bit_Driver g_Timer16Bit_Driver;
extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;

static void ISR( void* Param );

////HAL_CALLBACK_FPN VirtualTimerHandlerFPN = NULL;


////void VirtualTimerHardwareHandler(void *args);

//#define HALTIMERDEBUG

//	Returns true of false depending on whether timer creation was successful.
//	Designed to provide an interface to pal level functions to directly timers users include HALTimer,
//  Batched mode adc etc.
//  Can not accept a timer value equal to system timer or an isr input of null.
//BOOL CPU_Timer_Initialize(UINT16 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
BOOL CPU_Timer_Initialize(UINT16 Timer, BOOL IsOneShot, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
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
	////TODO: AnanthAtSamraksh - change comparison based on hardware timerIDs supported by the drivers
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		//if(!g_Timer16Bit_Driver.Initialize(Timer, IsOneShot, ClkSource, Prescaler, ISR, NULL))
		if(!g_Timer16Bit_Driver.Initialize(Timer, IsOneShot, Prescaler, ISR, NULL))
			return FALSE;
	}
	else if(Timer == ADVTIMER_32BIT )
	{
		if(g_STM32F10x_AdvancedTimer.Initialize(Prescaler, ISR, NULL) != DS_Success)
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

		//TODO: AnanthAtSamraksh -- g_Timer16Bit_Driver.GetCounter(Timer) returns a UINT16, whereas SetCompare takes a UINT32
		//g_Timer16Bit_Driver.SetCompare(Timer, g_Timer16Bit_Driver.GetCounter(Timer) + CompareValue);
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		g_STM32F10x_AdvancedTimer.SetCompare(0, CompareValue, SET_COMPARE_TIMER);
	}

	return TRUE;
}

// Returns a 16 bit current counter value
// Accepts only legal timer values between 0 and c_MaxTimers
// Internally call the Timer_Driver getcounter function
UINT16 CPU_Timer_GetCounter(UINT16 Timer)
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
	UINT16 counterValue = 0;
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		counterValue = g_Timer16Bit_Driver.GetCounter(Timer);
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		counterValue = g_STM32F10x_AdvancedTimer.GetCounter();
	}

	return counterValue;
}


UINT16 CPU_Timer_SetCounter(UINT16 Timer, UINT32 Count)
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
		//GLOBAL_LOCK(irq);
		//AnanthAtSamraksh
		////currentTicksValue = g_STM32F10x_AdvancedTimer.Get64Counter();
		currentTicksValue = g_STM32F10x_AdvancedTimer.Get64Counter();
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
}

BOOL CPU_Timer_DidTimerOverflow(UINT8 Timer)
{
	BOOL retVal = FALSE;
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		retVal = g_Timer16Bit_Driver.DidTimerOverFlow(g_Timer16Bit_Driver.c_SystemTimer);
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		retVal = g_STM32F10x_AdvancedTimer.DidTimerOverflow();
	}
	return retVal;
}

void CPU_Timer_ClearTimerOverflow(UINT8 Timer)
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		g_Timer16Bit_Driver.ClearTimerOverFlow(g_Timer16Bit_Driver.c_SystemTimer);
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		g_STM32F10x_AdvancedTimer.ClearTimerOverflow();
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
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		return g_HardwareTimerFrequency[1];
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return g_HardwareTimerFrequency[0];
	}
	//return SystemTimerClock;
}

UINT32 CPU_TicksPerSecond(UINT16 Timer)
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		return SLOW_CLOCKS_PER_SECOND;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return g_HardwareTimerFrequency[0];
	}

	//return SystemTimerClock;
}

// OK, so the DriftParameters *should* correct this in the PAL layer
// But that begs the question of what this function is really for...
UINT64 CPU_TicksToTime( UINT64 Ticks, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		Ticks *= (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
		Ticks /= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
		return Ticks;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
		Ticks /= (g_HardwareTimerFrequency[0]/CLOCK_COMMON_FACTOR);
		return Ticks;
	}
}

// OK, so the DriftParameters *should* correct this in the PAL layer
// But that begs the question of what this function is really for...
UINT64 CPU_TicksToTime( UINT32 Ticks32, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Ticks;
		Ticks  = (UINT64)Ticks32 * (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
		Ticks /=                   (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
		return Ticks;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		// this reduces to multiplying by 1 if CLOCK_COMMON_FACTOR == 1000000
		if (CLOCK_COMMON_FACTOR != 1000000){
			Ticks32 *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);				 
		}
		if (g_HardwareTimerFrequency[0] == 8000000){
			Ticks32 >> 3;
		} else {
			Ticks32 /= (g_HardwareTimerFrequency[0]/CLOCK_COMMON_FACTOR);
		}
		return Ticks32;
	}
}

UINT64 CPU_MillisecondsToTicks( UINT64 mSec, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Ticks;
		Ticks = mSec * (SLOW_CLOCKS_PER_SECOND/ 1000);
		return Ticks;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ( mSec * (g_HardwareTimerFrequency[0] / 1000) );
	}
	//return mSec * (SystemTimerClock/1000);
}


UINT64 CPU_MillisecondsToTicks( UINT32 mSec, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Ticks;
		Ticks = mSec * (SLOW_CLOCKS_PER_SECOND/ 1000);
		return Ticks;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ( mSec * (g_HardwareTimerFrequency[0] / 1000) );
	}
	//return mSec * (SystemTimerClock/1000);
}


UINT64 CPU_TicksToMicroseconds( UINT64 ticks, UINT16 Timer )
{
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
		if (g_HardwareTimerFrequency[0] == 8000000){
			return ticks >> 3;
		} else {
			return ((ticks * CLOCK_COMMON_FACTOR) / g_HardwareTimerFrequency[0]);
		}		 
	}
}

UINT32 CPU_TicksToMicroseconds( UINT32 ticks, UINT16 Timer )
{
    UINT32 ret;
    switch(Timer)
    {
    case ADVTIMER_32BIT:
		if (g_HardwareTimerFrequency[0] == 8000000)
		{
        	ret = ticks >> 3;
		} else {
			ret = ((ticks * CLOCK_COMMON_FACTOR) / g_HardwareTimerFrequency[0]);
		}
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
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
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
   // return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
	return uSec * 48;
#endif
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		//return ( uSec * (g_HardwareTimerFrequency[0] / 1000000) );
		return ( uSec * (g_HardwareTimerFrequency[0] / CLOCK_COMMON_FACTOR) );
	}
	//return uSec * (SystemTimerClock/1000000);
}


UINT32 CPU_MicrosecondsToTicks( UINT32 uSec, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
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
		return ( uSec * (g_HardwareTimerFrequency[0] / CLOCK_COMMON_FACTOR) );
	}
	//return uSec * (SystemTimerClock/1000000);
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
