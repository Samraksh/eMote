////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Samraksh Company
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <stm32f10x.h>
//#include <led/stm32f10x_led.h>
//#include <gpio/stm32f10x_gpio.h>
#include "STM32.h"

////////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

////////////////////////////////////////////////////////////////////////////////

//#define DEBUG_DOTNOW_ISR

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
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM1_CC       ,STM32_AITC::c_IRQ_Priority_1 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM2       ,STM32_AITC::c_IRQ_Priority_2 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM3       ,STM32_AITC::c_IRQ_Priority_5 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM4       ,STM32_AITC::c_IRQ_Priority_3 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C1_EV       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C1_ER       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C2_EV       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_I2C2_ER       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SPI1       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_SPI2       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART1       ,STM32_AITC::c_IRQ_Priority_0 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART2       ,STM32_AITC::c_IRQ_Priority_5 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USART3       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_EXTI15_10       ,STM32_AITC::c_IRQ_Priority_10 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_RTCAlarm       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_USBWakeUp       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM8_BRK_TIM12       ,STM32_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM8_UP       ,STM32_AITC::c_IRQ_Priority_14 ),
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_TIM8_TRG_COM       ,STM32_AITC::c_IRQ_Priority_15 ),
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
	 
#ifdef DEBUG_DOTNOW_ISR
	memset(interrupt_count, 0, 4*64);
#endif

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


		#if defined(DEBUG)
		// help GDB inspect CMSIS register definitions in core_cm3.h
		volatile NVIC_Type*          pNVIC      = NVIC;
		volatile SCB_Type*           pSCB       = SCB;
		volatile SysTick_Type*       pSysTick   = SysTick;
		volatile ITM_Type*           pITM       = ITM;
		volatile InterruptType_Type* pInterruptType = InterruptType;
		volatile MPU_Type*           pMPU       = MPU;
		volatile CoreDebug_Type*     pCoreDebug = CoreDebug;
		#endif


