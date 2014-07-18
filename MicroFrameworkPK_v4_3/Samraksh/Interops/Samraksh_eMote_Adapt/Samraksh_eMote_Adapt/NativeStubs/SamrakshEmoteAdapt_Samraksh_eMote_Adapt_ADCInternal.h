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


#ifndef _SAMRAKSHEMOTEADAPT_SAMRAKSH_EMOTE_ADAPT_ADCINTERNAL_H_
#define _SAMRAKSHEMOTEADAPT_SAMRAKSH_EMOTE_ADAPT_ADCINTERNAL_H_

namespace Samraksh
{
    namespace eMote
    {
        namespace Adapt
        {
            struct ADCInternal
            {
                // Helper Functions to access fields of managed object
                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT32 Initialize( HRESULT &hr );
                static UINT32 Read( INT32 param0, HRESULT &hr );
                static INT32 ConfigureContinuousMode( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, HRESULT &hr );
                static INT32 StopSampling( HRESULT &hr );
                static UINT32 GetMaxSampleRate( HRESULT &hr );
                static INT32 Uninitialize( HRESULT &hr );
            };
        }
    }
}
#endif  //_SAMRAKSHEMOTEADAPT_SAMRAKSH_EMOTE_ADAPT_ADCINTERNAL_H_
