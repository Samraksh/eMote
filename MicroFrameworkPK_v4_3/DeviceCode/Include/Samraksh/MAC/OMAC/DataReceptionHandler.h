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
	Message_15_4_t  dataBeaconBuffer,receiveMsgBuffer;
	static UINT16 currentRandomValue;

public:
	UINT16	m_nextSeed, m_mask; // m_nextSeed stores the next seed to be used in calculating the next wakeup slot and the m_mask is used as a mask in the pseduo random function
	UINT32 m_nextwakeupSlot;//This variable stores the wakeup time in absolute slot number
	//UINT64 m_nextwakeupTicks; //This variable stores the absolute tick number to be used in the next
	UINT32 m_seedUpdateIntervalinSlots;


	void Initialize(UINT8 radioID, UINT8 macID);
	UINT64 NextEvent();
	void UpdateSeedandCalculateWakeupSlot(UINT32 &wakeupSlot, UINT16 &next_seed, const UINT16 &mask, const UINT32 &seedUpdateIntervalinSlots,  const UINT32 &currentSlotNum );
 	void ExecuteEvent();

	void PostExecuteEvent();
	UINT32 GetWakeupSlot();
	void SetWakeupSlot(UINT32);
	void SetCurrentRandomValue(UINT16 tmp_randVal)
	{
		currentRandomValue = tmp_randVal;
	}

	UINT16 GetCurrentRandomValue()
	{
		return currentRandomValue;
	}
};

UINT16 DataReceptionHandler::currentRandomValue = 0;

#endif /* DATARECEPTIONHANDLER_H_ */
