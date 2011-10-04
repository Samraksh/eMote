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


#ifndef _SPOT_HARDWARE_USB_NATIVE_MICROSOFT_SPOT_HARDWARE_USBCLIENT_USBCONTROLLER_H_
#define _SPOT_HARDWARE_USB_NATIVE_MICROSOFT_SPOT_HARDWARE_USBCLIENT_USBCONTROLLER_H_

namespace Microsoft
{
    namespace SPOT
    {
        namespace Hardware
        {
            namespace UsbClient
            {
                struct UsbController
                {
                    // Helper Functions to access fields of managed object
                    static INT32& Get_m_controllerIndex( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_UsbController::FIELD__m_controllerIndex ); }

                    static INT32& Get_m_configError( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_UsbController::FIELD__m_configError ); }

                    // Declaration of stubs. These functions are implemented by Interop code developers
                    static INT32 get_Status( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static UNSUPPORTED_TYPE get_Configuration( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static void set_Configuration( CLR_RT_HeapBlock* pMngObj, UNSUPPORTED_TYPE param0, HRESULT &hr );
                    static INT8 nativeStart( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT8 nativeStop( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT32 get_Count( HRESULT &hr );
                };
            }
        }
    }
}
#endif  //_SPOT_HARDWARE_USB_NATIVE_MICROSOFT_SPOT_HARDWARE_USBCLIENT_USBCONTROLLER_H_
