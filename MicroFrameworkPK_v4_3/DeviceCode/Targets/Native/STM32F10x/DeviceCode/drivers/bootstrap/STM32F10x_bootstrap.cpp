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
#include <pwr/stm32f10x_pwr.h>
#include <pwr/netmf_pwr.h>
#include <tim/stm32f10x_tim.h>

ErrorStatus HSEStartUpStatus;


/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

static void RCC_Configuration(void);

// This function should be unused.
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

/*
	RCC_Configuration : Controls the speed at which the System Clock, ADCClock, AHB, APB1 and APB2 run
	SYSCLK runs from HSI at 8 Mhz
	ADC Clock runs at 4 Mhz
	HCLK, AHB run at 8 Mhz
	APB2 runs at 8 Mhz
	APB1 runs at 8 Mhz
	
	Nathan, 2014-01-28 
*/

static void RCC_Configuration(void)
{
  RCC_DeInit();
  
  // Reset TIM1-5
  TIM_DeInit(TIM1);
  TIM_DeInit(TIM2);
  TIM_DeInit(TIM3);
  TIM_DeInit(TIM4);
  TIM_DeInit(TIM5);

  //STM32F1x_Power_Driver::Low_Power();
  STM32F1x_Power_Driver::High_Power(); // Default to high power for now to help with MFDeploy issues

/* RTC clock and backup domain init. Not fully tested. Do not use (yet).
  PWR_BackupAccessCmd(ENABLE);
  BKP_DeInit();
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR,ENABLE);
  RCC_LSEConfig(RCC_LSE_ON);
  //while ( RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET ) { }	// Wait for LSE ready (working???)
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  RCC_RTCCLKCmd(ENABLE);
  RTC_WaitForLastTask();
  RTC_WaitForSynchro();
  RTC_WaitForLastTask();
  BKP_RTCOutputConfig(BKP_RTCOutputSource_None);
  */ // End RTC init Code
  
  // Low power "numerical mode" flash. Here for reference only, conflicts with above.
  //FLASH_HalfCycleAccessCmd(FLASH_HalfCycleAccess_Enable);
  //FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Disable);

  // Enabling FSMC peripheral clock here
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
}


/**
 * Calls various bootstrap routines
 **/
void BootstrapCode() {

	LowLevelInit();
	PrepareImageRegions();
	RCC_Configuration();
	//Boot_DisableWDT();
}

extern  "C"
{
void SystemInit_ExtMemCtl(void)
{
/*!< FSMC Bank1 NOR/SRAM3 is used for the STM3210E-EVAL, if another Bank is
  required, then adjust the Register Addresses */

  /* Enable FSMC clock */
  RCC->AHBENR = 0x00000114;

  /* Enable GPIOD, GPIOE, GPIOF and GPIOG clocks */
  RCC->APB2ENR = 0x000001E0;

/* ---------------  SRAM Data lines, NOE and NWE configuration ---------------*/
/*----------------  SRAM Address lines configuration -------------------------*/
/*----------------  NOE and NWE configuration --------------------------------*/
/*----------------  NE3 configuration ----------------------------------------*/
/*----------------  NBL0, NBL1 configuration ---------------------------------*/

  GPIOD->CRL = 0x44BB44BB;
  GPIOD->CRH = 0xBBBBBBBB;

  GPIOE->CRL = 0xB44444BB;
  GPIOE->CRH = 0xBBBBBBBB;

  GPIOF->CRL = 0x44BBBBBB;
  GPIOF->CRH = 0xBBBB4444;

  GPIOG->CRL = 0x44BBBBBB;
  GPIOG->CRH = 0x44444B44;

/*----------------  FSMC Configuration ---------------------------------------*/
/*----------------  Enable FSMC Bank1_SRAM Bank ------------------------------*/

  FSMC_Bank1->BTCR[4] = 0x00001011;
  FSMC_Bank1->BTCR[5] = 0x00000200;
}

}
