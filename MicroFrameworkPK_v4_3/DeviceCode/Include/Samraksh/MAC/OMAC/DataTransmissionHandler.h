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
	DTS_BACKOFF,
	DTS_DEST_NOT_AVAILABLE,
	DTS_SEND_INITIATION_SUCCESS,
	DTS_SEND_INITIATION_FAIL,
	DTS_SEND_FINISHED,
	DTS_WAITING_FOR_ACKS,
	DTS_RECEIVEDDATAACK,
	DTS_POSTEXECUTION,
};

//page 70 in RF231 datasheet
enum TRAC_STATUS
{
	TRAC_STATUS_SUCCESS = 0x00,
	TRAC_STATUS_SUCCESS_DATA_PENDING = 0x20,	//0010 0000 (32)
	TRAC_STATUS_SUCCESS_WAIT_FOR_ACK = 0x40,	//0100 0000 (64)
	TRAC_STATUS_CHANNEL_ACCESS_FAILURE = 0x60,	//0110 0000 (96)
	TRAC_STATUS_FAIL_TO_SEND = 0x80,			//1000 0000 (128)
	TRAC_STATUS_NO_ACK = 0xA0,					//1010 0000 (160)
	TRAC_STATUS_INVALID	= 0xE0,					//1110 0000 (224)
};


/*
 *
 */
class DataTransmissionHandler: public EventHandler {

	//Message_15_4_t m_piggybackBeaconCopy;
	Message_15_4_t* m_outgoingEntryPtr; //Pointer to the packet to be sent next. Set by schedule data packet
	//UINT8 m_outgoingEntryPtr_pos; //Position of the packet in the buffer to be sent next set by next_event
	UINT16 m_outgoingEntryPtr_dest;

	//Message_15_4_t m_TXMsgBuffer;
	//DataMsg_t *m_TXMsg;
	BOOL isDataPacketScheduled;
	UINT8 m_currentSlotRetryAttempt;

	BOOL m_RANDOM_BACKOFF;
	UINT16 m_backoff_seed;
	UINT16 m_backoff_mask;

	DataTransmissionHandlerStates txhandler_state;
	UINT64 CalculateNextTxMicro(UINT16 dest);
	void SelectRetrySlotNumForNeighborBackOff();

public:
	void Initialize();
	UINT64 NextEvent();
	//UINT64 NextEvent(UINT64 currentTicks);
	void ExecuteEvent();
	void ExecuteEventHelper();
	//UINT8 ExecuteEventDone();
	void PostExecuteEvent();

	//BOOL ScheduleDataPacket(UINT8 _skipperiods);

	void DropPacket();
	bool Send();
	void SendRetry();
	void SendACKHandler(Message_15_4_t* rcv_msg, UINT8 radioAckStatus);
	void ReceiveDATAACK(UINT16 address);
	void FailsafeStop();

	UINT64 CalculateNextRXOpp(UINT16 dest);
	BOOL UpdateNeighborsWakeUpSlot(UINT16 dest, UINT8 _skipperiods);
};


#endif /* DATATRANSMISSIONHANDLER_H_ */
