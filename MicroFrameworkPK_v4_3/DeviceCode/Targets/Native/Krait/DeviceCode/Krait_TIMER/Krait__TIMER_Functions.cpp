/*
 * Name : Krait__TIMER_Functions.cpp
 *
 * Author :Mukundan.Sridharan@Samraksh.com
 * Description : Contains the hal level api for time
 * Update:
 *       Date: July 9, 2014
 *       Author: Ananth.Muralidharan@Samraksh.info
 *       Description: Porting virtual timers
 *
 */

#ifndef SAM_FORCE_GCC_CMDLINE_OPTS
#pragma GCC optimize ("O3")
#endif

#include <tinyhal.h>

#include "Krait__TIMER.h"

extern Krait_Timer g_Krait_Timer;


////BOOL CPU_Timer_Initialize(UINT16 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
BOOL CPU_Timer_Initialize(UINT16 Timer, BOOL IsOneShot, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
{
	return g_Krait_Timer.InitializeTimer(0, IsOneShot, ISR, ISR_PARAM);
}

BOOL CPU_Timer_UnInitialize(UINT16 Timer)
{
	return FALSE;
}

#define TICKS_PROXIMITY_FORCE 10
BOOL CPU_Timer_SetCompare(UINT16 Timer, UINT32 CompareValue)
{
	UINT32 diff;
	UINT32 now, setCompareValue;
	//int forceInterrupt=0;

	if (g_Krait_Timer.nextCompare != 0 && CompareValue > g_Krait_Timer.nextCompare) {
		return FALSE;
	}

	GLOBAL_LOCK(irq);

	g_Krait_Timer.nextCompare = CompareValue;

	//now = TimeNow();
	now = CPU_Timer_CurrentTicks(0);
	/*if( CompareValue - now <= TICKS_PROXIMITY_FORCE ) {
		forceInterrupt=1;
		//hal_printf("force interrupt\r\n");
	}
	else if(CompareValue - now > MAX_TIMER_ROLLOVER) { // 0xFFFFFF00
		//hal_printf("debug: new compare too far in future, deferring\r\n");
		return FALSE; // We'll get it later
	}*/

	/*if (forceInterrupt)
		diff = TICKS_PROXIMITY_FORCE; // a small time in the future
	else
	{
		//if(CompareValue < now)
			//diff = now - CompareValue;
		//else
			diff = CompareValue + now;
	}*/

	setCompareValue = CompareValue + now;

	//g_Krait_Timer.SetCompare(Timer, CompareValue);
	////g_Krait_Timer.SetCompare(Timer, setCompareValue);
	/*if(setCompareValue > 0xFFFF0000 || setCompareValue < 0x0000FFFF)
	{
		writel(0, DGT_CLEAR);
		g_Krait_Timer.SetCompare(0, (0xFFFFFFFF - now) + CompareValue);
	}
	else*/
		g_Krait_Timer.SetCompare(0, setCompareValue);
	//SetCompareValue(CompareValue);

	//AnanthAtSamraksh
	/*static UINT8 setCompareCount = 0;
	if(setCompareCount == 0){
		setCompareCount++;
		CPU_INTC_InterruptEnable( INT_DEBUG_TIMER_EXP );
	}*/

}

UINT16 CPU_Timer_SetCounter(UINT16 Timer, UINT32 Count)
{
	g_Krait_Timer.SetCounter(Timer, Count);
}

UINT16 CPU_Timer_GetCounter(UINT16 Timer)
{
	return (UINT16)g_Krait_Timer.GetCounter(Timer);
}

UINT32 CPU_Timer_CurrentTicks(UINT16 Timer)
{
	return g_Krait_Timer.GetCounter(0);
}


// Correction factor. Assumes -O0
// Only corrects for native, not managed.
#define TINYCLR_TIMER_MUNGE 4

void CPU_Timer_Sleep_MicroSeconds( UINT32 uSec, UINT16 Timer)
{
	GLOBAL_LOCK(irq);

	if(uSec <= TINYCLR_TIMER_MUNGE) {
		return;
	}

	UINT32 maxDiff  = CPU_MicrosecondsToSystemClocks( uSec - TINYCLR_TIMER_MUNGE );
	UINT32 value   = g_Krait_Timer.GetCounter( 0 );

	while((g_Krait_Timer.GetCounter(0) - value) <= maxDiff);
}


BOOL CPU_Timer_DidTimerOverflow(UINT8 Timer)
{
	return g_Krait_Timer.DidTimerOverflow();
}

void CPU_Timer_ClearTimerOverflow(UINT8 Timer)
{
	g_Krait_Timer.ClearTimerOverflow();
}

UINT32 CPU_Timer_GetMaxTicks(UINT8 Timer)
{
	return g_Krait_Timer.GetMaxTicks();
}


/*void IRQ_Handler(void* arg)
{
	uint32_t num;
	enum handler_return ret;

	num = readl(GIC_CPU_INTACK);
	if (num > NR_IRQS)
		return;

	ret = handler[num].func(handler[num].arg);
	writel(num, GIC_CPU_EOI);
}*/

//AnanthAtSamraksh -- end of timer interfaces


UINT32 CPU_SystemClock(UINT16 Timer)
{
    return SYSTEM_CLOCK_HZ;
}

UINT32 CPU_TicksPerSecond(UINT16 Timer)
{
    return SLOW_CLOCKS_PER_SECOND;
}

//--//

UINT64 CPU_TicksToTime( UINT64 Ticks, UINT16 Timer )
{
/*#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	Ticks *= (ONE_MHZ               /SLOW_CLOCKS_PER_SECOND);

	//UINT64 divisor = (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
	//Ticks /= divisor;
	return Ticks;
#else*/
	Ticks *= (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);

    UINT64 divisor = (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
    Ticks /= divisor;
    return Ticks;

    /*if(Ticks > 0xFFFFFFFF)
		return 0xFFFFFFFF;
	else
		return Ticks;*/

//#endif
}

UINT64 CPU_TicksToTime( UINT32 Ticks32, UINT16 Timer )
{
/*#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	UINT64 Ticks;

	Ticks  = (UINT64)Ticks32 * (ONE_MHZ               /SLOW_CLOCKS_PER_SECOND);

	//UINT64 divisor = (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
	//Ticks /=                   divisor;
	return Ticks;
#else*/
	UINT64 Ticks;

    Ticks  = (UINT64)Ticks32 * (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);

    UINT64 divisor = (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
    Ticks /=                   divisor;
    return Ticks;

    /*if(Ticks > 0xFFFFFFFF)
    	return 0xFFFFFFFF;
    else
    	return Ticks;*/

//#endif
}

//--//

UINT64 CPU_MillisecondsToTicks( UINT64 Ticks, UINT16 Timer )
{
    Ticks *= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
    Ticks /= (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);

    return Ticks;
}

UINT64 CPU_MillisecondsToTicks( UINT32 Ticks32, UINT16 Timer )
{
    UINT64 Ticks;

    Ticks  = (UINT64)Ticks32 * (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
    Ticks /=                   (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);

    return Ticks;
}

//--//

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec, UINT16 Timer )
{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	//AnanthAtSamraksh - defaulting to an int during division results in lose of precision. Hence typecasting to a double.
    //return uSec * ((double)SLOW_CLOCKS_PER_SECOND / (double)ONE_MHZ);
	UINT64 slowClocksFactor = SLOW_CLOCKS_PER_SECOND / SLOW_CLOCKS_TEN_MHZ_GCD;
	return (uSec * slowClocksFactor) / 4;
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec, UINT16 Timer )
{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	//AnanthAtSamraksh - there is a lose of precision during the division below. Hence typecasting to a double.
	//Floating point operations slow down the CPU. So, removing double and getting the same factor using int arithmetic.
	//return uSec * ((double)SLOW_CLOCKS_PER_SECOND / (double)ONE_MHZ);

	//There is a loss of precision for a uSec value that is not exactly divisible by 4
	//4 * SLOW_CLOCKS_TEN_MHZ_GCD = ONE_MHZ
	UINT32 slowClocksFactor = SLOW_CLOCKS_PER_SECOND / SLOW_CLOCKS_TEN_MHZ_GCD;
	UINT32 ticksValue = (uSec * slowClocksFactor) / 4;
	return ticksValue;
	/*UINT64 ticksValue = ((UINT64)uSec * (UINT64)slowClocksFactor) / 4;
	if(ticksValue > 0xFFFFFFFF)
	{
		hal_printf("returning 0xFFFFFFFF\r\n");
		return 0xFFFFFFFF;
	}
	else
		return (UINT32)ticksValue;*/
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

//--//

UINT32 __section(SectionForFlashOperations) CPU_MicrosecondsToSystemClocks( UINT32 uSec )
{
    uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);

    return uSec;
}

int __section(SectionForFlashOperations) CPU_MicrosecondsToSystemClocks( int uSec )
{
    uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);

    return uSec;
}


//--//

int __section(SectionForFlashOperations) CPU_SystemClocksToMicroseconds( int Ticks )
{
    Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
    Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);

    return Ticks;
}

// -- //





#pragma GCC reset_options
