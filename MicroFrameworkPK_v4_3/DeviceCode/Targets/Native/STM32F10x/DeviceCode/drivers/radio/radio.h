#ifndef RADIO_H
#define RADIO_H

/* === INCLUDES ============================================================ */
#include <Tinyhal_types.h>
#include <tinyhal.h>
#include "radio_error.h"
#include <..\pal\COM\Radio\MAC\Base\Radio_decl.h>

#define MAX_RADIOS_SUPPORTED 16
#define MAX_MACS_SUPPORTED 16

//////////////////////////// Default Event Handler Definitions //////////////////////////////////////////


extern "C"
{
	void* DefaultRecieveHandler(void *msg, UINT16 Size)
	{

	}

	BOOL DefaultSendAckHandler(void *msg, UINT16 Size)
	{

	}

	BOOL DefaultRadioInterruptHandler(RadioInterrupt Interrupt, void *param)
	{

	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


// Base class definition for the radio driver interface for all radio device implementations
template<class T>
class Radio{

	// Contains a unique id for each radio registered
	UINT8 radio_id_;

	// Responsible for generating unique ids, a single variable shared by multiple instances of radio
	static UINT8 unique_radio_id_ ;

	// Keeps track of all the mac ids that are currently supported
	static UINT8 MacIDs[MAX_MACS_SUPPORTED];
	static UINT8 MacIDIndex;

	RadioEventHandler defaultHandler;

	// Keeps a record of all the handlers registered by the MACs
	static RadioEventHandler* MacHandlers[MAX_MACS_SUPPORTED];

	// Type of radio (802.15.4 or 802.11 or ByteRadio or SoftwareDefined)
	RadioType radio_type_;

	// Is the radio active
	BOOL is_active_;

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
	DeviceStatus UnInitialize()
	{
		return DS_Fail;
	}

	// Defines the send interface recieves an empty pointer from the radio layer
	//virtual T* Send(T* msg, UINT16 size)
	void* Send(void* msg, UINT16 size)
	{
		return NULL;
	}

	// Preload the message on to the radio frame buffer/ RAM and dont send
	//virtual T* Preload(T* msg, UINT16 size)
	T* Preload(T* msg, UINT16 size)
	{
		return NULL;
	}

	// Initiate the process of send
	//virtual DeviceStatus Send()
	DeviceStatus Send()
	{
		return DS_Fail;
	}


	//virtual DeviceStatus ClearChannelAssesment()
	DeviceStatus ClearChannelAssesment()
	{
		return DS_Fail;
	}

	//virtual DeviceStatus ClearChannelAssesment(UINT32 numberMicroSecond)
	DeviceStatus ClearChannelAssesment(UINT32 numberMicroSecond)
	{
		return DS_Fail;
	}

	//virtual DeviceStatus SetTimeStamp()
	DeviceStatus SetTimeStamp()
	{
		return DS_Fail;
	}

	//virtual DeviceStatus SetTimeStamp(UINT64 timeStamp)
	DeviceStatus SetTimeStamp(UINT64 timeStamp)
	{
		return DS_Fail;
	}

	//virtual INT32 GetSNR()
	INT32 GetSNR()
	{
		return 0;
	}

	//virtual INT32 GetRSSI()
	INT32 GetRSSI()
	{
		return 0;
	}

	//virtual UINT8 GetRadioID()
	UINT8 GetRadioID()
	{
		return 0;
	}

	// Mutators for radio_type and is_active variables
	RadioType get_radio_type() const
	{
		return radio_type_;
	}

	void set_radio_type(RadioType radio_type)
	{
		radio_type_ = radio_type;
	}

	BOOL get_is_active() const
	{
		return is_active_;
	}

	void set_is_active(BOOL is_active)
	{
		is_active_ = is_active;
	}

	UINT8 GetMacIdIndex()
	{
		return MacIDIndex;
	}

};

template<class T>
UINT8 Radio<T>::MacIDIndex = 0;

template<class T>
UINT8 Radio<T>::MacIDs[MAX_MACS_SUPPORTED] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

template<class T>
RadioEventHandler* Radio<T>::MacHandlers[MAX_MACS_SUPPORTED] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
#endif /* RADIO_H */














