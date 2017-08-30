//#include <mxc_sys.h>
#include <Device/Maxim/MAX3263X/Include/system_max3263x.h>
#include <tinyhal.h>

/**
 * Calls various bootstrap routines
 **/

extern "C"
{
void BootstrapCode()
{

	//PreInit();
	//Default initialization of the system that came with maxim. This probably needs to be customized for Sarmaksh use.
	SystemInit();
	PrepareImageRegions();
}

};
