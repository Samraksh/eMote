#include "radio_exti_test.h"
#include "stm3210e_eval.h"
#include <exti/stm32f10x_exti.h>
#include <led/stm32f10x_led.h>


void test_exti()
{


  LED_BLUE();
  
  STM_EVAL_LEDInit(0);
  STM_EVAL_PBInit(2, 1);

  /* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  EXTI_GenerateSWInterrupt(EXTI_Line8);

  while(1)
  {
	  LED_ORANGE();
  }


}