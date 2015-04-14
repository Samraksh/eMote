#ifndef RADIO_H
#define RADIO_H

/* === INCLUDES ============================================================ */
#include <Tinyhal_types.h>
#include <tinyhal.h>
#include "Radio_decl.h"

//////////////////////////// Radio Errors/////////////////////////////////////////

#define RADIOERROR01  "Frame Buffer Overrun"
#define RADIOERROR02  "Battery Low"
#define RADIOERROR03  "State Change Failed"

//////////////////////////////////////////////////////////////////////////////////////

#define MAX_RADIOS_SUPPORTED 4
#define MAX_MACS_SUPPORTED 4

//////////////////////////// Default Event Handler Definitions //////////////////////////////////////////


extern "C"
{
	void* DefaultRecieveHandler(void *msg, UINT16 Size)
	{

	}

	void DefaultSendAckHandler(void *msg, UINT16 Size, NetOpStatus status)
	{

	}

	BOOL DefaultRadioInterruptHandler(RadioInterrupt Interrupt, void *param)
	{

	}
}


// The radio id being a static in the radio class has been removed

#if 0
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//All Radio objects needs to share the ID class to get a unique number
class RadioID {
private:
	static UINT8 UniqueRadioId;
public:
	static UINT8 GetUniqueRadioId(){
		return ++UniqueRadioId;
	}
};

UINT8 RadioID::UniqueRadioId=0;
#endif

// Base class definition for the radio driver interface for all radio device implementations
template<class T>
class Radio
{
	// Contains a unique id for each radio registered

	UINT8 RadioId;
	RadioAddress_t MyAddress;

	// Keeps track of all the mac ids that are currently supported
	static UINT8 MacIDs[MAX_MACS_SUPPORTED];
	static UINT8 MacIDIndex;

	RadioEventHandler defaultHandler;

	// Keeps a record of all the handlers registered by the MACs
	static RadioEventHandler* MacHandlers[MAX_MACS_SUPPORTED];

	// Type of radio (802.15.4 or 802.11 or ByteRadio or SoftwareDefined)
	RadioType Radio_Type;

	// Is the radio active
	BOOL Initialized;

public:
	// Set the mac handlers
	BOOL SetMacHandlers(RadioEventHandler* handler)
	{
		if(handler == NULL)
			return FALSE;

		MacHandlers[MacIDIndex++] = handler;
		return TRUE;
	}

	RadioEventHandler* GetMacHandler(UINT8 MacIndex)
	{
		return MacHandlers[MacIndex];
	}

	BOOL SetDefaultHandlers()
	{
		defaultHandler.SetRecieveHandler(DefaultRecieveHandler);
		defaultHandler.SetSendAckHandler(DefaultSendAckHandler);
		defaultHandler.SetRadioInterruptHandler(DefaultRadioInterruptHandler);
		MacHandlers[MacIDIndex] = &defaultHandler;

	}
	UINT16 GetAddress(){
		return (UINT16) MyAddress;
	}

	BOOL SetAddress(UINT16 address){
		MyAddress = (RadioAddress_t)address;
		return TRUE;
	}

	//virtual DeviceStatus Initialize(RadioEventHandler *event_handler, UINT8 mac_id)=0;

	// Responsible for initializing the radio
	//virtual DeviceStatus Initialize(RadioEventHandler *event_handler, UINT8 mac_id)
	DeviceStatus Initialize(RadioEventHandler *event_handler, UINT8 mac_id)
	{

		Radio<T>::MacIDs[MacIDIndex] = mac_id;
		if(!event_handler)
		{
			SetDefaultHandlers();
		}
		else
		{
			MacHandlers[MacIDIndex] = event_handler;

		}

		// Increment the mac id index
		MacIDIndex++;

		return DS_Success;

	}

	// Responsible for uninitializing the radio
	//virtual DeviceStatus UnInitialize()
	DeviceStatus UnInitialize();


	// Defines the send interface recieves an empty pointer from the radio layer
	//virtual T* Send(T* msg, UINT16 size)
	void* Send(void* msg, UINT16 size);



	// Defines the send interface recieves an empty pointer from the radio layer
	//virtual T* Send(T* msg, UINT16 size)
	void* Send_TimeStamped(void* msg, UINT16 size, UINT32 eventTime);


	// Preload the message on to the radio frame buffer/ RAM and dont send
	//virtual T* Preload(T* msg, UINT16 size)
	T* Preload(T* msg, UINT16 size);

	//virtual DeviceStatus ClearChannelAssesment()
	DeviceStatus ClearChannelAssesment();


	//virtual DeviceStatus ClearChannelAssesment(UINT32 numberMicroSecond)
	DeviceStatus ClearChannelAssesment(UINT32 numberMicroSecond);


	//virtual DeviceStatus SetTimeStamp()
	DeviceStatus SetTimeStamp();


	//virtual DeviceStatus SetTimeStamp(UINT64 timeStamp)
	DeviceStatus SetTimeStamp(UINT64 timeStamp);


	//virtual INT32 GetSNR()
	INT32 GetSNR();


	//virtual INT32 GetRSSI()
	INT32 GetRSSI();


	//virtual UINT8 GetRadioID()
	UINT8 GetRadioID(){
		return RadioId;
	}

	void SetRadioID(UINT8 id)
	{
		RadioId=id;
	}

	// Mutators for radio_type and is_active variables
	RadioType GetRadioType() const
	{
		return Radio_Type;
	}

	void SetRadioType(RadioType radio_type)
	{
		Radio_Type = radio_type;
	}

	BOOL IsInitialized() const
	{
		return Initialized;
	}

	void SetInitialized(BOOL initialize)
	{
		Initialized = initialize;
	}

	UINT8 GetMacIdIndex()
	{
		return MacIDIndex;
	}

};

template<class T>
UINT8 Radio<T>::MacIDIndex = 0;

template<class T>
UINT8 Radio<T>::MacIDs[MAX_MACS_SUPPORTED] = {0,0,0,0};

template<class T>
RadioEventHandler* Radio<T>::MacHandlers[MAX_MACS_SUPPORTED] = {NULL,NULL,NULL,NULL};
#endif /* RADIO_H */













