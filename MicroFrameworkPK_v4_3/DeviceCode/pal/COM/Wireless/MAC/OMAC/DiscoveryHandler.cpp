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

extern OMACType g_OMAC;


/*
 *
 */
void PublicBeaconNCallback(void * param){
	g_OMAC.m_omac_scheduler.m_DiscoveryHandler.BeaconNTimerHandler();
}


/*
 *
 */
void DiscoveryHandler::Initialize(UINT8 radioID, UINT8 macID){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin( DISCO_SYNCSENDPIN, TRUE);
	CPU_GPIO_EnableOutputPin( DISCO_SYNCRECEIVEPIN, TRUE);
	CPU_GPIO_SetPinState(  DISCO_SYNCSENDPIN, FALSE );
	CPU_GPIO_SetPinState(  DISCO_SYNCRECEIVEPIN, FALSE );
#endif


	//m_discoveryMsg = (DiscoveryMsg_t*)m_discoveryMsgBuffer.GetPayload() ;
	firstDiscoTimeinSlotNum = 0;

	m_period1 = CONTROL_P1[g_OMAC.GetMyAddress() % 7] ;
	m_period2 = CONTROL_P2[g_OMAC.GetMyAddress() % 7] ;
	highdiscorate = true;
	hal_printf("prime 1: %d\tprime 2: %d\r\n",m_period1, m_period2);

	discoInterval = m_period1 * m_period2;	// Initially set to 1 to accelerate self-declaration as root
	hal_printf("discoInterval: %d\r\n", discoInterval);

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_DISCOVERY, 0, SLOT_PERIOD_MILLI * 2 * MICSECINMILISEC, TRUE, FALSE, PublicBeaconNCallback, OMACClockSpecifier); //1 sec Timer in micro seconds
	//ASSERT_SP(rm == TimerSupported);
}

UINT64 DiscoveryHandler::NextEvent(){
	UINT64 currentSlotNum = g_OMAC.m_omac_scheduler.GetSlotNumber();
	UINT16 nextEventsSlot = 0;
	UINT64 nextEventsMicroSec = 0;
	if(firstDiscoTimeinSlotNum == 0) {
		firstDiscoTimeinSlotNum = currentSlotNum;
	}
	if(highdiscorate && ( (currentSlotNum - firstDiscoTimeinSlotNum) / HIGH_DISCO_PERIOD_IN_SLOTS ) %2 == 1 ) {
		m_period1 = m_period1 * 100;
		m_period2 = m_period2 * 100;
		highdiscorate = false;
	}

	nextEventsSlot = NextEventinSlots(currentSlotNum);
	if(nextEventsSlot == 0) {
		currentSlotNum = currentSlotNum +1;
		nextEventsSlot = NextEventinSlots(currentSlotNum);
		nextEventsSlot = nextEventsSlot + 1;
	}
	nextEventsMicroSec = nextEventsSlot * SLOT_PERIOD_MILLI * MICSECINMILISEC;
	nextEventsMicroSec = nextEventsMicroSec + g_OMAC.m_omac_scheduler.GetTimeTillTheEndofSlot();
	return(nextEventsMicroSec);
}

/*
 *
 */
UINT64 DiscoveryHandler::NextEventinSlots(const UINT64 &currentSlotNum){
	//UINT64 currentSlotNum = g_OMAC.m_omac_scheduler.GetSlotNumber();
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
	e = g_OMAC.m_omac_RadioControl.StartRx();
	if (e == DS_Success){
		VirtualTimerReturnMessage rm;
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_DISCOVERY);
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
	//Scheduler's PostExecution stops the radio
	//stop the radio
	/*DeviceStatus  ds;
	ds = g_OMAC.m_omac_RadioControl.Stop();
	if (ds != DS_Success) {
		hal_printf("DiscoveryHandler::Cannot stop the radio");
	}*/
	g_OMAC.m_omac_scheduler.PostExecution();
}

