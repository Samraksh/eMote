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
UINT8* Radio_802_15_4::managedRadioMsg;
CLR_RT_HeapBlock_Array* Radio_802_15_4::pHeapBlockMsgArray;

UINT8 recvMesgBuffer[128];
UINT8 recvMesgBufferLength = 0;

void*  ManagedRadioCallback_802_15_4(void *msg, UINT16 size);


INT32 Radio_802_15_4::UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return CPU_Radio_UnInitialize(RadioID);
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
    return CPU_Radio_TurnOn(RadioID);
}

INT32 Radio_802_15_4::Sleep( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr )
{
   return CPU_Radio_Sleep(RadioID, param0);
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
	UINT8 *buffer = param0.GetBuffer();

	memcpy(SendMsgPtr->GetPayload(),  buffer, param1);

    SendMsgPtr = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(RadioID,(void *)SendMsgPtr, (SendMsgPtr->GetHeader())->GetLength(), param2);

    return 1;
}

INT8 Radio_802_15_4::ClearChannelAssesment( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return CPU_Radio_ClearChannelAssesment(RadioID);
}

INT8 Radio_802_15_4::ClearChannelAssesment( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr )
{
	return CPU_Radio_ClearChannelAssesment2(RadioID, param0);
}

INT32 Radio_802_15_4::GetNextPacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{

	// Return a devicestatus of fail if the buffer is empty
	if(recvMesgBufferLength == 0)
		return 1;

    UINT8* managedBuffer = param0.GetBuffer();

    // Copy to the managed buffer
    memcpy (managedBuffer, recvMesgBuffer, recvMesgBufferLength);

    // Flush the recvMesgBuffer
    memset(recvMesgBuffer, 0, 128);

    // Set the length of the buffer to 0
    recvMesgBufferLength = 0;

    return 0;
}

INT32 Radio_802_15_4::InternalInitialize( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
     INT32 retVal = 0; 
	
    // The marshalling and unmarshalling is handled manually by the drivers
	UINT8 TxPower = param0[0];
	UINT8 Channel = param0[1];

	UINT8 numberOfRadios=1;
	UINT8 MacId=254;
	Radio_Event_Handler.SetRecieveHandler(&ManagedRadioCallback_802_15_4);
	retVal = CPU_Radio_Initialize(&Radio_Event_Handler, &RadioID, numberOfRadios, MacId);

	CPU_Radio_ChangeTxPower(RadioID, TxPower);
	CPU_Radio_ChangeChannel(RadioID, Channel);

    return retVal;
}

INT32 Radio_802_15_4::SetTxPower( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr )
{
    return (INT32) CPU_Radio_ChangeTxPower(RadioID, param0);
}

INT32 Radio_802_15_4::SetChannel( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr )
{
    return (INT32) CPU_Radio_ChangeChannel(RadioID, param0);
}

INT32 Radio_802_15_4::GetTxPower( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return CPU_Radio_GetTxPower(RadioID);
}

INT32 Radio_802_15_4::GetChannel( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return CPU_Radio_GetChannel(RadioID);
}

INT32 Radio_802_15_4::ReConfigure( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
 INT32 retVal = 0; 

	UINT8 TxPower = param0[0];
	UINT8 Channel = param0[1];

    return (CPU_Radio_ChangeTxPower(RadioID, TxPower) & CPU_Radio_ChangeChannel(RadioID, Channel));

}

void*  ManagedRadioCallback_802_15_4(void *msg, UINT16 Size){

	Message_15_4_t* r_msg = (Message_15_4_t*) msg;
	if(Size > IEEE802_15_4_FRAME_LENGTH){
			hal_printf("Radio Receive Error: Packet is too big: %d ", Size);
			return msg;
	}

	UINT32 data1, data2;
	recvMesgBuffer[0] = (UINT8) Size;
	recvMesgBuffer[1] = (UINT8) (Size >> 8);

	UINT16 payloadSize = r_msg->GetHeader()->GetLength();

	UINT8* payload = r_msg->GetPayload();

	for(UINT8 i = 0; i < payloadSize; i++)
	{
		recvMesgBuffer[i + 2] = payload[i];
	}

	recvMesgBufferLength = payloadSize + 2;

	recvMesgBuffer[recvMesgBufferLength++] = r_msg->GetMetaData()->GetRssi();
	recvMesgBuffer[recvMesgBufferLength++] = r_msg->GetMetaData()->GetLqi();

	recvMesgBuffer[recvMesgBufferLength++] = (UINT8) r_msg->GetHeader()->src;
	recvMesgBuffer[recvMesgBufferLength++] = (UINT8) (r_msg->GetHeader()->src >> 8);

	recvMesgBuffer[recvMesgBufferLength++] = (r_msg->GetHeader()->src == RADIO_BROADCAST_ADDRESS) ? 0 : 1;

	recvMesgBufferLength -= 1;

	// Number of packets is 1 always in the radio
	data1 = 1;
	data2 = 0;

	SaveNativeEventToHALQueue( Radio_802_15_4::ne_Context, data1, data2 );

	return msg;

#if 0
	if(g_CLR_RT_ExecutionEngine.m_heapState ==  CLR_RT_ExecutionEngine::c_HeapState_Normal){
		UINT8 *managedMsg=(UINT8 *)Radio_802_15_4::pHeapBlockMsgArray->GetFirstElement();
		Message_15_4_t* r_msg = (Message_15_4_t*) msg;
		if(size > IEEE802_15_4_FRAME_LENGTH){
				hal_printf("Radio Receive Error: Packet is too big: %d ", size);
				return msg;
		}
		UINT16 payload_size, src;
		BOOL unicast;
		UINT32 data1, data2;
		UINT8 rssi, lqi;
		payload_size = r_msg->GetHeader()->GetLength();
		src = r_msg->GetHeader()->src;
		unicast = (src == RADIO_BROADCAST_ADDRESS) ?  FALSE: TRUE;
		rssi = r_msg->GetMetaData()->GetRssi();
		lqi = r_msg->GetMetaData()->GetLqi();
		data1 = 0; data2=0;
		data1 = (size & 0x0000FFFF);
		data1 = data1 | ((src << 16) & 0xFFFF0000);
		data2 = (rssi & 0x000000FF);
		data2 = data2 | ((lqi << 8) & 0x0000FF00);
		data2 = data2 | ((unicast << 16) & 0x00FF0000);

		memcpy (managedMsg, r_msg->GetPayload(), payload_size );
		SaveNativeEventToHALQueue( Radio_802_15_4::ne_Context, data1, data2 );
	}else {

	}
	return msg;
#endif


}

static HRESULT Initialize_Radio_802_15_4_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData ){
	Radio_802_15_4::ne_Context = pContext;
	Radio_802_15_4::ne_userData = userData;
	return S_OK;
}

static HRESULT  EnableDisable_Radio_802_15_4_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable ){
	return S_OK;
}

static HRESULT Cleanup_Radio_802_15_4_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext ){
	Radio_802_15_4::ne_Context = NULL;
	Radio_802_15_4::ne_userData = 0;
	CleanupNativeEventsFromHALQueue( pContext );
	return S_OK;
}



static const CLR_RT_DriverInterruptMethods g_CLR_Radio_802_15_4_DriverMethods =
{ Initialize_Radio_802_15_4_Driver,
  EnableDisable_Radio_802_15_4_Driver,
  Cleanup_Radio_802_15_4_Driver
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_RadioCallback_802_15_4  =
{
    "RadioCallback_802_15_4",
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_CLR_Radio_802_15_4_DriverMethods
};

