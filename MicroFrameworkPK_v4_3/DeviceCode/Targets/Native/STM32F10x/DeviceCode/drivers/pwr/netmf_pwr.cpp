
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
	POWER_STATE_LOW,
	POWER_STATE_HIGH,
};

// Default state is LOW
static enum stm_power_modes stm_power_state = POWER_STATE_LOW;

// Deprecated. init now takes place in bootstrap section
static void init_power() {

}

void STM32F1x_Power_Driver::Low_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_LOW) {
		return;
	}

	__disable_irq();

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
	TIM1->PSC = 0;
	TIM2->PSC = 0;
	TIM_GenerateEvent(TIM1, TIM_EventSource_Update);
	TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	// Set flash speeds
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_0);

	stm_power_state = POWER_STATE_LOW;
	SystemCoreClock = 8000000;
	SystemTimerClock = 8000000;

	__enable_irq();
}


void STM32F1x_Power_Driver::High_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_HIGH) {
		return;
	}
	
	__disable_irq();

	// Setup PLL for 8/2*12 = 48 MHz
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
	
	// Set timer prescaler for constant 8 MHz
	// Very not tested
	TIM1->PSC = 2;
	TIM2->PSC = 2;
	TIM_GenerateEvent(TIM1, TIM_EventSource_Update);
	TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	// Set Bus Speeds
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  // 48 MHz
	RCC_PCLK1Config(RCC_HCLK_Div2);   // 24 MHz
	RCC_PCLK2Config(RCC_HCLK_Div2);   // 24 MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div4); // 6 MHz

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
	SystemCoreClock = 48000000;
	SystemTimerClock = 8000000; // I hope...

	__enable_irq();
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
	return;
}




