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


#include "Samraksh_SPOT.h"
#include "Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataStore.h"
#include <Samraksh/DataStore/Datastore.h>

extern Data_Store g_dataStoreObject;

using namespace Samraksh::SPOT::NonVolatileMemory;

float DataStore::GetUsedKBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    float retVal = 0; 
    return retVal;
}

float DataStore::GetFreeKBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    float retVal = 0; 
    return retVal;
}

UINT32 DataStore::GetNumberOfDataRecords( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    UINT32 retVal = 0; 
    return retVal;
}

INT8 DataStore::GetReadAllRecordIDs( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT32 DataStore::Read( CLR_RT_HeapBlock* pMngObj, UINT32 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

UINT16 DataStore::ReadAllDataIDs( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT32 param0, HRESULT &hr )
{
    UINT16 retVal = 0; 
    return retVal;
}

INT8 DataStore::Delete( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT32 DataStore::Write( CLR_RT_HeapBlock* pMngObj, UINT32 param0, CLR_RT_TypedArray_UINT8 param1, UINT32 param2, INT32 param3, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT8 DataStore::DeleteAll( HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 DataStore::DataStoreGC( HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 DataStore::GetReadWriteStatus( HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT32 DataStore::GetLastErrorStatus( HRESULT &hr )
{
    INT32 retVal = 0; 
    retVal = g_dataStoreObject.getLastError();
    return retVal;
}

