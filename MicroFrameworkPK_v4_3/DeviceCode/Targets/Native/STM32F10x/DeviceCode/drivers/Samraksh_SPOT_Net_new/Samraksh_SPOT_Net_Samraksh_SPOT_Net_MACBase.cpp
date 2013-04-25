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

using namespace Samraksh::SPOT::Net;

extern CLR_RT_HeapBlock_NativeEventDispatcher *Net_ne_Context;

MacEventHandler_t MACBase::Event_Handler;
UINT8 MACBase::MacID;
UINT8 MACBase::MyAppID;

// CSMA callbacks to be registered with the radio
void  ManagedCallback(UINT16 numberOfPackets);
void  ManagedSendAckCallback(void *msg, UINT16 size, NetOpStatus status);

UINT8 CSMAInteropBuffer[128];

INT32 MACBase::RemovePacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

UINT8 MACBase::GetPendingPacketCount( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    UINT8 retVal = 0; 
    return retVal;
}

UINT8 MACBase::GetID( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    UINT8 retVal = 0; 
    return retVal;
}

INT8 MACBase::SetAddress( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

UINT16 MACBase::GetAddress( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    UINT16 retVal = 0; 
    return retVal;
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

INT32 MACBase::GetNextPacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
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

	Event_Handler.SetRecieveHandler(&ManagedCallback);
	Event_Handler.SetSendAckHandler(&ManagedSendAckCallback);

	MACBase::MacID = param1;

	MyAppID=3; //pick a number less than MAX_APPS currently 4.

	if(Mac_Initialize(&Event_Handler, &MACBase::MacID, MyAppID, (void*) &config) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeTxPower(1, configParams[5]) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeChannel(1, configParams[6]) != DS_Success)
		return DS_Fail;

}

INT8 MACBase::GetNeighborInternal( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

void  ManagedSendAckCallback(void *msg, UINT16 size, NetOpStatus status){
}

void ManagedCallback(UINT16 numberOfPackets)
{
	UINT32 data1, data2;
	data1 = numberOfPackets;
	data2 = 0;

	SaveNativeEventToHALQueue( Net_ne_Context, data1, data2 );

}
