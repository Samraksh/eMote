/*
 * DiscoveryTimesyncHandler.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan Sridharan
 */

#include <Samraksh/Radio_decl.h>
#include <Samraksh/MAC/OMAC/DiscoveryHandler.h>
#include <Samraksh/MAC/OMAC/OMACConstants.h>
#include <Samraksh/MAC/OMAC/Scheduler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <DeviceCode\LCD_PCF85162_HAL\LCD_PCF85162.h>

#ifndef DEBUG_TSYNC
#define DEBUG_TSYNC 1
//#define DISCOSYNCSENDPIN 24 //2
//#define DISCOSYNCRECEIVEPIN 25 //25
#endif

OMACScheduler *g_scheduler;
extern NeighborTable g_NeighborTable;
extern RadioControl_t g_omac_RadioControl;
extern OMACType g_OMAC;

/*
 *
 */
void PublicBeaconNCallback(void * param){
	g_scheduler->m_DiscoveryHandler.BeaconNTimerHandler(param);
	g_scheduler->PostExecution();
}

/*
 *
 */
void DiscoveryHandler::SetParentSchedulerPtr(void * scheduler){
	m_parentScheduler = scheduler;
	g_scheduler = (OMACScheduler*)scheduler;
}

/*
 *
 */
void DiscoveryHandler::Initialize(UINT8 radioID, UINT8 macID){
	CPU_GPIO_EnableOutputPin( DISCO_SYNCSENDPIN, TRUE);
	CPU_GPIO_EnableOutputPin( DISCO_SYNCRECEIVEPIN, TRUE);
	RadioID = radioID;
	MacID = macID;
	m_receivedPiggybackBeacon = FALSE;
	m_idxForComputation = INVALID_INDEX;
	counterOffsetAvg = 0;
	m_busy = FALSE;
	//dataAlarmDuration = 0;


	m_discoveryMsg = (DiscoveryMsg_t*)m_discoveryMsgBuffer.GetPayload() ;
	//m_discoveryMsg->globalTime = 0;
	//m_discoveryMsg->flag = 0x0;
	//m_discoveryMsg->localTime = 0;

	m_period1 = CONTROL_P1[g_OMAC.GetAddress() % 7];
	m_period2 = CONTROL_P2[g_OMAC.GetAddress() % 7];
	hal_printf("prime 1: %d\tprime 2: %d\r\n",m_period1, m_period2);

	discoInterval = m_period1 * m_period2;	// Initially set to 1 to accelerate self-declaration as root
	hal_printf("discoInterval: %d\r\n", discoInterval);

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(HAL_DISCOVERY_TIMER, 0, SLOT_PERIOD_MILLI * 2 * MICSECINMILISEC, TRUE, FALSE, PublicBeaconNCallback); //1 sec Timer in micro seconds
}

UINT64 DiscoveryHandler::NextEvent(UINT32 currentSlotNum){
	UINT16 nextEventsSlot = 0;
	UINT64 nextEventsMicroSec = 0;
	nextEventsSlot = NextEventinSlots(currentSlotNum);
	if(nextEventsSlot == 0) return(nextEventsMicroSec-1);//BK: Current slot is already too late. Hence return a large number back
	nextEventsMicroSec = nextEventsSlot * SLOT_PERIOD_MILLI * MICSECINMILISEC;
	nextEventsMicroSec = nextEventsMicroSec + g_scheduler->GetTimeTillTheEndofSlot();
	return(nextEventsMicroSec);
}
/*
 *
 */
UINT64 DiscoveryHandler::NextEventinSlots(UINT32 currentSlotNum){
	UINT64 period1Remaining, period2Remaining;
	period1Remaining = currentSlotNum % m_period1;
	period2Remaining = currentSlotNum % m_period2;

	if (period1Remaining == 0 || period2Remaining == 0) {
		return 0;
	}
	else  {
		return ((period1Remaining < period2Remaining) ? (period1Remaining ) : (period2Remaining ));
	}
}

/*
 *
 */
void DiscoveryHandler::ExecuteEvent(UINT32 slotNum){
	DeviceStatus e = DS_Fail;
	e = g_omac_RadioControl.StartRx();
	if (e == DS_Success){
		Beacon1();
	}
}

/*
 *
 */
/*UINT8 DiscoveryHandler::ExecuteEventDone(){
	g_scheduler->Stop();
	return 0;
}*/

/*
 *
 */
void DiscoveryHandler::PostExecuteEvent(){
	////hal_printf("DiscoveryHandler::PostExecuteEvent\n");
	////m_busy = FALSE;
	//stop the radio
	g_scheduler->PostExecution();
}

/*
 *
 */
void DiscoveryHandler::SetWakeup(bool shldWakeup){
	//hal_printf("DiscoveryHandler::SetWakeup\n");
}

