/*
 * Samraksh Timer Driver
 * Initial Create - Kartik Natarajan
 * 04/20/2011
 * Notes -
 * 1) Driver uses NVIC directly, rewrite parts with INTC
 * 2) The delay between initializing timer and enabling interrupts
 */

#include "netmf_timers16Bit.h"
#include <intc/stm32.h>

Timer16Bit_Driver g_Timer16Bit_Driver;

HAL_CALLBACK_FPN HALTimerHandlerFPN = NULL;


void HALTimerHardwareHandler(void *args);

//#define HALTIMERDEBUG



BOOL Timer16Bit_Driver::Timer_Test()
{
	return TRUE;
}

UINT8 Timer16Bit_Driver::System_Clock_Frequency()
{
	UINT8 System_Clock = RCC_GetSYSCLKSource();
	return System_Clock;
}

BOOL Timer16Bit_Driver::Initialize( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
	//Assumption - System clock is available at this point.

	UINT16 PrescalerValue = Prescaler;
	UINT16 CCR1_Val = Timer16Bit_Driver :: c_MaxTimerValue;

	ASSERT(Timer < MAX_TIMER);

	GLOBAL_LOCK(irq);

	//Check if timer is already enabled or return false
	if(g_Timer16Bit_Driver.m_descriptors[Timer].configured == TRUE)
	{
		return FALSE;
	}
	switch(Timer)
	{
		case 2:
			TIM_DeInit(TIM2);
			TIM_Cmd(TIM2, DISABLE);
			break;
		case 3:
			TIM_DeInit(TIM3);
			TIM_Cmd(TIM3, DISABLE);
			break;
		case 4:
			TIM_DeInit(TIM4);
			TIM_Cmd(TIM4, DISABLE);
			break;
		case 5:
			TIM_DeInit(TIM5);
			TIM_Cmd(TIM5, DISABLE);
			break;
	}
	//TIM_DeInit(TIM2);
	//TIM_Cmd(TIM2, DISABLE);

	DisableCompareInterrupt ( Timer );

	RCC_DeInit(Timer);

	//Initialize clocks for the timer, prescale if needed
	RCC_Init( Timer, 0 );

	//TODO Prescaler Calculation here

	//Disable any compare interrupts for the timer


	g_Timer16Bit_Driver.m_descriptors[Timer].isr = ISR;
	g_Timer16Bit_Driver.m_descriptors[Timer].arg = ISR_Param;

	if(ISR)
	{
		UINT32 interrupt = 0;
		switch(Timer)
		{
		case 2:
			//interrupt = TIM2_IRQn;
			//Timer base initialize
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_Period = 65535;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_Prescaler = PrescalerValue;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_ClockDivision = 0;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM2, &g_Timer16Bit_Driver.m_descriptors[Timer].timer);
			//TODO Testing time
			//TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);
			//Timer OC initialize
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OCMode = TIM_OCMode_Toggle;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OutputState = TIM_OutputState_Enable;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_Pulse = CCR1_Val;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OCPolarity = TIM_OCPolarity_Low;
			TIM_OC1Init(TIM2, &g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc);
			TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
			//NVIC Init
#if 0
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannel = TIM2_IRQn;
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannelPreemptionPriority = 0;
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannelSubPriority = 1;
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannelCmd = ENABLE;
			//NVIC_Init(&g_Timer16Bit_Driver.m_descriptors[Timer].interrupt);
#endif
			if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM2, ISR, ISR_Param) ) return FALSE;
			break;
		case 3:
			//interrupt = TIM3_IRQn;
			//Timer base initialize
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_Period = 65535;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_Prescaler = PrescalerValue;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_ClockDivision = 0;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM3, &g_Timer16Bit_Driver.m_descriptors[Timer].timer);
			//Timer OC initialize
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OCMode = TIM_OCMode_Toggle;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OutputState = TIM_OutputState_Enable;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_Pulse = CCR1_Val;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OCPolarity = TIM_OCPolarity_Low;
			TIM_OC1Init(TIM3, &g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc);
			TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
			//NVIC Init
