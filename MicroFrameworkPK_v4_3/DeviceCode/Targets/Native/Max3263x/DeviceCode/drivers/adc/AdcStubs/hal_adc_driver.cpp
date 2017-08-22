/*
 * hal_adc_driver.c
 *
 *  Created on: Sep 7, 2011
 *      Author: Sandip

 Would be cooler to use TIM3 trigger instead of CC
 Or use TIM1/TIM2 CC (already in use as main system timers).

 But all kinds of timing problems.
 Lots of cleanup could be done here after Nived's dual mode / continuous mode addition.
 --Nathan



 */

#include <tinyhal.h>
#include "hal_adc_driver.h"
#include <Samraksh/Hal_util.h>
#include "../Include/Samraksh/VirtualTimer.h"

uint8_t EMOTE_ADC_CHANNEL[3] = {0, 0, 0};
uint32_t ADC_MODULE[3] = { 0, 0, 0};

void ADC_GPIO_Configuration(BOOL enable);
BOOL ADC_NVIC_Configuration(BOOL enable);
void ADC_RCC_Configuration(BOOL enable);
static double callbacksPerSecond = 0;

//TODO: Why do we use g_* when the variables are not extern?
HAL_CALLBACK_FPN g_callback = NULL;
static UINT64 g_timeStamp = 0;

UINT16 *g_adcUserBufferChannel1Ptr = NULL;
UINT16 *g_adcUserBufferChannel2Ptr = NULL;

UINT16 *g_adcDriverBufferChannel1Ptr = NULL;
//UINT16* g_adcDriverBufferChannel2Ptr = NULL; // TODO: Remove. Obsolete.

UINT32 *g_adcDriverBufferDualModePtr = NULL;

UINT32 adcNumSamples = 0;
UINT32 adcSamplingTime = 0;

static UINT32 adcPeriodicOnTimeUs = 1000;
static UINT32 adcPeriodicOffTimeUs = 1000;
static bool adcPeriodicOnTimeRunning = true;
// TODO: consolidate state machine into one variable.
BOOL batchModeADC = FALSE;
BOOL dmaModeInitialized = FALSE;
BOOL dualADCMode = FALSE;

#ifdef NATIVE_TEST
UINT16 g_adcDriverBufferChannel1[1000];
UINT32 g_adcDriverBufferDualMode[1000];
#endif

extern "C"
{
void ADC_HAL_HANDLER(void *param);
void DMA_HAL_HANDLER(void *param);
void TIM_HAL_HANDLER(void *param);
}

/**
 * interface for Microsoft.SPOT.Hardware.AnalogInput
 */
BOOL AD_Initialize( ANALOG_CHANNEL channel, INT32 precisionInBits )
{

	return TRUE;
}

void AD_Uninitialize( ANALOG_CHANNEL channel )
{

}

BOOL AD_Uninitialize(void)
{


	return TRUE;
}

INT32 AD_Read( ANALOG_CHANNEL channel )
{

	return true;
}

UINT32 AD_ADChannels()
{
    return 0;
}

GPIO_PIN AD_GetPinForChannel( ANALOG_CHANNEL channel )
{
    return 0;
}

BOOL AD_GetAvailablePrecisionsForChannel( ANALOG_CHANNEL channel, INT32* precisions, UINT32& size )
{
    size = 0; return FALSE;
}


/**
 *  Turn on adc/dma and tim4 clocks
 *  @TODO: check for resource sharing before disable.
 */
void ADC_RCC_Configuration(BOOL enable)
{


}

BOOL ADC_NVIC_Configuration(BOOL enable)
{
	BOOL retInit = TRUE;
	BOOL retCleanup = TRUE;

	BOOL retFinal = (retCleanup && retInit);
	return retFinal;
}

void ADC_GPIO_Configuration(BOOL enable)
{

}

DeviceStatus AD_ConfigureContinuousMode(UINT16* sampleBuff1, UINT32 numSamples, UINT32 samplingTime, HAL_CALLBACK_FPN userCallback, void* Param)
{
	DeviceStatus retVal = DS_Success;

	return retVal;
}

void AD_StopSampling()
{

}

double AD_GetCallbacksPerSecond()
{
	return 0;
}

void ISR_ADC_PERIOD (void* Param)
{

}

INT32 AD_SetPeriodicSamplingInterval(double secondsADCSamples, double secondsADCsleeps)
{

	

	return true;
}

DeviceStatus AD_ConfigureBatchMode(UINT16* sampleBuff1, UINT32 numSamples, UINT32 samplingTime, HAL_CALLBACK_FPN userCallback, void* Param)
{
	return DS_Success;
}


DeviceStatus AD_ConfigureBatchModeDualChannel(UINT16* sampleBuff1, UINT16* sampleBuff2, UINT32 numSamples, UINT32 samplingTime, HAL_CALLBACK_FPN userCallback, void* Param)
{
	return DS_Success;
}

DeviceStatus AD_ConfigureContinuousModeDualChannel(UINT16* sampleBuff1, UINT16* sampleBuff2, UINT32 numSamples, UINT32  samplingTime, HAL_CALLBACK_FPN userCallback, void* Param)
{


	return DS_Success;

}


void ADC_Configuration ( uint8_t sampTime )
{
  
}


void hal_adc_init(uint8_t sampTime)
{

}

uint8_t hal_adc_getData(uint16_t *dataBuf, uint8_t startChannel, uint8_t numChannels)
{


	return 1;
}

extern "C"
{
	void ADC_HAL_HANDLER(void *param)
	{

	}

	void DMA_HAL_HANDLER(void *param)
	{
		
	}

	void TIM_HAL_HANDLER(void *param)
	{

	}
}

