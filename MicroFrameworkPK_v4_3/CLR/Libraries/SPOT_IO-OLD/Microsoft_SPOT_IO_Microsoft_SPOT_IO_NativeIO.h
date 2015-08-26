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


#ifndef _MICROSOFT_SPOT_IO_MICROSOFT_SPOT_IO_NATIVEIO_H_
#define _MICROSOFT_SPOT_IO_MICROSOFT_SPOT_IO_NATIVEIO_H_

namespace Microsoft
{
    namespace SPOT
    {
        namespace IO
        {
            struct NativeIO
            {
                // Helper Functions to access fields of managed object
                // Declaration of stubs. These functions are implemented by Interop code developers
                static void Format( LPCSTR param0, LPCSTR param1, LPCSTR param2, UINT32 param3, HRESULT &hr );
                static void Delete( LPCSTR param0, HRESULT &hr );
                static INT8 Move( LPCSTR param0, LPCSTR param1, HRESULT &hr );
                static void CreateDirectory( LPCSTR param0, HRESULT &hr );
                static UINT32 GetAttributes( LPCSTR param0, HRESULT &hr );
                static void SetAttributes( LPCSTR param0, UINT32 param1, HRESULT &hr );
            };
        }
    }
}
#endif  //_MICROSOFT_SPOT_IO_MICROSOFT_SPOT_IO_NATIVEIO_H_
