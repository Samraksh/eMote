/*
 * DiscoveryTimesyncHandler.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan Sridharan
 */

#include "DiscoveryHandler.h"
#include <Samraksh/Radio_decl.h>
#include "OMACConstants.h"
#include "Scheduler.h"

OMACScheduler *g_scheduler;
extern RadioControl_t g_omac_RadioControl;

void PublicBeaconNCallback(void * param){
	g_scheduler->m_DiscoveryHandler.BeaconNTimerHandler(param);
}

void DiscoveryHandler::SetParentSchedulerPtr(void * scheduler){
  		m_parentScheduler = scheduler;
  		g_scheduler = (OMACScheduler*)scheduler;
  	}

void DiscoveryHandler::Initialize(){
	m_receivedPiggybackBeacon = FALSE;
	m_idxForComputation = INVALID_INDEX;
	counterOffsetAvg = 0;
	//dataAlarmDuration = 0;


	m_discoveryMsg= (DiscoveryMsg_t*)m_discoveryMsgBuffer.GetPayload() ;
	//m_discoveryMsg->globalTime = 0;
	//m_discoveryMsg->flag = 0x0;
	//m_discoveryMsg->localTime = 0;

	m_p1 = CONTROL_P1[MF_NODE_ID % 7];
	m_p2 = CONTROL_P2[MF_NODE_ID % 7];

	discoInterval = m_p1 * m_p2;	// Initially set to 1 to accelerate self-declaration as root
	ClearTable();
}

void DiscoveryHandler::ExecuteSlot(UINT32 slotNum){
	CPU_GPIO_SetPinState((GPIO_PIN) 10, TRUE);
	Beacon1();
	CPU_GPIO_SetPinState((GPIO_PIN) 10, FALSE);
}

UINT8 DiscoveryHandler::ExecuteSlotDone(){
	g_scheduler->Stop();
}

UINT32 DiscoveryHandler::NewSlot(UINT32 slotNum){
	UINT32 p1Remaining, p2Remaining;
	p1Remaining = slotNum % m_p1;
	p2Remaining = slotNum % m_p2;

	if (p1Remaining == 0 || p2Remaining == 0) {
		return 0;
	}
	else  {
		p1Remaining = m_p1 - p1Remaining;
		p2Remaining = m_p2 - p2Remaining;
		if (p1Remaining >= 0xffff >> SLOT_PERIOD_BITS && p2Remaining >= 0xffff >> SLOT_PERIOD_BITS) {
			return 0xffff;
		} else {
			return p1Remaining < p2Remaining ? p1Remaining << SLOT_PERIOD_BITS : p2Remaining << SLOT_PERIOD_BITS;
		}
	}
}


void DiscoveryHandler::PostExecuteSlot(){

}


void DiscoveryHandler::SetWakeup(BOOL shldWakeup){

}

////////////////////Private Functions////////////////////////////

//Mukundan: Can add conditions to suppress beaconing, will keep this true for now
BOOL DiscoveryHandler::ShouldBeacon(){
	return TRUE;
}


DeviceStatus DiscoveryHandler::Beacon(RadioAddress_t dst, Message_15_4_t *msgPtr){
	CPU_GPIO_SetPinState((GPIO_PIN) 10, TRUE);
	DeviceStatus e = DS_Fail;
	UINT64 localTime = 0;

	m_discoveryMsg= (DiscoveryMsg_t*)msgPtr->GetPayload();
	m_discoveryMsg->dataInterval = DATA_INTERVAL / SLOT_PERIOD_MILLI;
	if (m_discoveryMsg->dataInterval < 1) {
			m_discoveryMsg->dataInterval = 1;
		}

	if (m_busy == FALSE) {
		m_discoveryMsg->radioStartDelay = ((OMACScheduler *)m_parentScheduler)->GetRadioDelay();
		//m_discoveryMsg->flag |= FLAG_TIMESTAMP_VALID;
		m_discoveryMsg->counterOffset = ((OMACScheduler *)m_parentScheduler)->GetCounterOffset();


		//call PacketAcknowledgements.noAck(msgPtr);

		if((e = Send(dst, msgPtr, sizeof(DiscoveryMsg_t), localTime )) == DS_Success ) {
			m_busy = TRUE;
		}
	}
	else {
		// Why busy? Timing issue?
		e = DS_Busy;
	}
	CPU_GPIO_SetPinState((GPIO_PIN) 10, FALSE);
	return e;
}

