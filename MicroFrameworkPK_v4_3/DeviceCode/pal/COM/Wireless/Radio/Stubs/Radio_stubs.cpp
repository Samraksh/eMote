/*
 * Radio_stubs.c
 *
 *  Created on: Oct 1, 2012
 *      Author: Mukundan
 */
#include "../MAC/Base/Radio_decl.h"


DeviceStatus CPU_Radio_Initialize(RadioEventHandler* eventHandlers, UINT8* radioIDs, UINT8 numberRadios ) //Initializes Return the ID of the Radio layer that was initialized
{
	return DS_Fail;
}

bool CPU_Radio_UnInitialize()
{
	return FALSE;
}

UINT8 CPU_Radio_GetRadioIDs(UINT8* radioIDs)
{
	return 0;
}

void* CPU_Radio_Preload(UINT8 radioID,void * msg, UINT16 size)
{
}

void* CPU_Radio_Send(UINT8 radioID,void * msg, UINT16 size){
}

void* CPU_Radio_Send_TimeStamped(UINT8 radioID,void * msg, UINT16 size, UINT32 eventTime){
}


DeviceStatus CPU_Radio_Send_Strobe(UINT8 radioID)	//Send preloaded message
{
	return DS_Fail;
}

DeviceStatus CPU_Radio_ClearChannelAssesment (UINT8 radioID)
{
	return DS_Fail;
}

bool CPU_Radio_ClearChannelAssesment2(UINT8 radioID, UINT32 numberMicroSecond){
	return FALSE;
}

bool CPU_Radio_SetTimeStamp(UINT8 radioID){
	return FALSE;
}

bool CPU_Radio_SetTimeStamp2(UINT8 radioID, UINT64 timeStamp){
	return FALSE;
}

UINT32 CPU_Radio_GetSNR(UINT8 radioID){
	return 0;
}

UINT32 CPU_Radio_GetRSSI(UINT8 radioID){
	return 0;
}


//Aggregate apis
bool CPU_Radio_SwitchType(UINT8 radioID){
	return FALSE;
}

//Radio* CPU_Radio_GetRadio(UINT8 radioID);
bool CPU_Radio_Intialize2(UINT8  radioID)
{
	return FALSE;
}

bool CPU_Radio_UnInitialize2(UINT8 radioID)
{
	return FALSE;
}

UINT8 CPU_Radio_NumberRadiosSupported()
{
	return 1;
}


