//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "Samraksh_eMote_Net.h"
#include "Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase.h"
#include <Samraksh/MAC_decl.h>
#include <Samraksh/MAC.h>

using namespace Samraksh::eMote::Net;

extern CLR_RT_HeapBlock_NativeEventDispatcher *Net_ne_Context;

MACEventHandler_t MACBase::Event_Handler;
UINT8 MacID = 0;
UINT8 MACBase::MyAppID;

//extern UINT8 MacName;

enum CallBackTypes
{
	ReceivedCallback,
	NeighborChangedCallback,
};

void ManagedCallback(UINT16 arg1, UINT16 arg2);
void ManagedSendAckCallback(void *msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus);

void NeighborChangedCallbackFn(INT16 numberOfNeighbors);
void ReceiveDoneCallbackFn(void* msg, UINT16 numberOfPackets);

extern Buffer_15_4_t g_send_buffer;
extern Buffer_15_4_t g_receive_buffer;

UINT8 CSMAInteropBuffer[128];

INT32 MACBase::RemovePacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 msgBuffer, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

UINT8 MACBase::GetPendingPacketCount_Receive( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return g_receive_buffer.GetNumberMessagesInBuffer();
}

UINT8 MACBase::GetPendingPacketCount_Send( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return g_send_buffer.GetNumberMessagesInBuffer();
}

UINT8 MACBase::GetMACType( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    UINT8 retVal = 0; 
    return retVal;
}

INT32 MACBase::SetRadioAddress( CLR_RT_HeapBlock* pMngObj, UINT16 address, HRESULT &hr )
{
	BOOL status = FALSE;
	status = MAC_SetRadioAddress(address);
	return status;
}

UINT16 MACBase::GetRadioAddress( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	UINT16 temp = MAC_GetRadioAddress();
	return temp;
}

INT32 MACBase::UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	DeviceStatus status;
	status = MAC_UnInitialize();
    return status;
}

/*void MACBase::ReleasePacket( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
}*/

INT32 MACBase::GetNextPacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 nativeBuffer, HRESULT &hr )
{
	UINT8* managedBuffer = nativeBuffer.GetBuffer();
    return MAC_GetNextPacket(&managedBuffer);
}

INT32 MACBase::InternalReConfigure( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 marshalBuffer, HRESULT &hr )
{
	INT32 result = DS_Success;
	UINT8* configParams = marshalBuffer.GetBuffer();
	MACConfig config;

	config.CCA = (configParams[0] == 1) ? TRUE :  FALSE;
	config.NumberOfRetries = configParams[1];
	config.CCASenseTime = configParams[2];
	config.BufferSize = configParams[3];
	config.RadioType = configParams[4];
	config.NeighborLivenessDelay = configParams[5];
	config.NeighborLivenessDelay |= configParams[6] << 8;
	config.NeighborLivenessDelay |= configParams[7] << 16;
	config.NeighborLivenessDelay |= configParams[8] << 24;

	if( MAC_Reconfigure((void*) &config) != DS_Success ){
		hr = -1;
		result = DS_Fail;
	}
	return result;
}

INT32 MACBase::InternalInitialize( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 marshalBuffer, UINT8 macname, HRESULT &hr )
{
	INT32 result = DS_Success;
	UINT8* configParams = marshalBuffer.GetBuffer();
	MACConfig config;

	config.CCA = (configParams[0] == 1) ? TRUE :  FALSE;
	config.NumberOfRetries = configParams[1];
	config.CCASenseTime = configParams[2];
	config.BufferSize = configParams[3];
	config.RadioType = configParams[4];
	config.NeighborLivenessDelay = configParams[5];
	config.NeighborLivenessDelay |= configParams[6] << 8;
	config.NeighborLivenessDelay |= configParams[7] << 16;
	config.NeighborLivenessDelay |= configParams[8] << 24;

	Event_Handler.SetReceiveHandler(&ReceiveDoneCallbackFn);
	Event_Handler.SetNeighborChangeHandler(&NeighborChangedCallbackFn);
	Event_Handler.SetSendAckHandler(&ManagedSendAckCallback);

	MyAppID=3; //pick a number less than MAX_APPS currently 4.

	if(MAC_Initialize(&Event_Handler, macname, MyAppID, configParams[11], (void*) &config) != DS_Success) {
	    hr = -1;
	    result = DS_Fail;
	}

	if(CPU_Radio_ChangeTxPower( configParams[11], configParams[9]) != DS_Success) {
	    hr = -1;
	    result = DS_Fail;
	}

	if(CPU_Radio_ChangeChannel( configParams[11], configParams[10]) != DS_Success) {
	    hr = -1;
	    result = DS_Fail;
	}

	return result;
}

