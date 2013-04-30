/*
 * csmaMAC.h
 *
 *  Created on: Sep 27, 2012
 *      Author: Nived
 */

#ifndef CSMAMAC_H_
#define CSMAMAC_H_

/*********************************  Included header files **************************************/

#include <tinyhal.h>
#include <Samraksh\Message.h>
#include <Samraksh\Neighbors.h>
#include <Samraksh\MAC.h>
#include <Samraksh\Buffer.h>
#include <Samraksh\Radio_decl.h>

//typedef  struct Buffer<Message_15_4_t> Buffer_15_4_t;

#define LEVEL_0_RECOVER (1 << 0)
#define LEVEL_1_RECOVER (1 << 1)
#define LEVEL_2_RECOVER (1 << 2)

extern Buffer_15_4_t m_send_buffer;
extern Buffer_15_4_t m_receive_buffer;
extern NeighborTable m_NeighborTable;



/*******************************************************************************************************/

// core mac class definitions extends the MAC class and implements its virtual functions
// MAC does not have a template because of function pointer issues as they were primarily for the return type of handlers

class csmaMAC: public MAC<Message_15_4_t, MacConfig>
{
	//Buffer variable
	UINT8 CurrentActiveApp;
	BOOL RadioAckPending;
	UINT8 m_recovery;
public:
	UINT16 GetAddress(){return MyAddress;}
	UINT16 GetMaxPayload(){return MaxPayload;	}
	BOOL SetAddress(UINT16 address){MyAddress=address; return TRUE;}
	void SetMaxPayload(UINT16 payload){MaxPayload = payload;}


	BOOL Resend(void* msg, int Size);
	DeviceStatus Initialize(MacEventHandler* eventHandler, UINT8* macIDs, UINT8 routingAppID, MacConfig *config);
	DeviceStatus SetConfig(MacConfig *config);
	BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int Size);
	Message_15_4_t* ReceiveHandler(Message_15_4_t* msg, int Size);
	BOOL RadioInterruptHandler(RadioInterrupt Interrupt, void* Param);
	void SendAckHandler(void* msg, int Size, NetOpStatus status);
	BOOL UnInitialize(void);
	BOOL HandleBroadcastMessage(Message_15_4_t * msg);
	BOOL HandleUnicastMessage(Message_15_4_t * msg);
	BOOL HandlePromiscousMessage(Message_15_4_t * msg);
	void SendToRadio();
	void UpdateNeighborTable();
	UINT8 GetBufferSize();
	UINT16 GetSendPending(){ return m_send_buffer.GetNumberMessagesInBuffer();}
	UINT16 GetReceivePending(){return m_receive_buffer.GetNumberMessagesInBuffer();}

	NeighborTable* GetNeighborTable(){return &m_NeighborTable;}
	Neighbor_t* GetNeighbor(UINT16 macAddress){return m_NeighborTable.GetNeighborPtr(macAddress); }

};




#endif /* CSMAMAC_H_ */
