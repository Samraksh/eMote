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


class DataTransmissionHandler: public EventHandler {
	UINT8   m_dsn;
	UINT8   m_retryCnt, m_dwellCnt;
	UINT16  randVal;
	UINT16 m_nextDestination;
	bool m_receivedDataBeacon;
	bool   m_busy;   //indicates if radio is busy.

	  //the # of times the current packet has been sent
	UINT8   m_dataHeartBeats;
	UINT32  m_nextTXCounter;
	UINT64  m_nextTXTicks;
	UINT32  m_lastSlot;
	Message_15_4_t m_piggybackBeaconCopy;
	Message_15_4_t* m_outgoingEntryPtr;
	UINT32  m_collisionCnt;



public:
	void Initialize();
	UINT16 NextEvent(UINT32 slotNum);
	void ExecuteEvent(UINT32 slotNum);
	UINT8 ExecuteEventDone();
	void PostExecuteEvent();
	void SetWakeup(bool shldWakeup);

	bool IsSynced(UINT16 nbr);
	void DataBeaconReceive(UINT8 type, Message_15_4_t *msg, UINT8 size);
	void ScheduleDataPacket();
	bool Send();
};
#endif /* DATATRANSMISSIONHANDLER_H_ */
