#include "netmf_time.h"

//#define DEBUG_PRINT

void CPU_CPWAIT()
{
	//TODO Implement it in boot area
#ifdef DEBUG_PRINT
	debug_printf("In CPU_CPWAI\n");
#endif
}

UINT32 CPU_SystemClock()
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_SystemClock");
#endif
	return SYSTEM_CLOCK_HZ;
}

UINT32 CPU_TicksPerSecond()
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_TicksPerSecond");
#endif
	return SLOW_CLOCKS_PER_SECOND;
}

UINT64 CPU_TicksToTime( UINT64 Ticks )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_TicksToTime(UINT64), Ticks %lld\n", Ticks);
#endif
    Ticks *= (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
    Ticks /= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_TicksToTime(UINT64), Ticks %lld\n", Ticks);
#endif
	return Ticks;
}

UINT64 CPU_TicksToTime( UINT32 Ticks32 )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_TicksToTime(UINT32), Ticks32 %u\n", Ticks32);
#endif
	UINT64 Ticks;
	Ticks  = (UINT64)Ticks32 * (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
	Ticks /=                   (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_TicksToTime(UINT32), Ticks %lld\n", Ticks);
#endif
	return Ticks;
}

UINT64 CPU_MillisecondsToTicks( UINT64 Ticks )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MillisecondsToTicks(UINT64), Ticks %lld\n", Ticks);
#endif
	Ticks *= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
	Ticks /= (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MillisecondsToTicks(UINT64), Ticks %lld\n", Ticks);
#endif
	return Ticks;
}

UINT64 CPU_MillisecondsToTicks( UINT32 Ticks32 )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MillisecondsToTicks(UINT32), Ticks %u\n", Ticks32);
#endif
    UINT64 Ticks;
    Ticks  = (UINT64)Ticks32 * (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
    Ticks /=                   (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MillisecondsToTicks(UINT32), Ticks %lld\n", Ticks);
#endif
    return Ticks;
}

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec )
{
	//return uSec;	
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
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec )
{
	//return uSec;
	//return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);		
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
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicosecondsToSystemClocks(UINT32), Ticks before %u\n", uSec);
#endif
	uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
	uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicosecondsToSystemClocks(UINT32), Ticks after %u\n", uSec);
#endif
	return uSec;
}

int CPU_MicosecondsToSystemClocks( int uSec )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicosecondsToSystemClocks, Ticks before %d\n", uSec);
#endif
    uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicosecondsToSystemClocks, Ticks after %d\n", uSec);
#endif
	return uSec;
}

int CPU_SystemClocksToMicroseconds( int Ticks )
{
	//return Ticks;
#ifdef DEBUG_PRINT
	debug_printf("In CPU_SystemClocksToMicroseconds, Ticks before %d\n", Ticks);
#endif
    Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
    Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_SystemClocksToMicroseconds, Ticks after %d\n", Ticks);
#endif
   return Ticks;
}

//#pragma arm section code = "SectionForFlashOperations"

//UINT32 __section(SectionForFlashOperations) CPU_MicrosecondsToSystemClocks( UINT32 uSec )
//{
    //uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
//    //uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
//
//    return uSec;
//}
//
//int __section(SectionForFlashOperations) CPU_MicrosecondsToSystemClocks( int uSec )
//{
//    //uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
//    //uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
//
//    return uSec;
//}
//
//
////--//
//
//int __section(SectionForFlashOperations) CPU_SystemClocksToMicroseconds( int Ticks )
//{
//    //Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
//    //Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
//
//    return Ticks;
//}
//
//#pragma arm section code


BOOL HAL_Time_Initialize()
{		
	return TRUE;
	//return Time_Driver::Initialize();
}

BOOL HAL_Time_Uninitialize()
{
	return Time_Driver::Uninitialize();
}

UINT64 HAL_Time_CurrentTicks()
{
	return Time_Driver::CounterValue();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
	return Time_Driver::TicksToTime( Ticks );
}

INT64 HAL_Time_CurrentTime()
{
	return Time_Driver::CurrentTime();
}

void HAL_Time_SetCompare( UINT64 CompareValue )
{
	Time_Driver::SetCompareValue( CompareValue );
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}

UINT64 Time_CurrentTicks()
{
    return HAL_Time_CurrentTicks();
}

void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    Time_Driver::Sleep_uSec( uSec );
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	Time_Driver::Sleep_uSec_Loop( uSec );
}
