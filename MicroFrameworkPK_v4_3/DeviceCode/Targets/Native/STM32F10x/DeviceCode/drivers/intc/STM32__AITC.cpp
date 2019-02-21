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
    DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_RTC     ,STM32_AITC::c_IRQ_Priority_1  ),
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
	DEFINE_IRQ( STM32_AITC::c_IRQ_INDEX_RTCAlarm       ,STM32_AITC::c_IRQ_Priority_1 ),
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
  	volatile unsigned long _BFSR ;
  	volatile unsigned long _MMFSR ;
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
 
	
	//
  	// Configurable Fault Status Register
  	// Consists of MMSR, BFSR and UFSR
  	_CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;

	_BFSR = (_CFSR >> 8) & 0xFF;

	_MMFSR = (_CFSR) & 0xFF;

	// Usage Fault Status Register
	_UFSR = _CFSR >> 16;
 
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
//MemManage Fault Address Register (MMFAR) valid flag:
//0 = value in MMAR is not a valid fault address
//1 = MMAR holds a valid fault address.
//If a MemManage fault occurs and is escalated to a HardFault because of priority, the HardFault handler must set this bit to 0. This prevents problems on return to a stacked active MemManage fault handler whose MMAR value has been overwritten.
	volatile bool MMARVALID = (_MMFSR & (1<<7)) > 0; // Indicates the Memory Manage Address register (MMAR) contains a valid fault addressing value.


//MemManage fault on stacking for exception entry:
//0 = no stacking fault
//1 = stacking for an exception entry has caused one or more access violations. 
//When this bit is 1, the SP is still adjusted but the values in the context area on the stack might be incorrect. The processor has not written a fault address to the MMAR. 
volatile bool MSTKERR   = (_MMFSR & (1<<4)) > 0; // Error occurred during stacking (starting of exception).
	                                       // 1. Stack pointer is corrupted.
	                                       // 2. Stack size is too large, reaching a region not defined by the MPU or disallowed in the MPU configuration.



//MemManage fault on unstacking for a return from exception:
//0 = no unstacking fault
//1 = unstack for an exception return has caused one or more access violations.
//This fault is chained to the handler. This means that when this bit is 1, the original return stack is still present. The processor has not adjusted the SP from the failing return, and has not performed a new save. The processor has not written a fault address to the MMAR.
	volatile bool MUNSTKERR = (_MMFSR & (1<<3)) > 0; // Error occurred during unstacking (ending of exception). If there was no error
	                                       // stacking but error occurred during unstacking, it might be because of the
	                                       // following reasons:
	                                       // 1. Stack pointer was corrupted during exception.
	                                       // 2. MPU configuration was changed by exception handler.
										   
										   
										   
//Data access violation flag:
//0 = no data access violation fault
//1 = the processor attempted a load or store at a location that does not permit the operation.
//When this bit is 1, the PC value stacked for the exception return points to the faulting instruction. The processor has loaded the MMAR with the address of the attempted access.
	volatile bool DACCVIOL = (_MMFSR & (1<<1)) > 0;  // Violation to memory access protection, which is defined by MPU setup.
	                                       // For example, user application trying to access privileged-only region.
										   
										   
										   
//Instruction access violation flag:
//0 = no instruction access violation fault
//1 = the processor attempted an instruction fetch from a location that does not permit execution. 
//This fault occurs on any access to an XN region, even when the MPU is disabled or not present.
//When this bit is 1, the PC value stacked for the exception return points to the faulting instruction. The processor has not written a fault address to the MMAR.										   
	volatile bool IACCVIOL = (_MMFSR & (1<<0)) > 0;  // 1. Violation to memory access protection, which is defined by MPU setup.
	                                       // For example, user application trying to access privileged-only region.
	                                       // Stacked PC might be able to locate the code that caused the problem.
	                                       // 2. Branch to nonexecutable regions.
	                                       // 3. Invalid exception return code.
	                                       // 4. Invalid entry in exception vector table. For example, loading of an executable
	                                       // image for traditional ARM core into the memory, or exception happened before vector table was set.
	                                       // 5. Stacked PC corrupted during exception handling.


//BusFault Address Register (BFAR) valid flag:
//0 = value in BFAR is not a valid fault address
//1 = BFAR holds a valid fault address.
//The processor sets this bit to 1 after a BusFault where the address is known. Other faults can set this bit to 0, such as a MemManage fault occurring later.
//If a BusFault occurs and is escalated to a hard fault because of priority, the hard fault handler must set this bit to 0. This prevents problems if returning to a stacked active BusFault handler whose BFAR value has been overwritten.
	volatile bool BFARVALID = (_BFSR & (1<<7)) > 0; // Indicates the Bus Fault Address register contains a valid bus fault address.


//BusFault on stacking for exception entry:
//0 = no stacking fault
//1 = stacking for an exception entry has caused one or more BusFaults. 
//When the processor sets this bit to 1, the SP is still adjusted but the values in the context area on the stack might be incorrect. The processor does not write a fault address to the BFAR.	
	volatile bool STKERR    = (_BFSR & (1<<4)) > 0; // Error occurred during stacking (starting of exception).


