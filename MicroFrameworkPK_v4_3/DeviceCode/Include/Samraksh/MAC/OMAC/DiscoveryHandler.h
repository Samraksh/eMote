/*
 * DiscoveryTimeSync.h
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan
 */

#ifndef DISCOVERYTIMESYNCHANDLER_H_
#define DISCOVERYTIMESYNCHANDLER_H_

#include <Samraksh/Message.h>
//#include <Samraksh/HALTimer.h>
#include <Samraksh/VirtualTimer.h>
#include "OMACConstants.h"
#include "Handlers.h"


/*
 *
 */
typedef struct MessageCacheEntry {
	Message_15_4_t*  msgPtr;
	UINT8   length;
#ifdef BENCHMARK
	UINT32  insertedTime;
#endif
} MessageCacheEntry_t;

/*
 *
 */
class DiscoveryHandler: public EventHandler {
 public:


	UINT32 discoInterval;
	UINT16 m_seed;
	UINT32 m_nextFrame;


	//Message_15_4_t m_discoveryMsgBuffer;
	//Message_15_4_t *m_processedMsg;
	//DiscoveryMsg_t *m_discoveryMsg;

	UINT16	m_period1, m_period2;

    //private methods
	BOOL ShouldBeacon();
	void Beacon1();
	void BeaconN();
	DeviceStatus Beacon(RadioAddress_t, Message_15_4_t *);
 // public:
	void Initialize(UINT8 radioID, UINT8 macID);
	void BeaconAckHandler(Message_15_4_t* msg, UINT8 len, NetOpStatus success);
	void BeaconNTimerHandler();

	UINT64 NextEvent();
  	UINT64 NextEventinSlots(const UINT64 &currentSlotNum);
	//UINT64 NextEvent(UINT64 currentTicks);
  	void ExecuteEvent();
  	UINT8 ExecuteEventDone();
  	void PostExecuteEvent();

  	void CreateMessage(DiscoveryMsg_t* discoveryMsg);


  	void SetParentSchedulerPtr(void * scheduler);
  	DeviceStatus Receive(RadioAddress_t source, DiscoveryMsg_t* disMsg);
  	DeviceStatus Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time);
  	void SetSeed(UINT16 seed, UINT32 nextFrame){
  		m_seed = seed;
  		m_nextFrame = nextFrame;
  	}

  	UINT16 GetSeed()
  	{
  		return m_seed;
  	}

  	~DiscoveryHandler();
};





#endif /* DISCOVERYTIMESYNCHANDLER_H_ */
