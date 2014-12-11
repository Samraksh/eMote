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

 Took a knife to this to clean it up for the demo.
 Some of the ideas can be pushed forward.

 */

#include <tinyhal.h>
#include <stm32f10x.h>
#include "hal_adc_driver.h"

//#define DEBUG_3_CHAN
//#define USE_DMA_AUDIO

uint8_t EMOTE_ADC_CHANNEL[3] = {ADC_Channel_14, ADC_Channel_10, ADC_Channel_0};
uint32_t ADC_MODULE[3] = { ADC1_BASE, ADC2_BASE, ADC3_BASE};

static void ADC_GPIO_Configuration(void);
static BOOL ADC_NVIC_Configuration(void);
static void ADC_RCC_Configuration(void);

static void ADC_HAL_HANDLER(void *param);
static void DMA_HAL_HANDLER_FOR_RADAR(void *param);

#ifdef USE_DMA_AUDIO
static void DMA_HAL_HANDLER_FOR_AUDIO(void *param);
#endif

static HAL_CALLBACK_FPN g_callback = NULL;
static UINT64 g_timeStamp = 0;

static UINT16 *radar_I_return = NULL;
static UINT16 *radar_Q_return = NULL;
static UINT16 *audio_return = NULL;

static UINT32 adcNumSamples = 0;
static UINT32 adcNumSamplesRadar = 0;
static UINT32 adcNumSamplesAudio = 0;

static UINT16 *audioBuffer;
static UINT32 *radarBuffer;
//static BOOL pingpong = FALSE; // ping=0 pong=1


// Turn on adc/dma and tim4 clocks
static void ADC_RCC_Configuration(void)
{
	/* Enable peripheral clocks ------------------------------------------------*/
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#ifdef USE_DMA_AUDIO
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
#endif

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB |
		RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3, ENABLE);
}

static BOOL ADC_NVIC_Configuration(void)
{

	if( !CPU_INTC_ActivateInterrupt(DMA1_Channel1_IRQn, DMA_HAL_HANDLER_FOR_RADAR, NULL) ) {
		return FALSE;
	}
#ifndef USE_DMA_AUDIO
	if( !CPU_INTC_ActivateInterrupt(ADC3_IRQn, ADC_HAL_HANDLER, NULL) ) {
		return FALSE;
	}
#else
	if( !CPU_INTC_ActivateInterrupt(DMA2_Channel4_5_IRQn, DMA_HAL_HANDLER_FOR_AUDIO, NULL) ) {
		return FALSE;
	}
#endif
	return TRUE;
}

static void ADC_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	/* Configure PC.01 and PC.04 (ADC Channel11 and Channel14) as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PA.0 (ADC Channel0) as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// All init done in ThreeChannelConfig
BOOL AD_Initialize( ANALOG_CHANNEL channel, INT32 precisionInBits )
{
	return TRUE;
}

void AD_Uninitialize( ANALOG_CHANNEL channel )
{
	AD_StopSampling();
}


INT32 AD_Read( ANALOG_CHANNEL channel )
{
	/*
	ADC_SoftwareStartConvCmd(((ADC_TypeDef *) ADC_MODULE[channel]), ENABLE);
	for(UINT32 i=0;i<50;i++);
	return ADC_GetConversionValue((ADC_TypeDef *) ADC_MODULE[channel]);
	*/
	return 0;
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
    size = 0;
	return FALSE;
}

void AD_StopSampling()
{
	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM8, DISABLE);
}