////////////////////Private Functions////////////////////////////
/*
 *
 */
//Mukundan: Can add conditions to suppress beaconing, will keep this true for now
BOOL DiscoveryHandler::ShouldBeacon(){
	return TRUE;
}

/*
 *
 */
DeviceStatus DiscoveryHandler::Beacon(RadioAddress_t dst, Message_15_4_t* msgPtr){
	////hal_printf("start DiscoveryHandler::Beacon\n");
#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState(  DISCO_SYNCSENDPIN, TRUE );
#endif
	DeviceStatus e = DS_Fail;
	UINT64 localTime = 0;

	m_discoveryMsg = (DiscoveryMsg_t*)msgPtr->GetPayload();
	//m_discoveryMsg->seed = m_seed;
	m_discoveryMsg -> counterOffset = 0;
	m_discoveryMsg->radioStartDelay = ((OMACScheduler *)m_parentScheduler)->GetRadioDelay();

	m_discoveryMsg->nextSeed = g_scheduler->m_DataReceptionHandler.m_nextSeed;
	m_discoveryMsg->mask = g_scheduler->m_DataReceptionHandler.m_mask;
	m_discoveryMsg->nextwakeupSlot =  g_scheduler->m_DataReceptionHandler.m_nextwakeupSlot;
	m_discoveryMsg->seedUpdateIntervalinSlots = g_scheduler->m_DataReceptionHandler.m_seedUpdateIntervalinSlots;

	m_discoveryMsg->nodeID = g_OMAC.GetAddress();
	localTime = HAL_Time_CurrentTicks();
	m_discoveryMsg->localTime0 = (UINT32) localTime ;
	m_discoveryMsg->localTime1 = (UINT32) (localTime>>32);

	if (m_busy == FALSE) {
		m_busy = TRUE;
		e = Send(dst, msgPtr, sizeof(DiscoveryMsg_t), localTime );
		m_busy = FALSE;
		//if (e != DS_Success) {
			//localTime = 2;
		//}
	}
	else {
		// Why busy? Timing issue?
		hal_printf("DiscoveryHandler::Beacon m_busy\n");
		e = DS_Busy;
	}
	////hal_printf("end DiscoveryHandler::Beacon\n");
	return e;
}

/*
 *
 */
void DiscoveryHandler::BeaconAckHandler(Message_15_4_t* msg, UINT8 len, NetOpStatus status){
	if(status == NO_Busy){
		hal_printf("NO_Busy - What do we do? Just ignore?\n");
		m_busy = TRUE;
	}
	else if(status == NO_Success){
		//hal_printf("SUCCESS!\n");
		m_busy = FALSE;
	}
	else{
		hal_printf("Need to investigate. Status: %d\n", status);
		ASSERT(0);
	}

	if (msg != &m_discoveryMsgBuffer) {
		////hal_printf("if m_busy DiscoveryHandler::BeaconAckHandler\n");
		////m_busy = FALSE;
		//signal AMSend.sendDone(ptr, error);
		return;
	}

	////hal_printf("m_busy DiscoveryHandler::BeaconAckHandler\n");
	#ifndef DISABLE_SIGNAL
			//call SlotScheduler.printState();
			//signalBeaconDone(error, call GlobalTime.getLocalTime());
	#endif

	//this->ExecuteEventDone();
	//this->PostExecuteEvent();
}

/*
 *
 */
void DiscoveryHandler::Beacon1(){
	////hal_printf("start Beacon1\n");
	((OMACScheduler *)m_parentScheduler)->ProtoState.ForceState(S_BEACON_1);
	StartBeaconNTimer(TRUE);
	if (ShouldBeacon()) {
		DeviceStatus ds = Beacon(RADIO_BROADCAST_ADDRESS, &m_discoveryMsgBuffer);
		if(ds != DS_Success) {
			hal_printf("Beacon1 failed. ds = %d\n", ds);
		}
		else {
			//hal_printf("Beacon1 succeeded\n");
		}
	}
	//if beacon fails, the radio automatically changes to RX state
	////hal_printf("end Beacon1\n");
}

/*
 *
 */
void DiscoveryHandler::BeaconN(){
	////hal_printf("start BeaconN\n");
	((OMACScheduler *)m_parentScheduler)->ProtoState.ForceState(S_BEACON_N);

	DeviceStatus ds = Beacon(RADIO_BROADCAST_ADDRESS, &m_discoveryMsgBuffer);
	if (ds != DS_Success) {
		if (m_busy == TRUE) {
			hal_printf("BeaconN failed. m_busy\n");
			// Ignore it, AMSend.sendDone will provide continuation.
			this->PostExecuteEvent();
		}
		else {
			// HACK: for now, just turn off radio.
			hal_printf("BeaconN failed\n");
			//this->ExecuteEventDone();
			this->PostExecuteEvent();
		}
	}
	else {
		//hal_printf("BeaconN succeeded\n");
	}
	////hal_printf("end BeaconN\n");
}

