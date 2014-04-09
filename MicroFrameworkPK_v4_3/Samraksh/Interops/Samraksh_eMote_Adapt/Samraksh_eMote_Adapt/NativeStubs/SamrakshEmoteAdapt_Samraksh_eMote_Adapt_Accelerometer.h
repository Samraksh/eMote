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


#ifndef _SAMRAKSHEMOTEADAPT_SAMRAKSH_EMOTE_ADAPT_ACCELEROMETER_H_
#define _SAMRAKSHEMOTEADAPT_SAMRAKSH_EMOTE_ADAPT_ACCELEROMETER_H_

namespace Samraksh
{
    namespace eMote
    {
        namespace Adapt
        {
            struct Accelerometer
            {
                // Helper Functions to access fields of managed object
                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT8 Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT8 Reset( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT8 SelfTest( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT16 GetX( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT16 GetY( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT16 GetZ( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT16 GetTemperature( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT8 ReadRegister( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                static INT8 WriteRegister( CLR_RT_HeapBlock* pMngObj, UINT8 param0, UINT8 param1, HRESULT &hr );
                static INT8 AdvancedConfiguration( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT8 SetPowerMode( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT8 SetBandwidth( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT8 SetRange( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                static INT8 GetAllData( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT16 param0, HRESULT &hr );
            };
        }
    }
}
#endif  //_SAMRAKSHEMOTEADAPT_SAMRAKSH_EMOTE_ADAPT_ACCELEROMETER_H_
