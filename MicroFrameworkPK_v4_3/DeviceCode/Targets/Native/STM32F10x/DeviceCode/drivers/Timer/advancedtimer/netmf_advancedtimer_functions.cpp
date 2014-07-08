/*
#include <tinyhal.h>
#include <netmf_advancedtimer.h>
#include <pwr/netmf_pwr.h>

extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
extern STM32F10x_Timer_Configuration g_STM32F10x_Timer_Configuration;
*/

/*

A "time" tick in MF parlance is a 100 ns tick used internally.
A "SystemTimerClock" is defined as an 8 MHz tick on TIM1 and TIM2.
The prescaler on TIM1 is changed along with clock to enforce this.
Not so on other timers, they must be checked.


The difference between the 8 MHz clock we use and the 10 MHz MF expects
is accounted for in the DriftParameters (terrible naming, thanks MS).

--NPS

*/

/*
////TODO: AnanthAtSamraksh - This is not to be used anymore. All functions below have been moved to netmf_timers.cpp.

void CPU_CPWAIT()
{

}

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


UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec ){
	return uSec * (SystemTimerClock/1000000);
}

int CPU_MicosecondsToSystemClocks( int uSec ) {
	return uSec * (SystemTimerClock/1000000);
}

int CPU_SystemClocksToMicroseconds( int Ticks ) {
	return Ticks * 1000000 / SystemTimerClock ;
}
*/

