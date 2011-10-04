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


#ifndef _INTERFACELIBRARY_SAMRAKSH_MESSAGELAYER_H_
#define _INTERFACELIBRARY_SAMRAKSH_MESSAGELAYER_H_

namespace Samraksh
{
    struct MessageLayer
    {
        // Helper Functions to access fields of managed object
        // Declaration of stubs. These functions are implemented by Interop code developers
        static INT32 Init( HRESULT &hr );
        static INT32 Send( UNSUPPORTED_TYPE param0, UINT32 param1, UINT32 param2, CLR_RT_TypedArray_UINT8 param3, INT32 param4, HRESULT &hr );
        static INT32 Received( HRESULT &hr );
        static INT32 ConfigureReceive( HRESULT &hr );
    };
}
#endif  //_INTERFACELIBRARY_SAMRAKSH_MESSAGELAYER_H_
