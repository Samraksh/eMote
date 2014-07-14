/*
 * Samraksh Time Driver
 * Initial Create - Kartik Natarajan
 * 04/20/2011
 * Notes -
 *
 */

#ifndef __NETMF_TIME_DRIVER__
#define __NETMF_TIME_DRIVER__

#include <tinyhal.h>
#include "Time_decl.h"

//AnanthAtSamraksh
#include "Timer\advancedtimer\netmf_advancedtimer.h"
#include "Timer\Timer16Bit\netmf_timers16Bit.h"
//AnanthAtSamraksh


struct Time_Driver
{
    UINT64 m_lastRead;
    UINT64 m_nextCompare;

    static BOOL Initialize  ();
    static BOOL Uninitialize();
    static UINT64 CounterValue();
    static void SetCompareValue( UINT64 CompareValue );
    static INT64 TicksToTime( UINT64 Ticks );
    static INT64 CurrentTime();
    static void Sleep_uSec( UINT32 uSec );
    static void Sleep_uSec_Loop( UINT32 uSec );
    static void ISR( void* Param );
};

extern Time_Driver g_Time_Driver;

#endif //__NETMF_TIME_DRIVER__