void DiscoveryHandler::BeaconAckHandler(Message_15_4_t* msg, UINT8 len, NetOpStatus success){
	if (msg != &m_discoveryMsgBuffer) {
		m_busy = FALSE;
		//signal AMSend.sendDone(ptr, error);
		return;
	}

	m_busy = FALSE;
	#ifndef DISABLE_SIGNAL
			//call SlotScheduler.printState();
			//signalBeaconDone(error, call GlobalTime.getLocalTime());
	#endif

	this->ExecuteSlotDone();
}

void DiscoveryHandler::Beacon1(){
	((OMACScheduler *)m_parentScheduler)->ProtoState.ForceState(S_BEACON_1);
	StartBeaconNTimer(TRUE,SLOT_PERIOD_MILLI * 2);
	if (ShouldBeacon()) {
		Beacon(RADIO_BROADCAST_ADDRESS, &m_discoveryMsgBuffer);
	}
	//if beacon fails, the radio automatically changes to RX state
}

void DiscoveryHandler::BeaconN(){
	((OMACScheduler *)m_parentScheduler)->ProtoState.ForceState(S_BEACON_N);

	if (Beacon(RADIO_BROADCAST_ADDRESS, &m_discoveryMsgBuffer) != DS_Success) {
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

void DiscoveryHandler::StartBeaconNTimer(BOOL oneShot, UINT64 delay){
	//Start the BeaconTimer
		//HALTimer()
		if(delay==0){
			//start default time
			gHalTimerManagerObject.CreateTimer(7, 0, delay*1000, oneShot, FALSE, PublicBeaconNCallback); //1 sec Timer in micro seconds

		}else {
			//Change next slot time with delay
			gHalTimerManagerObject.CreateTimer(7, 0, delay*1000, oneShot, FALSE, PublicBeaconNCallback); //1 sec Timer in micro seconds
		}
}

void DiscoveryHandler::BeaconNTimerHandler(void* Param){
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
	//gHalTimerManagerObject.StopTimer(7);
}



// neighbor table util functions
UINT8 DiscoveryHandler::GetFreeIdx(){

}

DeviceStatus DiscoveryHandler::NbrToIdx(UINT16 nodeid, UINT8* idx){

}

void DiscoveryHandler::ClearNeighbor(RadioAddress_t nodeID){

}

void DiscoveryHandler::ClearTable(){

}

// util functions for time sync beaocn processing
DeviceStatus DiscoveryHandler::IsSynced(RadioAddress_t address){

}

void DiscoveryHandler::CleanUpAfterProcessing(){

}

void DiscoveryHandler::ProcessMsg(){
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

void DiscoveryHandler::CalculateConversion(){

}

DeviceStatus DiscoveryHandler::Receive(Message_15_4_t* msg, void* payload, UINT8 len){
	DiscoveryMsg_t* disMsg = (DiscoveryMsg_t *) msg->GetPayload();
	ProcessMsg();
	return DS_Success;
}

DeviceStatus DiscoveryHandler::Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time){
	IEEE802_15_4_Header_t * header = msg->GetHeader();
	//UINT8 * payload = msg->GetPayload();
	header->dest= address;
	header->type=MFM_DISCOVERY;

	g_omac_RadioControl.Send_TimeStamped(address,msg,sizeof(Message_15_4_t),event_time);
}
