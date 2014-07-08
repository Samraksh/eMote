/*
 * Name : Krait__TIME_Functions.cpp
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

#include "Krait__TIME.h"
#include "Krait__TIMER.h"


//AnanthAtSamraksh -- adding timer interfaces as part of porting virtual timers
BOOL CPU_Timer_Initialize(UINT16 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
{
	return InitializeTimer(Timer);
}

BOOL CPU_Timer_SetCompare(UINT16 Timer, UINT32 CompareValue)
{
	SetCompare(Timer, CompareValue);
}

UINT16 CPU_Timer_GetCounter(UINT16 Timer)
{
	return GetCounter(Timer);
}

UINT64 CPU_Timer_CurrentTicks(UINT16 Timer)
{
	return GetCounter(Timer);
}

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
    Ticks *= (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
    Ticks /= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);

    return Ticks;
}

UINT64 CPU_TicksToTime( UINT32 Ticks32, UINT16 Timer )
{
    UINT64 Ticks;

    Ticks  = (UINT64)Ticks32 * (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
    Ticks /=                   (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);

    return Ticks;
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
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec, UINT16 Timer )
{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
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


BOOL HAL_Time_Initialize()
{
    return Initialize();
}

BOOL HAL_Time_Uninitialize()
{
    return Uninitialize();
}

UINT64 Time_CurrentTicks()
{
    return HAL_Time_CurrentTicks(0);
}

UINT64 HAL_Time_CurrentTicks(UINT16 Timer)
{
    return TimeNow();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
    return TicksToTime( Ticks );
}

INT64 HAL_Time_CurrentTime(UINT16 Timer)
{
    return CurrentTime();
}

void HAL_Time_SetCompare( UINT64 CompareValue, UINT16 Timer )
{
    SetCompareValue( CompareValue );
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}


void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    Sleep_uSec( uSec );
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
    Sleep_uSec_Loop( uSec );
}

#pragma GCC reset_options