#if 0
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannel = TIM3_IRQn;
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannelPreemptionPriority = 0;
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannelSubPriority = 1;
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannelCmd = ENABLE;
#endif
			//NVIC_Init(&g_Timer16Bit_Driver.m_descriptors[Timer].interrupt);
			if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM3, ISR, ISR_Param) ) return FALSE;
			break;
		case 4:
			//interrupt = TIM4_IRQn;
			//Timer base initialize
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_Period = 65535;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_Prescaler = PrescalerValue;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_ClockDivision = 0;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM4, &g_Timer16Bit_Driver.m_descriptors[Timer].timer);
			//Timer OC initialize
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OCMode = TIM_OCMode_Toggle;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OutputState = TIM_OutputState_Enable;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_Pulse = CCR1_Val;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OCPolarity = TIM_OCPolarity_Low;
			TIM_OC1Init(TIM4, &g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc);
			TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
			//NVIC Init
#if 0
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannel = TIM4_IRQn;
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannelPreemptionPriority = 0;
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannelSubPriority = 1;
			g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannelCmd = ENABLE;
#endif
			//NVIC_Init(&g_Timer16Bit_Driver.m_descriptors[Timer].interrupt);
			if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM4, ISR, ISR_Param) ) return FALSE;
			break;
		case 5:
			//Timer base initialize
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_Period = 65535;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_Prescaler = PrescalerValue;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_ClockDivision = 0;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM5, &g_Timer16Bit_Driver.m_descriptors[Timer].timer);
			//Timer OC initialize
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OCMode = TIM_OCMode_Toggle;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OutputState = TIM_OutputState_Enable;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_Pulse = CCR1_Val;
			g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc.TIM_OCPolarity = TIM_OCPolarity_Low;
			TIM_OC1Init(TIM5, &g_Timer16Bit_Driver.m_descriptors[Timer].timer_oc);
			TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Disable);
			//NVIC Init
			//NVIC_Init(&g_Timer16Bit_Driver.m_descriptors[Timer].interrupt);
			if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM5, ISR, ISR_Param) ) return FALSE;
			break;
		default:
			ASSERT(TRUE);
			return FALSE;
		}

		//TODO else (!ISR)

		//if( !CPU_INTC_ActivateInterrupt(interrupt, ISR, ISR_Param) ) return FALSE;
		//NVIC_Init( Timer, ISR, ISR_Param );

		if (FreeRunning)
		{
			//TODO: Add code
			//Set up a free running counter here, which matches its count register and generate interrupts
			switch ( Timer )
			{
			case 2:
				TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Repetitive);
				//TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
				Timer16Bit_Driver::EnableCompareInterrupt( 2 );
				TIM_Cmd(TIM2, ENABLE);
				break;
			case 3:
				TIM_SelectOnePulseMode(TIM3, TIM_OPMode_Repetitive);
				//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
				Timer16Bit_Driver::EnableCompareInterrupt( 3 );
				TIM_Cmd(TIM3, ENABLE);
				break;
			case 4:
				TIM_SelectOnePulseMode(TIM4, TIM_OPMode_Repetitive);
				//TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
				Timer16Bit_Driver::EnableCompareInterrupt( 4 );
				TIM_Cmd(TIM4, ENABLE);
				break;
			case 5:
				TIM_SelectOnePulseMode(TIM5, TIM_OPMode_Repetitive);
				//TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
				Timer16Bit_Driver::EnableCompareInterrupt( 5 );
				TIM_Cmd(TIM5, ENABLE);
				break;
			}

		}
		else
		{
			//TODO: Add code
			//Set up a timer which will execute only once
			switch ( Timer )
			{
				case 2:
					TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);
					//TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
					//for(UINT32 i = 0; i < 10000; i++);
					Timer16Bit_Driver::EnableCompareInterrupt( 2 );
					TIM_Cmd(TIM2, ENABLE);
					break;
				case 3:
					TIM_SelectOnePulseMode(TIM3, TIM_OPMode_Single);
					//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
					Timer16Bit_Driver::EnableCompareInterrupt( 3 );
					TIM_Cmd(TIM3, ENABLE);
					break;
				case 4:
					TIM_SelectOnePulseMode(TIM4, TIM_OPMode_Single);
					//TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
					Timer16Bit_Driver::EnableCompareInterrupt( 4 );
					TIM_Cmd(TIM4, ENABLE);
					break;
				case 5:
					TIM_SelectOnePulseMode(TIM5, TIM_OPMode_Single);
					//TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
					Timer16Bit_Driver::EnableCompareInterrupt( 5 );
					TIM_Cmd(TIM5, ENABLE);
					break;
			}
		}

	}

	g_Timer16Bit_Driver.m_descriptors[Timer].configured = TRUE;

	return TRUE;
}

