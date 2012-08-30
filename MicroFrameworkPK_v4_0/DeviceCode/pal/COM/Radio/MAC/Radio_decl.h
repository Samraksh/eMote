/*
 * Copyright The Samraksh Company
 *
 * Author: Mukundan.Sridharan, Nived.Sivadas
 *
 * Description :  Radio HAL APIs, v 1.0
 *
 *
*/

#ifndef _RADIO_DECL_H_
#define _RADIO_DECL_H_

#include <tinyhal.h>
#include <Hal_uitl.h>

////////////////////////////////////// Macro Definitions  //////////////////////////////////////////////////

//////////////////////////////////// Interrupt mask macro definitions //////////////////////////////////////

// Used by the MAC layer to register the appropriate interrupts
#define PD 		1
#define SFD 	(1 << 1)
#define STX 	(1 << 2)
#define SRX 	(1 << 4)
#define CRC_E 	(1 << 6)
#define ENCRYPT_E (1 << 7)
#define DECRYPT_E (1 << 8)
#define INIT_E  (1 << 9)
#define SYNC_E (1 << 10)

///////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////Enum Definitions /////////////////////////////////////////////////////

// Represents a generic group of interrupts supported by any radio. There is a good chance some of these are not relevant to a radio,
// its is upto to the radio layer to determine this and inform the user appropriately

enum RadioInterrupt
{
	PreambleDetect,
	StartFrameDetect,
	StartOfTransmission,
	EndOfTransmission,
	StartofReception,
	EndofReception,
	CRCError,
	EncryptionError,
	DecryptionError,
	InitializationError,
	SynchronizationError,
};

// Defines the type of the radio that is being configured
enum RadioType{
	IEEE802_15_4,
	IEEE802_11,
	IEEE802_15_1,
	ByteRadio,
	SoftwareDefined,
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////Typedef Definitions //////////////////////////////////////////////////

typedef UINT8 ErrorType;

// Typedef defining the signature of the receive function
typedef void* (*ReceiveFuncPtrType) (void *msg, int Size);

// Typedef defining the signature of the send function
typedef bool (*SendAckFuncPtrType) (void* msg, int Size);

// Typedef defining the signature of the RadioInterruptFuncPtr function
typedef bool (*RadioInterruptFuncPtrType) (RadioInterrupt Interrupt, void *param);



////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0
class CCA
{
public:
	// Request cca to the radio
	static ErrorType request();
};
#endif

//////////////////////////////// Class Definitions //////////////////////////////////////////////////////
class RadioEventHandler{

	UINT32 radio_interrupt_mask_;
public:
	ReceiveFuncPtrType recieve_handler_;
	SendAckFuncPtrType send_ack_handler_;
	RadioInterruptFuncPtrType radio_interrupt_handler_;

	RadioEventHandler(ReceiveFuncPtrType recieve_handler, SendAckFuncPtrType send_ack_handler,RadioInterruptFuncPtrType  radio_interrupt_handler, UINT32 radio_interrupt_mask )
	{
		this->recieve_handler_ = recieve_handler;
		this->send_ack_handler_ = send_ack_handler;
		this->radio_interrupt_handler_ = radio_interrupt_handler;
		this->radio_interrupt_mask_ = radio_interrupt_mask;
	}

	void set_recieve_handler(ReceiveFuncPtrType recieve_handler)
	{
		this->recieve_handler_ = recieve_handler;
	}

	void set_send_ack_handler(SendAckFuncPtrType send_ack_handler)
	{
		this->send_ack_handler_ = send_ack_handler;
	}

	void set_radio_interrupt_handler(RadioInterruptFuncPtrType radio_interrupt_handler)
	{
		this->radio_interrupt_handler_ = radio_interrupt_handler;
	}

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0
// This structure defines an interface for radio events
struct IRadioEvents
{
	///////////////////////////////////////////////
	// Description : called in the event of end of transmission interrupt from the radio
	// 				 driver layer
	///////////////////////////////////////////////
	BOOL (*ReceiveHandler)(void *data);

