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
BOOL CPU_Timer_SetCompare(UINT16 Timer, UINT32 CompareValue)
{
	// Check to make sure timer is not system timer
	/*if(Timer == 0 || Timer == g_Timer16Bit_Driver.c_SystemTimer)
		return FALSE;*/

	////TODO: AnanthAtSamraksh - change comparison based on hardware timerIDs supported
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
				diff = g_Timer16Bit_Driver.c_MaxTimerValue;
			}
			//Else store the difference
			else
			{
				diff = (UINT32)(CompareValue - CntrValue);
			}

			//Store the compare to now + diff
			g_Timer16Bit_Driver.SetCompare ( g_Timer16Bit_Driver.c_SystemTimer, (UINT16)(g_Timer16Bit_Driver.GetCounter( g_Timer16Bit_Driver.c_SystemTimer ) + diff));

			//If meanwhile happens
			if(HAL_Time_CurrentTicks() > CompareValue)
			{
				fForceInterrupt = true;
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
		g_STM32F10x_AdvancedTimer.SetCompare(CPU_Timer_CurrentTicks(Timer), CompareValue, SET_COMPARE_TIMER);
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

// This function is tuned for 8MHz of the emote
// Will not work at other speeds at low uSec values ie ( < 30)
// This function has poor accuracy at less than 10 microsecs
// Coming to the first if condition takes 13.5 us so for values less than 10 this is the best we can do
void CPU_Timer_Sleep_MicroSeconds( UINT32 uSec, UINT16 Timer)
{
	/*
	//AnanthAtSamraksh - CPU_Sleep is commented out as of 7/8/2014. So, does not matter what sleep level is entered.
	//But sleep level 2 is "SLEEP_LEVEL__SLEEP".
	UINT32 sleepLevel = 2;
	UINT32 WakeupSystemEvents = 0;
	UINT32 Timeout_Milliseconds = uSec / 1000;
	Events_WaitForEvents( sleepLevel, WakeupSystemEvents, Timeout_Milliseconds );
	*/
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{

	}
	else if(Timer == ADVTIMER_32BIT)
	{
		if(uSec <= 10)
		{
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

/*
// Default hardware handler for the HAL Timer
// Function is designed to handle only timer 4, changing this to another timer will require adding a case in the switch case
void VirtualTimerHardwareHandler(void *args)
{

#ifdef TIMERDEBUG
	CPU_GPIO_SetPinState((GPIO_PIN) 1 , TRUE);
#endif

	// Calculate the current counter value to remove the error due to instructions executed in this handler
	UINT32 tick_correction = g_Timer16Bit_Driver.GetCounter(HALTIMER);
	int VirtualTimer = HALTIMER;
	UINT32 resolution = HALTIMER_RESOLUTION_USEC;
	//UINT32 resolution = gHalTimerManagerObject.get_timer_resolution();

	UINT32 ticks = CPU_MicrosecondsToTicks(resolution);


	switch(VirtualTimer)
	{
	case 4:
		if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
		{
		    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
		    // Call PAL layer handler
		    VirtualTimerHandlerFPN((void *) &tick_correction);

		    g_Timer16Bit_Driver.SetCompare(VirtualTimer, (UINT16)(tick_correction + ticks));
		    //Timer_Driver::SetCompare(HALTimer, (UINT16)(0xFFFF));

		}
		break;
	case 5:
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 );
			// Call PAL layer handler
			VirtualTimerHandlerFPN((void *) &tick_correction);

			//Timer_Driver::SetCompare(HALTimer, (UINT16)(tick_correction + ticks));
			//Timer_Driver::SetCompare(HALTimer, (UINT16)(0xFFFF));

		}
		break;
	default:
		// Something went terribly wrong loop here to help debug
		HARD_BREAKPOINT();
	}

#ifdef TIMERDEBUG
	CPU_GPIO_SetPinState((GPIO_PIN) 1 , FALSE);
#endif
}
*/


/*
void Timer_IRQ_Handler( UINT32 Timer )
{
	//Execute the ISR for the timer
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
   	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
      TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );
	  STM_EVAL_LEDToggle(1);
	  (g_Timer_Driver.m_descriptors[Timer].isr)(g_Timer_Driver.m_descriptors[Timer].arg);
      //Timer_IRQ_Handler( 2 );
	}	
}

void Timer_IRQ_Handler()
{
	STM_EVAL_LEDInit((Led_TypeDef)3);
	STM_EVAL_LEDToggle((Led_TypeDef)3);

}
*/

#if 0
//AnanthAtSamraksh -- moved below functions from netmf_advancedtimer_functions.cpp

/*
A "time" tick in MF parlance is a 100 ns tick used internally. A "SystemTimerClock" is defined as an 8 MHz tick on TIM1 and TIM2.
The prescaler on TIM1 is changed along with clock to enforce this. Not so on other timers, they must be checked.

The difference between the 8 MHz clock we use and the 10 MHz MF expects is accounted for in the DriftParameters (terrible naming, thanks MS).

--NPS
*/

void CPU_CPWAIT()
{

}
#endif



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
		////return Ticks;
		//AnanthAtSamraksh -- took this from CPU_SystemClocksToMicroseconds
		Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
		//Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
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
		////return Ticks32;
		//AnanthAtSamraksh -- took this from CPU_SystemClocksToMicroseconds
		Ticks32 *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
		//Ticks32 /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
		Ticks32 /= (g_HardwareTimerFrequency[0]/CLOCK_COMMON_FACTOR);
		return Ticks32;
	}
}

UINT64 CPU_MillisecondsToTicks( UINT64 mSec, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Ticks;
		Ticks = mSec * (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
		// Nived.Sivadas - rewriting to avoid possibility of floating point arithmetic in intemediate stages
		//Ticks /= (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);
		Ticks = (Ticks * SLOW_CLOCKS_MILLISECOND_GCD)/ 1000;
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
		Ticks  = (UINT64)mSec * (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
		// Nived.Sivadas - rewriting to avoid possibility of floating point arithmetic in intemediate stages
		//Ticks /= (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);
		Ticks = (Ticks * SLOW_CLOCKS_MILLISECOND_GCD)/ 1000;
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
		return (ticks * (ONE_MHZ / SLOW_CLOCKS_PER_SECOND));
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ((ticks * CLOCK_COMMON_FACTOR) / g_HardwareTimerFrequency[0]);
	}
}

UINT32 CPU_TicksToMicroseconds( UINT32 ticks, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		return (ticks * (ONE_MHZ / SLOW_CLOCKS_PER_SECOND));
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ((ticks * CLOCK_COMMON_FACTOR) / g_HardwareTimerFrequency[0]);
	}
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


#if 0
//AnanthAtSamraksh -- moved below functions from netmf_advancedtimer.cpp
INT64 CPU_Timer_TicksToTime( UINT64 Ticks )
{
	return CPU_TicksToTime( Ticks );
}

// This function has been tested using the rollover test for advanced timers - level_0c
INT64 CPU_Timer_CurrentTime()
{
	// time and ticks are actually the same thing, so we just return UINT64 of ticks
	return CPU_TicksToTime(CPU_Time_CurrentTicks());
}

// This SetCompare function will tie in to the HALTimerManager framework in \pal\HALTimer\HALTimer.cpp
// On a successful compare, will callback the HALTimerCallback in \pal\HALTimer\HALTimer.cpp
//TODO: AnanthAtSamraksh -- this name matches with the one in netmf_timers.cpp
/*void CPU_Timer_SetCompare( UINT64 CompareValue )
{
	g_STM32F10x_AdvancedTimer.SetCompare(0, CompareValue, SET_COMPARE_TIMER);
}*/

// This SetCompare works within the HAL_COMPLETION \ HAL_CONTINUATION framework in \pal\AsyncProcCall
// On a successful compare HAL_COMPLETION::DequeueAndExec(); will be called
void CPU_Timer_SetCompare_Completion( UINT64 CompareValue )
{
	g_STM32F10x_AdvancedTimer.SetCompare(0, CompareValue, SET_COMPARE_COMPLETION);
}

void CPU_Timer_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
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
    return CPU_Time_CurrentTicks();
}
#endif



#if 0
// This function is tuned to work when the processor is running at 8 MHz
// When that changes this function no longer works efficiently
void CPU_Timer_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	UINT32 limit = (uSec / 5);
	for(volatile UINT32 i = 0; i < limit; i++);
}
#endif



#if 0
//TODO: AnanthAtSamraksh --- moved below functions from netmf_time_functions.cpp
BOOL HAL_Time_Initialize(UINT16 Timer)
{
	//return Time_Driver::Initialize();
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		g_Time_Driver.m_lastRead = 0;
		g_Time_Driver.m_nextCompare = (UINT64) g_Timer16Bit_Driver.c_MaxTimerValue;

		if (!g_Timer16Bit_Driver.Initialize (g_Timer16Bit_Driver.c_SystemTimer, TRUE, 0, 0, ISR, NULL))
		{
			return FALSE;
		}
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		if (g_STM32F10x_AdvancedTimer.Initialize (0, ISR, NULL) != DS_Success)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL HAL_Time_Uninitialize(UINT16 Timer)
{
	//return Time_Driver::Uninitialize();
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		if(!g_Timer16Bit_Driver.Uninitialize( g_Timer16Bit_Driver.c_SystemTimer ))
		{
			return FALSE;
		}
	}
	else if(Timer == ADVTIMER_32BIT)
	{

	}

	return TRUE;
}

UINT64 HAL_Time_CurrentTicks(UINT16 Timer)
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT16 value;
		UINT64 m_lastRead = 0;
		GLOBAL_LOCK(irq);
		value = g_Timer16Bit_Driver.GetCounter(g_Timer16Bit_Driver.c_SystemTimer);

		// Nived.Sivadas
		// Workaround for the unusual didtimeoverflow bug, added another check
		UINT16 lastSixteenBits = m_lastRead & 0x0000FFFFull;
		m_lastRead &= (0xFFFFFFFFFFFF0000ull);

		if(g_Timer16Bit_Driver.DidTimeOverFlow( g_Timer16Bit_Driver.c_SystemTimer ) || (value < lastSixteenBits))
		{
			g_Timer16Bit_Driver.ClearTimeOverFlow( g_Timer16Bit_Driver.c_SystemTimer );
			m_lastRead += (0x1ull << 16);
		}

		//Or else the value gets added simply
		m_lastRead += value;

		ENABLE_INTERRUPTS();

		return m_lastRead;
	}
	//TODO: AnanthAtSamraksh -- verify below, especially "value"
	else if(Timer == ADVTIMER_32BIT)
	{
		UINT32 value;
		UINT64 m_lastRead = 0;
		GLOBAL_LOCK(irq);
		value = g_STM32F10x_AdvancedTimer.Get64Counter();

		UINT32 last32Bits = m_lastRead & 0x00000000FFFFFFFFull;
		m_lastRead &= (0xFFFFFFFF00000000ull);

		if(g_STM32F10x_AdvancedTimer.DidTimerOverflow( ) || (value < last32Bits))
		{
			g_STM32F10x_AdvancedTimer.ClearTimerOverflow( );
			m_lastRead += (0x1ull << 32);
		}

		//Or else the value gets added simply
		m_lastRead += value;

		ENABLE_INTERRUPTS();

		return m_lastRead;
	}
	//return Time_Driver::CounterValue();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
	//return Time_Driver::TicksToTime( Ticks );
	return CPU_TicksToTime( Ticks );
}

