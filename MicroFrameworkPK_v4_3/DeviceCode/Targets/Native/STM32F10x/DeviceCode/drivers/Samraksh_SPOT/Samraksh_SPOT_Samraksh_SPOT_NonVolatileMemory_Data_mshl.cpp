//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "Samraksh_SPOT.h"
#include "Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data.h"

using namespace Samraksh::SPOT::NonVolatileMemory;


HRESULT Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data::LookupData___I4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param0 ) );

        INT32 retVal = Data::LookupData( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data::DeleteData___BOOLEAN__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param0 ) );

        INT8 retVal = Data::DeleteData( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data::ConstructNativeMemoryPointer___U4__U4__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param0 ) );

        UINT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 2, param1 ) );

        UINT32 retVal = Data::ConstructNativeMemoryPointer( pMngObj,  param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UINT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data::DisposeNativeMemoryPointer___BOOLEAN__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param0 ) );

        INT8 retVal = Data::DisposeNativeMemoryPointer( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
