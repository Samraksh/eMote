//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "SamrakshEmote.h"


static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    NULL,
    NULL,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__AcousticDetection::DetectionCalculation___BOOLEAN__SZARRAY_U2__I4__SZARRAY_R8__BOOLEAN,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__AcousticDetection::SetDetectionParameters___BOOLEAN__I4__I4,
    NULL,
    NULL,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection::Initialize___BOOLEAN,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection::Uninitialize___BOOLEAN,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection::DetectionCalculation___BOOLEAN__SZARRAY_U2__SZARRAY_U2__SZARRAY_U2__I4__SZARRAY_I2,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection::DetectionCalculation___BOOLEAN__SZARRAY_U2__SZARRAY_U2__I4__SZARRAY_I2,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection::SetDetectionParameters___BOOLEAN__SamraksheMoteRADARNOISECONTROL__R8__R8__U2__U2__U2__U2,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection::GetBackgroundNoiseLevel___I4__SamraksheMoteRADARNOISEREQUEST,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection::ResetBackgroundNoiseTracking___BOOLEAN,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection::GetIQRejectionLevel___I4,
    Library_SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection::GetLastUnwrap___I4__SamraksheMoteRADARNOISEREQUEST,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::GetDataReference___I4__U4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::GetDataLocation___I4__U4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::LookupDataType___I4__U4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::LookupDataSize___I4__U4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::DeleteData___BOOLEAN__U4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::CreateData___I4__U4__U1,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::CreateData___I4__U4__U2,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::CreateData___I4__U4__U4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::GetDataID___U4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::GetLastDatastoreStatus___I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference::DisposeNativeMemoryPointer___BOOLEAN__U4,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::CreateDataStore___I4__BOOLEAN,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::GetMaxAllocationSize___I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::GetTotalSpace___I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::GetUsedBytes___I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::GetFreeBytes___I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::GetReadAllDataIds___I4__SZARRAY_I4__I4__I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::GetCountOfDataIds___I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::DeleteAll___I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::EraseAllBlocks___I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::DataStoreGC___BOOLEAN,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::GetReadWriteStatus___BOOLEAN,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::GetLastDatastoreStatus___I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::WriteRaw___I4__U4__SZARRAY_U1__U4__U4__U1__I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::WriteRaw___I4__U4__SZARRAY_U1__U4__U4__U2__I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::WriteRaw___I4__U4__SZARRAY_U1__U4__U4__U4__I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::ReadRaw___I4__U4__SZARRAY_U1__U4__U4__U1__I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::ReadRaw___I4__U4__SZARRAY_U1__U4__U4__U2__I4,
    Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore::ReadRaw___I4__U4__SZARRAY_U1__U4__U4__U4__I4,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_eMote =
{
    "Samraksh_eMote", 
    0xAA8C698D,
    method_lookup
};

