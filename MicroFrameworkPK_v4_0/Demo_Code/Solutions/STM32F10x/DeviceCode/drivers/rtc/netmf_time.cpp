////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#ifndef __cplusplus
//#define __cplusplus
//#endif

#include <tinyhal.h>
#include "netmf_time.h"
#include "int_time.h"
#include <rcc/stm32f10x_rcc.h>
#include <pwr/stm32f10x_pwr.h>
#include <bkp/stm32f10x_bkp.h>
#include "stm32f10x_rtc.h"
#include <tim/stm32f10x_tim.h>
#include <misc/misc.h>

//--//
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure;

extern "C" {
void TimeLinkDequeueAndExec() {
	HAL_COMPLETION::DequeueAndExec();
}
}

BOOL HAL_Time_Initialize()
{
	/* Configure SysTick to AHB Div 8 (9MHz)*/
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	
	/* Configure SysTick to 1+ sec roll over */
	SysTick->LOAD  = (10000000 & SysTick_LOAD_RELOAD_Msk); // set reload register
		
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
	
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	/* Reset Backup Domain */
	BKP_DeInit();
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {}
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	
	/* Enable the TIM1 interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; // update event interrupt enabled
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the TIM1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	/* Timerconfiguration */
	SET_BIT(TIM1->CR1, 0x0004); // TIM1->CR1 = 0x0004; // URS = 0x1
	SET_BIT(TIM1->DIER, 0x0001); // TIM1->DIER = 0x0001; // UIF = 0x1

	//FIXME - Code to check the timer. It makes the green blink
	//LED_GREEN();
	//LED_ORANGE();
	//LED_ORANGE_OFF();
	
	/* Init complete */
	return TRUE;
}

BOOL HAL_Time_Uninitialize()
{
    return TRUE;
}

UINT64 HAL_Time_CurrentTicks()
{

	return (UINT64)((RTC_GetCounter()*9000000) + (9999986 - SysTick->VAL));
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
    return CPU_TicksToTime(Ticks);
}

INT64 HAL_Time_CurrentTime()
{
	
    return CPU_TicksToTime(HAL_Time_CurrentTicks());
	
}

void HAL_Time_SetCompare( UINT64 CompareValue )
{
	if(HAL_Time_CurrentTicks() >= CompareValue) {
		/* Set Pending IRQ
			- the system is requesting us to serve up an interrupt
			at a time that has already expired - so serve one now */
		NVIC_SetPendingIRQ(TIM1_UP_IRQn);
	} else {
		CompareValue -= HAL_Time_CurrentTicks();
		if       (CompareValue <= 0xFFFF) {
			/* Autoreload Value Configuration */
			TIM1->ARR = CompareValue;
			/* Repition Counter */
			TIM1->RCR = 0x0000;
			/* Prescaler Configuration */
			TIM1->PSC = 0x0000;
		} else if(CompareValue <= 0xFFFFFF) {
			/* Autoreload Value Configuration */
			TIM1->ARR = 0xFFFF;
			/* Repition Counter */
			//TIM1->RCR = CompareValue / 0xFFFF;
			TIM1->RCR = 0x00FF;
			/* Prescaler Configuration */
			TIM1->PSC = 0x0000;
		} else {
			/* Autoreload Value Configuration */
			TIM1->ARR = 0xFFFF;
			/* Repition Counter */
			//TIM1->RCR = CompareValue / 0xFFFFF;
			TIM1->RCR = 0x00FF;
			/* Prescaler Configuration */
			TIM1->PSC = 0x000F;
		}
		/* Event Generation */
		SET_BIT(TIM1->EGR, 0x0001); // TIM1->EGR=0x0001; // UG event, loads Prescaler into shadow register
						  // Autoreload is NOT configured to enter shadow register
		/* CEN bit */
		SET_BIT(TIM1->CR1, 0x0001); // TIM1->CR1|=0X0001; // Timing has commenced
	}
}

void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    GLOBAL_LOCK(irq);
	UINT64 target = uSec / 9;
	//guarantees a minimum amount of time, but fairly close
	while(HAL_Time_CurrentTicks() < target);
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	UINT64 target = uSec / 9;
	//guarantees a minimum amount of time, but fairly close
	while(HAL_Time_CurrentTicks() < target);
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}

UINT32 CPU_SystemClock()
{	
    return 72000000;
}


UINT32 CPU_TicksPerSecond()
{
    return 9000000;
}

UINT64 CPU_MillisecondsToTicks( UINT64 Ticks )
{
    return (Ticks * 9000);
}

UINT64 CPU_MillisecondsToTicks( UINT32 Ticks32 )
{
    return ((UINT64)Ticks32 * 9000);
}

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec )
{
    return (uSec * 9);
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec )
{
    return (uSec * 9);
}

UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec )
{
    return (uSec * 72);
}

UINT64 CPU_TicksToTime( UINT64 Ticks )
{
    return (((Ticks * 9) / 10) + 1);
}

UINT64 CPU_TicksToTime( UINT32 Ticks32 )
{
    return (((Ticks32 * 9) / 10) + 1);
}

