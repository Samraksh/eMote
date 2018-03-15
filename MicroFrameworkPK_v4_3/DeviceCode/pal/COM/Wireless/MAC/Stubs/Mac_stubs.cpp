/*
 * Mac_stubs.cpp
 *
 *  Created on: March 22, 2017
 *      Author: Mukundan
 */

#include <Samraksh/MAC_decl.h>
#include <Samraksh/Mac.h>

Buffer_15_4_t g_receive_buffer;
Buffer_15_4_t g_send_buffer;
//UINT8 currentMacName;

#if defined(DEBUG_MACFUNCTIONS)
#define ENABLE_PIN_MAC(x,y) CPU_GPIO_EnableOutputPin(x,y)
#define SET_PIN(x,y) CPU_GPIO_SetPinState(x,y)
#define DEBUG_PRINTF_MAC(x) CLR_Debug::Printf(x)
#else
#define ENABLE_PIN_MAC(x,y)
#define SET_PIN(x,y)
#define DEBUG_PRINTF_MAC(x)
#endif



DeviceStatus MAC_Initialize(MACEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioName, void* config){
	return DS_Fail;}

DeviceStatus MAC_Reconfigure(void* config)
{
	return DS_Fail;
}

UINT16 MAC_GetRadioAddress(){
	return 0;
}

BOOL MAC_SetRadioAddress(UINT16 address){
	return FALSE;
}

BOOL MAC_SetRadioName(INT8 radioName){
	return FALSE;
}

BOOL MAC_SetRadioTxPower(int power){
	return FALSE;
}

BOOL MAC_SetRadioChannel(int channel){
	return DS_Fail;
}

DeviceStatus MAC_GetNextPacket(UINT8 **managedBuffer)
{
	return DS_Fail;
}

DeviceStatus MAC_UnInitialize(){
	return DS_Fail;
}

UINT8 MAC_GetID(){
	return 0;
}

DeviceStatus MAC_SendTimeStamped(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size, UINT32 eventTime){
	return DS_Fail;
}

DeviceStatus MAC_Send(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size){
	return DS_Fail;
}

DeviceStatus MAC_GetNeighborList(UINT16 *buffer)
{
	return DS_Fail;
}
DeviceStatus MAC_GetMACNeighborList(UINT16 *buffer)
{
	return DS_Fail;
}

DeviceStatus MAC_GetNeighborStatus(UINT16 macAddress, UINT8 *buffer)
{
	return DS_Fail;
}

#if  0
//Neighbor functions
NeighborTable* MAC_GetNeighborTable(UINT8 macID){
	return NULL;
}

Neighbor_t* MAC_GetNeighbor(UINT8 macID, UINT16 macAddress){

}
#endif
//Channel/freq functions


//Buffer functions
UINT8 MAC_GetSendBufferSize(){
	return 0;
}

UINT8 MAC_GetReceiveBufferSize(){
	return 0;
}

UINT8 MAC_GetPendingPacketsCount_Send(){

	return 0;
}

UINT8 MAC_GetPendingPacketsCount_Receive(){
	return 0;
}

DeviceStatus MAC_RemovePacket(UINT8* msg){
	return DS_Fail;
}

//Mac Aggregate APIs
BOOL MACLayer_Initialize(){
	return FALSE;
}

BOOL MACLayer_UnInitialize(){
	return FALSE;
}

UINT8 MACLayer_NumberMacsSupported(){
	return FALSE;
}





