/*
 * Samraksh Time Driver
 * Initial Create - Kartik Natarajan
 * 04/20/2011
 *
 * Modified - Mukundan Sridharan; Ananth Muralidharan
 * 07/25/2014
 * Notes -
 *
 */

#ifndef __NETMF_TIME_DRIVER__
#define __NETMF_TIME_DRIVER__

#include <tinyhal.h>
#include "Time_decl.h"


struct Time_Driver
{
    static UINT32 maxTicks;
	static UINT64 bigCounter;
	static UINT32 prevTicks;
	static bool bigCounterUpdated;
	static bool overflowCondition;

    //AnanthAtSamraksh - changing all functions to non-static
    BOOL Initialize  ();
    BOOL Uninitialize();
    UINT64 CounterValue();
	void StopTimer();
    void SetCompareValue( UINT64 CompareValue );
    INT64 TicksToTime( UINT64 Ticks );
    INT64 CurrentTime();
    INT64 TimeNow();
    UINT64 CurrentTicks();
    void Sleep_uSec( UINT32 uSec );
    void Sleep_uSec_Loop( UINT32 uSec );
    ////static void ISR( void* Param );
};

extern Time_Driver g_Time_Driver;

// Profile delays related to setting the hal system event timer.
#if defined(DEBUG_EMOTE_TIME)
extern volatile UINT64 badComparesCount;
extern volatile UINT64 badComparesAvg;
extern volatile UINT64 badComparesMax;
#endif

#endif //__NETMF_TIME_DRIVER__