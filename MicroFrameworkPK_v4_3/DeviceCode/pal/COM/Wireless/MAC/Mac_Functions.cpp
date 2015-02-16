/*
 * Mac_Functions.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: Mukundan
 */

#include ".\CSMAMAC\csmaMAC.h"
#include <Samraksh\Mac_decl.h>
#include <Samraksh\PacketTimeSync_15_4.h>

extern csmaMAC gcsmaMacObject;


Buffer_15_4_t m_send_buffer;
Buffer_15_4_t m_receive_buffer;
NeighborTable m_NeighborTable;

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

DeviceStatus Mac_Initialize(MacEventHandler* eventHandler, UINT8 macID, UINT8 routingAppID, UINT8 radioName, void* config){

	if(macID == CSMAMAC)
		return gcsmaMacObject.Initialize(eventHandler, macID, routingAppID, radioName, (MacConfig*)config) ;
	else if(macID == OMAC)
		//return g_OMAC.Initialize(eventHandler, macID, routingAppID, (MacConfig *) config);
		return DS_Fail;
	else
		return DS_Fail;

	return DS_Success;
}

UINT16 Mac_GetRadioAddress(){
	UINT16 temp = gcsmaMacObject.GetRadioAddress();
	hal_printf("Mac_GetRadioAddress: %d\r\n",temp);
	return temp;
}
BOOL Mac_SetRadioAddress(UINT16 address){
	gcsmaMacObject.SetRadioAddress(address);
	hal_printf("Mac_SetRadioAddress: %d\r\n",address);
	return true;
}

DeviceStatus Mac_GetNextPacket(UINT8 **managedBuffer)
{
	GLOBAL_LOCK(irq);

	Message_15_4_t** temp = m_receive_buffer.GetOldestPtr();

	if((*temp) == NULL)
		return DS_Fail;

	UINT8 Size = ((*temp)->GetHeader())->length - sizeof(IEEE802_15_4_Header_t);

	if(Size > 127)
		return DS_Fail;
		
	(*managedBuffer)[0] = Size & 0xff;
	(*managedBuffer)[1] =  (Size & 0xff00) >> 8;

	memcpy(&((*managedBuffer)[2]), (*temp)->GetPayload(), Size);

	(*managedBuffer)[2 + Size] = (*temp)->GetMetaData()->GetRssi();
	(*managedBuffer)[3 + Size] = (*temp)->GetMetaData()->GetLqi();
	(*managedBuffer)[4 + Size] = (*temp)->GetHeader()->src;
	(*managedBuffer)[5 + Size] = ((*temp)->GetHeader()->src & 0Xff00) >> 8;
	(*managedBuffer)[6 + Size] = ((*temp)->GetHeader()->dest == MAC_BROADCAST_ADDRESS) ? 0 : 1;
	//memcpy(*managedBuffer, *temp, ((*temp)->GetHeader())->length - sizeof(IEEE802_15_4_Header_t));

	if(((*temp)->GetHeader())->GetFlags() & MFM_TIMESYNC)
	{
		// The packet is timestamped
		(*managedBuffer)[7 + Size] = (BYTE) TRUE;
		UINT64 EventTime = PacketTimeSync_15_4::EventTime((*temp), ((*temp)->GetHeader())->length);

		UINT32 eventTime = (EventTime & 0xffffffff);

		hal_printf("The Snap Shot in native is %u\n", eventTime);

		(*managedBuffer)[8 + Size] = (EventTime & 0xff);
		(*managedBuffer)[9 + Size] = (EventTime >> 8) & 0xff;
		(*managedBuffer)[10 + Size] = (EventTime >> 16) & 0xff;
		(*managedBuffer)[11 + Size] = (EventTime >> 24) & 0xff;
		(*managedBuffer)[12 + Size] = (EventTime >> 32) & 0xff;
		(*managedBuffer)[13 + Size] = (EventTime >> 40) & 0xff;
		(*managedBuffer)[14 + Size] = (EventTime >> 48) & 0xff;
		(*managedBuffer)[15 + Size] = (EventTime >> 56) & 0xff;


	}
	else
	{
		(*managedBuffer)[7 + Size] = (BYTE) FALSE;
		(*managedBuffer)[8 + Size] = ((*temp)->GetMetaData()->GetReceiveTimeStamp() & 0xff);
     	(*managedBuffer)[9 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 8) & 0xff);
		(*managedBuffer)[10 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 16) & 0xff);
		(*managedBuffer)[11 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 24) & 0xff);
		(*managedBuffer)[12 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 32) & 0xff);
		(*managedBuffer)[13 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 40) & 0xff);
		(*managedBuffer)[14 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 48) & 0xff);
		(*managedBuffer)[15 + Size] = (((*temp)->GetMetaData()->GetReceiveTimeStamp() >> 56) & 0xff);
	}


	//m_receive_buffer.DropOldest(1);

	return DS_Success;

}

DeviceStatus Mac_UnInitialize(UINT8 macID){
	return DS_Fail;
}

UINT8 Mac_GetID(){
	return gcsmaMacObject.macName ;
}

