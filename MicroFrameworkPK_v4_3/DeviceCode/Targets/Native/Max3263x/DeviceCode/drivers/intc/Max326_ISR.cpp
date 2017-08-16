////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Samraksh Company
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <string.h>
#include "Max326_Intc.h"
#include <CPU_GPIO_decl.h>

#ifdef DEBUG_MAX326_ISR
	extern int* interrupt_count;
#endif

#ifdef DEBUG_MAX326_ISR
#define HANDLER_DEFAULT(irqArrayIndex) {\
	UINT32 index;\
	GLOBAL_LOCK(irq);\
	SystemState_SetNoLock(SYSTEM_STATE_ISR); \
	SystemState_SetNoLock(SYSTEM_STATE_NO_CONTINUATIONS);\
	interrupt_count[irqArrayIndex]++;\
	IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[irqArrayIndex]; \
	IsrVector->Handler.Execute(); \
	SystemState_ClearNoLock(SYSTEM_STATE_NO_CONTINUATIONS); \
	SystemState_ClearNoLock(SYSTEM_STATE_ISR); \
}
#else
#define HANDLER_DEFAULT(irqArrayIndex) { \
	UINT32 index; \
	GLOBAL_LOCK(irq); \
	SystemState_SetNoLock(SYSTEM_STATE_ISR); \
	SystemState_SetNoLock(SYSTEM_STATE_NO_CONTINUATIONS); \
	IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[irqArrayIndex]; \
	IsrVector->Handler.Execute(); \
	SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); \
	SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); \
}
#endif


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

   // pNVIC      = NVIC;
    //pSCB       = SCB;
    //pSysTick   = SysTick;
    //pITM       = ITM;
    //pInterruptType = InterruptType;
    //pMPU       = MPU;
    //pCoreDebug = CoreDebug;
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

void __irq SVC_Handler(){  HANDLER_DEFAULT(SVCall_IRQn+MAX326_AITC::c_IRQToIndexOffset) }

void __irq PendSV_Handler(){ HANDLER_DEFAULT(PendSV_IRQn+MAX326_AITC::c_IRQToIndexOffset) }

void __irq SysTick_Handler()
{
	HARD_BREAKPOINT();
}

void __irq Default_Handler()
{
	HARD_BREAKPOINT();
}

void __irq  CLKMAN_IRQHandler (){ HANDLER_DEFAULT(CLKMAN_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  PWRMAN_IRQHandler(){ HANDLER_DEFAULT(PWRMAN_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  FLC_IRQHandler(){ HANDLER_DEFAULT(FLC_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  RTC0_IRQHandler(){ HANDLER_DEFAULT(RTC0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  RTC1_IRQHandler(){ HANDLER_DEFAULT(RTC1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  RTC2_IRQHandler(){ HANDLER_DEFAULT(RTC2_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  RTC3_IRQHandler(){ HANDLER_DEFAULT(RTC3_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  PMU_IRQHandler(){ HANDLER_DEFAULT(PMU_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  USB_IRQHandler(){ HANDLER_DEFAULT(USB_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  AES_IRQHandler(){ HANDLER_DEFAULT(AES_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  MAA_IRQHandler(){ HANDLER_DEFAULT(MAA_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  WDT0_IRQHandler(){ HANDLER_DEFAULT(WDT0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  WDT0_P_IRQHandler(){ HANDLER_DEFAULT(WDT0_P_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  WDT1_IRQHandler(){ HANDLER_DEFAULT(WDT1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  WDT1_P_IRQHandler(){ HANDLER_DEFAULT(WDT1_P_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  GPIO_P0_IRQHandler(){ HANDLER_DEFAULT(GPIO_P0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  GPIO_P1_IRQHandler(){ HANDLER_DEFAULT(GPIO_P1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  GPIO_P2_IRQHandler(){ HANDLER_DEFAULT(GPIO_P2_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  GPIO_P3_IRQHandler(){ HANDLER_DEFAULT(GPIO_P3_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  GPIO_P4_IRQHandler(){ HANDLER_DEFAULT(GPIO_P4_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  GPIO_P5_IRQHandler(){ HANDLER_DEFAULT(GPIO_P5_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  GPIO_P6_IRQHandler(){ HANDLER_DEFAULT(GPIO_P6_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR0_0_IRQHandler(){ HANDLER_DEFAULT(TMR0_0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR0_1_IRQHandler(){ HANDLER_DEFAULT(TMR0_1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR1_0_IRQHandler(){ HANDLER_DEFAULT(TMR1_0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR1_1_IRQHandler(){ HANDLER_DEFAULT(TMR1_1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR2_0_IRQHandler(){ HANDLER_DEFAULT(TMR2_0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR2_1_IRQHandler(){ HANDLER_DEFAULT(TMR2_1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR3_0_IRQHandler(){ HANDLER_DEFAULT(TMR3_0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR3_1_IRQHandler(){ HANDLER_DEFAULT(TMR3_1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR4_0_IRQHandler(){ HANDLER_DEFAULT(TMR4_0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR4_1_IRQHandler(){ HANDLER_DEFAULT(TMR4_1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR5_0_IRQHandler(){ HANDLER_DEFAULT(TMR5_0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  TMR5_1_IRQHandler(){ HANDLER_DEFAULT(TMR5_1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  UART0_IRQHandler(){ HANDLER_DEFAULT(UART0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  UART1_IRQHandler(){ HANDLER_DEFAULT(UART1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  UART2_IRQHandler(){ HANDLER_DEFAULT(UART2_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  UART3_IRQHandler(){ HANDLER_DEFAULT(UART3_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  PT_IRQHandler(){ HANDLER_DEFAULT(PT_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  I2CM0_IRQHandler(){ HANDLER_DEFAULT(I2CM0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  I2CM1_IRQHandler(){ HANDLER_DEFAULT(I2CM1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  I2CM2_IRQHandler(){ HANDLER_DEFAULT(I2CM2_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  I2CS_IRQHandler(){ HANDLER_DEFAULT(I2CS_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  SPIM0_IRQHandler(){ HANDLER_DEFAULT(SPIM0_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  SPIM1_IRQHandler(){ HANDLER_DEFAULT(SPIM1_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  SPIM2_IRQHandler(){ HANDLER_DEFAULT(SPIM2_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  SPIB_IRQHandler(){ HANDLER_DEFAULT(SPIB_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  OWM_IRQHandler(){ HANDLER_DEFAULT(OWM_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  AFE_IRQHandler(){ HANDLER_DEFAULT(AFE_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  SPIS_IRQHandler(){ HANDLER_DEFAULT(SPIS_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  GPIO_P7_IRQHandler(){ HANDLER_DEFAULT(GPIO_P7_IRQn+MAX326_AITC::c_IRQToIndexOffset) }
void __irq  GPIO_P8_IRQHandler(){ HANDLER_DEFAULT(GPIO_P8_IRQn+MAX326_AITC::c_IRQToIndexOffset) }

} //end of extern C

