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
public:
	UINT16	m_nextSeed, m_mask; // m_nextSeed stores the next seed to be used in calculating the next wakeup slot and the m_mask is used as a mask in the pseduo random function
	UINT64 m_nextwakeupSlot;//This variable stores the wakeup time in absolute slot number
	UINT32 m_seedUpdateIntervalinSlots;//Frame Length. One reception slot is selected among this many number of slots

	UINT64 m_lastScheduledOriginTime;
	UINT64 m_currtime;
	UINT64 m_lastScheduledTargetTime;
	bool m_isreceiving;
	UINT8 m_receptionstate;
	UINT64 lastwakeupSlotUpdateTimeinTicks;

	Message_15_4_t m_ACKmsg ;

	void Initialize(UINT8 radioID, UINT8 macID);
	UINT64 NextEvent();
	void UpdateSeedandCalculateWakeupSlot(UINT64 &wakeupSlot, UINT16 &next_seed, const UINT16 &mask, const UINT32 &seedUpdateIntervalinSlots,  const UINT64 &currentSlotNum );
 	void ExecuteEvent();

 	void SendDataACK(UINT16 address);
 	void SendACKHandler();

 	void HandleRadioInterrupt();
	void PostExecuteEvent();
	void HandleEndofReception(UINT16 address);
};


#endif /* DATARECEPTIONHANDLER_H_ */
