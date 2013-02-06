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

BOOL enable = FALSE;

HRESULT Watchdog_Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK isr, void* context )
{
	if(enable)
		return S_OK;

	// Set the barktime
	UINT32  barktime = TimeoutMilliseconds;



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
