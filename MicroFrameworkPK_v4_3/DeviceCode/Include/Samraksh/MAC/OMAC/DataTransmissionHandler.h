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
	UINT8   m_dsn;
	UINT8   m_retryCnt, m_dwellCnt;
	UINT16  randVal;
	UINT16 m_nextDestination;
	bool m_receivedDataBeacon;
	//indicates if radio is busy
	bool   m_busy;

	//Count of times current packet has been sent
	UINT8   m_dataHeartBeats;
	static UINT32  m_nextTXCounter;
	static UINT64  m_nextTXTicks;
	static INT64 m_nbrGlobalTime;
	static UINT32 m_nbrWakeupSlot;
	UINT32  m_lastSlot;
	Message_15_4_t m_piggybackBeaconCopy;
	Message_15_4_t* m_outgoingEntryPtr;
	UINT32  m_collisionCnt;



public:
	void Initialize();
	UINT64 NextEvent(UINT32 currentSlotNum);
	//UINT64 NextEvent(UINT64 currentTicks);
	void ExecuteEvent(UINT32 currentSlotNum);
	UINT8 ExecuteEventDone();
	void PostExecuteEvent();

	void DataBeaconReceive(UINT8 type, Message_15_4_t *msg, UINT8 size);
	void ScheduleDataPacket();
	bool Send();

};

UINT32 DataTransmissionHandler::m_nextTXCounter = 0;
UINT64 DataTransmissionHandler::m_nextTXTicks = 0;
INT64 DataTransmissionHandler::m_nbrGlobalTime = 0;
UINT32 DataTransmissionHandler::m_nbrWakeupSlot = 0;

#endif /* DATATRANSMISSIONHANDLER_H_ */
