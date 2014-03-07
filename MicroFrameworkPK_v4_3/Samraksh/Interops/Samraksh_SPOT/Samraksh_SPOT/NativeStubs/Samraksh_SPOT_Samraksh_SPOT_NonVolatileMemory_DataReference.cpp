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
#include "Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataReference.h"
#include <Samraksh\DataStore\Datastore.h>

using namespace Samraksh::SPOT::NonVolatileMemory;

extern Data_Store g_dataStoreObject;
static UINT32 dataID;

INT32 DataReference::LookupData( CLR_RT_HeapBlock* pMngObj, UINT32 dataId, HRESULT &hr )
{
	return (INT32)g_dataStoreObject.getAddress(dataId);
}

INT32 DataReference::LookupDataType( CLR_RT_HeapBlock* pMngObj, UINT32 dataId, HRESULT &hr )
{
	return (INT32)g_dataStoreObject.getDataType(dataId);
}

INT32 DataReference::LookupSize( CLR_RT_HeapBlock* pMngObj, UINT32 dataId, HRESULT &hr )
{
    return (INT32)g_dataStoreObject.getAllocationSize(dataId);
}

INT8 DataReference::DeleteData( CLR_RT_HeapBlock* pMngObj, UINT32 dataId, HRESULT &hr )
{
    g_dataStoreObject.deleteRecord(dataId);
    if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return true;
	else
		return false;
}

INT32 DataReference::CreateData( CLR_RT_HeapBlock* pMngObj, UINT32 numBytes, UINT8 dataType, HRESULT &hr )
{
	if(dataID == 0)
		dataID = g_dataStoreObject.getRecentRecordID();

    dataID++;
	return  (int)g_dataStoreObject.createRecord(dataID, numBytes, dataType);
    //return  (int)g_dataStoreObject.createRecord(dataID, numBytes, 0);
}

INT32 DataReference::CreateData( CLR_RT_HeapBlock* pMngObj, UINT32 numBytes, UINT16 dataType, HRESULT &hr )
{
	if(dataID == 0)
		dataID = g_dataStoreObject.getRecentRecordID();

    dataID++;
	return  (int)g_dataStoreObject.createRecord(dataID, numBytes, dataType);
}

INT32 DataReference::CreateData( CLR_RT_HeapBlock* pMngObj, UINT32 numBytes, UINT32 dataType, HRESULT &hr )
{
	if(dataID == 0)
		dataID = g_dataStoreObject.getRecentRecordID();

	dataID++;
	return  (int)g_dataStoreObject.createRecord(dataID, numBytes, dataType);
}

UINT32 DataReference::GetDataID( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	if(dataID == 0)
		dataID = g_dataStoreObject.getRecentRecordID();

    return dataID;
}

INT8 DataReference::DisposeNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, UINT32 dataId, HRESULT &hr )
{
    return g_dataStoreObject.deleteRecord(dataId);
}