DeviceStatus AD_ConfigureScanModeThreeChannels(UINT16* sampleBuff1, UINT16* sampleBuff2, UINT16* sampleBuff3, UINT32 numSamples, UINT32  samplingTimeRadar, HAL_CALLBACK_FPN userCallback, void* Param)
{
	ADC_InitTypeDef           ADC_InitStructure;
	DMA_InitTypeDef           DMA_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	UINT32 periodRadar, periodAudio;
	UINT32 prescalerRadar, prescalerAudio;
	UINT32 samplingTimeAudio;

#ifdef DEBUG_3_CHAN
	CPU_GPIO_EnableOutputPin(25, FALSE);
	CPU_GPIO_EnableOutputPin(1,  FALSE);
#endif

	ADC_RCC_Configuration();
	ADC_GPIO_Configuration();
	ADC_NVIC_Configuration();

	g_callback = userCallback;

	if(Param != NULL)
	{
		Param = &g_timeStamp;
	}

	radar_I_return 	= sampleBuff1;
	radar_Q_return 	= sampleBuff2;
	audio_return 	= sampleBuff3;

	adcNumSamplesRadar = numSamples/8;
	adcNumSamplesAudio = numSamples;
	samplingTimeAudio = samplingTimeRadar/8;

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	if (RCC_Clocks.HCLK_Frequency == RCC_Clocks.PCLK1_Frequency) {
		// No prescaler, so TIM clock == PCLK1
		periodRadar = samplingTimeRadar * (RCC_Clocks.PCLK1_Frequency/1000000);
		periodAudio = samplingTimeAudio * (RCC_Clocks.PCLK1_Frequency/1000000);
	}
	else {
		// Prescaler, so TIM clock = PCLK1 x 2
		periodRadar = samplingTimeRadar * (RCC_Clocks.PCLK1_Frequency*2/1000000);
		periodAudio = samplingTimeAudio * (RCC_Clocks.PCLK1_Frequency*2/1000000);
	}

	//----------RADAR---------------
	prescalerRadar = 1;

	// Only have 16-bit timer
	// If sampling rate is too low, period will be too large.
	// So keep doubling the prescaler and halving the period.
	// Note that actual prescaler used in timer is prescaler+1, adjust at end.
	// TODO Add error check if sampling rate is too low.
	if (periodRadar > 0xFFFF) {
		periodRadar = periodRadar >> 1;
		prescalerRadar = 2;
	}

	while (periodRadar > 0xFFFF) {
		periodRadar = periodRadar >> 1;
		prescalerRadar = prescalerRadar << 1;
	}

	// Adjust
	prescalerRadar--;

	//----------AUDIO---------------
	prescalerAudio = 1;

	if (periodAudio > 0xFFFF) {
		periodAudio = periodAudio >> 1;
		prescalerAudio = 2;
	}

	while (periodAudio > 0xFFFF) {
		periodAudio = periodAudio >> 1;
		prescalerAudio = prescalerAudio << 1;
	}

	// Adjust
	prescalerAudio--;
	//-------------------------------

	//-------------------------TIMER SETUP---------------------------------------------------------------------
	//AnanthAtSamraksh - setup a timer for Radar (TIM3)
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = (UINT16) periodRadar;	//250Hz @ 8MHz
    TIM_TimeBaseStructure.TIM_Prescaler = (UINT16) prescalerRadar;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);

	//AnanthAtSamraksh - setup a timer for Audio (TIM8)
	TIM_DeInit(TIM8);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = (UINT16) periodAudio;	//2KHz @ 8MHz
	TIM_TimeBaseStructure.TIM_Prescaler = (UINT16) prescalerAudio;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);


	radarBuffer = (UINT32 *) private_malloc( (sizeof(UINT32)) * adcNumSamplesRadar );
	audioBuffer = (UINT16 *) private_malloc( adcNumSamplesAudio * (sizeof(UINT16)) );


    //-------------------------DMA SETUP------------------------------------------------------------------------
    //AnanthAtSamraksh - setup DMA for radar
	/* DMA1 channel1 configuration for ADC1 & 2 ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)radarBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = adcNumSamplesRadar;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);


	//AnanthAtSamraksh - setup DMA for audio
	/* DMA2 channel5 configuration for ADC3 ----------------------------------------------*/
#ifdef USE_DMA_AUDIO
	DMA_DeInit(DMA2_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&audioBuffer);
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = numSamples;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);

	DMA_ITConfig(DMA2_Channel5, DMA2_IT_TC5, ENABLE);
	// Enable DMA2 Channel5
	DMA_Cmd(DMA2_Channel5, ENABLE);
