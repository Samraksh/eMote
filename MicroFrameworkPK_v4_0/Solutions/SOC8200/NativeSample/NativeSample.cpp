////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <tinyhal.h>
#include <Tests.h>
#include "nativesample.h"
#include "..\..\..\DeviceCode\Targets\Native\TIAM3517\DeviceCode\TIAM3517.h"

void TIMER_ISR(void* Param);

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
    //GPIO        gpioTest   ( GPIOTestPin );   
    //SPI         spiTest    ( SPIChipSelect, SPIModule, g_EEPROM_STM95x );
    //Timers      timersTest ( DisplayInterval, TimerDuration );

    //GPIO_PIN g = 58;

    //CPU_GPIO_EnableOutputPin(g,false);
    //CPU_GPIO_SetPinState(g,true);    
	
	CPU_INTC_Initialize();
	
	if(TIAM3517_TIMER_Driver::Initialize(1,TRUE,0,0, TIMER_ISR, NULL))
	{
		// Success
	}
	
	TIAM3517_TIMER_Driver::SetCompare(1,10000);

    // do
    // {   
        // //result = RamTest.Execute   ( STREAM__OUT );       
      // //  result = eventsTest.Execute( STREAM__OUT );
        // //result = usartTest.Execute ( STREAM__OUT );
       // // result = gpioTest.Execute  ( STREAM__OUT );
        // //result = spiTest.Execute   ( STREAM__OUT );
      // //  result = timersTest.Execute( STREAM__OUT );

    // } while(FALSE); // run only once!

    while(TRUE);
}

void TIMER_ISR(void* Param)
{

	if(TIAM3517_TIMER_Driver::DidCompareHit( 1 ))
    {
        TIAM3517_TIMER_Driver::ResetCompareHit( 1 );
    }

}
