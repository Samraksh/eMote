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
#include <stm32f10x.h>
#include "hal_adc_driver.h"
#include <Samraksh/Hal_util.h>
#include <Timer/advancedtimer/netmf_advancedtimer.h>

uint8_t EMOTE_ADC_CHANNEL[3] = {ADC_Channel_14, ADC_Channel_10, ADC_Channel_0};
uint32_t ADC_MODULE[3] = { ADC1_BASE, ADC2_BASE, ADC3_BASE};

void ADC_GPIO_Configuration(BOOL enable);
BOOL ADC_NVIC_Configuration(BOOL enable);
void ADC_RCC_Configuration(BOOL enable);

//TODO: Why do we use g_* when the variables are not extern?
HAL_CALLBACK_FPN g_callback = NULL;
static UINT64 g_timeStamp = 0;

UINT16 *g_adcUserBufferChannel1Ptr = NULL;
UINT16 *g_adcUserBufferChannel2Ptr = NULL;

UINT16 *g_adcDriverBufferChannel1Ptr = NULL;
//UINT16* g_adcDriverBufferChannel2Ptr = NULL; // TODO: Remove. Obsolete.

UINT32 *g_adcDriverBufferDualModePtr = NULL;

UINT32 adcNumSamples = 0;
static UINT32 adcNumSamplesRadar = 0;

static UINT32 adDebugMode = 0;
// TODO: consolidate state machine into one variable.
BOOL batchModeADC = FALSE;
BOOL dmaModeInitialized = FALSE;
BOOL dualADCMode = FALSE;

#ifdef NATIVE_TEST
UINT16 g_adcDriverBufferChannel1[1000];
UINT32 g_adcDriverBufferDualMode[1000];
#endif

void ADC_HAL_HANDLER(void *param);

/**
 * interface for Microsoft.SPOT.Hardware.AnalogInput
 */
BOOL AD_Initialize( ANALOG_CHANNEL channel, INT32 precisionInBits )
{

	ADC_InitTypeDef           ADC_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	// Turn on the clocks for all the peripherals required
	ADC_RCC_Configuration(TRUE);

	// Enable the appropriate gpio pins
	ADC_GPIO_Configuration(TRUE);

	// Enable interrupts for dma/adc and tim
	// Note that adc and tim enables are for debugging purposes
	// The production code should only have dma
	if(!ADC_NVIC_Configuration(TRUE))
		return FALSE;

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


/**
 *  Turn on adc/dma and tim4 clocks
 *  @TODO: check for resource sharing before disable.
 */
void ADC_RCC_Configuration(BOOL enable)
{
	/* Enable peripheral clocks ------------------------------------------------*/

	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, (FunctionalState)enable);

	/* Enable all possibly needed clocks */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
	                        RCC_APB2Periph_GPIOB |
	                        RCC_APB2Periph_GPIOC |
	                        RCC_APB2Periph_ADC1  |
	                        RCC_APB2Periph_ADC2,   (FunctionalState)enable);

}

BOOL ADC_NVIC_Configuration(BOOL enable)
{
	BOOL retInit = TRUE;
	BOOL retCleanup = TRUE;
	if(enable == TRUE) {
		if((retInit &= CPU_INTC_ActivateInterrupt(ADC1_2_IRQn,        ADC_HAL_HANDLER, NULL)) != TRUE) {
			goto adc_nvic_cleanup_2;
		}
	}
	else { /* enable == FALSE */
adc_nvic_cleanup_3:
		retCleanup &= CPU_INTC_DeactivateInterrupt(TIM4_IRQn);
adc_nvic_cleanup_2:
		retCleanup &= CPU_INTC_DeactivateInterrupt(ADC1_2_IRQn);
	}
	BOOL retFinal = (retCleanup && retInit);
	return retFinal;
}

void ADC_GPIO_Configuration(BOOL enable)
{

// Note from Nathan: I don't think you actually have to toggle the pin...
// TODO cleanup, maybe use TIM3 instead (can use ADC_ExternalTrigConv_T3_TRGO) instead of CC

	GPIO_InitTypeDef GPIO_InitStructure;
	if(enable == TRUE)
	{
		/* Configure PC.01 and PC.04 (ADC Channel11 and Channel14) as analog input */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		//GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_ConfigurePin(GPIOC, GPIO_InitStructure.GPIO_Pin, GPIO_InitStructure.GPIO_Mode);
	}
	else /* enable == FALSE */
	{
		CPU_GPIO_DisablePin(GPIO_GetNumber(GPIOC, GPIO_Pin_0), RESISTOR_DISABLED, GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
		CPU_GPIO_DisablePin(GPIO_GetNumber(GPIOC, GPIO_Pin_1), RESISTOR_DISABLED, GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
		CPU_GPIO_DisablePin(GPIO_GetNumber(GPIOC, GPIO_Pin_4), RESISTOR_DISABLED, GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
	}
}

DeviceStatus AD_ConfigureContinuousMode(UINT16* sampleBuff1, UINT32 numSamples, UINT32 samplingTime, HAL_CALLBACK_FPN userCallback, void* Param)
{
	DeviceStatus retVal = DS_Success;
	UINT32 period;
	UINT32 prescaler = 1;

	// check if already initialized.
	if(dmaModeInitialized)
	{
		TIM_Cmd(TIM4, ENABLE);
		retVal = DS_Success;
		goto ad_ccm_out;
	}

	// Can not do anything with a null sample buffer
	if(!sampleBuff1)
	{
		retVal = DS_Fail;
	}

	// If number of samples is 0 there is nothing to do
	if(numSamples == 0)
	{
		retVal = DS_Fail;
	}

	// If the callback is null there is no point initializing the drivers and
	// running the adc as the users will never to get see the data
	if(!userCallback)
	{
		retVal = DS_Fail;
	}

	if(retVal == DS_Fail)
	{
		goto ad_ccm_out;
	}

	ADC_InitTypeDef           ADC_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	ADC_RCC_Configuration(TRUE);

	ADC_GPIO_Configuration(TRUE);

	if(!ADC_NVIC_Configuration(TRUE))
	{
		retVal = DS_Fail;
		goto ad_ccm_out_config;
	}

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
	if(g_adcDriverBufferChannel1Ptr != NULL)
	{
		SOFT_BREAKPOINT();
		private_free(g_adcDriverBufferChannel1Ptr);
	}
	g_adcDriverBufferChannel1Ptr = (UINT16 *) private_malloc(sizeof(UINT16) * numSamples);
	if(g_adcDriverBufferChannel1Ptr == NULL)
	{
		ASSERT(0);
		retVal = DS_Fail;
		goto ad_ccm_out;
	}
#endif


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

    ADC_ExternalTrigConvCmd(ADC1, ENABLE);

    /* Enable ADC1 */
    //ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

    ADC_Cmd(ADC1, ENABLE);

    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));	// FIXME: add timeout

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));	//FIXME: add timeout

    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//FIXME: add timeout


    TIM_Cmd(TIM4, ENABLE);

   	adcNumSamples = numSamples;