DeviceStatus Mac_SendTimeStamped(UINT8 macID, UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size, UINT32 eventTime){
	if(macID == CSMAMAC)
	{
		if(gcsmaMacObject.SendTimeStamped(destAddress, dataType, msg, size, eventTime) != TRUE)
		    return DS_Fail;
		else
			return DS_Success;
	}
	else if(macID == OMAC)
			//return g_OMAC.Send(destAddress, dataType, msg, size);
		return DS_Fail;
	else
		return DS_Fail;
}

DeviceStatus Mac_Send(UINT8 macID, UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size){

	//msg is just the payload,

	if(macID == CSMAMAC)
	{
		  if(gcsmaMacObject.Send(destAddress, dataType, msg, size) != TRUE)
			  return DS_Fail;
		  else
			  return DS_Success;
	}
	else if(macID == OMAC)
		//return g_OMAC.Send(destAddress, dataType, msg, size);
		return DS_Fail;
	else
		return DS_Fail;

}

DeviceStatus Mac_GetNeighbourList(UINT16 *buffer)
{
	UINT8 neighbourCount = 0;

	DEBUG_PRINTF_MAC("[NATIVE] : Calling GetNeighbour List\n");

	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if(m_NeighborTable.Neighbor[i].Status == Alive)
		{
			buffer[neighbourCount++] = m_NeighborTable.Neighbor[i].MacAddress;
		}
	}

	if(neighbourCount == 0)
	{
		DEBUG_PRINTF_MAC("[NATIVE] : Neighbour Count is 0\n");
		buffer[0] = 0;
	}

	return DS_Success;
}

DeviceStatus Mac_GetNeighbourStatus(UINT16 macAddress, UINT8 *buffer)
{
	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if(m_NeighborTable.Neighbor[i].MacAddress == macAddress)
		{
			buffer[0] = m_NeighborTable.Neighbor[i].MacAddress & 0xff;
			buffer[1] = (m_NeighborTable.Neighbor[i].MacAddress & 0xff00) >> 8;
			buffer[2] = (m_NeighborTable.Neighbor[i].ForwardLink.AvgRSSI);
			buffer[3] = (m_NeighborTable.Neighbor[i].ForwardLink.LinkQuality);
			buffer[4] = (m_NeighborTable.Neighbor[i].ForwardLink.AveDelay);
			buffer[5] = (m_NeighborTable.Neighbor[i].ReverseLink.AvgRSSI);
			buffer[6] = (m_NeighborTable.Neighbor[i].ReverseLink.LinkQuality);
			buffer[7] = (m_NeighborTable.Neighbor[i].ReverseLink.AveDelay);
			buffer[8] = (m_NeighborTable.Neighbor[i].Status);
			buffer[9] = (m_NeighborTable.Neighbor[i].PacketsReceived & 0xff);
			buffer[10] = (m_NeighborTable.Neighbor[i].PacketsReceived & 0xff00) >> 8;
			buffer[11] = (m_NeighborTable.Neighbor[i].LastHeardTime) & 0xff;
			buffer[12] = (m_NeighborTable.Neighbor[i].LastHeardTime & 0xff00) >> 8;
			buffer[13] = (m_NeighborTable.Neighbor[i].LastHeardTime & 0xff0000) >> 16;
			buffer[14] = (m_NeighborTable.Neighbor[i].LastHeardTime & 0xff000000) >> 24;
			buffer[15] = (m_NeighborTable.Neighbor[i].LastHeardTime & 0xff00000000) >> 32;
			buffer[16] = (m_NeighborTable.Neighbor[i].LastHeardTime & 0xff0000000000) >> 40;
			buffer[17] = (m_NeighborTable.Neighbor[i].LastHeardTime & 0xff000000000000) >> 48;
			buffer[18] = (m_NeighborTable.Neighbor[i].LastHeardTime & 0xff00000000000000) >> 56;
			buffer[19] = (m_NeighborTable.Neighbor[i].ReceiveDutyCycle);
			buffer[20] = (m_NeighborTable.Neighbor[i].FrameLength);
			buffer[21] = (m_NeighborTable.Neighbor[i].FrameLength & 0xff00) >> 8;

			return DS_Success;

		}
	}

	return DS_Fail;
}

#if  0
//Neighbor functions
NeighborTable* Mac_GetNeighborTable(UINT8 macID){
	if(MacName == CSMAMAC)
		return gcsmaMacObject.GetNeighborTable();
	else if(MacName == OMAC)
		return NULL;
	//return (NeighborTable *)(NULL);
}

Neighbor_t* Mac_GetNeighbor(UINT8 macID, UINT16 macAddress){

}
#endif
//Channel/freq functions


//Buffer functions
UINT8 Mac_GetBufferSize(UINT8 macID){
	if(macID == CSMAMAC)
		return  gcsmaMacObject.GetBufferSize();
	else if(macID == OMAC)
		return 0;
}

UINT8 Mac_GetNumberPendingPackets(UINT8 macID){
	if(macID == CSMAMAC)
		return gcsmaMacObject.GetSendPending();
	else if(macID == OMAC)
		return 0;
}

DeviceStatus Mac_RemovePacket(UINT8 macID, UINT8* msg){
	return DS_Fail;
}

//Mac Aggregate APIs
BOOL MacLayer_Initialize(){
	return FALSE;
}

BOOL MacLayer_UnInitialize(){
	return FALSE;
}

UINT8 MacLayer_NumberMacsSupported(){
	return FALSE;
}


