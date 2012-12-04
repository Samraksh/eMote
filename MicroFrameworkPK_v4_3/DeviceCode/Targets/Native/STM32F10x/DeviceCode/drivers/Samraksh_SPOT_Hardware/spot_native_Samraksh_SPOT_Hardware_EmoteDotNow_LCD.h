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


#ifndef _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_EMOTEDOTNOW_LCD_H_
#define _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_EMOTEDOTNOW_LCD_H_

namespace Samraksh
{
    namespace SPOT
    {
        namespace Hardware
        {
            namespace EmoteDotNow
            {
                struct LCD
                {
                    // Helper Functions to access fields of managed object
                    // Declaration of stubs. These functions are implemented by Interop code developers
                    static INT8 Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT8 Write( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, INT32 param2, INT32 param3, HRESULT &hr );
                    static INT8 Clear( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                };
            }
        }
    }
}
#endif  //_SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_EMOTEDOTNOW_LCD_H_
