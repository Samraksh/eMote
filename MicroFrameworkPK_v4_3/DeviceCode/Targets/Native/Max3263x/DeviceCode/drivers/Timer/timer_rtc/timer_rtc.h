#ifndef _NETMF_RTC_H_
#define _NETMF_RTC_H_

#include <tinyhal.h>
#include <max3263x.h>
#include <Samraksh/Hal_util.h>
#include <Samraksh/Clock.h>
#include <rtc.h>

class Max3263x_timer_RTC : public HWClock_I
{
	UINT16 m_clockRate;
	UINT16 m_prescalar;

	static BOOL initialized;

	UINT16 tar_lower;
	rtc_cfg_t RTCconfig;

public:
	UINT32 currentCounterValue; 	// Stores the last read value of the counter
	UINT64 m_systemTime;

	// Stores the current active compare value in the system
	bool setCompareRunning;

	HAL_CALLBACK_FPN callBackISR;
	UINT32 callBackISR_Param;

	// Not exposing the ability to modify the timers involved because it involved knowledge of internal workings
	// The advanced timer combination will always be TIM1 -> TIM2.
	bool Initialize(HAL_CALLBACK_FPN ISR, UINT32 ISR_Param=0);

	UINT32 GetCompare();
	UINT32 GetCounter();
	UINT32 SetCounter(UINT32 counterValue);
	UINT64 Get64Counter();
	UINT32 GetMaxTicks();

	//bool Initialize ();
	bool UnInitialize ();
	ClockAttributes GetAttributes ();
	UINT64 CurrentTicks ();
	//UINT64 TicksToTime (UINT64 Ticks);
	//bool SetCallback (ClockCallbackType callback);
	//UINT64 CurrentTime ( );
	bool SetCompare (UINT64 CompareValue_ticks, bool callback=FALSE);
	/// correct-time = (raw-time * b + c) / a. b is multiplication factor, a is the divisor and c is offset (if any).
	/// maxDrift is the maximum uncompensated drift from manufacturer in parts per million, set to 0 if unknown.
	void GetDriftParameters(INT32* a, INT32* b, INT64* c, UINT32 *maxDrift );
};

BOOL Max3263x_timer_RTC::initialized = FALSE;

#endif
