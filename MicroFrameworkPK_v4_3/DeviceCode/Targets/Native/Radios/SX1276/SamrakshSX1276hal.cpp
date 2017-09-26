/*
 * SamrakshSX1276hal.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: Bora
 */

#include "SamrakshSX1276hal.h"
#include <Samraksh/VirtualTimer.h>
#include "SamrakshSX1276Parameters.h"


SamrakshRadio_I* gsx1276radio_ptr;

//extern SX1276_Semtech::SX1276M1BxASWrapper g_SX1276M1BxASWrapper;
extern Samraksh_SX1276Wrapper::SX1276M1BxASWrapper* g_SX1276M1BxASWrapper_ptr;

namespace EMOTE_SX1276_LORA {




//const Samraksh_SX1276_hal::InternalRadioProperties_t Samraksh_SX1276_hal::SX1276_hal_wrapper_internal_radio_properties(10, 10, 1000, 100, MODEM_LORA);

//Samraksh_SX1276_hal grfsx1276Radio;

void Samraksh_SX1276_hal::ValidHeaderDetected(){
	if(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.PacketDetected)
		static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.PacketDetected();
}
void Samraksh_SX1276_hal::TxDone(){
	static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_rm = SamrakshRadio_I::RadioMode_t::STANDBY;
	static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.ClearPaylod();
	if(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.TxDone)
		static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.TxDone(true);
}
void Samraksh_SX1276_hal::TxTimeout(){
	static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_rm = SamrakshRadio_I::RadioMode_t::STANDBY;
	static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.ClearPaylod();
	static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->ChooseRadioConfig();
	if(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.TxDone)
		static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.TxDone(false);
};
void Samraksh_SX1276_hal::RxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
	if(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.RxDone)
		static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.RxDone(payload, size);
}
void Samraksh_SX1276_hal::RxTimeout(){
	if(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.RxDone)
		static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.RxDone(NULL, 0);
}
void Samraksh_SX1276_hal::RxError(){
	if(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.RxDone)
		static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.RxDone(NULL, 0);
}
void Samraksh_SX1276_hal::FhssChangeChannel(uint8_t currentChannel ){

}
void Samraksh_SX1276_hal::CadDone(bool channelActivityDetected){
	static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_rm = SamrakshRadio_I::RadioMode_t::SLEEP;
	if(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.CadDone)
		static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_re.CadDone(channelActivityDetected);
}



void Samraksh_SX1276_hal::ChooseRadioConfig() {



	g_SX1276M1BxASWrapper_ptr->SetChannel( RF_FREQUENCY );

#if defined( USE_MODEM_LORA )

	g_SX1276M1BxASWrapper_ptr->SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    g_SX1276M1BxASWrapper_ptr->SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

#elif defined( USE_MODEM_FSK )

    g_SX1276M1BxASWrapper_ptr->SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                                  FSK_DATARATE, 0,
                                  FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                                  true, 0, 0, 0, 3000 );

    g_SX1276M1BxASWrapper_ptr->SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                                  0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                                  0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                                  0, 0,false, true );

#else
    #error "Please define a frequency band in the compiler options."
#endif

}

Samraksh_SX1276_hal::Samraksh_SX1276_hal()
: isRadioInitialized(false)
, m_rp() 	//TODO: BK: We need to initialize these parameters
, isCallbackIssued(false)
{

}


Samraksh_SX1276_hal::~Samraksh_SX1276_hal() {
}


