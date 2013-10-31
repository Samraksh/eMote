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

#include <Samraksh/HALTimer.h>
#include <Samraksh/Hal_util.h>

using namespace Samraksh::SPOT::Hardware::EmoteDotNow;

#include "../tim/netmf_timers.h"


#define INTERNAL_BUFFER_SIZE 1000

#define ADC_VIRTUAL_TIMER 4

#define ADC_DEBUG_ENABLED 1

#if defined(ADC_DEBUG_ENABLED)
#define ADC_DEBUG_ENABLEOUTPUTPIN(x,y) CPU_GPIO_EnableOutputPin(x,y)
#define ADC_DEBUG_SETPINSTATE(x, y) CPU_GPIO_SetPinState(x,y)
#else
#define ADC_DEBUG_ENABLEOUTPUTPIN(x,y)
#define ADC_DEBUG_SETPINSTATE(x,y)
#endif

CLR_RT_HeapBlock_NativeEventDispatcher *g_adcContext = NULL;
static UINT64 g_adcUserData = 0;
static bool g_adcInterruptEnabled = false;
UINT32 adcChannel = 0;
//UINT32 adcNumChannel = 0;
UINT32 adcNumSamples = 0;
UINT32 adcTicks = 0;
UINT32 prescaler = 0;

BOOL thresholdingEnabled = FALSE;

UINT32 gThresholdTime = 0;

UINT32 gThresholdValue = 0;

BOOL gThresholdingDone =FALSE;

bool batchModeADC = false;

extern HALTimerManager gHalTimerManagerObject;

extern "C"
{
	void ADCCallback(void *param);
}



UINT16 *adcNativeBuffer = NULL;
UINT16 *adcManagedBuffer = NULL;
UINT32 adcBufferCounter = 0;


INT32 ADCInternal::Init( INT32 param0, HRESULT &hr )
{
	ADC_DEBUG_ENABLEOUTPUTPIN(25, FALSE);
    return AD_Initialize((ANALOG_CHANNEL) param0, 0);
}

double ADCInternal::Read( INT32 param0, HRESULT &hr )
{
	return (double) AD_Read((ANALOG_CHANNEL) param0);
}

INT32 ADCInternal::ConfigureBatchMode( CLR_RT_TypedArray_UINT16 sampleBuff, INT32 channel, UINT32 numSamples, UINT32 samplingTime, HRESULT &hr )
{
    INT32 retVal = 0; 

    // Make sure the number of samples can be held in the buffer
    if(sampleBuff.GetSize() < numSamples)
    	return DS_Fail;

    // Get Buffer
    adcManagedBuffer = sampleBuff.GetBuffer();

    // Attempt at creating dynamic memory for the driver
    adcNativeBuffer = (UINT16 *) private_malloc(sizeof(UINT16) * numSamples);

    // Initialize the virtual timer
    gHalTimerManagerObject.Initialize();

    // Create a hal timer
    if(!gHalTimerManagerObject.CreateTimer(ADC_VIRTUAL_TIMER, 0, samplingTime, FALSE, FALSE, ADCCallback)){ //50 milli sec Timer in micro seconds
    			return DS_Fail;
    }

    adcChannel = channel;

    adcNumSamples = numSamples;

    batchModeADC = TRUE;

    return DS_Success;
}

INT32 ADCInternal::ConfigureContinuousMode( CLR_RT_TypedArray_UINT16 sampleBuff, INT32 channel, UINT32 numSamples, UINT32 samplingTime, HRESULT &hr )
{
	 INT32 retVal = 0;

	    // Make sure the number of samples can be held in the buffer
	 if(sampleBuff.GetSize() < numSamples)
	   	return DS_Fail;

	 // Get Buffer
	 adcManagedBuffer = sampleBuff.GetBuffer();

	 // Attempt at creating dynamic memory for the driver
	 adcNativeBuffer = (UINT16 *) private_malloc(sizeof(UINT16) * numSamples);

	 // Initialize the virtual timer
	 gHalTimerManagerObject.Initialize();

	    // Create a hal timer
	 if(!gHalTimerManagerObject.CreateTimer(ADC_VIRTUAL_TIMER, 0, samplingTime, FALSE, FALSE, ADCCallback)){ //50 milli sec Timer in micro seconds
	   			return DS_Fail;
	 }

	 adcChannel = channel;

	 adcNumSamples = numSamples;

	 return DS_Success;
}

