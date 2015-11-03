/*
 * DiscoveryTimesyncHandler.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan Sridharan
 *
 *  Modified on: Oct 30, 2015
 *  	Authors: Bora Karaoglu; Ananth Muralidharan
 *
 *  Copyright The Samraksh Company
 */

#include <Samraksh/Radio_decl.h>
#include <Samraksh/MAC/OMAC/DiscoveryHandler.h>
#include <Samraksh/MAC/OMAC/OMACConstants.h>
#include <Samraksh/MAC/OMAC/Scheduler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <DeviceCode\LCD_PCF85162_HAL\LCD_PCF85162.h>
#include "..\DeviceCode\Targets\Native\STM32F10x\DeviceCode\drivers\radio\RF231\RF231.h"

extern OMACScheduler g_omac_scheduler;
extern NeighborTable g_NeighborTable;
extern RadioControl_t g_omac_RadioControl;
extern OMACType g_OMAC;
extern RF231Radio grf231Radio;

static bool stopBeacon = false;

/*
 *
 */
void PublicBeaconNCallback(void * param){
	g_omac_scheduler.m_DiscoveryHandler.BeaconNTimerHandler();
}


/*
 *
 */
void DiscoveryHandler::Initialize(UINT8 radioID, UINT8 macID){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin( DISCO_SYNCSENDPIN, TRUE);
	CPU_GPIO_EnableOutputPin( DISCO_SYNCRECEIVEPIN, TRUE);
#endif


	m_discoveryMsg = (DiscoveryMsg_t*)m_discoveryMsgBuffer.GetPayload() ;

	m_period1 = CONTROL_P1[g_OMAC.GetAddress() % 7];
	m_period2 = CONTROL_P2[g_OMAC.GetAddress() % 7];
	hal_printf("prime 1: %d\tprime 2: %d\r\n",m_period1, m_period2);

	discoInterval = m_period1 * m_period2;	// Initially set to 1 to accelerate self-declaration as root
	hal_printf("discoInterval: %d\r\n", discoInterval);

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(HAL_DISCOVERY_TIMER, 0, SLOT_PERIOD_MILLI * 2 * MICSECINMILISEC, TRUE, FALSE, PublicBeaconNCallback); //1 sec Timer in micro seconds
}

UINT64 DiscoveryHandler::NextEvent(){
	if(!stopBeacon){
		UINT16 nextEventsSlot = 0;
		UINT64 nextEventsMicroSec = 0;
		nextEventsSlot = NextEventinSlots();
		if(nextEventsSlot == 0) {
			//hal_printf("DiscoveryHandler::NextEvent returning nextEventsMicroSec-1 :%llu\n", nextEventsMicroSec-1);
			return(nextEventsMicroSec-1);//BK: Current slot is already too late. Hence return a large number back
		}
		nextEventsMicroSec = nextEventsSlot * SLOT_PERIOD_MILLI * MICSECINMILISEC;
		nextEventsMicroSec = nextEventsMicroSec + g_omac_scheduler.GetTimeTillTheEndofSlot();
		return(nextEventsMicroSec);
	}
	else{
		return MAX_UINT64;
	}
}

/*
 *
 */
