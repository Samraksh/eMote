////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __cplusplus

#include <tinyhal.h>
#include <Tests.h>
#include "nativesample.h"
//#include <led/stm32f10x_led.h>
//#include <lcd_basic/stm32f10x_lcd_basic.h>
//#include <rs232/stm32f10x_rs232.h>
//#include <rtc/netmf_time.h>
//#include <rtc/int_time.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "_Font/RGB.h"




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

void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
	RTC_IRQHandler();
	TIM1_UP_IRQHandler();
}

HAL_DECLARE_NULL_HEAP();

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

void ApplicationEntryPoint()
{
   // LCD init
  /* LCD_Init();
   BackLight_On();
   
   // Setup Font
   Font_BGnd_Color_u16  = RGB_WHITE;
   Font_Text_Color_u16  = RGB_BLUE;
   Font_Scale_u08       = 1;
   Font_Transparent_Bol = FALSE;
   
   Font_Set_Font_u8(Font_Primer);
   
   LCD_Draw_Surface(0, 0, 128, 160, RGB_WHITE);
   Font_Scale_u08       = 2;
   LCD_Write_Text    (  8, 120, "SAMRAKSH", 50 );
   Font_Scale_u08       = 1;
   LCD_Write_Text    (  35,100, "announces", 50 );
   LCD_Write_Text    (  50, 90, "the", 50 );
   LCD_Write_Text    (  15, 80, "next generation", 50 );
   Font_Scale_u08       = 2;
   LCD_Write_Text    (  25, 50, "eMote", 50 );
   Font_Scale_u08       = 1;
   LCD_Write_Text    (  30, 30, "supporting", 50 );
   LCD_Write_Text    (  35, 15, ".NET MF", 50 );    */
   Configuration_SysTick();
   Configuration_ADC();
   Configuration_SPI();
   
   AccelSensor_Init_Sensor();
   
   /* CAN configuration */
   Configuration_CAN();
   CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
   
   // LCD init
   LCD_Init();
   BackLight_On();
   
   // Create with _Background_image/Create_background_image.xls
   LCD_background_image();
   
   // Setup Font
   Font_BGnd_Color_u16  = RGB_WHITE;
   Font_Text_Color_u16  = RGB_BLUE;
   Font_Scale_u08       = 1;
   Font_Transparent_Bol = FALSE;
   
   Font_Set_Font_u8(Font_Primer);
   
   LCD_Draw_Surface(0, 0, 128, 160, RGB_WHITE);
   Font_Scale_u08       = 2;
   LCD_Write_Text    (  8, 120, "SAMRAKSH", 50 );
   Font_Scale_u08       = 1;
   LCD_Write_Text    (  35,100, "announces", 50 );
   LCD_Write_Text    (  50, 90, "the", 50 );
   LCD_Write_Text    (  15, 80, "next generation", 50 );
   //LCD_Write_Text    (  47, 70, "mote", 50 );
   Font_Scale_u08       = 2;
   LCD_Write_Text    (  25, 50, "eMote", 50 );
   Font_Scale_u08       = 1;
   LCD_Write_Text    (  30, 30, "supporting", 50 );
   LCD_Write_Text    (  35, 15, ".NET MF", 50 );   
 
 //while(TRUE);
}
