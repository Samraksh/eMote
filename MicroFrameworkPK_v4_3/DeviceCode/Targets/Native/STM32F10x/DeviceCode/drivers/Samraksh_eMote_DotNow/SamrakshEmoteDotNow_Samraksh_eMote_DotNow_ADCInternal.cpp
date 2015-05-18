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


#include "SamrakshEmoteDotNow.h"
#include "SamrakshEmoteDotNow_Samraksh_eMote_DotNow_ADCInternal.h"

#include <tinyhal.h>

#include "..\adc\hal_adc_driver.h"

using namespace Samraksh::eMote::DotNow;

CLR_RT_HeapBlock_NativeEventDispatcher *g_adcContext = NULL;
static UINT64 g_adcUserData = 0;
static BOOL g_adcInterruptEnabled = TRUE;

extern "C"
{
	void ADCInteropCallback(void *param)
	{
		 GLOBAL_LOCK(irq);

		 g_adcUserData = *((UINT64 *) param);

		 SaveNativeEventToHALQueue( g_adcContext, UINT32(g_adcUserData >> 32), UINT32(g_adcUserData & 0xFFFFFFFF) );
	}
}

INT32 ADCInternal::Init( INT32 param0, HRESULT &hr )
{

	return AD_Initialize((ANALOG_CHANNEL) param0, 0);

}

double ADCInternal::Read( INT32 param0, HRESULT &hr )
{
	return (double) AD_Read((ANALOG_CHANNEL) param0);
}

INT32 ADCInternal::ConfigureBatchMode( CLR_RT_TypedArray_UINT16 sampleBuff, INT32 channel, UINT32 numSamples, UINT32 samplingTime, HRESULT &hr )
{
    return AD_ConfigureBatchMode(sampleBuff.GetBuffer(), numSamples, samplingTime, ADCInteropCallback, NULL);
}

INT32 ADCInternal::ConfigureContinuousMode( CLR_RT_TypedArray_UINT16 sampleBuff, INT32 channel, UINT32 numSamples, UINT32 samplingTime, HRESULT &hr )
{
    return AD_ConfigureContinuousMode(sampleBuff.GetBuffer(), numSamples, samplingTime, ADCInteropCallback, NULL);
}

INT32 ADCInternal::ConfigureContinuousModeDualChannel( CLR_RT_TypedArray_UINT16 sampleBuff1, CLR_RT_TypedArray_UINT16 sampleBuff2, UINT32 numSamples, UINT32 samplingTime, UINT32 debugMode, HRESULT &hr )
{
    return AD_ConfigureContinuousModeDualChannel(sampleBuff1.GetBuffer(), sampleBuff2.GetBuffer(), numSamples, samplingTime, debugMode, ADCInteropCallback, NULL);
}

INT32 ADCInternal::ConfigureBatchModeDualChannel( CLR_RT_TypedArray_UINT16 sampleBuff1, CLR_RT_TypedArray_UINT16 sampleBuff2, UINT32 numSamples, UINT32 samplingTime, HRESULT &hr )
{
    return AD_ConfigureBatchModeDualChannel(sampleBuff1.GetBuffer(), sampleBuff2.GetBuffer(), numSamples, samplingTime, ADCInteropCallback, NULL);
}

// The functions are deprecated, this was added to help with the india effort
// but this breaks abstraction and is a really poor design
INT32 ADCInternal::ConfigureContinuousModeWithThresholding( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, UINT32 param4, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

// The functions are deprecated, this was added to help with the india effort
// but this breaks abstraction and is a really poor design
INT32 ADCInternal::ConfigureBatchModeWithThresholding( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, UINT32 param4, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT8 ADCInternal::DualChannelRead( CLR_RT_TypedArray_UINT16 param0, HRESULT &hr )
{
	return hal_adc_getData(param0.GetBuffer(), 0 , 2);

}

INT32 ADCInternal::StopSampling( HRESULT &hr )
{

	AD_StopSampling();

    return DS_Success;
}

static HRESULT InitializeADCDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
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

