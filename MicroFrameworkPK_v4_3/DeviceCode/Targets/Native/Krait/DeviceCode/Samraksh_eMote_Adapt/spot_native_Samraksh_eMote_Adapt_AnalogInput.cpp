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


#include "spot_native.h"
#include "spot_native_Samraksh_eMote_Adapt_AnalogInput.h"
#include "..\Krait_ADC\Krait_ADC.h"

using namespace Samraksh::eMote::Adapt;

INT32 AnalogInput::Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT32 retVal = fpga_init(); 
    return retVal;
}

UINT16 AnalogInput::Read( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr )
{
    UINT16 retVal = fpga_adc_now(param0); 
    return retVal;
}

INT32 AnalogInput::ReadBatch( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, HRESULT &hr )
{
    INT32 retVal = fpga_read_batch(param0.GetBuffer(), param1, param2, param3); 
    return retVal;
}

UINT32 AnalogInput::GetMaxSampleRate( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    UINT32 retVal = fpga_adc_get_bounds();  
    return retVal;
}

