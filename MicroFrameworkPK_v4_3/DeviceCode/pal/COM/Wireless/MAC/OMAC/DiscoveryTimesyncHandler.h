/*
 * DiscoveryTimeSync.h
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan
 */

#ifndef DISCOVERYTIMESYNCHANDLER_H_
#define DISCOVERYTIMESYNCHANDLER_H_

#include <Samraksh/Message.h>
#include "OMACConstants.h"
#include "Handlers.h"
#include "FTSPTimeSync.h"


typedef struct MessageCacheEntry {
	Message_15_4_t*  msgPtr;
	UINT8   length;
#ifdef BENCHMARK
	UINT32  insertedTime;
#endif
} MessageCacheEntry_t;

typedef struct TimeSyncSample {
	UINT64    localTime;
	INT64     timeOffset; // globalTime - localTime
} TimeSyncSample_t;

typedef struct TableItem
{
	UINT16    nodeID;
	//the seed we have from in the control message last received
	UINT16  lastSeed;
	//the starting slot of the frame immediately after last seed's update
	//e.g. if seeds are updated every 8 frames, then the following invariant holds:
	//nextFrameAfterSeedUpdate = 8N + 1, N = 0, 1, 2...
	UINT32  nextFrameAfterSeedUpdate;
	UINT16  dataInterval;
	UINT16  radioStartDelay;
	UINT16  counterOffset;
	UINT8   numErrors;
	UINT8     size;
	bool    isInTransition;
	UINT32  localAvg;
	INT32   offsetAvg;
	float   skew;
	TimeSyncSample_t samples[MAX_ENTRIES];
} TableItem_t;


class DiscoveryTimesyncHandler: public SlotHandler {
  private:

#ifdef OMAC_DEBUG
	UINT8		m_transitionFromDone = 0;
#endif
	void * m_parentScheduler;
	bool 		m_busy, m_receivedPiggybackBeacon, m_lockSeed;
	// Wenjie: worst case interval between two consecutive received beacons
	// Declare oneself as root if heartBeat >= discoInterval * ROOT_TIMEOUT
	UINT32	discoInterval;
	// table to store tsync beacons for up to NBR_TBL_SZ neighbors
	TableItem   m_beaconTable[MAX_NBR_SIZE];
	// the number of entries after clock wrap
	UINT32		m_tableEntriesUnderTransition[MAX_NBR_SIZE];
	UINT32 	m_idxForComputation;
	bool		m_inTransition;

	UINT8		m_nbrCnt;
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
	UINT32 	m_lastReceiveTime; // Wenjie: used to filter out consecutive timesync beacons at the
								 // two ends of a timesyenc period
	Message_15_4_t m_timeSyncBeaconBuffer;
	Message_15_4_t *m_processedMsg;
	TimeSyncMsg *m_timeSyncBeacon;

	UINT16	m_p1, m_p2;
	UINT8		m_updateIdx;

    //private methods
	bool ShouldBeacon();
	void Beacon1();
	void BeaconN();
	DeviceStatus Beacon(RadioAddress_t, Message_15_4_t *);
	// neighbor table util functions
	UINT8 GetFreeIdx();
	DeviceStatus NbrToIdx(UINT16 nodeid, UINT8* idx);
	void ClearNeighbor(RadioAddress_t nodeID);
	void ClearTable();
	// util functions for time sync beaocn processing
	DeviceStatus IsSynced(RadioAddress_t);
	void CleanUpAfterProcessing();
	void ProcessMsg();
	void CalculateConversion();
  public:
	FTSPTimeSync_t m_FTSPTimeSync;

	void Initialize();
	void StartBeaconNTimer(bool oneshot, UINT64 delay);	//Start BeaconN Timer
	void BeaconNTimerHandler(void* Param); //Handler BeaconN Timer firing
  	UINT16 NewSlot(UINT32 slotNum);
  	void ExecuteSlot(UINT32 slotNum);
  	UINT8 ExecuteSlotDone();
  	void PostExecuteSlot();
  	void SetWakeup(bool shldWakeup);
  	void SetParentSchedulerPtr(void * scheduler){
  		m_parentScheduler = scheduler;
  	}

};





#endif /* DISCOVERYTIMESYNCHANDLER_H_ */
