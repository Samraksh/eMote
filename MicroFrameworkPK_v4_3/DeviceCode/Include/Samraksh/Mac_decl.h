#ifndef _MAC_DECL_H_
#define _MAC_DECL_H_

#include "Hal_util.h"
#include "Radio_decl.h"
#include "Neighbors.h"

//typedef UINT16 MacAddress_t;

enum MacNames
{
	CSMAMAC,
	OMAC,
};

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
    UINT32 NeighborLivenessDelay;
};
//struct MacConfig;

// Typedef defining the signature of the receive function
//typedef  void (*MacReceiveFuncPtrType) (void *msg, UINT16 Size, UINT16 Src, BOOL Unicast, UINT8 RSSI, UINT8 LinkQuality);
typedef void (*MacReceiveFuncPtrType) (void *msg, UINT16 arg1);

typedef void (*NeighborChangeFuncPtrType) (INT16 arg2);

// Typedef defining the signature of the send function
typedef void (*SendAckFuncPtrType) (void* msg, UINT16 Size, NetOpStatus status, UINT8 radioAckStatus);

typedef  class MacEventHandler{

public:
	MacReceiveFuncPtrType ReceiveHandler;
	SendAckFuncPtrType SendAckHandler;
	NeighborChangeFuncPtrType neighborHandler;

	void SetReceiveHandler(MacReceiveFuncPtrType receive_handler)
	{
		this->ReceiveHandler = receive_handler;
	}

	void SetSendAckHandler(SendAckFuncPtrType send_ack_handler)
	{
		this->SendAckHandler = send_ack_handler;
	}

	void SetNeighborChangeHandler(NeighborChangeFuncPtrType neighbor_change_handler)
	{
		this->neighborHandler = neighbor_change_handler;
	}

	MacReceiveFuncPtrType GetReceiveHandler()
	{
		return this->ReceiveHandler;
	}

	SendAckFuncPtrType GetSendAckHandler()
	{
		return this->SendAckHandler;
	}

	NeighborChangeFuncPtrType GetNeighborChangeHandler()
	{
		return this->neighborHandler;
	}

}MacEventHandler_t;


UINT8 currentMacName;

//Basic functions
DeviceStatus Mac_Initialize(MacEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioName, void* macConfig);
DeviceStatus Mac_Reconfigure(void* config);
UINT16 Mac_GetRadioAddress();
BOOL Mac_SetRadioAddress(UINT16 address);
DeviceStatus Mac_UnInitialize();
UINT8 Mac_GetID();
DeviceStatus Mac_Send(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size); //msg is just the payload,
DeviceStatus Mac_SendTimeStamped(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size, UINT32 timeStamp); //msg is just the payload,
DeviceStatus Mac_Config(void *macConfig);

//Neighbor functions
//NeighborTable* Mac_GetNeighborTable(UINT8 macID);
//Neighbor_t* Mac_GetNeighbor(UINT8 macID, UINT16 macAddress);

// Get the neighbor list
DeviceStatus Mac_GetNeighborList(UINT16 *buffer);
DeviceStatus Mac_GetNeighborStatus(UINT16 macAddress, UINT8 *buffer);

//Channel/freq functions
DeviceStatus Mac_GetNextPacket(UINT8 **managedBuffer);
//Buffer functions
UINT8 Mac_GetSendBufferSize();
UINT8 Mac_GetReceiveBufferSize();
UINT8 Mac_GetPendingPacketsCount_Send();
UINT8 Mac_GetPendingPacketsCount_Receive();
DeviceStatus Mac_RemovePacket(UINT8* msg);

//MAC Aggregate APIs
BOOL MacLayer_Initialize();
BOOL MacLayer_UnInitialize();
UINT8 MacLayer_NumberMacsSupported();


#endif
