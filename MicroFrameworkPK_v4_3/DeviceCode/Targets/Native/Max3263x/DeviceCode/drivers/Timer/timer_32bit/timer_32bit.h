#ifndef _NETMF_ADVANCEDTIMER_H_
#define _NETMF_ADVANCEDTIMER_H_

#include <tinyhal.h>
#include <max3263x.h>
#include <Samraksh/Hal_util.h>

struct Max3263x_Timer_Configuration
{
	UINT32 systemClockHz;
	UINT32 systemCycleClockHz;
	UINT32 clockCommonFactor;
	UINT32 slowClocksPerSecond;
	UINT32 slowClocksTenMhzGcd;
	UINT32 slowClocksMillisecondGcd;
	UINT32 ratio1;
	UINT32 ratio2;
	UINT32 ratio3;
	UINT32 ratio4;

public:

	void Initialize()
	{
		systemClockHz = SYSTEM_CLOCK_HZ;
		systemCycleClockHz = systemClockHz;
		clockCommonFactor = CLOCK_COMMON_FACTOR;
		slowClocksPerSecond = SLOW_CLOCKS_PER_SECOND;
		slowClocksTenMhzGcd = SLOW_CLOCKS_TEN_MHZ_GCD;
		slowClocksMillisecondGcd = SLOW_CLOCKS_MILLISECOND_GCD;
		ratio1 = (slowClocksPerSecond/slowClocksTenMhzGcd);
		ratio2 = (slowClocksPerSecond/slowClocksMillisecondGcd);
		ratio3 = (systemCycleClockHz/clockCommonFactor);
		ratio4 = (slowClocksPerSecond / ONE_MHZ);
	}

};

class Max3263x_timer_32bit
{
	// Stores the last read value of the counter
	UINT16 m_clockRate;
	UINT16 m_prescalar;
	//LOCK timerLock;

public:
	UINT64 m_systemTime;
	static BOOL initialized;
	UINT32 currentCounterValue;
	UINT16 tar_lower;
	volatile UINT64 currentTarget;
	tmr_prescale_t prescale = TMR_PRESCALE_DIV_2_12;

	// Stores the current active compare value in the system
	bool setCompareRunning;

	HAL_CALLBACK_FPN callBackISR;
	UINT32 callBackISR_Param;

	// Not exposing the ability to modify the timers involved because it involved knowledge of internal workings
	// The advanced timer combination will always be TIM1 -> TIM2.
	DeviceStatus Initialize(HAL_CALLBACK_FPN ISR, UINT32 ISR_Param);

	DeviceStatus SetCompare(UINT64 compareValue);

	UINT32 GetCompare();
	UINT32 GetCounter();
	UINT32 SetCounter(UINT32 counterValue);

	void TriggerSoftwareInterrupt()
	{
		TIM1->EGR |= (1 << 2);
	}

	DeviceStatus UnInitialize();
	BOOL DidCompareHit();
	BOOL DidTimerOverflow();
	void ClearTimerOverflow();
	BOOL ResetCompareHit();
	UINT64 Get64Counter();
	BOOL AddTicks(UINT64 ticksToAdd);

	UINT32 GetMaxTicks();


};

BOOL Max3263x_timer_32bit::initialized = FALSE;

// Profile delays related to setting the advanced timer comparison.
#if defined(DEBUG_EMOTE_ADVTIME)
extern volatile UINT64 badSetComparesCount;
extern volatile UINT64 badSetComparesAvg;
extern volatile UINT64 badSetComparesMax;
#endif

#endif
