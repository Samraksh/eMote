//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "InterfaceLibrary.h"


static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    Library_InterfaceLibrary_Samraksh_FileSystem::Init___STATIC__SamrakshFileSystemError,
    Library_InterfaceLibrary_Samraksh_FileSystem::Write___STATIC__SamrakshFileSystemError__SZARRAY_U1__U4__U2,
    Library_InterfaceLibrary_Samraksh_FileSystem::Read___STATIC__SamrakshFileSystemError__SZARRAY_U1__U4__U2,
    Library_InterfaceLibrary_Samraksh_FileSystem::Erase___STATIC__SamrakshFileSystemError__U4__U4,
    NULL,
    Library_InterfaceLibrary_Samraksh_MessageLayer::Init___STATIC__I4,
    Library_InterfaceLibrary_Samraksh_MessageLayer::Send___STATIC__I4__U1__U2__SZARRAY_U1__U2,
    Library_InterfaceLibrary_Samraksh_MessageLayer::Received___STATIC__I4__SZARRAY_U1,
    Library_InterfaceLibrary_Samraksh_MessageLayer::ConfigureReceiver___STATIC__I4__U1__SamrakshMessageLayerRadioCallBack,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_InterfaceLibrary_Samraksh_ADC::Init___STATIC__I4__I4,
    Library_InterfaceLibrary_Samraksh_ADC::getData___STATIC__I4__SZARRAY_U2__U4__U4,
    NULL,
    NULL,
    Library_InterfaceLibrary_Samraksh_Memory::write___BOOLEAN__I4__SZARRAY_U1,
    Library_InterfaceLibrary_Samraksh_Memory::constructNativeMemoryPointer___BOOLEAN__I4__U4,
    Library_InterfaceLibrary_Samraksh_Memory::disposeNativeMemoryPointer___BOOLEAN__I4,
    NULL,
    NULL,
    Library_InterfaceLibrary_Samraksh_Memory::read___BOOLEAN__I4__SZARRAY_U1,
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
    Library_InterfaceLibrary_Samraksh_TimeSync::Init___STATIC__VOID,
    Library_InterfaceLibrary_Samraksh_TimeSync::setBeaconRate___STATIC__VOID__I4,
    Library_InterfaceLibrary_Samraksh_TimeSync::sendTimeSyncMessage___STATIC__VOID,
    Library_InterfaceLibrary_Samraksh_TimeSync::getLocalTime___STATIC__I4,
    Library_InterfaceLibrary_Samraksh_TimeSync::getGlobalTime___STATIC__I4,
    Library_InterfaceLibrary_Samraksh_TimeSync::getSkew___STATIC__R4,
    Library_InterfaceLibrary_Samraksh_TimeSync::local2Global___STATIC__VOID,
    Library_InterfaceLibrary_Samraksh_TimeSync::getRootId___STATIC__I2,
    NULL,
    NULL,
    NULL,
    Library_InterfaceLibrary_Samraksh_USBPipe::Write___STATIC__I4__SZARRAY_U1__I4__I4,
    Library_InterfaceLibrary_Samraksh_USBPipe::Read___STATIC__I4__SZARRAY_U1__I4__I4,
    Library_InterfaceLibrary_Samraksh_USBPipe::Flush___STATIC__BOOLEAN,
    NULL,
    Library_InterfaceLibrary_Samraksh_rDAC::Init___STATIC__VOID,
    Library_InterfaceLibrary_Samraksh_rDAC::setValue___STATIC__I4__U2,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh =
{
    "Samraksh", 
    0xDC0A4C07,
    method_lookup
};