UINT64 DiscoveryHandler::NextEventinSlots(){
	UINT64 currentSlotNum = g_omac_scheduler.GetSlotNumber();
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
void DiscoveryHandler::ExecuteEvent(){
	DeviceStatus e = DS_Fail;
	e = g_omac_RadioControl.StartRx();
	if(true){//if (e == DS_Success){
		//StartBeaconNTimer(TRUE);
		VirtualTimerReturnMessage rm;
		rm = VirtTimer_Start(HAL_DISCOVERY_TIMER);
		if(rm == TimerSupported) {
			Beacon1();
		}
		else {
			PostExecuteEvent();
		}
	}
	else {
		PostExecuteEvent();
	}
}


/*
 *
 */
void DiscoveryHandler::PostExecuteEvent(){
	//stop the radio
	DeviceStatus  ds;
	ds = g_omac_RadioControl.Stop();
	if (ds != DS_Success) {
		hal_printf("DiscoveryHandler::Cannot stop the radio");
	}
	g_omac_scheduler.PostExecution();
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
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(  DISCO_SYNCSENDPIN, TRUE );
#endif
	DeviceStatus e = DS_Fail;
	UINT64 localTime = 0;

	m_discoveryMsg = (DiscoveryMsg_t*)msgPtr->GetPayload();

	m_discoveryMsg->nextSeed = g_omac_scheduler.m_DataReceptionHandler.m_nextSeed;
	m_discoveryMsg->mask = g_omac_scheduler.m_DataReceptionHandler.m_mask;
	UINT64 nextwakeupSlot = g_omac_scheduler.m_DataReceptionHandler.m_nextwakeupSlot;;
	m_discoveryMsg->nextwakeupSlot0 = (UINT32)nextwakeupSlot;
	m_discoveryMsg->nextwakeupSlot1 = (UINT32)(nextwakeupSlot>>32);
	m_discoveryMsg->seedUpdateIntervalinSlots = g_omac_scheduler.m_DataReceptionHandler.m_seedUpdateIntervalinSlots;

	m_discoveryMsg->nodeID = g_OMAC.GetAddress();
	localTime = HAL_Time_CurrentTicks();
	m_discoveryMsg->localTime0 = (UINT32) localTime ;
	m_discoveryMsg->localTime1 = (UINT32) (localTime>>32);

	e = Send(dst, msgPtr, sizeof(DiscoveryMsg_t), localTime );

	/*if (m_busy == FALSE) {
		m_busy = TRUE;
		if(!stopBeacon){
			e = Send(dst, msgPtr, sizeof(DiscoveryMsg_t), localTime );
		}
		m_busy = FALSE;
	}
	else {
		// Why busy? Timing issue?
		hal_printf("DiscoveryHandler::Beacon m_busy\n");
		e = DS_Busy;
	}*/

	return e;
}

/*
 *
 */
void DiscoveryHandler::BeaconAckHandler(Message_15_4_t* msg, UINT8 len, NetOpStatus status){
	// Don't use this for now
	if(status == NO_Busy){
		hal_printf("NO_Busy - What do we do? Just ignore?\n");
		HAL_Time_Sleep_MicroSeconds(1000);
		UINT8 radioState = grf231Radio.GetState();
		ASSERT(radioState != 8);
		if(radioState == 8){
			hal_printf("Asserting as radio is still busy\n");
			HAL_AssertEx();
		}
		//ASSERT(radioState != 8);
		//m_busy = TRUE;
	}
	else if(status == NO_Success){
		//m_busy = FALSE;
	}
	else{
		hal_printf("Need to investigate. Status: %d\n", status);
		ASSERT(0);
	}

	if (msg != &m_discoveryMsgBuffer) {
		return;
	}

#ifndef DISABLE_SIGNAL
		//call SlotScheduler.printState();
		//signalBeaconDone(error, call GlobalTime.getLocalTime());
#endif

}

/*
 *
 */
void DiscoveryHandler::Beacon1(){
	if (ShouldBeacon()) {
		DeviceStatus ds = Beacon(RADIO_BROADCAST_ADDRESS, &m_discoveryMsgBuffer);
		if(ds != DS_Success) {
			hal_printf("Beacon1 failed. ds = %d;\n", ds);
		}
		//If Beacon 1 fails, just continue operation. There is one more beacon
	}

}

/*
 *
 */
void DiscoveryHandler::BeaconN(){
	DeviceStatus ds = Beacon(RADIO_BROADCAST_ADDRESS, &m_discoveryMsgBuffer);
	if (ds != DS_Success) {
		hal_printf("BeaconN failed. ds = %d; \n", ds);
	}
}


/*
 *
 */
void DiscoveryHandler::BeaconNTimerHandler(){
	if (ShouldBeacon()) {
		BeaconN();
	}
	this->PostExecuteEvent();
}

/*
 *
 */
DeviceStatus DiscoveryHandler::Receive(Message_15_4_t* msg, void* payload, UINT8 len){
	DiscoveryMsg_t* disMsg = (DiscoveryMsg_t *) msg->GetPayload();
	RadioAddress_t source = msg->GetHeader()->src;
	Neighbor_t tempNeighbor;
	UINT8 nbrIdx;
	UINT64 localTime = HAL_Time_CurrentTicks();
	UINT64 nextwakeupSlot = (((UINT64)disMsg->nextwakeupSlot1) <<32) + disMsg->nextwakeupSlot0;

	if (g_NeighborTable.FindIndex(source, &nbrIdx) == DS_Success) {
		g_NeighborTable.UpdateNeighbor(source, Alive, localTime, disMsg->nextSeed, disMsg->mask, nextwakeupSlot, disMsg->seedUpdateIntervalinSlots, &nbrIdx);
		//stop disco when there are 2 or more neighbors
		/*if(nbrIdx >= 1){
			stopBeacon = true;
		}*/
	} else {
		g_NeighborTable.InsertNeighbor(source, Alive, localTime, disMsg->nextSeed, disMsg->mask, nextwakeupSlot, disMsg->seedUpdateIntervalinSlots, &nbrIdx);
	}

#ifdef def_Neighbor2beFollowed
	if (source == g_OMAC.Neighbor2beFollowed ){
		if (g_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(source) >=2  ){
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState(  DISCO_SYNCRECEIVEPIN, TRUE );
#endif
		}
	}
#endif

	UINT64 EventTime = PacketTimeSync_15_4::EventTime(msg,len);
	UINT64 rcv_ltime;
	INT64 l_offset;
	rcv_ltime = (((UINT64)disMsg->localTime1) <<32) + disMsg->localTime0;
	l_offset = rcv_ltime - EventTime;
	g_NeighborTable.RecordTimeSyncRecv(source, EventTime);
	g_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.Insert(source, rcv_ltime, l_offset);


#ifdef def_Neighbor2beFollowed
	if (source == g_OMAC.Neighbor2beFollowed){
		if (g_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(source) >=2  ){
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState(  DISCO_SYNCRECEIVEPIN, FALSE );
#endif
		}
	}
#endif

	return DS_Success;
}

DeviceStatus DiscoveryHandler::Send(RadioAddress_t address, Message_15_4_t* msg, UINT16 size, UINT64 event_time){
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

	retValue = g_omac_RadioControl.Send(address, msg, size + sizeof(IEEE802_15_4_Header_t), event_time);

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(  DISCO_SYNCSENDPIN, FALSE );
#endif

	return retValue;
}


