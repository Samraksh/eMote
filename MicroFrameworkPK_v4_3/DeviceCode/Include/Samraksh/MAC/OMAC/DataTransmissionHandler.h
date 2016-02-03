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
enum DataTransmissionHandlerStates{
	DTS_EXECUTE_START,
	DTS_RADIO_START_SUCCESS,
	DTS_RADIO_START_FAILED,
	DTS_SEND_INITIATION_SUCCESS,
	DTS_SEND_INITIATION_FAIL,
	DTS_SEND_FINISHED,
	DTS_WAITING_FOR_ACKS,
	DTS_RECEIVEDDATAACK,
	DTS_POSTEXECUTION,
};

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

	UINT8 m_currentFrameRetryAttempt;

	DataTransmissionHandlerStates txhandler_state;
public:
	void Initialize();
	UINT64 NextEvent();
	//UINT64 NextEvent(UINT64 currentTicks);
	void ExecuteEvent();
	void ExecuteEventHelper();
	//UINT8 ExecuteEventDone();
	void PostExecuteEvent();

	BOOL ScheduleDataPacket(UINT8 _skipperiods);
	bool Send();
	void SendRetry();
	void HardwareACKHandler();
	void SendACKHandler();
	void ReceiveDATAACK(UINT16 address);
	void FailsafeStop();
};


#endif /* DATATRANSMISSIONHANDLER_H_ */
