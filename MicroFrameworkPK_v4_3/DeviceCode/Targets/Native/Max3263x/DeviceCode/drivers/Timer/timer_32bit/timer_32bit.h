#ifndef _NETMF_ADVANCEDTIMER_H_
#define _NETMF_ADVANCEDTIMER_H_

#include <tinyhal.h>
#include <max3263x.h>
#include <Samraksh/Hal_util.h>
#include <tmr.h>

class Max3263x_timer_32bit
{
	public:
		UINT64 m_systemTime;
		static BOOL initialized;
		UINT32 currentCounterValue;
		HAL_CALLBACK_FPN callBackISR;
		UINT32 callBackISR_Param;
		mxc_tmr_regs_t* tmrSystemTime;
		mxc_tmr_regs_t* tmrOneShot;
		// Stores the current active compare value in the system
	bool setCompareRunning;

		// Not exposing the ability to modify the timers involved because it involved knowledge of internal workings
	// The advanced timer combination will always be TIM1 -> TIM2.
	DeviceStatus InitializeOneShot(HAL_CALLBACK_FPN ISR, UINT32 ISR_Param);
	DeviceStatus InitializeSystemTime();

	DeviceStatus SetCompare(UINT64 compareValue);

	UINT32 GetCompare();
	UINT32 GetCounter();
	UINT32 SetCounter(UINT32 counterValue);
		BOOL AddTicks(UINT64 ticksToAdd);
		UINT64 Get64Counter();
		DeviceStatus UnInitialize();
		UINT32 GetMaxTicks();
	// Stores the last read value of the counter
/*	UINT16 m_clockRate;
	UINT16 m_prescalar;
	//LOCK timerLock;

public:
	
	UINT16 tar_lower;
	volatile UINT64 currentTarget;
	tmr_prescale_t prescale = TMR_PRESCALE_DIV_2_12;

	void TriggerSoftwareInterrupt()
	{
		TIM1->EGR |= (1 << 2);
	}

	
	BOOL DidCompareHit();
	BOOL DidTimerOverflow();
	void ClearTimerOverflow();
	BOOL ResetCompareHit();

*/
};

BOOL Max3263x_timer_32bit::initialized = FALSE;

// Profile delays related to setting the advanced timer comparison.
#if defined(DEBUG_EMOTE_ADVTIME)
extern volatile UINT64 badSetComparesCount;
extern volatile UINT64 badSetComparesAvg;
extern volatile UINT64 badSetComparesMax;
#endif

#endif
