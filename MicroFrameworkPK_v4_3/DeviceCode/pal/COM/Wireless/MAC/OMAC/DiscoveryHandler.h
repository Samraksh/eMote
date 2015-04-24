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
//#include "FTSPTimeSync.h"
//#include "CMaxTimeSync.h"


typedef struct MessageCacheEntry {
	Message_15_4_t*  msgPtr;
	UINT8   length;
#ifdef BENCHMARK
	UINT32  insertedTime;
#endif
} MessageCacheEntry_t;

class DiscoveryHandler: public SlotHandler {
  private:
	UINT8 RadioID;
	UINT8 MacID;
#ifdef OMAC_DEBUG
	UINT8		m_transitionFromDone = 0;
#endif
	void * m_parentScheduler;
	BOOL 		m_busy, m_receivedPiggybackBeacon, m_lockSeed;
	// Wenjie: worst case interval between two consecutive received beacons
	// Declare oneself as root if heartBeat >= discoInterval * ROOT_TIMEOUT
	UINT32	discoInterval;
	UINT16 m_seed;
	UINT32 m_nextFrame;
	// the number of entries after clock wrap
	UINT32		m_tableEntriesUnderTransition[MAX_NBR_SIZE];
	UINT32 	m_idxForComputation;
	BOOL		m_inTransition;

	/*
		 We do linear regression from localTime to timeOffset (globalTime - localTime).
		 This way we can keep the slope close to zero (ideally) and represent it
		 as a float with high precision.

		 timeOffset - offsetAverage = skew * (localTime - localAverage)
		 timeOffset = offsetAverage + skew * (localTime - localAverage)
		 globalTime = localTime + offsetAverage + skew * (localTime - localAverage)
	 */
	float   m_skew;
	UINT32  m_localAverage, counterOffsetAvg, radioTiming;
	UINT32  m_offsetAverage;

	Message_15_4_t m_discoveryMsgBuffer;
	Message_15_4_t *m_processedMsg;
	DiscoveryMsg_t *m_discoveryMsg;

	UINT16	m_p1, m_p2;

    //private methods
	BOOL ShouldBeacon();
	void Beacon1();
	void BeaconN();
	DeviceStatus Beacon(RadioAddress_t, Message_15_4_t *);
  public:
	void Initialize(UINT8 radioID, UINT8 macID);
	void StartBeaconNTimer(BOOL oneshot, UINT64 delay);	//Start BeaconN Timer
	void BeaconNTimerHandler(void* Param); //Handler BeaconN Timer firing
	void BeaconAckHandler(Message_15_4_t* msg, UINT8 len, NetOpStatus success);
  	UINT32 NextSlot(UINT32 slotNum);
  	void ExecuteSlot(UINT32 slotNum);
  	UINT8 ExecuteSlotDone();
  	void PostExecuteSlot();
  	void SetWakeup(BOOL shldWakeup);
  	void SetParentSchedulerPtr(void * scheduler);
  	DeviceStatus Receive(Message_15_4_t* msg, void* payload, UINT8 len);
  	DeviceStatus Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time);
  	void SetSeed(UINT16 seed, UINT32 nextFrame){
  		m_seed = seed;
  		m_nextFrame = nextFrame;
  	}
};





#endif /* DISCOVERYTIMESYNCHANDLER_H_ */
