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
#include <Samraksh/Mac_decl.h>
#include <Samraksh/MAC.h>

using namespace Samraksh::eMote::Net;

extern CLR_RT_HeapBlock_NativeEventDispatcher *Net_ne_Context;

MacEventHandler_t MACBase::Event_Handler;
UINT8 MacID = 0;
UINT8 MACBase::MyAppID;

//extern UINT8 MacName;

enum CallBackTypes
{
	RecievedCallback,
	NeighborChangedCallback,
};

void ManagedCallback(UINT16 arg1, UINT16 arg2);
void  ManagedSendAckCallback(void *msg, UINT16 size, NetOpStatus status);

void NeighborChangedCallbackFn(INT16 numberOfNeighbors);
void ReceiveDoneCallbackFn(UINT16 numberOfPackets);

extern Buffer_15_4_t m_receive_buffer;

UINT8 CSMAInteropBuffer[128];

INT32 MACBase::RemovePacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 packet, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

UINT8 MACBase::GetPendingPacketCount( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return m_receive_buffer.GetNumberMessagesInBuffer();
}

UINT8 MACBase::GetID( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    UINT8 retVal = 0; 
    return retVal;
}

INT8 MACBase::SetAddress( CLR_RT_HeapBlock* pMngObj, UINT16 address, HRESULT &hr )
{
	Mac_SetRadioAddress(address);
	return 1;
}

UINT16 MACBase::GetAddress( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	UINT16 temp = Mac_GetRadioAddress();
	return temp;
}

INT32 MACBase::Send( CLR_RT_HeapBlock* pMngObj, UINT16 _address, CLR_RT_TypedArray_UINT8 _packet, UINT16 _offset, UINT16 _size, HRESULT &hr )
{
    UINT16 address, offset, length;
    UINT8* payload = _packet.GetBuffer();
    address = _address;
    offset = _offset;
    length = _size;
    memcpy (CSMAInteropBuffer, payload,  length);

    return Mac_Send(MacID, address, MFM_DATA, (void*) CSMAInteropBuffer, length);

}

INT32 MACBase::UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

void MACBase::ReleasePacket( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
}

INT32 MACBase::GetNextPacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 nativeBuffer, HRESULT &hr )
{
	UINT8* managedBuffer = nativeBuffer.GetBuffer();
    return Mac_GetNextPacket(&managedBuffer);
}

INT32 MACBase::InternalReConfigure( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 marshalBuffer, UINT8 macname, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 MACBase::InternalInitialize( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 marshalBuffer, UINT8 macname, HRESULT &hr )
{
	INT32 result;

	UINT8* configParams = marshalBuffer.GetBuffer();

	MacConfig config;

	config.CCA = (configParams[0] == 1) ? TRUE :  FALSE;
	config.NumberOfRetries = configParams[1];
	config.CCASenseTime = configParams[2];
	config.BufferSize = configParams[3];
	config.RadioID = configParams[4];
	config.NeighborLivelinessDelay = configParams[5];
	config.NeighborLivelinessDelay |= configParams[6] << 8;
	config.NeighborLivelinessDelay |= configParams[7] << 16;
	config.NeighborLivelinessDelay |= configParams[8] << 24;

	Event_Handler.SetRecieveHandler(&ReceiveDoneCallbackFn);
	Event_Handler.SetNeighborChangeHandler(&NeighborChangedCallbackFn);
	Event_Handler.SetSendAckHandler(&ManagedSendAckCallback);

	MyAppID=3; //pick a number less than MAX_APPS currently 4.

	if(Mac_Initialize(&Event_Handler, macname, MyAppID, configParams[11], (void*) &config) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeTxPower( configParams[11], configParams[9]) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeChannel( configParams[11], configParams[10]) != DS_Success)
		return DS_Fail;

}

INT32 MACBase::GetNeighborListInternal( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 neighborlist, HRESULT &hr )
{
    return Mac_GetNeighborList(neighborlist.GetBuffer());
}

INT32 MACBase::GetNeighborInternal( CLR_RT_HeapBlock* pMngObj, UINT16 macAddress, CLR_RT_TypedArray_UINT8 byte_nbr, HRESULT &hr )
{
   return Mac_GetNeighborStatus(macAddress, byte_nbr.GetBuffer());
}

void  ManagedSendAckCallback(void *msg, UINT16 size, NetOpStatus status)
{
}

INT32 MACBase::SendTimeStamped( CLR_RT_HeapBlock* pMngObj, UINT16 _address, CLR_RT_TypedArray_UINT8 _packet, UINT16 _offset, UINT16 _size, HRESULT &hr )
{
	UINT16 address, offset, length;
	UINT8* payload = _packet.GetBuffer();
	address = _address;
	offset = _offset;
	length = _size;
	memcpy (CSMAInteropBuffer, payload,  length);

	return Mac_SendTimeStamped(MacID, address, MFM_DATA, (void*) CSMAInteropBuffer, length, (UINT32) HAL_Time_CurrentTicks());
}

void ReceiveDoneCallbackFn(UINT16 numberOfPackets)
{
	ManagedCallback(RecievedCallback, numberOfPackets);
}

INT32 MACBase::SendTimeStamped( CLR_RT_HeapBlock* pMngObj, UINT16 _address, CLR_RT_TypedArray_UINT8 _packet, UINT16 _offset, UINT16 _size, UINT32 _eventTime, HRESULT &hr )
{
	UINT16 address, offset, length;
	UINT8* payload = _packet.GetBuffer();
	address = _address;
	offset = _offset;
	length = _size;
	memcpy (CSMAInteropBuffer, payload,  length);

	return Mac_SendTimeStamped(MacID, address, MFM_DATA, (void*) CSMAInteropBuffer, length, _eventTime);
}

void NeighborChangedCallbackFn(INT16 numberOfNeighbors)
{
	ManagedCallback(NeighborChangedCallback,(UINT16) numberOfNeighbors);
}

void ManagedCallback(UINT16 arg1, UINT16 arg2)
{
	UINT32 data1, data2;
	data1 = arg1;
	data2 = arg2;
	//data2 = arg2;

	SaveNativeEventToHALQueue( Net_ne_Context, data1, data2 );

}
