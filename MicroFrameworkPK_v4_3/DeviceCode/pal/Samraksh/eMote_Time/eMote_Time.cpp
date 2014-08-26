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
	g_Time_Driver.bigCounter += g_Time_Driver.maxTicks;
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

//BOOL state = FALSE;

UINT64 Time_Driver::CurrentTicks()
{
	return VirtTimer_GetTicks(VIRT_TIMER_TIME);
}

UINT64 Time_Driver::CounterValue()
{
	return VirtTimer_GetCounter(VIRT_TIMER_TIME);
}


void SetCompareHandler(void *arg)
{
	HAL_COMPLETION::DequeueAndExec();
}

void Time_Driver::SetCompareValue( UINT64 compareTicks )
{
	//CPU_GPIO_SetPinState((GPIO_PIN) 52, TRUE);
	//CPU_GPIO_SetPinState((GPIO_PIN) 52, FALSE);

	UINT32 compareTimeInMicSecs = 0;

	/*if(compareTicks >= 0xFFFFFFFF)
	{
		compareTimeInMicSecs = 0xFFFFFFFF;
		//hal_printf("if compareTimeInMicSecs: %d \r\n", compareTimeInMicSecs-1);
	}
	else{
		compareTimeInMicSecs = CPU_TicksToTime((UINT32)compareTicks, 1);
		//hal_printf("else compareTimeInMicSecs: %d \r\n", compareTimeInMicSecs-1);
	}*/

	//if(compareTicks <= 0x25ED097B)
	if(compareTicks <= 0x19999999)
		compareTimeInMicSecs = CPU_TicksToTime((UINT32)compareTicks, 1);
	else
		compareTimeInMicSecs = 0xFFFFFFFF;

	if(VirtTimer_SetTimer(VIRT_TIMER_EVENTS, 0, compareTimeInMicSecs, TRUE, TRUE, SetCompareHandler) == TimerReserved)
	{
		VirtTimer_Change(VIRT_TIMER_EVENTS, 0, compareTimeInMicSecs, TRUE);
	}

	VirtTimer_Start( VIRT_TIMER_EVENTS );

	/*if(VirtTimer_SetTimer(VIRT_TIMER_EVENTS, 0, compareTicks, TRUE, TRUE, SetCompareHandler) == TimerReserved)
	{
		//VirtTimer_Change(VIRT_TIMER_EVENTS, 0, compareTicks, TRUE);
	}

	VirtTimer_Start( VIRT_TIMER_EVENTS );
	//VirtTimer_Stop( VIRT_TIMER_EVENTS );
	CPU_Timer_SetCompare(1, compareTicks);*/
}


INT64 Time_Driver::TicksToTime( UINT64 Ticks )
{
	return VirtTimer_TicksToTime(VIRT_TIMER_TIME, Ticks);
}

INT64 Time_Driver::CurrentTime()
{
	UINT32 currentTicks = VirtTimer_GetTicks(VIRT_TIMER_TIME);
	UINT64 currentTime = 0;

	if(currentTicks < prevTicks)
	{
		UINT32 diff = (maxTicks - prevTicks ) + currentTicks;
		currentTime = VirtTimer_TicksToTime(VIRT_TIMER_TIME, (bigCounter + (UINT64)prevTicks + (UINT64)diff));
	}
	else
	{
		currentTime = VirtTimer_TicksToTime(VIRT_TIMER_TIME, (bigCounter + (UINT64)currentTicks));
	}

	prevTicks = currentTicks;

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
