////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __cplusplus

#include <tinyhal.h>
//#include <Tests.h>
#include "nativesample.h"
//#include <led/stm32f10x_led.h>
#include <lcd_basic/stm32f10x_lcd_basic.h>
//#include <rs232/stm32f10x_rs232.h>
//#include <rtc/netmf_time.h>
//#include <rtc/int_time.h>
#include <gpio/stm32f10x_gpio.h>
#include <rcc/stm32f10x_rcc.h>
#include <flash/stm32f10x_flash.h>
#include <misc/misc.h>
#include <stdlib.h>


//LEDs port and pins
#define LEDS_GPIO       (GPIOE)
#define LEDS_RCC_GPIO   (RCC_APB2Periph_GPIOE)
#define LED0_PIN        (GPIO_Pin_0)
#define LED1_PIN        (GPIO_Pin_1)
#define LEDS_BOTH_PINS  (LED0_PIN|LED1_PIN)

//joystick port and pins
#define JSTK_GPIO       (GPIOE)
#define JSTK_RCC_GPIO   (RCC_APB2Periph_GPIOE)
#define JSTK_LEFT_PIN   (GPIO_Pin_3)
#define JSTK_RIGHT_PIN  (GPIO_Pin_4)
#define JSTK_UP_PIN     (GPIO_Pin_5)
#define JSTK_DOWN_PIN   (GPIO_Pin_6)
#define JSTK_ALL_PINS   (JSTK_LEFT_PIN|JSTK_RIGHT_PIN|JSTK_UP_PIN|JSTK_DOWN_PIN)

//push button port and pin
#define BP_GPIO         (GPIOA)
#define BP_RCC_GPIO     (RCC_APB2Periph_GPIOA)
#define BP_PIN          (GPIO_Pin_8)


//LCD backlight enable port and pin
#define LCDBLEN_GPIO         (GPIOB)
#define LCDBLEN_RCC_GPIO     (RCC_APB2Periph_GPIOB)
#define LCDBLEN_PIN          (GPIO_Pin_8)


/* example defines ------------------------------------------------------------*/

//start, min and max for the delay variable
#define DELAYVAR_MAXVAL         (0x100000)
#define DELAYVAR_STARTVAL       (0x40000)
#define DELAYVAR_MINVAL         (0x1000)

//small and big steps for the delay variable
#define DELAYVAR_BIGCHANGE      (0x4000)
#define DELAYVAR_SMALLCHANGE    (0x1000)


  #define MESSAGE1   "     Samraksh       "
  #define MESSAGE2   " .NET MF v4.0       "
  #define MESSAGE3   "                    "
  #define MESSAGE4   " DRIVER LIB v0.1    "
  #define MESSAGE5   " GCC BUILD          "
  #define MESSAGE6   "     on STM32F103ZE "
  
/* -------- _Font/Font.c --------------------------------------------------------------*/
extern "C" void Font_Set_Font_u8( u8 Font );                // to change the font
#define     Font_7x8                      1             // available fonts
#define     Font_7x11                     2             // available fonts
#define     Font_10x16                    3             // available fonts
#define     Font_16x16                    4             // available fonts
#define     Font_Symbols                  5             // available fonts
#define     Font_BigNumber                6             // available fonts
#define     Font_Primer                   0             // available fonts
extern u8   Font_Scale_u08;                             // Scale of the selected font
extern bool Font_Transparent_Bol;                       // if true, the background image is not overwritten. Background image is "Pictures/bmp_background_image.h generate" with "Create_background_image.xls"
extern u16  Font_Text_Color_u16;                        // Text color of the selected font
extern u16  Font_BGnd_Color_u16;                        // Background color of the selected font

/*
void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
	RTC_IRQHandler();
	TIM1_UP_IRQHandler();
}
*/
//void Delay(vu32 nCount);

void Delay(vu32 nCount)
    {
    for(; nCount!= 0;nCount--);
    }


/*void RCC_Configuration(void)
    {
    // RCC system reset(for debug purpose) 
    RCC_DeInit();

    // Enable HSE 
    RCC_HSEConfig(RCC_HSE_ON);

    // Wait till HSE is ready 
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
        {;}  

    // Enable Prefetch Buffer 
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    // Flash 2 wait state 
    FLASH_SetLatency(FLASH_Latency_2);

    // HCLK = SYSCLK 
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 

    // PCLK2 = HCLK 
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    // PCLK1 = HCLK 
    RCC_PCLK1Config(RCC_HCLK_Div1);

    // Select HSE as system clock source 
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

    // Wait till HSE is used as system clock source 
    while(RCC_GetSYSCLKSource() != 0x04)
        {;}  
    }
	
	
	void NVIC_Configuration(void)
    {
#ifdef  VECT_TAB_RAM  
    // Set the Vector Table base location at 0x20000000  
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  // VECT_TAB_FLASH  
    // Set the Vector Table base location at 0x08000000  
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
    }
	
*/	
	HAL_DECLARE_NULL_HEAP();

GPIO_InitTypeDef GPIO_InitStructure;
volatile int delayvar=DELAYVAR_STARTVAL;
volatile unsigned char toggle=0;
vu16 jstkinput=0;
vu16 bpinput=0;

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

void ApplicationEntryPoint()
{


}