INT32 ADCInternal::ConfigureContinuousModeWithThresholding( CLR_RT_TypedArray_UINT16 sampleBuff, INT32 channel, UINT32 numSamples, UINT32 samplingTime, UINT32 threshold, HRESULT &hr )
{
	 INT32 retVal = 0;

		    // Make sure the number of samples can be held in the buffer
		 if(sampleBuff.GetSize() < numSamples)
		   	return DS_Fail;

		 // Get Buffer
		 adcManagedBuffer = sampleBuff.GetBuffer();

		 // Attempt at creating dynamic memory for the driver
		 adcNativeBuffer = (UINT16 *) private_malloc(sizeof(UINT16) * numSamples);

		 // Initialize the virtual timer
		 gHalTimerManagerObject.Initialize();

		    // Create a hal timer
		 if(!gHalTimerManagerObject.CreateTimer(ADC_VIRTUAL_TIMER, 0, samplingTime, FALSE, FALSE, ADCCallback)){ //50 milli sec Timer in micro seconds
		   			return DS_Fail;
		 }

		 adcChannel = channel;

		 adcNumSamples = numSamples;

		 thresholdingEnabled = TRUE;

		 return DS_Success;
}

INT32 ADCInternal::ConfigureBatchModeWithThresholding( CLR_RT_TypedArray_UINT16 sampleBuff, INT32 channel, UINT32 numSamples, UINT32 samplingTime, UINT32 threshold, HRESULT &hr )
{
	 INT32 retVal = 0;

	    // Make sure the number of samples can be held in the buffer
	    if(sampleBuff.GetSize() < numSamples)
	    	return DS_Fail;

	    // Get Buffer
	    adcManagedBuffer = sampleBuff.GetBuffer();

	    // Attempt at creating dynamic memory for the driver
	    adcNativeBuffer = (UINT16 *) private_malloc(sizeof(UINT16) * numSamples);

	    // Initialize the virtual timer
	    gHalTimerManagerObject.Initialize();

	    // Create a hal timer
	    if(!gHalTimerManagerObject.CreateTimer(ADC_VIRTUAL_TIMER, 0, samplingTime, FALSE, FALSE, ADCCallback)){ //50 milli sec Timer in micro seconds
	    			return DS_Fail;
	    }

	    adcChannel = channel;

	    adcNumSamples = numSamples;

	    batchModeADC = TRUE;

	    thresholdingEnabled = TRUE;

	    return DS_Success;
}

INT8 ADCInternal::DualChannelRead( CLR_RT_TypedArray_UINT16 param0, HRESULT &hr )
{
    return hal_adc_getData(param0.GetBuffer(), 0 , 2);
}

INT32 ADCInternal::StopSampling( HRESULT &hr )
{
    INT32 retVal = 0; 

    // Stop the timer
    gHalTimerManagerObject.StopTimer(ADC_VIRTUAL_TIMER);

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
		ADC_DEBUG_SETPINSTATE(25, TRUE);
		ADC_DEBUG_SETPINSTATE(25, FALSE);

		INT32 adcSampleValue = AD_Read((ANALOG_CHANNEL) adcChannel);

		if(thresholdingEnabled && !gThresholdingDone)
		{
			if(adcSampleValue >= gThresholdValue)
			{
				gThresholdTime = (UINT32) (HAL_Time_CurrentTicks() & 0xffffffff);
				gThresholdingDone = TRUE;
			}

		}
		adcNativeBuffer[adcBufferCounter++] = adcSampleValue;

		if(adcBufferCounter >=  adcNumSamples)
		{
		  	adcBufferCounter = 0;

		  	memcpy(adcManagedBuffer, adcNativeBuffer, adcNumSamples * sizeof(UINT16));

		  	if(thresholdingEnabled)
		  	{
		  		g_adcUserData = gThresholdTime;

		  		if(gThresholdingDone)
		  		{
		  			gThresholdingDone = FALSE;
		  		}

		  	}

		    ISR_adcProc(g_adcContext);

		    if(batchModeADC == true)
		    {
		    	gHalTimerManagerObject.StopTimer(ADC_VIRTUAL_TIMER);
		    	private_free(adcNativeBuffer);
		    }

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

