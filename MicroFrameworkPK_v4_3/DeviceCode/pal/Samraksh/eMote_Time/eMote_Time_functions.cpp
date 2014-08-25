/*
 * Samraksh Time Driver
 * Initial Create - Mukundan Sridharan; Ananth Muralidharan
 * 07/24/2014
 *
 */

#include "eMote_Time.h"


static void ISR( void* Param );


BOOL HAL_Time_Initialize()
{
	return g_Time_Driver.Initialize();
}

BOOL HAL_Time_Uninitialize()
{
	return g_Time_Driver.Uninitialize();
}

INT64 HAL_Time_CurrentTime()
{
	return g_Time_Driver.CurrentTime();
}


UINT64 Time_CurrentTicks()
{
	//return g_Time_Driver.CurrentTicks();
	return HAL_Time_CurrentTicks();
}


UINT64 HAL_Time_CurrentTicks()
{
	//return g_Time_Driver.CurrentTicks();
	return g_Time_Driver.TimeNow();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
	return g_Time_Driver.TicksToTime(Ticks);
}

void HAL_Time_SetCompare( UINT64 CompareValue )
{
	g_Time_Driver.SetCompareValue(CompareValue);
}

void HAL_Time_SetCompare_Completion(UINT64 CompareValue)
{
	g_Time_Driver.SetCompareValue(CompareValue);
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}

void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
	g_Time_Driver.Sleep_uSec(uSec);
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	g_Time_Driver.Sleep_uSec(uSec);
}

