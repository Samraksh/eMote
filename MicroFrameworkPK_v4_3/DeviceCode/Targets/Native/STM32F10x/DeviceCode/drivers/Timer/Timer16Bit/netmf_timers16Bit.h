/*
 * Samraksh Timer Driver
 * Initial Create - Kartik Natarajan
 * 04/20/2011
 * Notes -
 * 1) Configuring the TIM2, 3, 4 as of now.
 * 2) Add clock source as a prescaled value from the APB1 clock
 */
#ifndef _NETMF_TIMERS_H_
#define _NETMF_TIMERS_H_


#include <tinyhal.h>
#include <stm32f10x.h>



#define MAX_TIMER 10
#define DEBUG_ON

struct Timer16Bit_Driver
{
	static const UINT32 c_SystemTimer   = 2;
	static const UINT32 c_ADCTimer = 4;
    static const UINT16 c_MaxTimerValue = 0xFFFF;
	static const UINT32 c_MaxTimers = 10;

	static BOOL m_timeOverFlowFlag;

#ifdef DEBUG_ON
	static BOOL Timer_Test();
#endif

	//static BOOL Initialize   ( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_Param );
	static BOOL Initialize   ( UINT32 Timer, BOOL FreeRunning, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_Param );
    static BOOL Uninitialize ( UINT32 Timer );
    static UINT8 System_Clock_Frequency();

    UINT32 GetMaxTicks();
    //static void Timer_IRQ_Handler( UINT32 Timer );

