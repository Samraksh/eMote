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

//AnanthAtSamraksh - to be kept in sync with the one in NonVolatileMemory.cs
typedef enum DATASTORE_RETURN_STATUS
{
	Success = 0,
	Failure = -1,
	InvalidArgument = -2,
	InvalidReference = -3
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

float DataStore::GetMaxAllocationSize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return g_dataStoreObject.maxAllocationSize();
}

float DataStore::GetTotalSpace( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return g_dataStoreObject.returnTotalSpace();
}

float DataStore::GetUsedBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return g_dataStoreObject.returnLogPoint();
}

float DataStore::GetFreeBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return g_dataStoreObject.returnFreeSpace();
}

INT8 DataStore::GetReadAllDataIds( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT32 dataIdArray, INT32 arrayLength, INT32 dataIdOffset, HRESULT &hr )
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

/*INT8 DataStore::Write( CLR_RT_HeapBlock* pMngObj, UINT32 destAddress, CLR_RT_TypedArray_UINT8 data, UINT32 numBytes, INT32 storageType, HRESULT &hr )
{
	g_dataStoreObject.writeRawData((void*)destAddress, (void*) data.GetBuffer(), numBytes);
	if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return true;
	else
		return false;
}*/

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

INT8 DataStore::DeleteAll( HRESULT &hr )
{
    g_dataStoreObject.DeleteAll();

    if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT8 DataStore::EraseAllBlocks( HRESULT &hr )
{
	g_dataStoreObject.EraseAllBlocks();

	if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
}

INT8 DataStore::DataStoreGC( HRESULT &hr )
{
    g_dataStoreObject.DataStoreGC();

    if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return Success;
	else if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_INVALID_GIVEN_ADDR)
		return InvalidReference;
	else
		return Failure;
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

