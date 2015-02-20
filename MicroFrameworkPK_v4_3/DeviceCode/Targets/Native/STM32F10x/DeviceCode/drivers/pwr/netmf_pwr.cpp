
/*

STM32F1x Microframework power HAL driver

Nathan Stohs
nathan.stohs@samraksh.com

*/

#include "netmf_pwr.h"

uint32_t SystemTimerClock; // Probably should get rid of this.
static volatile int pwr_hsi_clock_measure;

static enum stm_power_modes stm_power_state = POWER_STATE_DEFAULT;

// Sets up the interrupt do the measurement. Then blocks until done.
// Result is in static global pwr_hsi_clock_measure
static void do_hsi_measure() {
	pwr_hsi_clock_measure=0;
	RTC_SetAlarm(RTC_GetCounter()+1);
	while(pwr_hsi_clock_measure < 2);
}

static void align_to_rtc() {
	pwr_hsi_clock_measure=0;
	RTC_SetAlarm(RTC_GetCounter()+2);
	while(pwr_hsi_clock_measure < 2);
}

// A total hack to determine if we're in TinyCLR or TinyBooter.
// There is probably a preprocessor define somewhere but I wasted 15 minutes on it.
// If you know it please fix me. -- NPS
static BOOL Am_I_TinyBooter() {
	BOOL (*myaddr)() = &Am_I_TinyBooter;
	if ((uint32_t)myaddr < 0x8020000) {
		return TRUE;
	}
	return FALSE;
}

// Returns measured HSI speed if the calibrate function was used.
UINT32 pwr_get_hsi(void) {
#ifdef DOTNOW_HSI_CALIB
	return pwr_hsi_clock_measure;
#else
	return 0;
#endif
}

// A total hack to determine if we're in TinyCLR or TinyBooter.
// There is probably a preprocessor define somewhere but I wasted 15 minutes on it.
// If you know it please fix me. -- NPS
void PowerInit() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	if ( Am_I_TinyBooter() ) {
		High_Power();
		return;
	}

#if defined(DEBUG) // For flavors without DEBUG (e.g. Release RTM), do not artificially raise lowest power mode. But, in flavors Debug, Instrumented ...
	if(JTAG_Attached() > 0) // ... when JTAG is attached, artificially raise lowest power mode to support JTAG connection.
	{
	DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STANDBY | DBGMCU_STOP, ENABLE);
	}
	else {
#endif
	DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STANDBY | DBGMCU_STOP, DISABLE);
#if defined(DEBUG)
	}
#endif

#ifdef DOTNOW_HSI_CALIB
	Low_Power();
	CalibrateHSI();
#endif

	High_Power();
}

// Currently this is only safe to do at startup.
// Not tested for mid program execution.
void CalibrateHSI() {
	uint32_t trim;
	NVIC_InitTypeDef NVIC_InitStruct;
	uint32_t hsi_trim_error[32]; // Memoization table of absolute frequency error vs. TRIM
	uint32_t hsi_trim_val[32]; // Memoization table of absolute frequency vs TRIM

	if (SystemTimerClock != 8000000) {
		return; // Must be done from low power mode.
	}

	// for some reason need to memset this... I think due to C++ object issue
	memset(hsi_trim_error, 0xFFFFFFFF, sizeof(hsi_trim_error));

	// First make sure the RTC is running, we will use that to measure.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_SetPrescaler(0x0800); // 1/16 second
	RCC_LSEConfig(RCC_LSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET); /* Wait till LSE is ready */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForLastTask();
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RCC_LSICmd(DISABLE);

	pwr_hsi_clock_measure = 0;

	// Setup RTC interrupt
	NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	RTC_ITConfig(RTC_IT_ALR, ENABLE);

	// Setup TIM6 to do the actual timing
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_DeInit(TIM6);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);

	TIM_Cmd(TIM6, ENABLE);

	trim = BKP_ReadBackupRegister(BKP_DR1); // backup register 1 contains last good TRIM value
	if (trim == 0 || trim > 31) {
		trim = 16; // default
	}

	align_to_rtc(); // Wait 2 clocks so we are reasonably sure we are close to RTC clock edge.

	// Find trim with min error.
	// Note implicit assumptions about clock speed and TRIM relationship (not random).
	while(trim > 0 && trim < 31) {
		int min_trim = trim;
		int skip=0; // if we get close enough, skip rest of tests.

		if (hsi_trim_error[trim] == 0xFFFFFFFF && !skip) {
			RCC_AdjustHSICalibrationValue(trim);
			do_hsi_measure();
			if (pwr_hsi_clock_measure < PWR_HSI_SPEED) {
				hsi_trim_error[trim] = PWR_HSI_SPEED - pwr_hsi_clock_measure;
			}
			else {
				hsi_trim_error[trim] = pwr_hsi_clock_measure - PWR_HSI_SPEED;
			}
			hsi_trim_val[trim] = pwr_hsi_clock_measure;
			if (hsi_trim_error[trim] < PWR_HSI_CLOSE_ENOUGH) {
				skip=1;
			}
		}

		if (hsi_trim_error[trim-1] == 0xFFFFFFFF && !skip) {
			RCC_AdjustHSICalibrationValue(trim-1);
			do_hsi_measure();
			if (pwr_hsi_clock_measure < PWR_HSI_SPEED) {
				hsi_trim_error[trim-1] = PWR_HSI_SPEED - pwr_hsi_clock_measure;
			}
			else {
				hsi_trim_error[trim-1] = pwr_hsi_clock_measure - PWR_HSI_SPEED;
			}
			hsi_trim_val[trim-1] = pwr_hsi_clock_measure;
			if (hsi_trim_error[trim-1] < PWR_HSI_CLOSE_ENOUGH) {
				skip=1;
			}
		}

		if (hsi_trim_error[trim+1] == 0xFFFFFFFF && !skip) {
			RCC_AdjustHSICalibrationValue(trim+1);
			do_hsi_measure();
			if (pwr_hsi_clock_measure < PWR_HSI_SPEED) {
				hsi_trim_error[trim+1] = PWR_HSI_SPEED - pwr_hsi_clock_measure;
			}
			else {
				hsi_trim_error[trim+1] = pwr_hsi_clock_measure - PWR_HSI_SPEED;
			}
			hsi_trim_val[trim+1] = pwr_hsi_clock_measure;
			if (hsi_trim_error[trim+1] < PWR_HSI_CLOSE_ENOUGH) {
				skip=1;
			}
		}

		// Find min error
		if (hsi_trim_error[min_trim] > hsi_trim_error[trim+1]) {
			min_trim = trim+1;
		}
		else if (hsi_trim_error[min_trim] > hsi_trim_error[trim-1]) {
			min_trim = trim-1;
		}

		// Success
		if (min_trim == trim || skip || min_trim == 0 || min_trim == 31) {
			RCC_AdjustHSICalibrationValue(min_trim);
			pwr_hsi_clock_measure = hsi_trim_val[min_trim];
			BKP_WriteBackupRegister(BKP_DR1, (uint16_t)min_trim);
			trim = min_trim;
			break;
		}
		// go around and try again
		trim = min_trim;
	}

	// Clean up
	TIM_DeInit(TIM6);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE);
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStruct);
	RTC_ITConfig(RTC_IT_ALR, DISABLE);
	PWR_BackupAccessCmd(DISABLE);
	// Turn off RTC since not used currently.
	RCC_RTCCLKCmd(DISABLE);
	RCC_LSEConfig(RCC_LSE_OFF);
	// Leave PWR module ON.
	RCC_APB1PeriphClockCmd(/*RCC_APB1Periph_PWR |*/ RCC_APB1Periph_BKP, DISABLE);
}

