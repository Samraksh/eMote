/*
 * Mac_Functions.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: Mukundan
 */

#include ".\CSMAMAC\csmaMAC.h"
#include <Samraksh\Mac_decl.h>

extern csmaMAC gcsmaMacObject;


Buffer_15_4_t m_send_buffer;
Buffer_15_4_t m_receive_buffer;
NeighborTable m_NeighborTable;

#define DEBUG_MACFUNCTIONS 1

#if defined(DEBUG_MACFUNCTIONS)
#define ENABLE_PIN(x,y) CPU_GPIO_EnableOutputPin(x,y)
#define SET_PIN(x,y) CPU_GPIO_SetPinState(x,y)
#define DEBUG_PRINTF_MAC(x) hal_printf(x)
#else
#define ENABLE_PIN(x,y)
#define SET_PIN(x,y)
#define DEBUG_PRINTF_MAC(x)
#endif


//Basic functions
extern UINT8 MacName;

DeviceStatus Mac_Initialize(MacEventHandler* eventHandler, UINT8* macID, UINT8 routingAppID, void* config){

	if(MacName == CSMAMAC)
		return gcsmaMacObject.Initialize(eventHandler, macID, routingAppID, (MacConfig*)config) ;
	else if(MacName == OMAC)
		//return g_OMAC.Initialize(eventHandler, macID, routingAppID, (MacConfig *) config);
		return DS_Fail;
	else
		return DS_Fail;
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

	m_receive_buffer.DropOldest(1);

	return DS_Success;

}

DeviceStatus Mac_UnInitialize(UINT8 macID){
	return DS_Fail;
}

UINT8 Mac_GetID(){
	return gcsmaMacObject.MacId ;
}

DeviceStatus Mac_Send(UINT8 macID, UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size){

	//msg is just the payload,

	if(MacName == CSMAMAC)
		return (DeviceStatus) gcsmaMacObject.Send(destAddress, dataType, msg, size);
	else if(MacName == OMAC)
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
		return DS_Fail;
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
	if(MacName == CSMAMAC)
		return  gcsmaMacObject.GetBufferSize();
	else if(MacName == OMAC)
		return 0;
}

UINT8 Mac_GetNumberPendingPackets(UINT8 macID){
	if(MacName == CSMAMAC)
		return gcsmaMacObject.GetSendPending();
	else if(MacName == OMAC)
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


