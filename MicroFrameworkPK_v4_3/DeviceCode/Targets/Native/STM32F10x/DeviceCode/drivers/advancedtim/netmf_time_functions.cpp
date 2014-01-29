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

// Returns ms??? --NPS
UINT64 CPU_TicksToTime( UINT64 Ticks ) {	
	return Ticks*1000/SystemTimerClock;
}

// Returns ms??? --NPS
UINT64 CPU_TicksToTime( UINT32 Ticks32 ) {
	return Ticks32*1000/SystemTimerClock;
}


UINT64 CPU_MillisecondsToTicks( UINT64 Ticks ) {
	return SystemTimerClock/1000*Ticks;
}

UINT64 CPU_MillisecondsToTicks( UINT32 Ticks32 ) {
	return SystemTimerClock/1000*Ticks32;
}

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec ) {
	return SystemTimerClock/1000000*uSec;
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec ) {
	return SystemTimerClock/1000000*uSec;
}

// What is a Tick vs. a SystemClock???? --NPS
UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec ){
	return SystemTimerClock/1000000*uSec;
}

// Is one of these (with above) not used??? --NPS
int CPU_MicosecondsToSystemClocks( int uSec ) {
	return SystemTimerClock/1000000*uSec;
}

int CPU_SystemClocksToMicroseconds( int Ticks ) {
	return Ticks*1000000/SystemTimerClock;
}
