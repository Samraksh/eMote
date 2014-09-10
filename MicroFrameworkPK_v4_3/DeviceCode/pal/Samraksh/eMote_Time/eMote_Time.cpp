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
	static int  timeHandlerCount = 0;

	if(timeHandlerCount > 0)
		g_Time_Driver.bigCounter += g_Time_Driver.maxTicks;

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

//BOOL state = FALSE;

UINT64 Time_Driver::CurrentTicks()
{
	//return VirtTimer_GetTicks(VIRT_TIMER_TIME);

	UINT64 currentTotalTicks = 0;
	UINT32 currentTicks = VirtTimer_GetTicks(VIRT_TIMER_TIME);

	if(currentTicks < prevTicks)
	{
		//This if loop is exercised when the counter overflows.
		//But, sometimes currentTicks is less than prevTicks (observed in .NOW and Adapt), even before the overflow. The reason is unknown.
		//currTicks is supposed to increase monotonically, but sometimes it does not. That is why, the check happens once again.
		currentTicks = VirtTimer_GetTicks(VIRT_TIMER_TIME);
		if(currentTicks < prevTicks)
		{
			UINT32 diff = (maxTicks - prevTicks ) + currentTicks;
			//currentTime = VirtTimer_TicksToTime(VIRT_TIMER_TIME, (bigCounter + (UINT64)prevTicks + (UINT64)diff));
			currentTotalTicks = bigCounter + (UINT64)prevTicks + (UINT64)diff;
		}
		else
		{
			currentTotalTicks = bigCounter + (UINT64)currentTicks;
		}
	}
	else
	{
		//currentTime = VirtTimer_TicksToTime(VIRT_TIMER_TIME, (bigCounter + (UINT64)currentTicks));
		currentTotalTicks = bigCounter + (UINT64)currentTicks;
	}

	prevTicks = currentTicks;

	return currentTotalTicks;
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

	UINT32 compareTimeInMicroSecs = 0;

	/*if(compareTicks >= 0xFFFFFFFF)
	{
		compareTimeInMicroSecs = 0xFFFFFFFF;
		//hal_printf("if compareTimeInMicroSecs: %d \r\n", compareTimeInMicroSecs-1);
	}
	else{
		compareTimeInMicroSecs = CPU_TicksToMicroseconds((UINT32)compareTicks, 1);
		//hal_printf("else compareTimeInMicroSecs: %d \r\n", compareTimeInMicroSecs-1);
	}*/

	//if(compareTicks <= 0x25ED097B)
	//if(compareTicks <= 0x19999999)
	if(compareTicks < 0xFFFFFFFF)
	{
		//compareTimeInMicSecs = CPU_TicksToTime((UINT32)compareTicks, 1);
#ifdef PLATFORM_ARM_EmoteDotNow
		compareTimeInMicroSecs = (UINT32)(CPU_TicksToMicroseconds(compareTicks, 1));
#else
		//compareTimeInMicroSecs = ((UINT32)compareTicks) * 4 / 27;
		compareTimeInMicroSecs = (UINT32)(CPU_TicksToMicroseconds(compareTicks, 1));
#endif
	}
	else
		compareTimeInMicroSecs = 0xFFFFFFFF;

	if(VirtTimer_SetTimer(VIRT_TIMER_EVENTS, 0, compareTimeInMicroSecs, TRUE, TRUE, SetCompareHandler) == TimerReserved)
	{
		VirtTimer_Change(VIRT_TIMER_EVENTS, 0, compareTimeInMicroSecs, TRUE);
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
	INT32 a, b;
	INT64 c;
	CPU_GetDriftParameters(&a, &b, &c);

	UINT64 currentTotalTicks = CurrentTicks();
	UINT64 currentTime = 0;

	if(a != 0)
	{
		currentTime = (currentTotalTicks * b + c) / a;
	}

	/*UINT64 currentTime = 0;
	UINT32 currentTicks = VirtTimer_GetTicks(VIRT_TIMER_TIME);

	if(currentTicks < prevTicks)
	{
		UINT32 diff = (maxTicks - prevTicks ) + currentTicks;
		//currentTime = VirtTimer_TicksToTime(VIRT_TIMER_TIME, (bigCounter + (UINT64)prevTicks + (UINT64)diff));
		currentTime = bigCounter + (UINT64)prevTicks + (UINT64)diff;
	}
	else
	{
		//currentTime = VirtTimer_TicksToTime(VIRT_TIMER_TIME, (bigCounter + (UINT64)currentTicks));
		currentTime = bigCounter + (UINT64)currentTicks;
	}

	prevTicks = currentTicks;*/

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
