////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __cplusplus

#include <tinyhal.h>
#include <Tests.h>
#include "nativesample.h"
#include <led/stm32f10x_led.h>
#include <lcd_basic/stm32f10x_lcd_basic.h>
#include <rs232/stm32f10x_rs232.h>
#include <usart/netmf_usart.h>

  #define MESSAGE1   "     Samraksh       "
  #define MESSAGE2   " .NET MF v4.0       "
  #define MESSAGE3   "                    "
  #define MESSAGE4   " DRIVER LIB v0.1    "
  #define MESSAGE5   " GCC BUILD          "
  #define MESSAGE6   "     on STM32F103ZE "

void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
}

HAL_DECLARE_NULL_HEAP();

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

void ApplicationEntryPoint()
{

	//while(1);
    BOOL result;
    //RAM         RamTest    ( (UINT32*)RAMTestBase, (UINT32)RAMTestSize, (ENDIAN_TYPE)ENDIANESS, BUS_WIDTH );
    //TimedEvents eventsTest;
    //UART        usartTest  ( COMTestPort, 9600, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
    //GPIO        gpioTest   ( GPIOTestPin );
    //SPI         spiTest    ( SPIChipSelect, SPIModule, g_EEPROM_STM95x );
    Timers      timersTest ( DisplayInterval, TimerDuration );

    do
    {
        //result = RamTest.Execute   ( STREAM__OUT );
        //result = eventsTest.Execute( STREAM__OUT );
        //result = usartTest.Execute ( STREAM__OUT );
        //result = gpioTest.Execute  ( STREAM__OUT );
        //result = spiTest.Execute   ( STREAM__OUT );
        result = timersTest.Execute( STREAM__OUT );

    } while(FALSE); // run only once!
	//LED_GREEN();
    while(TRUE);
}
