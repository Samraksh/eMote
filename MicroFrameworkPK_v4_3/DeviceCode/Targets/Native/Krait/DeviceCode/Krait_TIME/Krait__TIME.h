/*
 * Krait__TIMER.h
 *
 *  Created on: Sep 14, 2012
 *      Author: Mukundan Sridharan
 */

#ifndef KRAIT__TIME_H_
#define KRAIT__TIME_H_

BOOL Initialize  ();
BOOL Uninitialize();
INT64 TicksToTime( UINT64 Ticks );
INT64 CurrentTime();
UINT64 TimeNow();
void SetCompareValue( UINT64 CompareValue );
void Sleep_uSec( UINT32 uSec );
void Sleep_uSec_Loop( UINT32 uSec );

#endif /* KRAIT__TIME_H_ */
