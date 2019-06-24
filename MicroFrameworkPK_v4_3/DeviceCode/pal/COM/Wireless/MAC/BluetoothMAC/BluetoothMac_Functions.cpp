/*
 * MAC_Functions.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: Mukundan
 */

#include <Samraksh/MAC_decl.h>
#include <Samraksh\MAC.h>

Buffer_15_4_t g_send_buffer;
Buffer_15_4_t g_receive_buffer;
NeighborTable g_NeighborTable;

static UINT16 radioAddr = -1;

DeviceStatus MAC_Initialize(MACEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioName, void* config){
	DeviceStatus status = DS_Success;

	//status = Initialize(eventHandler, macName, routingAppID, radioName, (MACConfig*)config) ;
	status = CPU_Bluetooth_Initialize();

	return status;
}

DeviceStatus MAC_Reconfigure(void* config)
{
	DeviceStatus status = DS_Success;

	//status = SetConfig((MACConfig*)config);

	return status;
}

UINT16 MAC_GetRadioAddress(){
	return radioAddr;
}

BOOL MAC_SetRadioAddress(UINT16 address){
	BOOL status = TRUE;

	radioAddr = address;

	return status;
}

BOOL MAC_SetRadioName(INT8 radioName){
	BOOL status = TRUE;
		
	//status = SetRadioName(radioName);

	return status;
}

BOOL MAC_SetRadioTxPower(int power){
	BOOL status = TRUE;
		
	//status = SetRadioTxPower(power);

	return status;
}

BOOL MAC_SetRadioChannel(int channel){
	BOOL status = TRUE;
		
	//status = SetRadioChannel(channel);

	return status;
}

DeviceStatus MAC_DeletePacketWithIndexInternal(PacketID_T index){
	return DS_Fail;
}

UINT16 MAC_GetMsgIdWithPtr(Message_15_4_t* msg_carrier)
{
	return INVALID_PACKET_ID;
}

bool MAC_ChangeOwnerShipOfElementwIndex(PacketID_T index,  BufferOwner n_buf_ow)
{
	return INVALID_PACKET_ID;
}

DeviceStatus MAC_GetPacketWithIndex(UINT8 **managedBuffer, UINT8 buffersize, PacketID_T index)
{
	return DS_Fail;
}

DeviceStatus MAC_GetPacketSizeWithIndex( UINT8* buffersizeptr, PacketID_T index)
{
	return DS_Fail;
}


DeviceStatus MAC_GetNextPacket(UINT8 **managedBuffer)
{
	GLOBAL_LOCK(irq);

	/*Message_15_4_t** temp = g_receive_buffer.GetOldestPtr();

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

//#ifdef DEBUG_MACFUNCTIONS
		//hal_printf("The Snap Shot in native is %u\n", eventTime);
//#endif

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
	}*/

	//g_receive_buffer.DropOldest(1);

	return DS_Success;
}

DeviceStatus MAC_UnInitialize(){
	BOOL status = FALSE;

	status = CPU_Bluetooth_UnInitialize();

	return ((status == TRUE) ? DS_Success : DS_Fail);
}

UINT8 MAC_GetID(){
	return BLUETOOTHMAC;
}

DeviceStatus MAC_SendTimeStamped(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size, UINT32 eventTime){
	BOOL status = FALSE;
		
	status = Bluetooth_Send_Data(destAddress, (uint8_t*)msg, size);

	return DS_Success;
}

DeviceStatus MAC_Send(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size){
	BOOL status = FALSE;
		
	status = Bluetooth_Send_Data(destAddress, (uint8_t*)msg, size);

	return DS_Success;
}

PacketID_T MAC_EnqueueToSend(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size){
	return INVALID_PACKET_ID;
}

PacketID_T MAC_EnqueueToSendTimeStamped(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size, UINT32 eventTime){
	return INVALID_PACKET_ID;
}

DeviceStatus MAC_GetNeighborList(UINT16 *buffer)
{
	return GetNeighbors(buffer);
}
DeviceStatus MAC_GetMACNeighborList(UINT16 *buffer)
{
	return GetNeighbors(buffer);
}

DeviceStatus MAC_GetNeighborStatus(UINT16 macAddress, UINT8 *buffer)
{
	/*for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if(g_NeighborTable.Neighbor[i].MACAddress == macAddress)
		{
			buffer[0] = g_NeighborTable.Neighbor[i].MACAddress & 0xff;
			buffer[1] = (g_NeighborTable.Neighbor[i].MACAddress & 0xff00) >> 8;
			buffer[2] = (g_NeighborTable.Neighbor[i].SendLink.AvgRSSI);
			buffer[3] = (g_NeighborTable.Neighbor[i].SendLink.LinkQuality);
			buffer[4] = (g_NeighborTable.Neighbor[i].SendLink.AveDelay);
			buffer[5] = (g_NeighborTable.Neighbor[i].ReceiveLink.AvgRSSI);
			buffer[6] = (g_NeighborTable.Neighbor[i].ReceiveLink.LinkQuality);
			buffer[7] = (g_NeighborTable.Neighbor[i].ReceiveLink.AveDelay);
			buffer[8] = (g_NeighborTable.Neighbor[i].neighborStatus & 0x0F) || ( ((g_NeighborTable.Neighbor[i].IsAvailableForUpperLayers) & 0x0F) << 4)  ;
			buffer[9] = (g_NeighborTable.Neighbor[i].NumTimeSyncMessagesSent);
			buffer[10] = 0;

//			buffer[9] = (g_NeighborTable.Neighbor[i].CountOfPacketsReceived & 0xff);
//			buffer[10] = (g_NeighborTable.Neighbor[i].CountOfPacketsReceived & 0xff00) >> 8;
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
	}*/

	return DS_Fail;
}

//Buffer functions
UINT8 MAC_GetSendBufferSize(){
	return g_send_buffer.Size();
}

UINT8 MAC_GetReceiveBufferSize(){
	return g_receive_buffer.Size();
}

UINT8 MAC_GetPendingPacketsCount_Send(){
	return g_send_buffer.GetNumberMessagesInBuffer();
}

UINT8 MAC_GetPendingPacketsCount_Receive(){
	return g_receive_buffer.GetNumberMessagesInBuffer();
}

DeviceStatus MAC_RemovePacket(UINT8* msg){
	return DS_Fail;
}

BOOL MACLayer_Initialize(){
	return FALSE;
}

BOOL MACLayer_UnInitialize(){
	BOOL status = FALSE;
		
	status = CPU_Bluetooth_UnInitialize();

	return status;
}

UINT8 MACLayer_NumberMacsSupported(){
	return FALSE;
}


