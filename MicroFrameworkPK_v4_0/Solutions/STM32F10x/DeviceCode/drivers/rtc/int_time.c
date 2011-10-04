////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interrupt Handlers


#include <led/stm32f10x_led.h>
#include <exti/stm32f10x_exti.h>
#include <tim/stm32f10x_tim.h>
#include <gpio/stm32f10x_gpio.h>
#include <usart/stm32f10x_usart.h>
#include <test_native_drivers/main.h>
//#include <usb/netmf_usb.h>
//#include <radio/radio.h>

#include "netmf_time.h"
#include "int_time.h"
#include "stm32f10x_rtc.h"

#define DEBUG_ON

//extern void TimeLinkDequeueAndExec();
extern void usb_irq_handler();
//TODO I am not sure if we need this prototype here. Check when working with Radio
void radio_irq_handler();
int LEDState = 1;
int global_flag = 0;
static int Value;
uint16_t capture = 0;
extern CCR1_Val;
extern CCR2_Val;
extern int done;

/*For USART Test
*/
#define TxBufferSize   (countof(TxBuffer) - 1)
#define RxBufferSize   0x20

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint16_t TxBuffer[RxBufferSize];
//uint16_t RxBuffer[RxBufferSize];
char RxBuffer[RxBufferSize];
uint16_t NbrOfDataToTransfer = TxBufferSize;
uint16_t NbrOfDataToRead = 17;
uint16_t TxCounter = 0; 
uint16_t RxCounter = 0; 


///**
//  * @brief   This function handles NMI exception.
//  * @param  None
//  * @retval None
//  */
void NMI_Handler(void)
{
	//STM_EVAL_LEDToggle(2);		
//	while (1)
//	{
//	}
}
//
///**
//  * @brief  This function handles Hard Fault exception.
//  * @param  None
//  * @retval None
//  */
void HardFault_Handler(void)
{
	//STM_EVAL_LEDToggle(2);
//  /* Go to infinite loop when Hard Fault exception occurs */
//	while(1)
//	{
//	}
}
//
///**
//  * @brief  This function handles Memory Manage exception.
//  * @param  None
//  * @retval None
//  */
void MemManage_Handler(void)
{
	//STM_EVAL_LEDToggle(2);
//  /* Go to infinite loop when Memory Manage exception occurs */
//  while (1)
//  {
//  }
}
//
///**
//  * @brief  This function handles Bus Fault exception.
//  * @param  None
//  * @retval None
//  */
void BusFault_Handler(void)
{
	//STM_EVAL_LEDToggle(2);
//  /* Go to infinite loop when Bus Fault exception occurs */
//  while (1)
//  {
//  }
}
//
///**
//  * @brief  This function handles Usage Fault exception.
//  * @param  None
//  * @retval None
//  */
void UsageFault_Handler(void)
{
	//STM_EVAL_LEDToggle(2);
//  /* Go to infinite loop when Usage Fault exception occurs */
//  while (1)
//  {
//  }
}
//
///**
//  * @brief  This function handles SVCall exception.
//  * @param  None
//  * @retval None
//  */
void SVC_Handler(void)
{
	//STM_EVAL_LEDToggle(2);
}
//
///**
//  * @brief  This function handles Debug Monitor exception.
//  * @param  None
//  * @retval None
//  */
void DebugMon_Handler(void)
{
}
//
///**
//  * @brief  This function handles PendSVC exception.
//  * @param  None
//  * @retval None
//  */
void PendSV_Handler(void)
{
}
//
///**
//  * @brief  This function handles SysTick Handler.
//  * @param  None
//  * @retval None
//  */
//void SysTick_Handler(void)
//{
//}

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    //RxBuffer[RxCounter++] = (USART_ReceiveData(USART1) & 0x7F);
	//STM_EVAL_LEDToggle(1);
	RxBuffer[RxCounter++] = (char )USART_ReceiveData(USART1);

    if(RxCounter == NbrOfDataToRead)
    {
      /* Disable the EVAL_COM1 Receive interrupt */
      //USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	  RxCounter = 0;
	  done = 1;
    }
  }

  //if (done == 1)
  //{

	 // if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	 // {   
		///* Write one byte to the transmit data register */
		//USART_SendData(USART1, RxBuffer[RxCounter++]);	

		//if(TxCounter == NbrOfDataToTransfer)
		//{
		//  /* Disable the EVAL_COM1 Transmit interrupt */
		//  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		//}
	 // }
  //}
}



