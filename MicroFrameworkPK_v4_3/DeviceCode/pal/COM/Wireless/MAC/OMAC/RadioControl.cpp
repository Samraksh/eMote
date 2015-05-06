/*
 * RadioWrapper.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan
 */

#include <tinyhal.h>
#include <Samraksh/Radio_decl.h>
#include <Samraksh/Message.h>
#include "RadioControl.h"
#include "OMACConstants.h"
#include "OMAC.h"

extern OMACTypeBora g_OMAC;

#define LOCALSKEW 1
#define DEBUG_TIMESYNCPIN_OLD (GPIO_PIN)31

#define RADIO_STATEPIN 4

#define DEBUG_RADIO_STATE 1
//#define DEBUG_TIMESYNC 1

DeviceStatus RadioControl::Initialize(){
	CPU_GPIO_EnableOutputPin((GPIO_PIN) RADIO_STATEPIN, FALSE);
	CPU_GPIO_SetPinState( (GPIO_PIN) RADIO_STATEPIN, TRUE );
	CPU_GPIO_SetPinState( (GPIO_PIN) RADIO_STATEPIN, FALSE );
#ifdef DEBUG_TIMESYNC
	CPU_GPIO_EnableOutputPin(DEBUG_TIMESYNCPIN_OLD, FALSE);
#endif
	return DS_Success;
}


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

DeviceStatus RadioControl::Send(RadioAddress_t address, Message_15_4_t * msg, UINT16 size){


	IEEE802_15_4_Header_t *header = msg->GetHeader();
		header->length = size + sizeof(IEEE802_15_4_Header_t);
		header->fcf = (65 << 8);
		header->fcf |= 136;
		header->dsn = 97;
		header->destpan = (34 << 8);
		header->destpan |= 0;
		header->dest =address;
		header->src = CPU_Radio_GetAddress(g_OMAC.radioName);
		header->mac_id = g_OMAC.macName;
		//header->network = MyConfig.Network;

	//Check if we can send with timestamping, 4bytes for timestamping + 8 bytes for clock value
	if(size < IEEE802_15_4_MAX_PAYLOAD-(sizeof(TimeSyncMsg)+4)){
		TimeSyncMsg * tmsg = (TimeSyncMsg *) (msg->GetPayload()+size);
		UINT64 y = HAL_Time_CurrentTicks();
	#ifndef LOCALSKEW
		x = m_globalTime.Local2Global(y);
	#endif
		tmsg->localTime0 = (UINT32) y;
		tmsg->localTime1 = (UINT32) (y>>32);
		header->SetFlags(MFM_DATA | MFM_TIMESYNC);
		size += sizeof(TimeSyncMsg);
#ifdef DEBUG_TIMESYNC
		hal_printf("Added timsync to outgoing message: Localtime: %llu \n", y);
		CPU_GPIO_SetPinState(DEBUG_TIMESYNCPIN_OLD, TRUE);
		CPU_GPIO_SetPinState(DEBUG_TIMESYNCPIN_OLD, FALSE);
#endif
		msg = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(g_OMAC.radioName, msg, size+sizeof(IEEE802_15_4_Header_t), tmsg->localTime0);
	}else {
	//Radio implements the 'bag exchange' protocol, so store the pointer back to message
		msg = (Message_15_4_t *) CPU_Radio_Send(g_OMAC.radioName, msg, size+sizeof(IEEE802_15_4_Header_t));
	}
	return DS_Success;
}

DeviceStatus RadioControl::Send_TimeStamped(RadioAddress_t address, Message_15_4_t * msg, UINT16 size, UINT32 eventTime){
	//Radio implements the 'bag exchange' protocol, so store the pointer back to message

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

	header->SetFlags(header->GetFlags() | MFM_TIMESYNC);
	//header->network = MyConfig.Network;
#ifdef DEBUG_TIMESYNC
		CPU_GPIO_SetPinState(DEBUG_TIMESYNCPIN_OLD, TRUE);
		CPU_GPIO_SetPinState(DEBUG_TIMESYNCPIN_OLD, FALSE);
#endif
	msg = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(g_OMAC.radioName, msg, size+sizeof(IEEE802_15_4_Header_t), eventTime);
	return DS_Success;
}


//DeviceStatus RadioControl::Receive(Message_15_4_t * msg, UINT16 size){
//		return DS_Success;
//}

DeviceStatus RadioControl::Stop(){
	//DeviceStatus returnVal = DS_Success;
	DeviceStatus returnVal = CPU_Radio_Sleep(g_OMAC.radioName,0);
	CPU_GPIO_SetPinState( (GPIO_PIN) RADIO_STATEPIN, FALSE );

	return returnVal;
}

DeviceStatus RadioControl::Start(){
	CPU_Radio_TurnOnRx(g_OMAC.radioName);

	CPU_GPIO_SetPinState( (GPIO_PIN) RADIO_STATEPIN, TRUE );
	return DS_Ready;
}
