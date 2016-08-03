/*
 * RadioControl.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan
 *
 *  Modified on: Oct 30, 2015
 *  	Authors: Bora Karaoglu; Ananth Muralidharan
 *
 *  Copyright The Samraksh Company
 */

#include <tinyhal.h>
#include <Samraksh/Radio_decl.h>
#include <Samraksh/Message.h>
#include <Samraksh/MAC/OMAC/RadioControl.h>
#include <Samraksh/MAC/OMAC/OMAC.h>

extern OMACType g_OMAC;


#define LOCALSKEW 1
//#define DEBUG_RADIO_STATE 1


/*
 *
 */
DeviceStatus RadioControl_t::Initialize(){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(RADIOCONTROL_SEND_PIN, FALSE);
	CPU_GPIO_SetPinState( RADIOCONTROL_SEND_PIN, FALSE );
	CPU_GPIO_EnableOutputPin(RADIOCONTROL_SENDTS_PIN, FALSE);
	CPU_GPIO_SetPinState( RADIOCONTROL_SENDTS_PIN, FALSE );
	CPU_GPIO_EnableOutputPin(RADIOCONTROL_STATEPIN, FALSE);
	CPU_GPIO_EnableOutputPin(RC_TX_TIMESYNCREQ, FALSE);
	CPU_GPIO_EnableOutputPin(RC_TX_DATA, FALSE);

	CPU_GPIO_EnableOutputPin(OMAC_DRIVING_RADIO_SEND, FALSE);
	CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SEND, FALSE );
	CPU_GPIO_EnableOutputPin(OMAC_DRIVING_RADIO_RECV, TRUE);
	CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_RECV, FALSE );
	CPU_GPIO_EnableOutputPin(OMAC_DRIVING_RADIO_SLEEP, TRUE);
	CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SLEEP, FALSE );

#endif
	return DS_Success;
}

/*
 *
 */
DeviceStatus RadioControl_t::Preload(RadioAddress_t address, Message_15_4_t * msg, UINT16 size){
	static UINT8 seqNumber = 0;
	UINT8 finalSeqNumber = 0;

	IEEE802_15_4_Header_t *header = msg->GetHeader();
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
	header->fcf.fcfWordValue = FCF_WORD_VALUE;
	finalSeqNumber = g_OMAC.GetMyAddress() ^ 0xAA;
	finalSeqNumber += ((g_OMAC.GetMyAddress() >> 8) ^ 0x55);
	finalSeqNumber += seqNumber;
	header->dsn = finalSeqNumber;
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
	//header->SetLength(size + sizeof(IEEE802_15_4_Header_t)+sizeof(IEEE802_15_4_Footer_t)+sizeof(IEEE802_15_4_Metadata));
	header->length = (size + sizeof(IEEE802_15_4_Header_t));

	//msg = (Message_15_4_t *) CPU_Radio_Preload(g_OMAC.radioName, (void *)msg, size+sizeof(IEEE802_15_4_Header_t)+sizeof(IEEE802_15_4_Footer_t)+sizeof(IEEE802_15_4_Metadata));
	msg = (Message_15_4_t *) CPU_Radio_Preload(g_OMAC.radioName, (void *)msg, size+sizeof(IEEE802_15_4_Header_t));
	return DS_Success;
}


/*
 *
 */
DeviceStatus RadioControl_t::Send(RadioAddress_t address, Message_15_4_t* msg, UINT16 size){
	//Check if we can send with timestamping, 4bytes for timestamping + 8 bytes for clock value
	PiggybackMessages( msg, size);
	IEEE802_15_4_Header_t *header = msg->GetHeader();
	IEEE802_15_4_Metadata* metadata = msg->GetMetaData();
	Message_15_4_t* returnMsg;

	header->length = (size);


#ifdef OMAC_DEBUG_GPIO
		if(header->payloadType == MFM_OMAC_TIMESYNCREQ){
			CPU_GPIO_SetPinState( RC_TX_TIMESYNCREQ, TRUE );
		}
		else if(header->payloadType == MFM_DATA){
			CPU_GPIO_SetPinState( RC_TX_DATA, TRUE );
			//OMAC_HAL_PRINTF("RC send; Sending: %d\n", (msg->GetPayload())[8]);
		}
#endif

	if( (header->flags & TIMESTAMPED_FLAG) ){
		//Convert TimeStamp to high freq clock
//		UINT64 time_elapsed_since_TS = g_OMAC.m_Clock.GetCurrentTimeinTicks() - msg->GetMetaData()->GetReceiveTimeStamp();
//		UINT64 event_time = HAL_Time_CurrentTicks() - time_elapsed_since_TS;
		//msg->GetMetaData()->SetReceiveTimeStamp((INT64)event_time);
		if((g_OMAC.isSendDone)){//||(g_OMAC.radioName != SI4468_SPI2)){
			//Reset flag just before sending
			g_OMAC.isSendDone = false;
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SEND, TRUE );
#endif
			returnMsg = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(g_OMAC.radioName, msg, size, (UINT32)msg->GetMetaData()->GetReceiveTimeStamp(), g_OMAC.m_Clock.GetClockSpecifier());
		}
		else{
			goto endOfSend;
		}
	}
	else {
		if((g_OMAC.isSendDone)){//||(g_OMAC.radioName != SI4468_SPI2)){
			//Reset flag just before sending
			g_OMAC.isSendDone = false;
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SEND, TRUE );
#endif
			returnMsg = (Message_15_4_t *) CPU_Radio_Send(g_OMAC.radioName, msg, size);
		}
		else{
			goto endOfSend;
		}
	}

