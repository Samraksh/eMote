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

UINT8 MACInteropBuffer[IEEE802_15_4_FRAME_LENGTH];

INT32 MACBase::RemovePacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 msgBuffer, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

UINT8 MACBase::PendingReceivePacketCount( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return g_receive_buffer.GetNumberMessagesInBuffer();
}

UINT8 MACBase::PendingSendPacketCount( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return g_send_buffer.GetNumberMessagesInBuffer();
}

INT32 MACBase::Send( CLR_RT_HeapBlock* pMngObj, UINT16 address, CLR_RT_TypedArray_UINT8 payloadTemp, UINT16 offset, UINT16 size, HRESULT &hr )
{
	InteropNetOpStatus retVal;
    //UINT16 address, offset, length;
    UINT8* payload = payloadTemp.GetBuffer();
    //address=param0;
    //offset=param2;
    //length=param3;
    memcpy (MACInteropBuffer, payload, size);

    if (MAC_Send(address, MFM_DATA, (void*) MACInteropBuffer, size) == DS_Success)
		retVal = S_Success;
	else
		retVal = E_MacSendError;

	return retVal;
}

INT32 MACBase::UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	DeviceStatus status;
	status = MAC_UnInitialize();
    return status;
}

INT32 MACBase::InternalInitialize( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 marshalBuffer, UINT8 macname, HRESULT &hr )
{
	CPU_GPIO_EnableOutputPin(EMOTE_NET_MANAGED_CALLBACK, FALSE);
	CPU_GPIO_SetPinState(EMOTE_NET_MANAGED_CALLBACK, FALSE);

	INT32 result = DS_Success;
	UINT8* configParams = marshalBuffer.GetBuffer();
	MACConfig config;

	config.CCA = (configParams[0] == 1) ? TRUE :  FALSE;
	config.NumberOfRetries = configParams[1];
	config.CCASenseTime = configParams[2];
	config.BufferSize = configParams[3];
	//config.RadioType = configParams[4];
	config.NeighborLivenessDelay = configParams[4];
	config.NeighborLivenessDelay |= configParams[5] << 8;
	config.NeighborLivenessDelay |= configParams[6] << 16;
	config.NeighborLivenessDelay |= configParams[7] << 24;

	Event_Handler.SetReceiveHandler(&ReceiveDoneCallbackFn);
	Event_Handler.SetNeighborChangeHandler(&NeighborChangedCallbackFn);
	Event_Handler.SetSendAckHandler(&ManagedSendAckCallback);

	MyAppID=3; //pick a number less than MAX_APPS currently 4.

	if(MAC_Initialize(&Event_Handler, macname, MyAppID, configParams[8], (void*) &config) != DS_Success) {
	    hr = -1;
	    result = DS_Fail;
	}
	
	return result;
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
	//config.RadioType = configParams[4];
	config.NeighborLivenessDelay = configParams[4];
	config.NeighborLivenessDelay |= configParams[5] << 8;
	config.NeighborLivenessDelay |= configParams[6] << 16;
	config.NeighborLivenessDelay |= configParams[7] << 24;

	if( MAC_Reconfigure((void*) &config) != DS_Success ){
		hr = -1;
		result = DS_Fail;
	}
	return result;
}

INT32 MACBase::GetNextPacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 nativeBuffer, HRESULT &hr )
{
	UINT8* managedBuffer = nativeBuffer.GetBuffer();
    return MAC_GetNextPacket(&managedBuffer);
}

INT32 MACBase::GetNeighborInternal( CLR_RT_HeapBlock* pMngObj, UINT16 macAddress, CLR_RT_TypedArray_UINT8 neighborlist, HRESULT &hr )
{
   return MAC_GetNeighborStatus(macAddress, neighborlist.GetBuffer());
}

INT32 MACBase::GetNeighborListInternal( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 neighborlist, HRESULT &hr )
{
    return MAC_GetNeighborList(neighborlist.GetBuffer());
}

INT32 MACBase::Send( CLR_RT_HeapBlock* pMngObj, UINT16 address, UINT8 payloadType, CLR_RT_TypedArray_UINT8 payloadTemp, UINT16 offset, UINT16 size, HRESULT &hr )
{
	InteropNetOpStatus retVal;
    //UINT16 address, offset, length;
    UINT8* payload = payloadTemp.GetBuffer();
    //address=param0;
    //offset=param2;
    //length=param3;
    memcpy (MACInteropBuffer, payload, size);

    if (MAC_Send(address, payloadType, (void*) MACInteropBuffer, size) == DS_Success)
		retVal = S_Success;
	else
		retVal = E_MacSendError;

	return retVal;
}

INT32 MACBase::Send( CLR_RT_HeapBlock* pMngObj, UINT16 address, CLR_RT_TypedArray_UINT8 payloadTemp, UINT16 offset, UINT16 size, UINT32 eventTime, HRESULT &hr )
{
	InteropNetOpStatus retVal;
    //UINT16 address, offset, length;
    UINT8* payload = payloadTemp.GetBuffer();
    //address=param0;
    //offset=param2;
    //length=param3;
    memcpy (MACInteropBuffer, payload, size);

    //if (MAC_Send(address, MFM_DATA, (void*) MACInteropBuffer, size) == DS_Success)
    if (MAC_SendTimeStamped(address, MFM_DATA, (void*) MACInteropBuffer, size, eventTime) == DS_Success)
		retVal = S_Success;
	else
		retVal = E_MacSendError;

	return retVal;
}

INT32 MACBase::Send( CLR_RT_HeapBlock* pMngObj, UINT16 address, UINT8 payloadType, CLR_RT_TypedArray_UINT8 payloadTemp, UINT16 offset, UINT16 size, UINT32 eventTime, HRESULT &hr )
{
	InteropNetOpStatus retVal;
	//UINT16 address, offset, length;
	UINT8* payload = payloadTemp.GetBuffer();

	//address=param0;
	//offset=param2;
	//length=param3;
	memcpy (MACInteropBuffer, payload, size);

	if (MAC_SendTimeStamped(address, payloadType, (void*) MACInteropBuffer, size, eventTime) == DS_Success)
		retVal = S_Success;
	else
		retVal = E_MacSendError;

	return retVal;
}

void ReceiveDoneCallbackFn(void* msg, UINT16 payloadType)
{
	CPU_GPIO_SetPinState(EMOTE_NET_MANAGED_CALLBACK, TRUE);
	ManagedCallback(ReceivedCallback, payloadType);
	CPU_GPIO_SetPinState(EMOTE_NET_MANAGED_CALLBACK, FALSE);
}

void NeighborChangedCallbackFn(INT16 countOfNeighbors)
{
	ManagedCallback(NeighborChangedCallback,(UINT16) countOfNeighbors);
}

void ManagedSendAckCallback(void *msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus){
}

void ManagedCallback(UINT16 arg1, UINT16 arg2)
{
	CPU_GPIO_SetPinState(EMOTE_NET_MANAGED_CALLBACK, TRUE);
	UINT32 data1, data2;
	data1 = arg1;
	data2 = arg2;
	//data2 = arg2;

	GLOBAL_LOCK(irq);
	SaveNativeEventToHALQueue( Net_ne_Context, data1, data2 );
	CPU_GPIO_SetPinState(EMOTE_NET_MANAGED_CALLBACK, FALSE);
}
