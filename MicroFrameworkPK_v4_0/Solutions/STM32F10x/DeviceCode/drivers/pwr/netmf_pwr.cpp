////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <netmf_pwr.h>
#include <led/stm32f10x_led.h>
#include <wwdg/netmf_watchdog.h>
///////////////////////////////////////////////////////////////////////////////

//--//

//--//
Power_Driver g_Power_Driver;

/*
Function typically initializes the very low-level CPU functionalities, 
such as the GPIO, the interrupt controller, the direct memory access (DMA) controller, 
the serial peripheral interface (SPI)
*/
BOOL Power_Driver::Initialize()
{
    CPU_INTC_Initialize();    	
    //DMA_Initialize();
    return TRUE;
}

void Power_Driver::Pause()
{
	//Put the CPU into an interrupt based sleep mode    
}

void Power_Driver::Sleep()
{
    // it should never be called from an ISR
    ASSERT(!SystemState_Query(SYSTEM_STATE_ISR));
    // this doesn't work if we are taking interrupts!
    ASSERT_IRQ_MUST_BE_OFF();
    Pause();
}

/*
Use this function when your device encounters an unrecoverable error
Halts the CPU
*/
void Power_Driver::Halt()
{
    GLOBAL_LOCK(irq);
    // disable all interrupt source in controller that might wake us from our slumber
    CPU_INTC_Initialize();

    Pause();
}

void Power_Driver::Reset()
{
   //Use the Watch dog to invoke a reset on the board
   WatchDog_Driver::ResetCpu();
}

void Power_Driver::Shutdown()
{
    GLOBAL_LOCK(irq);
    while(true);
}

void Power_Driver::Hibernate()
{    
    // provide implementation
}

void HAL_AssertEx()
{
	LED_ERROR();
    // cause an abort and let the abort handler take over
    //*((char*)0xFFFFFFFF) = 'a';
}