#ifdef OMAC_DEBUG_GPIO
		if(header->payloadType == MFM_OMAC_TIMESYNCREQ){
			CPU_GPIO_SetPinState( RC_TX_TIMESYNCREQ, FALSE );
		}
		else if(header->payloadType == MFM_DATA){
			CPU_GPIO_SetPinState( RC_TX_DATA, FALSE );
		}
#endif

	if(returnMsg == msg){
		//OMAC_HAL_PRINTF("Returning success\n");
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SEND, FALSE );
#endif
		return DS_Success;
	}
endOfSend:
	//OMAC_HAL_PRINTF("Returning DS_Fail\n");
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SEND, FALSE );
	CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SEND, TRUE );
	CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SEND, FALSE );
#endif
	return DS_Fail;
}


bool RadioControl_t::PiggybackMessages(Message_15_4_t* msg, UINT16 &size){
	bool rv = false;
	IEEE802_15_4_Header_t *header = msg->GetHeader();
	IEEE802_15_4_Metadata* metadata = msg->GetMetaData();

	if(!(header->flags & MFM_TIMESYNC_FLAG) && (header->payloadType != MFM_TIMESYNC)) {
		rv = rv || PiggybackTimeSyncMessage(msg, size);
	}
	if( header->payloadType == MFM_OMAC_TIMESYNCREQ && !(header->flags & MFM_DISCOVERY_FLAG) && (header->payloadType != MFM_OMAC_DISCOVERY)) {
		if(header->dest != 0 && header->dest != RADIO_BROADCAST_ADDRESS &&  g_NeighborTable.GetNeighborPtr(header->dest)->NumTimeSyncMessagesSent < NUM_ENFORCED_TSR_PCKTS_BEFORE_DATA_PCKTS ){
		rv = rv || PiggybackDiscoMessage(msg, size);
		}
	}
	return rv;
}

