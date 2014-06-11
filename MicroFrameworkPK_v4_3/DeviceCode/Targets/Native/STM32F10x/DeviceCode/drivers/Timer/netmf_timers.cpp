/*
 * Samraksh Timer Driver
 * Initial Create - Kartik Natarajan
 * 04/20/2011
 * Notes -
 * 1) Driver uses NVIC directly, rewrite parts with INTC
 * 2) The delay between initializing timer and enabling interrupts
 */

#include <tinyhal.h>
#include <pwr/netmf_pwr.h>
#include "Timer16Bit/netmf_timers16Bit.h"
#include "advancedtim/netmf_advancedtimer.h"
#include <intc/stm32.h>


extern Timer16Bit_Driver g_Timer16Bit_Driver;
extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;

HAL_CALLBACK_FPN VirtualTimerHandlerFPN = NULL;


void VirtualTimerHardwareHandler(void *args);

//#define HALTIMERDEBUG

//	Returns true of false depending on whether timer creation was successful.
//	Designed to provide an interface to pal level functions to directly timers users include HALTimer,
//  Batched mode adc etc.
//  Can not accept a timer value equal to system timer or an isr input of null.
BOOL CPU_TIMER_Initialize(UINT16 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
{
	// Make sure timer input is not 0 in case macro is not defined or the timer is not trying to
	// re initialize the system  timer
	if(Timer == 0)
		return FALSE;

	////TODO: AnanthAtSamraksh - is this right to place this here
	if(Timer == g_Timer16Bit_Driver.c_SystemTimer)
		return FALSE;

	// Dont allow initializing of timers with NULL as the callback function
	if(ISR == NULL)
		return FALSE;

	VirtualTimerHandlerFPN = ISR;

#ifdef VIRTUALTIMERDEBUG
		CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, FALSE);
#endif

	// Call timer driver initialize
	////TODO: AnanthAtSamraksh - change comparison based on hardware timerIDs supported by the drivers
	if(Timer == 3 || Timer == 4 || Timer == 5)
	{
		if(g_Timer16Bit_Driver.Initialize(Timer, FreeRunning, ClkSource, Prescaler, ISR, NULL) != DS_Success)
			return FALSE;
	}
	else if(Timer == 1 || Timer == 2 )
	{
		if(g_STM32F10x_AdvancedTimer.Initialize(Prescaler, ISR, NULL) != DS_Success)
			return FALSE;
	}

	return TRUE;

}


// Returns true if set compare is successfull else returns false.
// Accepts only Timer values not equal to 0 and system timer.
// Calls the timer driver set compare function.
BOOL CPU_TIMER_SetCompare(UINT16 Timer, UINT32 CompareValue)
{
	// Check to make sure timer is not system timer
	if(Timer == 0 || Timer == g_Timer16Bit_Driver.c_SystemTimer)
		return FALSE;

	////TODO: AnanthAtSamraksh - change comparison based on hardware timerIDs supported
	if(Timer == 3 || Timer == 4 || Timer == 5)
	{
		//TODO: AnanthAtSamraksh -- g_Timer16Bit_Driver.GetCounter(Timer) returns a UINT16, whereas SetCompare takes a UINT32
		g_Timer16Bit_Driver.SetCompare(Timer, g_Timer16Bit_Driver.GetCounter(Timer) + CompareValue);
	}
	else if(Timer == 1 || Timer == 2 )
	{
		////g_STM32F10x_AdvancedTimer.SetCompare(Timer, g_Timer16Bit_Driver.GetCounter(Timer) + Compare);
		////g_STM32F10x_AdvancedTimer.SetCompare(counterCorrection, Compare, scType);
		////g_STM32F10x_AdvancedTimer.SetCompare(CPU_Timer_CurrentTicks(Timer), Compare, SET_COMPARE_TIMER);
		g_STM32F10x_AdvancedTimer.SetCompare(CPU_Timer_CurrentTicks(Timer), CompareValue, SET_COMPARE_TIMER);
	}

	return TRUE;
}

// Returns a 16 bit current counter value
// Accepts only legal timer values between 0 and c_MaxTimers
// Internally call the Timer_Driver getcounter function
UINT16 CPU_TIMER_GetCounter(UINT16 Timer)
{
	// Check to make sure timer is a legal timer
	// Typically should assert here, but returning 0
	////if(Timer < 0 || Timer > g_Timer16Bit_Driver.c_MaxTimers)
	//TODO: AnanthAtSamraksh - not checking if timer is greater than total timers configured. T
	//This can be done in the if...else loop
	////if(Timer < 0 || Timer > NUM_VIRTUALTIMER_TIMERS)
	if(Timer < 0)
		return 0;

	////TODO: AnanthAtSamraksh - change comparison based on hardware timerIDs supported
	UINT16 counterValue = 0;
	if(Timer == 3 || Timer == 4 || Timer == 5)
	{
		counterValue = g_Timer16Bit_Driver.GetCounter(Timer);
	}
	else if(Timer == 1 || Timer == 2 )
	{
		counterValue = g_STM32F10x_AdvancedTimer.GetCounter();
	}

	return counterValue;
}

