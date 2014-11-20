/*
 * hal_adc_driver.c
 *
 *  Created on: Sep 7, 2011
 *      Author: Sandip

 Would be cooler to use TIM3 trigger instead of CC
 Or use TIM1/TIM2 CC (already in use as main system timers).

 But all kinds of timing problems.
 Lots of cleanup could be done here after Nived's dual mode / continious mode addition.
 --Nathan

 */

#include <tinyhal.h>
#include <pwr/netmf_pwr.h>
#include <stm32f10x.h>
#include "hal_adc_driver.h"
#include <Samraksh/Hal_util.h>
#include <advancedtim/netmf_advancedtimer.h>

uint8_t EMOTE_ADC_CHANNEL[3] = {ADC_Channel_14, ADC_Channel_10, ADC_Channel_0};
uint32_t ADC_MODULE[3] = { ADC1_BASE, ADC2_BASE, ADC3_BASE};

void ADC_GPIO_Configuration(void);
BOOL ADC_NVIC_Configuration(void);
void ADC_RCC_Configuration(void);

extern STM32F10x_Timer_Configuration g_STM32F10x_Timer_Configuration;

HAL_CALLBACK_FPN g_callback = NULL;
static UINT64 g_timeStamp = 0;

UINT16 *g_adcUserBufferChannel1Ptr = NULL;
UINT16 *g_adcUserBufferChannel2Ptr = NULL;
UINT16 *g_adcUserBufferChannel3Ptr = NULL;

UINT16 *g_adcDriverBufferChannel1Ptr = NULL;
UINT16* g_adcDriverBufferChannel2Ptr = NULL;

UINT32 *g_adcDriverBufferDualModePtr = NULL;
UINT32 *g_adcDriverBufferDualModePtrAudio = NULL;

UINT32 adcNumSamples = 0;
//uint32_t ADC_RAW[1] = {0};

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
void DMA_HAL_HANDLER_FOR_RADAR(void *param);
void DMA_HAL_HANDLER_FOR_AUDIO(void *param);
//void TIM_HAL_HANDLER(void *param);
}


static void InitADC1()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	DMA_InitTypeDef DMA_InitStructure;

	ADC_DeInit(ADC1);

	// Setup ADC1 EOC interrupt
	NVIC_InitStruct.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,    ENABLE); //

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	//GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Init and calibrate
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	//ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));


	// Set up dma
	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (UINT32)g_adcDriverBufferChannel1Ptr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}




BOOL AD_Initialize( ANALOG_CHANNEL channel, INT32 precisionInBits )
{

	ADC_InitTypeDef           ADC_InitStructure;
	DMA_InitTypeDef           DMA_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;
	NVIC_InitTypeDef 		  NVIC_InitStruct;

	// Turn on the clocks for all the peripherals required
	ADC_RCC_Configuration();

	// Enable the appropriate gpio pins
	ADC_GPIO_Configuration();

	// Enable interrupts for dma/adc and tim
	// Note that adc and tim enables are for debugging purposes
	// The production code should only have dma
	if(!ADC_NVIC_Configuration())
		return FALSE;

	// Setup ADC1 EOC interrupt
	/*NVIC_InitStruct.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);*/

	    /* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	if(channel == 0)
		ADC_Init(ADC1, &ADC_InitStructure);
	else if(channel == 1)
		ADC_Init(ADC2, &ADC_InitStructure);
	else
		ADC_Init(ADC3, &ADC_InitStructure);

	  /* ADC1 regular channel10 configuration */
	if(channel == 0)
		ADC_RegularChannelConfig(ADC1, EMOTE_ADC_CHANNEL[0], 1, 0x04);
	else if(channel == 1)
		ADC_RegularChannelConfig(ADC2, EMOTE_ADC_CHANNEL[1], 1, 0x04);
	else
		ADC_RegularChannelConfig(ADC3, EMOTE_ADC_CHANNEL[2], 1, 0x04);

	  /* Enable ADC1 DMA */
	  //ADC_DMACmd(ADC1, ENABLE);

	  /* Enable ADC1 */
	if(channel == 0)
		ADC_Cmd(ADC1, ENABLE);
	else if(channel == 1)
		ADC_Cmd(ADC2, ENABLE);
	else
		ADC_Cmd(ADC3, ENABLE);

	  /* Enable ADC1 reset calibaration register */
	if(channel == 0)
		ADC_ResetCalibration(ADC1);
	else if(channel == 1)
		ADC_ResetCalibration(ADC2);
	else
		ADC_ResetCalibration(ADC3);

	  /* Check the end of ADC1 reset calibration register */
	if(channel == 0)
		while(ADC_GetResetCalibrationStatus(ADC1));
	else if(channel == 1)
		while(ADC_GetResetCalibrationStatus(ADC2));
	else
		while(ADC_GetResetCalibrationStatus(ADC3));

	  /* Start ADC1 calibaration */
	if(channel == 0)
		ADC_StartCalibration(ADC1);
	else if(channel == 1)
		ADC_StartCalibration(ADC2);
	else
		ADC_StartCalibration(ADC3);

	  /* Check the end of ADC1 calibration */
	if(channel == 0)
		while(ADC_GetCalibrationStatus(ADC1));
	else if(channel == 1)
		while(ADC_GetCalibrationStatus(ADC2));
	else
		while(ADC_GetCalibrationStatus(ADC3));
    return TRUE;
}

