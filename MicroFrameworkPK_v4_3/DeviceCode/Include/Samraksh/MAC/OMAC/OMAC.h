/*
 * Copyright: The Samraksh Company
 * Author: Mukundan.Sridharan, Nived.Sivadas, Bora Karaoglu
 * Description :  OMAC Implementation, v 2.0
 * Created on: April, 2015
 */


#ifndef OMAC_H_
#define OMAC_H_

#define TWO_NODES_TX_RX
//#define OMAC_DEBUG_GPIO

#if defined(TWO_NODES_TX_RX)
//#define def_Neighbor2beFollowed
/* // Nathan's nodes
//#define TXNODEID 18134
//#define RXNODEID 20181
*/
//Bora's Nodes
//#define TXNODEID 30906
#define TXNODEID 4028
#define RXNODEID 32696
/*// Ananth's Nodes
#define TXNODEID 3505
#define RXNODEID 6846
*/
#endif

#include <Samraksh/MAC.h>
#include <Samraksh/Radio_decl.h>
#include <Samraksh/Message.h>
#include <Samraksh/Buffer.h>
#include <Samraksh/Neighbors.h>

#include "OMACConstants.h"
#include "Scheduler.h"
#include "Handlers.h"


typedef Buffer_15_4<8> Buffer_15_4_t;

//#define TIMESYNC_SENDPIN 0 // 3 // PA3 J11-6
//#define TIMESYNC_RECEIVEPIN 31 // 23 //PB7 J11-10



// Ananth




//#define TXNODEID 30906
//#define RXNODEID 4028

/*
 *
 */
class OMACType: public MAC<Message_15_4_t, MacConfig>{
  private:
	//Underlying radio variables
	static const UINT8 NumberRadios =1;

	BOOL RadioAckPending;
	UINT8 m_recovery;
	BOOL flushTimerRunning;

	UINT8 CurrentActiveApp;
	UINT16 MyID;



	//Protocol variables
//	static const UINT8 SlotLength = 8; //slot Length in milliseconds

  public:

#ifdef def_Neighbor2beFollowed
	UINT16 Neighbor2beFollowed;
#endif

#ifdef def_Neighbor2beFollowed2
	UINT16 Neighbor2beFollowed1, Neighbor2beFollowed2;
#endif
	//Neighbors
	//NeighborTable m_NeighborTable;

	//Buffer variables
	//Buffer_15_4_t m_send_buffer;
	//Buffer_15_4_t m_receive_buffer;


	// Pointer to the outgoing message
	Message_15_4_t* tx_msg_ptr;
	// Pointer to the incoming message
	Message_15_4_t* rx_msg_ptr;
	volatile UINT16 tx_length;
	volatile UINT16 rx_length;

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

	void SetCurrentActiveApp(UINT8 CurrentActiveApp){
		this->CurrentActiveApp = CurrentActiveApp;
	}

	UINT8 GetCurrentActiveApp(){
		return this->CurrentActiveApp;
	}

	void SetMyID(UINT16 MyID){
		this->MyID = MyID;
	}

	UINT16 GetMyID(){
		return this->MyID;
	}

	//Override base class methods here, implement them later in cpp file
	DeviceStatus Initialize(MacEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioID, MacConfig *config);
	DeviceStatus SetConfig(MacConfig *config);
	BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int size, UINT32 eventTime);
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
	UINT8 GetBufferSize();
	UINT16 GetSendPending();
	UINT16 GetReceivePending();
};

extern OMACType g_OMAC;

#endif /* OMAC_H_ */
