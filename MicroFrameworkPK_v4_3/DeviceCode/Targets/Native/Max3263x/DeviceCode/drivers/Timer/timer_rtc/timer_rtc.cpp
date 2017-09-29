#include "timer_rtc.h"
//#include <Samraksh/VirtualTimer.h>
#include <pwr/netmf_pwr.h>


Max3263x_timer_RTC g_TimerRTC_Driver;
#define ONE_SEC 4096
#define COMPARE_INDEX 0

void ISR_RTC_ALARM(void* Param);

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

	if(RTC_GetFlags() & MXC_F_RTC_CTRL_ROLLOVER_CLR_ACTIVE)
	{
		RTC_ClearFlags(MXC_F_RTC_CTRL_ROLLOVER_CLR_ACTIVE);

		// An overflow just happened, updating variable that holds system time
		g_TimerRTC_Driver.m_systemTime += (0x1ull <<32);
		//RTC_WaitForLastTask();
		//RTC_WaitForSynchro();
		currentCounterValue = RTC_GetCount();
	}

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentCounterValue;

		
	return m_systemTime;
}

// Initialize the advanced timer system. This involves initializing timer1 as a master timer and tim2 as a slave
// and using timer1 as a prescaler to timer2.
bool Max3263x_timer_RTC::Initialize(HAL_CALLBACK_FPN ISR, UINT32 ISR_Param)
{
	CPU_GPIO_EnableOutputPin(TEST_PIN4, TRUE);
	CPU_GPIO_EnableOutputPin(TEST_PIN5, TRUE);
	CPU_GPIO_EnableOutputPin(TEST_PIN6, TRUE);
	CPU_GPIO_EnableOutputPin(TEST_PIN7, TRUE);

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
	RTCconfig.compareCount[COMPARE_INDEX] = ONE_SEC; //alarm0 time in seconds
	//RTCconfig.compareCount[1] = ONE_SEC; //alarm1 time in seconds
	RTCconfig.prescaler = RTC_PRESCALE_DIV_2_0; //4KHz clock
	//RTCconfig.prescalerMask = RTC_PRESCALE_DIV_2_0;//4KHz prescaler compare
	//RTCconfig.snoozeCount = ONE_SEC * 60;//snooze time in seconds
	//RTCconfig.snoozeMode = RTC_SNOOZE_MODE_B;

	if(RTC_Init(&RTCconfig)!=E_NO_ERROR){
		return false;
	}

	//SetCounter(0);

	//setup interrupt callbacks and enable
	//NVIC_SetVector(RTC0_IRQn, RTC0_handler_Compare0);
	//NVIC_SetVector(RTC2_IRQn, RTC2_handler_PrescalerCMP);

	//RTC_EnableINT(MXC_F_RTC_INTEN_COMP0 | MXC_F_RTC_INTEN_PRESCALE_COMP);
	RTC_EnableINT(MXC_F_RTC_INTEN_COMP0 );

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

	volatile UINT64 now;

	GLOBAL_LOCK(irq);

	while (RTC_GetActiveTrans()){}

	RTC_Stop(); //stop counting for a bit
	//clear flag
	COMPARE_INDEX == 0 ? RTC_ClearFlags(MXC_F_RTC_FLAGS_COMP0) : RTC_ClearFlags(MXC_F_RTC_FLAGS_COMP1);

	if (compareValue < (now + TIME_CUSHION)){
		compareValue = (now + TIME_CUSHION);

		//fire rightway?
		//COMPARE_INDEX == 0 ? RTC_ClearFlags(MXC_F_RTC_FLAGS_COMP0) : RTC_ClearFlags(MXC_F_RTC_FLAGS_COMP1);
		//ISR_RTC_ALARM(NULL);
		//return TRUE;
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


	volatile uint32_t cval = compareValue & 0x00000000FFFFFFFFull;
	if(RTC_SetCompare(COMPARE_INDEX, cval)!=E_NO_ERROR){
		return FALSE;
	}
	RTC_EnableINT(MXC_F_RTC_INTEN_COMP0 );

	//now = Get64Counter();

	//start RTC, needs to be restarted each time after compare is set
	RTC_Start();
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
	CPU_GPIO_TogglePinState(TEST_PIN4);
	RTC_DisableINT(MXC_F_RTC_INTEN_COMP0);
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