INT32 MACBase::GetNeighborListInternal( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 neighborlist, HRESULT &hr )
{
    return MAC_GetNeighborList(neighborlist.GetBuffer());
}

INT32 MACBase::GetNeighborInternal( CLR_RT_HeapBlock* pMngObj, UINT16 macAddress, CLR_RT_TypedArray_UINT8 neighborlist, HRESULT &hr )
{
   return MAC_GetNeighborStatus(macAddress, neighborlist.GetBuffer());
}

INT32 MACBase::Send( CLR_RT_HeapBlock* pMngObj, UINT16 address, CLR_RT_TypedArray_UINT8 payloadTemp, UINT16 offset, UINT16 size, HRESULT &hr )
{
	InteropNetOpStatus retVal;
    //UINT16 address, offset, length;
    UINT8* payload = payloadTemp.GetBuffer();
    //address=param0;
    //offset=param2;
    //length=param3;
    memcpy (CSMAInteropBuffer, payload,  size);

    if (MAC_Send(address, MFM_DATA, (void*) CSMAInteropBuffer, size) == DS_Success)
		retVal = S_Success;
	else
		retVal = E_MacSendError;

	return retVal;
}

INT32 MACBase::SendTimeStamped( CLR_RT_HeapBlock* pMngObj, UINT16 address, CLR_RT_TypedArray_UINT8 payloadTemp, UINT16 offset, UINT16 size, HRESULT &hr )
{
	InteropNetOpStatus retVal;
	//UINT16 address, offset, length;
	UINT8* payload = payloadTemp.GetBuffer();
	//address=param0;
	//offset=param2;
	//length=param3;
	memcpy (CSMAInteropBuffer, payload, size);

	if (MAC_SendTimeStamped(address, MFM_DATA, (void*) CSMAInteropBuffer, size, (UINT32) HAL_Time_CurrentTicks()) == DS_Success)
		retVal = S_Success;
	else
		retVal = E_MacSendError;

	return retVal;
}

INT32 MACBase::SendTimeStamped( CLR_RT_HeapBlock* pMngObj, UINT16 address, CLR_RT_TypedArray_UINT8 payloadTemp, UINT16 offset, UINT16 size, UINT32 eventTime, HRESULT &hr )
{
	InteropNetOpStatus retVal;
	//UINT16 address, offset, length;
	UINT8* payload = payloadTemp.GetBuffer();
	
	//address=param0;
	//offset=param2;
	//length=param3;
	memcpy (CSMAInteropBuffer, payload, size);

	if (MAC_SendTimeStamped(address, MFM_DATA, (void*) CSMAInteropBuffer, size, eventTime) == DS_Success)
		retVal = S_Success;
	else
		retVal = E_MacSendError;

	return retVal;
}

void ReceiveDoneCallbackFn(void* msg, UINT16 countOfPackets)
{
	ManagedCallback(ReceivedCallback, countOfPackets);
}

void NeighborChangedCallbackFn(INT16 countOfNeighbors)
{
	ManagedCallback(NeighborChangedCallback,(UINT16) countOfNeighbors);
}

void ManagedSendAckCallback(void *msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus){
}

void ManagedCallback(UINT16 arg1, UINT16 arg2)
{
	UINT32 data1, data2;
	data1 = arg1;
	data2 = arg2;
	//data2 = arg2;

	SaveNativeEventToHALQueue( Net_ne_Context, data1, data2 );
}