#endif

	//--------------------------------------------------------------------------

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	//ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	/* ADC1 regular channels configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_7Cycles5);
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);


	/* ADC2 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);
	/* ADC2 regular channels configuration */
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_7Cycles5);
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
#ifndef USE_DMA_AUDIO
	ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE);
#endif

	/* ADC3 regular channels configuration */
	ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
	/* Enable ADC3 external trigger conversion */
	ADC_ExternalTrigConvCmd(ADC3, ENABLE);
#ifdef USE_DMA_AUDIO
	ADC_DMACmd(ADC3, ENABLE);
#endif

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
	// Enable ADC3 reset calibration register
	ADC_ResetCalibration(ADC3);
	// Check the end of ADC3 reset calibration register
	while(ADC_GetResetCalibrationStatus(ADC3));
	// Start ADC3 calibration
	ADC_StartCalibration(ADC3);
	// Check the end of ADC3 calibration
	while(ADC_GetCalibrationStatus(ADC3));


	/* Start ADC1 Software Conversion */
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	/* Start ADC3 Software Conversion */
	//ADC_SoftwareStartConvCmd(ADC3, ENABLE);

	TIM_Cmd(TIM3, ENABLE);		//Radar
	//TIM_Cmd(TIM8, ENABLE);		//Audio DISABLED FOR NOW SINCE NOT USED

	return DS_Success;
}

static void push_data_up(uint32_t x) {
	static uint32_t flag=0;

	flag |= x;

	//if (flag == 0x3) {
	if (flag == 0x2) {
		g_timeStamp = HAL_Time_CurrentTicks();
		g_callback(&g_timeStamp);
		flag = 0;
	}
}

// Handler for audio (default 2000 Hz)
#ifndef USE_DMA_AUDIO
static void ADC_HAL_HANDLER(void *param)
{
#ifdef DEBUG_3_CHAN
	CPU_GPIO_SetPinState((GPIO_PIN)1, TRUE);
#endif

	static uint32_t buffer_index = 0;

	if(buffer_index < adcNumSamplesAudio) {
		audioBuffer[buffer_index++] = ADC_GetConversionValue(ADC3);
	}

	if(buffer_index == adcNumSamplesAudio)
	{
		memcpy(audio_return, audioBuffer, adcNumSamplesAudio*2); // NOTE: Must be able to do this fast enough (500 uS)
		push_data_up(1);
		buffer_index = 0;
	}
#ifdef DEBUG_3_CHAN
	CPU_GPIO_SetPinState((GPIO_PIN)1, FALSE);
#endif
}
#endif

static void DMA_HAL_HANDLER_FOR_RADAR(void *param)
{
#ifdef DEBUG_3_CHAN
	CPU_GPIO_SetPinState((GPIO_PIN)25, TRUE);
#endif

	if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1);

		for(int i = 0; i < adcNumSamplesRadar; i++)
		{
			radar_I_return[i] = (radarBuffer[i] & 0xffff);
			radar_Q_return[i] = (radarBuffer[i] >> 16);
		}
		push_data_up(2);
	}
#ifdef DEBUG_3_CHAN
	CPU_GPIO_SetPinState((GPIO_PIN)25, FALSE);
#endif
}

#ifdef USE_DMA_AUDIO
static void DMA_HAL_HANDLER_FOR_AUDIO(void *param)
{
#ifdef DEBUG_3_CHAN
	CPU_GPIO_SetPinState((GPIO_PIN)1, TRUE);
#endif

	if(DMA_GetITStatus(DMA2_IT_TC5) != RESET)
	{
		DMA_ClearITPendingBit(DMA2_IT_TC5);
		memcpy(audio_return, audioBuffer, adcNumSamplesAudio*2); // NOTE: Must be able to do this fast enough (500 uS)
		push_data_up(1);
	}
#ifdef DEBUG_3_CHAN
	CPU_GPIO_SetPinState((GPIO_PIN)1, TRUE);
#endif
}
#endif
