
/*

STM32F1x Microframework power HAL driver

Nathan Stohs
nathan.stohs@samraksh.com

Based on STM32F1x standard library

Supports 2/3 of the sleep modes, all but 'off'
due to concerns about bricking the device.
Should be able to fix that with some checking later.

*/

// TODO reconcile includes with .proj contents?
#include <tinyhal.h>
#include "netmf_pwr.h"
#include "stm32f10x_pwr.h"
#include "../rcc/stm32f10x_rcc.h"
#include "../flash/stm32f10x_flash.h"
#include "../gpio/stm32f10x_gpio.h"
#include "../tim/stm32f10x_tim.h"

enum stm_power_modes {
	POWER_STATE_DEFAULT,
	POWER_STATE_LOW,
	POWER_STATE_HIGH,
};

uint32_t SystemTimerClock;

// Default state is LOW
static enum stm_power_modes stm_power_state = POWER_STATE_DEFAULT;

// Deprecated. init now takes place in bootstrap section
static void init_power() {

}

void STM32F1x_Power_Driver::Low_Power() {

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


void STM32F1x_Power_Driver::High_Power() {

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

// Deprecated
BOOL STM32F1x_Power_Driver::Initialize() {
    return TRUE;
}

// Simple "wait for interrupt" sleep
// Will wake from any source
void STM32F1x_Power_Driver::Sleep() {
#if 0
	asm volatile ("CPSIE I");
	__WFI();
#endif
	//PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
}

// Nived.Sivadas : called from CPU_Halt should stop the cpu in its tracks
// Busy wait on true is the best solution at this point
void STM32F1x_Power_Driver::Halt() {
	//asm volatile ("CPSIE I");
	//__WFI();
	while(1);
	//PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
}

// Nived.Sivadas : called from CPU_Reset should make a reset request
void STM32F1x_Power_Driver::Reset() {
   //Use the Watch dog to invoke a reset on the board
   //WatchDog_Driver::ResetCpu();
}

// TODO
void STM32F1x_Power_Driver::Shutdown() {
	return;
}

// STOP mode in STM speak
// Regulator ON vs LowPower?
// Requires EXTI interrupt to wake?
void STM32F1x_Power_Driver::Hibernate() {
	asm volatile ("CPSIE I");
	__WFI();
    //PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
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




