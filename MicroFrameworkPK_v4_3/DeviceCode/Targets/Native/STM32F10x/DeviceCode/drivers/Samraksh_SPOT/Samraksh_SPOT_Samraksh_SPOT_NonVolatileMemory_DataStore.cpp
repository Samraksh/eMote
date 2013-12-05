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
#include <Samraksh\DataStore\Datastore.h>

using namespace Samraksh::SPOT::NonVolatileMemory;

extern Data_Store g_dataStoreObject;

INT32 DataStore::CreateRecord( CLR_RT_HeapBlock* pMngObj, UINT32 rId, UINT32 numBytes, HRESULT &hr )
{
	return  (int)g_dataStoreObject.createRecord(rId, numBytes);
}

INT8 DataStore::CreateDataStore( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return g_dataStoreObject.init();
}

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

INT8 DataStore::GetReadAllRecordIDs( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT32 recordIdArray, HRESULT &hr )
{
    INT32 *managedBuffer = recordIdArray.GetBuffer();
	g_dataStoreObject.getRecordIDAfterPersistence((UINT32 *) managedBuffer);
	if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return true;
	else
		return false;
}

UINT32 DataStore::GetNumberOfDataRecords( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    UINT32 retVal = 0; 
    return retVal;
}

INT32 DataStore::Read( CLR_RT_HeapBlock* pMngObj, UINT32 srcAddress, CLR_RT_TypedArray_UINT8 readBuffer, INT32 storageType, HRESULT &hr )
{
    return g_dataStoreObject.readRawData((void *) srcAddress, (void *) readBuffer.GetBuffer(), readBuffer.GetSize());
}

INT8 DataStore::Delete( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT32 DataStore::Write( CLR_RT_HeapBlock* pMngObj, UINT32 destAddress, CLR_RT_TypedArray_UINT8 data, UINT32 numBytes, INT32 storageType, HRESULT &hr )
{
   return g_dataStoreObject.writeRawData((void*)destAddress, (void*) data.GetBuffer(), numBytes);
}

INT32 DataStore::DeleteAll( HRESULT &hr )
{
    return g_dataStoreObject.DeleteAll();
}

INT32 DataStore::DataStoreGC( HRESULT &hr )
{
    return g_dataStoreObject.DataStoreGC();
}

INT8 DataStore::GetReadWriteStatus( HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT32 DataStore::GetLastErrorStatus( HRESULT &hr )
{
    return g_dataStoreObject.getLastError();
}

