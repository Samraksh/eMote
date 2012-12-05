#ifndef _MAC_DECL_H_
#define _MAC_DECL_H_

#include "Hal_util.h"
#include "Radio_decl.h"
#include "Neighbors.h"

//typedef UINT16 MacAddress_t;

#define MAC_BROADCAST_ADDRESS RADIO_BROADCAST_ADDRESS

struct MacConfig {
	UINT16 FCF;
	UINT16 DestPAN;
	UINT8 Network;
	BOOL CCA;
    UINT8 NumberOfRetries;
    UINT8 CCASenseTime;
    UINT8 BufferSize;
    UINT8 RadioID;
};
//struct MacConfig;

// Typedef defining the signature of the receive function
typedef  void (*MacReceiveFuncPtrType) (void *msg, UINT16 Size);

// Typedef defining the signature of the send function
typedef void (*SendAckFuncPtrType) (void* msg, UINT16 Size, NetOpStatus status);

typedef  class MacEventHandler{

public:
	MacReceiveFuncPtrType RecieveHandler;
	SendAckFuncPtrType SendAckHandler;

	void SetRecieveHandler(MacReceiveFuncPtrType recieve_handler)
	{
		this->RecieveHandler = recieve_handler;
	}

	void SetSendAckHandler(SendAckFuncPtrType send_ack_handler)
	{
		this->SendAckHandler = send_ack_handler;
	}

}MacEventHandler_t;


//Basic functions
DeviceStatus MAC_Initialize(MacEventHandler* eventHandler, UINT8* macID, UINT8 routingAppID, void* macConfig);
DeviceStatus MAC_UnInitialize(UINT8 macID);
UINT8 Mac_GetID();
DeviceStatus Mac_Send(UINT8 macID, UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size); //msg is just the payload,
DeviceStatus Mac_Config(UINT8 macID, void *macConfig);

//Neighbor functions
NeighborTable* MAC_GetNeighborTable(UINT8 macID);
Neighbor_t* MAC_GetNeighbors(UINT8 macID, UINT8 macAddress);

//Channel/freq functions

//Buffer functions
UINT8 Mac_GetBufferSize(UINT8 macID);
UINT8 Mac_GetNumberPendingPackets(UINT8 macID);
DeviceStatus Mac_RemovePacket(UINT8 macID, UINT8* msg);

//MAC Aggregate APIs
BOOL MacLayer_Initialize();
BOOL MacLayer_UnInitialize();
UINT8 MacLayer_NumberMacsSupported();


#endif
