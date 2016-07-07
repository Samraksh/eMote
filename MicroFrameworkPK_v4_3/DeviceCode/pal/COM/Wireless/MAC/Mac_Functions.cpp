/*
 * MAC_Functions.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: Mukundan
 */

#include <Samraksh/MAC/CSMAMAC/csmaMAC.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/MAC_decl.h>
#include <Samraksh/PacketTimeSync_15_4.h>

extern csmaMAC g_csmaMacObject;
extern OMACType g_OMAC;

Buffer_15_4_t g_send_buffer;
Buffer_15_4_t g_receive_buffer;
NeighborTable g_NeighborTable;

UINT8 currentMacName;

//#define DEBUG_MACFUNCTIONS 1

#if defined(DEBUG_MACFUNCTIONS)
#define ENABLE_PIN_MAC(x,y) CPU_GPIO_EnableOutputPin(x,y)
#define SET_PIN(x,y) CPU_GPIO_SetPinState(x,y)
#define DEBUG_PRINTF_MAC(x) CLR_Debug::Printf(x)
#else
#define ENABLE_PIN_MAC(x,y)
#define SET_PIN(x,y)
#define DEBUG_PRINTF_MAC(x)
#endif


//Basic functions
//UINT8 MacName = 0;

DeviceStatus MAC_Initialize(MACEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioName, void* config){
	DeviceStatus status = DS_Success;
	if(macName == CSMAMAC){
		currentMacName = macName;
		status = g_csmaMacObject.Initialize(eventHandler, macName, routingAppID, radioName, (MACConfig*)config) ;
	}
	else if(macName == OMAC) {
		currentMacName = macName;
		status = g_OMAC.Initialize(eventHandler, macName, routingAppID, radioName, (MACConfig *) config);
	}
	else{
		status = DS_Fail;
	}

	return status;
}

DeviceStatus MAC_Reconfigure(void* config)
{
	DeviceStatus status = DS_Success;
	if(currentMacName == CSMAMAC){
		status = g_csmaMacObject.SetConfig((MACConfig*)config);
	}
	else if(currentMacName == OMAC) {
		status = g_OMAC.SetConfig((MACConfig*)config);
	}
	else{
		status = DS_Fail;
	}

	return status;
}

UINT16 MAC_GetRadioAddress(){
	UINT16 tempMacName = -1;
	if(currentMacName == CSMAMAC){
		tempMacName = g_csmaMacObject.GetRadioAddress();
		return tempMacName;
	}
	else if(currentMacName == OMAC) {
		tempMacName = g_OMAC.GetRadioAddress();
		return tempMacName;
	}
	return tempMacName;
}

BOOL MAC_SetRadioAddress(UINT16 address){
	BOOL status = FALSE;
	if(currentMacName == CSMAMAC){
		status = g_csmaMacObject.SetRadioAddress(address);
		return status;
	}
	else if(currentMacName == OMAC) {
		status = g_OMAC.SetRadioAddress(address);
		return status;
	}
	return status;
}

BOOL MAC_SetRadioName(INT8 radioName){
	BOOL status = FALSE;
	if(currentMacName == CSMAMAC){
		status = g_csmaMacObject.SetRadioName(radioName);
		return status;
	}
	else if(currentMacName == OMAC) {
		status = g_OMAC.SetRadioName(radioName);
		return status;
	}
	return status;
}

BOOL MAC_SetRadioTxPower(int power){
	BOOL status = FALSE;
	if(currentMacName == CSMAMAC){
		status = g_csmaMacObject.SetRadioTxPower(power);
		return status;
	}
	else if(currentMacName == OMAC) {
		status = g_OMAC.SetRadioTxPower(power);
		return status;
	}
	return status;
}

BOOL MAC_SetRadioChannel(int channel){
	BOOL status = FALSE;
	if(currentMacName == CSMAMAC){
		status = g_csmaMacObject.SetRadioChannel(channel);
		return status;
	}
	else if(currentMacName == OMAC) {
		status = g_OMAC.SetRadioChannel(channel);
		return status;
	}
	return status;
}

