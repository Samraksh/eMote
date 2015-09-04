/*
 * DataReceptionHandler.h
 *
 *  Created on: Apr 30, 2013
 *      Author: Mukundan
 */

#include <Samraksh/Message.h>
//#include <Samraksh/HALTimer.h>
#include "OMACConstants.h"
#include "Handlers.h"
#include "Scheduler.h"

#ifndef DATARECEPTIONHANDLER_H_
#define DATARECEPTIONHANDLER_H_

/*
 *
 */
class DataReceptionHandler: public EventHandler {
	UINT8 RadioID;
	UINT8 MacID;

	BOOL m_busy, m_shldWakeup;
	UINT32 m_dataInterval;
	/* the source node we're expecting during dwell time, to filter out messages, accidentally sent
	 * back to back by another sender, that stops our dwell time */
	UINT16 	m_dwellSource;
	/*for calculateConversionTask*/

	Message_15_4_t 	dataBeaconBuffer, receiveMsgBuffer;
	//Message_15_4_t 	debugMsg;
	Message_15_4_t  *dataBeaconBufferPtr;
	//UINT8 m_dwellCnt ;
	bool m_receivedDataPacket, m_efdDetected;
	UINT16 	m_wakeupCnt, m_idleListenCnt, m_collisionCnt, m_overhearCnt;
	UINT16	m_sfdCnt;
	//wakeup slots that receive at least one packet
	UINT16 	m_receivedSlotCnt, m_reportPeriod;

	UINT32	m_lastBeaconRequestSlot;
	UINT32	m_nextWakeupSlot, m_seedUpdateInterval;
	UINT16	m_nextSeed, m_mask;

	UINT32	wakeupSlot, wakeupTime, scheduledWakeupTime;
	//SeedGenerator m_seedGenerator;

public:
	void Initialize(UINT8 radioID, UINT8 macID);
	UINT16 NextEvent(UINT32 currentSlotNum);
	//UINT64 NextEvent(UINT64 currentTicks);
	void ExecuteEvent(UINT32 currentSlotNum);
	UINT8 ExecuteEventDone();
	void PostExecuteEvent();
	void SetWakeup(bool shldWakeup);

	bool SendDataBeacon(bool sendPiggyback);
};


#endif /* DATARECEPTIONHANDLER_H_ */
