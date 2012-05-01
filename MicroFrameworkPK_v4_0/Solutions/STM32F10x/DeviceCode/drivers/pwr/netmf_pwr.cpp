
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


static void init_power() {
	RCC_DeInit();
	PWR_DeInit();
	RCC_HSEConfig(RCC_HSE_OFF);
	RCC_LSEConfig(RCC_LSE_ON);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RCC_LSICmd(DISABLE);
	STM32F1x_Power_Driver::Low_Power();
}

void STM32F1x_Power_Driver::Low_Power() {
	RCC_HSICmd(ENABLE);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	RCC_PLLCmd(DISABLE);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
}

void STM32F1x_Power_Driver::High_Power() {

	// Enable low mode and disable PLL before configure
	STM32F1x_Power_Driver::Low_Power();

	// Set source to HSI/2 * PLL_MUL_12 = 48 MHz
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_12);

	RCC_PCLK1Config(RCC_HCLK_Div2);  // 36 MHz Max, must divide by 2
	RCC_PCLK2Config(RCC_HCLK_Div1);  // 72 MHz Max, 48 MHz ok.

	RCC_PLLCmd(ENABLE);				 // Enable PLL
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // Set PLL as clock source
}

BOOL STM32F1x_Power_Driver::Initialize() {
	GPIO_InitTypeDef gpio_b_itd;
	GPIO_InitTypeDef gpio_g_itd;
	
	init_power();
	
	// The NOR chip must be explictly powered on
	// and then put to sleep else it will 
	// dump current (30+ mA!) in some undefined state
	
	// Enable 3.3v regulator for NOR
	GPIO_StructInit(&gpio_b_itd);
	gpio_b_itd.GPIO_Pin = GPIO_Pin_9;
	gpio_b_itd.GPIO_Speed = GPIO_Speed_2MHz;
	gpio_b_itd.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &gpio_b_itd);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	
	// Random init in between to take up time
    CPU_INTC_Initialize();
	
	// Set NOR 'Live' pin to 0 for sleep mode
	GPIO_StructInit(&gpio_g_itd);
	gpio_g_itd.GPIO_Pin = GPIO_Pin_7;
	gpio_g_itd.GPIO_Speed = GPIO_Speed_2MHz;
	gpio_g_itd.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOG, &gpio_g_itd);
	GPIO_ResetBits(GPIOG, GPIO_Pin_7);
	
	// Wait a few cycles
	{ __ASM volatile ("nop"); }
	{ __ASM volatile ("nop"); }
	{ __ASM volatile ("nop"); }
	{ __ASM volatile ("nop"); }

	// Disable 3.3v regulator
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	
    return TRUE;
}

// Simple "wait for interrupt" sleep
// Will wake from any source
void STM32F1x_Power_Driver::Sleep() {
	{ __ASM volatile ("wfi"); }  
}

// Locks the CPU until power cycle... probably not a good idea Kartik...
void STM32F1x_Power_Driver::Halt() {
    GLOBAL_LOCK(irq);
    // disable all interrupt source in controller that might wake us from our slumber
    CPU_INTC_Initialize();

    { __ASM volatile ("wfi"); }  
}

// TODO
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
// Requires EXTI interrupt to wake...
void STM32F1x_Power_Driver::Hibernate() {    
    PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
}

void HAL_AssertEx() {
	return;
}




