/**
 * Shutdown.cpp
 * UnInitialize subset of drivers
 */

#include <tinyhal.h>
#include <stm32f10x.h>
#include "drivers\adc\hal_adc_driver.h"
#include <Samraksh/MAC_decl.h>

/**
 *  Shut down some drivers for reprogramming... this should happen anyway with the soft reboot though.
 *  @see tinyhal.cpp HAL_Uninitialize s_rebootHandlers
 */
bool ShutdownDrivers(void)
{
	bool returnValue;

	returnValue = AD_Uninitialize();
	returnValue &= ( DS_Success == MAC_UnInitialize(/*Mac_GetID()*/) );

	return returnValue;
}
