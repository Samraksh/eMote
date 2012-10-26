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
#include "Radio_decl.h"
#include "Mac_decl.h"

extern UINT16 MF_NODE_ID;

extern "C" void __cxa_pure_virtual() { while(1); }

#define MAX_APPS 4

class MAC_ID {
private:
	static UINT8 Unique_Mac_ID;
public:
	UINT8 GetUniqueMacID(){
		return ++Unique_Mac_ID;
	}
};

UINT8 MAC_ID::Unique_Mac_ID=0;

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

template <class MessageT, class ConfigT>
class MAC : public MAC_ID
{

public:
    UINT8 MacId;
    BOOL Initialized;
    UINT8 AppCount;
    MacEventHandler_t* AppHandlers[MAX_APPS];
    UINT16 MyAddress;
    ConfigT MyConfig;

	RadioEventHandler_t Radio_Event_Handler;
	UINT32 Radio_Interrupt_Mask;

	BOOL UnInitialize(void);
	DeviceStatus SetConfig (ConfigT* config);
	BOOL Send(UINT16 address, void* msg, int Size);

	void* ReceiveHandler(void* msg, UINT16 Size);
	BOOL RadioInterruptHandler(RadioInterrupt Interrupt, void* Param);
	BOOL SendAckHandler(void *msg, UINT16 Size);
	UINT16 GetAddress(){return MyAddress;	}
	BOOL SetAddress(UINT16 address){MyAddress=address; return TRUE;}
	DeviceStatus Initialize(MacEventHandler* eventHandler, UINT8* macID, UINT8 routintAppID, ConfigT* config);
};

// Define static member for the template class
//template<class MessageT>
//UINT8 MAC<void>::Unique_Mac_Id = 0;

#endif
