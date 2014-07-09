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

#include "RF231\RF231.h"

// Calls the corresponding radio object initialize function based on the radio chosen
DeviceStatus CPU_Radio_Initialize(RadioEventHandler* eventHandlers, UINT8 radioID, UINT8 numberRadios, UINT8 mac_id )
{

	DeviceStatus status = DS_Fail;

	if(eventHandlers == NULL)
		return DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.Initialize(eventHandlers, radioID, mac_id);
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.Initialize(eventHandlers, radioID, mac_id);
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_RADIO_Initialize : Unidentified radio \n");
			break;
	}

	return status;

}


BOOL CPU_Radio_Reset(UINT8 id)
{
	return FALSE;
}


BOOL CPU_Radio_UnInitialize(UINT8 id)
{
	BOOL result = FALSE;

	switch(id)
	{
		case RF231RADIO:
			result = grf231Radio.UnInitialize();
			break;
		case RF231RADIOLR:
			result = grf231RadioLR.UnInitialize();
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_RADIO_UnInitialize : Unidentified radio \n");
			break;

	}

	return result;
}

// This function is no longer supported, it is upto the user to name the radio
UINT8 CPU_Radio_GetRadioIDs(UINT8* radioIDs)
{
	//*radioIDs=grf231Radio.GetRadioID();
	return 1;
}

UINT16 CPU_Radio_GetAddress(UINT8 radioID){

	UINT16 address = 0;

	switch(radioID)
	{
	case RF231RADIO:
		address = grf231Radio.GetAddress();
		break;
	case RF231RADIOLR:
		address = grf231RadioLR.GetAddress();
		break;
	default:
		hal_printf("[NATIVE] Error in function CPU_Radio_GetAddress : Unidentified radio \n");
		break;
	}

	return address;
}


BOOL CPU_Radio_SetAddress(UINT8 radioID, UINT16 address){

	BOOL status = FALSE;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.SetAddress(address);
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.SetAddress(address);
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_SetAddress : Unidentified radio \n");
			break;
	}

	return status;
}

// Function is not currently supported
void* CPU_Radio_Preload(UINT8 radioID, void * msg, UINT16 size)
{
	return NULL;
}

void* CPU_Radio_Send(UINT8 radioID, void * msg, UINT16 size)
{

	void *temptr = NULL;

	switch(radioID)
	{
		case RF231RADIO:
			return (void *) grf231Radio.Send(msg, size);
		case RF231RADIOLR:
			return (void *) grf231RadioLR.Send(msg, size);
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_Send : Unidentified radio \n");
			break;
	}

	// Can only be here if you tried to access a radio that does not exist
	return temptr;
}

// This function is used to send a time stamped packet, time stamping is done just before
// physical send in hardware
void* CPU_Radio_Send_TimeStamped(UINT8 radioID, void * msg, UINT16 size, UINT32 eventTime)
{

	switch(radioID)
	{
		case RF231RADIO:
			return (void *) grf231Radio.Send_TimeStamped(msg, size, eventTime);
		case RF231RADIOLR:
			return (void *) grf231RadioLR.Send_TimeStamped(msg, size, eventTime);
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_Send_TimeStamped : Unidentified radio \n");
			break;
	}

	// Can only be here if you tried to access a radio that does not exist
	return NULL;
}

// This function is used in combination with pre loading and is currently not supported
DeviceStatus CPU_Radio_Send(UINT8 radioID)
{
	return DS_Fail;
}

// This function calls the corresponding radio turn on function based on the input radio id
DeviceStatus CPU_Radio_TurnOn(UINT8 radioID)
{

	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.TurnOn();
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.TurnOn();
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_TurnOn : Unidentified radio \n");
			break;

	}
	return status;
}


DeviceStatus CPU_Radio_Sleep(UINT8 radioID, UINT8 level)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.Sleep(level);
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.Sleep(level);
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_Sleep : Unidentified radio \n");
			break;
	}

	return status;

}

DeviceStatus CPU_Radio_ChangeTxPower(UINT8 radioID, int power)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.ChangeTxPower(power);
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.ChangeTxPower(power);
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_ChangeTxPower : Unidentified radio \n");
			break;
	}

	return status;
}

DeviceStatus CPU_Radio_ChangeChannel(UINT8 radioID, int channel)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.ChangeChannel(channel);
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.ChangeChannel(channel);
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_ChangeTxPower : Unidentified radio \n");
			break;
	}

	return status;

}

DeviceStatus CPU_Radio_ClearChannelAssesment (UINT8 radioID)
{
	DeviceStatus status  = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.ClearChannelAssesment();
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.ClearChannelAssesment();
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_ClearChannelAssesment : Unidentified radio \n");
			break;
	}

	return status;
}


DeviceStatus CPU_Radio_ClearChannelAssesment2(UINT8 radioID, UINT32 numberMicroSecond)
{
	DeviceStatus status  = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.ClearChannelAssesment(numberMicroSecond);
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.ClearChannelAssesment(numberMicroSecond);
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_ClearChannelAssesment2 : Unidentified radio \n");
			break;
	}

	return status;

}

UINT32 CPU_Radio_GetChannel(UINT8 radioID)
{
	UINT32 channel = 0;

	switch(radioID)
	{
		case RF231RADIO:
			channel = grf231Radio.GetChannel();
			break;
		case RF231RADIOLR:
			channel = grf231RadioLR.GetChannel();
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_GetChannel : Unidentified radio \n");
			break;
	}

	return channel;
}

UINT32 CPU_Radio_GetTxPower(UINT8 radioID)
{
	UINT32 txPower = 0;

	switch(radioID)
	{
		case RF231RADIO:
			txPower = grf231Radio.GetTxPower();
			break;
		case RF231RADIOLR:
			txPower = grf231RadioLR.GetTxPower();
			break;
		default:
			hal_printf("[NATIVE] Error in function CPU_Radio_GetTxPower : Unidentified radio \n");
			break;
	}

	return txPower;
}

BOOL CPU_Radio_SetTimeStamp(UINT8 radioID)
{
	return FALSE;
}

BOOL CPU_Radio_SetTimeStamp(UINT64 timeStamp)
{
	return FALSE;
}

UINT32 CPU_Radio_GetSNR(UINT8 radioID)
{
	return 0;
}


UINT32 CPU_Radio_GetRSSI(UINT8 radioID)
{
	return 0;
}


//Aggregate apis
BOOL CPU_RadioLayer_SwitchType(UINT8 radioID)
{
	return FALSE;
}

#if 0
Radio* CPU_RadioLayer_GetRadio(UINT8 radioID)
{
}
#endif

// At this point we are only supporting the RF231 radio, if this situation changes in future we can have
// a more sophisticated way of determining the number of radios supported, for now this is good
UINT8 CPU_RadioLayer_NumberRadiosSupported()
{
	return 2;
}