void AD_Uninitialize( ANALOG_CHANNEL channel )
{
	if(channel == 0)
		ADC_Cmd(ADC1, DISABLE);
	else if(channel == 1)
		ADC_Cmd(ADC2, DISABLE);
	else
		ADC_Cmd(ADC3, DISABLE);
}

INT32 AD_Read( ANALOG_CHANNEL channel )
{

	ADC_SoftwareStartConvCmd(((ADC_TypeDef *) ADC_MODULE[channel]), ENABLE);

	for(UINT32 i=0;i<50;i++);

	return ADC_GetConversionValue((ADC_TypeDef *) ADC_MODULE[channel]);
}

UINT32 AD_ADChannels()
{
    return NUMBER_ADC_CHANNELS;
}

GPIO_PIN AD_GetPinForChannel( ANALOG_CHANNEL channel )
{
    return GPIO_PIN_NONE;
}

BOOL AD_GetAvailablePrecisionsForChannel( ANALOG_CHANNEL channel, INT32* precisions, UINT32& size )
{
    size = 0; return FALSE;
}


// Turn on adc/dma and tim4 clocks
void ADC_RCC_Configuration(void)
{
	/* Enable peripheral clocks ------------------------------------------------*/
	/* Enable DMA1 clock */
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	 /* Enable GPIOA, GPIOC, ADC1 and TIM1 clock */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB |
		                    RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3, ENABLE);

}

BOOL ADC_NVIC_Configuration(void)
{

	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL1, DMA_HAL_HANDLER, NULL) )
		 		return FALSE;

	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_ADC1_2, ADC_HAL_HANDLER, NULL) )
	 		return FALSE;

	//if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM4, TIM_HAL_HANDLER, NULL) )
		 	//return FALSE;

	return TRUE;
}

void ADC_RCC_DUALMODE_Configuration(void)
{
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOC, ENABLE);
}

void ADC_GPIO_DUALMODE_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void ADC_GPIO_Configuration(void)
{

// Note from Nathan: I don't think you actually have to toggle the pin...
// TODO cleanup, maybe use TIM3 instead (can use ADC_ExternalTrigConv_T3_TRGO) instead of CC

	GPIO_InitTypeDef GPIO_InitStructure;

	// Output of TIM4 CH4 will trigger the adc conversion
	/* Configure TIM4_CH4 (PB9) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	// GPIO_Init(GPIOB, &GPIO_InitStructure); // Disabling to allow for GPIO output

	/* Configure PC.06 as output push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	// GPIO_Init(GPIOC, &GPIO_InitStructure); // Disabling to allow for GPIO output

	/* Configure PC.01 and PC.04 (ADC Channel11 and Channel14) as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PA.0 (ADC Channel0) as analog input */
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);*/
}

