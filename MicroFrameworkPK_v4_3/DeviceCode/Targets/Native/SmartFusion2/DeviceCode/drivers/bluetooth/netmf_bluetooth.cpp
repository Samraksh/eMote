#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
extern "C" {
//#include "include\SS1BTPS.h"
#include "sf2\btstack_port.h"
};


DeviceStatus CPU_Bluetooth_Initialize( )
{
	DeviceStatus status = DS_Fail;
	int Result;
	
	hal_printf("init Bluetooth\r\n");
	// configure hardware
	
	// Configure the nSHUT pin, drive it low to put the radio into reset.
	CPU_GPIO_EnableOutputPin(5, FALSE);

	// **** delay
	CPU_GPIO_SetPinState(5,TRUE);

	bluetooth_main();
 
	return status;
}


BOOL CPU_Bluetooth_Reset()
{
	BOOL result = FALSE;

	
	return result;
}


BOOL CPU_Bluetooth_UnInitialize()
{
	BOOL result = FALSE;

	
	return result;
}



