//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "SamrakshEmoteDotNow.h"
#include "SamrakshEmoteDotNow_Samraksh_eMote_DotNow_SDInternal.h"

using namespace Samraksh::eMote::DotNow;


HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_SDInternal::InternalInitialize___STATIC__SamraksheMoteDotNowDeviceStatus( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        INT32 retVal = SDInternal::InternalInitialize( hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_SDInternal::InternalWrite___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U1__U2__U2__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 0, param0 ) );

        UINT16 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 1, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 2, param2 ) );

        UINT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 3, param3 ) );

        INT32 retVal = SDInternal::InternalWrite( param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_SDInternal::InternalRead___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U1__U2__U2__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 0, param0 ) );

        UINT16 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 1, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 2, param2 ) );

        UINT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 3, param3 ) );

        INT32 retVal = SDInternal::InternalRead( param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_SDInternal::InternalTotalSize___STATIC__U8( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT64 retVal = SDInternal::InternalTotalSize( hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT64( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_SDInternal::InternalTotalFreeSpace___STATIC__U8( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT64 retVal = SDInternal::InternalTotalFreeSpace( hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT64( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
