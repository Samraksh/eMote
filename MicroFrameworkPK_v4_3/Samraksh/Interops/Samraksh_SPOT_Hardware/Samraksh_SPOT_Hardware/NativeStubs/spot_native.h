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
struct Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal
{
    TINYCLR_NATIVE_DECLARE(Init___STATIC__I4__I4);
    TINYCLR_NATIVE_DECLARE(Read___STATIC__R8__I4);
    TINYCLR_NATIVE_DECLARE(ConfigureBatchMode___STATIC__I4__SZARRAY_U2__I4__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureContinuousMode___STATIC__I4__SZARRAY_U2__I4__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureContinuousModeWithThresholding___STATIC__I4__SZARRAY_U2__I4__U4__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureBatchModeWithThresholding___STATIC__I4__SZARRAY_U2__I4__U4__U4__U4);
    TINYCLR_NATIVE_DECLARE(DualChannelRead___STATIC__BOOLEAN__SZARRAY_U2);
    TINYCLR_NATIVE_DECLARE(StopSampling___STATIC__I4);

    //--//

};

struct Library_spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_AnalogInput
{
    static const int FIELD_STATIC__AdcInternal = 0;
    static const int FIELD_STATIC__MyCallback = 1;


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
    static const int FIELD_STATIC__writeAddressPtr = 2;
    static const int FIELD_STATIC__readAddressPtr = 3;
    static const int FIELD_STATIC__startOfRecordDelimiter = 4;
    static const int FIELD_STATIC__endOfRecordDelimiter = 5;
    static const int FIELD_STATIC__maxSize = 6;
    static const int FIELD_STATIC__fullFlag = 7;

    TINYCLR_NATIVE_DECLARE(InternalInitialize___STATIC__BOOLEAN);
    TINYCLR_NATIVE_DECLARE(InternalWrite___STATIC__BOOLEAN__SZARRAY_U2__U4__U2);
    TINYCLR_NATIVE_DECLARE(InternalWrite___STATIC__BOOLEAN__SZARRAY_U2__U4__U2__U2);
    TINYCLR_NATIVE_DECLARE(InternalRead___STATIC__BOOLEAN__SZARRAY_U2__U4__U2);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_SPOT_Hardware;

#endif  //_SPOT_NATIVE_H_
