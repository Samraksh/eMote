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


#include "SamrakshEmote.h"
#include "SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore.h"
#include <Samraksh\DataStore\Datastore.h>

using namespace Samraksh::eMote::NonVolatileMemory;
//AnanthAtSamraksh - to be kept in sync with the one in NonVolatileMemory.cs
typedef enum DATASTORE_RETURN_STATUS
{
	Success = 0,
	Failure = -1,
	InvalidArgument = -2,
	InvalidReference = -3,
	DataStoreNotInitialized = -4
	//AlreadyExists,
	//InvalidPointer
};

extern Data_Store g_dataStoreObject;

INT8 DataStore::CreateDataStore( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    g_dataStoreObject.init();

    if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT32 DataStore::GetMaxAllocationSize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return g_dataStoreObject.maxAllocationSize();
}

INT32 DataStore::GetTotalSpace( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return g_dataStoreObject.returnTotalSpace();
}

INT32 DataStore::GetUsedBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return g_dataStoreObject.returnLogPoint();
}

INT32 DataStore::GetFreeBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return g_dataStoreObject.returnFreeSpace();
}

INT32 DataStore::GetReadAllDataIds( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT32 dataIdArray, INT32 arrayLength, INT32 dataIdOffset, HRESULT &hr )
{
    INT32 *managedBuffer = dataIdArray.GetBuffer();
	g_dataStoreObject.getRecordIDAfterPersistence((UINT32 *) managedBuffer, arrayLength, dataIdOffset);

	if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT32 DataStore::GetCountOfDataIds( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return (INT32)g_dataStoreObject.getCountOfRecordIds();
}

INT32 DataStore::DeleteAll( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    g_dataStoreObject.DeleteAll();

    if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT32 DataStore::EraseAllBlocks( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	g_dataStoreObject.EraseAllBlocks();

	if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT8 DataStore::DataStoreGC( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    g_dataStoreObject.DataStoreGC();

    if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT8 DataStore::GetReadWriteStatus( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT32 DataStore::GetLastErrorStatus( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return g_dataStoreObject.getLastError();
}

INT32 DataStore::Write( CLR_RT_HeapBlock* pMngObj, UINT32 destAddress, CLR_RT_TypedArray_UINT8 data, UINT32 offset, UINT32 numBytes, UINT8 dataType, INT32 storageType, HRESULT &hr )
{
	g_dataStoreObject.writeRawData((void*)destAddress, (void*) data.GetBuffer(), offset * sizeof(UINT8), numBytes);

	if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT32 DataStore::Write( CLR_RT_HeapBlock* pMngObj, UINT32 destAddress, CLR_RT_TypedArray_UINT8 data, UINT32 offset, UINT32 numBytes, UINT16 dataType, INT32 storageType, HRESULT &hr )
{
	g_dataStoreObject.writeRawData((void*)destAddress, (void*) data.GetBuffer(), offset * sizeof(UINT16), numBytes);

	if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT32 DataStore::Write( CLR_RT_HeapBlock* pMngObj, UINT32 destAddress, CLR_RT_TypedArray_UINT8 data, UINT32 offset, UINT32 numBytes, UINT32 dataType, INT32 storageType, HRESULT &hr )
{
    g_dataStoreObject.writeRawData((void*)destAddress, (void*) data.GetBuffer(), offset * sizeof(UINT32), numBytes);

    if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT32 DataStore::Read( CLR_RT_HeapBlock* pMngObj, UINT32 srcAddress, CLR_RT_TypedArray_UINT8 readBuffer, UINT32 offset, UINT32 numBytes, UINT8 dataType, INT32 storageType, HRESULT &hr )
{
    g_dataStoreObject.readRawData((void *) srcAddress, (void *) readBuffer.GetBuffer(), offset * sizeof(UINT8), numBytes);
	//readBuffer = readBuffer[offset * sizeof(dataType)];
    if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT32 DataStore::Read( CLR_RT_HeapBlock* pMngObj, UINT32 srcAddress, CLR_RT_TypedArray_UINT8 readBuffer, UINT32 offset, UINT32 numBytes, UINT16 dataType, INT32 storageType, HRESULT &hr )
{
	g_dataStoreObject.readRawData((void *) srcAddress, (void *) readBuffer.GetBuffer(), offset * sizeof(UINT16), numBytes);
	//readBuffer = readBuffer[offset * sizeof(dataType)];
	if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT32 DataStore::Read( CLR_RT_HeapBlock* pMngObj, UINT32 srcAddress, CLR_RT_TypedArray_UINT8 readBuffer, UINT32 offset, UINT32 numBytes, UINT32 dataType, INT32 storageType, HRESULT &hr )
{
	g_dataStoreObject.readRawData((void *) srcAddress, (void *) readBuffer.GetBuffer(), offset * sizeof(UINT32), numBytes);
	//readBuffer = readBuffer[offset * sizeof(dataType)];
	if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

