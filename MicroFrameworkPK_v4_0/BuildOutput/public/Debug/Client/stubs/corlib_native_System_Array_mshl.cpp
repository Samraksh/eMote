//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "corlib_native.h"
#include "corlib_native_System_Array.h"

using namespace System;


HRESULT Library_corlib_native_System_Array::System_Collections_IList_get_Item___OBJECT__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param0 ) );

        UNSUPPORTED_TYPE retVal = Array::System_Collections_IList_get_Item( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UNSUPPORTED_TYPE( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_corlib_native_System_Array::System_Collections_IList_set_Item___VOID__I4__OBJECT( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param0 ) );

        UNSUPPORTED_TYPE param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 2, param1 ) );

        Array::System_Collections_IList_set_Item( pMngObj,  param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_corlib_native_System_Array::get_Length___I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT32 retVal = Array::get_Length( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_corlib_native_System_Array::CreateInstance___STATIC__SystemArray__SystemType__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        UNSUPPORTED_TYPE param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 1, param1 ) );

        UNSUPPORTED_TYPE retVal = Array::CreateInstance( param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UNSUPPORTED_TYPE( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_corlib_native_System_Array::Copy___STATIC__VOID__SystemArray__I4__SystemArray__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        UNSUPPORTED_TYPE param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        UNSUPPORTED_TYPE param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 3, param3 ) );

        UNSUPPORTED_TYPE param4;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 4, param4 ) );

        Array::Copy( param0, param1, param2, param3, param4, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_corlib_native_System_Array::Clear___STATIC__VOID__SystemArray__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        UNSUPPORTED_TYPE param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        Array::Clear( param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_corlib_native_System_Array::TrySZIndexOf___STATIC__BOOLEAN__SystemArray__I4__I4__OBJECT__BYREF_I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        UNSUPPORTED_TYPE param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        INT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 3, param3 ) );

        UNSUPPORTED_TYPE param4;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 4, param4 ) );

        INT8 retVal = Array::TrySZIndexOf( param0, param1, param2, param3, param4, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