ad_ccm_out_config:
    if(retVal == DS_Fail)
    {
        ADC_NVIC_Configuration(FALSE);
        ADC_GPIO_Configuration(FALSE);
        ADC_RCC_Configuration(FALSE);
    }
ad_ccm_out:
	if(dmaModeInitialized && retVal == DS_Fail)
	{
		TIM_Cmd(TIM4, DISABLE);
	}
	if(g_adcDriverBufferChannel1Ptr != NULL && retVal == DS_Fail)
	{
		private_free(g_adcDriverBufferChannel1Ptr);
	}
	return retVal;
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

	return AD_ConfigureContinuousModeDualChannel(sampleBuff1, sampleBuff2, numSamples, samplingTime, 0, userCallback, Param);
}

DeviceStatus AD_ConfigureContinuousModeDualChannel(UINT16* sampleBuff1, UINT16* sampleBuff2, UINT32 numSamples, UINT32  samplingTime, UINT32 debugMode, HAL_CALLBACK_FPN userCallback, void* Param)
{
	ADC_InitTypeDef           ADC_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	UINT32 period;
	UINT32 prescaler;

	adDebugMode = debugMode;

	ADC_RCC_Configuration(TRUE);

	ADC_GPIO_Configuration(TRUE);

	if(!ADC_NVIC_Configuration(TRUE))
		return DS_Fail;

	g_callback = userCallback;

	if(Param != NULL)
	{
		Param = &g_timeStamp;
	}

	g_adcUserBufferChannel1Ptr = sampleBuff1;
	g_adcUserBufferChannel2Ptr = sampleBuff2;

	

	if (adDebugMode == 1){
		adcNumSamplesRadar = numSamples;
		hal_printf("ADC debug mode\r\n");
	} else {
		hal_printf("ADC debug mode OFF\r\n");
	}

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
	if(g_adcDriverBufferDualModePtr != NULL) {
		ASSERT(0);
		private_free(g_adcDriverBufferDualModePtr);
	}
	g_adcDriverBufferDualModePtr = (UINT32 *) private_malloc(sizeof(UINT32) * numSamples);
	if(g_adcDriverBufferDualModePtr == NULL) {
		ASSERT(0);
		return DS_Fail;
	}
#endif

	 
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
		ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

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

void ADC_HAL_HANDLER(void *param)
{
	static uint32_t count=0;
	
	if (adDebugMode == 1){
		g_adcUserBufferChannel1Ptr[count] = ADC_GetConversionValue(ADC1);
		g_adcUserBufferChannel2Ptr[count] = ADC_GetConversionValue(ADC2);
	
		USART_Write( 0, (char *)&g_adcUserBufferChannel1Ptr[count], 2 );
		USART_Write( 0, (char *)&g_adcUserBufferChannel2Ptr[count], 2 );

		count++;
		if (count == adcNumSamplesRadar) {
			
			{ // Nathan's hack to peek at the pin to determine if we need to reset to Bootloader. SPRING CAMP ONLY.
				GPIO_InitTypeDef GPIO_InitStruct;
				GPIO_InitStruct.GPIO_Pin  	= GPIO_Pin_14;
				GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_2MHz;
				GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_IPD;
				GPIO_Init(GPIOB, &GPIO_InitStruct);
				
				unsigned ret = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
				
				// This will bring us to STM hardware bootloader.
				if (ret) { NVIC_SystemReset(); }
				
				// Put the pin back to output.
				// Zero it first so we don't get another edge.
				// Probably will happen anyway.
				GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_Out_PP;
				GPIO_ResetBits(GPIOB, GPIO_Pin_14);
				GPIO_Init(GPIOB, &GPIO_InitStruct);
			}
			
			g_timeStamp = HAL_Time_CurrentTicks();
			g_callback(&g_timeStamp);
			count=0;
		}
	}
}

// This is in Samraksh C# ADC API. What the hell is this for?
uint8_t hal_adc_getData(uint16_t *dataBuf, uint8_t startChannel, uint8_t numChannels)
{
	uint8_t i,j;

	if (startChannel > 2 || (startChannel + numChannels > 3))
	{
		ASSERT(0);
		return 0;
	}

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
