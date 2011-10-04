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

#include <stdio.h>
#include <stdlib.h>
#include <tinyhal.h>
#include "stm32f10x.h"
#include <misc/misc.h>
#include <adc/stm32f10x_adc.h>
#include <rcc/stm32f10x_rcc.h>
#include <gpio/stm32f10x_gpio.h>
#include <lcd_basic/STM32F10x_lcd_basic.h>
#include <led/stm32f10x_led.h>
#include <flash/stm32f10x_flash.h>

#include "ADC.h"
#include "ADC_ADC_library_ADC.h"

using namespace ADC_library;

ADC_InitTypeDef ADC_InitStructure;

extern void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
extern void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
extern uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);

void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void ADC_Configuration(void);

ErrorStatus HSEStartUpStatus;
int count = 0;

INT8 ADC::initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
   INT8 retVal = 0; 
	
   //hal_printf("Interop call in Initialize\n");
	
   NVIC_Configuration();

   GPIO_Configuration();
   
   //ADC_Configuration();
   ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
   ADC_InitStructure.ADC_ScanConvMode = ENABLE;
   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   ADC_InitStructure.ADC_NbrOfChannel = 1;
   ADC_Init(ADC1, &ADC_InitStructure);
   
   //ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);
	
   ADC_Cmd(ADC1, ENABLE);
	
   ADC_ResetCalibration(ADC1);

  /* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);

  /* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	
    return retVal;
}

INT32 ADC::getData( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{

    //LED_BLUE_OFF();
    
	INT32 retVal = 0; 
	char b[30];
	int i = 0;
	
	uint16_t adcData1 = (uint16_t) 0;
	uint16_t adcData2 = (uint16_t) 0;
	
	//LED_RED();
			
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd( ADC1, ENABLE);
	//for(i=0;i<10;i++);
	for(i=0;i<250;i++);
	adcData1 = ADC_GetConversionValue(ADC1);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd( ADC1, ENABLE);
	//for(i=0;i<10;i++);
	for(i=0;i<250;i++);
	adcData2 = ADC_GetConversionValue(ADC1);
		
	// Nived : This line will be commented when the prints are happening from csharp
//	hal_printf("Sample,%d,%d\n",adcData1,adcData2);
	
	//LCD_DisplayStringLine(Line5, (uint8_t *)"Interops in action...");	
	
	retVal = adcData1;
	
	retVal = retVal << 16;
	
	retVal = retVal + adcData2;
		
	for (i=0;i<9000;i++);
	
	//LED_RED_OFF();
	
    return retVal;
}

#if 0 
void ADC_Configuration(void)
{
    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);
	
	ADC_ResetCalibration(ADC1);

  /* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);

  /* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

}
#endif


#if 0
void RCC_Configuration(void)
{

  //LED_RED();
  //hal_printf("RCC system reset(for debug purpose)\n");
  RCC_DeInit();

  //hal_printf("Enable HSE\n");
  RCC_HSEConfig(RCC_HSE_ON);

  //hal_printf("Wait till HSE is ready/n");
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  
  if(HSEStartUpStatus == SUCCESS)
  {

    //hal_printf("Enable Prefetch Buffer/n");
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    //hal_printf("Flash 2 wait state/n");
    FLASH_SetLatency(FLASH_Latency_2);


    //hal_printf("/* HCLK = SYSCLK *//n");
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    //hal_printf("/* PCLK2 = HCLK *//n");
    RCC_PCLK2Config(RCC_HCLK_Div1);

    //hal_printf("/* PCLK1 = HCLK/2 *//n");
    RCC_PCLK1Config(RCC_HCLK_Div2);

    //hal_printf("/* ADCCLK = PCLK2/4 *//n");
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    //hal_printf("/* PLLCLK = 8MHz * 7 = 56 MHz *//n");
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);

    //hal_printf("/* Enable PLL *//n");
    RCC_PLLCmd(ENABLE);
	
	
    //hal_printf("/* Wait till PLL is ready *//n");
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    //hal_printf("/* Select PLL as system clock source *//n");    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
    //hal_printf("/* Wait till PLL is used as system clock source *//n");
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }


  }

/* Enable peripheral clocks --------------------------------------------------*/  
	
	
  //hal_printf("/* Enable DMA clock *//n");
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  
  //hal_printf("/* Enable ADC1 and GPIOC clock *//n");
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

  //hal_printf("/* Enable Peripheral on ABP1 : Timet TIM3 Clock Enable*//n");
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2, ENABLE);

  //hal_printf("/* Enable USART1 clock if Thermal Noise or Oversampling test is undertaken *//n");  
  
}
#endif



void GPIO_Configuration(void)
{

  //LED_GREEN();
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure PC.00 (ADC Channel10) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
}



/**
  * @brief  Configures Vector Table base location.
  * @param  None
  * @retval : None
  */
void NVIC_Configuration(void)
{

  //LED_ORANGE();

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
