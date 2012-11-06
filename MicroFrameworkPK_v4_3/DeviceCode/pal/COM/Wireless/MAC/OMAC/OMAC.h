/*
 * Copyright The Samraksh Company
 *
 * Author: Mukundan.Sridharan, Nived.Sivadas
 *
 * Description :  Radio HAL APIs, v 1.0
 *
 *  Created on: Aug 30, 2012
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

class OMAC: public MAC <Message_15_4_t, MacConfig>{

  private:
	//Underlying radio variables
	static const UINT8 NumberRadios =1;
	UINT8 RadioIDs[NumberRadios];
	UINT8 CurrentActiveApp;

	//Neighbors
	NeighborTable m_NeighborTable;

	//Buffer variables
	Buffer_15_4_t m_send_buffer;
	Buffer_15_4_t m_receive_buffer;

	//Protocol variables
	static const UINT8 SlotLength = 8; //slot Length in milliseconds

  public:

	OMACScheduler m_omac_scheduler;

	//Override base class methods here, implement them later in cpp file
	DeviceStatus Initialize(MacEventHandler* eventHandler, UINT8* macIDs, UINT8 routingAppID, MacConfig *config);
	DeviceStatus SetConfig(MacConfig *config);
	BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int size);
	Message_15_4_t* ReceiveHandler(Message_15_4_t* msg, int size);
	BOOL RadioInterruptHandler(RadioInterrupt Interrupt, void* Param);
	void SendAckHandler(void* msg, int Size, NetOpStatus status);
	BOOL UnInitialize(void);
	BOOL HandleBroadcastMessage(Message_15_4_t * msg);
	BOOL HandleUnicastMessage(Message_15_4_t * msg);

};

OMAC g_OMAC;

#endif /* OMAC_H_ */
