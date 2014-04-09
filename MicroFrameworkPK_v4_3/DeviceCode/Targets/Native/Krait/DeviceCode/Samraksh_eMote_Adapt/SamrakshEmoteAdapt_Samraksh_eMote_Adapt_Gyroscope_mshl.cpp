//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "SamrakshEmoteAdapt.h"
#include "SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope.h"

using namespace Samraksh::eMote::Adapt;


HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::Initialize___BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT8 retVal = Gyroscope::Initialize( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::Reset___BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT8 retVal = Gyroscope::Reset( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::SelfTest___BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT8 retVal = Gyroscope::SelfTest( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::GetX___I2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT16 retVal = Gyroscope::GetX( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT16( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::GetY___I2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT16 retVal = Gyroscope::GetY( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT16( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::GetZ___I2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT16 retVal = Gyroscope::GetZ( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT16( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::GetTemperature___I2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT16 retVal = Gyroscope::GetTemperature( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT16( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::ReadRegister___U1__U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 1, param0 ) );

        UINT8 retVal = Gyroscope::ReadRegister( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::WriteRegister___BOOLEAN__U1__U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 1, param0 ) );

        UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8( stack, 2, param1 ) );

        INT8 retVal = Gyroscope::WriteRegister( pMngObj,  param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::AdvancedConfiguration___BOOLEAN__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param0 ) );

        INT8 retVal = Gyroscope::AdvancedConfiguration( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::SetPowerMode___BOOLEAN__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param0 ) );

        INT8 retVal = Gyroscope::SetPowerMode( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::SetOffsets___BOOLEAN__I2__I2__I2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        INT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT16( stack, 1, param0 ) );

        INT16 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT16( stack, 2, param1 ) );

        INT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT16( stack, 3, param2 ) );

        INT8 retVal = Gyroscope::SetOffsets( pMngObj,  param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::SampleConfiguration___BOOLEAN__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param0 ) );

        INT8 retVal = Gyroscope::SampleConfiguration( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope::GetAllData___BOOLEAN__SZARRAY_I2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_INT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT16_ARRAY( stack, 1, param0 ) );

        INT8 retVal = Gyroscope::GetAllData( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
