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
//#include <DeviceCode\LCD_PCF85162_HAL\LCD_PCF85162.h>

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
	CPU_GPIO_EnableOutputPin(  DISCO_NEXT_EVENT, FALSE );
	CPU_GPIO_SetPinState(  DISCO_NEXT_EVENT, FALSE );
	CPU_GPIO_EnableOutputPin(DISCO_BEACON_N, TRUE);
	CPU_GPIO_SetPinState( DISCO_BEACON_N, FALSE );
#endif


	//m_discoveryMsg = (DiscoveryMsg_t*)m_discoveryMsgBuffer.GetPayload() ;
	PermanentlyDecreaseDiscoRate();
	TempIncreaseDiscoRate();
	hal_printf("prime 1: %d\tprime 2: %d\r\n",m_period1, m_period2);

	discoInterval = m_period1 * m_period2;	// Initially set to 1 to accelerate self-declaration as root
	hal_printf("discoInterval: %d\r\n", discoInterval);

	m_disco_getting_send = 0;

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_DISCOVERY, 0, SLOT_PERIOD_MILLI * DISCOPERIODINSLOTS * MICSECINMILISEC, TRUE, FALSE, PublicBeaconNCallback, OMACClockSpecifier); //1 sec Timer in micro seconds
	//ASSERT_SP(rm == TimerSupported);
}

UINT64 DiscoveryHandler::NextEvent(){
	UINT64 currentSlotNum = g_OMAC.m_omac_scheduler.GetSlotNumber();
	UINT16 nextEventsSlot = 0;
	UINT64 nextEventsMicroSec = 0;
	if(firstHighRateDiscoTimeinSlotNum == 0) {
		firstHighRateDiscoTimeinSlotNum = currentSlotNum;
	}

	if(highdiscorate && ( (currentSlotNum - firstHighRateDiscoTimeinSlotNum) > HIGH_DISCO_PERIOD_IN_SLOTS ) ) {
		PermanentlyDecreaseDiscoRate();
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
	period1Remaining = (currentSlotNum/2) % m_period1;
	period2Remaining = (currentSlotNum/2) % m_period2;

	if (period1Remaining == 0 || period2Remaining == 0) {
		return 0;
	}
	else  {
		if(period1Remaining < period2Remaining){
			return (2*period1Remaining);
		}
		else{
			return (2*period2Remaining);
		}
//		return ((period1Remaining < period2Remaining) ? (2*period1Remaining ) : (2*period2Remaining ));
	}
}

/*
 *
 */
void DiscoveryHandler::ExecuteEvent(){
	DeviceStatus e = DS_Fail;
	VirtualTimerReturnMessage rm;
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(  DISCO_NEXT_EVENT, TRUE );
#endif
	e = g_OMAC.m_omac_RadioControl.StartRx();
	if (e == DS_Success){
		//hal_printf("DiscoveryHandler::ExecuteEvent turned on Rx\n");
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(  DISCO_NEXT_EVENT, FALSE );
#endif
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_DISCOVERY, 0, SLOT_PERIOD_MILLI * DISCOPERIODINSLOTS * MICSECINMILISEC - TIMEITTAKES2TXDISCOPACKETINMICSEC, FALSE, OMACClockSpecifier); //1 sec Timer in micro seconds
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_DISCOVERY);
		if(rm == TimerSupported) {
			Beacon1();
		}
		else {
			//rm = VirtTimer_Start(VIRT_TIMER_OMAC_POST_EXEC);
			PostExecuteEvent();
		}
	}
	else {
		//hal_printf("DiscoveryHandler::ExecuteEvent Could not turn on Rx\n");
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(  DISCO_NEXT_EVENT, FALSE );
		CPU_GPIO_SetPinState(  DISCO_NEXT_EVENT, TRUE );
		CPU_GPIO_SetPinState(  DISCO_NEXT_EVENT, FALSE );
#endif
		//hal_printf("DiscoveryHandler::ExecuteEvent StartRx not successful\n");
		//rm = VirtTimer_Start(VIRT_TIMER_OMAC_POST_EXEC);
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
	if(m_disco_getting_send == false){
		return TRUE;
	}
	else {
		return FALSE;
	}
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

	//localTime = g_OMAC.m_Clock.GetCurrentTimeinTicks();
	//m_discoveryMsg->localTime0 = (UINT32) localTime ;
	//m_discoveryMsg->localTime1 = (UINT32) (localTime>>32);

	e = Send(dst, msgPtr, sizeof(DiscoveryMsg_t), localTime );

	if(e == DS_Success){
		m_disco_getting_send = true;
	}
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
	//discoveryMsg->nodeID = g_OMAC.GetMyAddress();

//	UINT64 curticks = g_OMAC.m_Clock.GetCurrentTimeinTicks();
//	discoveryMsg->localTime0 = (UINT32) curticks;
//	discoveryMsg->localTime1 = (UINT32) (curticks>>32);
//
//	discoveryMsg->lastwakeupSlotUpdateTimeinTicks0 = (UINT32) g_OMAC.m_omac_scheduler.m_DataReceptionHandler.lastwakeupSlotUpdateTimeinTicks;
//	discoveryMsg->lastwakeupSlotUpdateTimeinTicks1 = (UINT32) (g_OMAC.m_omac_scheduler.m_DataReceptionHandler.lastwakeupSlotUpdateTimeinTicks>>32);
//	discoveryMsg->msg_identifier = 33686018; // 0x02020202
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
	m_disco_getting_send = false;
	/*if(m_disco_state == DISCO_STATE_BEACON_N){
		VirtualTimerReturnMessage rm;
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_DISCOVERY);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_DISCOVERY, 0, 0, FALSE, OMACClockSpecifier); //1 sec Timer in micro seconds
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_DISCOVERY);
	}*/
}

