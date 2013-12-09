#include <tinyhal.h>
#include <netmf_advancedtimer.h>

extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
extern STM32F10x_Timer_Configuration g_STM32F10x_Timer_Configuration;

void CPU_CPWAIT()
{

}

UINT32 CPU_SystemClock()
{
	return g_STM32F10x_Timer_Configuration.systemClockHz;
}

UINT32 CPU_TicksPerSecond()
{
	return g_STM32F10x_Timer_Configuration.slowClocksPerSecond;
}

// This api fails for small values of ticks, because that means the result would essentially be a fraction
// of a ms and since the return type is a UINT64 there is no way to represent the fraction unless i use binary
// fixed point arithmetic, but the user of this api still has no idea of the nature of the result and hence
// using fixed point arithmetic would still not solve this problem
// This is a broken api from microsoft, but since float (on the cortex m3
// there is no floating point unit and hence has to be simulated in software) is an expensive
// operation on microcontrollers and users are rarely going to pass values less than a ms
// we will live with this api. - nived.sivadas
UINT64 CPU_TicksToTime( UINT64 Ticks )
{
	if(Ticks == 0)
		return 0;

	Ticks *= (TEN_MHZ / g_STM32F10x_Timer_Configuration.slowClocksTenMhzGcd);

	Ticks /= g_STM32F10x_Timer_Configuration.ratio1;

    return Ticks;
}

UINT64 CPU_TicksToTime( UINT32 Ticks32 )
{
	return CPU_TicksToTime((UINT64) Ticks32);
}

UINT64 CPU_MillisecondsToTicks( UINT64 Ticks )
{

	Ticks *= g_STM32F10x_Timer_Configuration.ratio2;
	Ticks *= g_STM32F10x_Timer_Configuration.slowClocksMillisecondGcd/1000;

	return Ticks;
}

UINT64 CPU_MillisecondsToTicks( UINT32 Ticks32 )
{
	return CPU_MillisecondsToTicks((UINT64) Ticks32);
}

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec )
{
	return (uSec * g_STM32F10x_Timer_Configuration.ratio4);
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec )
{
	return (UINT32) CPU_MicrosecondsToTicks((UINT64) uSec);
}

UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec )
{
	uSec *= g_STM32F10x_Timer_Configuration.ratio3;
	uSec /= (ONE_MHZ / g_STM32F10x_Timer_Configuration.clockCommonFactor);

	return uSec;
}

int CPU_MicosecondsToSystemClocks( int uSec )
{

	uSec *= g_STM32F10x_Timer_Configuration.ratio3;
    uSec /= (ONE_MHZ / g_STM32F10x_Timer_Configuration.clockCommonFactor);

    return uSec;
}

int CPU_SystemClocksToMicroseconds( int Ticks )
{
    Ticks *= (ONE_MHZ        /g_STM32F10x_Timer_Configuration.clockCommonFactor);
    Ticks /= g_STM32F10x_Timer_Configuration.ratio3;
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