DeviceStatus AD_ConfigureContinuousMode(UINT16* sampleBuff1, UINT32 numSamples, UINT32 samplingTime, HAL_CALLBACK_FPN userCallback, void* Param)
{

	UINT32 period;
	UINT32 prescaler = 1;

	if(dmaModeInitialized)
	{
		TIM_Cmd(TIM4, ENABLE);
		return DS_Success;
	}

	// Can not do anything with a null sample buffer
	if(!sampleBuff1)
		return DS_Fail;

	// If number of samples is 0 there is nothing to do
	if(numSamples == 0)
		return DS_Fail;

	// If the callback is null there is no point initializing the drivers and
	// running the adc as the users will never to get see the data
	if(!userCallback)
		return DS_Fail;

	ADC_InitTypeDef           ADC_InitStructure;
	DMA_InitTypeDef           DMA_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	 ADC_RCC_Configuration();

	 ADC_GPIO_Configuration();

	 if(!ADC_NVIC_Configuration())
		 return DS_Fail;


	g_callback = userCallback;

	if(Param != NULL)
	{
		Param = &g_timeStamp;
	}

	// Hold the user buffer
	g_adcUserBufferChannel1Ptr = sampleBuff1;

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	if (RCC_Clocks.HCLK_Frequency == RCC_Clocks.PCLK1_Frequency) {
		// No prescaler, so TIM clock == PCLK1
		period = samplingTime * (RCC_Clocks.PCLK1_Frequency/1000000);
	}
	else {
		// Prescaler, so TIM clock = PCLK1 x 2
		period = samplingTime * (RCC_Clocks.PCLK1_Frequency*2/1000000);
	}

	// Only have 16-bit timer
	// If sampling rate is too low, period will be too large.
	// So keep doubling the prescaler and halving the period.
	// Note that actual prescaler used in timer is prescaler+1, adjust at end.
	// TODO Add error check if sampling rate is too low.

	if (period > 0xFFFF) {
		period = period >> 1;
		prescaler = 2;
	}

	while (period > 0xFFFF) {
		prescaler = prescaler << 1;
		period = period >> 1;
	}

	// Adjust
	prescaler--;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = (UINT16) period;
	TIM_TimeBaseStructure.TIM_Prescaler = (UINT16) prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	// Set up the compare channel
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = period-1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE);

#ifdef NATIVE_TEST
    g_adcDriverBufferChannel1Ptr = g_adcDriverBufferChannel1;
#else
    g_adcDriverBufferChannel1Ptr = (UINT16 *) private_malloc(sizeof(UINT16) * numSamples);
#endif


    // Set up dma
    /* DMA1 channel1 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (UINT32)g_adcDriverBufferChannel1Ptr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = numSamples;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    /* Enable DMA1 channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // Set up adc
    /* ADC1 configuration ------------------------------------------------------*/
  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channel14 configuration */
 	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);

   	  /* Enable ADC1 DMA */
   	ADC_DMACmd(ADC1, ENABLE);

  	ADC_ExternalTrigConvCmd(ADC1, ENABLE);

    	  /* Enable ADC1 */
    	  //ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

   	ADC_Cmd(ADC1, ENABLE);

    	  /* Enable ADC1 reset calibration register */
  	ADC_ResetCalibration(ADC1);
    	  /* Check the end of ADC1 reset calibration register */
   	while(ADC_GetResetCalibrationStatus(ADC1));

    	  /* Start ADC1 calibration */
   	ADC_StartCalibration(ADC1);
    	  /* Check the end of ADC1 calibration */
   	while(ADC_GetCalibrationStatus(ADC1));

    	  /* Start ADC1 Software Conversion */
   	ADC_SoftwareStartConvCmd(ADC1, ENABLE);


    TIM_Cmd(TIM4, ENABLE);

   	adcNumSamples = numSamples;

   	// Set DMA mode initialized to true
   	// This is useful in batch mode scenarious where there is a lot of starting and stopping
   	dmaModeInitialized = TRUE;

   	return DS_Success;

}

void AD_StopSampling()
{
	TIM_Cmd(TIM4, DISABLE);
}

DeviceStatus AD_ConfigureBatchMode(UINT16* sampleBuff1, UINT32 numSamples, UINT32 samplingTime, HAL_CALLBACK_FPN userCallback, void* Param)
{
	batchModeADC = TRUE;

	return AD_ConfigureContinuousMode(sampleBuff1, numSamples, samplingTime, userCallback, Param);

}


DeviceStatus AD_ConfigureBatchModeDualChannel(UINT16* sampleBuff1, UINT16* sampleBuff2, UINT32 numSamples, UINT32 samplingTime, HAL_CALLBACK_FPN userCallback, void* Param)
{
	batchModeADC = TRUE;

	return AD_ConfigureContinuousModeDualChannel(sampleBuff1, sampleBuff2, numSamples, samplingTime,userCallback, Param );
}

