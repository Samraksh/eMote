//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "SamrakshEmoteDotNow.h"
#include "SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD.h"

using namespace Samraksh::eMote::DotNow;


HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::Initialize___BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT8 retVal = EmoteLCD::Initialize( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::Uninitialize___BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT8 retVal = EmoteLCD::Uninitialize( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::Write___BOOLEAN__SamraksheMoteDotNowLCD__SamraksheMoteDotNowLCD__SamraksheMoteDotNowLCD__SamraksheMoteDotNowLCD( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 3, param2 ) );

        INT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 4, param3 ) );

        INT8 retVal = EmoteLCD::Write( pMngObj,  param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::SetDP___BOOLEAN__BOOLEAN__BOOLEAN__BOOLEAN__BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT8( stack, 1, param0 ) );

        INT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT8( stack, 2, param1 ) );

        INT8 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT8( stack, 3, param2 ) );

        INT8 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT8( stack, 4, param3 ) );

        INT8 retVal = EmoteLCD::SetDP( pMngObj,  param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::WriteN___BOOLEAN__I4__SamraksheMoteDotNowLCD( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param1 ) );

        INT8 retVal = EmoteLCD::WriteN( pMngObj,  param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::WriteRawBytes___BOOLEAN__I4__I4__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 3, param2 ) );

        INT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 4, param3 ) );

        INT8 retVal = EmoteLCD::WriteRawBytes( pMngObj,  param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::Blink___BOOLEAN__SamraksheMoteDotNowBlink( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param0 ) );

        INT8 retVal = EmoteLCD::Blink( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::Clear___BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT8 retVal = EmoteLCD::Clear( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}