DeviceStatus Samraksh_SX1276_hal::Initialize(SamrakshRadio_I::RadioEvents_t re){
	if(isRadioInitialized) return DS_Fail;



	isCallbackIssued = false;

	SX1276_hal_wrapper_internal_radio_properties.SetDefaults(10, 10, 1000, 1000, MODEM_LORA);

	sx1276_re.ValidHeaderDetected = Samraksh_SX1276_hal::ValidHeaderDetected;
	sx1276_re.TxDone = Samraksh_SX1276_hal::TxDone;
	sx1276_re.TxTimeout = Samraksh_SX1276_hal::TxTimeout;
	sx1276_re.RxDone = Samraksh_SX1276_hal::RxDone;
	sx1276_re.RxTimeout = Samraksh_SX1276_hal::RxTimeout;
	sx1276_re.RxError = Samraksh_SX1276_hal::RxError;
	sx1276_re.FhssChangeChannel = Samraksh_SX1276_hal::FhssChangeChannel;
	sx1276_re.CadDone = Samraksh_SX1276_hal::CadDone;


	m_re.CadDone = re.CadDone;
	m_re.DataStatusCallback = re.DataStatusCallback;
	m_re.PacketDetected = re.PacketDetected;
	m_re.RxDone = re.RxDone;
	m_re.TxDone = re.TxDone;


	SanityCheckOnConstants();

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(PacketLoadTimerName, 0, 1000, TRUE, FALSE, Samraksh_SX1276_hal::PacketLoadTimerHandler);
	rm = VirtTimer_SetTimer(PacketTxTimerName, 0, 1000, TRUE, FALSE, Samraksh_SX1276_hal::PacketTxTimerHandler);

	Samraksh_SX1276Wrapper::Samraksh_SX1276M1BxASWrapper_Init();
	g_SX1276M1BxASWrapper_ptr->Initialize(&sx1276_re);
	ChooseRadioConfig();

	isRadioInitialized = true;
	return DS_Success;
}
DeviceStatus Samraksh_SX1276_hal::UnInitialize(){
	bool t = isCallbackIssued;
	isCallbackIssued = false; Sleep(); while(!isCallbackIssued){} isCallbackIssued = t;
	m_re.CadDone = NULL;
	m_re.DataStatusCallback = NULL;
	m_re.PacketDetected = NULL;
	m_re.RxDone = NULL;
	m_re.TxDone = NULL;

//	m_re = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
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
	if(request_ack) { //Currently ACK request is not supported
		m_re.DataStatusCallback(false,size);
		return;
	}
	if(!IsPacketTransmittable(msg, size)) {
		 // BEGIN WORKAROUND

		        // Reset the radio
				g_SX1276M1BxASWrapper_ptr->Reset( );

		        // Calibrate Rx chain
//		        RxChainCalibration( );

		        // Initialize radio default values
				Sleep(); //SetOpMode( RF_OPMODE_SLEEP );

//		        RadioRegistersInit( );

//		        SetModem( MODEM_FSK );

		        // Restore previous network type setting.
//		        SetPublicNetwork( this->settings.LoRa.PublicNetwork );
		        // END WORKAROUND

//		        this->settings.State = RF_IDLE;
//		        if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->TxTimeout != NULL ) )
//		        {
//		            this->RadioEvents->TxTimeout( );
//		        }

				TxTimeout();
//		m_re.DataStatusCallback(false,size);
	}
	m_re.DataStatusCallback(true,size);
	m_packet.PreparePayload(msg, size, 0, 0);

	m_rm = SamrakshRadio_I::RadioMode_t::TX;


	g_SX1276M1BxASWrapper_ptr->Send(m_packet.GetPayload(), m_packet.GetSize());
}


