/*
 * hal_adc_driver.c
 *
 *  Created on: Sep 7, 2011
 *      Author: Sandip
 */

#include <tinyhal.h>
#include "stm32f10x.h"
#include "hal_adc_driver.h"

uint8_t EMOTE_ADC_CHANNEL[3] = {ADC_Channel_14, ADC_Channel_10, ADC_Channel_4};
uint32_t ADC_MODULE[3] = { ADC1_BASE, ADC2_BASE, ADC3_BASE};

void ADC_GPIO_Configuration(void);
void ADC_NVIC_Configuration(void);
void ADC_RCC_Configuration(void);

BOOL AD_Initialize( ANALOG_CHANNEL channel, INT32 precisionInBits )
{
	ADC_RCC_Configuration();
	ADC_NVIC_Configuration();
	ADC_GPIO_Configuration();

	ADC_InitTypeDef ADC_InitStructure;

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



void ADC_RCC_Configuration(void)
{
   RCC_ADCCLKConfig(RCC_PCLK2_Div2);
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3 , ENABLE);
}

void ADC_NVIC_Configuration(void)
{

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

  /* Enable DMA channel1 IRQ Channel -----------------------------------------*/
   NVIC_SetPriorityGrouping(7); /* 0 bits for pre-emption priority 4 bits for
subpriority*/
  NVIC_SetPriority(DMA1_Channel1_IRQn, 0x00); /* 0x00 = 0x0 << 3 | (0x0 & 0x7)*/
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);


  /* Enable TIM2 IRQ Channel -------------------------------------------------*/
  NVIC_SetPriorityGrouping(7); /* 0 bits for pre-emption priority 4 bits for
subpriority*/
  NVIC_SetPriority(TIM2_IRQn, 0x02); /* 0x00 = 0x0 << 3 | (0x02 & 0x7) */
  NVIC_EnableIRQ(TIM2_IRQn);

}

void ADC_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  GPIO_WriteBit(GPIOF, GPIO_Pin_8, Bit_SET);

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
