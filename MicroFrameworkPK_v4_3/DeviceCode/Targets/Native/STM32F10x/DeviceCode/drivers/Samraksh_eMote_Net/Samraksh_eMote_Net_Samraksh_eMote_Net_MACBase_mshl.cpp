//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "Samraksh_eMote_Net.h"
#include "Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase.h"

using namespace Samraksh::eMote::Net;


HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::UnInitialize___SamraksheMoteNetDeviceStatus( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT32 retVal = MACBase::UnInitialize( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::RemovePacket___SamraksheMoteNetDeviceStatus__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param0 ) );

        INT32 retVal = MACBase::RemovePacket( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetPendingPacketCount_Receive___U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT8 retVal = MACBase::GetPendingPacketCount_Receive( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetPendingPacketCount_Send___U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT8 retVal = MACBase::GetPendingPacketCount_Send( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetMACType___U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT8 retVal = MACBase::GetMACType( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::SetRadioAddress___SamraksheMoteNetDeviceStatus__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 1, param0 ) );

        INT32 retVal = MACBase::SetRadioAddress( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetRadioAddress___U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT16 retVal = MACBase::GetRadioAddress( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT16( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::Send___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 1, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 2, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param2 ) );

        UINT16 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 4, param3 ) );

        INT32 retVal = MACBase::Send( pMngObj,  param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::InternalInitialize___SamraksheMoteNetDeviceStatus__SZARRAY_U1__U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param0 ) );

        UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 2, param1 ) );

        INT32 retVal = MACBase::InternalInitialize( pMngObj,  param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::InternalReConfigure___SamraksheMoteNetDeviceStatus__SZARRAY_U1__U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param0 ) );

        UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 2, param1 ) );

        INT32 retVal = MACBase::InternalReConfigure( pMngObj,  param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetNextPacket___SamraksheMoteNetDeviceStatus__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param0 ) );

        INT32 retVal = MACBase::GetNextPacket( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetNeighborInternal___SamraksheMoteNetDeviceStatus__U2__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 1, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 2, param1 ) );

        INT32 retVal = MACBase::GetNeighborInternal( pMngObj,  param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::GetNeighborListInternal___SamraksheMoteNetDeviceStatus__SZARRAY_U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 1, param0 ) );

        INT32 retVal = MACBase::GetNeighborListInternal( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::SendTimeStamped___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 1, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 2, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param2 ) );

        UINT16 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 4, param3 ) );

        INT32 retVal = MACBase::SendTimeStamped( pMngObj,  param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::SendTimeStamped___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 1, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 2, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param2 ) );

        UINT16 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 4, param3 ) );

        UINT32 param4;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 5, param4 ) );

        INT32 retVal = MACBase::SendTimeStamped( pMngObj,  param0, param1, param2, param3, param4, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