DeviceStatus MAC_GetNextPacket(UINT8 **managedBuffer)
{
	GLOBAL_LOCK(irq);

	Message_15_4_t** temp = g_receive_buffer.GetOldestPtr();

	//Message_15_4_t* temp = (Message_15_4_t*)managedBuffer;

	if((*temp) == NULL){
		return DS_Fail;
	}

	UINT8 Size = ((*temp)->GetHeader())->length - sizeof(IEEE802_15_4_Header_t);

	if(Size > 127){
		return DS_Fail;
	}
		
	(*managedBuffer)[0] = Size & 0x00ff;
	(*managedBuffer)[1] = (Size & 0xff00) >> 8;

	memcpy(&((*managedBuffer)[2]), (*temp)->GetPayload(), Size);

	(*managedBuffer)[2 + Size] = (*temp)->GetHeader()->payloadType;
	(*managedBuffer)[3 + Size] = (*temp)->GetMetaData()->GetRssi();
	(*managedBuffer)[4 + Size] = (*temp)->GetMetaData()->GetLqi();
	(*managedBuffer)[5 + Size] = (*temp)->GetHeader()->src;
	(*managedBuffer)[6 + Size] = ((*temp)->GetHeader()->src & 0Xff00) >> 8;
	(*managedBuffer)[7 + Size] = ((*temp)->GetHeader()->dest == MAC_BROADCAST_ADDRESS) ? 0 : 1;
	//memcpy(*managedBuffer, *temp, ((*temp)->GetHeader())->length - sizeof(IEEE802_15_4_Header_t));

	if( ((*temp)->GetHeader())->flags & MFM_TIMESYNC )
	{
		// The packet is timestamped
		(*managedBuffer)[8 + Size] = (BYTE) TRUE;
		UINT64 EventTime = PacketTimeSync_15_4::EventTime((*temp), ((*temp)->GetHeader())->length);

		UINT32 eventTime = (EventTime & 0xffffffff);

#ifdef DEBUG_MACFUNCTIONS
		hal_printf("The Snap Shot in native is %u\n", eventTime);
#endif

		(*managedBuffer)[9 + Size] = (EventTime & 0xff);
		(*managedBuffer)[10 + Size] = (EventTime >> 8) & 0xff;
		(*managedBuffer)[11 + Size] = (EventTime >> 16) & 0xff;
		(*managedBuffer)[12 + Size] = (EventTime >> 24) & 0xff;
		(*managedBuffer)[13 + Size] = (EventTime >> 32) & 0xff;
		(*managedBuffer)[14 + Size] = (EventTime >> 40) & 0xff;
		(*managedBuffer)[15 + Size] = (EventTime >> 48) & 0xff;
		(*managedBuffer)[16 + Size] = (EventTime >> 56) & 0xff;
	}
	else
	{
		(*managedBuffer)[8 + Size] = (BYTE) FALSE;
		(*managedBuffer)[9 + Size] = ((*temp)->GetMetaData()->GetReceiveTimeStamp() & 0xff);
     	(*managedBuffer)[10 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 8) & 0xff);
		(*managedBuffer)[11 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 16) & 0xff);
		(*managedBuffer)[12 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 24) & 0xff);
		(*managedBuffer)[13 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 32) & 0xff);
		(*managedBuffer)[14 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 40) & 0xff);
		(*managedBuffer)[15 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 48) & 0xff);
		(*managedBuffer)[16 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 56) & 0xff);
	}

	//g_receive_buffer.DropOldest(1);

	return DS_Success;
}

DeviceStatus MAC_UnInitialize(){
	BOOL status = FALSE;
	if(currentMacName == CSMAMAC){
		status = g_csmaMacObject.UnInitialize();
	}
	else if(currentMacName == OMAC) {
		status = g_OMAC.UnInitialize();
	}

	return ((status == TRUE) ? DS_Success : DS_Fail);
}

UINT8 MAC_GetID(){
	return currentMacName;
}

DeviceStatus MAC_SendTimeStamped(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size, UINT32 eventTime){
	BOOL status = FALSE;
	if(currentMacName == CSMAMAC){
		status = g_csmaMacObject.SendTimeStamped(destAddress, dataType, msg, size, eventTime);
	}
	else if(currentMacName == OMAC){
		status = g_OMAC.SendTimeStamped(destAddress, dataType, msg, size, eventTime);
	}

	if(status != TRUE)
	    return DS_Fail;

	return DS_Success;
}

DeviceStatus MAC_Send(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size){
	//msg is just the payload,

	BOOL status = FALSE;
	if(currentMacName == CSMAMAC){
		status = g_csmaMacObject.Send(destAddress, dataType, msg, size);
	}
	else if(currentMacName == OMAC){
		status = g_OMAC.Send(destAddress, dataType, msg, size);
	}

	if(status != TRUE)
		return DS_Fail;

	return DS_Success;
}

DeviceStatus MAC_GetNeighborList(UINT16 *buffer)
{
	UINT8 neighborCount = 0;

	DEBUG_PRINTF_MAC("[NATIVE] : Calling GetNeighbor List\n");

	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		// initializing buffer
		buffer[i] = 0;
	}
	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if(g_NeighborTable.Neighbor[i].Status == Alive)
		{
			buffer[neighborCount++] = g_NeighborTable.Neighbor[i].MacAddress;
		}
	}

	if(neighborCount == 0)
	{
		DEBUG_PRINTF_MAC("[NATIVE] : Neighbor Count is 0\n");
		buffer[0] = 0;
	}

	return DS_Success;
}

