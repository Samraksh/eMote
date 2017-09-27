/* Author: Mukundan Sridharan
 * A interface that must be implement by all clocks to be accessed uniformly by client modules, such as
 * timers, radios, time, etc.
 */


#ifndef SAMRAKSH_CLOCK_H
#define SAMRAKSH_CLOCK_H



/* === INCLUDES ============================================================ */
#include <Tinyhal_types.h>
#include <tinyhal.h>

typedef void (*ClockCallbackType)(UINT64 Ticks);

/*enum ClockTypeE {
	RTC,
	HighSpeedCrystal,
	PLL,
};
*/

//This interface forces you to support one of the standard time units
/*
enum TimeUnitE {
	NANO,
	MICRO,
	MILLI,
	SEC
};
*/

struct ClockAttributes {
	//TimeUnitE timeUnit;
	//in hz.this means max freq supported is 4,294,967,295 or ~4.3 GHz
	UINT32 freq;
	///un compensated drift in parts per billion (ppb) should be able to get from data sheet
	UINT16 uncompensatedMaxDrift;
	//bool callback;
};

class HWClock_I {

public:
	virtual bool Initialize (UINT32 Prescaler, HAL_CALLBACK_FPN ISR, UINT32 ISR_Para )=0;
	virtual bool UnInitialize ()=0;
	virtual ClockAttributes GetAttributes ()=0;
	virtual UINT64 CurrentTicks ()=0;
	//UINT64 TicksToTime (UINT64 Ticks)=0;
	//bool SetCallback (ClockCallbackType callback)=0;
	//UINT64 CurrentTime ( )=0;
	virtual bool SetCompare  ( UINT64 CompareValue_ticks, bool callback=FALSE)=0;
	/// correct-time = (raw-time * b + c) / a. b is multiplication factor, a is the divisor and c is offset (if any).
	/// maxDrift is the maximum uncompensated drift from manufacturer in parts per million, set to 0 if unknown.
	virtual void GetDriftParameters (INT32* a, INT32* b, INT64* c, UINT32 *maxDrift )=0;
};

#endif //SAMRAKSH_CLOCK_H