//BusFault on unstacking for a return from exception:
//0 = no unstacking fault
//1 = unstack for an exception return has caused one or more BusFaults.
//This fault is chained to the handler. This means that when the processor sets this bit to 1, the original return stack is still present. The processor does not adjust the SP from the failing return, does not performed a new save, and does not write a fault address to the BFAR.
	volatile bool UNSTKERR  = (_BFSR & (1<<3)) > 0; // Error occurred during unstacking (ending of exception. If there was no error
	                                       // stacking but error occurred during unstacking, it might be that the stack pointer was corrupted during exception.
	
	
//Imprecise data bus error: 
//0 = no imprecise data bus error
//1 = a data bus error has occurred, but the return address in the stack frame is not related to the instruction that caused the error. 
//When the processor sets this bit to 1, it does not write a fault address to the BFAR.
//This is an asynchronous fault. Therefore, if it is detected when the priority of the current process is higher than the BusFault priority, the BusFault becomes pending and becomes active only when the processor returns from all higher priority processes. If a precise fault occurs before the processor enters the handler for the imprecise BusFault, the handler detects both IMPRECISERR set to 1 and one of the precise fault status bits set to 1.	
	volatile bool IMPRECISERR=(_BFSR & (1<<2)) > 0; // Bus error during data access. The fault address may be indicated by BFAR.



//Precise data bus error: 
//0 = no precise data bus error
//1 = a data bus error has occurred, and the PC value stacked for the exception return points to the instruction that caused the fault.
//When the processor sets this bit is 1, it writes the faulting address to the BFAR.
	volatile bool PRECISERR = (_BFSR & (1<<1)) > 0; // Bus error during data access. The fault address may be inidcated by BFAR.




//Instruction bus error:
//0 = no instruction bus error
//1 = instruction bus error.
//The processor detects the instruction bus error on prefetching an instruction, but it sets the IBUSERR flag to 1 only if it attempts to issue the faulting instruction. 
//When the processor sets this bit is 1, it does not write a fault address to the BFAR.
	volatile bool IBUSERR   = (_BFSR & (1<<0)) > 0; // Branch to invalid memory regions, or invalid exception return code, or invalid entry in exception vector table, or stacked PC corrupted during function calls, or access to NVIC or SCB in user mode (nonprivileged).


//Divide by zero UsageFault: 
//0 = no divide by zero fault, or divide by zero trapping not enabled
//1 = the processor has executed an SDIV or UDIV instruction with a divisor of 0.
//When the processor sets this bit to 1, the PC value stacked for the exception return points to the instruction that performed the divide by zero.
//Enable trapping of divide by zero by setting the DIV_0_TRP bit in the CCR to 1, see Configuration and Control Register.	
	volatile bool DIVBYZERO = (_UFSR & (1<<9)) > 0; // is DIV_0_TRP set? find code at fault with stacked PC



//Unaligned access UsageFault:
//0 = no unaligned access fault, or unaligned access trapping not enabled
//1 = the processor has made an unaligned memory access.
//Enable trapping of unaligned accesses by setting the UNALIGN_TRP bit in the CCR to 1, see Configuration and Control Register.
//Unaligned LDM, STM, LDRD, and STRD instructions always fault irrespective of the setting of UNALIGN_TRP.	
	volatile bool UNALIGNED = (_UFSR & (1<<8)) > 0;


//No coprocessor UsageFault. The processor does not support coprocessor instructions:
//0 = no UsageFault caused by attempting to access a coprocessor
//1 = the processor has attempted to access a coprocessor.
	volatile bool NOCP      = (_UFSR & (1<<3)) > 0;


//Invalid PC load UsageFault, caused by an invalid PC load by EXC_RETURN:
//0 = no invalid PC load UsageFault
//1 = the processor has attempted an illegal load of EXC_RETURN to the PC, as a result of an invalid context, or an invalid EXC_RETURN value. 
//When this bit is set to 1, the PC value stacked for the exception return points to the instruction that tried to perform the illegal load of the PC.	
	volatile bool INVPC     = (_UFSR & (1<<2)) > 0;



//Invalid state UsageFault:
//0 = no invalid state UsageFault
//1 = the processor has attempted to execute an instruction that makes illegal use of the EPSR.
//When this bit is set to 1, the PC value stacked for the exception return points to the instruction that attempted the illegal use of the EPSR.
//This bit is not set to 1 if an undefined instruction uses the EPSR.
	volatile bool INVSTATE  = (_UFSR & (1<<1)) > 0;



