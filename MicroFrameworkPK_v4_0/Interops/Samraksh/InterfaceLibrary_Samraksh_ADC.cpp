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


#include "InterfaceLibrary.h"
#include "InterfaceLibrary_Samraksh_ADC.h"

using namespace Samraksh;

extern void hal_adc_init(uint8_t sampTime);
extern uint8_t hal_adc_getData(uint16_t *dataBuf, uint8_t startChannel, uint8_t numChannels);


INT32 ADC::Init( INT32 param0, HRESULT &hr )
{
    INT32 retVal = 0;

    hal_adc_init((UINT8) param0);

    return retVal;
}

INT32 ADC::getData( CLR_RT_TypedArray_UINT16 param0, UINT32 param1, UINT32 param2, HRESULT &hr )
{
    INT32 retVal = 0;

    CLR_RT_TypedArray_UINT16 dataBuf = param0;

    retVal = hal_adc_getData(dataBuf.GetBuffer(), (UINT8) param1, (UINT8) param2);

    return retVal;
}