	///////////////////////////////////////////////
	// Description : Called on an efd event from the radio driver layer
	///////////////////////////////////////////////
	BOOL (*EFDEvent)(void *data);

	///////////////////////////////////////////////
	// Description : Called on completion of cca
	//////////////////////////////////////////////
	BOOL (*ccaDoneEvent) (void *data);

	///////////////////////////////////////////////
	// Description : Called on completion of radio send
	//////////////////////////////////////////////
	BOOL (*radioSendDoneEvent)(void *data);

};

// Default handler
BOOL Default_Handler(void *data)
{
	return TRUE;
}

struct RadioEventHandler
{
public:
	IRadioEvents *m_RadioEvents;

	// Temporarily passing null to the event handlers
	BOOL SFDEvent(){
		if(this->m_RadioEvents->SFDEvent == NULL)
		{
			return Default_Handler(NULL);
		}

		return this->m_RadioEvents->SFDEvent(NULL);
	}

	BOOL EFDEvent(){
		if(this->m_RadioEvents->EFDEvent == NULL)
			return Default_Handler(NULL);

		return this->m_RadioEvents->EFDEvent(NULL);
	}

	BOOL CCADoneEvent(){
		if(this->m_RadioEvents->ccaDoneEvent == NULL)
			return Default_Handler(NULL);

		return this->m_RadioEvents->ccaDoneEvent(NULL);
	}

	BOOL RadioSendDoneEvent(){
		if(this->m_RadioEvents->radioSendDoneEvent == NULL)
			return Default_Handler(NULL);

		return this->m_RadioEvents->radioSendDoneEvent(NULL);
	}
};
#endif

// Contains the PAL interface functions used defined in the HAL aggregation layer
extern "C"
{
//Radio HAL declarations

// Called by MAC layers responsible for registering of eventhandlers
DeviceStatus CPU_Radio_Initialize(RadioEventHandler_t* eventHandlers, uint8* radioIDs, uint8 numberRadios ); //Initializes Return the ID of the Radio layer that was initialized
bool CPU_Radio_UnInitialize();
uint8 CPU_Radio_GetRadioIDs(uint8* radioIDs);
void* CPU_Radio_Preload(uint8 radioID,void * msg, uint16 size);
void* CPU_Radio_Send(uint8 radioID,void * msg, uint16 size);
DeviceError CPU_Radio_Send(uint8 radioID);	//Send preloaded message
DeviceError CPU_Radio_ClearChannelAssesment (uint8 radioID);
bool CPU_Radio_ClearChannelAssesment(uint8 radioID, uint32 numberMicroSecond);
bool CPU_Radio_SetTimeStamp(uint8 radioID);
bool CPU_Radio_SetTimeStamp(uint64 timeStamp);
int32 CPU_Radio_GetSNR(uint8 radioID);
int32 CPU_Radio_GetRSSI(uint8 radioID);

//Aggregate apis
bool CPU_Radio_SwitchType(uint8 radioID);
Radio* CPU_Radio_GetRadio(uint8 radioID);
bool CPU_Radio_Intialize(uint8  radioID);
bool CPU_Radio_UnInitialize(Radio* radioID);
uint8 CPU_Radio_NumberRadiosSupported();

}



#if 0
// Enables timestamping of radio packets, identical to the timestamping interface suggested by TEP132 in tinyos and closely follows
// their rf230 implementation
template<class T>
class TimeStampLayer
{


public:
	static BOOL isValid(T *msg)
	{
		return TRUE;
	}

	static UINT32 timestamp(T *msg)
	{
		return 0;
	}

	static void clear(T *msg)
	{

	}

	static void set(T *msg, UINT32 value)
	{

	}

	static void set(T *msg)
	{

	}

};
#endif

#endif
