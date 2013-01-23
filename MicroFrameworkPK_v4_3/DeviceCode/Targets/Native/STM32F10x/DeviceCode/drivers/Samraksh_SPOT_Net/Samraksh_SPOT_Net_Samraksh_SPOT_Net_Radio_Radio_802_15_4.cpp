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
#include "Samraksh_SPOT_Net_Samraksh_SPOT_Net_Radio_Radio_802_15_4.h"
#include <TinyCLR_Runtime.h>
#include <TinyCLR_Hardware.h>

using namespace Samraksh::SPOT::Net::Radio;

RadioEventHandler_t Radio_802_15_4::Radio_Event_Handler;
UINT8 Radio_802_15_4::RadioID;
Message_15_4_t Radio_802_15_4::SendMsg, Radio_802_15_4::RcvMsg;
Message_15_4_t *Radio_802_15_4::SendMsgPtr, *Radio_802_15_4::RcvMsgPtr;
CLR_RT_HeapBlock_NativeEventDispatcher* Radio_802_15_4::ne_Context;
UINT64 Radio_802_15_4::ne_userData;
CLR_RT_TypedArray_UINT8 Radio_802_15_4::managedRadioMsg;

void*  ManagedRadioCallback_802_15_4(void *msg, UINT16 size);

INT32 Radio_802_15_4::Configure( CLR_RT_HeapBlock* pMngObj, UNSUPPORTED_TYPE param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 Radio_802_15_4::UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

UINT8 Radio_802_15_4::GetID( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	//return 0;
    return RadioID;
}

UINT16 Radio_802_15_4::GetAddress( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return MF_NODE_ID;
}

INT8 Radio_802_15_4::SetAddress( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr )
{
	MF_NODE_ID = param0;
	return 1;
}

INT32 Radio_802_15_4::TurnOn( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 Radio_802_15_4::Sleep( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 Radio_802_15_4::PreLoad( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, UINT16 param1, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 Radio_802_15_4::SendStrobe( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 Radio_802_15_4::Send( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, UINT16 param1, HRESULT &hr )
{

	UINT8 *buffer = param0.GetBuffer();

	memcpy (SendMsgPtr->GetPayload(), buffer, param1);
	SendMsgPtr = (Message_15_4_t *) CPU_Radio_Send(RadioID, (void *)SendMsgPtr, (SendMsgPtr->GetHeader())->GetLength());

    return 1;
}

INT32 Radio_802_15_4::SendTimeStamped( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, UINT16 param1, UINT32 param2, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT8 Radio_802_15_4::ClearChannelAssesment( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Radio_802_15_4::ClearChannelAssesment( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT32 Radio_802_15_4::InternalInitialize( UNSUPPORTED_TYPE param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{
	CLR_RT_HeapBlock* config = ((CLR_RT_HeapBlock*)param0)->Dereference();
	//FAULT_ON_NULL(config);
	RadioConfig nativeConfig;
	Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_Radio_RadioConfiguration configFields;
	nativeConfig.Channel = config[configFields.FIELD__Channel].NumericByRef().u1;
	nativeConfig.Sensitivity = config[configFields.FIELD__Sensitivity].NumericByRef().u1;
	nativeConfig.TimeStampOffset = config[configFields.FIELD__TimeStampOffset].NumericByRef().u1;
	nativeConfig.TxPower = config[configFields.FIELD__TxPower].NumericByRef().u1;

	//We dont do anything with the radio config right now, but this needs to change soon.

	UINT8 numberOfRadios=1;
	UINT8 MacId=254;
	Radio_Event_Handler.SetRecieveHandler(ManagedRadioCallback_802_15_4);
	CPU_Radio_Initialize(&Radio_Event_Handler, &RadioID, numberOfRadios, MacId);
	SendMsgPtr = &SendMsg;
	RcvMsgPtr = &RcvMsg;

	//Initialize the pointer to the managed message to be copied when you receive a radio messgae
	managedRadioMsg = param1;

    return 1;
}

void*  ManagedRadioCallback_802_15_4(void *msg, UINT16 size){
	UINT8 * managedBuffer = Radio_802_15_4::managedRadioMsg.GetBuffer();
	memcpy (managedBuffer, msg,  size);
	SaveNativeEventToHALQueue( Radio_802_15_4::ne_Context, UINT32(Radio_802_15_4::ne_userData >> 16), UINT32(Radio_802_15_4::ne_userData & 0xFFFFFFFF) );
	return msg;
}

