//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#ifndef _SAMRAKSHEMOTEWLN_H_
#define _SAMRAKSHEMOTEWLN_H_

#include <TinyCLR_Interop.h>
struct Library_SamrakshEmoteWLN_Samraksh_eMote_WLN_SD
{
    static const int FIELD_STATIC__sdInternalObject = 0;
    static const int FIELD_STATIC__sdCallbackFunction = 1;
    static const int FIELD_STATIC__writeAddressPtr = 2;
    static const int FIELD_STATIC__readAddressPtr = 3;


    //--//

};

struct Library_SamrakshEmoteWLN_Samraksh_eMote_WLN_ADCInternal
{
    TINYCLR_NATIVE_DECLARE(Init___STATIC__I4__I4);
    TINYCLR_NATIVE_DECLARE(Uninit___STATIC__BOOLEAN);
    TINYCLR_NATIVE_DECLARE(Read___STATIC__R8__I4);
    TINYCLR_NATIVE_DECLARE(ConfigureBatchMode___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U2__I4__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureContinuousMode___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U2__I4__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureContinuousModeDualChannel___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U2__SZARRAY_U2__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureBatchModeDualChannel___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U2__SZARRAY_U2__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureContinuousModeWithThresholding___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U2__I4__U4__U4__U4);
    TINYCLR_NATIVE_DECLARE(ConfigureBatchModeWithThresholding___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U2__I4__U4__U4__U4);
    TINYCLR_NATIVE_DECLARE(DualChannelRead___STATIC__BOOLEAN__SZARRAY_U2);
    TINYCLR_NATIVE_DECLARE(StopSampling___STATIC__I4);
    TINYCLR_NATIVE_DECLARE(SetPeriodicADCSamplingInterval___STATIC__I4__R8__R8);

    //--//

};

struct Library_SamrakshEmoteWLN_Samraksh_eMote_WLN_AnalogInput
{
    static const int FIELD_STATIC___adcInternal = 4;
    static const int FIELD_STATIC___myCallback = 5;


    //--//

};

struct Library_SamrakshEmoteWLN_Samraksh_eMote_WLN_NOR
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
    TINYCLR_NATIVE_DECLARE(InternalWrite___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U2__U4__U2);
    TINYCLR_NATIVE_DECLARE(InternalWrite___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U2__U4__U2__U2);
    TINYCLR_NATIVE_DECLARE(InternalRead___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U2__U4__U2);

    //--//

};

struct Library_SamrakshEmoteWLN_Samraksh_eMote_WLN_SDInternal
{
    TINYCLR_NATIVE_DECLARE(InternalInitialize___STATIC__SamraksheMoteWLNDeviceStatus);
    TINYCLR_NATIVE_DECLARE(InternalWrite___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U1__U2__U2__U4);
    TINYCLR_NATIVE_DECLARE(InternalRead___STATIC__SamraksheMoteWLNDeviceStatus__SZARRAY_U1__U2__U2__U4);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_eMote_WLN;

#endif  //_SAMRAKSHEMOTEWLN_H_
