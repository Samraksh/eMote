/*
 * DiscoveryTimesyncHandler.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan Sridharan
 */

#include "DiscoveryTimesyncHandler.h"
#include <Samraksh/Radio_decl.h>
#include "OMACConstants.h"
#include "Scheduler.h"

void DiscoveryTimesyncHandler::Initialize(){
	m_receivedPiggybackBeacon = FALSE;
	m_idxForComputation = INVALID_INDEX;
	counterOffsetAvg = 0;
	//dataAlarmDuration = 0;

	//m_timeSyncBeacon = (TimeSyncMsg*)m_FTSPTimeSync.GetPayload(&m_timeSyncBeaconBuffer, sizeof(TimeSyncMsg));
	m_timeSyncBeacon = (TimeSyncMsg*)m_timeSyncBeaconBuffer.GetPayload() ;
	m_timeSyncBeacon->globalTime = 0;
	m_timeSyncBeacon->flag = 0x0;
	m_timeSyncBeacon->localTime = 0;

	m_p1 = CONTROL_P1[MF_NODE_ID % 7];
	m_p2 = CONTROL_P2[MF_NODE_ID % 7];

	discoInterval = m_p1 * m_p2;	// Initially set to 1 to accelerate self-declaration as root
	ClearTable();
}

void DiscoveryTimesyncHandler::ExecuteSlot(UINT32 slotNum){

}

UINT8 DiscoveryTimesyncHandler::ExecuteSlotDone(){

}

UINT16 DiscoveryTimesyncHandler::NewSlot(UINT32 slotNum){

}


void DiscoveryTimesyncHandler::PostExecuteSlot(){

}


void DiscoveryTimesyncHandler::SetWakeup(bool shldWakeup){

}

////////////////////Private Functions////////////////////////////

//Mukundan: Can add conditions to suppress beaconing, will keep this true for now
bool DiscoveryTimesyncHandler::ShouldBeacon(){
	return TRUE;
}


DeviceStatus DiscoveryTimesyncHandler::Beacon(RadioAddress_t dst, Message_15_4_t *msgPtr){
	DeviceStatus e = DS_Fail;
	UINT64 localTime = 0;
	//m_timeSyncBeacon = (TimeSyncMsg*)m_FTSPTimeSync.GetPayload(msgPtr, sizeof(TimeSyncMsg));
	m_timeSyncBeacon = (TimeSyncMsg*)msgPtr->GetPayload();
	m_timeSyncBeacon->dataInterval = DATA_INTERVAL / SLOT_PERIOD_MILLI;
	if (m_timeSyncBeacon->dataInterval < 1) {
			m_timeSyncBeacon->dataInterval = 1;
		}

	m_timeSyncBeacon->flag = 0;

	if (m_busy == FALSE) {
		m_timeSyncBeacon->radioStartDelay = ((OMACScheduler *)m_parentScheduler)->GetRadioDelay();
		m_timeSyncBeacon->flag |= FLAG_TIMESTAMP_VALID;
		m_timeSyncBeacon->counterOffset = ((OMACScheduler *)m_parentScheduler)->GetCounterOffset();

		//localTime = call GlobalTime.getLocalTime();
		localTime = Time_GetLocalTime();

		//Wenjie: currently we set both the type of the AMMsg
		//and the am_type field in TimeSyncMsg to be the same
		m_timeSyncBeacon->globalTime = localTime;

		//call PacketAcknowledgements.noAck(msgPtr);

		if((e = m_FTSPTimeSync.Send(dst, msgPtr, TIMESYNCMSG_LEN, localTime )) == DS_Success ) {
			m_busy = TRUE;
		}
	}
	else {
		// Why busy? Timing issue?
		e = DS_Busy;
	}
	return e;
}

void DiscoveryTimesyncHandler::Beacon1(){
	((OMACScheduler *)m_parentScheduler)->ProtoState.ForceState(S_BEACON_1);
	StartBeaconNTimer(TRUE,SLOT_PERIOD_MILLI * 2);
	if (ShouldBeacon()) {
		Beacon(RADIO_BROADCAST_ADDRESS, &m_timeSyncBeaconBuffer);
	}
	//if beacon fails, the radio automatically changes to RX state
}

void DiscoveryTimesyncHandler::BeaconN(){
	((OMACScheduler *)m_parentScheduler)->ProtoState.ForceState(S_BEACON_N);

	if (Beacon(RADIO_BROADCAST_ADDRESS, &m_timeSyncBeaconBuffer) != DS_Success) {
		if (m_busy == TRUE) {
			// Ignore it, AMSend.sendDone will provide continuation.
		}
		else {
			// HACK: for now, just turn off radio.
			debug_printf("BeaconN fails\n");
			this->ExecuteSlotDone();
		}
	}
}

void DiscoveryTimesyncHandler::StartBeaconNTimer(bool oneShot, UINT64 delay){
	//Start the BeaconTimer
		//HALTimer()
		if(delay==0){
			//start alarm in default periodic mode
			//HALTimer :: Initialize (, TRUE, 0, 0, m_OMACScheduler.SlotAlarm, NULL)
		}else {
			//Change next slot time with delay
			//HALTimer :: Initialize (, TRUE, 0, 0, m_OMACScheduler.SlotAlarm, NULL)
		}
}

void DiscoveryTimesyncHandler::BeaconNTimerHandler(void* Param){
#ifdef TESTBED
#warning ** USING TESTBED CONFIG
		debug_printf("[%u] %u %u\n", TOS_NODE_ID, m_p1, m_p2);
#endif
	if (ShouldBeacon()) {
		BeaconN();
	} else {
		m_busy = FALSE;
		ExecuteSlotDone();
	}
}



