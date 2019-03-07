#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include "c_code_calling_cpp.cpp"

extern "C" {
//#include "include\SS1BTPS.h"
#include "sf2\btstack_port.h"
};


DeviceStatus CPU_Bluetooth_Initialize( )
{
	DeviceStatus status = DS_Success;
	int Result;
	
	hal_printf("init Bluetooth\r\n");
	// configure hardware

	//configuring and resetting bluetooth module
	// Configure the nSHUT pin, drive it low to put the radio into reset.
	CPU_GPIO_EnableOutputPin(5, FALSE);
	// delay
	CPU_Timer_Sleep_MicroSeconds(500000,ADVTIMER_32BIT);
	CPU_GPIO_SetPinState(5,TRUE);

	bluetooth_main();

	/*uint8_t x[5];
	x[0] = 't';
	x[1] = 'e';
	x[2] = 's';
	x[3] = 't';
	x[4] = '\n';
	btUartWrite(&x[0], 5);*/
 
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