/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void) {
	/*
	 * Not configured the RTC yet
	 */
//	/* Clear SysTick to start sub-second timing from 0 this
//		is the first thing that needs to be done in this ISR */
//	SysTick->VAL = 0;
//
//	if(LEDState) {
//		GPIOF->BRR = (0x1 << 6);
//		LEDState = 0;
//	} else {
//		GPIOF->BSRR = (0x1 << 6);
//		LEDState = 1;
//	}
//
//	if (READ_BIT(RTC->CRL, 0x0001)) {
//		/* Clear the RTC Second interrupt */
//		CLEAR_BIT(RTC->CRL, 0x0001);
//		/* Wait until last write operation on RTC registers has finished */
//		RTC_WaitForLastTask();
//	}
}

/**
  * @brief  This function handles TIM1 update interrupt request.
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void)
{
	/*
	 * Kartik commented, TIM1 and TIM8 are advanced timers.
	 */

//	/* Clear UIF */
//	CLEAR_BIT(TIM1->SR, 0x0001); // TIM1->SR = 0x0000;
//	/* CEN bit */
//	CLEAR_BIT(TIM1->CR1, 0x0001); // TIM1->CR1 = 0x0000; // timing stop
//
//	GPIOF->BSRR = (0x1 << 7);
//	/* HAL Execution */
//	TimeLinkDequeueAndExec(); // This also schedules next SetCompare @todo: call function directly (problem is with c/c++ calling)
//
//	GPIOF->BRR = (0x1 << 7);

}

void EXTI9_5_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {

	//STM_EVAL_LEDInit(1);
	//STM_EVAL_LEDToggle(1);
	//LED_GREEN_OFF();
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
}


void EXTI0_IRQHandler()
{
  //STM_EVAL_LEDToggle(2);
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
	radio_irq_handler();
	EXTI_ClearITPendingBit(EXTI_Line0);
  }
}


void TIM2_IRQHandler(void)
{
  /*
   * We always use the timer in OC mode, never in counter mode
   */

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
//
   	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//      Value = (int) TIM_GetFlagStatus(TIM2, TIM_FLAG_CC1);
//   	  //Value = (int) TIM_GetFlagStatus(TIM2, TIM_FLAG_Update);
//   	  //Timer_IRQ_Handler( 2 );
//   	  //GPIO_ResetBits(GPIOC, GPIO_Pin_6);
//      //TIM_GenerateEvent(TIM2, TIM_EventSource_CC1);
   	  //STM_EVAL_LEDToggle(0);
	  //Timer_IRQ_Handler( 2 );
//		  capture = TIM_GetCapture1(TIM2);
//	      TIM_SetCompare1(TIM2, capture + CCR1_Val );
	}

  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
      TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );
	  //STM_EVAL_LEDToggle(1);
      //Timer_IRQ_Handler( 2 );
  }

  /*
   * This version of the netmf_timer.cpp supports only one
   * OC channel, OC1, so we don't need the other handler
   */

//  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
//  {
//      TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );
//      capture = TIM_GetCapture1(TIM2);
//      TIM_SetCompare1(TIM2, capture + CCR1_Val );
//      STM_EVAL_LEDToggle(3);
//  }

}

void TIM3_IRQHandler(void)
{
//  STM_EVAL_LEDInit(0);
//  STM_EVAL_LEDInit(1);
//  STM_EVAL_LEDInit(2);
//  STM_EVAL_LEDInit(3);
//
//  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//  {
//	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//	  STM_EVAL_LEDToggle(1);
//
////	  if (value)
////	  {
////		  LED_RED();
////	  }
////
////	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
////	  Timer_IRQ_Handler( 2 );
////	  GPIO_ResetBits(GPIOC, GPIO_Pin_6);
////	  STM_EVAL_LEDToggle(1);
//  }
//
//  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
//  {
//      TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
//      capture = TIM_GetCapture1(TIM3);
//      TIM_SetCompare1(TIM3, 10000 + capture );
//      STM_EVAL_LEDToggle(0);
//      //Timer_Driver ::SetCompare( ,  )
//      //capture = TIM_GetCapture1(TIM2);
//      //TIM_SetCompare1(TIM2, capture + CCR1_Val );
//      //Timer_IRQ_Handler();
//   }

}

void OTG_FS_IRQHandler(void)
{
  
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	STM_EVAL_LEDToggle(0);  	
	usb_irq_handler();
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
	//STM_EVAL_LEDToggle(1);
}

