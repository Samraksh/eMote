/*
 * Mac_Functions.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: Mukundan
 */

#include "csmaMAC.h"
#include <Samraksh\Mac_decl.h>

extern csmaMAC gcsmaMacObject;

//Basic functions
DeviceStatus Mac_Initialize(MacEventHandler* eventHandler, UINT8* macID, UINT8 routingAppID, void* config){

	return gcsmaMacObject.Initialize(eventHandler, macID, routingAppID, (MacConfig*)config) ;
}

DeviceStatus Mac_UnInitialize(UINT8 macID){
	return DS_Fail;
}

UINT8 Mac_GetID(){
	return gcsmaMacObject.MacId ;
}

DeviceStatus Mac_Send(UINT8 macID, UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size){
	//msg is just the payload,
	if(gcsmaMacObject.Send(destAddress, dataType, msg, size))
		return DS_Success;
	else
		return DS_Fail;
}


//Neighbor functions
NeighborTable* Mac_GetNeighborTable(UINT8 macID){
	return gcsmaMacObject.GetNeighborTable();
	//return (NeighborTable *)(NULL);
}

Neighbor_t* Mac_GetNeighbor(UINT8 macID, UINT16 macAddress){
	return gcsmaMacObject.GetNeighbor(macAddress);
	//return (Neighbor_t *)(NULL);
}

//Channel/freq functions


//Buffer functions
UINT8 Mac_GetBufferSize(UINT8 macID){
	return  gcsmaMacObject.GetBufferSize();
}
UINT8 Mac_GetNumberPendingPackets(UINT8 macID){
	return gcsmaMacObject.GetSendPending();
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


