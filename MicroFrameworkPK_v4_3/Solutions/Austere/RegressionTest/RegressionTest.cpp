////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// Author: Mukundan Sridharan
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <tinyhal.h>
//#include <Tests.h>
#include "RegressionTest.h"
//#include <TestSuite/TimerTest/TimerTest.h>
#include <TestSuite/LCDTest/LCDTest.h>
#include <TestSuite/UARTTest/UARTTest.h>
#include "..\..\Targets\Native\STM32F10x\DeviceCode\drivers\usart\netmf_usart.h"
//#include <TestSuite/TimerTestAdvanced/Timers.h>

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
    //UARTTest        usartTest  ( COMTestPort, 9600, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
   // GPIO        gpioTest   ( GPIOTestPin );
    //SPI         spiTest    ( SPIChipSelect, SPIModule, g_EEPROM_STM95x );
    //TimerTest      timersTest ( DisplayInterval, TimerDuration );
    //Timers		   timerTest(30,200);
    LCDTest        lcdTest;

    do
    {
        //result = RamTest.Execute   ( STREAM__OUT );
        //result = eventsTest.Execute( STREAM__OUT );
        result = lcdTest.Execute ();
       // result = gpioTest.Execute2  ( );
        //result = spiTest.Execute   ( STREAM__OUT );
        //result = timersTest.Execute( STREAM__OUT );
    	//result = timerTest.Execute(TESTRNDGEN);

    } while(FALSE); // run only once!

      while(TRUE);
}
