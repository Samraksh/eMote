/*
 * Krait__TIMER.h
 *
 *  Created on: Sep 14, 2012
 *      Author: Mukundan Sridharan
 */

#ifndef KRAIT__TIME_H_
#define KRAIT__TIME_H_

#include "..\Krait_TIMER\Krait__TIMER.h"


struct Krait_TIME_Driver
{
	// Need to confirm this number
	// Setting this to zero to resolve warning, otherwise not sure -NPS
    static const UINT32 c_OverflowCheck = 0 /*(1 << 32)*/;

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

private:
    static void ISR( void* Param );
};

//extern Krait_TIME_Driver g_Krait_TIME_Driver;

#endif /* KRAIT__TIME_H_ */
