////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Samraksh Company
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "STM32.h"

////////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

////////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "s_IsrTable_STM32"
#endif

#define DEFINE_IRQ(index, priority) { priority, { NULL, (void*)(size_t)index } }

STM32_AITC_Driver::IRQ_VECTORING __section(rwdata) STM32_AITC_Driver::s_IsrTable[] =
{
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_PVD          ,STM32_AITC::c_IRQ_Priority_8  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TAMPER            ,STM32_AITC::c_IRQ_Priority_9  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_RTC     ,STM32_AITC::c_IRQ_Priority_10  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_FLASH     ,STM32_AITC::c_IRQ_Priority_11  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_RCC    ,STM32_AITC::c_IRQ_Priority_12  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI0   ,STM32_AITC::c_IRQ_Priority_13  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI1        ,STM32_AITC::c_IRQ_Priority_14  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI2        ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI3        ,STM32_AITC::c_IRQ_Priority_16  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI4        ,STM32_AITC::c_IRQ_Priority_17  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL1       ,STM32_AITC::c_IRQ_Priority_18  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL2       ,STM32_AITC::c_IRQ_Priority_19  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL3       ,STM32_AITC::c_IRQ_Priority_20  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL4       ,STM32_AITC::c_IRQ_Priority_21  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL5       ,STM32_AITC::c_IRQ_Priority_22  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL6       ,STM32_AITC::c_IRQ_Priority_23  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL7       ,STM32_AITC::c_IRQ_Priority_24  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_ADC1_2       ,STM32_AITC::c_IRQ_Priority_25  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USB_HP_CAN_TX       ,STM32_AITC::c_IRQ_Priority_26  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USB_LP_CAN_RX0       ,STM32_AITC::c_IRQ_Priority_27  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_CAN_RX1       ,STM32_AITC::c_IRQ_Priority_28 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_CAN_SCE       ,STM32_AITC::c_IRQ_Priority_29 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI9_5       ,STM32_AITC::c_IRQ_Priority_30 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM1_BRK_TIM9       ,STM32_AITC::c_IRQ_Priority_31 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM1_UP_TIM10       ,STM32_AITC::c_IRQ_Priority_32 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM1_TRG_COM_TIM11       ,STM32_AITC::c_IRQ_Priority_33 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM1_CC       ,STM32_AITC::c_IRQ_Priority_34 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM2       ,STM32_AITC::c_IRQ_Priority_35 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM3       ,STM32_AITC::c_IRQ_Priority_36 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM4       ,STM32_AITC::c_IRQ_Priority_37 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C1_EV       ,STM32_AITC::c_IRQ_Priority_38 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C1_ER       ,STM32_AITC::c_IRQ_Priority_39 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C2_EV       ,STM32_AITC::c_IRQ_Priority_40 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C2_ER       ,STM32_AITC::c_IRQ_Priority_41 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SPI1       ,STM32_AITC::c_IRQ_Priority_42 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SPI2       ,STM32_AITC::c_IRQ_Priority_43 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART1       ,STM32_AITC::c_IRQ_Priority_44 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART2       ,STM32_AITC::c_IRQ_Priority_45 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART3       ,STM32_AITC::c_IRQ_Priority_46 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI15_10       ,STM32_AITC::c_IRQ_Priority_47 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_RTCAlarm       ,STM32_AITC::c_IRQ_Priority_48 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USBWakeUp       ,STM32_AITC::c_IRQ_Priority_49 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM8_BRK_TIM12       ,STM32_AITC::c_IRQ_Priority_50 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM8_UP_TIM13       ,STM32_AITC::c_IRQ_Priority_51 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM8_TRG_COM_TIM14       ,STM32_AITC::c_IRQ_Priority_52 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM8_CC       ,STM32_AITC::c_IRQ_Priority_53 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_ADC3       ,STM32_AITC::c_IRQ_Priority_54 ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_FSMC          ,STM32_AITC::c_IRQ_Priority_55  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SDIO          ,STM32_AITC::c_IRQ_Priority_56  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM5          ,STM32_AITC::c_IRQ_Priority_57  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SPI3          ,STM32_AITC::c_IRQ_Priority_58  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_UART4          ,STM32_AITC::c_IRQ_Priority_59  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_UART5          ,STM32_AITC::c_IRQ_Priority_60  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM6          ,STM32_AITC::c_IRQ_Priority_61  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM7          ,STM32_AITC::c_IRQ_Priority_62  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA2_Channel1          ,STM32_AITC::c_IRQ_Priority_63  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA2_Channel2          ,STM32_AITC::c_IRQ_Priority_64  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA2_Channel3          ,STM32_AITC::c_IRQ_Priority_65  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA2_Channel4_5          ,STM32_AITC::c_IRQ_Priority_66  ),

};