bool RadioControl_t::PiggybackTimeSyncMessage(Message_15_4_t* msg, UINT16 &size){
	const int crc_size = 2;			//used in Radio driver's RF231Radio::Send_TimeStamped
	const int timestamp_size = TIMESTAMP_SIZE;	//used in Radio driver's RF231Radio::Send_TimeStamped
	int additional_overhead = crc_size;

	UINT64 event_time,y;
	UINT32 y_lo, event_time_lo;
	RadioAddress_t dest = 0;
	IEEE802_15_4_Header_t *header = msg->GetHeader();
	IEEE802_15_4_Metadata* metadata = msg->GetMetaData();

	if((header->flags & MFM_TIMESYNC_FLAG) || (header->payloadType == MFM_TIMESYNC)){ //Already embedded
		return false;
	}

	if( (header->flags & TIMESTAMPED_FLAG) ){ //Check if already stamped
		event_time = msg->GetMetaData()->GetReceiveTimeStamp();
	}
	else{ //Otherwise calculate it . Will be added later add it
		additional_overhead += timestamp_size;
		//g_OMAC.m_Clock.CreateSyncPointBetweenClocks();
		event_time = g_OMAC.m_Clock.GetCurrentTimeinTicks();
	}

	if( (size-sizeof(IEEE802_15_4_Header_t)) < IEEE802_15_4_MAX_PAYLOAD - (sizeof(TimeSyncMsg)+additional_overhead) ){
		TimeSyncMsg * tmsg = (TimeSyncMsg *) (msg->GetPayload()+(size-sizeof(IEEE802_15_4_Header_t)));
		// Event time already exists in the packet (either just added or added by the C# application earlier)
		// Adjust the time stamp of the timesync packet accordingly.
		msg->GetMetaData()->SetReceiveTimeStamp((INT64)event_time);

		CPU_GPIO_SetPinState( RADIOCONTROL_SENDTS_PIN, TRUE );
		CPU_GPIO_SetPinState( RADIOCONTROL_SENDTS_PIN, FALSE );
		CPU_GPIO_SetPinState( RADIOCONTROL_SENDTS_PIN, TRUE );
		CPU_GPIO_SetPinState( RADIOCONTROL_SENDTS_PIN, FALSE );

		header->flags = ((header->flags | TIMESTAMPED_FLAG));
		y = g_OMAC.m_Clock.GetCurrentTimeinTicks();
		y_lo = y & 0xFFFFFFFF;
		event_time_lo = event_time & 0xFFFFFFFF;
		y = y - ( y_lo - event_time_lo );
		g_OMAC.m_omac_scheduler.m_TimeSyncHandler.CreateMessage(tmsg, y);
		dest = header->dest;
		if(dest != 0 && dest != RADIO_BROADCAST_ADDRESS) {
		DeviceStatus ds = g_NeighborTable.RecordTimeSyncSent(dest,y);
			if(ds != DS_Success && dest != RADIO_BROADCAST_ADDRESS){
#ifdef OMAC_DEBUG_PRINTF
				OMAC_HAL_PRINTF("RadioControl_t::PiggybackTimeSyncMessage RecordTimeSyncSent failure; address: %d; line: %d\n", dest, __LINE__);
#endif
			}
		}
		msg->GetHeader()->flags = ((UINT8)(msg->GetHeader()->flags | MFM_TIMESYNC_FLAG));
		size += sizeof(TimeSyncMsg);
	}
}

bool RadioControl_t::PiggybackDiscoMessage(Message_15_4_t* msg, UINT16 &size){
	const int crc_size = 2;			//used in Radio driver's RF231Radio::Send_TimeStamped
	//const int timestamp_size = TIMESTAMP_SIZE;	//used in Radio driver's RF231Radio::Send_TimeStamped
	int additional_overhead = crc_size;
	IEEE802_15_4_Header_t *header = msg->GetHeader();
	IEEE802_15_4_Metadata* metadata = msg->GetMetaData();

	if((header->flags & MFM_DISCOVERY_FLAG) || (header->payloadType == MFM_OMAC_DISCOVERY)){ //Already embedded
		return false;
	}

	if( (size-sizeof(IEEE802_15_4_Header_t)) < IEEE802_15_4_MAX_PAYLOAD - (sizeof(TimeSyncMsg)+additional_overhead) ){
		DiscoveryMsg_t * tmsg = (DiscoveryMsg_t *) (msg->GetPayload()+(size-sizeof(IEEE802_15_4_Header_t)));
		g_OMAC.m_omac_scheduler.m_DiscoveryHandler.CreateMessage(tmsg);
		msg->GetHeader()->flags = ((UINT8)(msg->GetHeader()->flags | MFM_DISCOVERY_FLAG));
		size += sizeof(DiscoveryMsg_t);
	}
}




//DeviceStatus RadioControl_t::Receive(Message_15_4_t * msg, UINT16 size){
//		return DS_Success;
//}

/*
 *
 */
DeviceStatus RadioControl_t::Stop(){
	if(stayOn) return DS_Success;
	else{
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SLEEP, TRUE );
#endif
		DeviceStatus returnVal = CPU_Radio_Sleep(g_OMAC.radioName,0);

		if(returnVal == DS_Success){
	#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SLEEP, FALSE );
			CPU_GPIO_SetPinState( RADIOCONTROL_STATEPIN, FALSE );
	#endif
		}
		else{
			//OMAC_HAL_PRINTF("RadioControl_t::Stop Radio did not go to sleep\n");
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SLEEP, FALSE );
			CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SLEEP, TRUE );
			CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_SLEEP, FALSE );
#endif
		}
		return returnVal;
	}
}

/*
 *
 */
DeviceStatus RadioControl_t::StartRx(){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_RECV, TRUE );
#endif
	DeviceStatus returnVal = CPU_Radio_TurnOnRx(g_OMAC.radioName);
	if(returnVal == DS_Success){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( RADIOCONTROL_STATEPIN, TRUE );
		CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_RECV, FALSE );
#endif
	}
	else{
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_RECV, FALSE );
	CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_RECV, TRUE );
	CPU_GPIO_SetPinState( OMAC_DRIVING_RADIO_RECV, FALSE );
#endif
	}
	return returnVal;
}


