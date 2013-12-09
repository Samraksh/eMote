//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "Samraksh_SPOT_DSP.h"
#include "Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Transforms.h"

using namespace Samraksh::SPOT::DSP;


HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Transforms::FFT___BOOLEAN__SZARRAY_U1__SZARRAY_U1__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 2, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param2 ) );

        INT8 retVal = Transforms::FFT( pMngObj,  param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Transforms::FFT___BOOLEAN__SZARRAY_U2__SZARRAY_U2__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 1, param0 ) );

        CLR_RT_TypedArray_UINT16 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 2, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param2 ) );

        INT8 retVal = Transforms::FFT( pMngObj,  param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Transforms::FFT___BOOLEAN__SZARRAY_U4__SZARRAY_U4__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ARRAY( stack, 1, param0 ) );

        CLR_RT_TypedArray_UINT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ARRAY( stack, 2, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param2 ) );

        INT8 retVal = Transforms::FFT( pMngObj,  param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Transforms::FFT___BOOLEAN__SZARRAY_I2__SZARRAY_I2__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_INT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT16_ARRAY( stack, 1, param0 ) );

        CLR_RT_TypedArray_INT16 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT16_ARRAY( stack, 2, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param2 ) );

        INT8 retVal = Transforms::FFT( pMngObj,  param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Transforms::IFFT___BOOLEAN__SZARRAY_U1__SZARRAY_U1__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 2, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param2 ) );

        INT8 retVal = Transforms::IFFT( pMngObj,  param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Transforms::IFFT___BOOLEAN__SZARRAY_U2__SZARRAY_U2__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 1, param0 ) );

        CLR_RT_TypedArray_UINT16 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 2, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param2 ) );

        INT8 retVal = Transforms::IFFT( pMngObj,  param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Transforms::IFFT___BOOLEAN__SZARRAY_U4__SZARRAY_U4__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ARRAY( stack, 1, param0 ) );

        CLR_RT_TypedArray_UINT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ARRAY( stack, 2, param1 ) );

        UINT16 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 3, param2 ) );

        INT8 retVal = Transforms::IFFT( pMngObj,  param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
