////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interrupt Handlers


#include <led/stm32f10x_led.h>
#include <exti/stm32f10x_exti.h>
#include <tim/stm32f10x_tim.h>
#include "radio.h"

void EXTI9_5_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
  
	//STM_EVAL_LEDInit(1);
    /* Toggle LED1 */	
	//STM_EVAL_LEDToggle(1);

	//LED_RED();

    /* Clear the Key Button EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
}

void EXTI0_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
	//STM_EVAL_LEDInit(0);
	//STM_EVAL_LEDToggle(0);
    /* Toggle LED1 */
    //STM_EVAL_LEDToggle(LED1);
	//LED_GREEN();	  
	//hal_printf("Calling radio's handler now!");
	//irq_handler();	
	//radio_irq_handler();
    /* Clear the Key Button EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }	
}
