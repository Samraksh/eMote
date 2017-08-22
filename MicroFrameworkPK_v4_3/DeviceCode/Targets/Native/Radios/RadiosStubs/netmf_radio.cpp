/*
 * 	Copyright The Samraksh Company
 *
 *	Name 		: 	netmf_radio.cpp
 *
 *  Author  	:   nived.sivads@samraksh.com, mukundan.sridharan@samrkash.com
 *
 *	Description :   Contains the interface functions between the radio pal (mac layers defined in Radio_decl.h) and
 *					the aggregation layer
 *
 */

// Called by MAC layer to register handler functions and start the radio
// Takes inputs of a pointer to eventhandler, radioid and numberRadios
// Returns the status of the initalization task whether successful or not

// History 		v0.1 - Added sleep functionality, channel change and power change functionality (nived.sivadas)
//              v0.2 - Added long range radio support (nived.sivadas)
//              v0.3 - Added asserts (MichaelAtSamraksh)
//				v0.4 - Added initial support for Si446x driver (Nathan Stohs)

#include <tinyhal.h>
#include <Samraksh\Radio_decl.h>
//#include "RF231\RF231.h"
//#include "SI4468\si446x.h"

// Calls the corresponding radio object initialize function based on the radio chosen
DeviceStatus CPU_Radio_Initialize(RadioEventHandler* eventHandlers, UINT8 radioName, UINT8 numberRadios, UINT8 macName )
{
	DeviceStatus status = DS_Fail;

	return status;
}


BOOL CPU_Radio_Reset(UINT8 radioName)
{
	BOOL result = FALSE;
	DeviceStatus status = DS_Fail;


	return result;
}


BOOL CPU_Radio_UnInitialize(UINT8 radioName)
{
	BOOL result = FALSE;
	DeviceStatus status = DS_Fail;

	return result;
}

// This function is no longer supported, it is upto the user to name the radio
UINT8 CPU_Radio_GetRadioIDs(UINT8* radioNames)
{

	return 1;
}

UINT16 CPU_Radio_GetAddress(UINT8 radioName)
{
	UINT16 address = 0;

	return address;
}


BOOL CPU_Radio_SetAddress(UINT8 radioName, UINT16 address)
{
	BOOL status = FALSE;

	return status;
}

INT8 CPU_Radio_GetRadioAckType()
{
	return 0;
}


INT8 CPU_Radio_GetRadioName()
{
	INT8 radioType = -1;

	return radioType;
}

DeviceStatus CPU_Radio_SetRadioName(INT8 radioName)
{
	DeviceStatus status = DS_Fail;


	return status;
}

DeviceStatus CPU_Radio_ChangeTxPower(UINT8 radioName, int power)
{
	DeviceStatus status = DS_Fail;

	return status;
}

UINT32 CPU_Radio_GetTxPower(UINT8 radioName)
{
	UINT32 txPower = 0;

	return txPower;
}

DeviceStatus CPU_Radio_ChangeChannel(UINT8 radioName, int channel)
{
	DeviceStatus status = DS_Fail;


	return status;
}

UINT32 CPU_Radio_GetChannel(UINT8 radioName)
{
	UINT32 channel = 0;


	return channel;
}

// Function is not currently supported
void* CPU_Radio_Preload(UINT8 radioName, void * msg, UINT16 size)
{

	return NULL;
}

void* CPU_Radio_SendRetry(UINT8 radioName)
{
	void* ptr_temp = NULL;

	return ptr_temp;
}

void* CPU_Radio_SendStrobe(UINT8 radioName, UINT16 size)
{
	void* ptr_temp = NULL;

	return ptr_temp;
}

void* CPU_Radio_Send(UINT8 radioName, void* msg, UINT16 size)
{
	void* ptr_temp = NULL;

	return ptr_temp;
}

// This function is used to send a time stamped packet, time stamping is done just before
// physical send in hardware
void* CPU_Radio_Send_TimeStamped(UINT8 radioName, void* msg, UINT16 size, UINT32 eventTime)
{
	void* ptr_temp = NULL;

	return ptr_temp;
}

NetOpStatus CPU_Radio_PreloadMessage(UINT8* msgBuffer, UINT16 size)
{
	NetOpStatus status = NetworkOperations_Success;

	return status;
}



// This function calls the corresponding radio turn on function based on the input radio id
DeviceStatus CPU_Radio_TurnOnRx(UINT8 radioName)
{
	DeviceStatus status = DS_Fail;

	return status;
}

// This function calls the corresponding radio turn off function based on the input radio id
DeviceStatus CPU_Radio_TurnOffRx(UINT8 radioName)
{
	DeviceStatus status = DS_Fail;

	return status;
}


// This function calls the corresponding radio turn on function based on the input radio id
DeviceStatus CPU_Radio_TurnOnPLL(UINT8 radioName)
{
	return DS_Success;

}


DeviceStatus CPU_Radio_Sleep(UINT8 radioName, UINT8 level)
{
	DeviceStatus status = DS_Fail;

	return status;

}

DeviceStatus CPU_Radio_ClearChannelAssesment (UINT8 radioName)
{
	DeviceStatus status  = DS_Fail;

	return status;
}


DeviceStatus CPU_Radio_ClearChannelAssesment(UINT8 radioName, UINT32 numberMicroSecond)
{
	DeviceStatus status  = DS_Fail;


	return status;

}

BOOL CPU_Radio_SetTimeStamp(UINT8 radioName)
{
	return FALSE;
}

BOOL CPU_Radio_SetTimeStamp(UINT64 timeStamp)
{
	return FALSE;
}

UINT32 CPU_Radio_GetSNR(UINT8 radioName)
{
	return 0;
}

// Gets the RSSI. Does not use the ED module of the RF231 currently.
// Must be called when in RX mode. Caller must do this.
// Return MAX_INT for total fail.
// Returns 0x7FFFFFFF if not in RX mode.
UINT32 CPU_Radio_GetRSSI(UINT8 radioName)
{
	UINT32 val;


	return val;
}


//Aggregate apis
BOOL CPU_RadioLayer_SwitchType(UINT8 radioName)
{
	return FALSE;
}


// At this point we are only supporting the RF231 radio, if this situation changes in future we can have
// a more sophisticated way of determining the number of radios supported, for now this is good
UINT8 CPU_RadioLayer_NumberRadiosSupported()
{
	return 2;
}
