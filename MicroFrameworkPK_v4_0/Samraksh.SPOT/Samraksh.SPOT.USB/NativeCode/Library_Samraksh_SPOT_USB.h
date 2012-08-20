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


#ifndef _LIBRARY_SAMRAKSH_SPOT_USB_H_
#define _LIBRARY_SAMRAKSH_SPOT_USB_H_

namespace Samraksh
{
    namespace SPOT
    {
        struct USB
        {
            // Helper Functions to access fields of managed object
            // Declaration of stubs. These functions are implemented by Interop code developers
            static INT32 Write( CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, HRESULT &hr );
            static INT32 Read( CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, HRESULT &hr );
            static INT8 Flush( HRESULT &hr );
        };
    }
}
#endif  //_LIBRARY_SAMRAKSH_SPOT_USB_H_
