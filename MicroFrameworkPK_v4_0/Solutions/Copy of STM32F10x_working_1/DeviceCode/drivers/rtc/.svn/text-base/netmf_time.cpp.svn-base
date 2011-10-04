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
#include <led/stm32f10x_led.h>

//--//
NVIC_InitTypeDef NVIC_InitStructure;

/**
  * @brief  Calls HAL_COMPLETION::DequeueAndExec()
  * @note   Problems with C/C++ compatability prevent interrupt from calling directly
  * @param  None
  * @retval None
  */
extern "C" {
void TimeLinkDequeueAndExec() {
	HAL_COMPLETION::DequeueAndExec();
}
}

/**
  * @brief  Initializes the HAL Time Driver
  * @note   Sets up SysTick, RTC, and TIM1 as well as NVIC for RTC and TIM1
  * @param  None
  * @retval None
  */
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
	//NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the TIM1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	/* Timerconfiguration */
	SET_BIT(TIM1->CR1, 0x0004); // TIM1->CR1 = 0x0004; // URS = 0x1
	SET_BIT(TIM1->DIER, 0x0001); // TIM1->DIER = 0x0001; // UIF = 0x1
	
	//LED_GREEN();
	//LED_ORANGE();
	//LED_ORANGE_OFF();
	
	/* Init complete */
	return TRUE;
}

/**
  * @brief  Uninitializes the HAL Time Driver
  * @param  None
  * @retval None
  */
BOOL HAL_Time_Uninitialize() // @todo
{
    return TRUE;
}

/**
  * @brief  Returns the current ticks
  * @note   Adjusts for computation time
  * @param  None
  * @retval UINT64 - current ticks
  */
UINT64 HAL_Time_CurrentTicks()
{
	/* Returns current ticks, adjusts 14 ticks for calculation time */
    return (UINT64)((RTC_GetCounter()*9000000) + (9999986 - SysTick->VAL)); // @todo: compute exact calc value
}

/**
  * @brief  Converts arbitrary amount of ticks to time
  * @note   Uses CPU_TicksToTime();
  * @param  UINT64 Ticks - arbitrary number of ticks
  * @retval INT64 - time from ticks in CLR-based time in 100-nanoseconds
  */
INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
    return CPU_TicksToTime(Ticks);
}

/**
  * @brief  Gets the current time
  * @note   Based off current ticks
  * @param  None
  * @retval INT64 - current CLR-based time in 100-nanoseconds
  */
INT64 HAL_Time_CurrentTime()
{
    return CPU_TicksToTime(HAL_Time_CurrentTicks());
}

/**
  * @brief  Sets a timer for the specified tick
  * @note   Keeps as accurate resolution as possible with little resources
  *			If the tick is in the past, pends interrupt immediately
  * @param  UINT64 CompareValue - the exact tick when the interrupt should be fired
  * @retval None
  */
void HAL_Time_SetCompare( UINT64 CompareValue )
{
	if(HAL_Time_CurrentTicks() >= CompareValue) {
		/* Set Pending IRQ
			- the system is requesting us to serve up an interrupt
			at a time that has already expired - so serve one now */
		NVIC_SetPendingIRQ(TIM1_UP_IRQn);
	} else { //@todo
		CompareValue -= HAL_Time_CurrentTicks();
		if       (CompareValue <= 0xFFFF) {
			/* Autoreload Value Configuration */
			TIM1->ARR = CompareValue & 0xFFFF;
			/* Repition Counter */
			TIM1->RCR = 0x0000;
			/* Prescaler Configuration */
			TIM1->PSC = 0x0001;
		} else if(CompareValue <= 0xFFFFFF) {
			/* Autoreload Value Configuration */
			TIM1->ARR = 0xFFFF;
			/* Repition Counter */
			TIM1->RCR = (CompareValue / 0xFFFF) & 0xFFFF;
			/* Prescaler Configuration */
			TIM1->PSC = 0x0001;
		} else {
			/* Autoreload Value Configuration */
			TIM1->ARR = 0xFFFF;
			/* Repition Counter */
			TIM1->RCR = (CompareValue / 0xFFFFF)  & 0xFFFF;
			/* Prescaler Configuration */
			TIM1->PSC = 0x0010;
		}
		/* Event Generation */
		SET_BIT(TIM1->EGR, 0x0001); // UG event, loads Prescaler into shadow register
									// Autoreload is NOT configured to enter shadow register
		/* CEN bit */
		SET_BIT(TIM1->CR1, 0x0001); // Timing has commenced
	}
}