	static void EnableCompareInterrupt( UINT32 Timer )
    {

		//Enable the compare interrupt for the specified timer

        ASSERT(Timer < c_MaxTimers);

		switch ( Timer )
		{
		case 2:
			TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
			break;
		case 3:
			TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
			break;
		case 4:
			TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
			break;
		case 5:
			TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);
			break;
		default:
			break;
		}
	}

	static void DisableCompareInterrupt( UINT32 Timer )
    {
		//Disable the compare interrupt for the specified timer

		ASSERT(Timer < c_MaxTimers);

		switch ( Timer )
		{
		case 2:
			TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
			break;
		case 3:
			TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
			break;
		case 4:
			TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);
			break;
		case 5:
			TIM_ITConfig(TIM5, TIM_IT_CC1, DISABLE);
			break;
		default:
			break;
		}
	}

	static void ForceInterrupt( UINT32 Timer )
    {
		//Write the counter value to the match register and there by force an interrupt
		ASSERT(Timer < c_MaxTimers);

		switch ( Timer )
		{
			case 2:
				TIM_GenerateEvent(TIM2, TIM_EventSource_CC1);
				break;
			case 3:
				TIM_GenerateEvent(TIM3, TIM_EventSource_CC1);
				break;
			case 4:
				TIM_GenerateEvent(TIM4, TIM_EventSource_CC1);
				break;
			case 5:
				TIM_GenerateEvent(TIM5, TIM_EventSource_CC1);
				break;
			default:
				break;
		}
	}

	static void SetCompare( UINT32 Timer, UINT32 Compare )
    {
		//Set the compare register value for the specified time

		ASSERT(Timer < c_MaxTimers);

		switch ( Timer )
		{
		case 2:
			TIM_SetCompare1(TIM2, Compare );
			break;
		case 3:
			TIM_SetCompare1(TIM3, Compare );
			break;
		case 4:
			TIM_SetCompare1(TIM4, Compare );
			break;
		case 5:
			TIM_SetCompare1(TIM5, Compare );
			break;
		default:
			break;
		}
	}

	static UINT16 GetCompare ( UINT32 Timer )
	{
		ASSERT(Timer < c_MaxTimers);
		switch ( Timer )
		{
		case 2:
			return TIM2->CCR1;
			break;
		case 3:
			return TIM3->CCR1;
			break;
		case 4:
			return TIM4->CCR1;
			break;
		case 5:
			return TIM5->CCR1;
			break;
		default:
			break;
		}

	}

	static UINT16 GetCounter( UINT32 Timer )
    {
		//Read the counter value for the specified timer

		ASSERT(Timer < c_MaxTimers);

		switch ( Timer )
		{
		case 2:
			return TIM_GetCounter(TIM2);
			break;
		case 3:
			return TIM_GetCounter(TIM3);
			break;
		case 4:
			return TIM_GetCounter(TIM4);
			break;
		case 5:
			return TIM_GetCounter(TIM5);
			break;
		default:
			break;
		}

	}

	static void SetCounter ( UINT32 Timer, UINT32 Count )
	{

		ASSERT(Timer < c_MaxTimers);

		switch ( Timer )
		{
		case 2:
			return TIM_SetCounter(TIM2, Count);
			break;
		case 3:
			return TIM_SetCounter(TIM3, Count);
			break;
		case 4:
			return TIM_SetCounter(TIM4, Count);
			break;
		case 5:
			return TIM_SetCounter(TIM5, Count);
			break;
		default:
			break;
		}

	}

	//TODO: Verify the working of this function from main.cpp

	static BOOL DidTimerOverFlow ( UINT32 Timer )
	{

		//GLOBAL_LOCK(irq);

		ASSERT(Timer < c_MaxTimers);

//		return m_timeOverFlowFlag;

		switch ( Timer )
		{
		case 2:
			return (BOOL) TIM_GetFlagStatus(TIM2, TIM_FLAG_Update);
			break;
		case 3:
			return (BOOL) TIM_GetFlagStatus(TIM3, TIM_FLAG_Update);
			break;
		case 4:
			return (BOOL) TIM_GetFlagStatus(TIM4, TIM_FLAG_Update);
			break;
		case 5:
			return (BOOL) TIM_GetFlagStatus(TIM5, TIM_FLAG_Update);
			break;
		default:
			return FALSE;
			break;
		}

	}

	//TODO: Verify the working of this function!

	static void ClearTimerOverFlow ( UINT32 Timer )
	{
		//GLOBAL_LOCK(irq);

		ASSERT(Timer < c_MaxTimers);

	//	m_timeOverFlowFlag = FALSE;

		switch ( Timer )
		{
		case 2:
			return TIM_ClearFlag(TIM2, TIM_FLAG_Update);
			break;
		case 3:
			return TIM_ClearFlag(TIM3, TIM_FLAG_Update);
			break;
		case 4:
			return TIM_ClearFlag(TIM4, TIM_FLAG_Update);
			break;
		case 5:
			return TIM_ClearFlag(TIM5, TIM_FLAG_Update);
			break;
		default:
			return;
			break;
		}

	}

	static BOOL DidCompareHit( UINT32 Timer )
    {
		ASSERT(Timer < c_MaxTimers);

		switch ( Timer )
		{
		case 2:
			return (BOOL) TIM_GetFlagStatus(TIM2, TIM_FLAG_CC1);
			break;
		case 3:
			return (BOOL) TIM_GetFlagStatus(TIM3, TIM_FLAG_CC1);
			break;
		case 4:
			return (BOOL) TIM_GetFlagStatus(TIM4, TIM_FLAG_CC1);
			break;
		case 5:
			return (BOOL) TIM_GetFlagStatus(TIM5, TIM_FLAG_CC1);
			break;
		default:
			break;
		}

	}

	static void ResetCompareHit (UINT32 Timer )
	{
		ASSERT(Timer < c_MaxTimers);

		switch ( Timer )
		{
		case 2:
			TIM_ClearFlag(TIM2, TIM_FLAG_CC1);
			break;
		case 3:
			TIM_ClearFlag(TIM3, TIM_FLAG_CC1);
			break;
		case 4:
			TIM_ClearFlag(TIM4, TIM_FLAG_CC1);
			break;
		case 5:
			TIM_ClearFlag(TIM5, TIM_FLAG_CC1);
			break;
		default:
			break;
		}
	}

	/*Commented the Private because
	 * I need to access the Descriptors from Timer_IRQ_Handler
	 * to execute the Timer_IRQ_Handler; Think of a smarter way!
	 */
	//private:
    struct Descriptors
    {
        HAL_CALLBACK_FPN isr;
        void* arg;
        BOOL configured;
		TIM_TimeBaseInitTypeDef timer; //The STM Timer Initialize
		TIM_OCInitTypeDef timer_oc;    //The Output compare function
		NVIC_InitTypeDef interrupt;    //Interrupt details
    };
    Descriptors m_descriptors[MAX_TIMER];
    static void RCC_Init( UINT32 Timer, UINT32 Clock_Prescaler );
    static void RCC_DeInit(UINT32 Timer);
};

BOOL Timer16Bit_Driver::m_timeOverFlowFlag = FALSE;

extern Timer16Bit_Driver g_Timer16Bit_Driver;
extern "C"
{
	void Timer_IRQ_Handler( UINT32 Timer );
	//void Timer_IRQ_Handler();
}

#endif