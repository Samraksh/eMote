/*	Shutdown.cpp
 *	Used to shutdown all drivers currently running.
*/

#include <tinyhal.h>
#include <stm32f10x.h>
#include "drivers\adc\hal_adc_driver.h"
#include <Samraksh/MAC_decl.h>

bool ShutdownDrivers(void)
{
	bool returnValue;

	returnValue = AD_Uninitialize();
	MAC_UnInitialize();

	return returnValue;
}