/*
 *
 */
void DiscoveryHandler::StartBeaconNTimer(BOOL oneShot){
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_Start(HAL_DISCOVERY_TIMER);
}

/*
 *
 */
void DiscoveryHandler::BeaconNTimerHandler(void* Param){
#ifdef TESTBED
#warning ** USING TESTBED CONFIG
		debug_printf("[%u] %u %u\n", TOS_NODE_ID, m_p1, m_p2);
#endif
	if (ShouldBeacon()) {
		BeaconN();
	} else {
		hal_printf("m_busy DiscoveryHandler::BeaconNTimerHandler\n");
		m_busy = FALSE;
		this->PostExecuteEvent();
	}
	//VirtTimer_Stop(7);
}

/*
 *
 */
DeviceStatus DiscoveryHandler::Receive(Message_15_4_t* msg, void* payload, UINT8 len){
	//hal_printf("start DiscoveryHandler::Receive\n");
	DiscoveryMsg_t* disMsg = (DiscoveryMsg_t *) msg->GetPayload();
	RadioAddress_t source = msg->GetHeader()->src;
	//RadioAddress_t source = disMsg->nodeID;
	Neighbor_t tempNeighbor;
	UINT8 nbrIdx;
	UINT64 localTime = HAL_Time_CurrentTicks();

	if (g_NeighborTable.FindIndex(source, &nbrIdx) == DS_Success) {
		//hal_printf("DiscoveryHandler::Receive already found neighbor: %d at index: %d\ttime: %lld\r\n", source, nbrIdx, localTime);
		g_NeighborTable.UpdateNeighbor(source, Alive, localTime, disMsg->nextSeed, disMsg->mask, disMsg->nextwakeupSlot, disMsg->seedUpdateIntervalinSlots, &nbrIdx);
	} else {
		g_NeighborTable.InsertNeighbor(source, Alive, localTime, disMsg->nextSeed, disMsg->mask, disMsg->nextwakeupSlot, disMsg->seedUpdateIntervalinSlots, &nbrIdx);
	}

#ifdef DEBUG_TSYNC
	if (source == g_scheduler->m_TimeSyncHandler.Neighbor2beFollowed ){
		if (g_scheduler->m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(source) >=2  ){
			CPU_GPIO_SetPinState(  DISCO_SYNCRECEIVEPIN, TRUE );
		}
	}
#endif
	UINT64 EventTime = PacketTimeSync_15_4::EventTime(msg,len);
	UINT64 rcv_ltime;
	INT64 l_offset;
	rcv_ltime = (((UINT64)disMsg->localTime1) <<32) + disMsg->localTime0;
	l_offset = rcv_ltime - EventTime;
	g_NeighborTable.RecordTimeSyncRecv(source, EventTime);
	g_scheduler->m_TimeSyncHandler.m_globalTime.regressgt2.Insert(source, rcv_ltime, l_offset);

	////MacReceiveFuncPtrType appHandler = g_OMAC.AppHandlers[g_OMAC.GetCurrentActiveApp()]->ReceiveHandler;
	MacReceiveFuncPtrType appHandler = g_OMAC.GetAppHandler(g_OMAC.GetCurrentActiveApp())->GetReceiveHandler();
	//(*appHandler);



#ifdef DEBUG_TSYNC
	if (source == g_scheduler->m_TimeSyncHandler.Neighbor2beFollowed){
		if (g_scheduler->m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(source) >=2  ){
			CPU_GPIO_SetPinState(  DISCO_SYNCRECEIVEPIN, FALSE );
		}
	}
#endif

	////hal_printf("end DiscoveryHandler::Receive\n");
	return DS_Success;
}

/*
 *
 */
DeviceStatus DiscoveryHandler::Send(RadioAddress_t address, Message_15_4_t* msg, UINT16 size, UINT64 event_time){
	////hal_printf("start DiscoveryHandler::Send\n");
	DeviceStatus retValue;
	IEEE802_15_4_Header_t * header = msg->GetHeader();
	//UINT8 * payload = msg->GetPayload();
	header->length = size + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest = address;
	header->src = CPU_Radio_GetAddress(g_OMAC.radioName);
	header->mac_id = g_OMAC.macName;
	header->type = MFM_DISCOVERY;
	//header->type = MFM_TIMESYNC;

	retValue = g_omac_RadioControl.Send(address, msg, size + sizeof(IEEE802_15_4_Header_t), event_time);

#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState(  DISCO_SYNCSENDPIN, FALSE );
#endif

	////hal_printf("end DiscoveryHandler::Send\n");
	return retValue;
}
