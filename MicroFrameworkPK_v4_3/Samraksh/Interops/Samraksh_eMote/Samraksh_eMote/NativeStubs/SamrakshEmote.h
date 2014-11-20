//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#ifndef _SAMRAKSHEMOTE_H_
#define _SAMRAKSHEMOTE_H_

#include <TinyCLR_Interop.h>
struct Library_SamrakshEmote_Samraksh_eMote_Algorithm__AcousticDetection
{
    TINYCLR_NATIVE_DECLARE(DetectionCalculation___BOOLEAN__SZARRAY_U2__I4__SZARRAY_R8__BOOLEAN);
    TINYCLR_NATIVE_DECLARE(SetDetectionParameters___BOOLEAN__I4__I4);

    //--//

};

struct Library_SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection
{
    TINYCLR_NATIVE_DECLARE(DetectionCalculation___BOOLEAN__SZARRAY_U2__SZARRAY_U2__SZARRAY_U2__I4__SZARRAY_I2);
    TINYCLR_NATIVE_DECLARE(DetectionCalculation___BOOLEAN__SZARRAY_U2__SZARRAY_U2__I4__SZARRAY_I2);
    TINYCLR_NATIVE_DECLARE(SetDetectionParameters___BOOLEAN__U4__U4__I4);

    //--//

};

struct Library_SamrakshEmote_Samraksh_eMote_GC
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

struct Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataReference
{
    static const int FIELD__dStore = 1;
    static const int FIELD__dataId = 2;
    static const int FIELD__referenceSize = 3;
    static const int FIELD__dataReference = 4;
    static const int FIELD__referenceDataType = 5;
    static const int FIELD__dataType = 6;
    static const int FIELD__dataTypeByte = 7;
    static const int FIELD__dataTypeUInt16 = 8;
    static const int FIELD__dataTypeUInt32 = 9;

    TINYCLR_NATIVE_DECLARE(GetDataReference___I4__U4);
    TINYCLR_NATIVE_DECLARE(LookupDataType___I4__U4);
    TINYCLR_NATIVE_DECLARE(LookupDataSize___I4__U4);
    TINYCLR_NATIVE_DECLARE(DeleteData___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(CreateData___I4__U4__U1);
    TINYCLR_NATIVE_DECLARE(CreateData___I4__U4__U2);
    TINYCLR_NATIVE_DECLARE(CreateData___I4__U4__U4);
    TINYCLR_NATIVE_DECLARE(GetDataID___U4);
    TINYCLR_NATIVE_DECLARE(DisposeNativeMemoryPointer___BOOLEAN__U4);

    //--//

};

struct Library_SamrakshEmote_Samraksh_eMote_NonVolatileMemory_DataStore
{
    static const int FIELD_STATIC__DSInstance = 0;
    static const int FIELD_STATIC__syncObject = 1;
    static const int FIELD_STATIC__dataStoreInstances = 2;

    static const int FIELD__storageType = 1;

    TINYCLR_NATIVE_DECLARE(CreateDataStore___I4__BOOLEAN);
    TINYCLR_NATIVE_DECLARE(GetMaxAllocationSize___I4);
    TINYCLR_NATIVE_DECLARE(GetTotalSpace___I4);
    TINYCLR_NATIVE_DECLARE(GetUsedBytes___I4);
    TINYCLR_NATIVE_DECLARE(GetFreeBytes___I4);
    TINYCLR_NATIVE_DECLARE(GetReadAllDataIds___I4__SZARRAY_I4__I4__I4);
    TINYCLR_NATIVE_DECLARE(GetCountOfDataIds___I4);
    TINYCLR_NATIVE_DECLARE(DeleteAll___I4);
    TINYCLR_NATIVE_DECLARE(EraseAllBlocks___I4);
    TINYCLR_NATIVE_DECLARE(DataStoreGC___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(GetReadWriteStatus___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(GetLastErrorStatus___I4);
    TINYCLR_NATIVE_DECLARE(Write___I4__U4__SZARRAY_U1__U4__U4__U1__I4);
    TINYCLR_NATIVE_DECLARE(Write___I4__U4__SZARRAY_U1__U4__U4__U2__I4);
    TINYCLR_NATIVE_DECLARE(Write___I4__U4__SZARRAY_U1__U4__U4__U4__I4);
    TINYCLR_NATIVE_DECLARE(Read___I4__U4__SZARRAY_U1__U4__U4__U1__I4);
    TINYCLR_NATIVE_DECLARE(Read___I4__U4__SZARRAY_U1__U4__U4__U2__I4);
    TINYCLR_NATIVE_DECLARE(Read___I4__U4__SZARRAY_U1__U4__U4__U4__I4);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_eMote;

#endif  //_SAMRAKSHEMOTE_H_
