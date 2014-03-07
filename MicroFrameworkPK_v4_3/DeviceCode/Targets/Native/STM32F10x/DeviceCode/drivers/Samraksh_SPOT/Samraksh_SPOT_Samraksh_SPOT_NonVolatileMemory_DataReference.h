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


#ifndef _SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_DATAREFERENCE_H_
#define _SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_DATAREFERENCE_H_

namespace Samraksh
{
    namespace SPOT
    {
        namespace NonVolatileMemory
        {
            struct DataReference
            {
                // Helper Functions to access fields of managed object
                static UINT32& Get_dataId( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataReference::FIELD__dataId ); }

                static UINT32& Get_m_Size( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataReference::FIELD__m_Size ); }

                static UINT32& Get_dataReference( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataReference::FIELD__dataReference ); }

                static UNSUPPORTED_TYPE& Get_dataType( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataReference::FIELD__dataType ); }

                static UINT8& Get_dataTypeByte( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT8( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataReference::FIELD__dataTypeByte ); }

                static UINT16& Get_dataTypeUInt16( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT16( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataReference::FIELD__dataTypeUInt16 ); }

                static UINT32& Get_dataTypeUInt32( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataReference::FIELD__dataTypeUInt32 ); }

                static UNSUPPORTED_TYPE& Get_dStore( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataReference::FIELD__dStore ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT32 LookupData( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT32 LookupDataType( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT32 LookupSize( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT8 DeleteData( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT32 CreateData( CLR_RT_HeapBlock* pMngObj, UINT32 param0, UINT8 param1, HRESULT &hr );
                static INT32 CreateData( CLR_RT_HeapBlock* pMngObj, UINT32 param0, UINT16 param1, HRESULT &hr );
                static INT32 CreateData( CLR_RT_HeapBlock* pMngObj, UINT32 param0, UINT32 param1, HRESULT &hr );
                static UINT32 GetDataID( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT8 DisposeNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
            };
        }
    }
}
#endif  //_SAMRAKSH_SPOT_SAMRAKSH_SPOT_NONVOLATILEMEMORY_DATAREFERENCE_H_
