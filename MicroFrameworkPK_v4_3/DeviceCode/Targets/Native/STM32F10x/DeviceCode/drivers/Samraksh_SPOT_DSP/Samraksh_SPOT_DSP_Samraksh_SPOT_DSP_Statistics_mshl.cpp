//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "Samraksh_SPOT_DSP.h"
#include "Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics.h"

using namespace Samraksh::SPOT::DSP;


HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Sum___U8__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param0 ) );

        UINT64 retVal = Statistics::Sum( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT64( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Sum___U8__SZARRAY_U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 1, param0 ) );

        UINT64 retVal = Statistics::Sum( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT64( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Sum___U8__SZARRAY_U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ARRAY( stack, 1, param0 ) );

        UINT64 retVal = Statistics::Sum( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT64( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Sum___U8__SZARRAY_U8( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT64 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT64_ARRAY( stack, 1, param0 ) );

        UINT64 retVal = Statistics::Sum( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT64( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Median___U1__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param0 ) );

        UINT8 retVal = Statistics::Median( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Median___U2__SZARRAY_U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 1, param0 ) );

        UINT16 retVal = Statistics::Median( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT16( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Median___U4__SZARRAY_U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ARRAY( stack, 1, param0 ) );

        UINT32 retVal = Statistics::Median( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Median___U8__SZARRAY_U8( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT64 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT64_ARRAY( stack, 1, param0 ) );

        UINT64 retVal = Statistics::Median( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT64( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Mode___U1__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param0 ) );

        UINT8 retVal = Statistics::Mode( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Mode___U2__SZARRAY_U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 1, param0 ) );

        UINT16 retVal = Statistics::Mode( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT16( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Mode___U4__SZARRAY_U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ARRAY( stack, 1, param0 ) );

        UINT32 retVal = Statistics::Mode( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Mode___U8__SZARRAY_U8( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT64 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT64_ARRAY( stack, 1, param0 ) );

        UINT64 retVal = Statistics::Mode( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT64( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Variance___R8__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT8 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param0 ) );

        double retVal = Statistics::Variance( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_double( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Variance___R8__SZARRAY_U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16_ARRAY( stack, 1, param0 ) );

        double retVal = Statistics::Variance( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_double( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Variance___R8__SZARRAY_U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ARRAY( stack, 1, param0 ) );

        double retVal = Statistics::Variance( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_double( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_DSP_Samraksh_SPOT_DSP_Statistics::Variance___R8__SZARRAY_U8( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        CLR_RT_TypedArray_UINT64 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT64_ARRAY( stack, 1, param0 ) );

        double retVal = Statistics::Variance( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_double( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
