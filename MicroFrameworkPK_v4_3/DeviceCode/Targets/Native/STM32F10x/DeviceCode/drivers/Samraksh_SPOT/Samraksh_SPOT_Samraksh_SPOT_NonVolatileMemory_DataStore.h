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


#ifndef _SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_DATASTORE_H_
#define _SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_DATASTORE_H_

namespace Samraksh
{
    namespace SPOT
    {
        namespace NonVolatileMemory
        {
            struct DataStore
            {
                // Helper Functions to access fields of managed object
                static UINT32& Get_m_Size( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataStore::FIELD__m_Size ); }

                static INT32& Get_storageType( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataStore::FIELD__storageType ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static float GetUsedKBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static float GetFreeKBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT32 GetNumberOfDataRecords( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT8 GetReadAllRecordIDs( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT32 Read( CLR_RT_HeapBlock* pMngObj, UINT32 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );
                static UINT16 ReadAllDataIDs( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT32 param0, HRESULT &hr );
                static INT8 Delete( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT32 Write( CLR_RT_HeapBlock* pMngObj, UINT32 param0, CLR_RT_TypedArray_UINT8 param1, UINT32 param2, INT32 param3, HRESULT &hr );
                static INT8 DeleteAll( HRESULT &hr );
                static INT8 DataStoreGC( HRESULT &hr );
                static INT8 GetReadWriteStatus( HRESULT &hr );
                static INT32 GetLastErrorStatus( HRESULT &hr );
            };
        }
    }
}
#endif  //_SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_DATASTORE_H_
