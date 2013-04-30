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

#include "RF231\RF231.h"

// Currently supports only one radio
DeviceStatus CPU_Radio_Initialize(RadioEventHandler* eventHandlers, UINT8* radioID, UINT8 numberRadios, UINT8 mac_id )
{
	if(eventHandlers == NULL)
		return DS_Fail;

	return grf231Radio.Initialize(eventHandlers, radioID, mac_id);

}

BOOL CPU_Radio_Reset(UINT8 id)
{
	return grf231Radio.Reset();
}


BOOL CPU_Radio_UnInitialize(UINT8 id)
{
	if(id == RF231RADIO)
		return grf231Radio.UnInitialize();
}

UINT8 CPU_Radio_GetRadioIDs(UINT8* radioIDs)
{
	*radioIDs=grf231Radio.GetRadioID();
	return 1;
}

UINT16 CPU_Radio_GetAddress(UINT8 radioID){
	return grf231Radio.GetAddress();
}
BOOL CPU_Radio_SetAddress(UINT8 radioID, UINT16 address){
	return grf231Radio.SetAddress(address);
}

void* CPU_Radio_Preload(UINT8 radioID, void * msg, UINT16 size)
{
	return NULL;
}

void* CPU_Radio_Send(UINT8 radioID, void * msg, UINT16 size)
{

	UINT8* msgcheck = (UINT8 *) msg;

	if(radioID == RF231RADIO)
		return (void *) grf231Radio.Send(msg, size);
	else
		return NULL;
}

void* CPU_Radio_Send_TimeStamped(UINT8 radioID, void * msg, UINT16 size, UINT32 eventTime)
{

	UINT8* msgcheck = (UINT8 *) msg;

	if(radioID == RF231RADIO)
		return (void *) grf231Radio.Send_TimeStamped(msg, size, eventTime);
	else
		return NULL;
}


DeviceStatus CPU_Radio_Send(UINT8 radioID)
{

}

DeviceStatus CPU_Radio_TurnOn(UINT8 radioID)
{
	if(radioID == RF231RADIO)
		return grf231Radio.TurnOn();
}

DeviceStatus CPU_Radio_Sleep(UINT8 radioID, UINT8 level)
{
	if(radioID == RF231RADIO)
		return grf231Radio.Sleep(level);
}

DeviceStatus CPU_Radio_ChangeTxPower(UINT8 radioID, int power)
{
	if(radioID == RF231RADIO)
		return grf231Radio.ChangeTxPower(power);

}

DeviceStatus CPU_Radio_ChangeChannel(UINT8 radioID, int channel)
{
	if(radioID == RF231RADIO)
		return grf231Radio.ChangeChannel(channel);
}

DeviceStatus CPU_Radio_ClearChannelAssesment (UINT8 radioID)
{
	if(radioID == RF231RADIO)
		return grf231Radio.ClearChannelAssesment();
	else
		return DS_Fail;
}


DeviceStatus CPU_Radio_ClearChannelAssesment2(UINT8 radioID, UINT32 numberMicroSecond)
{
	if(radioID == RF231RADIO)
		return grf231Radio.ClearChannelAssesment(numberMicroSecond);
	else
		return DS_Fail;
}

UINT32 CPU_Radio_GetChannel(UINT8 radioID)
{
	if(radioID == RF231RADIO)
		return grf231Radio.GetChannel();
	else
		return 0;
}

UINT32 CPU_Radio_GetTxPower(UINT8 radioID)
{
	if(radioID == RF231RADIO)
		return grf231Radio.GetTxPower();
	else
		return 0;
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
	return 1;
}
