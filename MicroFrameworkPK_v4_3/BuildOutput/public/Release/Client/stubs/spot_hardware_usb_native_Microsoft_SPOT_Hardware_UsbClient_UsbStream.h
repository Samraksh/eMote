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


#ifndef _SPOT_HARDWARE_USB_NATIVE_MICROSOFT_SPOT_HARDWARE_USBCLIENT_USBSTREAM_H_
#define _SPOT_HARDWARE_USB_NATIVE_MICROSOFT_SPOT_HARDWARE_USBCLIENT_USBSTREAM_H_

namespace Microsoft
{
    namespace SPOT
    {
        namespace Hardware
        {
            namespace UsbClient
            {
                struct UsbStream
                {
                    // Helper Functions to access fields of managed object
                    static INT8& Get_m_disposed( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_UsbStream::FIELD__m_disposed ); }

                    static INT32& Get_m_streamIndex( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_UsbStream::FIELD__m_streamIndex ); }

                    static INT32& Get_m_controllerIndex( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_UsbStream::FIELD__m_controllerIndex ); }

                    static INT32& Get_WriteEndpoint( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_UsbStream::FIELD__WriteEndpoint ); }

                    static INT32& Get_ReadEndpoint( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_UsbStream::FIELD__ReadEndpoint ); }

                    // Declaration of stubs. These functions are implemented by Interop code developers
                    static INT32 nativeOpen( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, HRESULT &hr );
                    static void nativeClose( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT32 nativeRead( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, HRESULT &hr );
                    static INT32 nativeWrite( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, HRESULT &hr );
                    static void nativeFlush( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                };
            }
        }
    }
}
#endif  //_SPOT_HARDWARE_USB_NATIVE_MICROSOFT_SPOT_HARDWARE_USBCLIENT_USBSTREAM_H_
