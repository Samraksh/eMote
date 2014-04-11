//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#ifndef _SAMRAKSH_SPOT_H_
#define _SAMRAKSH_SPOT_H_

#include <TinyCLR_Interop.h>
struct Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataStore
{
    static const int FIELD_STATIC__DSInstance = 0;
    static const int FIELD_STATIC__syncObject = 1;
    static const int FIELD_STATIC__dataStoreInstances = 2;

    static const int FIELD__storageType = 1;

    TINYCLR_NATIVE_DECLARE(CreateDataStore___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(CreateData___I4__U4__U1);
    TINYCLR_NATIVE_DECLARE(CreateData___I4__U4__U2);
    TINYCLR_NATIVE_DECLARE(CreateData___I4__U4__U4);
    TINYCLR_NATIVE_DECLARE(GetDataID___U4);
    TINYCLR_NATIVE_DECLARE(GetMaxAllocationSize___R4);
    TINYCLR_NATIVE_DECLARE(GetTotalSpace___R4);
    TINYCLR_NATIVE_DECLARE(GetUsedBytes___R4);
    TINYCLR_NATIVE_DECLARE(GetFreeBytes___R4);
    TINYCLR_NATIVE_DECLARE(GetReadAllDataIds___BOOLEAN__SZARRAY_I4__I4__I4);
    TINYCLR_NATIVE_DECLARE(GetCountOfDataIds___U4);
    TINYCLR_NATIVE_DECLARE(Delete___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(Write___BOOLEAN__U4__SZARRAY_U1__U4__U4__U1__I4);
    TINYCLR_NATIVE_DECLARE(Write___BOOLEAN__U4__SZARRAY_U1__U4__U4__U2__I4);
    TINYCLR_NATIVE_DECLARE(Write___BOOLEAN__U4__SZARRAY_U1__U4__U4__U4__I4);
    TINYCLR_NATIVE_DECLARE(Read___BOOLEAN__U4__SZARRAY_U1__U4__U4__U1__I4);
    TINYCLR_NATIVE_DECLARE(Read___BOOLEAN__U4__SZARRAY_U1__U4__U4__U2__I4);
    TINYCLR_NATIVE_DECLARE(Read___BOOLEAN__U4__SZARRAY_U1__U4__U4__U4__I4);
    TINYCLR_NATIVE_DECLARE(DeleteAll___STATIC__BOOLEAN);
    TINYCLR_NATIVE_DECLARE(EraseAllBlocks___STATIC__BOOLEAN);
    TINYCLR_NATIVE_DECLARE(DataStoreGC___STATIC__BOOLEAN);
    TINYCLR_NATIVE_DECLARE(GetReadWriteStatus___STATIC__BOOLEAN);
    TINYCLR_NATIVE_DECLARE(GetLastErrorStatus___STATIC__I4);

    //--//

};

struct Library_Samraksh_SPOT_Samraksh_SPOT_GC
{
    TINYCLR_NATIVE_DECLARE(GetUsedKBytes___R4);
    TINYCLR_NATIVE_DECLARE(GetFreeKBytes___R4);
    TINYCLR_NATIVE_DECLARE(IsGCSamExtensionsEnabled___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(EnableIncrementalCleaning___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(EnableIncrementalCompaction___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(DisableIncrementalCleaning___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(DisableIncrementalCompaction___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(CleanBeforeSleep___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(CleanNow___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(CleanNow___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(AvgTimeForCleaning___U4);
    TINYCLR_NATIVE_DECLARE(AvgTimeForCompaction___U4);

    //--//

};

struct Library_Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataAllocation
{
    static const int FIELD__dataId = 1;
    static const int FIELD__m_Size = 2;
    static const int FIELD__dataPointer = 3;
    static const int FIELD__dataType = 4;
    static const int FIELD__dataTypeByte = 5;
    static const int FIELD__dataTypeUInt16 = 6;
    static const int FIELD__dataTypeUInt32 = 7;
    static const int FIELD__dStore = 8;

    TINYCLR_NATIVE_DECLARE(LookupData___I4__U4);
    TINYCLR_NATIVE_DECLARE(LookupDataType___U4__U4);
    TINYCLR_NATIVE_DECLARE(LookupSize___U4__U4);
    TINYCLR_NATIVE_DECLARE(DeleteData___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(ConstructNativeMemoryPointer___U4__U4__U4);
    TINYCLR_NATIVE_DECLARE(DisposeNativeMemoryPointer___BOOLEAN__U4);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_SPOT;

#endif  //_SAMRAKSH_SPOT_H_
