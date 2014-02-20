//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#ifndef _SPOT_NATIVE_H_
#define _SPOT_NATIVE_H_

#include <TinyCLR_Interop.h>
struct Library_spot_native_Samraksh_SPOT_Hardware_Accelerometer
{
    static const int FIELD__currentRange = 1;
    static const int FIELD__currentResolution = 2;
    static const int FIELD__sensorData = 3;
    static const int FIELD__refreshRate = 4;
    static const int FIELD__sensorSamplingTimer = 5;
    static const int FIELD__userCallbackFn = 6;
    static const int FIELD__accelerometerDataBuffer = 7;

    TINYCLR_NATIVE_DECLARE(Initialize___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(Reset___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(SelfTest___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(GetX___I2);
    TINYCLR_NATIVE_DECLARE(GetY___I2);
    TINYCLR_NATIVE_DECLARE(GetZ___I2);
    TINYCLR_NATIVE_DECLARE(GetTemperature___I2);
    TINYCLR_NATIVE_DECLARE(ReadRegister___U1__U1);
    TINYCLR_NATIVE_DECLARE(WriteRegister___BOOLEAN__U1__U1);
    TINYCLR_NATIVE_DECLARE(AdvancedConfig___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(SetPowerMode___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(SetBandwidth___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(SetRange___BOOLEAN__U1);
    TINYCLR_NATIVE_DECLARE(GetAllData___BOOLEAN__SZARRAY_I2);

    //--//

};

struct Library_spot_native_Samraksh_SPOT_Hardware_Accelerometer__Data
{
    static const int FIELD__RawX = 1;
    static const int FIELD__RawY = 2;
    static const int FIELD__RawZ = 3;
    static const int FIELD__X = 4;
    static const int FIELD__Y = 5;
    static const int FIELD__Z = 6;


    //--//

};

struct Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_SD
{
    static const int FIELD_STATIC__sdInternalObject = 0;
    static const int FIELD_STATIC__sdCallbackFunction = 1;
    static const int FIELD_STATIC__writeAddressPtr = 2;
    static const int FIELD_STATIC__readAddressPtr = 3;


    //--//

};

struct Library_spot_native_Samraksh_SPOT_Hardware_AnalogInput
{
    static const int FIELD__MyCallback = 1;

    TINYCLR_NATIVE_DECLARE(Initialize___SamrakshSPOTHardwareDeviceStatus);
    TINYCLR_NATIVE_DECLARE(Read___U2__I4);
    TINYCLR_NATIVE_DECLARE(ReadBatch___SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__I4__U4__U4);
    TINYCLR_NATIVE_DECLARE(GetMaxSampleRate___U4);

    //--//

};

struct Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal
{
    TINYCLR_NATIVE_DECLARE(Init___STATIC__I4__I4);
    TINYCLR_NATIVE_DECLARE(Read___STATIC__R8__I4);
    TINYCLR_NATIVE_DECLARE(ConfigureBatchMode___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__I4__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureContinuousMode___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__I4__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureContinuousModeDualChannel___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__SZARRAY_U2__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureBatchModeDualChannel___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__SZARRAY_U2__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureContinuousModeWithThresholding___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__I4__U4__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureBatchModeWithThresholding___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__I4__U4__U4__U4);
    TINYCLR_NATIVE_DECLARE(DualChannelRead___STATIC__BOOLEAN__SZARRAY_U2);
    TINYCLR_NATIVE_DECLARE(StopSampling___STATIC__I4);

    //--//

};

struct Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_AnalogInput
{
    static const int FIELD_STATIC__AdcInternal = 4;
    static const int FIELD_STATIC__MyCallback = 5;


    //--//

};

struct Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_EmoteLCD
{
    static const int FIELD__currentColumn1 = 1;
    static const int FIELD__currentColumn2 = 2;
    static const int FIELD__currentColumn3 = 3;
    static const int FIELD__currentColumn4 = 4;
    static const int FIELD__DP1 = 5;
    static const int FIELD__DP2 = 6;
    static const int FIELD__DP3 = 7;
    static const int FIELD__DP4 = 8;

    TINYCLR_NATIVE_DECLARE(Initialize___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(Uninitialize___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(Write___BOOLEAN__SamrakshSPOTHardwareEmoteDotNowLCD__SamrakshSPOTHardwareEmoteDotNowLCD__SamrakshSPOTHardwareEmoteDotNowLCD__SamrakshSPOTHardwareEmoteDotNowLCD);
    TINYCLR_NATIVE_DECLARE(SetDP___BOOLEAN__BOOLEAN__BOOLEAN__BOOLEAN__BOOLEAN);
    TINYCLR_NATIVE_DECLARE(WriteN___BOOLEAN__I4__SamrakshSPOTHardwareEmoteDotNowLCD);
    TINYCLR_NATIVE_DECLARE(WriteRawBytes___BOOLEAN__I4__I4__I4__I4);
    TINYCLR_NATIVE_DECLARE(Blink___BOOLEAN__SamrakshSPOTHardwareEmoteDotNowBlink);
    TINYCLR_NATIVE_DECLARE(Clear___BOOLEAN);

    //--//

};

struct Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_NOR
{
    static const int FIELD_STATIC__writeAddressPtr = 6;
    static const int FIELD_STATIC__readAddressPtr = 7;
    static const int FIELD_STATIC__startOfRecordDelimiter = 8;
    static const int FIELD_STATIC__endOfRecordDelimiter = 9;
    static const int FIELD_STATIC__startOfRecordInBytes = 10;
    static const int FIELD_STATIC__endOfRecordInBytes = 11;
    static const int FIELD_STATIC__maxSize = 12;
    static const int FIELD_STATIC__fullFlag = 13;

    TINYCLR_NATIVE_DECLARE(InternalInitialize___STATIC__BOOLEAN);
    TINYCLR_NATIVE_DECLARE(InternalWrite___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__U4__U2);
    TINYCLR_NATIVE_DECLARE(InternalWrite___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__U4__U2__U2);
    TINYCLR_NATIVE_DECLARE(InternalRead___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U2__U4__U2);

    //--//

};

struct Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_SDInternal
{
    TINYCLR_NATIVE_DECLARE(InternalInitialize___STATIC__SamrakshSPOTHardwareDeviceStatus);
    TINYCLR_NATIVE_DECLARE(InternalWrite___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U1__U2__U2__U4);
    TINYCLR_NATIVE_DECLARE(InternalRead___STATIC__SamrakshSPOTHardwareDeviceStatus__SZARRAY_U1__U2__U2__U4);

    //--//

};

struct Library_spot_native_Samraksh_SPOT_Hardware_FPGA
{
    TINYCLR_NATIVE_DECLARE(Initialize___BOOLEAN);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_SPOT_Hardware;

#endif  //_SPOT_NATIVE_H_
