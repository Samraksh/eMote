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
#include <Samraksh/MAC/OMAC/OMACConstants.h>
#include <Samraksh/MAC/OMAC/OMAC.h>

extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;

#define LOCALSKEW 1
//#define DEBUG_RADIO_STATE 1


/*
 *
 */
DeviceStatus RadioControl_t::Initialize(){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(RADIOCONTROL_SEND_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(RADIOCONTROL_SENDTS_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(RADIOCONTROL_STATEPIN, FALSE);
#endif
	return DS_Success;
}

/*
 *
 */
DeviceStatus RadioControl_t::Preload(RadioAddress_t address, Message_15_4_t * msg, UINT16 size){
	IEEE802_15_4_Header_t *header = msg->GetHeader();
	header->length = size + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest =address;
	header->src = CPU_Radio_GetAddress(g_OMAC.radioName);

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

	header->length = size;
	if( (header->GetFlags() & TIMESTAMPED_FLAG) ){
		msg = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(g_OMAC.radioName, msg, size, (UINT32)msg->GetMetaData()->GetReceiveTimeStamp());
	}
	else {
		msg = (Message_15_4_t *) CPU_Radio_Send(g_OMAC.radioName, msg, size);
	}
	return DS_Success;
}


bool RadioControl_t::PiggybackMessages(Message_15_4_t* msg, UINT16 &size){
	bool rv = false;
	IEEE802_15_4_Header_t *header = msg->GetHeader();

	if(!(header->GetFlags() & MFM_TIMESYNC) && (header->type != MFM_TIMESYNC)) {
		rv = rv || PiggybackTimeSyncMessage(msg, size);
	}
	if(!(header->GetFlags() & MFM_DISCOVERY) && (header->type != MFM_DISCOVERY)) {
		rv = rv || PiggybackDiscoMessage(msg, size);
	}
	return rv;
}

bool RadioControl_t::PiggybackTimeSyncMessage(Message_15_4_t* msg, UINT16 &size){
	const int crc_size = 2;			//used in Radio driver's RF231Radio::Send_TimeStamped
	const int timestamp_size = TIMESTAMP_SIZE;	//used in Radio driver's RF231Radio::Send_TimeStamped
	int additional_overhead = crc_size;

	UINT64 event_time,y;
	UINT32 y_lo, event_time_lo;
	IEEE802_15_4_Header_t *header = msg->GetHeader();

	if((header->GetFlags() & MFM_TIMESYNC) || (header->type == MFM_TIMESYNC)){ //Already embedded
		return false;
	}

	if( (header->GetFlags() & TIMESTAMPED_FLAG) ){ //Check if already stamped
		event_time = msg->GetMetaData()->GetReceiveTimeStamp();
	}
	else{ //Otherwise add it
		additional_overhead += timestamp_size;
		header->SetFlags((header->GetFlags() | TIMESTAMPED_FLAG));
		event_time = HAL_Time_CurrentTicks();
		msg->GetMetaData()->SetReceiveTimeStamp(event_time);
	}

	if( (size-sizeof(IEEE802_15_4_Header_t)) < IEEE802_15_4_MAX_PAYLOAD - (sizeof(TimeSyncMsg)+additional_overhead) ){
		TimeSyncMsg * tmsg = (TimeSyncMsg *) (msg->GetPayload()+(size-sizeof(IEEE802_15_4_Header_t)));
		// Event time already exists in the packet (either just added or added by the C# application earlier)
		// Adjust the time stamp of the timesync packet accordingly.
		y_lo = y & 0xFFFFFFFF;
		event_time_lo = event_time & 0xFFFFFFFF;
		y = y - ( y_lo - event_time_lo );
		g_omac_scheduler.m_TimeSyncHandler.CreateMessage(tmsg, y);
		msg->GetHeader()->SetFlags((UINT8)(msg->GetHeader()->GetFlags() | MFM_TIMESYNC));
		size += sizeof(TimeSyncMsg);
	}
}

bool RadioControl_t::PiggybackDiscoMessage(Message_15_4_t* msg, UINT16 &size){
	const int crc_size = 2;			//used in Radio driver's RF231Radio::Send_TimeStamped
	//const int timestamp_size = TIMESTAMP_SIZE;	//used in Radio driver's RF231Radio::Send_TimeStamped
	int additional_overhead = crc_size;
	IEEE802_15_4_Header_t *header = msg->GetHeader();

	if((header->GetFlags() & MFM_DISCOVERY) || (header->type == MFM_DISCOVERY)){ //Already embedded
		return false;
	}

	if( (size-sizeof(IEEE802_15_4_Header_t)) < IEEE802_15_4_MAX_PAYLOAD - (sizeof(TimeSyncMsg)+additional_overhead) ){
		DiscoveryMsg_t * tmsg = (DiscoveryMsg_t *) (msg->GetPayload()+(size-sizeof(IEEE802_15_4_Header_t)));
		g_omac_scheduler.m_DiscoveryHandler.CreateMessage(tmsg);
		msg->GetHeader()->SetFlags((UINT8)(msg->GetHeader()->GetFlags() | MFM_DISCOVERY));
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
	DeviceStatus returnVal = CPU_Radio_Sleep(g_OMAC.radioName,0);

	if(returnVal == DS_Success){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( RADIOCONTROL_STATEPIN, FALSE );
#endif
	}
	return returnVal;
}

/*
 *
 */
DeviceStatus RadioControl_t::StartRx(){
	DeviceStatus returnVal = CPU_Radio_TurnOnRx(g_OMAC.radioName);
	if(returnVal == DS_Success){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( RADIOCONTROL_STATEPIN, TRUE );
#endif
	}
	return returnVal;
}


