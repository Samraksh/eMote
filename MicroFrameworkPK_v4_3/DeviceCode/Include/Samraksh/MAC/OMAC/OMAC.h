/*
 * Copyright: The Samraksh Company
 * Author: Mukundan.Sridharan, Nived.Sivadas, Bora Karaoglu
 * Description :  OMAC Implementation, v 2.0
 * Created on: April, 2015
 */


#ifndef OMAC_H_
#define OMAC_H_

#include <Samraksh/MAC.h>
#include <Samraksh/Radio_decl.h>
#include <Samraksh/Message.h>
#include <Samraksh/Buffer.h>
#include <Samraksh/Neighbors.h>

#include "OMACConstants.h"
#include "Scheduler.h"
#include "Handlers.h"


typedef Buffer_15_4<8> Buffer_15_4_t;

/*
 *
 */
class OMACTypeBora: public MAC<Message_15_4_t, MacConfig>{
  private:
	//Underlying radio variables
	static const UINT8 NumberRadios =1;

	BOOL RadioAckPending;
	UINT8 m_recovery;
	BOOL flushTimerRunning;

	//Protocol variables
//	static const UINT8 SlotLength = 8; //slot Length in milliseconds

  public:
	UINT8 CurrentActiveApp;
	UINT16 MyID;
	//Neighbors
	//NeighborTable m_NeighborTable;

	//Buffer variables
	//Buffer_15_4_t m_send_buffer;
	//Buffer_15_4_t m_receive_buffer;

	UINT16 GetAddress(){return MyID;}
	UINT16 GetMaxPayload(){return MaxPayload;	}
	BOOL SetAddress(UINT16 address){MyAddress = address; return TRUE;}

	BOOL SetRadioAddress(UINT16 address){
		BOOL ret = CPU_Radio_SetAddress(this->radioName, address);
		return ret;
	}

	UINT16 GetRadioAddress(){
		UINT16 temp = CPU_Radio_GetAddress(this->radioName);
		return temp;
	}

	void SetMaxPayload(UINT16 payload){MaxPayload = payload;}

	//Override base class methods here, implement them later in cpp file
	DeviceStatus Initialize(MacEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioID, MacConfig *config);
	DeviceStatus SetConfig(MacConfig *config);
	BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int size);
	////BOOL SendTimeStamped(RadioAddress_t dest, UINT8 dataType, Message_15_4_t* msg, int Size, UINT32 eventTime);
	BOOL SendTimeStamped(UINT16 dest, UINT8 dataType, void* msg, int Size, UINT32 eventTime);
	Message_15_4_t* ReceiveHandler(Message_15_4_t* msg, int size);
	BOOL RadioInterruptHandler(RadioInterrupt Interrupt, void* Param);
	void SendAckHandler(void* msg, int Size, NetOpStatus status);
	BOOL UnInitialize(void);
	BOOL HandleBroadcastMessage(Message_15_4_t * msg);
	BOOL HandleUnicastMessage(Message_15_4_t * msg);
	Message_15_4_t* FindFirstSyncedNbrMessage();
	Message_15_4_t* FindFirstMessageForNbr(UINT16 nbr);
	void UpdateNeighborTable();
};

extern OMACTypeBora g_OMAC;

#endif /* OMAC_H_ */
