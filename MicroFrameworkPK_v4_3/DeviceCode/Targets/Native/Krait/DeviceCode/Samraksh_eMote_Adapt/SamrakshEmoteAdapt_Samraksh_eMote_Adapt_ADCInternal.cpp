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


#include "SamrakshEmoteAdapt.h"
#include "SamrakshEmoteAdapt_Samraksh_eMote_Adapt_ADCInternal.h"
#include <tinyhal.h>
#include "..\Krait_ADC\Krait_ADC.h"

using namespace Samraksh::eMote::Adapt;

CLR_RT_HeapBlock_NativeEventDispatcher *g_adcContext = NULL;
static UINT64 g_adcUserData = 0;
static BOOL g_adcInterruptEnabled = TRUE;

extern "C"
{
	void ADCInteropCallback(void *param)
	{
		 GLOBAL_LOCK(irq);

		 g_adcUserData = *((UINT64 *) param);

		 hal_printf("in ADCInteropCallback %d\r\n", g_adcUserData);
		 SaveNativeEventToHALQueue( g_adcContext, UINT32(g_adcUserData >> 32), UINT32(g_adcUserData & 0xFFFFFFFF) );
	}
}

INT32 ADCInternal::Initialize( HRESULT &hr )
{
	hal_printf("adc initialize\r\n");
	INT32 retVal = 1;
    //INT32 retVal = fpga_init(); 
    return retVal;
}

UINT32 ADCInternal::Read( INT32 param0, HRESULT &hr )
{
    UINT32 retVal = fpga_adc_now(param0); 
    return retVal;
}

INT32 ADCInternal::ConfigureContinuousMode( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, HRESULT &hr )
{
	hal_printf("conf cont mode: 0x%x\r\n",param1);
	INT32 retVal = fpga_adc_cont_start(param0.GetBuffer(), param1, param2, param3, ADCInteropCallback, NULL);
	//INT32 retVal = 0;
    return retVal;
}

INT32 ADCInternal::StopSampling( HRESULT &hr )
{
    INT32 retVal = fpga_adc_cont_stop(); 
	hal_printf("conf cont mode stopped %d\r\n",retVal);
    return retVal;
}

UINT32 ADCInternal::GetMaxSampleRate( HRESULT &hr )
{
    UINT32 retVal = fpga_adc_get_bounds();  
    return retVal;
}

INT32 ADCInternal::Uninitialize( HRESULT &hr )
{
    INT32 retVal = fpga_uninit(); 
	//INT32 retVal = 0;
	hal_printf("adc Uninitialize %d\r\n", retVal);
    return retVal;
}

static HRESULT InitializeADCDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
	hal_printf("InitializeADCDriver called\r\n");
   g_adcContext  = pContext;
   g_adcUserData = userData;
   return S_OK;
}

static HRESULT EnableDisableADCDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
   g_adcInterruptEnabled = fEnable;
   return S_OK;
}

static HRESULT CleanupADCDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
    g_adcContext = NULL;
    g_adcUserData = 0;
    CleanupNativeEventsFromHALQueue( pContext );
    return S_OK;
}

void ISR_adcProc( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
    GLOBAL_LOCK(irq);
    SaveNativeEventToHALQueue( pContext, UINT32(g_adcUserData >> 32), UINT32(g_adcUserData & 0xFFFFFFFF) );
}


static const CLR_RT_DriverInterruptMethods g_AdcInteropDriverMethods =
{
  InitializeADCDriver,
  EnableDisableADCDriver,
  CleanupADCDriver
};


const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_ADC =
{
    "ADCCallback",
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_AdcInteropDriverMethods
};

