#include <tinyhal.h>
#include <netmf_advancedtimer.h>
#include <pwr/netmf_pwr.h>

#define TICK_ADJUST 5 / 4

extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
extern STM32F10x_Timer_Configuration g_STM32F10x_Timer_Configuration;

/*

A "time" tick in MF parlance is a 100 ns tick used internally.
A "SystemCoreClock" is the speed of the timer bus, usually either 8 or 24 MHz
A "SystemTimerClock" is defined as an 8 MHz tick on TIM1 and TIM2.
The prescaler on TIM1 and TIM2 is changed along with clock to enforce this.
Not so on other timers.

I'm sad I used a hard-coded TICK_ADJUST.
Somebody fix this if you get time.

Update: Apparently maybe this is handled in the "DriftParameters".
What horrible naming.
--NPS

*/

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
	//return Ticks * TICK_ADJUST;
	return Ticks;
}

// OK, so the DriftParameters *should* correct this in the PAL layer
// But that begs the question of what this function is really for...
UINT64 CPU_TicksToTime( UINT32 Ticks32 ) {
	//return Ticks32 * TICK_ADJUST;
	return Ticks32;
}

// Milli-seconds to real-time ticks???
UINT64 CPU_MillisecondsToTicks( UINT64 mSec ) {
	return mSec * SystemTimerClock/1000;
}

// Milli-seconds to real-time ticks???
UINT64 CPU_MillisecondsToTicks( UINT32 mSec ) {
	return mSec * SystemTimerClock/1000;
}

// Micro-seconds to real-time ticks?
UINT64 CPU_MicrosecondsToTicks( UINT64 uSec ) {
	return uSec * SystemTimerClock/1000000;
}

// Micro-seconds to "real-time ticks" or "fake 100ns" ticks? arrrrggghhhh!
UINT32 CPU_MicrosecondsToTicks( UINT32 uSec ) {
	return uSec * SystemTimerClock/1000000;
}

// wtf is a SystemClock vs a tick?!!??!
UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec ){
	return uSec * SystemCoreClock/1000000;
}

int CPU_MicosecondsToSystemClocks( int uSec ) {
	return uSec * SystemCoreClock/1000000;
}

int CPU_SystemClocksToMicroseconds( int Ticks ) {
	return Ticks * 1000000/SystemCoreClock ;
}
