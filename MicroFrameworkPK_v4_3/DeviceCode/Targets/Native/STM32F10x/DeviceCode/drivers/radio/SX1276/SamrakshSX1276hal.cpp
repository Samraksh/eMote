/*
 * SamrakshSX1276hal.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: Bora
 */

#include "SamrakshSX1276hal.h"

namespace EMOTE_SX1276_LORA {

const struct InternalRadioProperties_t Samraksh_SX1276_hal::SX1276_hal_wrapper_internal_radio_properties = {10, 10};



//Samraksh_SX1276_hal grfsx1276Radio;

void Samraksh_SX1276_hal::ValidHeaderDetected(){
	if(m_re.PacketDetected) m_re.PacketDetected();
}
void Samraksh_SX1276_hal::TxDone(){
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

//	VirtualTimerReturnMessage rm;
//	rm = VirtTimer_SetTimer(VIRT_TIMER_SX1276, 0, MILLISECINMICSEC, TRUE, FALSE, PublicSchedulerTaskHandler1, OMACClockSpecifier);

}


Samraksh_SX1276_hal::~Samraksh_SX1276_hal() {
}


DeviceStatus Samraksh_SX1276_hal::Initialize(RadioEvents_t re){
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
	if(size > SX1276_hal_wrapper_max_packetsize) {
		m_re.TxDone(false);
		return;
	}
	radio.Send(static_cast<uint8_t *>(msg), size);
}


void Samraksh_SX1276_hal::RequestSendAtTimeInstanst(void* msg, UINT16 size, TimeVariable_t PacketTransmissionTime, ClockIdentifier_t ClockIdentifier){
	m_re.DataStatusCallback(false, 0);

//	VirtualTimerReturnMessage rm;
//	AddToTxBuffer(msg,size);
//	rm = VirtTimer_Change(VIRT_TIMER_SX1276, 0, CPU_TicksToMicroseconds(PacketTransmissionTime - (CPU_Timer_CurrentTicks(ClockIdentifier)) , TRUE, ClockIdentifier); //1 sec Timer in micro seconds

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






SamrakshRadio_I::RadioMode_t Samraksh_SX1276_hal::StartListenning(){
	radio.Rx(0);
	m_rm = RX;
	return GetRadioState();
}

SamrakshRadio_I::RadioMode_t Samraksh_SX1276_hal::Sleep(){
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


} /* namespace Samraksh_SX1276 */
