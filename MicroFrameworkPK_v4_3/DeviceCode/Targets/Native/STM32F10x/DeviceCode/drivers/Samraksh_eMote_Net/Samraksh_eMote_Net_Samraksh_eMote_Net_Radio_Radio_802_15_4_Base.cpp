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

UINT16 Radio_802_15_4_Base::GetRadioAddress( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, HRESULT &hr )
{
	UINT16 address;
	address = CPU_Radio_GetAddress(radioType);
	return address;
}

INT8 Radio_802_15_4_Base::SetRadioAddress( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, UINT16 address, HRESULT &hr )
{
	BOOL status;
	status = CPU_Radio_SetAddress(radioType, address);
	return status;
}

INT32 Radio_802_15_4_Base::TurnOnRx( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_TurnOnRx(radioType);
	return status;
}

INT32 Radio_802_15_4_Base::TurnOffRx( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_TurnOffRx(radioType);
	return status;
}

INT32 Radio_802_15_4_Base::Sleep( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, UINT8 level, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_Sleep(radioType, level);
	return status;
}

INT32 Radio_802_15_4_Base::PreLoad( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 messageBuffer, UINT16 size, HRESULT &hr )
{
	UINT8* managedBuffer = messageBuffer.GetBuffer();
	NetOpStatus status = CPU_Radio_PreloadMessage(managedBuffer, size);
    return status;
}

INT32 Radio_802_15_4_Base::SendStrobe( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, UINT16 size, HRESULT &hr )
{
	NetOpStatus status = NetworkOperations_Success;
	void* returnMsg = CPU_Radio_SendStrobe(radioType, size);
	if(returnMsg == NULL){
		status = NetworkOperations_Fail;
	}

    return status;
}

INT32 Radio_802_15_4_Base::Send( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, CLR_RT_TypedArray_UINT8 message, UINT16 size, HRESULT &hr )
{
	void* msg;
	msg = CPU_Radio_Send(radioType, message.GetBuffer(), size);
	if(msg == NULL)
		return -1;
	else
		return 0;
}

INT32 Radio_802_15_4_Base::SendTimeStamped( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, CLR_RT_TypedArray_UINT8 message, UINT16 size, UINT32 eventTime, HRESULT &hr )
{
	void* msg;
	msg = CPU_Radio_Send_TimeStamped(radioType, message.GetBuffer(), size, eventTime);
	if(msg == NULL)
		return -1;
	else
		return 0;
}

INT8 Radio_802_15_4_Base::ClearChannelAssesment( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, HRESULT &hr )
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

INT8 Radio_802_15_4_Base::ClearChannelAssesment( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, UINT16 numberOfMicroSecond, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_ClearChannelAssesment(radioType, numberOfMicroSecond);
	return status;
}

/*INT32 Radio_802_15_4_Base::GetNextPacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 dataBuffer, HRESULT &hr )
{
	INT32 retVal = 0;
    return retVal;
}*/

INT32 Radio_802_15_4_Base::InternalInitialize( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 marshalBuffer, HRESULT &hr )
{
	INT32 result = DS_Success;
	UINT8* configParams = marshalBuffer.GetBuffer();

	if(CPU_Radio_SetRadioType(configParams[2]) != DS_Success) {
	    hr = -1;
	    result = DS_Fail;
	}

	if(CPU_Radio_ChangeTxPower( configParams[2], configParams[1]) != DS_Success) {
	    hr = -1;
	    result = DS_Fail;
	}

	if(CPU_Radio_ChangeChannel( configParams[2], configParams[0]) != DS_Success) {
	    hr = -1;
	    result = DS_Fail;
	}

	return result;
}

INT32 Radio_802_15_4_Base::SetRadioType( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_SetRadioType(radioType);
	return status;
}

INT32 Radio_802_15_4_Base::SetTxPower( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, INT32 power, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_ChangeTxPower(radioType, power);
	return status;
}

INT32 Radio_802_15_4_Base::SetChannel( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, INT32 channel, HRESULT &hr )
{
	DeviceStatus status;
	status = CPU_Radio_ChangeChannel(radioType, channel);
	return status;
}

UINT8 Radio_802_15_4_Base::GetRadioTypeValue( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	UINT8 radioType;
	radioType = CPU_Radio_GetRadioType();
	return radioType;
}

UINT8 Radio_802_15_4_Base::GetTxPowerValue( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, HRESULT &hr )
{
	UINT8 txPower;
	txPower = CPU_Radio_GetTxPower(radioType);
	return txPower;
}

UINT8 Radio_802_15_4_Base::GetActiveChannel( CLR_RT_HeapBlock* pMngObj, UINT8 radioType, HRESULT &hr )
{
	UINT8 channel;
	channel = CPU_Radio_GetChannel(radioType);
	return channel;
}

/*INT32 Radio_802_15_4_Base::ReConfigure( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 marshalBuffer, HRESULT &hr )
{
	INT32 result = DS_Success;
	UINT8* configParams = marshalBuffer.GetBuffer();

	if(CPU_Radio_SetRadioType(configParams[2]) != DS_Success) {
	    hr = -1;
	    result = DS_Fail;
	}

	if(CPU_Radio_ChangeTxPower( configParams[2], configParams[1]) != DS_Success) {
	    hr = -1;
	    result = DS_Fail;
	}

	if(CPU_Radio_ChangeChannel( configParams[2], configParams[0]) != DS_Success) {
	    hr = -1;
	    result = DS_Fail;
	}

	return result;
}*/

