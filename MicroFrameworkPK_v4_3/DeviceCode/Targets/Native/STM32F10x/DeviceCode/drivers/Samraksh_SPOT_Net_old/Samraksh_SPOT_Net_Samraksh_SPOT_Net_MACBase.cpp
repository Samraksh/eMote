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


#include "Samraksh_SPOT_Net.h"
#include "Samraksh_SPOT_Net_Samraksh_SPOT_Net_MACBase.h"
#include <Samraksh/Mac_decl.h>
#include <Samraksh/MAC.h>

using namespace Samraksh::SPOT::Net;

extern CLR_RT_HeapBlock_NativeEventDispatcher *Net_ne_Context;

MacEventHandler_t MACBase::Event_Handler;
UINT8 MacID = 0;
UINT8 MACBase::MyAppID;

//extern UINT8 MacName;

enum CallBackTypes
{
	RecievedCallback,
	NeighbourChangedCallback,
};

void ManagedCallback(UINT16 arg1, UINT16 arg2);
void  ManagedSendAckCallback(void *msg, UINT16 size, NetOpStatus status);

void NeighbourChangedCallbackFn(INT16 numberOfNeighbours);
void ReceiveDoneCallbackFn(UINT16 numberOfPackets);

extern Buffer_15_4_t m_receive_buffer;

UINT8 CSMAInteropBuffer[128];

INT32 MACBase::RemovePacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
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

INT8 MACBase::SetAddress( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr )
{
	MF_NODE_ID = param0;
	return 1;;
}

UINT16 MACBase::GetAddress( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return MF_NODE_ID;
}

INT32 MACBase::Send( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, UINT16 param2, UINT16 param3, HRESULT &hr )
{
    UINT16 address, offset, length;
    UINT8* payload =param1.GetBuffer();
    address=param0;
    offset=param2;
    length=param3;
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

INT32 MACBase::GetNextPacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
	UINT8* managedBuffer = param0.GetBuffer();
    return Mac_GetNextPacket(&managedBuffer);
}

INT32 MACBase::InternalReConfigure( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, UINT8 param1, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 MACBase::InternalInitialize( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, UINT8 param1, HRESULT &hr )
{
	INT32 result;

	UINT8* configParams = param0.GetBuffer();

	MacConfig config;

	config.CCA = (configParams[0] == 1) ? TRUE :  FALSE;
	config.NumberOfRetries = configParams[1];
	config.CCASenseTime = configParams[2];
	config.BufferSize = configParams[3];
	config.RadioID = configParams[4];
	config.NeighbourLivelinessDelay = configParams[5];
	config.NeighbourLivelinessDelay |= configParams[6] << 8;
	config.NeighbourLivelinessDelay |= configParams[7] << 16;
	config.NeighbourLivelinessDelay |= configParams[8] << 24;

	Event_Handler.SetRecieveHandler(&ReceiveDoneCallbackFn);
	Event_Handler.SetNeighbourChangeHandler(&NeighbourChangedCallbackFn);
	Event_Handler.SetSendAckHandler(&ManagedSendAckCallback);

	MyAppID=3; //pick a number less than MAX_APPS currently 4.

	if(Mac_Initialize(&Event_Handler, param1, MyAppID, configParams[11], (void*) &config) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeTxPower( configParams[11], configParams[9]) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeChannel( configParams[11], configParams[10]) != DS_Success)
		return DS_Fail;

}

INT32 MACBase::GetNeighbourListInternal( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, HRESULT &hr )
{
    return Mac_GetNeighbourList(param0.GetBuffer());
}

INT32 MACBase::GetNeighborInternal( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{
   return Mac_GetNeighbourStatus(param0, param1.GetBuffer());
}
void  ManagedSendAckCallback(void *msg, UINT16 size, NetOpStatus status){
}

INT32 MACBase::SendTimeStamped( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, UINT16 param2, UINT16 param3, HRESULT &hr )
{
	UINT16 address, offset, length;
	UINT8* payload =param1.GetBuffer();
	address=param0;
	offset=param2;
	length=param3;
	memcpy (CSMAInteropBuffer, payload,  length);

	return Mac_Send(MacID, address, MFM_DATA, (void*) CSMAInteropBuffer, length);
}

void ReceiveDoneCallbackFn(UINT16 numberOfPackets)
{
	ManagedCallback(RecievedCallback, numberOfPackets);
}

void NeighbourChangedCallbackFn(INT16 numberOfNeighbours)
{
	ManagedCallback(NeighbourChangedCallback,(UINT16) numberOfNeighbours);
}

void ManagedCallback(UINT16 arg1, UINT16 arg2)
{
	UINT32 data1, data2;
	data1 = arg1;
	data2 = arg2;
	//data2 = arg2;

	SaveNativeEventToHALQueue( Net_ne_Context, data1, data2 );

}