__attribute__((optimize("O0")))
void HardFault_HandlerC(unsigned long *hardfault_args)
{
  	volatile unsigned long stacked_r0 ;
  	volatile unsigned long stacked_r1 ;
  	volatile unsigned long stacked_r2 ;
  	volatile unsigned long stacked_r3 ;
  	volatile unsigned long stacked_r12 ;
  	volatile unsigned long stacked_lr ;
  	volatile unsigned long stacked_pc ;
  	volatile unsigned long stacked_psr ;
  	volatile unsigned long _CFSR ;
	volatile unsigned long _UFSR ;
  	volatile unsigned long _HFSR ;
  	volatile unsigned long _DFSR ;
  	volatile unsigned long _AFSR ;
  	volatile unsigned long _BFAR ;
  	volatile unsigned long _MMAR ;
 
  	stacked_r0 = ((unsigned long)hardfault_args[0]) ;
  	stacked_r1 = ((unsigned long)hardfault_args[1]) ;
  	stacked_r2 = ((unsigned long)hardfault_args[2]) ;
  	stacked_r3 = ((unsigned long)hardfault_args[3]) ;
  	stacked_r12 = ((unsigned long)hardfault_args[4]) ;
  	stacked_lr = ((unsigned long)hardfault_args[5]) ;
  	stacked_pc = ((unsigned long)hardfault_args[6]) ;
  	stacked_psr = ((unsigned long)hardfault_args[7]) ;
 
  	// Configurable Fault Status Register
  	// Consists of MMSR, BFSR and UFSR
  	_CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;

	// Usage Fault Status Register
	_UFSR = _CFSR >> 0x10;
 
  	// Hard Fault Status Register
  	_HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;
 
  	// Debug Fault Status Register
  	_DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;
 
  	// Auxiliary Fault Status Register
  	_AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;
 
  	// Read the Fault Address Registers. These may not contain valid values.
  	// Check BFARVALID/MMARVALID to see if they are valid values
  	// MemManage Fault Address Register
  	_MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
  	// Bus Fault Address Register
  	_BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;

#if defined(DEBUG)
	                                       // Possible Causes of Fault Status Registers (Yiu, The definitive guide to the ARM Cortex-M3, 2010)
	bool MMARVALID = (_MMAR & (1<<7)) > 0; // Indicates the Memory Manage Address register (MMAR) contains a valid fault addressing value.
	bool MSTKERR   = (_MMAR & (1<<4)) > 0; // Error occurred during stacking (starting of exception).
	                                       // 1. Stack pointer is corrupted.
	                                       // 2. Stack size is too large, reaching a region not defined by the MPU or disallowed in the MPU configuration.
	bool MUNSTKERR = (_MMAR & (1<<3)) > 0; // Error occurred during unstacking (ending of exception). If there was no error
	                                       // stacking but error occurred during unstacking, it might be because of the
	                                       // following reasons:
	                                       // 1. Stack pointer was corrupted during exception.
	                                       // 2. MPU configuration was changed by exception handler.
	bool DACCVIOL = (_MMAR & (1<<1)) > 0;  // Violation to memory access protection, which is defined by MPU setup.
	                                       // For example, user application trying to access privileged-only region.
	bool IACCVIOL = (_MMAR & (1<<0)) > 0;  // 1. Violation to memory access protection, which is defined by MPU setup.
	                                       // For example, user application trying to access privileged-only region.
	                                       // Stacked PC might be able to locate the code that caused the problem.
	                                       // 2. Branch to nonexecutable regions.
	                                       // 3. Invalid exception return code.
	                                       // 4. Invalid entry in exception vector table. For example, loading of an executable
	                                       // image for traditional ARM core into the memory, or exception happened before vector table was set.
	                                       // 5. Stacked PC corrupted during exception handling.

	bool BFARVALID = (_BFAR & (1<<7)) > 0; // Indicates the Bus Fault Address register contains a valid bus fault address.
	bool STKERR    = (_BFAR & (1<<4)) > 0; // Error occurred during stacking (starting of exception).
	bool UNSTKERR  = (_BFAR & (1<<3)) > 0; // Error occurred during unstacking (ending of exception. If there was no error
	                                       // stacking but error occurred during unstacking, it might be that the stack pointer was corrupted during exception.
	bool IMPRECISERR=(_BFAR & (1<<2)) > 0;
	bool PRECISERR = (_BFAR & (1<<1)) > 0;
	bool IBUSERR   = (_BFAR & (1<<0)) > 0;

	bool DIVBYZERO = (_UFSR & (1<<9)) > 0;
	bool UNALIGNED = (_UFSR & (1<<8)) > 0;
	bool NOCP      = (_UFSR & (1<<3)) > 0;
	bool INVPC     = (_UFSR & (1<<2)) > 0;
	bool INVSTATE  = (_UFSR & (1<<1)) > 0;
	bool UNDEFINSTR= (_UFSR & (1<<0)) > 0;

	bool DEBUGEVF  = (_HFSR &(1<<31)) > 0;
	bool FORCED    = (_HFSR &(1<<30)) > 0; // 1. Trying to run SVC/BKPT within SVC/monitor or another handler with same or higher priority.
	                                       // 2. A hard fault occurred if the corresponding handler is disabled or cannot be started because
	                                       // another exception with the same or higher priority is running, or because another exception
	                                       // with the same or higher priority is running, or because exception mask is set.
	bool VECTBL    = (_HFSR &(1<< 1)) > 0;

	bool EXTERNAL  = (_DFSR & (1<<4)) > 0;
	bool VCATCH    = (_DFSR & (1<<3)) > 0;
	bool DWTTRAP   = (_DFSR & (1<<2)) > 0;
	bool BKPT      = (_DFSR & (1<<1)) > 0;
	bool HALTED    = (_DFSR & (1<<0)) > 0;

    pNVIC      = NVIC;
    pSCB       = SCB;
    pSysTick   = SysTick;
    pITM       = ITM;
    pInterruptType = InterruptType;
    pMPU       = MPU;
    pCoreDebug = CoreDebug;
#endif // defined(DEBUG)

 	// at this point you can read data from the variables with 
	// "p/x stacked_pc"
	// "info symbol <address>" should list the code line
	// "info address <FunctionName>"
	// "info registers" might help 
	// "*((char *)0x00) = 5;" should create a hard-fault to test
    SOFT_BREAKPOINT();
#if !defined(NDEBUG)
 	while (1){
	}
#endif
	
}


	void __irq HardFault_Handler()
	{
			// This assembly code will find the location of the stack and pass it to a C function hard fault handler (HardFault_HandlerC)
			asm(
				"TST LR, #4 \n"
				"ITE EQ \n"
				"MRSEQ R0, MSP \n"
				"MRSNE R0, PSP \n"
				"B HardFault_HandlerC \n"
   			);
	}


	void __irq SVC_Handler()
	{
		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_SVCall]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_PendSV]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_PendSV];

		// In case the interrupt was forced, remove the flag.
		AITC.RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();

		//ISR_PendSV_Handler(NULL);

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	// Stub here to save potential confusion later.
	void __irq SysTick_Handler()
	{
		HARD_BREAKPOINT();
	}

	void __irq Default_Handler()
	{
		HARD_BREAKPOINT();
	}

	void __irq PVD_IRQHandler()
	{

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_PVD]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TAMPER]++;
#endif

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
			
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_RTC]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_FLASH]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_RCC]++;
#endif

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
		GLOBAL_LOCK(irq);
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		irq.Release();
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_EXTI1]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI1];

		IsrVector->Handler.Execute();

		irq.Acquire();
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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_EXTI2]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_EXTI3]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_EXTI4]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL1]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL2]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL3]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL4]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL5]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL6]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL7]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_ADC1_2]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_USB_HP_CAN_TX]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_USB_LP_CAN_RX0]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_CAN_RX1]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_CAN_SCE]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_EXTI9_5]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM1_BRK_TIM9]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM1_UP_TIM10]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM1_TRG_COM_TIM11]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM1_TRG_COM_TIM11];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM1_CC_IRQHandler()
	{
		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();
		GLOBAL_LOCK(irq);
		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM1_CC]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM1_CC];

		IsrVector->Handler.Execute();


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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM3]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM4]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_I2C1_EV]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_I2C1_ER]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_I2C2_EV]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_I2C2_ER]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_SPI1]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_SPI2]++;
#endif

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_SPI2];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
#ifndef PLATFORM_ARM_EmoteDotNow
		void __irq USART1_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_USART1]++;
#endif

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_USART1];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
#endif
		
		void __irq USART2_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_USART2]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_USART3]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_EXTI15_10]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_RTCAlarm]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_USBWakeUp]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM8_BRK_TIM12]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_USART4]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_USART5]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM8_CC]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_ADC3]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_FSMC]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_SDIO]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM5]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_SPI3]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_SPI3];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	

	void __irq TIM6_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM6]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM7]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA2_Channel1]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA2_Channel2]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA2_Channel3]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA2_Channel4_5]++;
#endif

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
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_EXTI0]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI0];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq TIM2_IRQHandler()
	{

		UINT32 index;

		STM32_AITC& AITC = STM32::AITC();

		GLOBAL_LOCK(irq);
		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM2]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_TIM2];

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

