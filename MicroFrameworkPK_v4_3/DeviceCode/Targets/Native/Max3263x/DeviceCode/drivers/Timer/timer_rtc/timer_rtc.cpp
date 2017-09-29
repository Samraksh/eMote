#include "timer_rtc.h"
//#include <Samraksh/VirtualTimer.h>
#include <pwr/netmf_pwr.h>


Max3263x_timer_RTC g_TimerRTC_Driver;
#define ONE_SEC 4096
#define COMPARE_INDEX 0

void ISR_RTC_ALARM(void* Param);
void ISR_RTC_OVERFLOW(void* Param);

//const uint16_t TIME_CUSHION = (uint16_t)(0.000015 * g_HardwareTimerFrequency[0]); // 15us @ 8 MHz
const UINT64 TIME_CUSHION = 5;

UINT32 Max3263x_timer_RTC::SetCounter(UINT32 counterValue)
{
	currentCounterValue = counterValue;
	RTC_SetCount(currentCounterValue);
	return currentCounterValue;
}

UINT32 Max3263x_timer_RTC::GetCounter()
{
	currentCounterValue = RTC_GetCount();
	return currentCounterValue;
}

UINT64 Max3263x_timer_RTC::Get64Counter()
{
	GLOBAL_LOCK(irq);
	currentCounterValue = RTC_GetCount();

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentCounterValue;

		
	return m_systemTime;
}

// Initialize the advanced timer system. This involves initializing timer1 as a master timer and tim2 as a slave
// and using timer1 as a prescaler to timer2.
bool Max3263x_timer_RTC::Initialize(HAL_CALLBACK_FPN ISR, UINT32 ISR_Param)
{
	// Return if already initialized
	if(Max3263x_timer_RTC::initialized)
		return TRUE;

	m_systemTime = 0;
	m_clockRate = 4 * 1024; //4096 Hz/

	// Maintains the last recorded 32 bit counter value
	setCompareRunning = false;

	callBackISR = ISR;
	callBackISR_Param = ISR_Param;

	int err  = 0;

	//set RTC configuration
	 RTCconfig.compareCount[0] = 0;
	 RTCconfig.compareCount[1] = 0;
	 RTCconfig.prescaler = RTC_PRESCALE_DIV_2_0;     //4kHz clock
	 RTCconfig.prescalerMask = RTC_PRESCALE_DIV_2_0;
	 RTCconfig.snoozeCount = 0;
	 RTCconfig.snoozeMode = RTC_SNOOZE_DISABLE;

	if(RTC_Init(&RTCconfig)!=E_NO_ERROR){
		return false;
	}

	//RTC_EnableINT(MXC_F_RTC_INTEN_COMP0 | MXC_F_RTC_INTEN_OVERFLOW);
	RTC_EnableINT(MXC_F_RTC_INTEN_OVERFLOW);

	if( !CPU_INTC_ActivateInterrupt(RTC3_IRQn, ISR_RTC_OVERFLOW, NULL) )
		return false;

	if( !CPU_INTC_ActivateInterrupt(RTC0_IRQn, ISR_RTC_ALARM, NULL) )
		return false;

	//if( !CPU_INTC_ActivateInterrupt(RTC2_IRQn, ISR_RTC_PRESCALER_CMP, NULL) )
	//		return false;

	//start RTC
	RTC_Start();

	Max3263x_timer_RTC::initialized = TRUE;

    return TRUE;

}

bool Max3263x_timer_RTC::UnInitialize()
{
    callBackISR = NULL;
    RTC_Stop();
    return TRUE;
}

// Set compare happens in two stages, the first stage involves setting the msb on tim2
// the second stage involves lsb on tim1
bool Max3263x_timer_RTC::SetCompare ( UINT64 compareValue, bool callback)
{
	//uint16_t tar_upper;
	//uint16_t now_upper;
	//uint16_t now_lower;

	volatile UINT64 now=0;

	GLOBAL_LOCK(irq);

	while (RTC_GetActiveTrans()){}

	//RTC_Stop(); //stop counting for a bit

	now = Get64Counter();
	//clear flag
	COMPARE_INDEX == 0 ? RTC_ClearFlags(MXC_F_RTC_FLAGS_COMP0) : RTC_ClearFlags(MXC_F_RTC_FLAGS_COMP1);

	if (compareValue < (now + TIME_CUSHION)){
		compareValue = (now + TIME_CUSHION);
	}

	volatile uint32_t cval = compareValue & 0x00000000FFFFFFFFull;
	if(RTC_SetCompare(COMPARE_INDEX, cval)!=E_NO_ERROR){
		return FALSE;
	}
	RTC_EnableINT(MXC_F_RTC_INTEN_COMP0 );

	//now = Get64Counter();

	//start RTC, needs to be restarted each time after compare is set
	//RTC_Start();
	while (RTC_GetActiveTrans()){}
	setCompareRunning = true;
	return TRUE;
}

UINT32 Max3263x_timer_RTC::GetMaxTicks()
{
	return (UINT32)0xFFFFFFFF;
}

void ISR_RTC_ALARM(void* Param){
	RTC_DisableINT(MXC_F_RTC_INTEN_COMP0);
	RTC_ClearFlags(MXC_F_RTC_FLAGS_COMP0);

	g_TimerRTC_Driver.setCompareRunning = false; // Reset
	g_TimerRTC_Driver.callBackISR(&g_TimerRTC_Driver.callBackISR_Param);
}

void ISR_RTC_OVERFLOW(void* Param){
	RTC_DisableINT(MXC_F_RTC_INTEN_OVERFLOW);
	//Do we really need to check if OVERFLOW flag is set this? An interrupt just happened. so why check?
	//if(RTC_GetFlags() & MXC_F_RTC_FLAGS_OVERFLOW)
	{
		RTC_ClearFlags(MXC_F_RTC_FLAGS_OVERFLOW );

		// An overflow just happened, updating variable that holds system time
		g_TimerRTC_Driver.m_systemTime += (0x1ull <<32);

		while (RTC_GetActiveTrans()){}
		g_TimerRTC_Driver.currentCounterValue = RTC_GetCount();
	}
	RTC_EnableINT(MXC_F_RTC_INTEN_OVERFLOW);
}

//below not used
void ISR_RTC_PRESCALAR_CMP(void* Param){
	//clear flag
	RTC_ClearFlags(MXC_F_RTC_FLAGS_PRESCALE_COMP);
}
