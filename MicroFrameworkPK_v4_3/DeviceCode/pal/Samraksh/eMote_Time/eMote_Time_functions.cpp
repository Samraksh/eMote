/*
 * Samraksh Time Driver
 * Initial Create - Mukundan Sridharan; Ananth Muralidharan
 * 07/24/2014
 *
 */

#include "eMote_Time.h"

static const UINT64 HAL_Completion_IdleValue = 0x0000FFFFFFFFFFFFull;

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
	return g_Time_Driver.CurrentTicks();
	//return g_Time_Driver.TimeNow();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
	return g_Time_Driver.TicksToTime(Ticks);
}

#if defined(DEBUG_EMOTE_TIME)
volatile UINT64 badComparesCount = 0;    //!< number of requests set in the past.
volatile UINT64 badComparesAvg = 11;     //!< average delay of requests set in the past. init to observed value.
volatile UINT64 badComparesMax = 0;      //!< observed worst-case.
#endif

void HAL_Time_SetCompare( UINT64 CompareValue )
{
	if (CompareValue == HAL_Completion_IdleValue){
		g_Time_Driver.StopTimer();
	} else {
		UINT64 Now = HAL_Time_CurrentTicks();
		if(CompareValue > Now) {
			g_Time_Driver.SetCompareValue(CompareValue - Now );
		}
		else {
#if defined(DEBUG_EMOTE_TIME)
			++badComparesCount;
			if(badComparesMax < (Now - CompareValue)) { badComparesMax = Now - CompareValue; }
			badComparesAvg = (badComparesAvg * (badComparesCount - 1) + (Now - CompareValue)) / badComparesCount;
#endif
			g_Time_Driver.SetCompareValue(Now + 100);  // assume g_Time_Driver uses virtual timer so compare value cannot miss and therefore any small compare value suffices.
		}
	}
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
	CPU_GetDriftParameters(a, b, c);
}

void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
	g_Time_Driver.Sleep_uSec(uSec);
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	g_Time_Driver.Sleep_uSec(uSec);
}