extern "C" {

void __irq RTC_IRQHandler() {
	uint32_t time = TIM6->CNT;
	TIM6->CNT=0;
	RTC_ClearITPendingBit(RTC_IT_ALR);

	if(pwr_hsi_clock_measure == 0) { // do the measurement and come back.
		RTC_SetAlarm(RTC_GetCounter()+1);
		pwr_hsi_clock_measure=1;
		return;
	}
	else {
		pwr_hsi_clock_measure = (time << 7);
	}
}

}

void Low_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_LOW) {
		return;
	}

	GLOBAL_LOCK(irq);

	// Set HSI (instead of PLL) as SYSCLK source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

	// Spin waiting for HSI to be used.
	while ( RCC_GetSYSCLKSource() != 0x00 ) { ; }

	// Disable (now unused) PLL
	RCC_PLLCmd(DISABLE);

	// Set Bus speeds
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  // 8 MHz
	RCC_PCLK1Config(RCC_HCLK_Div1);   // 8 MHz
	RCC_PCLK2Config(RCC_HCLK_Div1);   // 8 MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div2); // 4 MHz
	
	// Set timer prescaler for constant 8 MHz
	// Very not tested
	TIM_PrescalerConfig(TIM1, 0, TIM_PSCReloadMode_Immediate);

	// Set flash speeds
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_0);

	stm_power_state = POWER_STATE_LOW;
	SystemTimerClock = 8000000;
}

void High_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_HIGH) {
		return;
	}

	GLOBAL_LOCK(irq);

	// Setup PLL for 8/2*12 = 48 MHz
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
	
	// Set timer prescaler for constant 8 MHz
	// Only TIM1 needed, TIM2 is slave
	// PCLK @ 24 MHz, TIM runs x2, so /6
	TIM_PrescalerConfig(TIM1, 5, TIM_PSCReloadMode_Immediate);

	// Set Bus Speeds
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  // 48 MHz
	RCC_PCLK1Config(RCC_HCLK_Div2);   // 24 MHz
	RCC_PCLK2Config(RCC_HCLK_Div2);   // 24 MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 4 MHz
	
	// This is confusing, pay attention:
	// If the PCLK1/2 prescaler != 1
	// Then the associated TIM clock is x2
	// so PCLK @ 48/2 MHz really means TIM @ 48 MHz.

	// Set flash speeds
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_1);

	// Enable PLL and spin waiting for PLL ready
	RCC_PLLCmd(ENABLE);
	while ( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET ) { ; }
	
	// Set PLL as clock source and spin waiting for active
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while ( RCC_GetSYSCLKSource() != 0x08 ) { ; }
	
	stm_power_state = POWER_STATE_HIGH;
	SystemTimerClock = 8000000;
}

// Exit in the same power state as we entered.
void Sleep() {

	switch(stm_power_state) {
		case POWER_STATE_LOW:
			PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
			break;
		default:
			__WFI();
			break;
	}

}

// Shouldn't be used, possibly for unrecoverable error in debug mode.
void Halt() {
    SOFT_BREAKPOINT();
	while(1);
}

void Reset() {
	__disable_irq();
	NVIC_SystemReset();
}

void Shutdown() {
    SOFT_BREAKPOINT();
	while(1) { PWR_EnterSTANDBYMode(); }
}

void HAL_AssertEx() {
/*
// leave commented out because there was no response the pull request on 2014-03-14.
#if !defined(NDEBUG)
	if(JTAG_Attached()) {
		SOFT_BREAKPOINT(); // use SOFT_BREAKPOINT() because there are too many assertions being thrown, and some assertions might be false positives due to other companies submitting junk to CodePlex.
	}
	return;
#endif
*/
}
