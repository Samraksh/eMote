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


#ifndef _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ADAPT_ADC_H_
#define _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ADAPT_ADC_H_

namespace Samraksh
{
    namespace SPOT
    {
        namespace Hardware
        {
            namespace Adapt
            {
                struct ADC
                {
                    // Helper Functions to access fields of managed object
                    // Declaration of stubs. These functions are implemented by Interop code developers
                    static INT8 ADAPT_ADC_Init( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT8 ADAPT_ADC_Conf( CLR_RT_HeapBlock* pMngObj, UINT8 param0, UINT8 param1, UINT8 param2, HRESULT &hr );
                    static INT8 ADAPT_ADC_Command( CLR_RT_HeapBlock* pMngObj, UINT8 param0, UINT8 param1, HRESULT &hr );
                    static UINT8 ADAPT_ADC_Status( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT8 ADAPT_ADC_Sample_Rate( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                    static UINT16 ADAPT_ADC_Get_Sample( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT8 ADAPT_ADC_Set_Chan( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                    static INT8 ADAPT_ADC_Raw( CLR_RT_HeapBlock* pMngObj, UINT8 param0, UINT8 param1, UINT8 param2, HRESULT &hr );
                    static UINT16 ADAPT_ADC_Cont_Get( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, UINT8 param1, HRESULT &hr );
                    static INT8 ADAPT_ADC_Now( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                    static INT8 ADAPT_ADC_Cont_Start( CLR_RT_HeapBlock* pMngObj, UINT8 param0, UINT8 param1, HRESULT &hr );
                    static INT8 ADAPT_ADC_Cont_Stop( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                };
            }
        }
    }
}
#endif  //_SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ADAPT_ADC_H_
