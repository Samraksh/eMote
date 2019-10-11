#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include "c_code_calling_cpp.h"
#include <Samraksh\BluetoothMac_Functions.h>

extern "C" {
#include "sf2\btstack_port.h"
};

MACEventHandler* btEventHandler = NULL;
NeighborChangeFuncPtrType neighborChangeHandler = NULL;
MACReceiveFuncPtrType rxDataHandler = NULL;
static int neighborCount = 0;

DeviceStatus CPU_Bluetooth_Initialize(MACEventHandler* eventHandler )
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

	btEventHandler = eventHandler;
	neighborChangeHandler = eventHandler->GetNeighborChangeHandler();
	rxDataHandler = eventHandler->GetReceiveHandler();

	neighborCount = 0;

	return status;
}

BOOL CPU_Bluetooth_Reset()
{
	BOOL result = FALSE;

	//configuring and resetting bluetooth module
	// Configure the nSHUT pin, drive it low to put the radio into reset.
	CPU_GPIO_EnableOutputPin(5, FALSE);
	// delay
	CPU_Timer_Sleep_MicroSeconds(500000, ADVTIMER_32BIT);
	CPU_GPIO_SetPinState(5, TRUE);

	bluetooth_main();

	neighborCount = 0;
	
	return result;
}

BOOL CPU_Bluetooth_UnInitialize()
{
	BOOL result = FALSE;

	// turning off the Bluetooth module
	CPU_GPIO_EnableOutputPin(5, FALSE);

	// the bluetooth driver does not have an uninit function as of yet.
	// TODO: implement
	
	return result;
}

DeviceStatus Bluetooth_Send_Data(UINT16 dest, uint8_t *buffer, uint16_t buffer_size){
	sendBTPacket(dest, buffer, buffer_size);
	
	return DS_Success;
}

DeviceStatus Bluetooth_Receive_Data(UINT16 packetType, uint8_t *buffer, uint16_t buffer_size){
	Message_15_4_t msg;
	Message_15_4_t* msgPtr = &msg;
	msgPtr->GetHeader()->src = 1;
	msgPtr->GetHeader()->dest = 0;
	msgPtr->GetHeader()->length = buffer_size + sizeof(IEEE802_15_4_Header_t);
	msgPtr->GetHeader()->payloadType = packetType;
	memcpy(msgPtr->GetPayload(), buffer , buffer_size);
	//hal_printf("bt rx buffer size: %d len: %d addr: %x\r\n", buffer_size, msgPtr->GetHeader()->length, msgPtr);
	(rxDataHandler)((void*)msgPtr, packetType);
}

void CPU_Bluetooth_Connected(int number, int connectionType){
	neighborCount++;
	(neighborChangeHandler)(neighborCount);
}

void CPU_Bluetooth_Disconnected(int number, int connectionType){
	neighborCount--;
	(neighborChangeHandler)(neighborCount);
}

// for now we just have the one neighbor
DeviceStatus GetNeighbors(UINT16* buffer){
	buffer[0] = 1;

	return DS_Success;
}
