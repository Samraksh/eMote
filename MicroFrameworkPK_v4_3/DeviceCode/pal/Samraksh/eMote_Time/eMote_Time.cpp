/*
 * Samraksh Time Driver
 * Initial Create - Kartik Natarajan
 * 04/20/2011
 * Modified - Mukundan Sridharan; Ananth Muralidharan
 * 07/25/2014
 * Notes - 1) Check ticks to time conversion math
 * 		   2) Implement the Sleep functions, CPU_CPWAIT()
 * 		   3) CounterValue function, maybe need to re-implement in a different way!
 *
 */

#include "eMote_Time.h"
#include "../Include/Samraksh/VirtualTimer.h"

//#define DEBUG_TIMER

Time_Driver g_Time_Driver;
UINT32 Time_Driver::maxTicks = 0;
UINT32 Time_Driver::prevTicks = 0;
UINT64 Time_Driver::bigCounter = 0;
bool Time_Driver::bigCounterUpdated = false;
bool Time_Driver::overflowCondition = false;


void TimeCallback(void *arg);
void TimeHandler(void *arg);


BOOL Time_Driver::Initialize()
{
	maxTicks = VirtTimer_GetMaxTicks(VIRT_TIMER_TIME);

	if(VirtTimer_SetTimer(VIRT_TIMER_TIME, 0, maxTicks, FALSE, TRUE, TimeHandler) != TimerSupported)
		return FALSE;

	if(VirtTimer_Start( VIRT_TIMER_TIME ) != TimerSupported)
		return FALSE;

	return TRUE;
}


void TimeHandler(void *arg)
{
	static int  timeHandlerCount = 0;

	GLOBAL_LOCK(irq);

	if(timeHandlerCount > 0)
	{
		g_Time_Driver.bigCounter += g_Time_Driver.maxTicks;
		g_Time_Driver.bigCounterUpdated = true;
	}

	if(timeHandlerCount == 0)
		timeHandlerCount++;
}


BOOL Time_Driver::Uninitialize()
{
	if(VirtTimer_Stop( VIRT_TIMER_EVENTS ) != TimerSupported)
		return FALSE;

	if(VirtTimer_Stop( VIRT_TIMER_TIME ) != TimerSupported)
		return FALSE;

	if(!VirtTimer_UnInitialize())
		return FALSE;

	return TRUE;
}

UINT64 Time_Driver::CurrentTicks()
{
	UINT64 currentTicks = VirtTimer_GetTicks(VIRT_TIMER_TIME);

	return currentTicks;
}

UINT64 Time_Driver::CounterValue()
{
	return VirtTimer_GetCounter(VIRT_TIMER_TIME);
}


void SetCompareHandler(void *arg)
{
	HAL_COMPLETION::DequeueAndExec();
}

void Time_Driver::StopTimer()
{
	VirtTimer_Stop( VIRT_TIMER_EVENTS );
}

void Time_Driver::SetCompareValue( UINT64 compareTicks )
{
	UINT32 compareTimeInMicroSecs = 0;

	if(compareTicks < 0xFFFFFFFF)
	{
		compareTimeInMicroSecs = CPU_TicksToMicroseconds((UINT32)compareTicks, 1);
	}
	else
	{
		compareTimeInMicroSecs = 0xFFFFFFFF;
	}

	if(VirtTimer_SetTimer(VIRT_TIMER_EVENTS, 0, compareTimeInMicroSecs, TRUE, TRUE, SetCompareHandler) == TimerReserved)
	{
		VirtTimer_Change(VIRT_TIMER_EVENTS, 0, compareTimeInMicroSecs, TRUE);
	}

	VirtTimer_Start( VIRT_TIMER_EVENTS );
}


INT64 Time_Driver::TicksToTime( UINT64 Ticks )
{
	return VirtTimer_TicksToTime(VIRT_TIMER_TIME, Ticks);
}

INT64 Time_Driver::CurrentTime()
{
	INT32 a, b;
	INT64 c;
	CPU_GetDriftParameters(&a, &b, &c);

	UINT64 currentTotalTicks = CurrentTicks();

	GLOBAL_LOCK(irq);
	UINT64 currentTime = 0;

	if (a == 1)
	{
		currentTime = (currentTotalTicks * b + c);
	} else if (a == 2) {
		currentTime = (currentTotalTicks * b + c);
		currentTime = currentTime >> 1;
	} else if (a == 4) {
		currentTime = (currentTotalTicks * b + c);
		currentTime = currentTime >> 2;
	} else if(a != 0) {
		currentTime = (currentTotalTicks * b + c) / a;
	}

	return (INT64)currentTime;
}

INT64 Time_Driver::TimeNow()
{
	return CurrentTime();
}

void Time_Driver::Sleep_uSec( UINT32 uSec )
{	
	VirtTimer_SleepMicroseconds(VIRT_TIMER_TIME, uSec);
}

void Time_Driver::Sleep_uSec_Loop( UINT32 uSec )
{
	VirtTimer_SleepMicroseconds(VIRT_TIMER_TIME, uSec);
}