INT64 HAL_Time_CurrentTime(UINT16 Timer)
{
	//return Time_Driver::CurrentTime();
	return CPU_TicksToTime( HAL_Time_CurrentTicks(Timer) );
}

void HAL_Time_SetCompare( UINT64 CompareValue, UINT16 Timer )
{
	////Time_Driver::SetCompareValue( CompareValue );
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		////g_Time_Driver.SetCompareValue( CompareValue );
		GLOBAL_LOCK(irq);
		g_Time_Driver.m_nextCompare = CompareValue;
		bool fForceInterrupt = false;
		UINT64 CntrValue = HAL_Time_CurrentTicks(Timer);

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
				diff = g_Timer16Bit_Driver.c_MaxTimerValue;
			}
			//Else store the difference
			else
			{
				diff = (UINT32)(CompareValue - CntrValue);
			}

			//Store the compare to now + diff
			g_Timer16Bit_Driver.SetCompare ( g_Timer16Bit_Driver.c_SystemTimer, (UINT16)(g_Timer16Bit_Driver.GetCounter( g_Timer16Bit_Driver.c_SystemTimer ) + diff));

			//If meanwhile happens
			if(HAL_Time_CurrentTicks(Timer) > CompareValue)
			{
				fForceInterrupt = true;
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
		GLOBAL_LOCK(irq);
		g_Time_Driver.m_nextCompare = CompareValue;
		bool fForceInterrupt = false;
		UINT64 CntrValue = HAL_Time_CurrentTicks(Timer);

		if(CompareValue <= CntrValue)
		{
			fForceInterrupt = true;
		}
		else
		{
			//AnanthAtSamraksh -- commented out diff, as time keeping is done in the time driver
			/*UINT32 diff;
			//If compare is greater than total counter value, this is the best we can do
			if((CompareValue - CntrValue) > 0xFFFF)
			{
				diff = 0xFFFF;
			}
			//Else store the difference
			else
			{
				diff = (UINT32)(CompareValue - CntrValue);
			}

			//Store the compare to now + diff
			g_STM32F10x_AdvancedTimer.SetCompare(CPU_Timer_CurrentTicks(Timer), diff, SET_COMPARE_TIMER);*/
			g_STM32F10x_AdvancedTimer.SetCompare(CPU_Timer_CurrentTicks(Timer), CompareValue, SET_COMPARE_TIMER);
			////g_STM32F10x_AdvancedTimer.SetCompare ( g_STM32F10x_AdvancedTimer.c_SystemTimer, (UINT16)(Timer16Bit_Driver::GetCounter( Timer16Bit_Driver :: c_SystemTimer ) + diff));
			//If meanwhile happens
			if(HAL_Time_CurrentTicks(Timer) > CompareValue)
			{
				fForceInterrupt = true;
			}
		}

		if(fForceInterrupt)
		{
			// Force interrupt to process this.
			////Timer16Bit_Driver::ForceInterrupt( Timer16Bit_Driver::c_SystemTimer );
			g_STM32F10x_AdvancedTimer.TriggerSoftwareInterrupt();
		}
	}
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}

UINT64 Time_CurrentTicks(UINT16 Timer)
{
    return HAL_Time_CurrentTicks(Timer);
}

void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    //Time_Driver::Sleep_uSec( uSec );
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	//Time_Driver::Sleep_uSec_Loop( uSec );
}
#endif
