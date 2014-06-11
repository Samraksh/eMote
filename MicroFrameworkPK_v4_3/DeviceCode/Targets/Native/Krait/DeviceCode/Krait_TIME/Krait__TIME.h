/*
 * Krait__TIMER.h
 *
 *  Created on: Sep 14, 2012
 *      Author: Mukundan Sridharan
 */

#ifndef KRAIT__TIME_H_
#define KRAIT__TIME_H_

class Krait_Time
{
public:
	static UINT64 bigCounter;
	////static UINT64 nextCompare;

	BOOL Initialize  ();
	BOOL Uninitialize();
	INT64 CurrentTime();
	INT64 CurrentTicks();
	UINT64 TimeNow();
	INT64 TicksToTime( UINT64 Ticks );
	void SetCompareValue( UINT64 CompareValue );
	void Sleep_uSec( UINT32 uSec );
	void Sleep_uSec_Loop( UINT32 uSec );

	//static inline void flush_time();
	//static void TIME_HANDLER(void *arg);
	////void flush_time();
	////void TIME_HANDLER(void *arg);
};

//extern Krait_Time g_Krait_Time;

#endif /* KRAIT__TIME_H_ */