DeviceStatus MAC_GetNeighborStatus(UINT16 macAddress, UINT8 *buffer)
{
	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if(g_NeighborTable.Neighbor[i].MacAddress == macAddress)
		{
			buffer[0] = g_NeighborTable.Neighbor[i].MacAddress & 0xff;
			buffer[1] = (g_NeighborTable.Neighbor[i].MacAddress & 0xff00) >> 8;
			buffer[2] = (g_NeighborTable.Neighbor[i].ForwardLink.AvgRSSI);
			buffer[3] = (g_NeighborTable.Neighbor[i].ForwardLink.LinkQuality);
			buffer[4] = (g_NeighborTable.Neighbor[i].ForwardLink.AveDelay);
			buffer[5] = (g_NeighborTable.Neighbor[i].ReverseLink.AvgRSSI);
			buffer[6] = (g_NeighborTable.Neighbor[i].ReverseLink.LinkQuality);
			buffer[7] = (g_NeighborTable.Neighbor[i].ReverseLink.AveDelay);
			buffer[8] = (g_NeighborTable.Neighbor[i].Status);
			buffer[9] = (g_NeighborTable.Neighbor[i].PacketsReceived & 0xff);
			buffer[10] = (g_NeighborTable.Neighbor[i].PacketsReceived & 0xff00) >> 8;
			buffer[11] = (g_NeighborTable.Neighbor[i].LastHeardTime) & 0xff;
			buffer[12] = (g_NeighborTable.Neighbor[i].LastHeardTime & 0xff00) >> 8;
			buffer[13] = (g_NeighborTable.Neighbor[i].LastHeardTime & 0xff0000) >> 16;
			buffer[14] = (g_NeighborTable.Neighbor[i].LastHeardTime & 0xff000000) >> 24;
			buffer[15] = (g_NeighborTable.Neighbor[i].LastHeardTime & 0xff00000000) >> 32;
			buffer[16] = (g_NeighborTable.Neighbor[i].LastHeardTime & 0xff0000000000) >> 40;
			buffer[17] = (g_NeighborTable.Neighbor[i].LastHeardTime & 0xff000000000000) >> 48;
			buffer[18] = (g_NeighborTable.Neighbor[i].LastHeardTime & 0xff00000000000000) >> 56;
			buffer[19] = (g_NeighborTable.Neighbor[i].ReceiveDutyCycle);
			buffer[20] = (g_NeighborTable.Neighbor[i].FrameLength);
			buffer[21] = (g_NeighborTable.Neighbor[i].FrameLength & 0xff00) >> 8;

			return DS_Success;

		}
	}

	return DS_Fail;
}

#if  0
//Neighbor functions
NeighborTable* MAC_GetNeighborTable(UINT8 macID){
	if(MacName == CSMAMAC)
		return g_csmaMacObject.GetNeighborTable();
	else if(MacName == OMAC)
		return NULL;
	//return (NeighborTable *)(NULL);
}

Neighbor_t* MAC_GetNeighbor(UINT8 macID, UINT16 macAddress){

}
#endif
//Channel/freq functions


//Buffer functions
UINT8 MAC_GetSendBufferSize(){
	UINT8 bufferSize = -1;
	if(currentMacName == CSMAMAC){
		bufferSize = g_csmaMacObject.GetSendBufferSize();
	}
	else if(currentMacName == OMAC){
		bufferSize = g_OMAC.GetSendBufferSize();
	}

	return bufferSize;
}

UINT8 MAC_GetReceiveBufferSize(){
	UINT8 bufferSize = -1;
	if(currentMacName == CSMAMAC){
		bufferSize = g_csmaMacObject.GetReceiveBufferSize();
	}
	else if(currentMacName == OMAC){
		bufferSize = g_OMAC.GetReceiveBufferSize();
	}

	return bufferSize;
}

UINT8 MAC_GetPendingPacketsCount_Send(){
	UINT8 pendingPackets = -1;
	if(currentMacName == CSMAMAC){
		pendingPackets = g_csmaMacObject.GetSendPending();
	}
	else if(currentMacName == OMAC){
		pendingPackets = g_OMAC.GetSendPending();
	}

	return pendingPackets;
}

UINT8 MAC_GetPendingPacketsCount_Receive(){
	UINT8 pendingPackets = -1;
	if(currentMacName == CSMAMAC){
		pendingPackets = g_csmaMacObject.GetReceivePending();
	}
	else if(currentMacName == OMAC){
		pendingPackets = g_OMAC.GetReceivePending();
	}

	return pendingPackets;
}

DeviceStatus MAC_RemovePacket(UINT8* msg){
	return DS_Fail;
}

//Mac Aggregate APIs
BOOL MACLayer_Initialize(){
	return FALSE;
}

BOOL MACLayer_UnInitialize(){
	BOOL status = FALSE;
	if(currentMacName == CSMAMAC){
		status = g_csmaMacObject.UnInitialize();
	}
	else if(currentMacName == OMAC){
		status = g_OMAC.UnInitialize();
	}

	return status;
}

UINT8 MACLayer_NumberMacsSupported(){
	return FALSE;
}


