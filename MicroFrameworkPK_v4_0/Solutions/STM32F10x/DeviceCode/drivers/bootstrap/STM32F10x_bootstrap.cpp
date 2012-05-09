////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**======================================================
 *		SAMRAKSH
 *=======================================================
 *	NAME	DATE	NOTES
 *	----	----	--------------------
 *	@NK1	07/01	Initial create
 *
 ======================================================**/

#include <tinyhal.h>
#include <rcc/STM32F10x_rcc.h>
#include <misc/misc.h>
#include <flash/stm32f10x_flash.h>

ErrorStatus HSEStartUpStatus;


/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

static void RCC_Configuration(void);


static void SetSysClockTo72(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash wait state depends on frequency of system clock*/
    //Mukundan modified code
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
   /* if (SYSTEM_CLOCK_HZ <= 24000000)
    	FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;
    else if (SYSTEM_CLOCK_HZ <= 48000000)
    	FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;
    else*/

    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;

    // HCLK = SYSCLK
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    // PCLK2 = HCLK
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    // PCLK1 = HCLK
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

#ifdef STM32F10X_CL
    /* Configure PLLs ------------------------------------------------------*/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    /* PREDIV1 configuration: PREDIV1CLK = PLL2 / 5 = 8 MHz */

    RCC->CFGR2 &= (uint32_t)~(RCC_CFGR2_PREDIV2 | RCC_CFGR2_PLL2MUL |
                              RCC_CFGR2_PREDIV1 | RCC_CFGR2_PREDIV1SRC);
    RCC->CFGR2 |= (uint32_t)(RCC_CFGR2_PREDIV2_DIV5 | RCC_CFGR2_PLL2MUL8 |
                             RCC_CFGR2_PREDIV1SRC_PLL2 | RCC_CFGR2_PREDIV1_DIV5);

    /* Enable PLL2 */
    RCC->CR |= RCC_CR_PLL2ON;
    /* Wait till PLL2 is ready */
    while((RCC->CR & RCC_CR_PLL2RDY) == 0)
    {
    }


    /* PLL configuration: PLLCLK = PREDIV1 * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)~(RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLSRC_PREDIV1 |
                            RCC_CFGR_PLLMULL9);
#else
    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
                                        RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
#endif /* STM32F10X_CL */

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}

void LowLevelInit (void)
{
  /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
#ifndef STM32F10X_CL
  RCC->CFGR &= (uint32_t)0xF8FF0000;
#else
  RCC->CFGR &= (uint32_t)0xF0FF0000;
#endif /* STM32F10X_CL */

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

#ifdef STM32F10X_CL
  /* Reset PLL2ON and PLL3ON bits */
  RCC->CR &= (uint32_t)0xEBFFFFFF;

  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x00FF0000;

  /* Reset CFGR2 register */
  RCC->CFGR2 = 0x00000000;
#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL)
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;

  /* Reset CFGR2 register */
  RCC->CFGR2 = 0x00000000;
#else
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;
#endif /* STM32F10X_CL */

  /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  /* Configure the Flash Latency cycles and enable prefetch buffer */
  //SetSysClockTo72();

}

// RCC_Configuration : Controls the speed at which the System Clock, ADCClock, AHB, APB1 and APB2 run
//  System Clock runs off the HSI at 48 Mhz
//  ADC Clock runs at 12 Mhz
//  HCLK, AHB run at 48 Mhz
//  APB2 runs at 24 Mhz
//  APB1 runs at 24 Mhz
static void RCC_Configuration(void)
{

  RCC_DeInit();

  RCC_HSEConfig(RCC_HSE_OFF);

  //HSEStartUpStatus = RCC_WaitForHSEStartUp();


  //if(HSEStartUpStatus == SUCCESS)
  //{

    //hal_printf("Enable Prefetch Buffer/n");
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    //hal_printf("Flash 2 wait state/n");
    FLASH_SetLatency(FLASH_Latency_1);

    //hal_printf("/* HCLK = SYSCLK *//n");
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    //hal_printf("/* PCLK2 = HCLK *//n");
    //TODO: Its running at 4Mhz when SysClk is at 8Mhz - Check why!
    RCC_PCLK2Config(RCC_HCLK_Div1);

    //hal_printf("/* PCLK1 = HCLK/2 *//n");
    //Kartik : Changed it to RCC_HCLK_Div1 from RCC_HCLK_Div2
    RCC_PCLK1Config(RCC_HCLK_Div2);

    //hal_printf("/* ADCCLK = PCLK2/4 *//n");
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    // Enabling FSMC peripheral clock here
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

    //hal_printf("/* PLLCLK = 8MHz * 7 = 56 MHz *//n");
    //RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);

    //hal_printf("/* Enable PLL *//n");
    RCC_PLLCmd(ENABLE);


    //hal_printf("/* Wait till PLL is ready *//n");
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    //hal_printf("/* Select PLL as system clock source *//n");
    //RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	// Kartik : Change made to system clock from 72 to 8 Mhz; It is now powered of the HSI Oscillator
	//RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    //hal_printf("/* Wait till PLL is used as system clock source *//n");
    //while(RCC_GetSYSCLKSource() != 0x00) //if sysclk is HSI
	while(RCC_GetSYSCLKSource() != 0x08)	//if sysclk is PLL
    {
    }


  //}

/* Enable peripheral clocks --------------------------------------------------*/


  //hal_printf("/* Enable DMA clock *//n");
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


  //hal_printf("/* Enable ADC1 and GPIOC clock *//n");
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

  //hal_printf("/* Enable Peripheral on ABP1 : Timet TIM3 Clock Enable*//n");
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2, ENABLE);

  //hal_printf("/* Enable USART1 clock if Thermal Noise or Oversampling test is undertaken *//n");

  //Enable LEDs
  STM_EVAL_LEDInit((Led_TypeDef)0);
  STM_EVAL_LEDInit((Led_TypeDef)1);
  STM_EVAL_LEDInit((Led_TypeDef)2);
  STM_EVAL_LEDInit((Led_TypeDef)3);


}


/**
 * Calls various bootstrap routines
 **/
void BootstrapCode() {

	LowLevelInit();
	RCC_Configuration();
	//Boot_DisableWDT();
}
