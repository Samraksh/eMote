/*
 * SamrakshSX1276hal.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: Bora
 */

#include "SamrakshSX1276hal.h"

namespace EMOTE_SX1276_LORA {

const struct InternalRadioProperties_t Samraksh_SX1276_hal::SX1276_hal_wrapper_internal_radio_properties = {10, 10, 1000, 100, MODEM_LORA};



//Samraksh_SX1276_hal grfsx1276Radio;

void Samraksh_SX1276_hal::ValidHeaderDetected(){
	if(m_re.PacketDetected) m_re.PacketDetected();
}
void Samraksh_SX1276_hal::TxDone(){
	m_packet.ClearPaylod();
	if(m_re.TxDone) m_re.TxDone(true);
}
void Samraksh_SX1276_hal::TxTimeout(){
	if(m_re.TxDone) m_re.TxDone(false);
};
void Samraksh_SX1276_hal::RxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
	if(m_re.RxDone) m_re.RxDone(payload, size);
}
void Samraksh_SX1276_hal::RxTimeout(){
	if(m_re.RxDone) m_re.RxDone(NULL, 0);
}
void Samraksh_SX1276_hal::RxError(){
	if(m_re.RxDone) m_re.RxDone(NULL, 0);
}
void Samraksh_SX1276_hal::FhssChangeChannel(uint8_t currentChannel ){

}
void Samraksh_SX1276_hal::CadDone(bool channelActivityDetected){
	m_rm = SLEEP;
	if(m_re.CadDone) m_re.CadDone(channelActivityDetected);
}



Samraksh_SX1276_hal::Samraksh_SX1276_hal()
: m_re()
,isRadioInitialized(false)
, radio(
		{
	ValidHeaderDetected,
	TxDone,
	TxTimeout,
	RxDone,
	RxTimeout,
	RxError,
	FhssChangeChannel,
	CadDone
		}
)
, m_rp() 	//TODO: BK: We need to initialize these parameters
, m_rm(Uninitialized)
, isCallbackIssued(false)
{
	SanityCheckOnConstants();

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(PacketLoadTimerName, 0, 1000, TRUE, FALSE, PacketLoadTimerHandler);
	rm = VirtTimer_SetTimer(PacketTxTimerName, 0, 1000, TRUE, FALSE, PacketTxTimerHandler);

}


Samraksh_SX1276_hal::~Samraksh_SX1276_hal() {
}


DeviceStatus Samraksh_SX1276_hal::Initialize(SamrakshRadio_I::RadioEvents_t re){
	if(isRadioInitialized) return DS_Fail;

	radio.RadioRegistersInit();
	radio.IoInit();
	radio.SpiInit();


	m_re = re;
	isRadioInitialized = true;
	return DS_Success;
}
DeviceStatus Samraksh_SX1276_hal::UnInitialize(){
	bool t = isCallbackIssued;
	isCallbackIssued = false; Sleep(); while(!isCallbackIssued){} isCallbackIssued = t;
	m_re = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	isRadioInitialized  = false;
	return DS_Success;
}
DeviceStatus Samraksh_SX1276_hal::IsInitialized(){
	if(isRadioInitialized) return DS_Success;
	return DS_Fail;

}

DeviceStatus Samraksh_SX1276_hal::SetAddress(){
	//BK: Currently refuse and return the current properties;
	return DS_Fail;
}

SamrakshRadio_I::RadioProperties_t Samraksh_SX1276_hal::GetRadioProperties(){
	return m_rp;
}

void Samraksh_SX1276_hal::Send(void* msg, UINT16 size, bool request_ack) {
	if(!IsPacketTransmittable(msg, size)) {
		m_re.DataStatusCallback(false);
		return;
	}
	m_re.DataStatusCallback(true);
	radio.Send(static_cast<uint8_t *>(msg), size);
}


void Samraksh_SX1276_hal::RequestSendAtTimeInstanst(void* msg, UINT16 size, TimeVariable_t PacketTransmissionTime, ClockIdentifier_t ClockIdentifier){ //TODO:
	//	m_re.DataStatusCallback(false, 0); //

	if(!IsPacketTransmittable(msg, size)) { //Reject if the incoming packet is not transferrable
		m_re.DataStatusCallback(false);
		return;
	}
	auto curtime = VirtTimer_GetTicks(ClockIdentifier);
	if(PacketTransmissionTime < curtime){
		m_re.DataStatusCallback(false);
		return;
	}
	UINT64 delay = VirtTimer_TicksToTime(ClockIdentifier, PacketTransmissionTime - curtime);

	if(delay < SX1276_hal_wrapper_internal_radio_properties.TIME_ADVANCE_FOR_SCHEDULING_A_PACKET_MICRO){
		m_re.DataStatusCallback(false);
		return;
	}
	delay = delay - SX1276_hal_wrapper_internal_radio_properties.TIME_ADVANCE_FOR_SCHEDULING_A_PACKET_MICRO;


	bool rv = SetTimer(PacketLoadTimerName,0, delay, TRUE, low_precision_clock_id);
	if(!rv){
		m_re.DataStatusCallback(false);
		return;
	}
	rv = m_packet.CopyPayload(msg, size, PacketTransmissionTime, ClockIdentifier);
	if(!rv){
		m_re.DataStatusCallback(false);
		return;
	}
	m_re.DataStatusCallback(true);
	return;


}


