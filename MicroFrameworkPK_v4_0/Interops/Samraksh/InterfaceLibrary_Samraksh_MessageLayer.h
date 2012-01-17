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
        static INT32 Send( UINT8 param0, UINT16 param1, CLR_RT_TypedArray_UINT8 param2, UINT16 param3, HRESULT &hr );
        static INT32 Received( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
        static INT32 ConfigureReceiver( UINT8 param0, UNSUPPORTED_TYPE param1, HRESULT &hr );
    };
}
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Radio;
#endif  //_INTERFACELIBRARY_SAMRAKSH_MESSAGELAYER_H_