void Samraksh_SX1276_hal::RequestSendAtTimeInstanst(void* msg, UINT16 size, TimeVariable_t PacketTransmissionTime, ClockIdentifier_t ClockIdentifier,  bool request_ack){ //TODO:
	//	m_re.DataStatusCallback(false, 0); //

	if(!IsPacketTransmittable(msg, size)) { //Reject if the incoming packet is not transferrable
		m_re.DataStatusCallback(false, size);
		return;
	}
	UINT64 curtime = CPU_Timer_CurrentTicks(ClockIdentifier);
	if(PacketTransmissionTime < curtime){
		m_re.DataStatusCallback(false, size);
		return;
	}
	UINT64 delay = CPU_TicksToMicroseconds(PacketTransmissionTime - curtime, ClockIdentifier);

	if(delay < SX1276_hal_wrapper_internal_radio_properties.TIME_ADVANCE_FOR_SCHEDULING_A_PACKET_MICRO){
		m_re.DataStatusCallback(false, size);
		return;
	}
	delay = delay - SX1276_hal_wrapper_internal_radio_properties.TIME_ADVANCE_FOR_SCHEDULING_A_PACKET_MICRO;


	bool rv = SetTimer(PacketLoadTimerName,0, delay, TRUE, low_precision_clock_id);
	if(!rv){
		m_re.DataStatusCallback(false, size);
		return;
	}
	rv = m_packet.PreparePayload(msg, size, PacketTransmissionTime, ClockIdentifier);
	if(!rv){
		m_re.DataStatusCallback(false, size);
		return;
	}
	m_re.DataStatusCallback(true, size);
	return;


}


void Samraksh_SX1276_hal::RequestCancelSend(){
	// Initializes the payload size
	g_SX1276M1BxASWrapper_ptr->Write( REG_LR_PAYLOADLENGTH, 0 );

	// Full buffer used for Tx
	g_SX1276M1BxASWrapper_ptr->Write( REG_LR_FIFOTXBASEADDR, 0 );
	g_SX1276M1BxASWrapper_ptr->Write( REG_LR_FIFOADDRPTR, 0 );

	preloadedMsgSize = 0;
	m_re.DataStatusCallback(true, 0);
}

//DeviceStatus Samraksh_SX1276_hal::AddToTxBuffer(void* msg, UINT16 size){
//	// FIFO operations can not take place in Sleep mode
//	if(size + preloadedMsgSize > SX1276_hal_wrapper_max_packetsize) return DS_Fail;
//
//	if( ( g_SX1276M1BxASWrapper_ptr->Read( REG_OPMODE ) & ~RF_OPMODE_MASK ) == RF_OPMODE_SLEEP )
//	{
//		g_SX1276M1BxASWrapper_ptr->Standby( );
//		return DS_Fail;
//	}
//	preloadedMsgSize += size;
//	g_SX1276M1BxASWrapper_ptr->WriteFifo(static_cast<uint8_t*>(msg),size);
//
////	g_SX1276M1BxASWrapper_ptr->SetOpMode( RFLR_OPMODE_SYNTHESIZER_TX );
//	g_SX1276M1BxASWrapper_ptr->Tx(g_SX1276M1BxASWrapper_ptr->TimeOnAir(SX1276_hal_wrapper_internal_radio_properties.radio_modem, m_packet.GetSize()));
//
//	return DS_Success;
//}


void Samraksh_SX1276_hal::ChannelActivityDetection(){
	m_rm = RX;
	g_SX1276M1BxASWrapper_ptr->StartCad();
}

void Samraksh_SX1276_hal::PacketLoadTimerHandler(void* param) {
	UINT64 delay;
	UINT64 curtime = CPU_Timer_CurrentTicks(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.GetClockId());
//	g_SX1276M1BxASWrapper_ptr->WriteFifo(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.GetPayload(),static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.GetSize());
	g_SX1276M1BxASWrapper_ptr->AddToTxBuffer(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.GetPayload(),static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.GetSize());
	static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.MarkUploaded();
	if(curtime >= static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.GetDueTime()){ //Failed to load and send correctly
//		gsx1276radio_ptr->m_re.DataStatusCallback(false, gsx1276radio_ptr->m_packet.GetSize());
//		return;
		delay = 10000;
	}
	else{
		delay = CPU_TicksToMicroseconds( static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.GetDueTime() - curtime, static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.GetClockId());
	}
	SetTimer(PacketTxTimerName, 0 , delay, TRUE, high_precision_clock_id ); //Schedule PacketTxTimerHandler
}

