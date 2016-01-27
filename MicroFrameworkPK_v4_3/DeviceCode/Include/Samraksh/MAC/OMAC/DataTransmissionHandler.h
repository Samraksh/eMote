/*
 * DataTransmissionHandler.h
 *
 *  Created on: Apr 30, 2013
 *      Author: Mukundan
 */

#ifndef DATATRANSMISSIONHANDLER_H_
#define DATATRANSMISSIONHANDLER_H_

#include <tinyhal.h>
#include <Samraksh/Message.h>
//#include <Samraksh/HALTimer.h>
#include "OMACConstants.h"
#include "Handlers.h"
//#include "Scheduler.h"

/*
 *
 */
class DataTransmissionHandler: public EventHandler {

	Message_15_4_t m_piggybackBeaconCopy;
	Message_15_4_t* m_outgoingEntryPtr; //Pointer to the packet to be sent next. Set by schedule data packet

	Message_15_4_t m_TXMsgBuffer;
	DataMsg_t *m_TXMsg;
	BOOL isDataPacketScheduled;
	UINT8 currentAttempt;
	UINT8 maxRetryAttempts;

public:
	void Initialize();
	UINT64 NextEvent();
	//UINT64 NextEvent(UINT64 currentTicks);
	void ExecuteEvent();
	void ExecuteEventHelper();
	//UINT8 ExecuteEventDone();
	void PostExecuteEvent();

	void DataBeaconReceive(UINT8 type, Message_15_4_t *msg, UINT8 size);
	BOOL ScheduleDataPacket(UINT8 _skipperiods);
	bool Send();
	void SendRetry();
	void HardwareACKHandler();
	void SendACKHandler();
	void ReceiveDATAACK(UINT16 address);

	UINT64 GetTxTicks();
	UINT32 GetTxCounter();
	void SetTxTicks(UINT64);
	void SetTxCounter(UINT32);

};


#endif /* DATATRANSMISSIONHANDLER_H_ */
