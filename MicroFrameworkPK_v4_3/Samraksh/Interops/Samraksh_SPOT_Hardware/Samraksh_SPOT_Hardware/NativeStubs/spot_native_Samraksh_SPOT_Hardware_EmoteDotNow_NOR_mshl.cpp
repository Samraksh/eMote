//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "spot_native.h"
#include "spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_NOR.h"

using namespace Samraksh::SPOT::Hardware::EmoteDotNow;


HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_NOR::InternalInitialize___STATIC__BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        INT8 retVal = NOR::InternalInitialize( hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_NOR::InternalWrite___STATIC__BOOLEAN__SZARRAY_U2__U4__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 0, param0 ) );

        UINT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 2, param2 ) );

        INT8 retVal = NOR::InternalWrite( param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_NOR::InternalWrite___STATIC__BOOLEAN__SZARRAY_U2__U4__U2__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 0, param0 ) );

        UINT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 2, param2 ) );

        UINT16 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param3 ) );

        INT8 retVal = NOR::InternalWrite( param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_NOR::InternalRead___STATIC__BOOLEAN__SZARRAY_U2__U4__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 0, param0 ) );

        UINT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 2, param2 ) );

        INT8 retVal = NOR::InternalRead( param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