void Samraksh_SX1276_hal::RequestCancelSend(){
	// Initializes the payload size
	radio.Write( REG_LR_PAYLOADLENGTH, 0 );

	// Full buffer used for Tx
	radio.Write( REG_LR_FIFOTXBASEADDR, 0 );
	radio.Write( REG_LR_FIFOADDRPTR, 0 );

	preloadedMsgSize = 0;
	m_re.DataStatusCallback(true, 0);
}

DeviceStatus Samraksh_SX1276_hal::AddToTxBuffer(void* msg, UINT16 size){
	// FIFO operations can not take place in Sleep mode
	if(size + preloadedMsgSize > SX1276_hal_wrapper_max_packetsize) return DS_Fail;

	if( ( radio.Read( REG_OPMODE ) & ~RF_OPMODE_MASK ) == RF_OPMODE_SLEEP )
	{
		radio.Standby( );
		return DS_Fail;
	}
	preloadedMsgSize += size;
	radio.WriteFifo(static_cast<uint8_t*>(msg),size);

	radio.SetOpMode( RFLR_OPMODE_SYNTHESIZER_TX );
	return DS_Success;
}


void Samraksh_SX1276_hal::ChannelActivityDetection(){
	m_rm = RX;
	radio.StartCad();
}

void Samraksh_SX1276_hal::PacketLoadTimerHandler(void* param) {
	radio.WriteFifo(m_packet.msg_payload,m_packet.msg_size);
	m_packet.MarkUploaded();
	auto curtime = VirtTimer_GetTicks(m_packet.clock_id);
	UINT64 delay = VirtTimer_TicksToTime(m_packet.clock_id, m_packet.due_time - curtime);
	SetTimer(PacketLoadTimerName, 0 , delay, TRUE, high_precision_clock_id ); //Schedule PacketTxTimerHandler
}

void Samraksh_SX1276_hal::PacketTxTimerHandler(void* param) {
	if(m_packet.IsMsgUploaded()) radio.Tx(radio.TimeOnAir(SX1276_hal_wrapper_internal_radio_properties.radio_modem, m_packet.msg_size));
}

bool Samraksh_SX1276_hal::SetTimer(UINT8 timer_id, UINT32 start_delay,
		UINT32 period, BOOL is_one_shot, UINT8 hardwareTimerId) {
	VirtualTimerReturnMessage rm = TimerSupported;
	if(rm != TimerSupported) return false;  rm = VirtTimer_Stop(timer_id);
	if(rm != TimerSupported) return false;  rm = VirtTimer_Change(timer_id,start_delay,period, is_one_shot, hardwareTimerId);
	if(rm != TimerSupported) return false;  rm = VirtTimer_Start(timer_id);
	return true;
}

bool Samraksh_SX1276_hal::IsPacketTransmittable(void* msg, UINT16 size) {

	if(m_packet.msg_size > 0){ //Reject the request if there is a packet scheduled
		return false;
	}
	return (size <= SX1276_hal_wrapper_max_packetsize);
}




SamrakshRadio_I::RadioMode_t Samraksh_SX1276_hal::StartListenning(){
	if(m_packet.IsMsgUploaded()) return GetRadioState();
	radio.Rx(0);
	m_rm = RX;
	return GetRadioState();
}

SamrakshRadio_I::RadioMode_t Samraksh_SX1276_hal::Sleep(){
	if(m_packet.IsMsgUploaded()) return GetRadioState();
	m_rm = SLEEP;
	radio.Sleep();
	return GetRadioState();
}

SamrakshRadio_I::RadioMode_t Samraksh_SX1276_hal::Standby(){
	m_rm = STANDBY;
	radio.Standby();
	return GetRadioState();
}

SamrakshRadio_I::RadioMode_t Samraksh_SX1276_hal::GetRadioState() {
	return m_rm;
}


bool Samraksh_SX1276_hal::msgToBeTransmitted_t::PreparePayload(void* msg,
	UINT16 size, const UINT64& t, ClockIdentifier_t c) {
	if(IsMsgSaved()) return false;
	ClearPaylod();
	msg_size = size;
	memcpy(msg_payload, msg, msg_size );
	due_time = t;
	clock_id = c;
	return true;
}

Samraksh_SX1276_hal::msgToBeTransmitted_t::msgToBeTransmitted_t(){
	ClearPaylod();

}
void Samraksh_SX1276_hal::msgToBeTransmitted_t::ClearPaylod() {
	msg_size = 0;
	isUploaded = false;
	due_time = 0;

}

void Samraksh_SX1276_hal::msgToBeTransmitted_t::MarkUploaded() {
	isUploaded = true;
}

bool Samraksh_SX1276_hal::msgToBeTransmitted_t::IsMsgSaved() {
	if(msg_size > 0 ) return true;
	return false;
}

bool Samraksh_SX1276_hal::msgToBeTransmitted_t::IsMsgUploaded() {
	return isUploaded;
}


} /* namespace Samraksh_SX1276 */