//TODO: AnanthAtSamraksh - to check if this is the right place
UINT64 CPU_Timer_CurrentTicks(UINT16 Timer)
{
	//TODO: AnanthAtSamraksh - not checking if timer is greater than total timers configured. T
	//This can be done in the if...else loop
	////if(Timer < 0 || Timer > NUM_VIRTUALTIMER_TIMERS)
	if(Timer < 0)
		return 0;

	UINT64 currentTicksValue = 0;
	if(Timer == 3 || Timer == 4 || Timer == 5)
	{
		//TODO: AnanthAtSamraksh - what is the equivalent for 16 bit timer
		//currentTicksValue = g_Timer16Bit_Driver.GetCounter(Timer);
	}
	else if(Timer == 1 || Timer == 2 )
	{
		currentTicksValue = g_STM32F10x_AdvancedTimer.Get64Counter();
	}

	return currentTicksValue;

}


// Default hardware handler for the HAL Timer
// Function is designed to handle only timer 4, changing this to another timer will require adding a case in the switch case
void VirtualTimerHardwareHandler(void *args)
{

#ifdef VIRTUALTIMERDEBUG
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
		while(1);
	}

#ifdef VIRTUALTIMERDEBUG
	CPU_GPIO_SetPinState((GPIO_PIN) 1 , FALSE);
#endif
}


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

UINT32 CPU_SystemClock()
{
	return SystemTimerClock;
}

UINT32 CPU_TicksPerSecond()
{
	return SystemTimerClock;
}

// OK, so the DriftParameters *should* correct this in the PAL layer
// But that begs the question of what this function is really for...
UINT64 CPU_TicksToTime( UINT64 Ticks ) {
	return Ticks;
}

// OK, so the DriftParameters *should* correct this in the PAL layer
// But that begs the question of what this function is really for...
UINT64 CPU_TicksToTime( UINT32 Ticks32 ) {
	return Ticks32;
}

UINT64 CPU_MillisecondsToTicks( UINT64 mSec ) {
	return mSec * (SystemTimerClock/1000);
}


UINT64 CPU_MillisecondsToTicks( UINT32 mSec ) {
	return mSec * (SystemTimerClock/1000);
}

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec ) {
	return uSec * (SystemTimerClock/1000000);
}


UINT32 CPU_MicrosecondsToTicks( UINT32 uSec ) {
	return uSec * (SystemTimerClock/1000000);
}

#if 0
UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec ){
	return uSec * (SystemTimerClock/1000000);
}

int CPU_MicosecondsToSystemClocks( int uSec ) {
	return uSec * (SystemTimerClock/1000000);
}

int CPU_SystemClocksToMicroseconds( int Ticks ) {
	return Ticks * 1000000 / SystemTimerClock ;
}



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
// On a successful compare, a tasklet is launched that will callback the HALTimerCallback in \pal\HALTimer\HALTimer.cpp
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

// This function is tuned for 8MHz of the emote
// Will not work at other speeds at low uSec values ie ( < 30)
// This function has poor accuracy at less than 10 microsecs
// Coming to the first if condition takes 13.5 us so for values less than 10 this is the best we can do
void CPU_Timer_Sleep_MicroSeconds( UINT32 uSec )
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
	UINT32 ticks = CPU_MicrosecondsToTicks(uSec);
	while(g_STM32F10x_AdvancedTimer.GetCounter() - currentCounterVal <= ticks);
}

#if 0
// This function is tuned to work when the processor is running at 8 MHz
// When that changes this function no longer works efficiently
void CPU_Timer_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	UINT32 limit = (uSec / 5);
	for(volatile UINT32 i = 0; i < limit; i++);
}
#endif

//TODO: AnanthAtSamraksh --- moved below functions from netmf_time_functions.cpp
UINT64 CPU_Time_CurrentTicks()
{
	//return Time_Driver::CounterValue();
	//if(Timer == 3 || Timer == 4 || Timer == 5)
	//{
		UINT16 value;
		UINT64 m_lastRead = 0;
		GLOBAL_LOCK(irq);
		value = Timer16Bit_Driver::GetCounter(Timer16Bit_Driver::c_SystemTimer);

		// Nived.Sivadas
		// Workaround for the unusual didtimeoverflow bug, added another check
		UINT16 lastSixteenBits = m_lastRead & 0x0000FFFFull;
		m_lastRead &= (0xFFFFFFFFFFFF0000ull);

		if(Timer16Bit_Driver::DidTimeOverFlow( Timer16Bit_Driver::c_SystemTimer ) || (value < lastSixteenBits))
		{
			Timer16Bit_Driver::ClearTimeOverFlow( Timer16Bit_Driver::c_SystemTimer );
			m_lastRead += (0x1ull << 16);
		}

		//Or else the value gets added simply
		m_lastRead += value;

		ENABLE_INTERRUPTS();

		return m_lastRead;
	//}
	//else if(Timer == 1 || Timer == 2 )
	//{
	//}
}

INT64 CPU_Time_TicksToTime( UINT64 Ticks )
{
	//return Time_Driver::TicksToTime( Ticks );
	return CPU_TicksToTime( Ticks );
}

INT64 CPU_Time_CurrentTime()
{
	//return Time_Driver::CurrentTime();
	return CPU_TicksToTime( CPU_Time_CurrentTicks() );
}

void CPU_Time_SetCompare( UINT64 CompareValue )
{
	//Time_Driver::SetCompareValue( CompareValue );
}

void CPU_Time_SetCompare_Completion(UINT64 val)
{
	return;
}

void CPU_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}

/*UINT64 Time_CurrentTicks()
{
    return CPU_Time_CurrentTicks();
}*/

void CPU_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    //Time_Driver::Sleep_uSec( uSec );
}

void CPU_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	//Time_Driver::Sleep_uSec_Loop( uSec );
}

