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


#ifndef _LIB_SIMPLEINTEROP_LIBRARY_H_
#define _LIB_SIMPLEINTEROP_LIBRARY_H_

namespace SimpleInterop
{
    struct Library
    {
        // Helper Functions to access fields of managed object
        // Declaration of stubs. These functions are implemented by Interop code developers
        static void init( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static void execute( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
    };
}
#endif  //_LIB_SIMPLEINTEROP_LIBRARY_H_
