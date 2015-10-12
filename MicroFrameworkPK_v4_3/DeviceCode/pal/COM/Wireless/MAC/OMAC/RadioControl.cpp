/*
 * RadioWrapper.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan
 */

#include <tinyhal.h>
#include <Samraksh/Radio_decl.h>
#include <Samraksh/Message.h>
#include <Samraksh/MAC/OMAC/RadioControl.h>
#include <Samraksh/MAC/OMAC/OMACConstants.h>
#include <Samraksh/MAC/OMAC/OMAC.h>

extern OMACType g_OMAC;

#define LOCALSKEW 1
//#define RADIOCONTROL_SEND_PIN 120 //(GPIO_PIN)31

//#define RADIO_STATEPIN 4 // 120 //4

//#define DEBUG_RADIO_STATE 1
#define DEBUG_TIMESYNC 1

extern Buffer_15_4_t g_send_buffer;

/*
 *
 */
DeviceStatus RadioControl::Initialize(){
	CPU_GPIO_EnableOutputPin(RADIOCONTROL_SEND_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(RADIOCONTROL_SENDTS_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(RADIOCONTROL_STATEPIN, FALSE);
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
DeviceStatus RadioControl::Send(RadioAddress_t address, Message_15_4_t* msg, UINT16 size){
	IEEE802_15_4_Header_t *header = msg->GetHeader();
	header->length = size + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest = address;
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
		////header->SetFlags(MFM_DATA | MFM_TIMESYNC);
		header->SetFlags(header->GetFlags());
		size += sizeof(TimeSyncMsg);
#ifdef DEBUG_TIMESYNC
		hal_printf("Added timsync to outgoing message: Localtime: %llu \n", y);
		CPU_GPIO_SetPinState(RADIOCONTROL_SEND_PIN, TRUE);
		CPU_GPIO_SetPinState(RADIOCONTROL_SEND_PIN, FALSE);
		hal_printf("RadioControl::Send CPU_Radio_Send_TimeStamped\n");
#endif
		msg = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(g_OMAC.radioName, msg, size+sizeof(IEEE802_15_4_Header_t), tmsg->localTime0);
	}else {
		//Radio implements the 'bag exchange' protocol, so store the pointer back to message
#ifdef DEBUG_TIMESYNC
		hal_printf("RadioControl::Send CPU_Radio_Send\n");
#endif
		msg = (Message_15_4_t *) CPU_Radio_Send(g_OMAC.radioName, msg, size+sizeof(IEEE802_15_4_Header_t));
	}
	return DS_Success;
}

/*
 * Radio implements the 'bag exchange' protocol, so store the pointer back to message
 */
DeviceStatus RadioControl::Send_TimeStamped(RadioAddress_t address, Message_15_4_t * msg, UINT16 size, UINT32 eventTime){
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

#ifdef DEBUG_TIMESYNC
		CPU_GPIO_SetPinState(RADIOCONTROL_SENDTS_PIN, TRUE);
		CPU_GPIO_SetPinState(RADIOCONTROL_SENDTS_PIN, FALSE);
		//hal_printf("RadioControl::Send_TimeStamped CPU_Radio_Send_TimeStamped\n");
#endif

	msg = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(g_OMAC.radioName, msg, size+sizeof(IEEE802_15_4_Header_t), eventTime);


	/*Message_15_4_t txMsg;
	Message_15_4_t* txMsgPtr = &txMsg;
	Message_15_4_t** tempPtr = g_send_buffer.GetOldestPtr();
	Message_15_4_t* msgPtr = *tempPtr;
	memset(txMsgPtr, 0, msgPtr->GetMessageSize());
	memcpy(txMsgPtr, msgPtr, msgPtr->GetMessageSize());
	UINT8* snd_payload = txMsgPtr->GetPayload();
	txMsgPtr = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(g_OMAC.radioName, txMsgPtr, size+sizeof(IEEE802_15_4_Header_t), eventTime);*/

	return DS_Success;
}


//DeviceStatus RadioControl::Receive(Message_15_4_t * msg, UINT16 size){
//		return DS_Success;
//}

/*
 *
 */
DeviceStatus RadioControl::Stop(){
//	DeviceStatus returnVal = DS_Success;
	DeviceStatus returnVal = CPU_Radio_Sleep(g_OMAC.radioName,0);
	if(returnVal == DS_Success){
		CPU_GPIO_SetPinState( RADIOCONTROL_STATEPIN, FALSE );
	}
	return returnVal;
}

/*
 *
 */
DeviceStatus RadioControl::StartPLL(){
	//return DS_Success;
	return StartRx();
	//DeviceStatus returnVal = CPU_Radio_TurnOnPLL(g_OMAC.radioName);
	//if(returnVal == DS_Success){
	//	CPU_GPIO_SetPinState( (GPIO_PIN) RADIO_STATEPIN, TRUE );
	//}
	//return returnVal;
}

/*
 *
 */
DeviceStatus RadioControl::StartRx(){
	//return StartPLL();
	DeviceStatus returnVal = CPU_Radio_TurnOnRx(g_OMAC.radioName);
	if(returnVal == DS_Success){
		CPU_GPIO_SetPinState( RADIOCONTROL_STATEPIN, TRUE );
	}
	return returnVal;
}


