//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#ifndef _SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_H_
#define _SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_H_

namespace Samraksh
{
    namespace SPOT
    {
        struct NonVolatileMemory
        {
            // Helper Functions to access fields of managed object
            static UINT32& Get_m_Size( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory::FIELD__m_Size ); }

            static UINT32& Get_recordId( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory::FIELD__recordId ); }

            static UNSUPPORTED_TYPE& Get_buffer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory::FIELD__buffer ); }

            // Declaration of stubs. These functions are implemented by Interop code developers
            static UINT32 ConstructNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, UINT32 param0, UINT32 param1, HRESULT &hr );
            static float GetUsedKBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static float GetFreeKBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static INT32 Read( CLR_RT_HeapBlock* pMngObj, UINT32 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );
            static UINT32 Write( CLR_RT_HeapBlock* pMngObj, UINT32 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );
            static INT8 Delete( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
            static INT8 DeleteAll( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static INT8 DisposeNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
        };
    }
}
#endif  //_SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_H_
