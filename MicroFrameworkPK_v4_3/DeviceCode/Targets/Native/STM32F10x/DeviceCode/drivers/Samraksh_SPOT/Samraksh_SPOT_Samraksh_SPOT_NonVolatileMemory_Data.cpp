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
#include "Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_Data.h"
#include <Samraksh\DataStore\Datastore.h>

using namespace Samraksh::SPOT::NonVolatileMemory;

extern Data_Store g_dataStoreObject;

INT32 Data::LookupData( CLR_RT_HeapBlock* pMngObj, UINT32 dataId, HRESULT &hr )
{
	return (INT32)g_dataStoreObject.getAddress(dataId);
}

INT8 Data::DeleteData( CLR_RT_HeapBlock* pMngObj, UINT32 dataId, HRESULT &hr )
{
    g_dataStoreObject.deleteRecord(dataId);
    if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_NONE)
		return true;
	else
		return false;
}

UINT32 Data::ConstructNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, UINT32 param0, UINT32 param1, HRESULT &hr )
{
    UINT32 retVal = 0; 
    return retVal;
}

INT8 Data::DisposeNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, UINT32 dataId, HRESULT &hr )
{
    return g_dataStoreObject.deleteRecord(dataId);
}

