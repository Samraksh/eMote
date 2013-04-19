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
//#define DEBUG_CSMA 1

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

void  ManagedCSMACallback(void *msg, UINT16 size, UINT16 Src, BOOL Unicast, UINT8 RSSI, UINT8 LinkQuality);
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
#ifdef DEBUG_CSMA
    CPU_GPIO_SetPinState((GPIO_PIN)31, TRUE);
#endif
    UINT8* payload =param1.GetBuffer();
    address=param0;
    offset=param2;
    length=param3;
    memcpy (CSMAInteropBuffer, payload,  length);

    Mac_Send(MacID, address, MFM_DATA, (void*) CSMAInteropBuffer, length);
#ifdef DEBUG_CSMA
    CPU_GPIO_SetPinState((GPIO_PIN)31, FALSE);
#endif
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

INT8 CSMA::UnMarshalNeighbor( CLR_RT_TypedArray_UINT8 nbr, Neighbor_t *nativeNbr){

	/*
	//Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_Mac_Neighbor nbrFields;
	//Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_Mac_Link linkFields;
	nbr[nbrFields.FIELD__FrameLength].NumericByRef().u2=nativeNbr->FrameLength;
	//nbr[nbrFields.FIELD__LastHeardTime].NumericByRef().u8 = nativeNbr->LastHeardTime;

	nbr[nbrFields.FIELD__PacketsReceived].NumericByRef().u2=nativeNbr->PacketsReceived;
	nbr[nbrFields.FIELD__ReceiveDutyCycle].NumericByRef().u1=nativeNbr->ReceiveDutyCycle;
	//nbr[nbrFields.FIELD__Status].NumericByRef().u1=(UINT8)nativeNbr->Status;

	//CLR_RT_HeapBlock* fwdLink = nbr[nbrFields.FIELD__ForwardLink].ExtractValueBlock();
	CLR_RT_HeapBlock* rvseLink = nbr[nbrFields.FIELD__ReverseLink].m_data;

	rvseLink[linkFields.FIELD__AveRSSI].NumericByRef().u1 = nativeNbr->ReverseLink.AvgRSSI;
	rvseLink[linkFields.FIELD__LinkQuality].NumericByRef().u1 = nativeNbr->ReverseLink.LinkQuality;
	rvseLink[linkFields.FIELD__AveDelay].NumericByRef().u1 = nativeNbr->ReverseLink.AveDelay;

	fwdLink[linkFields.FIELD__AveRSSI].NumericByRef().u1 = nativeNbr->ForwardLink.AvgRSSI;
	fwdLink[linkFields.FIELD__LinkQuality].NumericByRef().u1 = nativeNbr->ForwardLink.LinkQuality;
	fwdLink[linkFields.FIELD__AveDelay].NumericByRef().u1 = nativeNbr->ForwardLink.AveDelay;
	*/
	if(nbr.GetSize() < 22){
		return 0;
	}
	UINT8* b_nbr = nbr.GetBuffer();
	UINT16 *address = (UINT16 *)b_nbr;
	(*address)=nativeNbr->MacAddress;
	b_nbr[2]=nativeNbr->ForwardLink.AvgRSSI;b_nbr[3]=nativeNbr->ForwardLink.LinkQuality;b_nbr[4]=nativeNbr->ForwardLink.AveDelay;
	b_nbr[5]=nativeNbr->ReverseLink.AvgRSSI;b_nbr[6]=nativeNbr->ReverseLink.LinkQuality;b_nbr[7]=nativeNbr->ReverseLink.AveDelay;
	b_nbr[8]=nativeNbr->Status;
	UINT16 *pksRcvd = (UINT16 *)&b_nbr[9];
	(*pksRcvd)=nativeNbr->PacketsReceived;
	//UINT *Time = (UINT64 *)&b_nbr[11];
	//(*Time)=nativeNbr->LastHeardTime;
	memcpy(&b_nbr[11],(UINT8 *)&nativeNbr->LastHeardTime,8);
	b_nbr[19]=nativeNbr->ReceiveDutyCycle;
	UINT16 *frame = (UINT16 *)&b_nbr[20];
	(*frame)=nativeNbr->FrameLength;
	return 1;
}

INT8 CSMA::GetNeighborInternal( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{
    Neighbor_t *node = Mac_GetNeighbor(MacID,param0);
    if(node==NULL){
    	return 0;
    }else{
    	return UnMarshalNeighbor(param1, node);
    }
}

INT32 CSMA::SetCCA( INT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 CSMA::SetNumberOfRetries( UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 CSMA::SetCCASenseTime( UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 CSMA::SetBufferSize( UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 CSMA::SetRadioID( UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT8 CSMA::GetCCA( HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

UINT8 CSMA::GetNumberOfRetries( HRESULT &hr )
{
    UINT8 retVal = 0; 
    return retVal;
}

UINT8 CSMA::GetCCASenseTime( HRESULT &hr )
{
    UINT8 retVal = 0; 
    return retVal;
}

UINT8 CSMA::GetRadioID( HRESULT &hr )
{
    UINT8 retVal = 0; 
    return retVal;
}

INT32 CSMA::ReConfigure( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
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
	Mac_Initialize(&CSMA_Event_Handler,&MacID, MyAppID, (void*) &nativeCSMAConfig);

	// Nived.Sivadas - turning on the radio to get emotePing going
	CPU_Radio_TurnOn(1);
	//SendMsgPtr = &SendMsg;
	//RcvMsgPtr = &RcvMsg;
	//Initialize a temporary neighbor_heap variable so act as managed callback return object

	//Initialize the managed receive message buffer to copy messages into, when you get a message from radio
	CSMA::pHeapBlockMsgArray->Pin();
	managedCSMAMsg= param1.GetBuffer();
#ifdef DEBUG_CSMA
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, FALSE);
#endif
    return 1;
}

void  ManagedCSMASendAckCallback(void *msg, UINT16 size, NetOpStatus status){
#ifdef DEBUG_CSMA
	CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
	//return msg;
}

void  ManagedCSMACallback(void *msg, UINT16 size, UINT16 src, BOOL unicast, UINT8 rssi, UINT8 linkquality){
#ifdef DEBUG_CSMA
	CPU_GPIO_SetPinState((GPIO_PIN)30, TRUE);
#endif
	if(g_CLR_RT_ExecutionEngine.m_heapState ==  CLR_RT_ExecutionEngine::c_HeapState_Normal){
		UINT8 *managedMsg=(UINT8 *) CSMA::pHeapBlockMsgArray->GetFirstElement();
		memcpy (managedMsg, msg,  size);
		UINT32 data1, data2;
		data1 = 0; data2=0;
		data1 = (size & 0x0000FFFF);
		data1 = data1 | ((((UINT32)src) << 16) & 0xFFFF0000);
		data2 = (rssi & 0x000000FF);
		data2 = data2 | ((((UINT32)linkquality) << 8) & 0x0000FF00);
		data2 = data2 | ((unicast << 16) & 0x00FF0000);
		//hal_printf("Calling CSMA Handler: Src: %x, data1: %x, data2: %x\n", src, data1, data2);
		SaveNativeEventToHALQueue( CSMA::ne_Context, data1, data2 );

	}else {
		//This needs to filled up. Right now if we receive a message if the heap is not normal we loose a packet.

	}
#ifdef DEBUG_CSMA
	CPU_GPIO_SetPinState((GPIO_PIN)30, FALSE);
#endif
	//return msg;
}

