////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Samraksh Company
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
//#include <led/stm32f10x_led.h>
#include <tim/netmf_timers.h>
#include <gpio/stm32f10x_gpio.h>
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
	DEFINE_IRQ(STM32_AITC::c_IRQ_INDEX_WWDG			, STM32_AITC::c_IRQ_Priority_15),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_PVD          ,STM32_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TAMPER            ,STM32_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_RTC     ,STM32_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_FLASH     ,STM32_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_RCC    ,STM32_AITC::c_IRQ_Priority_10  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI0   ,STM32_AITC::c_IRQ_Priority_10  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI1        ,STM32_AITC::c_IRQ_Priority_10  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI2        ,STM32_AITC::c_IRQ_Priority_10  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI3        ,STM32_AITC::c_IRQ_Priority_10  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI4        ,STM32_AITC::c_IRQ_Priority_10  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL1       ,STM32_AITC::c_IRQ_Priority_14  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL2       ,STM32_AITC::c_IRQ_Priority_14  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL3       ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL4       ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL5       ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL6       ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL7       ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_ADC1_2       ,STM32_AITC::c_IRQ_Priority_3  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USB_HP_CAN_TX       ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USB_LP_CAN_RX0       ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_CAN_RX1       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_CAN_SCE       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI9_5       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM1_BRK_TIM9       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM1_UP_TIM10       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM1_TRG_COM_TIM11       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM1_CC       ,STM32_AITC::c_IRQ_Priority_0 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM2       ,STM32_AITC::c_IRQ_Priority_1 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM3       ,STM32_AITC::c_IRQ_Priority_5 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM4       ,STM32_AITC::c_IRQ_Priority_3 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C1_EV       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C1_ER       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C2_EV       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C2_ER       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SPI1       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SPI2       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART1       ,STM32_AITC::c_IRQ_Priority_2 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART2       ,STM32_AITC::c_IRQ_Priority_5 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART3       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI15_10       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_RTCAlarm       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USBWakeUp       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM8_BRK_TIM12       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_Tasklet_High       ,STM32_AITC::c_IRQ_Priority_14 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_Tasklet_Low       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM8_CC       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_ADC3       ,STM32_AITC::c_IRQ_Priority_15 ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_FSMC          ,STM32_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SDIO          ,STM32_AITC::c_IRQ_Priority_5  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM5          ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SPI3          ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART4          ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART5          ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM6          ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM7          ,STM32_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA2_Channel1          ,STM32_AITC::c_IRQ_Priority_14  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA2_Channel2          ,STM32_AITC::c_IRQ_Priority_14  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA2_Channel3          ,STM32_AITC::c_IRQ_Priority_14  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_DMA2_Channel4_5          ,STM32_AITC::c_IRQ_Priority_14  ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_PendSV          ,STM32_AITC::c_IRQ_Priority_15 ), //Mukundan
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SVCall        ,STM32_AITC::c_IRQ_Priority_5  ),
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
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
		void HardFault_Display()
		{
			CPU_GPIO_EnableOutputPin (9, FALSE);
			CPU_GPIO_SetPinState(9,FALSE);
			CPU_GPIO_SetPinState(9,TRUE);
			CPU_GPIO_SetPinState(9,FALSE);
			CPU_GPIO_SetPinState(9,TRUE);
			CPU_GPIO_SetPinState(9,FALSE);


		}

		void __irq HardFault_Handler()
		{
			//Big thanks to joseph.yiu for this handler. This is simply an adaptation of:
			//http://www.st.com/mcu/forums-cat-6778-23.html
			//****************************************************
			//To test this application, you can use this snippet anywhere:
			// //Let's crash the MCU!
			// asm (" MOVS r0, #1 \n"
			// " LDM r0,{r1-r2} \n"
			// " BX LR; \n");
			// Note that this works as-is in IAR 5.20, but could have a different syntax
			// in other compilers.
			//****************************************************
			// hardfault_args is a pointer to your stack. you should change the adress
			// assigned if your compiler places your stack anywhere else!

			CPU_GPIO_EnableOutputPin (9, FALSE);
			CPU_GPIO_SetPinState(9,FALSE);
			CPU_GPIO_SetPinState(9,TRUE);
			CPU_GPIO_SetPinState(9,FALSE);
			CPU_GPIO_SetPinState(9,TRUE);
			CPU_GPIO_SetPinState(9,FALSE);

			unsigned int stacked_r0;
			unsigned int stacked_r1;
			unsigned int stacked_r2;
			unsigned int stacked_r3;
			unsigned int stacked_r12;
			unsigned int stacked_lr;
			unsigned int stacked_pc;
			unsigned int stacked_psr;
			u32* hardfault_args = (u32*) 0x20000400;

			asm( "TST LR, #4 \n"
			"ITE EQ \n"
			"MRSEQ R0, MSP \n"
			"MRSNE R0, PSP \n");

			stacked_r0 = ((unsigned long) hardfault_args[0]);
			stacked_r1 = ((unsigned long) hardfault_args[1]);
			stacked_r2 = ((unsigned long) hardfault_args[2]);
			stacked_r3 = ((unsigned long) hardfault_args[3]);

			stacked_r12 = ((unsigned long) hardfault_args[4]);
			stacked_lr = ((unsigned long) hardfault_args[5]);
			stacked_pc = ((unsigned long) hardfault_args[6]);
			stacked_psr = ((unsigned long) hardfault_args[7]);
			unsigned long BFAR = (*((volatile unsigned long *)(0xE000ED38)));
			unsigned long CFSR = (*((volatile unsigned long *)(0xE000ED28)));
			unsigned long HFSR = (*((volatile unsigned long *)(0xE000ED2C)));
			unsigned long DFSR = (*((volatile unsigned long *)(0xE000ED30)));
			unsigned long AFSR = (*((volatile unsigned long *)(0xE000ED3C)));

			/*printf ("[Hard fault handler]\n");
			printf ("R0 = %x\n", stacked_r0);
			printf ("R1 = %x\n", stacked_r1);
			printf ("R2 = %x\n", stacked_r2);
			printf ("R3 = %x\n", stacked_r3);
			printf ("R12 = %x\n", stacked_r12);
			printf ("LR = %x\n", stacked_lr);
			printf ("PC = %x\n", stacked_pc);
			printf ("PSR = %x\n", stacked_psr);
			printf ("BFAR = %x\n", (*((volatile unsigned long *)(0xE000ED38))));
			printf ("CFSR = %x\n", (*((volatile unsigned long *)(0xE000ED28))));
			printf ("HFSR = %x\n", (*((volatile unsigned long *)(0xE000ED2C))));
			printf ("DFSR = %x\n", (*((volatile unsigned long *)(0xE000ED30))));
			printf ("AFSR = %x\n", (*((volatile unsigned long *)(0xE000ED3C))));*/

			while (1)
			{}
		}


	void __irq SVC_Handler()
	{
		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_SVCall];

		// In case the interrupt was forced, remove the flag.
		AITC.RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();

		//ISR_PendSV_Handler(NULL);

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable

	}


	void __irq PendSV_Handler()
	{

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_PendSV];

		// In case the interrupt was forced, remove the flag.
		AITC.RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();

		//ISR_PendSV_Handler(NULL);

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq Default_Handler()
	{
		while(1);
	}

	void __irq PVD_IRQHandler()
	{

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_PVD];

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

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TAMPER];

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

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_RTC];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq FLASH_IRQHandler()
	{

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_FLASH];

		// In case the interrupt was forced, remove the flag.
		AITC.RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq RCC_IRQHandler()
	{

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_RCC];

		// In case the interrupt was forced, remove the flag.
		AITC.RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	/*
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
	*/

	void __irq EXTI1_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI1];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	

	void __irq EXTI2_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI2];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq EXTI3_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI3];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq EXTI4_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI4];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq DMA1_Channel1_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL1];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA1_Channel2_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL2];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA1_Channel3_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL3];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
		
		void __irq DMA1_Channel4_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL4];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq DMA1_Channel5_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL5];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA1_Channel6_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL6];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
		void __irq DMA1_Channel7_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL7];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq ADC1_2_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_ADC1_2];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq USB_HP_CAN1_TX_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		//STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[18];
		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_USB_HP_CAN_TX];
		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
		void __irq USB_LP_CAN1_RX0_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();
		//LED_GREEN();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		//STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[19];
		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_USB_LP_CAN_RX0];


		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq CAN1_RX1_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_CAN_RX1];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq CAN1_SCE_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_CAN_SCE];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq EXTI9_5_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI9_5];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM1_BRK_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM1_BRK_TIM9];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM1_UP_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM1_UP_TIM10];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM1_TRG_COM_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM1_TRG_COM_TIM11];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM1_CC_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM1_CC];

		IsrVector->Handler.Execute();

		//Tasklet_Run();
		// Tasklet run by directly writing to the register
		NVIC->ISPR[STM32_AITC::c_IRQ_INDEX_Tasklet_High >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_Tasklet_High & (UINT8)0x1F);
		NVIC->ISPR[STM32_AITC::c_IRQ_INDEX_Tasklet_Low >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_Tasklet_Low & (UINT8)0x1F);

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	

    /*	
	void __irq TIM2_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[27];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	*/

	void __irq TIM3_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			//STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[28];
			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM3];

			// In case the interrupt was forced, remove the flag.
			IsrVector->Handler.Execute();
			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}


	void __irq TIM4_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			//STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[29];
			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM4];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}

	/*
	int TimerUnderTest=4;
	int TimerTicks=220;
	int NoOfTimerFiring=100;
	int testCount = 0;
	//int secCount=0;
	UINT16 preCompare=0;
//	int count=0;
	void ISR_TIMER()
	{

		//count = Timer_Driver ::GetCounter(TimerUnderTest);
		GPIOF->BSRR = (0x1 << 7);
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
		preCompare+=TimerTicks;
		Timer_Driver::SetCompare( TimerUnderTest, preCompare);

		//GPIOF->ODR ^= TRUE;
		testCount++;

		GPIOF->BRR = (0x1 << 7);
	}

	void __irq TIM4_IRQHandler()
		{
			ISR_TIMER();
		}
*/

	
	void __irq I2C1_EV_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_I2C1_EV];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
	void __irq I2C1_ER_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_I2C1_ER];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
	
	void __irq I2C2_EV_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_I2C2_EV];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
	void __irq I2C2_ER_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_I2C2_ER];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
	
		void __irq SPI1_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_SPI1];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq SPI2_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_SPI2];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq USART1_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_USART1];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq USART2_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_USART2];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq USART3_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_USART3];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq EXTI15_10_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI15_10];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}

		void __irq RTCAlarm_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_RTCAlarm];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}

		void __irq USBWakeUp_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_USBWakeUp];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq TIM8_BRK_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM8_BRK_TIM12];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq USART4_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_USART4];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq USART5_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_USART5];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq TIM8_CC_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM8_CC];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq ADC3_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_ADC3];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		/*
		void __irq FSMC_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[47];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		*/
		
		void __irq FSMC_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_FSMC];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}


		

		void __irq SDIO_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_SDIO];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM5_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM5];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq SPI3_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_SPI3];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq Tasklet_High_IRQHandler()
	{


		//STM32_AITC& AITC = STM32::AITC();

		// Using direct clear register call here to shave off valuable time off
		// the software interrupt
		// Using the AITC api add overhead
		NVIC->ICPR[STM32_AITC::c_IRQ_INDEX_Tasklet_High >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_Tasklet_High & (UINT8)0x1F);
		//AITC.ClearInterrupt(STM32_AITC::c_IRQ_INDEX_Tasklet_High);


		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_Tasklet_High];

		IsrVector->Handler.Execute();

		//AITC.SetInterrupt(STM32_AITC::c_IRQ_INDEX_Tasklet_High);
		//NVIC->ISPR[STM32_AITC::c_IRQ_INDEX_Tasklet_High >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_Tasklet_High & (UINT8)0x1F);

		// set before jumping elsewhere or allowing other interrupts
		//SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		//SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		//STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_Tasklet_High];

		// In case the interrupt was forced, remove the flag.

		//IsrVector->Handler.Execute();

		//SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		//SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq Tasklet_Low_IRQHandler()
	{


		NVIC->ICPR[STM32_AITC::c_IRQ_INDEX_Tasklet_Low >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_Tasklet_Low & (UINT8)0x1F);

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_Tasklet_Low];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

	}
	
	void __irq TIM6_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM6];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM7_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM7];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA2_Channel1_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA2_Channel1];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA2_Channel2_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA2_Channel2];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA2_Channel3_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA2_Channel3];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA2_Channel4_5_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA2_Channel4_5];

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

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI0];

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

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM2];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		//Tasklet_Run();
		NVIC->ISPR[STM32_AITC::c_IRQ_INDEX_Tasklet_High >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_Tasklet_High & (UINT8)0x1F);
		NVIC->ISPR[STM32_AITC::c_IRQ_INDEX_Tasklet_Low >> 0x05] = (UINT32)0x01 << (STM32_AITC::c_IRQ_INDEX_Tasklet_Low & (UINT8)0x1F);

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

