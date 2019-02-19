/*
 * SamrakshSX1276hal.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: Bora
 */

#include "SamrakshSX1276halShim.h"
#include <Samraksh/VirtualTimer.h>
#include "SamrakshSX1276Parameters.h"

extern EMOTE_SX1276_LORA::Samraksh_SX1276_hal gsx1276radio;
EMOTE_SX1276_LORA::Samraksh_SX1276_hal_netmfadapter gsx1276radio_netmf_adapter;


namespace EMOTE_SX1276_LORA {

DeviceStatus Samraksh_SX1276_hal_netmfadapter::CPU_Radio_Initialize(RadioEventHandler* event_handler){
	Radio_event_handler.SetReceiveHandler(event_handler->GetReceiveHandler());
	Radio_event_handler.SetSendAckHandler(event_handler->GetSendAckHandler());
	Radio_event_handler.SetRadioInterruptHandler(event_handler->GetRadioInterruptHandler());

	radio_events.TxDone = TxDone;
	radio_events.PacketDetected = PacketDetected;
	radio_events.RxDone = RxDone;
	radio_events.CadDone = CadDone;
	radio_events.DataStatusCallback = DataStatusCallback;

	//Samraksh_SX1276_hal* base_ptr = this;
	DeviceStatus ds = gsx1276radio.Initialize(radio_events);
	//DeviceStatus ds = gsx1276radio.Initialize(radio_events);
	return ds;
}

void Samraksh_SX1276_hal_netmfadapter::TxDone (bool success){
	NetOpStatus ns;
	UINT8 radioAckStatus;
	if(success) {
		ns = NetworkOperations_Success;
		radioAckStatus = NetworkOperations_Success;
	}
	else {
		ns = NetworkOperations_Fail;
		radioAckStatus = NetworkOperations_Success;
	}
	(gsx1276radio_netmf_adapter.Radio_event_handler.GetSendAckHandler())(static_cast<void*>(gsx1276radio.m_packet.GetPayload()), gsx1276radio.m_packet.GetSize(), ns, radioAckStatus);
}

void  Samraksh_SX1276_hal_netmfadapter::PacketDetected( void ){
	void* dummy_ptr=NULL;
	gsx1276radio_netmf_adapter.Radio_event_handler.RadioInterruptHandler(StartOfReception, dummy_ptr);
}

/*!
 * @brief Rx Done callback prototype. In case of rx error payload is null and the size is zero
 *
 * @param [IN] payload Received buffer pointer
 * @param [IN] size    Received buffer size
 */
void    Samraksh_SX1276_hal_netmfadapter::RxDone( uint8_t *payload, uint16_t size ){
	(gsx1276radio_netmf_adapter.Radio_event_handler.GetReceiveHandler())(payload, size);
}

/*!
 * @brief CAD Done callback prototype.
 *
 * @param [IN] channelDetected    Whether Channel Activity detected during the CAD
 */
void Samraksh_SX1276_hal_netmfadapter::CadDone( bool channelActivityDetected ){

}

DeviceStatus Samraksh_SX1276_hal_netmfadapter::CPU_Radio_ClearChannelAssesment(){
	return DS_Success;
}

/*!
 * @brief Callback prototype for sending. This is generated when the data gets accepted to be sent
 *
 * @param [IN] success	Wheter
 * @param [IN] number_of_bytes_in_buffer
 *
 */
void Samraksh_SX1276_hal_netmfadapter::DataStatusCallback ( bool success, UINT16 number_of_bytes_in_buffer ){
	gsx1276radio_netmf_adapter.DataStatusCallback_success = success;
	gsx1276radio_netmf_adapter.DataStatusCallback_number_of_bytes_in_buffer = number_of_bytes_in_buffer;
}

void* Samraksh_SX1276_hal_netmfadapter::Send(void* msg, UINT16 size){
	gsx1276radio_netmf_adapter.DataStatusCallback_success = false;
	gsx1276radio.Send(msg,size,true,true);
	if(gsx1276radio_netmf_adapter.DataStatusCallback_success){
		return gsx1276radio.m_packet.GetPayload();
	}
		return NULL;
}

void* Samraksh_SX1276_hal_netmfadapter::SendTS(void* msg, UINT16 size, UINT32 eventTime){
	gsx1276radio_netmf_adapter.DataStatusCallback_success = false;
	gsx1276radio.SendTS(msg,size,true,true, eventTime);
	if(gsx1276radio_netmf_adapter.DataStatusCallback_success){
		return gsx1276radio.m_packet.GetPayload();
	}
	return NULL;
}

DeviceStatus Samraksh_SX1276_hal_netmfadapter::TurnOnRx(){
	if(gsx1276radio.Sleep() == SamrakshRadio_I::SLEEP)
		return DS_Success;
	else return DS_Fail;
}
DeviceStatus Samraksh_SX1276_hal_netmfadapter::Sleep(){
	if(gsx1276radio.Sleep() == SamrakshRadio_I::SLEEP)
		return DS_Success;
	else return DS_Fail;
}

} /* namespace Samraksh_SX1276 */