void Samraksh_SX1276_hal::PacketTxTimerHandler(void* param) {
	if(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.IsMsgUploaded()){
		static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_rm = SamrakshRadio_I::RadioMode_t::TX;
		g_SX1276M1BxASWrapper_ptr->Tx(10000+
				g_SX1276M1BxASWrapper_ptr->TimeOnAir(static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->SX1276_hal_wrapper_internal_radio_properties.radio_modem, static_cast<EMOTE_SX1276_LORA::Samraksh_SX1276_hal*>(gsx1276radio_ptr)->m_packet.GetSize())
				);
	}
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

	if(g_SX1276M1BxASWrapper_ptr->GetStatus() ==  RF_TX_RUNNING){
		return false;
	}
	if(m_packet.GetSize() > 0){ //Reject the request if there is a packet scheduled
		return false;
//		m_packet.ClearPaylod();
	}
	return (size <= SX1276_hal_wrapper_max_packetsize);
}




SamrakshRadio_I::RadioMode_t Samraksh_SX1276_hal::StartListenning(){
	if(m_packet.IsMsgUploaded()) return GetRadioState();
	g_SX1276M1BxASWrapper_ptr->Rx(0);
	m_rm = RX;
	return GetRadioState();
}

SamrakshRadio_I::RadioMode_t Samraksh_SX1276_hal::Sleep(){
	if(m_packet.IsMsgUploaded()) return GetRadioState();
	m_rm = SLEEP;
	g_SX1276M1BxASWrapper_ptr->Sleep();
	return GetRadioState();
}

SamrakshRadio_I::RadioMode_t Samraksh_SX1276_hal::Standby(){
	m_rm = STANDBY;
	g_SX1276M1BxASWrapper_ptr->Standby();
	return GetRadioState();
}

SamrakshRadio_I::RadioMode_t Samraksh_SX1276_hal::GetRadioState() {
	return m_rm;

//	Uninitialized,
//	SLEEP, 		//Low-power mode
//	STANDBY,	//both Crystal oscillator and Lora baseband blocks are turned on.RF part and PLLs are disabled
//	TX,			// TX
//	RX,
//	SamrakshRadio_I::RadioMode_t cur_radio_mode;
//	switch(g_SX1276M1BxASWrapper_ptr->GetStatus()){
//	case RF_IDLE :
//		cur_radio_mode = SamrakshRadio_I::RadioMode_t::STANDBY;
//		break;
//	case RF_RX_RUNNING :
//		cur_radio_mode = SamrakshRadio_I::RadioMode_t::RX;
//		break;
//	case RF_TX_RUNNING :
//		cur_radio_mode = SamrakshRadio_I::RadioMode_t::RX;
//		break;
//	case RF_CAD :
//		cur_radio_mode = SamrakshRadio_I::RadioMode_t::RX;
//		break;
//	default:
//		break;
//	}
//	return m_rm;
}


bool Samraksh_SX1276_hal::msgToBeTransmitted_t::PreparePayload(void* msg,
	UINT16 size, const UINT64& t, ClockIdentifier_t c) {
	if(IsMsgSaved()) return false;
	ClearPaylod();
	msg_size = size;
	msg_payload[0] = 0;
	memcpy(msg_payload+1, msg, msg_size ); //BK: one byte is reserved for addr transaction with SPI
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

UINT16 Samraksh_SX1276_hal::msgToBeTransmitted_t::GetSize() {
	return msg_size;
}

UINT8* Samraksh_SX1276_hal::msgToBeTransmitted_t::GetPayload() {
	return &(msg_payload[1]);
}

UINT64 Samraksh_SX1276_hal::msgToBeTransmitted_t::GetDueTime() {
	return due_time;
}

SamrakshRadio_I::ClockIdentifier_t Samraksh_SX1276_hal::msgToBeTransmitted_t::GetClockId() {
	return clock_id;
}


void Samraksh_SX1276_hal_Init() {
	static Samraksh_SX1276_hal gsx1276radio;
	gsx1276radio_ptr = &gsx1276radio;
}

} /* namespace Samraksh_SX1276 */


