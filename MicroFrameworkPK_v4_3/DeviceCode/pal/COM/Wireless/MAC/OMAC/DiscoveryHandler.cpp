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
#include "OMAC.h"
#include <DeviceCode\LCD_PCF85162_HAL\LCD_PCF85162.h>

OMACScheduler *g_scheduler;
extern NeighborTable g_NeighborTable;
extern RadioControl_t g_omac_RadioControl;
extern OMAC g_OMAC;

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
	m_busy = FALSE;
	//dataAlarmDuration = 0;


	m_discoveryMsg= (DiscoveryMsg_t*)m_discoveryMsgBuffer.GetPayload() ;
	//m_discoveryMsg->globalTime = 0;
	//m_discoveryMsg->flag = 0x0;
	//m_discoveryMsg->localTime = 0;

	m_p1 = CONTROL_P1[CPU_Radio_GetAddress(this->radioName) % 7];
	m_p2 = CONTROL_P2[CPU_Radio_GetAddress(this->radioName) % 7];
	hal_printf("prime 1: %d\tprime 2: %d\r\n",m_p1, m_p2);

	discoInterval = m_p1 * m_p2;	// Initially set to 1 to accelerate self-declaration as root
	hal_printf("discoInterval: %d\r\n", discoInterval);
}

void DiscoveryHandler::ExecuteSlot(UINT32 slotNum){
	Beacon1();
}

UINT8 DiscoveryHandler::ExecuteSlotDone(){
	g_scheduler->Stop();
}

UINT32 DiscoveryHandler::NextSlot(UINT32 slotNum){
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
	m_busy = FALSE;
}


void DiscoveryHandler::SetWakeup(BOOL shldWakeup){

}

////////////////////Private Functions////////////////////////////

//Mukundan: Can add conditions to suppress beaconing, will keep this true for now
BOOL DiscoveryHandler::ShouldBeacon(){
	return TRUE;
}


DeviceStatus DiscoveryHandler::Beacon(RadioAddress_t dst, Message_15_4_t *msgPtr){
	DeviceStatus e = DS_Fail;
	UINT64 localTime = 0;

	m_discoveryMsg= (DiscoveryMsg_t*)msgPtr->GetPayload();
	m_discoveryMsg->seed = m_seed;
	m_discoveryMsg->nextFrame = m_nextFrame;
	//m_discoveryMsg->dataInterval = DATA_INTERVAL / SLOT_PERIOD_MILLI;
	m_discoveryMsg->dataInterval = WAKEUP_INTERVAL / SLOT_PERIOD_MILLI;
	if (m_discoveryMsg->dataInterval < 1) {
			m_discoveryMsg->dataInterval = 1;
		}

	if (m_busy == FALSE) {
		m_discoveryMsg->radioStartDelay = ((OMACScheduler *)m_parentScheduler)->GetRadioDelay();
		//m_discoveryMsg->flag |= FLAG_TIMESTAMP_VALID;
		m_discoveryMsg->counterOffset = ((OMACScheduler *)m_parentScheduler)->GetCounterOffset();
		//m_discoveryMsg->seed = ((OMACScheduler *)m_parentScheduler)->GetSeed();
		m_discoveryMsg->nodeID = g_OMAC.GetAddress();
		m_busy = TRUE;
		Send(dst, msgPtr, sizeof(DiscoveryMsg_t), localTime );
	}
	else {
		// Why busy? Timing issue?
		e = DS_Busy;
	}
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
			this->ExecuteSlotDone();
		}
	}
}

void DiscoveryHandler::StartBeaconNTimer(BOOL oneShot, UINT64 delay){
	//Start the BeaconTimer
		//HALTimer()
		if(delay==0){
			//start default time
			gHalTimerManagerObject.CreateTimer(HAL_DISCOVERY_TIMER, 0, delay*1000, oneShot, FALSE, PublicBeaconNCallback); //1 sec Timer in micro seconds

		}else {
			//Change next slot time with delay
			gHalTimerManagerObject.CreateTimer(HAL_DISCOVERY_TIMER, 0, delay*1000, oneShot, FALSE, PublicBeaconNCallback); //1 sec Timer in micro seconds
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

DeviceStatus DiscoveryHandler::Receive(Message_15_4_t* msg, void* payload, UINT8 len){
	DiscoveryMsg_t* disMsg = (DiscoveryMsg_t *) msg->GetPayload();
	RadioAddress_t source = disMsg->nodeID;
	Neighbor_t tempNeighbor;
	UINT8 nbrIdx;

	UINT64 localTime = HAL_Time_CurrentTicks();

	MacReceiveFuncPtrType appHandler = g_OMAC.AppHandlers[g_OMAC.CurrentActiveApp]->RecieveHandler;

	if (g_NeighborTable.FindIndex(source, &nbrIdx) == DS_Success) {
		//hal_printf("DiscoveryHandler::Receive already found neighbor: %d at index: %d\ttime: %lld\r\n", source, nbrIdx, localTime);
		g_NeighborTable.UpdateNeighbor(source, Alive, localTime, disMsg->seed, disMsg->dataInterval, disMsg->radioStartDelay, disMsg->counterOffset, &nbrIdx);;
	} else {
		g_NeighborTable.InsertNeighbor(disMsg->nodeID, Alive, localTime, disMsg->seed, disMsg->dataInterval, disMsg->radioStartDelay, disMsg->counterOffset, &nbrIdx);
	}

	return DS_Success;
}

DeviceStatus DiscoveryHandler::Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time){
	DeviceStatus retValue;
	IEEE802_15_4_Header_t * header = msg->GetHeader();
	//UINT8 * payload = msg->GetPayload();
	header->dest= address;
	header->type=MFM_DISCOVERY;

	retValue = g_omac_RadioControl.Send(address,msg,size);

	return retValue;
}