//Undefined instruction UsageFault:
//0 = no undefined instruction UsageFault
//1 = the processor has attempted to execute an undefined instruction.
//When this bit is set to 1, the PC value stacked for the exception return points to the undefined instruction.
//An undefined instruction is an instruction that the processor cannot decode.	
	volatile bool UNDEFINSTR= (_UFSR & (1<<0)) > 0; // 1. Use of instructions not supported in Cortex-M3.
	                                       // 2. Bad/corrupted memory contents.
	                                       // 3. Loading of ARM object code during link stage. Checks compile steps.
	                                       // 4. Instruction align problem. for example, if GNU tool chain is used, omitting of
	                                       //    .align after .ascii might cause next instruction to be unaligned
	volatile bool DEBUGEVF  = (_HFSR &(1<<31)) > 0;



//Indicates a forced hard fault, generated by escalation of a fault with configurable priority that cannot be handles, either because of priority or because it is disabled:
//0 = no forced HardFault
//1 = forced HardFault.
//When this bit is set to 1, the HardFault handler must read the other fault status registers to find the cause of the fault.	
	volatile bool FORCED    = (_HFSR &(1<<30)) > 0; // 1. Trying to run SVC/BKPT within SVC/monitor or another handler with same or higher priority.
	                                       // 2. A hard fault occurred if the corresponding handler is disabled or cannot be started because
	                                       // another exception with the same or higher priority is running, or because another exception
	                                       // with the same or higher priority is running, or because exception mask is set.
										   
										   
										   
//Indicates a BusFault on a vector table read during exception processing:
//0 = no BusFault on vector table read
//1 = BusFault on vector table read.
//This error is always handled by the hard fault handler.
//When this bit is set to 1, the PC value stacked for the exception return points to the instruction that was preempted by the exception.
	volatile bool VECTBL    = (_HFSR &(1<< 1)) > 0;

	volatile bool EXTERNAL  = (_DFSR & (1<<4)) > 0;
	volatile bool VCATCH    = (_DFSR & (1<<3)) > 0;
	volatile bool DWTTRAP   = (_DFSR & (1<<2)) > 0; // DWT watchpoint event has occurred.
	volatile bool BKPT      = (_DFSR & (1<<1)) > 0; // Breakpoint instruction is executed, or FPB unit generated breakpoint event.
	volatile bool HALTED    = (_DFSR & (1<<0)) > 0; // Halt request in NVIC

    pNVIC      = NVIC;
    pSCB       = SCB;
    pSysTick   = SysTick;
    pITM       = ITM;
    pInterruptType = InterruptType;
    pMPU       = MPU;
    pCoreDebug = CoreDebug;
#endif // defined(DEBUG)

	//http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Cihdjcfc.html
	// use "backtrace full" to read out variables
 	// at this point you can read data from the variables with 
	// "p/x stacked_pc"
	// "info symbol <address>" should list the code line
	// "info address <FunctionName>"
	// "info registers" might help 
	// "*((char *)0x00) = 5;" should create a hard-fault to test
//    SOFT_BREAKPOINT();
//#if !defined(NDEBUG)
 	while (1){
	}
//#endif
	
}


	void __irq HardFault_Handler()
	{
			// This assembly code will find the location of the stack and pass it to a C function hard fault handler (HardFault_HandlerC)
			asm(
				"TST LR, #4 \n"          // Test EXC_RETURN number in LR bit 2 to determine if main stack or program stack is in use.
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
		SCB->ICSR |= SCB_ICSR_PENDSVCLR;

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

		GLOBAL_LOCK(irq);
		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		irq.Release();

#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_RTC]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_RTC];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		irq.Acquire();
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
		GLOBAL_LOCK(irq);
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		irq.Release();
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL1]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL1];

		IsrVector->Handler.Execute();

		irq.Acquire();
		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		irq.Release();
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
		GLOBAL_LOCK(irq);
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		// do not release irq. *SPOT_Hardware_Port::IsrProcedure() wants IRQ off.
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_EXTI9_5]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI9_5];

		IsrVector->Handler.Execute();

		irq.Acquire();
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

			GLOBAL_LOCK(irq);
			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
			irq.Release();
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_I2C1_EV]++;
#endif

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_I2C1_EV];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			irq.Acquire();
			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
			irq.Release();
		}
		
	void __irq I2C1_ER_IRQHandler()
		{

			UINT32 index;

			STM32_AITC& AITC = STM32::AITC();

			GLOBAL_LOCK(irq);
			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
			irq.Release();
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_I2C1_ER]++;
#endif

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_I2C1_ER];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			irq.Acquire();
			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
			irq.Release();
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
#if defined(PLATFORM_ARM_KRAIT)
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
			GLOBAL_LOCK(irq);
			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
			irq.Release();
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_EXTI15_10]++;
#endif

			STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI15_10];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();
			irq.Acquire();
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
		GLOBAL_LOCK(irq);
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		irq.Release();

#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_EXTI0]++;
#endif

		STM32_AITC_Driver::IRQ_VECTORING* IsrVector = &STM32_AITC_Driver::s_IsrTable[STM32_AITC::c_IRQ_INDEX_EXTI0];

		IsrVector->Handler.Execute();

		irq.Acquire();
		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		irq.Release();
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

