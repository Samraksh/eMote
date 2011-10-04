////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interrupt Handlers

#include "netmf_time.h"
//#include "int_time.h"
#include "stm32f10x_rtc.h"
#include <tim/stm32f10x_tim.h>

extern void TimeLinkDequeueAndExec();

int LEDState = 1;

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void) {
	/* Clear SysTick to start sub-second timing from 0 this
		is the first thing that needs to be done in this ISR */
	SysTick->VAL = 0;
	
	if(LEDState) {
		GPIOF->BRR = (0x1 << 6);
		LEDState = 0;
	} else {
		GPIOF->BSRR = (0x1 << 6);
		LEDState = 1;
	}

	if (READ_BIT(RTC->CRL, 0x0001)) {
		/* Clear the RTC Second interrupt */
		CLEAR_BIT(RTC->CRL, 0x0001);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}
}

/**
  * @brief  This function handles TIM1 update interrupt request.
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void) {
	/* Clear UIF */
	CLEAR_BIT(TIM1->SR, 0x0001); // TIM1->SR = 0x0000;
	/* CEN bit */
	CLEAR_BIT(TIM1->CR1, 0x0001); // TIM1->CR1 = 0x0000; // timing stop
	
	GPIOF->BSRR = (0x1 << 7);
	/* HAL Execution */
	TimeLinkDequeueAndExec(); // This also schedules next SetCompare @todo: call function directly (problem is with c/c++ calling)
	
	GPIOF->BRR = (0x1 << 7);
}
