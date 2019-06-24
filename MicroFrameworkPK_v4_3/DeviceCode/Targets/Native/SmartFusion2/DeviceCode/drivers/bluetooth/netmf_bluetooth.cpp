#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include "c_code_calling_cpp.h"

extern "C" {
//#include "include\SS1BTPS.h"
#include "sf2\btstack_port.h"
};

DeviceStatus CPU_Bluetooth_Initialize( )
{
	DeviceStatus status = DS_Success;
	int Result;
	
	hal_printf("init Bluetooth\r\n");

	//configuring and resetting bluetooth module
	// Configure the nSHUT pin, drive it low to put the radio into reset.
	CPU_GPIO_EnableOutputPin(5, FALSE);
	// delay
	CPU_Timer_Sleep_MicroSeconds(500000, ADVTIMER_32BIT);
	CPU_GPIO_SetPinState(5, TRUE);

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

DeviceStatus Bluetooth_Send_Data(UINT16 dest, uint8_t *buffer, uint16_t buffer_size){
	sendBTPacket(dest, buffer, buffer_size);
}

DeviceStatus Bluetooth_Receive_Data(UINT16 source, uint8_t *buffer, uint16_t buffer_size){
}

// for now we just have the one neighbor
DeviceStatus GetNeighbors(UINT16* buffer){
	buffer[0] = 1;

	return DS_Success;
}
