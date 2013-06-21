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
#include "spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_ADCInternal.h"

#include "..\adc\hal_adc_driver.h"

using namespace Samraksh::SPOT::Hardware::EmoteDotNow;

#include "../tim/netmf_timers.h"
#define INTERNAL_BUFFER_SIZE 1000
CLR_RT_HeapBlock_NativeEventDispatcher *g_adcContext = NULL;
static UINT64 g_adcUserData = 0;
static bool g_adcInterruptEnabled = false;
UINT32 adcChannel = 0;
//UINT32 adcNumChannel = 0;
UINT32 adcNumSamples = 0;
UINT32 adcTicks = 0;
UINT32 prescaler = 0;
CLR_RT_TypedArray_UINT16 gAdcCSharpBuffer;
bool batchModeADC = false;

extern "C"
{
	void ADCCallback(void *param);
}

UINT16 adcBuffer[INTERNAL_BUFFER_SIZE];
UINT32 adcBufferCounter = 0;


INT32 ADCInternal::Init( INT32 param0, HRESULT &hr )
{
    return AD_Initialize((ANALOG_CHANNEL) param0, 0);
}

double ADCInternal::Read( INT32 param0, HRESULT &hr )
{
	return (double) AD_Read((ANALOG_CHANNEL) param0);
}

INT32 ADCInternal::ConfigureBatchMode( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 ADCInternal::ConfigureContinuousMode( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 ADCInternal::ConfigureContinuousModeWithThresholding( CLR_RT_TypedArray_UINT16 param0, INT32 param1, UINT32 param2, UINT32 param3, UINT32 param4, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

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
    INT32 retVal = 0; 
    return retVal;
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
    SaveNativeEventToHALQueue( pContext, UINT32(g_adcUserData >> 16), UINT32(g_adcUserData & 0xFFFFFFFF) );
}


extern "C"
{
	void ADCCallback(void *param)
	{
		// Not able to do this more elegantly
		if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
		  {
		    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );

		    UINT16 adcSample;

		    //hal_adc_getData(adcSample, adcStartChannel,adcNumChannel);
		    adcSample = AD_Read((ANALOG_CHANNEL) adcChannel);

		    //for(UINT8 i = 0; i < 1; i++)
		    adcBuffer[adcBufferCounter++] = adcSample;

		    if(adcBufferCounter >=  adcNumSamples)
		    {
		    	adcBufferCounter = 0;
		    	//for(UINT32 j = 0; j < 1000; ++j)
		    	//	gAdcCSharpBuffer.SetValue(j, adcBuffer[j]);
				memcpy(gAdcCSharpBuffer.GetBuffer(), adcBuffer, adcNumSamples * sizeof(UINT16));

		    	ISR_adcProc(g_adcContext);
		    }
		    if(batchModeADC == true)
		    	Timer_Driver::Uninitialize(Timer_Driver :: c_ADCTimer);
		    else
		    	Timer_Driver :: SetCompare (Timer_Driver :: c_ADCTimer, Timer_Driver::GetCounter(Timer_Driver :: c_ADCTimer) + (adcTicks & 0xffff));
		  }
	}
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

