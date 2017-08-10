/**
 * Shutdown.cpp
 * UnInitialize subset of drivers
 */

#include <tinyhal.h>

/**
 *  Shut down some drivers for reprogramming... this should happen anyway with the soft reboot though.
 *  @see tinyhal.cpp HAL_Uninitialize s_rebootHandlers
 */
bool ShutdownDrivers(void)
{
	bool returnValue;
	return returnValue;
}
