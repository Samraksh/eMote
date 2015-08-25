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


#ifndef _SAMRAKSHEMOTEDOTNOW_SAMRAKSH_EMOTE_DOTNOW_ADCINTERNAL_H_
#define _SAMRAKSHEMOTEDOTNOW_SAMRAKSH_EMOTE_DOTNOW_ADCINTERNAL_H_

namespace Samraksh
{
    namespace eMote
    {
        namespace DotNow
        {
            struct ADCInternal
            {
                // Helper Functions to access fields of managed object
                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT32 Init( INT32 param0, HRESULT &hr );
                static double Read( INT32 param0, HRESULT &hr );
                static INT32 ConfigureBatchMode( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, HRESULT &hr );
                static INT32 ConfigureContinuousMode( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, HRESULT &hr );
                static INT32 ConfigureContinuousModeDualChannel( CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, UINT32 param2, UINT32 param3, HRESULT &hr );
                static INT32 ConfigureBatchModeDualChannel( CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, UINT32 param2, UINT32 param3, HRESULT &hr );
                static INT32 ConfigureContinuousModeWithThresholding( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, UINT32 param4, HRESULT &hr );
                static INT32 ConfigureBatchModeWithThresholding( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, UINT32 param4, HRESULT &hr );
                static INT8 DualChannelRead( CLR_RT_TypedArray_UINT16 param0, HRESULT &hr );
                static INT32 StopSampling( HRESULT &hr );
            };
        }
    }
}
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_ADC;
#endif  //_SAMRAKSHEMOTEDOTNOW_SAMRAKSH_EMOTE_DOTNOW_ADCINTERNAL_H_