DeviceStatus AD_ConfigureContinuousModeDualChannel(UINT16* sampleBuff1, UINT16* sampleBuff2, UINT32 numSamples, UINT32  samplingTime, HAL_CALLBACK_FPN userCallback, void* Param)
{
	ADC_InitTypeDef           ADC_InitStructure;
	DMA_InitTypeDef           DMA_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	UINT32 period;
	UINT32 prescaler;

	ADC_RCC_Configuration();

	ADC_GPIO_Configuration();

	if(!ADC_NVIC_Configuration())
		return DS_Fail;

	g_callback = userCallback;

	if(Param != NULL)
	{
		Param = &g_timeStamp;
	}

	g_adcUserBufferChannel1Ptr = sampleBuff1;
	g_adcUserBufferChannel2Ptr = sampleBuff2;

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	if (RCC_Clocks.HCLK_Frequency == RCC_Clocks.PCLK1_Frequency) {
		// No prescaler, so TIM clock == PCLK1
		period = samplingTime * (RCC_Clocks.PCLK1_Frequency/1000000);
	}
	else {
		// Prescaler, so TIM clock = PCLK1 x 2
		period = samplingTime * (RCC_Clocks.PCLK1_Frequency*2/1000000);
	}

	prescaler = 1;

	// Only have 16-bit timer
	// If sampling rate is too low, period will be too large.
	// So keep doubling the prescaler and halving the period.
	// Note that actual prescaler used in timer is prescaler+1, adjust at end.
	// TODO Add error check if sampling rate is too low.

	if (period > 0xFFFF) {
		period = period >> 1;
		prescaler = 2;
	}

	while (period > 0xFFFF) {
		prescaler = prescaler << 1;
		period = period >> 1;
	}

	// Adjust
	prescaler--;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = (UINT16) period;
	TIM_TimeBaseStructure.TIM_Prescaler = (UINT16) prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	// Set up the compare channel
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = period-1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);


#ifdef NATIVE_TEST
    g_adcDriverBufferDualModePtr = g_adcDriverBufferDualMode;
#else
    g_adcDriverBufferDualModePtr = (UINT32 *) private_malloc(sizeof(UINT32) * numSamples);
#endif

	 /* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_adcDriverBufferDualModePtr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = numSamples;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	/* Enable DMA1 Channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	 /* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	 /* ADC1 regular channels configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);

	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	   /* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	 /* ADC2 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);
	/* ADC2 regular channels configuration */
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);

	/* Enable ADC2 external trigger conversion */
	ADC_ExternalTrigConvCmd(ADC2, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

	/* Enable ADC2 */
	ADC_Cmd(ADC2, ENABLE);

	/* Enable ADC2 reset calibration register */
	ADC_ResetCalibration(ADC2);
	/* Check the end of ADC2 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC2));

	/* Start ADC2 calibration */
	ADC_StartCalibration(ADC2);
	/* Check the end of ADC2 calibration */
	while(ADC_GetCalibrationStatus(ADC2));

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	adcNumSamples = numSamples;

	dualADCMode = TRUE;


	TIM_Cmd(TIM4, ENABLE);

	return DS_Success;
}


