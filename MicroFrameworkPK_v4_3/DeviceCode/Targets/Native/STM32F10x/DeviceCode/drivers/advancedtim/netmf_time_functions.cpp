#include <tinyhal.h>
#include <netmf_advancedtimer.h>
#include <pwr/netmf_pwr.h>

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

// returning time in mSec
UINT64 CPU_TicksToTime( UINT64 Ticks ) {	
	return Ticks * 1000/SystemTimerClock;
}

// returning time in mSec
UINT64 CPU_TicksToTime( UINT32 Ticks32 ) {
	return Ticks32 * 1000/SystemTimerClock;
}

UINT64 CPU_MillisecondsToTicks( UINT64 mSec ) {
	return mSec * SystemTimerClock/1000;
}

UINT64 CPU_MillisecondsToTicks( UINT32 mSec ) {
	return mSec * SystemTimerClock/1000;
}

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec ) {
	return uSec * SystemTimerClock/1000000;
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec ) {
	return uSec * SystemTimerClock/1000000;
}

UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec ){
	return uSec * SystemCoreClock/1000000;
}

int CPU_MicosecondsToSystemClocks( int uSec ) {
	return uSec * SystemCoreClock/1000000;
}

int CPU_SystemClocksToMicroseconds( int Ticks ) {
	return Ticks * 1000000/SystemCoreClock ;
}
