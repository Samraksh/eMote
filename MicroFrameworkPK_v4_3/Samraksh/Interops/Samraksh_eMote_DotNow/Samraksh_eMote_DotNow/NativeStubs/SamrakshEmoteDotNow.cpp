//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "SamrakshEmoteDotNow.h"


static const CLR_RT_MethodHandler method_lookup[] =
{
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
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal::Init___STATIC__I4__I4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal::Read___STATIC__R8__I4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal::ConfigureBatchMode___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U2__I4__U4__U4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal::ConfigureContinuousMode___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U2__I4__U4__U4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal::ConfigureContinuousModeDualChannel___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U2__SZARRAY_U2__U4__U4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal::ConfigureBatchModeDualChannel___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U2__SZARRAY_U2__U4__U4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal::ConfigureContinuousModeWithThresholding___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U2__I4__U4__U4__U4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal::ConfigureBatchModeWithThresholding___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U2__I4__U4__U4__U4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal::DualChannelRead___STATIC__BOOLEAN__SZARRAY_U2,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal::StopSampling___STATIC__I4,
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
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::Initialize___BOOLEAN,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::Uninitialize___BOOLEAN,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::Write___BOOLEAN__SamraksheMoteDotNowLCD__SamraksheMoteDotNowLCD__SamraksheMoteDotNowLCD__SamraksheMoteDotNowLCD,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::SetDP___BOOLEAN__BOOLEAN__BOOLEAN__BOOLEAN__BOOLEAN,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::WriteN___BOOLEAN__I4__SamraksheMoteDotNowLCD,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::WriteRawBytes___BOOLEAN__I4__I4__I4__I4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::Blink___BOOLEAN__SamraksheMoteDotNowBlink,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::Clear___BOOLEAN,
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
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NOR::InternalInitialize___STATIC__BOOLEAN,
    NULL,
    NULL,
    NULL,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NOR::InternalWrite___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U2__U4__U2,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NOR::InternalWrite___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U2__U4__U2__U2,
    NULL,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NOR::InternalRead___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U2__U4__U2,
    NULL,
    NULL,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFileStream::_ctor___VOID__STRING__I4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFileStream::Read___I4__SZARRAY_U1__I4__I4__I4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFileStream::Write___I4__SZARRAY_U1__I4__I4__I4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFileStream::Seek___I8__I8__U4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFileStream::Flush___VOID,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFileStream::GetLength___I8,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFileStream::SetLength___VOID__I8,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFileStream::GetStreamProperties___VOID__BYREF_BOOLEAN__BYREF_BOOLEAN__BYREF_BOOLEAN,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFileStream::Close___VOID,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFindFile::_ctor___VOID__STRING__STRING,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFindFile::GetNext___SamraksheMoteDotNowNativeFileInfo,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFindFile::Close___VOID,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFindFile::GetFileInfo___STATIC__SamraksheMoteDotNowNativeFileInfo__STRING,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeIO::Format___STATIC__VOID__STRING__STRING__STRING__U4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeIO::Delete___STATIC__VOID__STRING,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeIO::Move___STATIC__BOOLEAN__STRING__STRING,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeIO::CreateDirectory___STATIC__VOID__STRING,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeIO::GetAttributes___STATIC__U4__STRING,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeIO::SetAttributes___STATIC__VOID__STRING__U4,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_RemovableMedia::MountRemovableVolumes___STATIC__VOID,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_SDInternal::InternalInitialize___STATIC__SamraksheMoteDotNowDeviceStatus,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_SDInternal::InternalWrite___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U1__U2__U2__U4,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_SDInternal::InternalRead___STATIC__SamraksheMoteDotNowDeviceStatus__SZARRAY_U1__U2__U2__U4,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_VolumeInfo::_ctor___VOID__STRING,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_VolumeInfo::_ctor___VOID__U4,
    NULL,
    NULL,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_VolumeInfo::Refresh___VOID,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_VolumeInfo::FlushAll___VOID,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_VolumeInfo::GetName___STRING,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_VolumeInfo::GetVolumes___STATIC__SZARRAY_SamraksheMoteDotNowVolumeInfo,
    Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_VolumeInfo::GetFileSystems___STATIC__SZARRAY_STRING,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_eMote_DotNow =
{
    "Samraksh_eMote_DotNow", 
    0x4E998AA3,
    method_lookup
};