DeviceStatus AD_ConfigureScanModeThreeChannels(UINT16* sampleBuff1, UINT16* sampleBuff2, UINT16* sampleBuff3, UINT32 numSamples, UINT32  samplingTime, HAL_CALLBACK_FPN userCallback, void* Param)
{
	ADC_InitTypeDef           ADC_InitStructure;
	DMA_InitTypeDef           DMA_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	UINT32 period;
	UINT32 prescaler;

	ADC_RCC_Configuration();

	ADC_GPIO_Configuration();


	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL1, DMA_HAL_HANDLER_FOR_RADAR, NULL) )
		return DS_Fail;

	//if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_DMA2_Channel4_5, DMA_HAL_HANDLER_FOR_AUDIO, NULL) )
		//return DS_Fail;

	//if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_DMA2_Channel4_5))
		//return DS_Fail;

	//if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_ADC1_2, ADC_HAL_HANDLER, NULL) )
		//return DS_Fail;

	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_ADC3, ADC_HAL_HANDLER, NULL) )
		return DS_Fail;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_ADC3))
		return DS_Fail;

	//if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM3, TIM_HAL_HANDLER, NULL) )
		//return DS_Fail;

	//if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM8_BRK_TIM12, TIM_HAL_HANDLER, NULL) )
		//return DS_Fail;

	/*if(!ADC_NVIC_Configuration())
		return DS_Fail;*/

	g_callback = userCallback;

	if(Param != NULL)
	{
		Param = &g_timeStamp;
	}

	g_adcUserBufferChannel1Ptr = sampleBuff1;
	g_adcUserBufferChannel2Ptr = sampleBuff2;
	g_adcUserBufferChannel3Ptr = sampleBuff3;



	/*RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	if (RCC_Clocks.HCLK_Frequency == RCC_Clocks.PCLK1_Frequency) {
		// No prescaler, so TIM clock == PCLK1
		period = samplingTime * (RCC_Clocks.PCLK1_Frequency/1000000);
	}
	else {
		// Prescaler, so TIM clock = PCLK1 x 2
		period = samplingTime * (RCC_Clocks.PCLK1_Frequency*2/1000000);
	}

	prescaler = 1;

	// Only have 16-bit timer
	// If sampling rate is too low, period will be too large.
	// So keep doubling the prescaler and halving the period.
	// Note that actual prescaler used in timer is prescaler+1, adjust at end.
	// TODO Add error check if sampling rate is too low.
	if (period > 0xFFFF) {
		period = period >> 1;
		prescaler = 2;
	}

	while (period > 0xFFFF) {
		prescaler = prescaler << 1;
		period = period >> 1;
	}

	// Adjust
	prescaler--;*/

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	//-------------------------TIMER SETUP---------------------------------------------------------------------
	//AnanthAtSamraksh - setup a timer for Radar (TIM4)
	TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Period = 32000;	//250Hz @ 8MHz
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);

	// Set up the compare channel
	/*TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 32000-1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);*/


	//AnanthAtSamraksh - setup a timer for Audio (TIM3)
	TIM_DeInit(TIM8);
	TIM_TimeBaseStructure.TIM_Period = 4000;	//2KHz @ 8MHz
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);

	// Set up the compare channel
	/*TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 16000-1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);*/
	//-------------------------------------------------------------------------------------------------------------------




#ifdef NATIVE_TEST
    g_adcDriverBufferDualModePtr = g_adcDriverBufferDualMode;
    g_adcDriverBufferDualModePtrAudio = g_adcDriverBufferDualMode;
#else
    g_adcDriverBufferDualModePtr = (UINT32 *) private_malloc((sizeof(UINT32) * 2) * numSamples);
    //g_adcDriverBufferDualModePtr = (UINT32 *) private_malloc(3 * numSamples);
    //g_adcDriverBufferDualModePtrAudio = (UINT32 *) private_malloc(sizeof(UINT32) * numSamples);
