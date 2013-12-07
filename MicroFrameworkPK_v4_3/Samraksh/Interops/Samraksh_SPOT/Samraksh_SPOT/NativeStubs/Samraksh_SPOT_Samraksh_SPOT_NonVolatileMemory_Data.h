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


#ifndef _SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_DATA_H_
#define _SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_DATA_H_

namespace Samraksh
{
    namespace SPOT
    {
        namespace NonVolatileMemory
        {
            struct Data
            {
                // Helper Functions to access fields of managed object
                static UINT32& Get_m_Size( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data::FIELD__m_Size ); }

                static UNSUPPORTED_TYPE& Get_buffer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data::FIELD__buffer ); }

                static INT32& Get_dataPointer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data::FIELD__dataPointer ); }

                static UNSUPPORTED_TYPE& Get_dataType( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data::FIELD__dataType ); }

                static UNSUPPORTED_TYPE& Get_dStore( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data::FIELD__dStore ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT32 LookupData( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT8 DeleteData( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static UINT32 ConstructNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, UINT32 param0, UINT32 param1, HRESULT &hr );
                static INT8 DisposeNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
            };
        }
    }
}
#endif  //_SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_DATA_H_
