/*
 * 	Name 		: MAC.h
 *
 *  Author 		: mukundan.sridharan, nived.sivadas
 *
 *  Description : Common MAC Layer Base Class Definition, v 1.0 Aug 28, 2012
 *  			  Modified some of the return types etc, v1.01	Aug 30, 2012
 */


#ifndef _MAC_H_
#define _MAC_H_
#include <tinyhal.h>
#include "Hal_util.h"
#include "Mac_decl.h"
#include <Samraksh\Message.h>
#include <Samraksh\Neighbors.h>
#include <Samraksh\Buffer.h>
#include <Samraksh\Radio_decl.h>


typedef Buffer_15_4<8> Buffer_15_4_t;

#define MAX_APPS 4

class MAC_ID {
private:
	static UINT8 Unique_Mac_ID;
public:
	UINT8 GetUniqueMacID(){
		return ++Unique_Mac_ID;
	}
};

UINT8 MAC_ID::Unique_Mac_ID = 0;

template <class MessageT, class ConfigT>
class MAC : public MAC_ID
{
private:
	static MacEventHandler_t* AppHandlers[MAX_APPS];
	static UINT8 AppIDIndex;
	UINT16 MyAddress;

public:
	UINT16 MaxPayload;
	UINT8 macName;
	UINT8 radioName;
	BOOL Initialized;
	UINT8 AppCount;

	ConfigT MyConfig;

	RadioEventHandler_t Radio_Event_Handler;
	UINT32 Radio_Interrupt_Mask;

	BOOL UnInitialize(void);
	DeviceStatus SetConfig (ConfigT* config);
	BOOL Send(UINT16 address, void* msg, int Size);
//	BOOL SendTimeStamped(UINT16 address, void* msg, int Size, UINT32 eventTime);

	void* ReceiveHandler(void* msg, UINT16 Size);
	BOOL RadioInterruptHandler(RadioInterrupt Interrupt, void* Param);
	BOOL SendAckHandler(void *msg, UINT16 Size);
	UINT16 GetSendPending();
	UINT16 GetReceivePending();

	BOOL SetMyAddress(UINT16 address){
		this->MyAddress = address;
	}
	UINT16 GetMyAddress(){
		return this->MyAddress;
	}

	/*void SetMaxPayload(UINT16 payload)
	{
		this->MaxPayload = payload;
	}

	UINT16 GetMaxPayload()
	{
		return this->MaxPayload;
	}*/

	NeighborTable* GetNeighborTable();
	Neighbor_t* GetNeighbor(UINT16 macAddress);

	DeviceStatus Initialize(MacEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioName, ConfigT* config)
	{
		/*if(routingAppID > MAX_APPS){
			return DS_Fail;
		}
		AppIDIndex = routingAppID;
		AppHandlers[AppIDIndex] = eventHandler;
		this->macName = macName;
		this->radioName = radioName;
		//this->SetConfig(config);

		return DS_Success;*/
	}

	BOOL SetAppHandlers(MacEventHandler* handler)
	{
		if(handler == NULL){
			return FALSE;
		}

		UINT8 TempAppIdIndex = GetAppIdIndex();
		hal_printf("SetAppHandlers %u\n", TempAppIdIndex);
		if(TempAppIdIndex >= MAX_APPS){
			return FALSE;
		}

		AppHandlers[TempAppIdIndex] = handler;
		return TRUE;
	}

	MacEventHandler* GetAppHandler(UINT8 MacIndex)
	{
		if(MacIndex >= MAX_APPS){
			return (MacEventHandler*)NULL;
		}
		return AppHandlers[MacIndex];
	}

	void SetAppIdIndex(UINT8 AppIDIndex)
	{
		this->AppIDIndex = AppIDIndex;
	}

	UINT8 GetAppIdIndex()
	{
		return AppIDIndex;
	}
};


template<class MessageT, class ConfigT>
UINT8 MAC<MessageT, ConfigT>::AppIDIndex = 0;

template<class MessageT, class ConfigT>
MacEventHandler_t* MAC<MessageT, ConfigT>::AppHandlers[MAX_APPS] = {NULL,NULL,NULL};

#endif
