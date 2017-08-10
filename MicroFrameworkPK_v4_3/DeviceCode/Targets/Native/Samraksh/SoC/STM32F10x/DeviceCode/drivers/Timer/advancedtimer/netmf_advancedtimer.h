#ifndef _NETMF_ADVANCEDTIMER_H_
#define _NETMF_ADVANCEDTIMER_H_

#include <tinyhal.h>
#include <stm32f10x.h>
//#include <intc/STM32.h>
#include <Samraksh/Hal_util.h>

enum TimerClockRate
{
	EightMHz,
	FourtyEightMHz,
	OneMHz
};

struct STM32F10x_Timer_Configuration
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
		systemClockHz = 48000000;
		systemCycleClockHz = systemClockHz;
		clockCommonFactor = 1000000;
		slowClocksPerSecond = 48000000;
		slowClocksTenMhzGcd = 2000000;
		slowClocksMillisecondGcd = 1000;
		ratio1 = (slowClocksPerSecond/slowClocksTenMhzGcd);
		ratio2 = (slowClocksPerSecond/slowClocksMillisecondGcd);
		ratio3 = (systemCycleClockHz/clockCommonFactor);
		ratio4 = (slowClocksPerSecond / ONE_MHZ);
	}

};

class STM32F10x_AdvancedTimer
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

	// Stores the current active compare value in the system
	bool setCompareRunning;

	HAL_CALLBACK_FPN callBackISR;
	UINT32 callBackISR_Param;

	// Not exposing the ability to modify the timers involved because it involved knowledge of internal workings
	// The advanced timer combination will always be TIM1 -> TIM2.
	DeviceStatus Initialize(UINT32 Prescaler, HAL_CALLBACK_FPN ISR, UINT32 ISR_Param);

	DeviceStatus SetCompare(UINT64 compareValue);

	UINT32 GetCompare();
	UINT32 GetCounter();
	UINT32 SetCounter(UINT32 counterValue);

	void TriggerSoftwareInterrupt()
	{
		//NVIC->ISPR[STM32_AITC::c_IRQ_INDEX_TIM1_CC >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_TIM1_CC & (UINT8)0x1F);
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

BOOL STM32F10x_AdvancedTimer::initialized = FALSE;

// Profile delays related to setting the advanced timer comparison.
#if defined(DEBUG_EMOTE_ADVTIME)
extern volatile UINT64 badSetComparesCount;
extern volatile UINT64 badSetComparesAvg;
extern volatile UINT64 badSetComparesMax;
#endif

#endif
