#ifndef _NETMF_RTC_H_
#define _NETMF_RTC_H_

#include <tinyhal.h>
#include <max3263x.h>
//#include <intc/STM32.h>
#include <Samraksh/Hal_util.h>

class STM32F10x_RTC
{
	// Stores the last read value of the counter
	UINT16 m_clockRate;
	UINT16 m_prescalar;

public:
	UINT64 m_systemTime;
	static BOOL initialized;
	UINT32 currentCounterValue;
	UINT16 tar_lower;

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

	DeviceStatus UnInitialize();
	UINT64 Get64Counter();

	UINT32 GetMaxTicks();
};

BOOL STM32F10x_RTC::initialized = FALSE;

#endif
