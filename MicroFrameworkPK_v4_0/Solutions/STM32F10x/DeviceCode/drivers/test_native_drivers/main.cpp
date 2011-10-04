/**
  ******************************************************************************
  * @file TIM/TimeBase/main.c
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f10x.h"
#include <rs232/stm32_eval.h>
#include <tinyhal.h>
#include <gpio/stm32f10x_gpio.h>
#include <rcc/stm32f10x_rcc.h>
#include <tim/stm32f10x_tim.h>
#include <led/stm32f10x_led.h>
#include <usart/stm32f10x_usart.h>
#include <misc/misc.h>
#include <tim/netmf_timers.h>
#include <intc/stm32.h>
#include "C:/MicroFrameworkPK_v4_0/Solutions/STM32F10x/DeviceCode/drivers/crypto/aes_native.c"
#include <system_stm32f10x.h>

#define TIMER_OC_MODE
//#define TIMER_DRIVER_MODE
//#define TIMER_COUNT_MODE

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
USART_InitTypeDef USART_InitStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
RCC_ClocksTypeDef RCC_Clocks;
int done = 0;
//extern uint16_t RxBuffer[];
extern char RxBuffer[];

__IO uint16_t CCR1_Val = 40961;
__IO uint16_t CCR2_Val = 27309;
__IO uint16_t CCR3_Val = 13654;
__IO uint16_t CCR4_Val = 6826;
uint16_t PrescalerValue = 0;


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
//void ISR( void* );
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
void test_timers()
{
	
	UINT8 encVal[16] = {0};
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */
  /* NVIC configuration */
  NVIC_Configuration();
  
/* USARTx configuration ------------------------------------------------------*/
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
  debug_printf("USART Camera Demo");
  while(1)
  {
	  while (!done)
	  {
	   
	  }	  
	  done = 0;	  	  
	  AES_Encrypt(RxBuffer, encVal, 16);
	  
	  for(int i =0; i < 16; i++)
	  {
		debug_printf("%02x",(char *)encVal[i]);
	  }
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{

}

/**
  * @brief  Configure the GPIOD Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{

  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


