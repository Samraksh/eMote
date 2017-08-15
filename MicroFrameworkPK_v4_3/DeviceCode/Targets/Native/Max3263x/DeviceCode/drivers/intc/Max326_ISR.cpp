////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Samraksh Company
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
//#include <mxc_config.h>
#include <string.h>
//#include <nvic_table.h>
#include "Max326_Intc.h"
#include <DeviceCode/Include/CPU_GPIO_decl.h>

#define HANDLER_DEFAULT(irq_index) { \
	UINT32 index; \
	MAX326_AITC& AITC = MAX326::AITC(); \
	GLOBAL_LOCK(irq); \
															\
	SystemState_SetNoLock( SYSTEM_STATE_ISR              ); \
	SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); \
															\
#ifdef DEBUG_MAX326_ISR \
	interrupt_count[c_IRQ_INDEX_TIM2]++; \
#endif \
	MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[irq_index]; \
	IsrVector->Handler.Execute(); \
															\
	SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); \
	SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); \
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
	bool IMPRECISERR=(_BFAR & (1<<2)) > 0; // Bus error during data access. The fault address may be indicated by BFAR.
	bool PRECISERR = (_BFAR & (1<<1)) > 0; // Bus error during data access. The fault address may be inidcated by BFAR.
	bool IBUSERR   = (_BFAR & (1<<0)) > 0; // Branch to invalid memory regions, or invalid exception return code, or invalid entry in exception vector table, or stacked PC corrupted during function calls, or access to NVIC or SCB in user mode (nonprivileged).

	bool DIVBYZERO = (_UFSR & (1<<9)) > 0; // is DIV_0_TRP set? find code at fault with stacked PC
	bool UNALIGNED = (_UFSR & (1<<8)) > 0;
	bool NOCP      = (_UFSR & (1<<3)) > 0;
	bool INVPC     = (_UFSR & (1<<2)) > 0;
	bool INVSTATE  = (_UFSR & (1<<1)) > 0;
	bool UNDEFINSTR= (_UFSR & (1<<0)) > 0; // 1. Use of instructions not supported in Cortex-M3.
	                                       // 2. Bad/corrupted memory contents.
	                                       // 3. Loading of ARM object code during link stage. Checks compile steps.
	                                       // 4. Instruction align problem. for example, if GNU tool chain is used, omitting of
	                                       //    .align after .ascii might cause next instruction to be unaligned
	bool DEBUGEVF  = (_HFSR &(1<<31)) > 0;
	bool FORCED    = (_HFSR &(1<<30)) > 0; // 1. Trying to run SVC/BKPT within SVC/monitor or another handler with same or higher priority.
	                                       // 2. A hard fault occurred if the corresponding handler is disabled or cannot be started because
	                                       // another exception with the same or higher priority is running, or because another exception
	                                       // with the same or higher priority is running, or because exception mask is set.
	bool VECTBL    = (_HFSR &(1<< 1)) > 0;

	bool EXTERNAL  = (_DFSR & (1<<4)) > 0;
	bool VCATCH    = (_DFSR & (1<<3)) > 0;
	bool DWTTRAP   = (_DFSR & (1<<2)) > 0; // DWT watchpoint event has occurred.
	bool BKPT      = (_DFSR & (1<<1)) > 0; // Breakpoint instruction is executed, or FPB unit generated breakpoint event.
	bool HALTED    = (_DFSR & (1<<0)) > 0; // Halt request in NVIC

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
    //    SOFT_BREAKPOINT();
 	while (1){
	}
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

void __irq SVC_Handler(){  HANDLER_DEFAULT(MAX326_AITC::c_IRQ_INDEX_SVCall) }

void __irq PendSV_Handler(){ HANDLER_DEFAULT(MAX326_AITC::c_IRQ_INDEX_PendSV) }

void __irq SysTick_Handler()
{
	HARD_BREAKPOINT();
}

void __irq Default_Handler()
{
	HARD_BREAKPOINT();
}

void __irq PVD_IRQHandler(){  HANDLER_DEFAULT(MAX326_AITC::c_IRQ_INDEX_PVD)}

void __irq USART1_IRQHandler(){}

void __irq USART2_IRQHandler(){}

void __irq USART3_IRQHandler(){}

void __irq EXTI15_10_IRQHandler(){}

void __irq RTCAlarm_IRQHandler(){}

void __irq USBWakeUp_IRQHandler(){}

void __irq TIM8_BRK_IRQHandler(){}
	
void __irq USART4_IRQHandler(){}
	
void __irq USART5_IRQHandler(){}
	
void __irq TIM8_CC_IRQHandler(){}
	
void __irq ADC1_2_IRQHandler(){}
void __irq ADC3_IRQHandler(){}

void __irq FSMC_IRQHandler(){}

void __irq SDIO_IRQHandler(){}

void __irq SPI1_IRQHandler(){}
void __irq SPI2_IRQHandler(){}
void __irq SPI3_IRQHandler(){}

void __irq TIM1_BRK_IRQHandler(){}
void __irq TIM1_UP_IRQHandler(){}
void __irq TIM1_TRG_IRQHandler(){}
void __irq TIM1_TRG_COM_IRQHandler(){}
void __irq TIM1_CC_IRQHandler(){}

void __irq TIM2_IRQHandler(){}
void __irq TIM3_IRQHandler(){}
void __irq TIM4_IRQHandler(){}
void __irq TIM5_IRQHandler(){}
void __irq TIM6_IRQHandler(){}
void __irq TIM7_IRQHandler(){}

void __irq I2C1_EV_IRQHandler(){}
void __irq I2C1_ER_IRQHandler(){}
void __irq I2C2_EV_IRQHandler(){}
void __irq I2C2_ER_IRQHandler(){}


void __irq DMA2_Channel1_IRQHandler(){}

void __irq DMA2_Channel2_IRQHandler(){}

void __irq DMA2_Channel3_IRQHandler(){}

void __irq DMA2_Channel4_5_IRQHandler(){}

void __irq DMA1_Channel1_IRQHandler(){}
void __irq DMA1_Channel2_IRQHandler(){}
void __irq DMA1_Channel3_IRQHandler(){}
void __irq DMA1_Channel4_IRQHandler(){}
void __irq DMA1_Channel5_IRQHandler(){}
void __irq DMA1_Channel6_IRQHandler(){}
void __irq DMA1_Channel7_IRQHandler(){}


void __irq EXTI0_IRQHandler() {}

void __irq USB_HP_CAN1_TX_IRQHandler() {}
void __irq USB_LP_CAN1_RX0_IRQHandler() {}
void __irq CAN1_TX_IRQHandler() {}
void __irq CAN1_RX1_IRQHandler() {}
void __irq CAN1_SCE_IRQHandler() {}


void __irq TAMPER_IRQHandler(){}

void __irq RTC_IRQHandler(){}

void __irq FLASH_IRQHandler(){}

void __irq RCC_IRQHandler(){}

void __irq EXTI1_IRQHandler(){}
void __irq EXTI2_IRQHandler(){}
void __irq EXTI3_IRQHandler(){}
void __irq EXTI4_IRQHandler(){}

void __irq EXTI9_5_IRQHandler(){}


} //end of extern C

