//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "spot_native.h"
#include "spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal.h"

using namespace Samraksh::SPOT::Hardware::EmoteDotNow;


HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal::Init___STATIC__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        INT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 0, param0 ) );

        INT32 retVal = ADCInternal::Init( param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal::Read___STATIC__R8__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        INT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 0, param0 ) );

        double retVal = ADCInternal::Read( param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_double( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal::ConfigureBatchMode___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__I4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        UINT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 2, param2 ) );

        UINT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 3, param3 ) );

        INT32 retVal = ADCInternal::ConfigureBatchMode( param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal::ConfigureContinuousMode___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__I4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        UINT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 2, param2 ) );

        UINT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 3, param3 ) );

        INT32 retVal = ADCInternal::ConfigureContinuousMode( param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal::ConfigureContinuousModeDualChannel___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__SZARRAY_U2__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 0, param0 ) );

        CLR_RT_TypedArray_UINT16 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 1, param1 ) );

        UINT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 2, param2 ) );

        UINT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 3, param3 ) );

        INT32 retVal = ADCInternal::ConfigureContinuousModeDualChannel( param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal::ConfigureBatchModeDualChannel___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__SZARRAY_U2__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 0, param0 ) );

        CLR_RT_TypedArray_UINT16 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 1, param1 ) );

        UINT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 2, param2 ) );

        UINT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 3, param3 ) );

        INT32 retVal = ADCInternal::ConfigureBatchModeDualChannel( param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal::ConfigureContinuousModeWithThresholding___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__I4__U4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        UINT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 2, param2 ) );

        UINT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 3, param3 ) );

        UINT32 param4;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 4, param4 ) );

        INT32 retVal = ADCInternal::ConfigureContinuousModeWithThresholding( param0, param1, param2, param3, param4, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal::ConfigureBatchModeWithThresholding___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__I4__U4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        UINT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 2, param2 ) );

        UINT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 3, param3 ) );

        UINT32 param4;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 4, param4 ) );

        INT32 retVal = ADCInternal::ConfigureBatchModeWithThresholding( param0, param1, param2, param3, param4, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal::DualChannelRead___STATIC__BOOLEAN__SZARRAY_U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 0, param0 ) );

        INT8 retVal = ADCInternal::DualChannelRead( param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal::StopSampling___STATIC__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        INT32 retVal = ADCInternal::StopSampling( hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
