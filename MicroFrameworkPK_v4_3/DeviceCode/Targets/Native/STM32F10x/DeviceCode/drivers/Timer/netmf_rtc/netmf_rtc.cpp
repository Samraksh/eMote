#include "netmf_rtc.h"
#include <stm32f10x.h>
#include <Samraksh/VirtualTimer.h>
#include <pwr/netmf_pwr.h>

STM32F10x_RTC g_STM32F10x_RTC;

void ISR_RTC_ALARM(void* Param);

//const uint16_t TIME_CUSHION = (uint16_t)(0.000015 * g_HardwareTimerFrequency[0]); // 15us @ 8 MHz
const UINT64 TIME_CUSHION = 7;

UINT32 STM32F10x_RTC::SetCounter(UINT32 counterValue)
{
	currentCounterValue = counterValue;
	RTC_EnterConfigMode();
	RTC_SetCounter(currentCounterValue);
	RTC_ExitConfigMode();
	return currentCounterValue;
}

UINT32 STM32F10x_RTC::GetCounter()
{
	return RTC_GetCounter();
}

UINT64 STM32F10x_RTC::Get64Counter()
{
	GLOBAL_LOCK(irq);
	UINT32 currentValue = RTC_GetCounter();

	if (prevCounterValue > currentValue){
		g_STM32F10x_RTC.m_systemTime += (0x1ull <<32);
	}
	prevCounterValue = currentValue;
	/*RTC_WaitForLastTask();
	RTC_WaitForSynchro();
	if(RTC_GetFlagStatus(RTC_FLAG_OW))
	{
		RTC_ClearFlag(RTC_FLAG_OW);

		// An overflow just happened, updating variable that holds system time
		g_STM32F10x_RTC.m_systemTime += (0x1ull <<32);
		RTC_WaitForLastTask();
		RTC_WaitForSynchro();
		currentValue = RTC_GetCounter();
	}*/

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentValue;

	/*UINT64 firstReading = m_systemTime;

	currentValue = RTC_GetCounter();

	if(RTC_GetFlagStatus(RTC_FLAG_OW))
	{
		RTC_ClearFlag(RTC_FLAG_OW);

		// An overflow just happened, updating variable that holds system time
		g_STM32F10x_RTC.m_systemTime += (0x1ull <<32);
	}

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentValue;

	if ((m_systemTime > (firstReading + 3))|| (m_systemTime < firstReading)){
		//hal_printf("\r\nrtc first: %llu cur: %llu\r\n",firstReading,m_systemTime);
		currentValue = RTC_GetCounter();

		m_systemTime &= (0xFFFFFFFF00000000ull);
		m_systemTime |= currentValue;
		return m_systemTime;
	}
	else */
		
	return m_systemTime;
}

// Initialize the advanced timer system. This involves initializing timer1 as a master timer and tim2 as a slave
// and using timer1 as a prescaler to timer2.
DeviceStatus STM32F10x_RTC::Initialize(UINT32 Prescaler, HAL_CALLBACK_FPN ISR, UINT32 ISR_Param)
{
	// Return if already initialized
	if(STM32F10x_RTC::initialized)
		return DS_Success;

	m_systemTime = 0;
	savedCompare = 0;

	STM32F10x_RTC::initialized = TRUE;

	// Maintains the last recorded 32 bit counter value
	setCompareRunning = false;

	callBackISR = ISR;
	callBackISR_Param = ISR_Param;

	int err  = 0;

	// First make sure the RTC is running, we will use that to measure.
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_SetPrescaler(1);
#if defined(PLATFORM_ARM_WLN) || defined(PLATFORM_ARM_AUSTERE)
	RCC_LSEConfig(RCC_LSE_Bypass);
#else
	RCC_LSEConfig(RCC_LSE_ON);
#endif
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
		if (err++ == PWR_RTC_TIMEOUT) {
			return DS_Fail; // Crystal not starting. Give up.
		}
	}



	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForLastTask();
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RCC_LSICmd(DISABLE);

	RTC_EnterConfigMode();
	RTC_ITConfig(RTC_IT_ALR, ENABLE);
	if( !CPU_INTC_ActivateInterrupt(RTC_IRQn, ISR_RTC_ALARM, NULL) )
		return DS_Fail;
#ifdef REBOOT_RTC_RESET_ZERO
	RTC_WaitForLastTask();
	RTC_SetCounter(0);
	RTC_WaitForLastTask();
#endif
	RTC_ExitConfigMode();
	//PWR_BackupAccessCmd(DISABLE);

    return DS_Success;

}

DeviceStatus STM32F10x_RTC::UnInitialize()
{
    callBackISR = NULL;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RCC_RTCCLKCmd(DISABLE);
	PWR_BackupAccessCmd(DISABLE);

    return DS_Success;
}

// Set compare happens in two stages, the first stage involves setting the msb on tim2
// the second stage involves lsb on tim1
DeviceStatus STM32F10x_RTC::SetCompare(UINT64 compareValue)
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
	RTC_WaitForLastTask();
	RTC_WaitForSynchro();
	RTC_ClearFlag(RTC_FLAG_ALR);
	savedCompare = compareValue&0xFFFFFFFF;
	RTC_SetAlarm(savedCompare);
	//PWR_BackupAccessCmd(DISABLE);
	RTC_WaitForLastTask();

	setCompareRunning = true;

	return DS_Success;
}

UINT32 STM32F10x_RTC::GetCompare(void) {
	return savedCompare;
}

UINT32 STM32F10x_RTC::GetMaxTicks()
{
	return (UINT32)0xFFFFFFFF;
}

// NOTE: This is apparently not actually the RTC_Alarm IRQ, but rather the general RTC_IRQ --NPS
void ISR_RTC_ALARM(void* Param){
	// TODO: check for overflow
	RTC_ClearFlag(RTC_FLAG_ALR); // Magical. I don't know why we need this... --NPS
	GLOBAL_LOCK(irq);
	g_STM32F10x_RTC.setCompareRunning = false; // Reset
	g_STM32F10x_RTC.savedCompare = 0;
	irq.Release();
	g_STM32F10x_RTC.callBackISR(&g_STM32F10x_RTC.callBackISR_Param);
}