#undef DEFINE_IRQ

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif

////////////////////////////////////////////////////////////////////////////////

void STM32_AITC_Driver::Initialize()
{
    STM32_AITC& AITC = STM32::AITC();

	// Nived : Remains of stm's code here .. need to make this more elegant
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    // // disable all interrupts
    // AITC.ICMR = 0;
    // AITC.ICMR2 = 0;
    // // Only active interrupt will trigger the IRQ on idle
    // AITC.ICCR = AITC.ICCR__DIM_OTHER;

	// Nived : Test 1 - Check if this actually disables all interrupts.
	for(int i = 0 ; i < 8; i++)
	{
		AITC.ICER[i] = 0xFFFFFFFF;   // Clear all enabled interrupts
		AITC.ICPR[i] = 0xFFFFFFFF;   // Clear all pending interrupts if any
	}

    // // set all priorities to the lowest
     IRQ_VECTORING* IsrVector = s_IsrTable;

    // // set the priority level for each IRQ and stub the IRQ callback
     for (int i=0; i<c_VECTORING_GUARD;i++)
     {
         AITC.SetPriority(i, IsrVector[i].Priority );
         IsrVector->Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector->Priority);
     }

}

BOOL STM32_AITC_Driver::ActivateInterrupt( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param)
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;

    {
        GLOBAL_LOCK(irq);

        STM32_AITC& AITC = STM32::AITC();

        // disable this interrupt while we change it
        AITC.DisableInterrupt(Irq_Index);

        // set the vector
        IsrVector->Handler.Initialize( ISR, ISR_Param );

        // enable the interrupt if we have a vector
        AITC.EnableInterrupt(Irq_Index);
    }

    return TRUE;
}

BOOL STM32_AITC_Driver::DeactivateInterrupt( UINT32 Irq_Index )
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;

    {
        GLOBAL_LOCK(irq);

        STM32_AITC& AITC = STM32::AITC();

        // disable this interrupt while we change it
        AITC.DisableInterrupt(Irq_Index);
        // as it is stub, just put the Priority to the ISR parameter
        IsrVector->Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector->Priority );
    }

    return TRUE;
}

extern "C"
{

	void __irq PVD_IRQHandler()
	{

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[0];

		// In case the interrupt was forced, remove the flag.
		AITC.RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq TAMPER_IRQHandler()
	{

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[1];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq RTC_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[2];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}



	void __irq EXTI0_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[5];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq TIM2_IRQHandler()
	{		
		
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[28];
		
		// In case the interrupt was forced, remove the flag.
		
		IsrVector->Handler.Execute();
		
		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
}


BOOL STM32_AITC_Driver::InterruptEnable( UINT32 Irq_Index  )
{

    if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;

    STM32_AITC& AITC = STM32::AITC();

    GLOBAL_LOCK(irq);

    BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

    AITC.EnableInterrupt(Irq_Index);

    return WasEnabled;
}


BOOL STM32_AITC_Driver::InterruptDisable( UINT32 Irq_Index )
{
    if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;

    STM32_AITC& AITC = STM32::AITC();

    GLOBAL_LOCK(irq);

    BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

    AITC.DisableInterrupt(Irq_Index);

    return WasEnabled;
}


BOOL STM32_AITC_Driver::InterruptEnableState( UINT32 Irq_Index )
{
    STM32_AITC& AITC = STM32::AITC();

    return AITC.IsInterruptEnabled( Irq_Index );
}


BOOL  STM32_AITC_Driver::InterruptState( UINT32 Irq_Index )
{
    STM32_AITC& AITC = STM32::AITC();

    return AITC.GetInterruptState( Irq_Index );
}

STM32_AITC_Driver::IRQ_VECTORING* STM32_AITC_Driver::IRQToIRQVector( UINT32 IRQ )
{
    IRQ_VECTORING* IsrVector = s_IsrTable;



    if (IRQ < c_VECTORING_GUARD)
    {
        return &IsrVector[IRQ];

    }

    return NULL;
}

void STM32_AITC_Driver::STUB_ISRVector( void* Param )
{
    lcd_printf( "\fSTUB_ISR %08x\r\n", (size_t)Param );
	//Nived :  Calling Default Interrupt Handler
	hal_printf(" Calling Default Interrupt Handler \n");
    HARD_BREAKPOINT();

}

