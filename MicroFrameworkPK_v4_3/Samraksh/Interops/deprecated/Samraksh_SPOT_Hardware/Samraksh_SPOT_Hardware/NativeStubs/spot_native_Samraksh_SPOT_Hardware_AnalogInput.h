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


#ifndef _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ANALOGINPUT_H_
#define _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ANALOGINPUT_H_

namespace Samraksh
{
    namespace SPOT
    {
        namespace Hardware
        {
            struct AnalogInput
            {
                // Helper Functions to access fields of managed object
                static UNSUPPORTED_TYPE& Get_MyCallback( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_spot_native_Samraksh_SPOT_Hardware_AnalogInput::FIELD__MyCallback ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT32 Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT16 Read( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
                static INT32 ReadBatch( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, HRESULT &hr );
                static UINT32 GetMaxSampleRate( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            };
        }
    }
}
#endif  //_SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ANALOGINPUT_H_
