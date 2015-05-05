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

#ifndef DEBUG_TSYNC
#define DEBUG_TSYNC 1
#define TIMESYNCSENDPIN 2
#define TIMESYNCRECEIVEPIN 23
#endif

OMACSchedulerBora *g_scheduler;
extern NeighborTable g_NeighborTable;
extern RadioControl_t g_omac_RadioControl;
extern OMACTypeBora g_OMAC;

void PublicBeaconNCallback(void * param){
	g_scheduler->m_DiscoveryHandler.BeaconNTimerHandler(param);
}

void DiscoveryHandler::SetParentSchedulerPtr(void * scheduler){
  		m_parentScheduler = scheduler;
  		g_scheduler = (OMACSchedulerBora*)scheduler;
  	}

void DiscoveryHandler::Initialize(UINT8 radioID, UINT8 macID){

	RadioID = radioID;
	MacID = macID;
	m_receivedPiggybackBeacon = FALSE;
	m_idxForComputation = INVALID_INDEX;
	counterOffsetAvg = 0;
	m_busy = FALSE;
	//dataAlarmDuration = 0;


	m_discoveryMsg= (DiscoveryMsg_t*)m_discoveryMsgBuffer.GetPayload() ;
	//m_discoveryMsg->globalTime = 0;
	//m_discoveryMsg->flag = 0x0;
	//m_discoveryMsg->localTime = 0;

	m_p1 = CONTROL_P1[CPU_Radio_GetAddress(RadioID) % 7];
	m_p2 = CONTROL_P2[CPU_Radio_GetAddress(MacID) % 7];
	hal_printf("prime 1: %d\tprime 2: %d\r\n",m_p1, m_p2);

	discoInterval = m_p1 * m_p2;	// Initially set to 1 to accelerate self-declaration as root
	hal_printf("discoInterval: %d\r\n", discoInterval);

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(HAL_DISCOVERY_TIMER, 0, SLOT_PERIOD_MILLI * 2, TRUE, FALSE, PublicBeaconNCallback); //1 sec Timer in micro seconds
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
#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCSENDPIN, TRUE );
#endif
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
		m_discoveryMsg->radioStartDelay = ((OMACSchedulerBora *)m_parentScheduler)->GetRadioDelay();
		//m_discoveryMsg->flag |= FLAG_TIMESTAMP_VALID;
		m_discoveryMsg->counterOffset = ((OMACSchedulerBora *)m_parentScheduler)->GetCounterOffset();
		//m_discoveryMsg->seed = ((OMACSchedulerBora *)m_parentScheduler)->GetSeed();
		m_discoveryMsg->nodeID = g_OMAC.GetAddress();
		m_busy = TRUE;

		UINT64 localTime = HAL_Time_CurrentTicks();
		m_discoveryMsg->localTime0 = (UINT32) localTime ;
		m_discoveryMsg->localTime1 = (UINT32) (localTime>>32);
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
	((OMACSchedulerBora *)m_parentScheduler)->ProtoState.ForceState(S_BEACON_1);
	StartBeaconNTimer(TRUE,SLOT_PERIOD_MILLI * 2);
	if (ShouldBeacon()) {
		Beacon(RADIO_BROADCAST_ADDRESS, &m_discoveryMsgBuffer);
	}
	//if beacon fails, the radio automatically changes to RX state
}

void DiscoveryHandler::BeaconN(){
	((OMACSchedulerBora *)m_parentScheduler)->ProtoState.ForceState(S_BEACON_N);

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
	VirtualTimerReturnMessage rm;
	//Start the BeaconTimer
		//HALTimer()
		if(delay==0){
			//start default time
			//VirtTimer_SetTimer(HAL_DISCOVERY_TIMER, 0, delay*1000, oneShot, FALSE, PublicBeaconNCallback); //1 sec Timer in micro seconds
			rm = VirtTimer_Change(HAL_DISCOVERY_TIMER, 0, delay*1000, oneShot); //1 sec Timer in micro seconds
			rm = VirtTimer_Start(HAL_DISCOVERY_TIMER);

		}else {
			//Change next slot time with delay
			//VirtTimer_SetTimer(HAL_DISCOVERY_TIMER, 0, delay*1000, oneShot, FALSE, PublicBeaconNCallback); //1 sec Timer in micro seconds
			rm = VirtTimer_Change(HAL_DISCOVERY_TIMER, 0, delay*1000, oneShot); //1 sec Timer in micro seconds
			rm = VirtTimer_Start(HAL_DISCOVERY_TIMER);
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
	//VirtTimer_Stop(7);
}

DeviceStatus DiscoveryHandler::Receive(Message_15_4_t* msg, void* payload, UINT8 len){



	DiscoveryMsg_t* disMsg = (DiscoveryMsg_t *) msg->GetPayload();
	RadioAddress_t source = msg->GetHeader()->src;
	//RadioAddress_t source = disMsg->nodeID;
	Neighbor_t tempNeighbor;
	UINT8 nbrIdx;
	UINT64 localTime = HAL_Time_CurrentTicks();

#ifdef DEBUG_TSYNC
	if (source == g_scheduler->m_TimeSyncHandler.Nbr2beFollowed ){
		if (g_scheduler->m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(source) >=2  ){
			CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, TRUE );
		}
	}
#endif
	UINT64 EventTime = PacketTimeSync_15_4::EventTime(msg,len);
	UINT64 rcv_ltime;
	INT64 l_offset;
	rcv_ltime=  (((UINT64)disMsg->localTime1) <<32) + disMsg->localTime0;
	l_offset = rcv_ltime - EventTime;
	g_scheduler->m_TimeSyncHandler.m_globalTime.regressgt2.Insert(source, rcv_ltime, l_offset);


	MacReceiveFuncPtrType appHandler = g_OMAC.AppHandlers[g_OMAC.CurrentActiveApp]->RecieveHandler;

	if (g_NeighborTable.FindIndex(source, &nbrIdx) == DS_Success) {
		//hal_printf("DiscoveryHandler::Receive already found neighbor: %d at index: %d\ttime: %lld\r\n", source, nbrIdx, localTime);
		g_NeighborTable.UpdateNeighbor(source, Alive, localTime, disMsg->seed, disMsg->dataInterval, disMsg->radioStartDelay, disMsg->counterOffset, &nbrIdx);;
	} else {
		g_NeighborTable.InsertNeighbor(source, Alive, localTime, disMsg->seed, disMsg->dataInterval, disMsg->radioStartDelay, disMsg->counterOffset, &nbrIdx);
	}

#ifdef DEBUG_TSYNC
	if (source == g_scheduler->m_TimeSyncHandler.Nbr2beFollowed ){
		if (g_scheduler->m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(source) >=2  ){
			CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, FALSE );
		}
	}
#endif

	return DS_Success;
}

DeviceStatus DiscoveryHandler::Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time){

	DeviceStatus retValue;
	IEEE802_15_4_Header_t * header = msg->GetHeader();
	//UINT8 * payload = msg->GetPayload();
	header->dest = address;
	header->type = MFM_DISCOVERY;

	retValue = g_omac_RadioControl.Send_TimeStamped(address,msg,size, event_time);

#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCSENDPIN, FALSE );
#endif

	return retValue;
}
