//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "Samraksh_eMote_Net.h"


CLR_RT_HeapBlock_NativeEventDispatcher *Net_ne_Context;
UINT64 Net_ne_userData;

static HRESULT Initialize_Net_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData ){
	Net_ne_Context = pContext;
	Net_ne_userData = userData;
	return S_OK;
}

static HRESULT  EnableDisable_Net_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable ){
	return S_OK;
}

static HRESULT Cleanup_Net_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext ){
	Net_ne_Context = NULL;
	Net_ne_userData = 0;
	CleanupNativeEventsFromHALQueue( pContext );
	return S_OK;
}


static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::RemovePacket___SamraksheMoteNetDeviceStatus__SZARRAY_U1,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetPendingPacketCount___U1,
    NULL,
    NULL,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetID___U1,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::SetAddress___BOOLEAN__U2,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetAddress___U2,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::Send___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::UnInitialize___SamraksheMoteNetDeviceStatus,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::ReleasePacket___VOID,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetNextPacket___SamraksheMoteNetDeviceStatus__SZARRAY_U1,
    NULL,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::InternalReConfigure___SamraksheMoteNetDeviceStatus__SZARRAY_U1__U1,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::InternalInitialize___SamraksheMoteNetDeviceStatus__SZARRAY_U1__U1,
    NULL,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetNeighborListInternal___SamraksheMoteNetDeviceStatus__SZARRAY_U2,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetNeighborInternal___SamraksheMoteNetDeviceStatus__U2__SZARRAY_U1,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::SendTimeStamped___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::SendTimeStamped___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2__U4,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::UnInitialize___SamraksheMoteNetDeviceStatus,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::GetID___U1,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::GetAddress___U2,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::SetAddress___BOOLEAN__U2,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::TurnOn___SamraksheMoteNetDeviceStatus,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::Sleep___SamraksheMoteNetDeviceStatus__U1,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::PreLoad___SamraksheMoteNetNetOpStatus__SZARRAY_U1__U2,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::SendStrobe___SamraksheMoteNetNetOpStatus,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::Send___SamraksheMoteNetNetOpStatus__SZARRAY_U1__U2,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::SendTimeStamped___SamraksheMoteNetNetOpStatus__SZARRAY_U1__U2__U4,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::ClearChannelAssesment___BOOLEAN,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::ClearChannelAssesment___BOOLEAN__U2,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::GetNextPacket___SamraksheMoteNetDeviceStatus__SZARRAY_U1,
    NULL,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::InternalInitialize___SamraksheMoteNetDeviceStatus__SZARRAY_U1,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::SetTxPower___SamraksheMoteNetDeviceStatus__I4,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::SetChannel___SamraksheMoteNetDeviceStatus__I4,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::GetTxPower___I4,
    NULL,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::GetChannel___I4,
    Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base::ReConfigure___SamraksheMoteNetDeviceStatus__SZARRAY_U1,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_eMote_Net =
{
    "Samraksh_eMote_Net", 
    0x357DDFC6,
    method_lookup
};

static const CLR_RT_DriverInterruptMethods g_CLR_Radio_802_15_4_DriverMethods =
{
  Initialize_Net_Driver,
  EnableDisable_Net_Driver,
  Cleanup_Net_Driver
};

static const CLR_RT_DriverInterruptMethods g_CLR_CSMA_MAC_DriverMethods =
{
  Initialize_Net_Driver,
  EnableDisable_Net_Driver,
  Cleanup_Net_Driver
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_RadioCallback_802_15_4  =
{
    "RadioCallback_802_15_4",
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_CLR_Radio_802_15_4_DriverMethods
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_CSMACallback  =
{
    "CSMACallback",
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_CLR_CSMA_MAC_DriverMethods
};


