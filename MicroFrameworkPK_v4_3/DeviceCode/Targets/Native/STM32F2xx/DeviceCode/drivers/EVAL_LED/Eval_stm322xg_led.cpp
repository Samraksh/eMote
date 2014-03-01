/*
 * Eval_stm322xg_led.c
 *
 *  Created on: Jun 21, 2013
 *      Author: Mukundan
 */

#include <tinyhal.h>
//#include <stm32f2xx.h>
#include "Eval_stm322xg_led.h"

//#include <rcc/stm32f2xx_rcc.h>
//#include <gpio/stm32f2xx_gpio.h>

STM322xG_LED g_STM322xG_LED;

//GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT, LED4_GPIO_PORT};
const UINT16 LED_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};
//const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK, LED4_GPIO_CLK};

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */

void STM322xG_LED::STM_EVAL_LEDInit(Led_TypeDef Led)
{
  /*GPIO_InitTypeDef  GPIO_InitStructure;

  // Enable the GPIO_LED Clock
  RCC_AHB1PeriphClockCmd(GPIO_CLK[Led], ENABLE);


  // Configure the GPIO_LED pin
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
  */

  CPU_GPIO_EnableOutputPin(LED1_PIN,TRUE);
  CPU_GPIO_EnableOutputPin(LED2_PIN,TRUE);
  CPU_GPIO_EnableOutputPin(LED3_PIN,TRUE);
  CPU_GPIO_EnableOutputPin(LED4_PIN,TRUE);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM322xG_LED::STM_EVAL_LEDOn(Led_TypeDef Led)
{
  //GPIO_PORT[Led]->BSRRL = GPIO_PIN[Led];
	CPU_GPIO_SetPinState(LED_PIN[Led],TRUE);
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM322xG_LED::STM_EVAL_LEDOff(Led_TypeDef Led)
{
  //GPIO_PORT[Led]->BSRRH = GPIO_PIN[Led];
	CPU_GPIO_SetPinState(LED_PIN[Led],FALSE);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void STM322xG_LED::STM_EVAL_LEDToggle(Led_TypeDef Led)
{
	 CPU_GPIO_TogglePinState (LED_PIN[Led]);
}
