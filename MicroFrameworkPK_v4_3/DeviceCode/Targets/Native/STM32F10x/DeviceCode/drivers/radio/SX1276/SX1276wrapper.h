/*
 * SX1276wrapper.h
 *
 *  Created on: Aug 10, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_SX1276WRAPPER_H_
#define MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_SX1276WRAPPER_H_

namespace EMOTE_SX1276_LORA{

#include <Samraksh/Radio.h>
#include "sx1276driver/sx1276/sx1276-hal.h"
#include "EmoteLoraHat.h"







/*!
 * @brief SX1276_hal_wrapper: 	The class that implements the SamrakshRadio_I interface
								which is used by the above layers within Emote like OMAC
 */
class SX1276_hal_wrapper : public  SamrakshRadio_I, LoraHat::LoraHardwareConfig{
public: //Public class definitions


private: //SX1276_s internal callbacks as defined in the driver
	static void ValidHeaderDetected();
	static void TxDone();
	static void TxTimeout();
	static void RxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
	static void RxTimeout();
	static void RxError();
	static void FhssChangeChannel(uint8_t currentChannel );
	static void CadDone(bool channelActivityDetected);

private:
	static struct InternalRadioProperties_t {
		UINT64 RADIO_BUS_SPEED; //Number of bytes written in a second to the radio buffer
		UINT16 RADIO_TURN_ON_DELAY_TX_MICRO;	//Tx initiation delay
	};

	static const InternalRadioProperties_t SX1276_hal_wrapper_internal_radio_properties;
	static const UINT16 SX1276_hal_wrapper_max_packetsize = 255;
private:
	SamrakshRadio_I::RadioEvents_t m_re;
	bool isCallbackIssued;
	RadioMode_t m_rm;

	UINT16 preloadedMsgSize;
	SX1276MB1xAS radio;

	RadioProperties_t m_rp;
	bool isRadioInitialized;

	UINT8 msg[SX1276_hal_wrapper_max_packetsize];

	bool SanityCheckOnConstants(){
		//if (SX1276_hal_wrapper_max_packetsize > std::numeric_limits<uint8_t>::max()) return false;
		if (SX1276_hal_wrapper_max_packetsize > 256) return false;

		return true;
	}

	DeviceStatus AddToTxBuffer(void* msg, UINT16 size);
public:
	SX1276_hal_wrapper();
	virtual ~SX1276_hal_wrapper();

	DeviceStatus Initialize(RadioEvents_t re);
	DeviceStatus UnInitialize();
	DeviceStatus IsInitialized();

	DeviceStatus SetAddress();
	RadioProperties_t GetRadioProperties();

	void Send(void* msg, UINT16 size, bool request_ack = false);
	void RequestSendAtTimeInstanst(void* msg, UINT16 size, TimeVariable_t PacketTransmissionTime, ClockIdentifier_t ClockIdentifier);
	void RequestCancelSend();

	void ChannelActivityDetection();

	RadioMode_t StartListenning();
	RadioMode_t Sleep();
	RadioMode_t Standby();
	RadioMode_t GetRadioState();

};


}
#endif /* MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_SX1276WRAPPER_H_ */
