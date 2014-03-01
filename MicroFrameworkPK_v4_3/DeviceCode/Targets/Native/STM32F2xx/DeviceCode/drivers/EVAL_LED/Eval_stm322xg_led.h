/*
 * Eval_stm322xg_led.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Mukundan
 */

#ifndef EVAL_STM322XG_LED_H_
#define EVAL_STM322XG_LED_H_


#define LEDn                             4

#define LED1_PIN 	102			//Port G, Pin 6
#define LED2_PIN 	104			//Port G, Pin 8
#define LED3_PIN 	137			//Port I, Pin 9
#define LED4_PIN 	39			//Port C, Pin 7

/*#define LED1_PIN                         GPIO_Pin_6
#define LED1_GPIO_PORT                   GPIOG
#define LED1_GPIO_CLK                    RCC_AHB1Periph_GPIOG

#define LED2_PIN                         GPIO_Pin_8
#define LED2_GPIO_PORT                   GPIOG
#define LED2_GPIO_CLK                    RCC_AHB1Periph_GPIOG

#define LED3_PIN                         GPIO_Pin_9
#define LED3_GPIO_PORT                   GPIOI
#define LED3_GPIO_CLK                    RCC_AHB1Periph_GPIOI

#define LED4_PIN                         GPIO_Pin_7
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK                    RCC_AHB1Periph_GPIOC
*/

typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
} Led_TypeDef;

struct STM322xG_LED {
	void STM_EVAL_LEDInit(Led_TypeDef Led);
	void STM_EVAL_LEDOn(Led_TypeDef Led);
	void STM_EVAL_LEDOff(Led_TypeDef Led);
	void STM_EVAL_LEDToggle(Led_TypeDef Led);
};

extern  STM322xG_LED g_STM322xG_LED;

#endif /* EVAL_STM322XG_LED_H_ */
