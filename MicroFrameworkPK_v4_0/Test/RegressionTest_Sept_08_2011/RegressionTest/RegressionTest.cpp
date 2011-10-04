////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// Author: Mukundan Sridharan
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <tinyhal.h>
//#include <Tests.h>
#include "RegressionTest.h"
#include <led/stm32f10x_led.h>
#include <TestSuite/TimerTest/TimerTest.h>

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
    BOOL result;
    //RAM         RamTest    ( (UINT32*)RAMTestBase, (UINT32)RAMTestSize, (ENDIAN_TYPE)ENDIANESS, BUS_WIDTH );
    //TimedEvents eventsTest;
    //UART        usartTest  ( COMTestPort, 9600, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
   // GPIO        gpioTest   ( GPIOTestPin );
    //SPI         spiTest    ( SPIChipSelect, SPIModule, g_EEPROM_STM95x );
    TimerTest      timersTest ( DisplayInterval, TimerDuration );

    do
    {   
        //result = RamTest.Execute   ( STREAM__OUT );       
        //result = eventsTest.Execute( STREAM__OUT );
        //result = usartTest.Execute ( STREAM__OUT );
       // result = gpioTest.Execute2  ( );
        //result = spiTest.Execute   ( STREAM__OUT );
        result = timersTest.Execute( STREAM__OUT );

    } while(FALSE); // run only once!


    uint32_t ik;
	if(result) {
		LED_GREEN();
		for(ik = 0; ik < 100000; ik++);
	} else {
		LED_RED();
	}
	//LED_WARNING();
	//LED_BLUE();
    while(TRUE);
}
