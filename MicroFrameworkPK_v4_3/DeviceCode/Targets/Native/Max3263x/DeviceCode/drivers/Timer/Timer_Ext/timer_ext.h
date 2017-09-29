#ifndef _NETMF_TIMER_EXT_H_
#define _NETMF_TIMER_EXT_H_

#include <tinyhal.h>
#include <max3263x.h>
#include <Samraksh/Hal_util.h>
#include <tmr.h>

//IMPORTANT:: This implementation is not meant to be used directly from a VirtualTimer. This is an EXTENSION, not a full timer.

//This timer does not keep time, only timer, that is we done care about overflow. As long as the hardware supports atleast 16-bits we are good.
//This is meant to be used as an extension to a existing timer, to improve its precision.
//i.e., if you have a timer that runs at say 4Khz, and say another clock that runs at 32Khz, you can use the 32Khz clock with
//this class to improve the 4Khz clocks percision to make it look like 32Khz. so to go from 4 to 32 you would need
// 2^3 or 3 bits extension time and would instantiate this with 3 as template parameter.
//Note all of the clocks mentions is after prescaler, the actual rate at which the timer count increases.


//Also, because it is a timer interface, it accepts only delays, not ticks.
//template <UINT8 ext_bits>
class Max3263x_Timer_Ext
{
	//UINT64 m_systemTime;
	static BOOL initialized;
	UINT16 currentCounterValue;
	//mxc_tmr_regs_t* tmrSystemTime;
	mxc_tmr_regs_t* tmrCounter;
	// Stores the current active compare value in the system
	UINT16 m_clockRate;

public:
	bool setCompareRunning;
	HAL_CALLBACK_FPN callBackISR;
	UINT32 callBackISR_Param;

	bool Initialize(HAL_CALLBACK_FPN ISR, UINT32 ISR_Param);
	//bool InitializeSystemTime();

	bool SetCompare(UINT16 compareValue);
	UINT16 GetCompare();
	UINT16 GetCounter();
	UINT16 SetCounter(UINT16 counterValue);

	//BOOL AddTicks(UINT16 ticksToAdd);
	//UINT64 Get64Counter();
	bool UnInitialize();
	UINT16 GetMaxTicks(){ return (UINT16)0xFFFF;}

};

BOOL Max3263x_timer_8bit::initialized = FALSE;

#endif //_NETMF_TIMER_EXT_H_
