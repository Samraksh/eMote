////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <tinyhal.h>
#include <Tests.h>
#include "nativesample.h"
#include "..\..\..\DeviceCode\Targets\Native\TIAM3517\DeviceCode\TIAM3517.h"

HAL_DECLARE_NULL_HEAP();

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

void blank()
{


}

UINT32 gVar = 32;


/*
void timer_overflow_int(void) {
	writel(0x2, TIMERtier);
}
*/

void ApplicationEntryPoint()
{
	GPMCTest gTest;

	gTest.Execute();

}

void ApplicationEntryPointOld()
{
    BOOL result;

	UINT32 temp = 5;

    //RAM         RamTest    ( (UINT32*)RAMTestBase, (UINT32)RAMTestSize, (ENDIAN_TYPE)ENDIANESS, BUS_WIDTH );
    //TimedEvents eventsTest;
    //UART        usartTest  ( COMTestPort, 9600, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
    //GPIO        gpioTest   ( GPIOTestPin );
    //SPI         spiTest    ( SPIChipSelect, SPIModule, g_EEPROM_STM95x );
    //Timers      timersTest ( DisplayInterval, TimerDuration );

    //GPIO_PIN g = 58;

    //CPU_GPIO_EnableOutputPin(g,false);
    //CPU_GPIO_SetPinState(g,true);



	//CPU_INTC_Initialize();

	//enable_interrupts();

	//disable_interrupts();

	//blank();
	CPU_INTC_Initialize();

	//if(TIAM3517_TIMER_Driver::Initialize(1,TRUE,0,0, TIMER_ISR, NULL))
	//{
		// Success
	//}

	//TIAM3517_TIMER &TIMERZ = TIAM3517_TIMER_Driver::getTimer(1);

	//TIMERZ.TIER |= 1;
	//TIMERZ.TIER |= 2;

	//enable_interrupts();
	/*
	if(TIAM3517_TIMER_Driver::Initialize(3,TRUE,0,0, TIMER_ISR, NULL))
	{
		// Success
	}

	if(TIAM3517_TIMER_Driver::Initialize(4,TRUE,0,0, TIMER_ISR, NULL))
	{
		// Success
	}
	*/
	//timer_overflow_int();

	TIAM3517_TIMER_Driver::SetCompare(1,10000);
	//TIAM3517_TIMER_Driver::SetCompare(2,10000);
	//TIAM3517_TIMER_Driver::SetCompare(3,10000);


    // do
    // {
        // //result = RamTest.Execute   ( STREAM__OUT );
      // //  result = eventsTest.Execute( STREAM__OUT );
        // //result = usartTest.Execute ( STREAM__OUT );
       // // result = gpioTest.Execute  ( STREAM__OUT );
        // //result = spiTest.Execute   ( STREAM__OUT );
      // //  result = timersTest.Execute( STREAM__OUT );

    // } while(FALSE); // run only once!
	TIAM3517_AITC &INTC = TIAM3517::AITC();

	/*
	for(UINT32 i = 0 ; i < 26 ; i++)
	{
		INTC.Reg[1].INTCPS_ISR_SET |= (1 << i);
	}
	*/

	temp = INTC.Reg[0].INTCPS_ITR;
	temp = INTC.Reg[1].INTCPS_ITR;
	temp = INTC.Reg[2].INTCPS_ITR;



	temp = INTC.INTCPS_IDLE;

	temp = INTC.Reg[0].INTCPS_MIR;
	temp = INTC.Reg[0].INTCPS_PENDING_IRQ;
	temp = INTC.Reg[0].INTCPS_PENDING_FIQ;


	temp = INTC.Reg[1].INTCPS_MIR;
	temp = INTC.Reg[1].INTCPS_PENDING_IRQ;
	temp = INTC.Reg[1].INTCPS_PENDING_FIQ;

	temp = INTC.Reg[2].INTCPS_MIR;
	temp = INTC.Reg[2].INTCPS_PENDING_IRQ;
	temp = INTC.Reg[2].INTCPS_PENDING_FIQ;

	temp = INTC.INTCPS_SIR_IRQ;

	TIAM3517_TIMER &TIMER = TIAM3517_TIMER_Driver::getTimer(1);

	temp = TIMER.TIER;
	temp = TIMER.TISR;

    while(TRUE);
}

void TIMER_ISR(void* Param)
{

	if(TIAM3517_TIMER_Driver::DidCompareHit( 1 ))
    {
        TIAM3517_TIMER_Driver::ResetCompareHit( 1 );
    }

}
