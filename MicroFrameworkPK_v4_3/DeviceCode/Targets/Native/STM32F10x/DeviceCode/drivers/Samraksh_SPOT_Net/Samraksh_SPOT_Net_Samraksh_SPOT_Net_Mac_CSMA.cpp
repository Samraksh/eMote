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
#include "Samraksh_SPOT_Net_Samraksh_SPOT_Net_Mac_CSMA.h"
#include <TinyCLR_Runtime.h>
#include <TinyCLR_Hardware.h>
//#include <Samraksh/HALTimer.h>


//extern HALTimerManager gHalTimerManagerObject;
using namespace Samraksh::SPOT::Net::Mac;

MacEventHandler_t CSMA::CSMA_Event_Handler;
UINT8 CSMA::MacID;
UINT8 CSMA::MyAppID;
//Message_15_4_t SendMsg, RcvMsg;
//Message_15_4_t *SendMsgPtr, *RcvMsgPtr;
CLR_RT_HeapBlock_NativeEventDispatcher* CSMA::ne_Context;
UINT64 CSMA::ne_userData;
UINT8 *CSMA::managedCSMAMsg;
CLR_RT_HeapBlock_Array* CSMA::pHeapBlockMsgArray;


UINT8 CSMAInteropBuffer[128];

void  ManagedCSMACallback(void *msg, UINT16 size);
void  ManagedCSMASendAckCallback(void *msg, UINT16 size, NetOpStatus status);

INT32 CSMA::Configure( CLR_RT_HeapBlock* pMngObj, UNSUPPORTED_TYPE param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 CSMA::UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 CSMA::Send( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, UINT16 param2, UINT16 param3, HRESULT &hr )
{
    UINT16 address, offset, length;
    CPU_GPIO_SetPinState((GPIO_PIN)31, TRUE);
    UINT8* payload =param1.GetBuffer();
    address=param0;
    offset=param2;
    length=param3;
    memcpy (CSMAInteropBuffer, payload,  length);

    Mac_Send(MacID, address, MFM_DATA, (void*) CSMAInteropBuffer, length);
    CPU_GPIO_SetPinState((GPIO_PIN)31, FALSE);
    return 1;
}

UINT16 CSMA::GetAddress( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return MF_NODE_ID;
}

INT8 CSMA::SetAddress( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr )
{
    MF_NODE_ID = param0;
    return 1;
}

UINT8 CSMA::GetID( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return MacID;
}

UNSUPPORTED_TYPE CSMA::GetNeighborTable( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    UNSUPPORTED_TYPE retVal = 0; 
    return retVal;
}

UNSUPPORTED_TYPE CSMA::GetNeighbors( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr )
{
    UNSUPPORTED_TYPE retVal = 0; 
    return retVal;
}

UINT8 CSMA::GetBufferSize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return Mac_GetBufferSize(MacID);
}

UINT8 CSMA::GetPendingPacketCount( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    UINT8 retVal = 0; 
    return retVal;
}

INT32 CSMA::RemovePacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 CSMA::InternalInitialize( UNSUPPORTED_TYPE param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{
	CLR_RT_HeapBlock* config = ((CLR_RT_HeapBlock*)param0)->Dereference();
	//FAULT_ON_NULL(config);

	MyAppID=3; //pick a number less than MAX_APPS currently 4.
	MacConfig nativeCSMAConfig;
	nativeCSMAConfig.Network = 138;
	//gHalTimerManagerObject.Initialize();
	CSMA_Event_Handler.SetRecieveHandler(&ManagedCSMACallback);
	CSMA_Event_Handler.SetSendAckHandler(&ManagedCSMASendAckCallback);
	MAC_Initialize(&CSMA_Event_Handler,&MacID, MyAppID, (void*) &nativeCSMAConfig);
	//SendMsgPtr = &SendMsg;
	//RcvMsgPtr = &RcvMsg;

	//Initialize the managed receive message buffer to copy messages into, when you get a message from radio
	CSMA::pHeapBlockMsgArray->Pin();
	managedCSMAMsg= param1.GetBuffer();
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, FALSE);

    return 1;
}

void  ManagedCSMASendAckCallback(void *msg, UINT16 size, NetOpStatus status){
	CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
	//return msg;
}

void  ManagedCSMACallback(void *msg, UINT16 size){
	CPU_GPIO_SetPinState((GPIO_PIN)30, TRUE);

	if(g_CLR_RT_ExecutionEngine.m_heapState ==  CLR_RT_ExecutionEngine::c_HeapState_Normal){
		UINT8 *managedMsg=(UINT8 *) CSMA::pHeapBlockMsgArray->GetFirstElement();
		memcpy (managedMsg, msg,  size);
		SaveNativeEventToHALQueue( CSMA::ne_Context, UINT32(CSMA::ne_userData >> 16), UINT32(CSMA::ne_userData & 0xFFFFFFFF) );
		CPU_GPIO_SetPinState((GPIO_PIN)30, FALSE);
	}else {
		//This needs to filled up. Right now if we receive a message if the heap is not normal we loose a packet.

	}
	//return msg;
}