// neighbor table util functions
UINT8 DiscoveryTimesyncHandler::GetFreeIdx(){

}

DeviceStatus DiscoveryTimesyncHandler::NbrToIdx(UINT16 nodeid, UINT8* idx){

}

void DiscoveryTimesyncHandler::ClearNeighbor(RadioAddress_t nodeID){

}

void DiscoveryTimesyncHandler::ClearTable(){

}

// util functions for time sync beaocn processing
DeviceStatus DiscoveryTimesyncHandler::IsSynced(RadioAddress_t address){

}

void DiscoveryTimesyncHandler::CleanUpAfterProcessing(){

}

void DiscoveryTimesyncHandler::ProcessMsg(){
	/*TimeSyncMsg* msg = (TimeSyncMsg*) m_FTSPTimeSync.GetPayload(m_processedMsg, sizeof(TimeSyncMsg)));
	am_addr_t source = msg->nodeID;
	uint8_t i, freeItem = INVALID_INDEX, oldestItem = 0;
	uint32_t age, oldestTime = 0;
	int32_t timeError;
	uint8_t numEntries = 0;
	uint8_t nbrIdx;

	if (nbrToIdx(source, &nbrIdx) == SUCCESS) {
		atomic {
			m_beaconTable[nbrIdx].dataInterval = msg->dataInterval;
			m_beaconTable[nbrIdx].radioStartDelay = msg->radioStartDelay;
			m_beaconTable[nbrIdx].counterOffset = msg->counterOffset;
			if (call TimeSyncInfo.lockSeed()) {
				if (m_beaconTable[nbrIdx].nextFrameAfterSeedUpdate < msg->nextFrame) {
					printf("received seed %u\n", msg->seed);
					m_beaconTable[nbrIdx].lastSeed = msg->seed;
					m_beaconTable[nbrIdx].nextFrameAfterSeedUpdate = msg->nextFrame;
				}
				call TimeSyncInfo.unlockSeed();
			}

		}
	}
	else {
		nbrIdx = getFreeIdx();
		if (nbrIdx != INVALID_INDEX) {
			atomic {
				m_nbrCnt++;
				m_beaconTable[nbrIdx].nodeID = source;
				m_beaconTable[nbrIdx].dataInterval = msg->dataInterval;
				m_beaconTable[nbrIdx].radioStartDelay = msg->radioStartDelay;
				m_beaconTable[nbrIdx].counterOffset = msg->counterOffset;
				if (m_beaconTable[nbrIdx].nextFrameAfterSeedUpdate < msg->nextFrame) {
					m_beaconTable[nbrIdx].lastSeed = msg->seed;
					m_beaconTable[nbrIdx].nextFrameAfterSeedUpdate = msg->nextFrame;
				}
			}
		}
	}

	if (nbrIdx != INVALID_INDEX) {
		TableItem *nbrPtr = &(m_beaconTable[nbrIdx]);
		TimeSyncSample *samples = nbrPtr->samples;

		// clear table if the received entry's been inconsistent for some time
		if (is_synced(source) == SUCCESS) {
			timeError = msg->localTime;
			call GlobalTime.local2Global((uint32_t*)(&timeError), source);
			timeError -= msg->globalTime;
			if (timeError > (int32_t)ENTRY_THROWOUT_LIMIT
				|| timeError < (int32_t)-ENTRY_THROWOUT_LIMIT) {
				debug_printf("tsync error = %ld\n", timeError);
				if (++(nbrPtr->numErrors) > 4) {
					clearNeighbor(source);
				}
				cleanUpAfterProcessing();
				return; // don't incorporate a bad reading
			}
		}
		atomic {
			nbrPtr->size = 0; // don't reset table size unless you're recounting
			nbrPtr->numErrors = 0;
		}

		for(i = 0; i < MAX_ENTRIES; ++i) {
			age = msg->localTime - samples[i].localTime;

			//logical time error compensation
			if( age >= 0x7FFFFFFFL )
				samples[i].localTime = INVALID_TIMESTAMP;

			if( samples[i].localTime == INVALID_TIMESTAMP )
				freeItem = i;
			else
				++numEntries;
			if( age >= oldestTime ) {
				oldestTime = age;
				oldestItem = i;
			}
		}

		if( freeItem == INVALID_INDEX )
			freeItem = oldestItem;
		else
			++numEntries;

		nbrPtr->nodeID = source;
		nbrPtr->size = numEntries;
		samples[freeItem].localTime = msg->localTime;
		samples[freeItem].timeOffset = msg->globalTime - msg->localTime;

		m_idxForComputation = nbrIdx;

		if(call TimeSyncInfo.isInTransitionPeriod(source)) {
			// my clock has just wrapped. Do not do skew calculation until we have
			// collected ENTRY_VALID_LIMIT samples after the clock wrap. We add a extra
			// margin into the advance wakeup time to compensate for the possible change
			// in clock drifts during this transition period. local2Global calculation will
			// still be valid due to the arithmetics nature of the two's complements representations

			cleanUpAfterProcessing();
#ifdef OMAC_DEBUG
			debug_printf("[transition period]: remaining=%u\n", m_transitionFromDone);
#endif
			return;
		}
		else if (post calculateConversion() != SUCCESS) {
			nbrPtr->isInTransition = FALSE;
			cleanUpAfterProcessing();
		}
		debug_printf("%u.cnt=%u\n", nbrPtr->nodeID, nbrPtr->size);
	} else {
		debug_printf("cannot add new beacon entry");
		cleanUpAfterProcessing();
	}
*/

}

void DiscoveryTimesyncHandler::CalculateConversion(){

}
