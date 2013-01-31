/*
 * 	Author : Nived Sivadas
 *
 *  File Name : Krait_watchdog_functions.cpp
 *
 *  Description : contains the core functions responsible for interfacing with MF
 *
 */

#include <tinyhal.h>

//--//

HRESULT Watchdog_Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK isr, void* context )
{
    return CLR_E_FAIL;
}

void Watchdog_Disable()
{
}

void Watchdog_ResetCpu()
{
}

void Watchdog_ResetCounter()
{
}
