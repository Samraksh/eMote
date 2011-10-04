////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <netmf_watchdog.h>


////////////////////////////////////////////////////////////////////////////////////////

WatchDog_Driver g_WatchDog_Driver;


HRESULT WatchDog_Driver::Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context )
{
    return S_OK;
}

void WatchDog_Driver::Disable()
{
    GLOBAL_LOCK(irq);
}

void WatchDog_Driver::ResetCounter()
{
    GLOBAL_LOCK(irq);        
}

void WatchDog_Driver::ResetCpu()
{
    // disable interrupts
    GLOBAL_LOCK(irq);

    // assure it's enabled and that timeout is set
 
	// Make sure timer clocks are enabled

    // Make sure Watchdog is enabled

    // sit here forever and wait for reset to happen
    while(true);
}

//--//

void WatchDog_Driver::ISR( void* Param )
{        
#if !defined(BUILD_RTM)
        debug_printf( "WATCHDOG ISR  \r\n" );

        debug_printf( "TimeoutTicks : 0x%08x\r\n", g_WatchDog_Driver.m_ResetCount);
        //debug_printf( "WatchdogCnt  : 0x%08x\r\n", Timer_Driver::GetCompare(PXA271_Driver::c_Watchdog_Timer));
        //debug_printf( "SystemTimeCnt: 0x%08x\r\n", Timer_Driver::ReadCounter(PXA271_Driver::c_SystemTime_Timer));    
#endif

    // clear the interrrupt
    //PXA271_TIMER_Driver::ResetCompareHit( PXA271_Driver::c_Watchdog_Timer );


    //g_PXA271_WATCHDOG_Driver.m_callback( g_PXA271_WATCHDOG_Driver.m_context );
}