#endif

    //-------------------------DMA SETUP------------------------------------------------------------------------
    //AnanthAtSamraksh - setup DMA for radar
	/* DMA1 channel1 configuration for ADC1 & 2 ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_adcDriverBufferDualModePtr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2 * numSamples;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel1, DMA1_IT_HT1, ENABLE);
	//DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);
	// Enable DMA1 Channel1
	DMA_Cmd(DMA1_Channel1, ENABLE);


	//AnanthAtSamraksh - setup DMA for audio
	/* DMA2 channel5 configuration for ADC3 ----------------------------------------------*/
	/*DMA_DeInit(DMA2_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_adcDriverBufferDualModePtrAudio;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = numSamples;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);

	//DMA_ITConfig(DMA2_Channel5, DMA2_IT_HT5, ENABLE);
	DMA_ITConfig(DMA2_Channel5, DMA2_IT_TC5, ENABLE);
	// Enable DMA2 Channel5
	DMA_Cmd(DMA2_Channel5, ENABLE);*/
	//-------------------------------------------------------------------------------------------------------------------



	 /* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	//ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	 /* ADC1 regular channels configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	   /* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);


	 /* ADC2 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);
	/* ADC2 regular channels configuration */
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
	/* Enable ADC2 external trigger conversion */
	ADC_ExternalTrigConvCmd(ADC2, ENABLE);



	//AnanthAtSamraksh - Configuring new channel
	/* ADC3 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T8_TRGO;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE);

	/* ADC3 regular channels configuration */
	ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	/* Enable ADC3 external trigger conversion */
	ADC_ExternalTrigConvCmd(ADC3, ENABLE);
	/* Enable ADC3 DMA */
	//ADC_DMACmd(ADC3, ENABLE);



	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* Enable ADC1 reset calibration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));


	/* Enable ADC2 */
	ADC_Cmd(ADC2, ENABLE);
	/* Enable ADC2 reset calibration register */
	ADC_ResetCalibration(ADC2);
	/* Check the end of ADC2 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC2));
	/* Start ADC2 calibration */
	ADC_StartCalibration(ADC2);
	/* Check the end of ADC2 calibration */
	while(ADC_GetCalibrationStatus(ADC2));


	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);
	/* Enable ADC3 reset calibration register */
	ADC_ResetCalibration(ADC3);
	/* Check the end of ADC3 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC3));
	/* Start ADC3 calibration */
	ADC_StartCalibration(ADC3);
	/* Check the end of ADC3 calibration */
	while(ADC_GetCalibrationStatus(ADC3));



	/* Start ADC1 Software Conversion */
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	/* Start ADC3 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);

	adcNumSamples = numSamples;

	dualADCMode = TRUE;

	//TIM_Cmd(TIM3, ENABLE);		//Radar
	//TIM_Cmd(TIM8, ENABLE);		//Audio

	return DS_Success;
}


void ADC_Configuration ( uint8_t sampTime )
{
  ADC_InitTypeDef ADC_InitStructure;

    /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Init(ADC2, &ADC_InitStructure);
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC1 regular channel10 configuration */
  ADC_RegularChannelConfig(ADC1, EMOTE_ADC_CHANNEL[0], 1, sampTime);
  ADC_RegularChannelConfig(ADC2, EMOTE_ADC_CHANNEL[1], 1, sampTime);
  ADC_RegularChannelConfig(ADC3, EMOTE_ADC_CHANNEL[2], 1, sampTime);

  /* Enable ADC1 DMA */
  //ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  ADC_Cmd(ADC3, ENABLE);

  /* Enable ADC1 reset calibaration register */
  ADC_ResetCalibration(ADC1);
  ADC_ResetCalibration(ADC2);
  ADC_ResetCalibration(ADC3);

  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  while(ADC_GetResetCalibrationStatus(ADC2));
  while(ADC_GetResetCalibrationStatus(ADC3));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  ADC_StartCalibration(ADC2);
  ADC_StartCalibration(ADC3);

  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
  while(ADC_GetCalibrationStatus(ADC2));
  while(ADC_GetCalibrationStatus(ADC3));
}

void hal_adc_init(uint8_t sampTime)
{
	ADC_RCC_Configuration();
	ADC_NVIC_Configuration();
	ADC_GPIO_Configuration();
	ADC_Configuration(sampTime);
}

uint8_t hal_adc_getData(uint16_t *dataBuf, uint8_t startChannel, uint8_t numChannels)
{
	uint8_t i,j;

	if (startChannel > 2 || (startChannel + numChannels > 3))
		return 0;

	for (j=startChannel; j < startChannel+numChannels; j++)
	{
		ADC_SoftwareStartConvCmd(((ADC_TypeDef *) ADC_MODULE[j]), ENABLE);
	}

	for(i=0;i<50;i++);

	for (j=0; j<numChannels; j++)
	{
		dataBuf[j] = ADC_GetConversionValue((ADC_TypeDef *) ADC_MODULE[j+startChannel]);
	}

	return 1;
}


