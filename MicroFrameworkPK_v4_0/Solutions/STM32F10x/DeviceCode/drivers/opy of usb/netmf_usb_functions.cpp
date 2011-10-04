////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>
#include <netmf_usb.h>


USB_CONTROLLER_STATE * CPU_USB_GetState( int Controller )
{
    return USBCS_Driver::GetState( Controller );
}

HRESULT CPU_USB_Initialize( int Controller )
{
    return USBCS_Driver::Initialize( Controller );
}

HRESULT CPU_USB_Uninitialize( int Controller )
{
    return USBCS_Driver::Uninitialize( Controller );
}

BOOL CPU_USB_StartOutput( USB_CONTROLLER_STATE* State, int endpoint )
{
    return USBCS_Driver::StartOutput( State, endpoint );
}

BOOL CPU_USB_RxEnable( USB_CONTROLLER_STATE* State, int endpoint )
{
    return USBCS_Driver::RxEnable( State, endpoint );
}

BOOL CPU_USB_GetInterruptState()
{
    return USBCS_Driver::GetInterruptState();
}

BOOL CPU_USB_ProtectPins( int Controller, BOOL On )
{
    return USBCS_Driver::ProtectPins( Controller, On );
}

