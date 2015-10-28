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

#define LOCALSKEW 1
//#define DEBUG_RADIO_STATE 1


/*
 *
 */
DeviceStatus RadioControl::Initialize(){
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
DeviceStatus RadioControl::Preload(RadioAddress_t address, Message_15_4_t * msg, UINT16 size){
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
DeviceStatus RadioControl::Send(RadioAddress_t address, Message_15_4_t* msg, UINT16 size, UINT32 eventTime){
	//Check if we can send with timestamping, 4bytes for timestamping + 8 bytes for clock value

	//Disco and DataTx handlers call this function with size parameter including the IEEE802_15_4_Header size.
	//So reduce header size from size before deciding if CPU_Radio_Send_TimeStamped or CPU_Radio_Send should be called.
	const int crc_size = 2;			//used in Radio driver's RF231Radio::Send_TimeStamped
	const int timestamp_size = 4;	//used in Radio driver's RF231Radio::Send_TimeStamped
	if( (size-sizeof(IEEE802_15_4_Header_t)) < IEEE802_15_4_MAX_PAYLOAD - (sizeof(TimeSyncMsg)+crc_size+timestamp_size) ){
		TimeSyncMsg * tmsg = (TimeSyncMsg *) (msg->GetPayload()+size);
		UINT64 y = HAL_Time_CurrentTicks();
#ifndef LOCALSKEW
		x = m_globalTime.Local2Global(y);
#endif
		tmsg->localTime0 = (UINT32) y;
		tmsg->localTime1 = (UINT32) (y>>32);
		////header->SetFlags(MFM_DATA | MFM_TIMESYNC);
		//header->SetFlags(header->GetFlags());
		size += sizeof(TimeSyncMsg);
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(RADIOCONTROL_SEND_PIN, TRUE);
		CPU_GPIO_SetPinState(RADIOCONTROL_SEND_PIN, FALSE);
#endif
		msg = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(g_OMAC.radioName, msg, size, eventTime);
	}else {
		//Radio implements the 'bag exchange' protocol, so store the pointer back to message
		if(eventTime == 0){
			msg = (Message_15_4_t *) CPU_Radio_Send(g_OMAC.radioName, msg, size);
		}
		else{
			msg = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(g_OMAC.radioName, msg, size, eventTime);
		}
	}
	return DS_Success;
}

#if 0
/*
 * Radio implements the 'bag exchange' protocol, so store the pointer back to message
 */
DeviceStatus RadioControl::Send_TimeStamped(RadioAddress_t address, Message_15_4_t* msg, UINT16 size, UINT32 eventTime){
	IEEE802_15_4_Header_t *header = msg->GetHeader();
	header->length = size + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest = address;
	header->src = CPU_Radio_GetAddress(g_OMAC.radioName);
	header->network = g_OMAC.MyConfig.Network;
	header->mac_id = g_OMAC.macName;
	////header->SetFlags(header->GetFlags() | MFM_TIMESYNC);
	header->SetFlags(header->GetFlags());
	//header->network = MyConfig.Network;

#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(RADIOCONTROL_SENDTS_PIN, TRUE);
		CPU_GPIO_SetPinState(RADIOCONTROL_SENDTS_PIN, FALSE);
		//hal_printf("RadioControl::Send_TimeStamped CPU_Radio_Send_TimeStamped\n");
#endif

	msg = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(g_OMAC.radioName, msg, size+sizeof(IEEE802_15_4_Header_t), eventTime);

	return DS_Success;
}
#endif


//DeviceStatus RadioControl::Receive(Message_15_4_t * msg, UINT16 size){
//		return DS_Success;
//}

/*
 *
 */
DeviceStatus RadioControl::Stop(){
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
DeviceStatus RadioControl::StartRx(){
	DeviceStatus returnVal = CPU_Radio_TurnOnRx(g_OMAC.radioName);
	if(returnVal == DS_Success){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( RADIOCONTROL_STATEPIN, TRUE );
#endif
	}
	return returnVal;
}