extern "C"
{
	void ADC_HAL_HANDLER(void *param)
	{
		//hal_printf("ADC_HAL_HANDLER\n");
		static int buffer_index = 0;
		buffer_index++;
		if(buffer_index % 10 == 0)
		{
			g_timeStamp = HAL_Time_CurrentTicks();
			//TIM_Cmd(TIM8, ENABLE);		//Audio
			memset(g_adcUserBufferChannel3Ptr, 0, adcNumSamples);
			//hal_printf("g_adcUserBufferChannel3Ptr: %d; buffer_index: %d \n", *g_adcUserBufferChannel3Ptr, buffer_index);
			for(UINT16 i = 0; i < adcNumSamples; i++)
			{
				g_adcUserBufferChannel3Ptr[i] = ADC_GetConversionValue(ADC3);
			}
			g_callback(&g_timeStamp);
		}
		//else if(buffer_index % 10 == 0)
			//TIM_Cmd(TIM8, DISABLE);		//Audio

		//hal_printf("ADC_HAL_HANDLER: %d\n", *g_adcUserBufferChannel3Ptr);
		/* Clear ADC1 JEOC pending interrupt bit */
		//ADC_ClearITPendingBit(ADC3, ADC_IT_EOC);		//Does not make any difference

	}

	void DMA_HAL_HANDLER_FOR_RADAR(void *param)
	{
		//hal_printf("DMA_HAL_HANDLER_FOR_RADAR\n");
		// Record the time as close to the completion of sampling as possible
		g_timeStamp = HAL_Time_CurrentTicks();

		if(DMA_GetFlagStatus(DMA1_FLAG_HT1) != RESET)
		{
			DMA_ClearITPendingBit(DMA1_FLAG_HT1);

			if(!dualADCMode)
			{
				memcpy(g_adcUserBufferChannel1Ptr, g_adcDriverBufferDualModePtr, adcNumSamples * sizeof(UINT16));
			}
			else
			{
				// Possible memory corruption if adcNumSamples != length of g_adcDriverBufferDualModePtr
				// Possible only while conducting native tests as C# programs use dynamic memory
				// allocated from the heap
				// BTW this is because the hardware in dual mode stores in the most significant bits of
				// ADC1_DR the result of ADC2 and the least significant bits of ADC1_DR the result
				// of ADC1. Pretty cool  !!!

				for(UINT16 i = 0; i < adcNumSamples; i++)
				{
					//hal_printf("Radar; g_adcDriverBufferDualModePtr[i]: %d \n", g_adcDriverBufferDualModePtr[i]);
					g_adcUserBufferChannel1Ptr[i] = (g_adcDriverBufferDualModePtr[i] & 0xffff);
					g_adcUserBufferChannel2Ptr[i] = (g_adcDriverBufferDualModePtr[i] >> 16);
					//hal_printf("Radar; g_adcUserBufferChannel1Ptr[i]: %d; g_adcUserBufferChannel2Ptr[i]: %d \n", g_adcUserBufferChannel1Ptr[i], g_adcUserBufferChannel2Ptr[i]);
					//g_adcUserBufferChannel3Ptr[i] = (g_adcDriverBufferDualModePtrAudio[i] >> 16);
				}

				/*static int buffer_index = 0;
				buffer_index++;
				//Sample audio and I channel
				if(buffer_index % 32 == 0)
				{
					//g_adcUserBufferChannel1Ptr = (uint16_t*)ADC_RAW[0];
					for(UINT16 i = 0; i < adcNumSamples; i++)
					{
						g_adcUserBufferChannel1Ptr[i] = (g_adcDriverBufferDualModePtr[i] & 0xffff);
						g_adcUserBufferChannel2Ptr[i] = (g_adcDriverBufferDualModePtr[i] >> 16);
						//g_adcUserBufferChannel3Ptr[i] = (g_adcDriverBufferDualModePtrAudio[i] >> 16);
						//hal_printf("audio and I; g_adcDriverBufferDualModePtr[i]: %d \n", g_adcDriverBufferDualModePtr[i]);
						g_adcUserBufferChannel3Ptr[i] = (g_adcDriverBufferDualModePtrAudio[i] & 0xffff);
					}
				}*/
				//Sample audio and Q channel
				/*else if(buffer_index % 32 == 16)
				{
					//g_adcUserBufferChannel2Ptr = (uint16_t*)ADC_RAW[1];
					for(UINT16 i = 0; i < adcNumSamples; i++)
					{
						g_adcUserBufferChannel2Ptr[i] = (g_adcDriverBufferDualModePtr[i] >> 16);
						//hal_printf("audio and Q; g_adcDriverBufferDualModePtr[i]: %d \n", g_adcDriverBufferDualModePtr[i]);
						g_adcUserBufferChannel3Ptr[i] = (g_adcDriverBufferDualModePtrAudio[i] & 0xffff);
					}
				}*/
				//Sample audio alone
				/*else
				{
					//g_adcUserBufferChannel3Ptr = (uint16_t*)ADC_RAW[2];
					for(UINT16 i = 0; i < adcNumSamples; i++)
					{
						//hal_printf("audio alone; buffer_index: %d; g_adcDriverBufferDualModePtrAudio[i]: %d \n", buffer_index, g_adcDriverBufferDualModePtrAudio[i]);
						g_adcUserBufferChannel3Ptr[i] = (g_adcDriverBufferDualModePtrAudio[i] & 0xffff);
					}
				}*/
			}

			// Call the user with the current value of ticks
			//if(g_adcUserBufferChannel1Ptr[adcNumSamples] != 0 || g_adcUserBufferChannel2Ptr[adcNumSamples] != 0)
				g_callback(&g_timeStamp);

			//hal_printf("DMA_HAL_HANDLER_THREE_CHANNELS\n");

			if(batchModeADC)
			{
				// Disable the trigger if the batch mode is true
				TIM_Cmd(TIM3, DISABLE);
				TIM_Cmd(TIM8, DISABLE);
			}
		}
	}


	void DMA_HAL_HANDLER_FOR_AUDIO(void *param)
	{
		hal_printf("DMA_HAL_HANDLER_FOR_AUDIO\n");
		// Record the time as close to the completion of sampling as possible
		g_timeStamp = HAL_Time_CurrentTicks();

		if(DMA_GetFlagStatus(DMA2_FLAG_TC5) != RESET)
		{
			DMA_ClearITPendingBit(DMA2_IT_TC5);

			if(!dualADCMode)
			{
				memcpy(g_adcUserBufferChannel3Ptr, g_adcDriverBufferDualModePtrAudio, adcNumSamples * sizeof(UINT16));
			}
			else
			{
				// Possible memory corruption if adcNumSamples != length of g_adcDriverBufferDualModePtr
				// Possible only while conducting native tests as C# programs use dynamic memory
				// allocated from the heap
				// BTW this is because the hardware in dual mode stores in the most significant bits of
				// ADC1_DR the result of ADC2 and the least significant bits of ADC1_DR the result
				// of ADC1. Pretty cool  !!!

				//Sample audio alone

				//g_adcUserBufferChannel3Ptr = (uint16_t*)ADC_RAW[2];
				for(UINT16 i = 0; i < adcNumSamples; i++)
				{
					//hal_printf("audio ; g_adcDriverBufferDualModePtrAudio[i]: %d \n", g_adcDriverBufferDualModePtrAudio[i]);
					g_adcUserBufferChannel3Ptr[i] = (g_adcDriverBufferDualModePtrAudio[i] & 0xffff);
					//hal_printf("audio ; g_adcUserBufferChannel3Ptr[i]: %d; g_adcDriverBufferDualModePtrAudio[i] >> 16: %d \n", g_adcUserBufferChannel3Ptr[i], g_adcDriverBufferDualModePtrAudio[i] >> 16);
				}
			}

			// Call the user with the current value of ticks
			//if(g_adcUserBufferChannel3Ptr[adcNumSamples] != 0)
				g_callback(&g_timeStamp);

			//hal_printf("DMA_HAL_HANDLER_THREE_CHANNELS\n");

			if(batchModeADC)
			{
				// Disable the trigger if the batch mode is true
				TIM_Cmd(TIM3, DISABLE);
				TIM_Cmd(TIM8, DISABLE);
			}
		}
	}


	void DMA_HAL_HANDLER(void *param)
	{
		// Record the time as close to the completion of sampling as possible
		g_timeStamp = HAL_Time_CurrentTicks();

		if(DMA_GetFlagStatus(DMA1_FLAG_TC1) != RESET)
		{
			DMA_ClearITPendingBit(DMA1_IT_TC1);

			if(!dualADCMode)
			{
				memcpy(g_adcUserBufferChannel1Ptr, g_adcDriverBufferChannel1Ptr, adcNumSamples * sizeof(UINT16));
			}
			else
			{
				// Possible memory corruption if adcNumSamples != length of g_adcDriverBufferDualModePtr
				// Possible only while conducting native tests as C# programs use dynamic memory
				// allocated from the heap
				// BTW this is because the hardware in dual mode stores in the most significant bits of
				// ADC1_DR the result of ADC2 and the least significant bits of ADC1_DR the result
				// of ADC1. Pretty cool  !!!
				for(UINT16 i = 0; i < adcNumSamples; i++)
				{
					g_adcUserBufferChannel1Ptr[i] = (g_adcDriverBufferDualModePtr[i] & 0xffff);
					g_adcUserBufferChannel2Ptr[i] = (g_adcDriverBufferDualModePtr[i] >> 16);
				}
			}

			// Call the user with the current value of ticks
			g_callback(&g_timeStamp);

			//hal_printf("DMA_HAL_HANDLER\n");

			if(batchModeADC)
			{
				// Disable the trigger if the batch mode is true
				TIM_Cmd(TIM4, DISABLE);
			}

		}
	}

	/*void TIM_HAL_HANDLER(void *param)
	{
		//hal_printf("TIM_HAL_HANDLER\n");
		//if(TIM_GetFlagStatus(TIM4, TIM_IT_CC4))
		if(TIM_GetITStatus(TIM4, TIM_IT_CC4))
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		}
	}*/
}

