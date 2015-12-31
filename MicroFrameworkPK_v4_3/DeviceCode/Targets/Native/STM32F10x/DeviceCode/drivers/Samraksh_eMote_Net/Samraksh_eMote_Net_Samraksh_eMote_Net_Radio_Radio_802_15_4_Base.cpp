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
#include "Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base.h"
#include <Samraksh/Radio_decl.h>
//#include <Samraksh/Radio.h>

using namespace Samraksh::eMote::Net::Radio;


INT32 Radio_802_15_4_Base::UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

UINT16 Radio_802_15_4_Base::GetAddress( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, HRESULT &hr )
{
	UINT16 address;
	address = CPU_Radio_GetAddress(radioID);
	return address;
}

INT8 Radio_802_15_4_Base::SetAddress( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, UINT16 address, HRESULT &hr )
{
	BOOL status;
	status = CPU_Radio_SetAddress(radioID, address);
	return status;
}

INT32 Radio_802_15_4_Base::TurnOnRx( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_TurnOnRx(radioID);
	return status;
}

INT32 Radio_802_15_4_Base::Sleep( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, UINT8 level, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_Sleep(radioID, level);
	return status;
}

INT32 Radio_802_15_4_Base::PreLoad( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, UINT16 param1, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 Radio_802_15_4_Base::SendStrobe( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 Radio_802_15_4_Base::Send( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, CLR_RT_TypedArray_UINT8 message, UINT16 size, HRESULT &hr )
{
	void* msg;
	msg = CPU_Radio_Send(radioID, message.GetBuffer(), size);
	if(msg == NULL)
		return -1;
	else
		return 0;
}

INT32 Radio_802_15_4_Base::SendTimeStamped( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, CLR_RT_TypedArray_UINT8 message, UINT16 size, UINT32 eventTime, HRESULT &hr )
{
	void* msg;
	msg = CPU_Radio_Send_TimeStamped(radioID, message.GetBuffer(), size, eventTime);
	if(msg == NULL)
		return -1;
	else
		return 0;
}

INT8 Radio_802_15_4_Base::ClearChannelAssesment( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, HRESULT &hr )
{
	DeviceStatus status;

	status = CPU_Radio_ClearChannelAssesment(0);
	if(status == DS_Success){
		return true;
	}
	else
		return false;

	//return status;
}

INT8 Radio_802_15_4_Base::ClearChannelAssesment( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, UINT16 numberOfMicroSecond, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_ClearChannelAssesment(radioID, numberOfMicroSecond);
	return status;
}

INT32 Radio_802_15_4_Base::GetNextPacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 Radio_802_15_4_Base::InternalInitialize( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 Radio_802_15_4_Base::SetTxPower( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, INT32 power, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_ChangeTxPower(radioID, power);
	return status;
}

INT32 Radio_802_15_4_Base::SetChannel( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, INT32 channel, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_ChangeChannel(radioID, channel);
	return status;
}

INT32 Radio_802_15_4_Base::GetTxPower( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, HRESULT &hr )
{
	UINT32 txPower;
	txPower = CPU_Radio_GetTxPower(radioID);
	return txPower;
}

INT32 Radio_802_15_4_Base::GetChannel( CLR_RT_HeapBlock* pMngObj, UINT8 radioID, HRESULT &hr )
{
	UINT32 channel;
	channel = CPU_Radio_GetChannel(radioID);
	return channel;
}

INT32 Radio_802_15_4_Base::ReConfigure( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