/**
  * @brief  Sleeps processor for number of microseconds with interrupts disabled
  * @note   Guarantees a minimum amount of time
  * @param  UINT32 uSec - arbitrary number of microseconds to sleep for
  * @retval None
  */
void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    GLOBAL_LOCK(irq);
	UINT64 target = uSec / 9;
	//guarantees a minimum amount of time, but fairly close
	while(HAL_Time_CurrentTicks() < target);
}

/**
  * @brief  Sleeps processor for number of microseconds with interrupts enabled
  * @note   Guarantees a minimum amount of time
  * @param  UINT32 uSec - arbitrary number of microseconds to sleep for
  * @retval None
  */
void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	UINT64 target = uSec / 9;
	//guarantees a minimum amount of time, but fairly close
	while(HAL_Time_CurrentTicks() < target);
}

/**
  * @brief  Provides PAL Time Driver with drift parameters
  * @note   a=1, b=1, c=0 is no drift
  * @param  INT32* a - drift multiplier
  * @param  INT32* b - drift divider
  * @param  INT32* c - drift shift
  * @retval None
  */
void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}

/**
  * @brief  Returns current system clock speed in Hz
  * @param  None
  * @retval UINT32 - system clock speed in Hz
  */
UINT32 CPU_SystemClock()
{
    return 72000000;
}

/**
  * @brief  Returns CPU ticks per second
  * @param  None
  * @retval UINT32 - CPU ticks per second
  */
UINT32 CPU_TicksPerSecond()
{
    return 9000000;
}

/**
  * @brief  Converts an arbitrary amount of milliseconds to ticks
  * @param  UINT64 mSec - arbitrary amount of milliseconds 
  * @retval UINT64 - ticks from milliseconds
  */
UINT64 CPU_MillisecondsToTicks( UINT64 mSec )
{
    return (mSec * 9000);
}

/**
  * @brief  Converts an arbitrary amount of milliseconds to ticks
  * @param  UINT32 mSec - arbitrary amount of milliseconds 
  * @retval UINT64 - ticks from milliseconds
  */
UINT64 CPU_MillisecondsToTicks( UINT32 mSec )
{
    return ((UINT64)mSec * 9000);
}

/**
  * @brief  Converts an arbitrary amount of microseconds to ticks
  * @param  UINT64 uSec - arbitrary amount of microseconds 
  * @retval UINT64 - ticks from microseconds
  */
UINT64 CPU_MicrosecondsToTicks( UINT64 uSec )
{
    return (uSec * 9);
}

/**
  * @brief  Converts an arbitrary amount of microseconds to ticks
  * @param  UINT32 uSec - arbitrary amount of microseconds 
  * @retval UINT64 - ticks from microseconds
  */
UINT32 CPU_MicrosecondsToTicks( UINT32 uSec )
{
    return (uSec * 9);
}

/**
  * @brief  Converts an arbitrary amount of microseconds to system clocks
  * @param  UINT32 uSec - arbitrary amount of microseconds 
  * @retval UINT32 - system clocks from microseconds
  */
UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec )
{
    return (uSec * 72);
}

/**
  * @brief  Converts arbitrary amount of ticks to time
  * @param  UINT64 Ticks - arbitrary number of ticks
  * @retval INT64 - time from ticks in CLR-based time in 100-nanoseconds
  */
UINT64 CPU_TicksToTime( UINT64 Ticks )
{
    return (((Ticks * 9) / 10) + 1);
}

/**
  * @brief  Converts arbitrary amount of ticks to time
  * @note   Uses CPU_TicksToTime();
  * @param  UINT32 Ticks - arbitrary number of ticks
  * @retval INT64 - time from ticks in CLR-based time in 100-nanoseconds
  */
UINT64 CPU_TicksToTime( UINT32 Ticks )
{
    return ((((UINT64)Ticks * 9) / 10) + 1);
}