void DiscoveryHandler::FailsafeStop(){
	VirtTimer_Stop(VIRT_TIMER_OMAC_DISCOVERY);
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

	DiscoveryMsg_t* m_discoveryMsg = (DiscoveryMsg_t*)msgPtr->GetPayload();

	CreateMessage(m_discoveryMsg);

	//localTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
	//m_discoveryMsg->localTime0 = (UINT32) localTime ;
	//m_discoveryMsg->localTime1 = (UINT32) (localTime>>32);

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

void DiscoveryHandler::CreateMessage(DiscoveryMsg_t* discoveryMsg){
	discoveryMsg->nextSeed = g_OMAC.m_omac_scheduler.m_DataReceptionHandler.m_nextSeed;
	discoveryMsg->mask = g_OMAC.m_omac_scheduler.m_DataReceptionHandler.m_mask;
	UINT64 nextwakeupSlot = g_OMAC.m_omac_scheduler.m_DataReceptionHandler.m_nextwakeupSlot;
	discoveryMsg->nextwakeupSlot0 = (UINT32)nextwakeupSlot;
	discoveryMsg->nextwakeupSlot1 = (UINT32)(nextwakeupSlot>>32);
	discoveryMsg->seedUpdateIntervalinSlots = g_OMAC.m_omac_scheduler.m_DataReceptionHandler.m_seedUpdateIntervalinSlots;
	discoveryMsg->nodeID = g_OMAC.GetMyAddress();

	UINT64 curticks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
	discoveryMsg->localTime0 = (UINT32) curticks;
	discoveryMsg->localTime1 = (UINT32) (curticks>>32);

	discoveryMsg->lastwakeupSlotUpdateTimeinTicks0 = (UINT32) g_OMAC.m_omac_scheduler.m_DataReceptionHandler.lastwakeupSlotUpdateTimeinTicks;
	discoveryMsg->lastwakeupSlotUpdateTimeinTicks1 = (UINT32) (g_OMAC.m_omac_scheduler.m_DataReceptionHandler.lastwakeupSlotUpdateTimeinTicks>>32);
	discoveryMsg->msg_identifier = 33686018; // 0x02020202
}

/*
 *
 */
void DiscoveryHandler::BeaconAckHandler(Message_15_4_t* msg, UINT8 len, NetOpStatus status){
	/* Don't use this for now
	if(status == NO_Busy){
		hal_printf("NO_Busy - What do we do? Just ignore?\n");
		//m_busy = TRUE;
	}
	else if(status == NO_Success){
		//m_busy = FALSE;
	}
	else{
		hal_printf("Need to investigate. Status: %d\n", status);
		ASSERT_SP(0);
	}

	if (msg != &m_discoveryMsgBuffer) {
		return;
	}

#ifndef DISABLE_SIGNAL
		//call SlotScheduler.printState();
		//signalBeaconDone(error, call GlobalTime.getLocalTime());
#endif
*/
}

/*
 *
 */
void DiscoveryHandler::Beacon1(){
	//Message_15_4_t m_discoveryMsgBuffer;
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
	//Message_15_4_t m_discoveryMsgBuffer;
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
DeviceStatus DiscoveryHandler::Receive(RadioAddress_t source, DiscoveryMsg_t* disMsg){  //(Message_15_4_t* msg, void* payload, UINT8 len){
	Neighbor_t tempNeighbor;
	UINT8 nbrIdx;
	UINT64 localTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();

	if(disMsg -> msg_identifier != 33686018){
		localTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
		//ASSERT_SP(0);
	}
#ifdef def_Neighbor2beFollowed
	if (source == g_OMAC.Neighbor2beFollowed){
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState(  DISCO_SYNCRECEIVEPIN, TRUE );
#endif
	}
#endif

#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(DISCO_SYNCRECEIVEPIN, TRUE );
#endif
	//DiscoveryMsg_t* disMsg = (DiscoveryMsg_t *) msg->GetPayload();
	//RadioAddress_t source = msg->GetHeader()->src;

	UINT64 nextwakeupSlot = (((UINT64)disMsg->nextwakeupSlot1) <<32) + disMsg->nextwakeupSlot0;

	if (g_OMAC.m_NeighborTable.FindIndex(source, &nbrIdx) == DS_Success) {
		g_OMAC.m_NeighborTable.UpdateNeighbor(source, Alive, localTime, disMsg->nextSeed, disMsg->mask, nextwakeupSlot, disMsg->seedUpdateIntervalinSlots, &nbrIdx);
		//stop disco when there are 2 or more neighbors
		/*if(nbrIdx >= 1){
			stopBeacon = true;
		}*/
	} else {
		g_OMAC.m_NeighborTable.InsertNeighbor(source, Alive, localTime, disMsg->nextSeed, disMsg->mask, nextwakeupSlot, disMsg->seedUpdateIntervalinSlots, &nbrIdx);
	}

#ifdef def_Neighbor2beFollowed
	if (source == g_OMAC.Neighbor2beFollowed){
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState(  DISCO_SYNCRECEIVEPIN, FALSE );
#endif
	}
#endif
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(DISCO_SYNCRECEIVEPIN, FALSE );
#endif

	return DS_Success;
}

DeviceStatus DiscoveryHandler::Send(RadioAddress_t address, Message_15_4_t* msg, UINT16 size, UINT64 event_time){
	DeviceStatus retValue;
	static UINT8 seqNumber = 0;
	UINT8 finalSeqNumber = 0;

	IEEE802_15_4_Header_t * header = msg->GetHeader();
	/****** Taking the word value of below bits gives 26150 *******/
	/*header->fcf->IEEE802_15_4_Header_FCF_BitValue.frameType = FRAME_TYPE_MAC;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.securityEnabled = 0;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.framePending = 0;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.ackRequired = 1;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.intraPAN = 1;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.reserved = 0;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.destAddrMode = 2;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.frameVersion = 1;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.srcAddrMode = 2;*/
	/**************************************************************/
	header->fcf.fcfWordValue = 26150;
	//header->fcf = (65 << 8);
	//header->fcf |= 136;
	finalSeqNumber = g_OMAC.GetMyAddress() ^ 0xAA;
	finalSeqNumber += ((g_OMAC.GetMyAddress() >> 8) ^ 0x55);
	finalSeqNumber += seqNumber;
	//header->dsn = finalSeqNumber;
	header->dsn = OMAC_DISCO_SEQ_NUMBER;
	header->srcpan = 0x0001;
	header->destpan = 0x0001;
	/*if(g_OMAC.GetMyAddress() == 6846){
		header->dest = 0x0DB1;
	}
	else{
		header->dest = 0x1ABE;
	}*/
	//6846(0x1ABE) is the receiver
	/*if(g_OMAC.GetMyAddress() != 0x1ABE){
		header->dest = 0x1ABE;
	}
	else{
		header->dest = 0x1111;
	}*/
	header->dest = address;
	header->src = g_OMAC.GetMyAddress();
	//header->retryAttempt = 0;
	seqNumber++;

	IEEE802_15_4_Metadata* metadata = msg->GetMetaData();
	//UINT8 * payload = msg->GetPayload();
	header->length = (size + sizeof(IEEE802_15_4_Header_t));
	//header->SetLength(IEEE802_15_4_MAX_PAYLOAD + sizeof(IEEE802_15_4_Header_t) + sizeof(IEEE802_15_4_Footer_t) + sizeof(IEEE802_15_4_Metadata));
	header->mac_id = (g_OMAC.macName);
	header->type = (MFM_DISCOVERY);
	header->flags = (0); //Initialize flags to zero

	msg->GetMetaData()->SetReceiveTimeStamp((INT64)event_time);

	retValue = g_OMAC.m_omac_RadioControl.Send(address, msg, header->length);

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(DISCO_SYNCSENDPIN, FALSE );
#endif

	return retValue;
}