/*
 *
 */
void DiscoveryHandler::Beacon1(){
	//Message_15_4_t m_discoveryMsgBuffer;
	m_disco_getting_send = false;
	m_disco_state = DISCO_STATE_BEACON_1;
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
	m_disco_getting_send = false;
	m_disco_state = DISCO_STATE_BEACON_N;
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DISCO_BEACON_N, TRUE );
#endif

	DeviceStatus ds = Beacon(RADIO_BROADCAST_ADDRESS, &m_discoveryMsgBuffer);
	if (ds != DS_Success) {
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DISCO_BEACON_N, FALSE );
		CPU_GPIO_SetPinState( DISCO_BEACON_N, TRUE );
#endif
		hal_printf("BeaconN failed. ds = %d; \n", ds);
	}
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DISCO_BEACON_N, FALSE );
#endif
	this->PostExecuteEvent();
}


/*
 *
 */
void DiscoveryHandler::BeaconNTimerHandler(){
	VirtualTimerReturnMessage rm;
	if(m_disco_state == DISCO_STATE_BEACON_N ){
		this->PostExecuteEvent();
		/*if( m_disco_getting_send == false){
			//rm = VirtTimer_Start(VIRT_TIMER_OMAC_POST_EXEC);
			this->PostExecuteEvent();
		}
		else{
			rm = VirtTimer_Change(VIRT_TIMER_OMAC_DISCOVERY, 0, SLOT_PERIOD_MILLI * DISCOPERIODINSLOTS * MICSECINMILISEC, FALSE, OMACClockSpecifier); //1 sec Timer in micro seconds
			rm = VirtTimer_Start(VIRT_TIMER_OMAC_DISCOVERY);
			if(rm  != TimerSupported){ //In this case no need to do anything. Failsafe timer will restore operation.
				//PostExecuteEvent();
			}
		}*/
	}
	else{
		m_disco_state = DISCO_STATE_BEACON_N;
		if (ShouldBeacon()) {
			BeaconN();
		}
	}

}

/*
 *
 */
DeviceStatus DiscoveryHandler::Receive(RadioAddress_t source, DiscoveryMsg_t* disMsg){  //(Message_15_4_t* msg, void* payload, UINT8 len){
	Neighbor_t tempNeighbor;
	UINT8 nbrIdx;
	UINT64 localTime = g_OMAC.m_Clock.GetCurrentTimeinTicks();

//	if(disMsg -> msg_identifier != 33686018){
//		localTime = g_OMAC.m_Clock.GetCurrentTimeinTicks();
//		//ASSERT_SP(0);
//	}
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

	if (g_NeighborTable.FindIndex(source, &nbrIdx) == DS_Success) {
		if(g_NeighborTable.Neighbor[nbrIdx].Status != Alive) {
			TempIncreaseDiscoRate();
		}
		hal_printf("DiscoveryHandler::Receive UpdateNeighbor\n");
		g_NeighborTable.UpdateNeighbor(source, Alive, localTime, disMsg->nextSeed, disMsg->mask, nextwakeupSlot, disMsg->seedUpdateIntervalinSlots, &nbrIdx);
		//stop disco when there are 2 or more neighbors
		/*if(nbrIdx >= 1){
			stopBeacon = true;
		}*/
	} else {
		TempIncreaseDiscoRate();
		hal_printf("DiscoveryHandler::Receive InsertNeighbor\n");
		g_NeighborTable.InsertNeighbor(source, Alive, localTime, disMsg->nextSeed, disMsg->mask, nextwakeupSlot, disMsg->seedUpdateIntervalinSlots, &nbrIdx);
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
	/****** Taking the word value of below bits gives FCF_WORD_VALUE *******/
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
	header->fcf.fcfWordValue = FCF_WORD_VALUE_DISCO;
	//header->fcf = (65 << 8);
	//header->fcf |= 136;
	finalSeqNumber = g_OMAC.GetMyAddress() ^ 0xAA;
	finalSeqNumber += ((g_OMAC.GetMyAddress() >> 8) ^ 0x55);
	finalSeqNumber += seqNumber;
	//header->dsn = finalSeqNumber;
	header->dsn = OMAC_DISCO_SEQ_NUMBER;
	//header->srcpan = SRC_PAN_ID;
	header->destpan = DEST_PAN_ID;
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
	header->macName = (g_OMAC.macName);
	header->payloadType = (MFM_OMAC_DISCOVERY);
	header->flags = (0); //Initialize flags to zero

	msg->GetMetaData()->SetReceiveTimeStamp((INT64)event_time);

	retValue = g_OMAC.m_omac_RadioControl.Send(address, msg, header->length);

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(DISCO_SYNCSENDPIN, FALSE );
#endif

	return retValue;
}

void DiscoveryHandler::TempIncreaseDiscoRate(){
	m_period1 = CONTROL_P1[g_OMAC.GetMyAddress() % 7] ;
	m_period2 = CONTROL_P2[g_OMAC.GetMyAddress() % 7] ;
	highdiscorate = true;
	firstHighRateDiscoTimeinSlotNum = g_OMAC.m_omac_scheduler.GetSlotNumber();
	 g_OMAC.m_omac_RadioControl.stayOn = true;
}

void DiscoveryHandler::PermanentlyDecreaseDiscoRate(){
	m_period1 = CONTROL_P3[g_OMAC.GetMyAddress() % 7] ;
	m_period2 = CONTROL_P4[g_OMAC.GetMyAddress() % 7] ;
	g_OMAC.m_omac_RadioControl.stayOn = false;
}
