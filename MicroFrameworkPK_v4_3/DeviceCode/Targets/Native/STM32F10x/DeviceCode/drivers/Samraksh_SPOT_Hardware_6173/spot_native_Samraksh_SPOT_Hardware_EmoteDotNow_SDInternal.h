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


#ifndef _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_EMOTEDOTNOW_SDINTERNAL_H_
#define _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_EMOTEDOTNOW_SDINTERNAL_H_

namespace Samraksh
{
    namespace SPOT
    {
        namespace Hardware
        {
            namespace EmoteDotNow
            {
                struct SDInternal
                {
                    // Helper Functions to access fields of managed object
                    // Declaration of stubs. These functions are implemented by Interop code developers
                    static INT32 InternalInitialize( HRESULT &hr );
                    static INT32 InternalWrite( CLR_RT_TypedArray_UINT8 param0, UINT16 param1, UINT16 param2, UINT32 param3, HRESULT &hr );
                    static INT32 InternalRead( CLR_RT_TypedArray_UINT8 param0, UINT16 param1, UINT16 param2, UINT32 param3, HRESULT &hr );
                };
            }
        }
    }
}
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_SD;
#endif  //_SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_EMOTEDOTNOW_SDINTERNAL_H_
