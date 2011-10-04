#include <led/stm32f10x_led.h>
#include <exti/stm32f10x_exti.h>
#include <led/stm32f10x_led.h>
#include <radio.h>
#include "radio_it.h"

void EXTI0_IRQHandler()
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    /* Toggle LED1 */
    //STM_EVAL_LEDToggle(LED1);
	  LED_BLUE_OFF();	  
	  //hal_printf("Calling radio's handler now!");
	  irq_handler();	
    /* Clear the Key Button EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }	
}

void EXTI9_5_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
    /* Toggle LED1 */
	//STM_EVAL_LEDToggle(0);

	LED_GREEN();

    /* Clear the Key Button EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
}
