#include "timer_rtc.h"
//#include <Samraksh/VirtualTimer.h>
#include <pwr/netmf_pwr.h>


Max3263x_timer_RTC g_TimerRTC_Driver;
#define SNOOZE_SEC 5
#define COMPARE_INDEX 0

void ISR_RTC_ALARM(void* Param);

//const uint16_t TIME_CUSHION = (uint16_t)(0.000015 * g_HardwareTimerFrequency[0]); // 15us @ 8 MHz
const UINT64 TIME_CUSHION = 7;

UINT32 Max3263x_timer_RTC::SetCounter(UINT32 counterValue)
{
	currentCounterValue = counterValue;
	RTC_SetCompare(COMPARE_INDEX,currentCounterValue);
	return currentCounterValue;
}

UINT32 Max3263x_timer_RTC::GetCounter()
{
	return RTC_GetCount();
}

UINT64 Max3263x_timer_RTC::Get64Counter()
{
	GLOBAL_LOCK(irq);
	UINT32 currentValue = RTC_GetCount();

	if(RTC_GetFlags() & MXC_F_RTC_CTRL_ROLLOVER_CLR_ACTIVE)
	{
		RTC_ClearFlags(MXC_F_RTC_CTRL_ROLLOVER_CLR_ACTIVE);

		// An overflow just happened, updating variable that holds system time
		g_TimerRTC_Driver.m_systemTime += (0x1ull <<32);
		//RTC_WaitForLastTask();
		//RTC_WaitForSynchro();
		currentValue = RTC_GetCount();
	}

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentValue;

		
	return m_systemTime;
}

// Initialize the advanced timer system. This involves initializing timer1 as a master timer and tim2 as a slave
// and using timer1 as a prescaler to timer2.
bool Max3263x_timer_RTC::Initialize(UINT32 Prescaler, HAL_CALLBACK_FPN ISR, UINT32 ISR_Param)
{
	// Return if already initialized
	if(Max3263x_timer_RTC::initialized)
		return TRUE;

	m_systemTime = 0;

	Max3263x_timer_RTC::initialized = TRUE;

	// Maintains the last recorded 32 bit counter value
	setCompareRunning = false;

	callBackISR = ISR;
	callBackISR_Param = ISR_Param;

	int err  = 0;

	//set RTC configuration
	RTCconfig.compareCount[COMPARE_INDEX] = 1; //alarm0 time in seconds
	//RTCconfig.compareCount[1] = ALARM1_SEC; //alarm1 time in seconds
	RTCconfig.prescaler = RTC_PRESCALE_DIV_2_0; //4KHz clock
	RTCconfig.prescalerMask = RTC_PRESCALE_DIV_2_0;//4KHz prescaler compare
	//RTCconfig.snoozeCount = SNOOZE_SEC;//snooze time in seconds
	//RTCconfig.snoozeMode = RTC_SNOOZE_MODE_B;
	RTC_Init(&RTCconfig);

	//setup interrupt callbacks and enable
	//NVIC_SetVector(RTC0_IRQn, RTC0_handler_Compare0);
	//NVIC_SetVector(RTC2_IRQn, RTC2_handler_PrescalerCMP);

	RTC_EnableINT(MXC_F_RTC_INTEN_COMP0 | MXC_F_RTC_INTEN_PRESCALE_COMP);

	if( !CPU_INTC_ActivateInterrupt(RTC0_IRQn, ISR_RTC_ALARM, NULL) )
		return false;
	//if( !CPU_INTC_ActivateInterrupt(RTC2_IRQn, ISR_RTC_PRESCALER_CMP, NULL) )
	//		return false;

	//start RTC
	RTC_Start();

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
	uint16_t tar_upper;
	uint16_t now_upper;
	uint16_t now_lower;
	UINT64 now;

	GLOBAL_LOCK(irq);
	now = Get64Counter();
	
	if (compareValue < (now + TIME_CUSHION)){
		compareValue = (now + TIME_CUSHION);
	}

	// RTC hardware needs to wait for synchronization at times.
	//PWR_BackupAccessCmd(ENABLE);
	/*RTC_WaitForLastTask();
	RTC_WaitForSynchro();
	RTC_ClearFlag(RTC_FLAG_ALR);
	RTC_SetAlarm(compareValue);
	//PWR_BackupAccessCmd(DISABLE);
	RTC_WaitForLastTask();
	*/
	//Wait for no RTC transactions
	while (RTC_GetActiveTrans()){}

	//clear flag
	COMPARE_INDEX == 0 ? RTC_ClearFlags(MXC_F_RTC_FLAGS_COMP0) : RTC_ClearFlags(MXC_F_RTC_FLAGS_COMP1);

	uint32_t cval = compareValue & 0x00000000FFFFFFFF;
	if(RTC_SetCompare(COMPARE_INDEX, cval)!=E_NO_ERROR){
		return FALSE;
	}

	setCompareRunning = true;
	return TRUE;
}

UINT32 Max3263x_timer_RTC::GetMaxTicks()
{
	return (UINT32)0xFFFFFFFF;
}

void ISR_RTC_ALARM(void* Param){
	// TODO: check for overflow
	//clear flag
	RTC_ClearFlags(MXC_F_RTC_FLAGS_COMP0);
	//PWR_BackupAccessCmd(ENABLE);
	//RTC_WaitForLastTask();

	//PWR_BackupAccessCmd(DISABLE);
	g_TimerRTC_Driver.setCompareRunning = false; // Reset
	g_TimerRTC_Driver.callBackISR(&g_TimerRTC_Driver.callBackISR_Param);
}


void ISR_RTC_PRESCALAR_CMP(void* Param){
	//clear flag
	RTC_ClearFlags(MXC_F_RTC_FLAGS_PRESCALE_COMP);
}
