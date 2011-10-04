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
#include <led/stm32f10x_led.h>
#include <lcd_basic/STM32F10x_lcd_basic.h>
#include <stm32f10x.h>
#include <misc/misc.h>
#include <gpio/stm32f10x_gpio.h>
#include <rcc/stm32f10x_rcc.h>
#include <flash/stm32f10x_flash.h>
#include <lcd_basic/stm32f10x_lcd_basic.h>
#include <stdio.h>
#include <stm32f10x.h>
#include "lcd.h"
#include "mems.h"

#define DELAY_VAL 3600

#define LEDS_GPIO       (GPIOE)
#define LEDS_RCC_GPIO   (RCC_APB2Periph_GPIOE)
#define LED0_PIN        (GPIO_Pin_0)
#define LED1_PIN        (GPIO_Pin_1)
#define LEDS_BOTH_PINS  (LED0_PIN|LED1_PIN)

//joystick port and pins
#define JSTK_GPIO       (GPIOE)
#define JSTK_RCC_GPIO   (RCC_APB2Periph_GPIOE)
#define JSTK_LEFT_PIN   (GPIO_Pin_3)
#define JSTK_RIGHT_PIN  (GPIO_Pin_4)
#define JSTK_UP_PIN     (GPIO_Pin_5)
#define JSTK_DOWN_PIN   (GPIO_Pin_6)
#define JSTK_ALL_PINS   (JSTK_LEFT_PIN|JSTK_RIGHT_PIN|JSTK_UP_PIN|JSTK_DOWN_PIN)

//push button port and pin
#define BP_GPIO         (GPIOA)
#define BP_RCC_GPIO     (RCC_APB2Periph_GPIOA)
#define BP_PIN          (GPIO_Pin_8)


//LCD backlight enable port and pin
#define LCDBLEN_GPIO         (GPIOB)
#define LCDBLEN_RCC_GPIO     (RCC_APB2Periph_GPIOB)
#define LCDBLEN_PIN          (GPIO_Pin_8)


/* example defines ------------------------------------------------------------*/

//start, min and max for the delay variable
#define DELAYVAR_MAXVAL         (0x100000)
#define DELAYVAR_STARTVAL       (0x40000)
#define DELAYVAR_MINVAL         (0x1000)

//small and big steps for the delay variable
#define DELAYVAR_BIGCHANGE      (0x4000)
#define DELAYVAR_SMALLCHANGE    (0x1000)

/* example function prototypes -----------------------------------------------*/
extern void RCC_DeInit(void);

void RCC_Configuration(void);
void NVIC_Configuration(void);
void Delay(vu32 nCount);

/* example global variables --------------------------------------------------*/

/*these variables could (and should, maybe) be defined as local to the main
function, but declaring them as global and volatile helps for debugging.
*/
GPIO_InitTypeDef GPIO_InitStructure;
volatile int delayvar=DELAYVAR_STARTVAL;
volatile unsigned char toggle=0;
vu16 jstkinput=0;
vu16 bpinput=0;

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

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

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;


    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    /* PCLK1 = HCLK */
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
  SetSysClockTo72();

}

/**
 * Calls various bootstrap routines
 **/
void RCC_Configuration(void)
    {
    // RCC system reset(for debug purpose)
    RCC_DeInit();

    // Enable HSE
    RCC_HSEConfig(RCC_HSE_ON);

    // Wait till HSE is ready
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
        {;}

    // Enable Prefetch Buffer
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    // Flash 2 wait state
    FLASH_SetLatency(FLASH_Latency_2);

    // HCLK = SYSCLK
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    // PCLK2 = HCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);

    // PCLK1 = HCLK
    RCC_PCLK1Config(RCC_HCLK_Div1);

    // Select HSE as system clock source
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

    // Wait till HSE is used as system clock source
    while(RCC_GetSYSCLKSource() != 0x04)
        {;}
    }

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
    {
#ifdef  VECT_TAB_RAM
    // Set the Vector Table base location at 0x20000000
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  // VECT_TAB_FLASH
    // Set the Vector Table base location at 0x08000000
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
    }


/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
    {
    // User can add his own implementation to report the file name and line number,
    //ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)

    // Infinite loop
    while (1)
        {
        }
    }


/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nCount)
    {
    for(; nCount!= 0;nCount--);
    }

