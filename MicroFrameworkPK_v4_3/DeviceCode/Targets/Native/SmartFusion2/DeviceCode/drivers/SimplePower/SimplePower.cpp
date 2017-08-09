
/*

STM32F1x Microframework power HAL driver

Nathan Stohs
nathan.stohs@samraksh.com

UPDATE:
SimplePower for TinyBooter.
Cuts out a lot of junk, but the real point is to create isolated and stable driver for TB.

*/

#include "SimplePower.h"

static enum stm_power_modes stm_power_state = POWER_STATE_DEFAULT;
static const int PWR_HSI_DEFAULT_TRIM = 16;

void PowerInit() {

	//DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STANDBY | DBGMCU_STOP, ENABLE);

	RCC_AdjustHSICalibrationValue(PWR_HSI_DEFAULT_TRIM); // Load default HSI setting
	RCC_LSICmd(DISABLE); // I think its already off anyway... --NPS

	// Choose default power mode here. I choose you, Low_Power()!
	Low_Power(); // IF YOU CHANGE THIS REMEMBER SIMPLETIMER --NPS
}


// Block and pause until clock-sensitive peripherals are ready.
// Add to this list as needed.
static void pause_peripherals(void) {
	// Nothing needed for TB
}

static void restart_peripherals(void) {
	// Nothing needed for TB
}

void Low_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_LOW) {
		return;
	}

	GLOBAL_LOCK(irq);

	pause_peripherals();

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
	FLASH_SetLatency(FLASH_Latency_0);
	//FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Disable); // Can't touch this in TinyBooter, breaks backwards compatibility.
	//FLASH_HalfCycleAccessCmd(FLASH_HalfCycleAccess_Enable); // Can't touch this in TinyBooter, breaks backwards compatibility.

	stm_power_state = POWER_STATE_LOW;

	restart_peripherals();
}

void Mid_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_MID) {
		return;
	}

	GLOBAL_LOCK(irq);

	pause_peripherals();

	// Set HSI (instead of PLL) as SYSCLK source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	while ( RCC_GetSYSCLKSource() != 0x00 ) { ; }
	RCC_PLLCmd(DISABLE);

	// Setup PLL for 8/2*6 = 24 MHz
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_6);

	// Set timer prescaler for constant 8 MHz
	// Only TIM1 needed, TIM2 is slave
	// PCLK @ 24 MHz, TIM runs x1, so /3
	TIM_PrescalerConfig(TIM1, 2, TIM_PSCReloadMode_Immediate);

	// Set Bus Speeds
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  // 24 MHz
	RCC_PCLK1Config(RCC_HCLK_Div1);   // 24 MHz
	RCC_PCLK2Config(RCC_HCLK_Div1);   // 24 MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div4); // 6 MHz

	// This is confusing, pay attention:
	// If the PCLK1/2 prescaler != 1
	// Then the associated TIM clock is x2
	// so PCLK @ 48/2 MHz really means TIM @ 48 MHz.

	// Set flash speeds
	FLASH_SetLatency(FLASH_Latency_0);
	FLASH_HalfCycleAccessCmd(FLASH_HalfCycleAccess_Disable);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	// Enable PLL and spin waiting for PLL ready
	RCC_PLLCmd(ENABLE);
	while ( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET ) { ; }

	// Set PLL as clock source and spin waiting for active
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while ( RCC_GetSYSCLKSource() != 0x08 ) { ; }

	stm_power_state = POWER_STATE_MID;

	restart_peripherals();
}

void High_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_HIGH) {
		return;
	}

	GLOBAL_LOCK(irq);

	pause_peripherals();

	// Set HSI (instead of PLL) as SYSCLK source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	while ( RCC_GetSYSCLKSource() != 0x00 ) { ; }
	RCC_PLLCmd(DISABLE);

	// Setup PLL for 8/2*16 = 64 MHz
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);

	// Set timer prescaler for constant 8 MHz
	// Only TIM1 needed, TIM2 is slave
	// PCLK @ 32 MHz, TIM runs x2, so /8
	TIM_PrescalerConfig(TIM1, 7, TIM_PSCReloadMode_Immediate);

	// Set Bus Speeds
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  // 64 MHz
	RCC_PCLK1Config(RCC_HCLK_Div2);   // 32 MHz
	RCC_PCLK2Config(RCC_HCLK_Div2);   // 32 MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 10.67 MHz

	// This is confusing, pay attention:
	// If the PCLK1/2 prescaler != 1
	// Then the associated TIM clock is x2
	// so PCLK @ 48/2 MHz really means TIM @ 48 MHz.

	// Set flash speeds
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_HalfCycleAccessCmd(FLASH_HalfCycleAccess_Disable);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	// Enable PLL and spin waiting for PLL ready
	RCC_PLLCmd(ENABLE);
	while ( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET ) { ; }

	// Set PLL as clock source and spin waiting for active
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while ( RCC_GetSYSCLKSource() != 0x08 ) { ; }

	stm_power_state = POWER_STATE_HIGH;

	restart_peripherals();
}

// Note: This is never called
void Sleep() {
	__DSB();
	__WFI();
}

// Shouldn't be used, possibly for unrecoverable error in debug mode.
void Halt() {
    SOFT_BREAKPOINT();
	while(1);
}

void Reset() {
	__disable_irq();
	NVIC_SystemReset();
	while(1);
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
