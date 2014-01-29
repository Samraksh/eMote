#include <tinyhal.h>
#include <netmf_advancedtimer.h>
#include <pwr/netmf_pwr.h>

#define TICK_ADJUST (5 / 4)

extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
extern STM32F10x_Timer_Configuration g_STM32F10x_Timer_Configuration;

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

// Apparently this is real-time ticks to 100ns ticks.
UINT64 CPU_TicksToTime( UINT64 Ticks ) {	
	return Ticks * TICK_ADJUST;
}

// Apparently this is real-time ticks to 100ns ticks.
UINT64 CPU_TicksToTime( UINT32 Ticks32 ) {
	return Ticks32 * TICK_ADJUST;
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
	return uSec * SystemTimerClock/1000000;
}

int CPU_MicosecondsToSystemClocks( int uSec ) {
	return uSec * SystemTimerClock/1000000;
}

int CPU_SystemClocksToMicroseconds( int Ticks ) {
	return Ticks * 1000000/SystemTimerClock ;
}