void BootstrapCode() {

	//LED_GREEN();
	//LED_BLUE();
	//LED_ORANGE();
	//while(1);
	//	LED_RED();
	//LowLevelInit();
	//while(1);
	//LED_GREEN();
	//while(1);
	//Boot_DisableWDT();
	
	//Start here.... 
	
	   // System Clocks Configuration
    RCC_Configuration();

    // NVIC Configuration 
    NVIC_Configuration();
    
    // turn LCD backlight OFF
    // !!! this prevents overcharging the 3.1V regulator in case of ESD !!!
    // also reduces power consumption
    RCC_APB2PeriphClockCmd(LCDBLEN_RCC_GPIO, ENABLE);

    // Configure PB.08 as output
    GPIO_InitStructure.GPIO_Pin = LCDBLEN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCDBLEN_GPIO, &GPIO_InitStructure);
    GPIO_Write(LCDBLEN_GPIO, ( 0 ) );
    
    
    // Enable LEDs GPIO clock 
    if(LEDS_RCC_GPIO!=LCDBLEN_RCC_GPIO)
        RCC_APB2PeriphClockCmd(LEDS_RCC_GPIO, ENABLE);

    // Configure PE.00 and PE.01 as output push-pull 
    GPIO_InitStructure.GPIO_Pin =  LEDS_BOTH_PINS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LEDS_GPIO, &GPIO_InitStructure);

    // Enable joystick GPIO clock 
    if( (JSTK_RCC_GPIO!=LCDBLEN_RCC_GPIO) && (JSTK_RCC_GPIO!=LEDS_RCC_GPIO) )
        RCC_APB2PeriphClockCmd(JSTK_RCC_GPIO, ENABLE);

    // Configure PE.03 to PE.06 as input 
    GPIO_InitStructure.GPIO_Pin = JSTK_ALL_PINS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(JSTK_GPIO, &GPIO_InitStructure);

    // Enable push button GPIO clock 
    if( (BP_RCC_GPIO!=LCDBLEN_RCC_GPIO) && (BP_RCC_GPIO!=LEDS_RCC_GPIO) && (BP_RCC_GPIO!=JSTK_RCC_GPIO) )
        RCC_APB2PeriphClockCmd(BP_RCC_GPIO, ENABLE);

    // Configure PA.08 as input 
    GPIO_InitStructure.GPIO_Pin = BP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BP_GPIO, &GPIO_InitStructure);
	
	STM3210E_LCD_Init();
	LCD_Clear(White); 
	LCD_SetBackColor(Blue);
	LCD_SetTextColor(White);
	LCD_DisplayStringLine(Line1, (uint8_t*)"Hello...");


    while(1)
        {
        //stop all toggling while push-button is down
        do
            {
            bpinput = ( (GPIO_ReadInputData(BP_GPIO)) & ( BP_PIN ) );
            if(bpinput)
                delayvar=DELAYVAR_STARTVAL;
            }
        while(bpinput);
        
        // change speed of toggling according to joystick direction 
        //jstkinput = ( (GPIO_ReadInputData(JSTK_GPIO)) & ( JSTK_ALL_PINS ) ); 
        jstkinput = ((u16)JSTK_GPIO->IDR) & ( JSTK_ALL_PINS );
        
        switch( jstkinput & ( JSTK_ALL_PINS ) )
            {
            case GPIO_Pin_3:
                //Left button is pressed. slow down toggling a little.
                if ( delayvar <= ( DELAYVAR_MAXVAL - DELAYVAR_SMALLCHANGE ) )
                    delayvar += DELAYVAR_SMALLCHANGE;
                else
                    delayvar = DELAYVAR_MAXVAL;
                    
                break;
                
            case GPIO_Pin_4:
                //Right button is pressed. speed up toggling a little.
                if ( delayvar >= ( DELAYVAR_MINVAL + DELAYVAR_SMALLCHANGE ) )
                    delayvar -= DELAYVAR_SMALLCHANGE;
                else
                    delayvar = DELAYVAR_MINVAL;
                break;
                
            case GPIO_Pin_5:
                //Up button is pressed. slow down toggling quite a lot.
                if ( delayvar <= ( DELAYVAR_MAXVAL - DELAYVAR_BIGCHANGE ) )
                    delayvar += DELAYVAR_BIGCHANGE;
                else
                    delayvar = DELAYVAR_MAXVAL;
                break;
                
            case GPIO_Pin_6:
                //Down button is pressed. speed up toggling quite a lot.
                if ( delayvar >= ( DELAYVAR_MINVAL + DELAYVAR_BIGCHANGE ) )
                    delayvar -= DELAYVAR_BIGCHANGE;
                else
                    delayvar = DELAYVAR_MINVAL;
                break;
                
            default :
                //no buttons are pressed (or several, which is a hardware error)
                //just go on toggling at the same speed
                break;
            }

        // Toggle LEDs 
        GPIO_Write(LEDS_GPIO, ( toggle & LEDS_BOTH_PINS ) );

        // increment Toggle variable 
        toggle++;

        // Insert delay 
        Delay(delayvar);

	}
	//End here....
}
