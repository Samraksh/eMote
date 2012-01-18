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


#ifndef _NATIVE_SAMRAKSH_TIMESYNC_H_
#define _NATIVE_SAMRAKSH_TIMESYNC_H_

namespace Samraksh
{
    struct TimeSync
    {
        // Helper Functions to access fields of managed object
        // Declaration of stubs. These functions are implemented by Interop code developers
        static void Init( HRESULT &hr );
        static void setBeaconRate( INT32 param0, HRESULT &hr );
        static void sendTimeSyncMessage( HRESULT &hr );
        static INT32 getLocalTime( HRESULT &hr );
        static INT32 getGlobalTime( HRESULT &hr );
        static float getSkew( HRESULT &hr );
        static void local2Global( HRESULT &hr );
        static INT16 getRootId( HRESULT &hr );
    };
}
#endif  //_NATIVE_SAMRAKSH_TIMESYNC_H_