BOOL Timer16Bit_Driver::Uninitialize ( UINT32 Timer )
{
    ASSERT(Timer < c_MaxTimers);

    GLOBAL_LOCK(irq);

    DisableCompareInterrupt( Timer );

    if(g_Timer16Bit_Driver.m_descriptors[Timer].configured == FALSE) return FALSE;
    g_Timer16Bit_Driver.m_descriptors[Timer].configured = FALSE;

    //g_Timer16Bit_Driver.m_descriptors[Timer].interrupt.NVIC_IRQChannelCmd = DISABLE;

    switch ( Timer )
    {
    case 2:
    	TIM_DeInit(TIM2);
    	break;
    case 3:
    	TIM_DeInit(TIM3);
    	break;
    case 4:
    	TIM_DeInit(TIM4);
    	break;
    case 5:
       	TIM_DeInit(TIM5);
       	break;
    }

    return TRUE;
}

UINT32 Timer16Bit_Driver::GetMaxTicks()
{
	return (UINT32)0xFFFF;
}

void Timer16Bit_Driver::RCC_DeInit(UINT32 Timer)
{
		UINT32 Timer_Clock;
		UINT32 Prescaler;

		switch ( Timer )
		{
		case 2:
			//RCC_PCLK1Config(RCC_HCLK_Div4);
			//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			Timer_Clock = RCC_APB1Periph_TIM2;
			break;
		case 3:
			//RCC_PCLK1Config(RCC_HCLK_Div4);
			//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			Timer_Clock = RCC_APB1Periph_TIM3;
			break;
		case 4:
			//RCC_PCLK1Config(RCC_HCLK_Div4);
			//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			Timer_Clock = RCC_APB1Periph_TIM4;
			break;
		case 5:
			//RCC_PCLK1Config(RCC_HCLK_Div4);
			//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			Timer_Clock = RCC_APB1Periph_TIM5;
			break;
		default:
	#ifdef DEBUG_ON
			debug_printf("Should not be here\n\r");
	#endif
			break;
		}
		RCC_APB1PeriphClockCmd(Timer_Clock, DISABLE);


}

void Timer16Bit_Driver::RCC_Init( UINT32 Timer, UINT32 Clock_Prescaler )
{
	//Configure Timer Prescaler and Enable the Peripheral clock

	UINT32 Timer_Clock;
	UINT32 Prescaler;

	switch ( Timer )
	{
	case 2:
		//RCC_PCLK1Config(RCC_HCLK_Div4);
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		Timer_Clock = RCC_APB1Periph_TIM2;
		break;
	case 3:
		//RCC_PCLK1Config(RCC_HCLK_Div4);
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		Timer_Clock = RCC_APB1Periph_TIM3;
		break;
	case 4:
		//RCC_PCLK1Config(RCC_HCLK_Div4);
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		Timer_Clock = RCC_APB1Periph_TIM4;
		break;
	case 5:
		//RCC_PCLK1Config(RCC_HCLK_Div4);
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
		Timer_Clock = RCC_APB1Periph_TIM5;
		break;
	default:
#ifdef DEBUG_ON
		debug_printf("Should not be here\n\r");
#endif
		break;
	}

	switch ( Clock_Prescaler )
	{
		case 2:
			Prescaler = RCC_HCLK_Div2;
			break;
		case 4:
			Prescaler = RCC_HCLK_Div4;
			break;
		case 8:
			Prescaler = RCC_HCLK_Div8;
			break;
		case 16:
			Prescaler = RCC_HCLK_Div16;
			break;
		default:
			Prescaler = RCC_HCLK_Div1;
			break;
	}

	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_APB1PeriphClockCmd(Timer_Clock, ENABLE);

}

/*
void Timer_IRQ_Handler( UINT32 Timer )
{
	//Execute the ISR for the timer
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
   	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
      TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );
	  STM_EVAL_LEDToggle(1);
	  (g_Timer16Bit_Driver.m_descriptors[Timer].isr)(g_Timer16Bit_Driver.m_descriptors[Timer].arg);
      //Timer_IRQ_Handler( 2 );
	}	
}

void Timer_IRQ_Handler()
{
	STM_EVAL_LEDInit((Led_TypeDef)3);
	STM_EVAL_LEDToggle((Led_TypeDef)3);

}
*/
