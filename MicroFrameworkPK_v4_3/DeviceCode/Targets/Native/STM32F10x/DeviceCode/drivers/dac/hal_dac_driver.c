/*
 * 	hal_adc_driver.c
 *
 *  Modified on: Sep 15, 2011
 *  Author: Sandip Bapat
 */

#include "stm32f10x.h"
#include "hal_dac_driver.h"

DAC_InitTypeDef            DAC_InitStructure;

void DAC_GPIO_Configuration(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;

   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DAC_RCC_Configuration(void)
 {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
 }



void hal_dac_init()
{
	DAC_RCC_Configuration();
	DAC_GPIO_Configuration();

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	/* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
	   automatically connected to the DAC converter. */
	DAC_Cmd(DAC_Channel_1, ENABLE);
}

uint8_t hal_dac_set(uint16_t val)
{
	if (val > 4095)
		return 0;
	else
	{
		DAC_SetChannel1Data(DAC_Align_12b_R, val);
		DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
	}
	return 1;
}
