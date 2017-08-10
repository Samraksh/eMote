////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Samraksh Company
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <cmsis/m2sxxx.h>
//#include <led/SF2f10x_led.h>
//#include <gpio/SF2f10x_gpio.h>
#include "SF2.h"

////////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

////////////////////////////////////////////////////////////////////////////////

//#define DEBUG_DOTNOW_ISR

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "s_IsrTable_SF2"
#endif

#define DEFINE_IRQ(index, priority) { priority, { NULL, (void*)(size_t)index } }

SF2_AITC_Driver::IRQ_VECTORING __section(rwdata) SF2_AITC_Driver::s_IsrTable[] =
{
	DEFINE_IRQ(VectorIndex::c_IRQ_INDEX_WWDG			, SF2_AITC::c_IRQ_Priority_15),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_RTC          ,SF2_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_SPI0            ,SF2_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_SPI1     ,SF2_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_I2C0     ,SF2_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_I2C0_SMBAlert    ,SF2_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_I2C0_SMBus   ,SF2_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_I2C1        ,SF2_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_I2C1_SMBAlert        ,SF2_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_I2C1_SMBus        ,SF2_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_USART1        ,SF2_AITC::c_IRQ_Priority_0  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_USART2       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_EthernetMAC       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_DMA       ,SF2_AITC::c_IRQ_Priority_3  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_TIM1_CC       ,SF2_AITC::c_IRQ_Priority_1  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_TIM2       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_CAN       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_ENVM0       ,SF2_AITC::c_IRQ_Priority_4  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_ENVM1       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_ComBlk       ,SF2_AITC::c_IRQ_Priority_2  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_USB       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_USB_DMA       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_PLL_Lock       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_PLL_LockLost      ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_CommSwitchError       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_CacheError       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_DDR       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_HPDMA_Complete       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_HPDMA_Error       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_ECC_Error       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_MDDR_IOCalib       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FAB_PLL_Lock       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FAB_PLL_LockLost       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FIC64       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq0       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq1       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq2       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq3       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq4       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq5       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq6       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq7       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq8       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq9       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq10       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq11       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq12       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq13       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq14       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_FabricIrq15       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO0       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO1       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO2       ,SF2_AITC::c_IRQ_Priority_15 ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO3       ,SF2_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO4       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO5       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO6       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO7       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO8       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO9       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO10       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO11       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO12       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO13       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO14       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO15       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO16       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO17       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO18       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO19       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO20       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO21       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO22       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO23       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO24       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO25       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO26       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO27       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO28       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO29       ,SF2_AITC::c_IRQ_Priority_15  ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO30       ,SF2_AITC::c_IRQ_Priority_15 ),
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_GPIO31       ,SF2_AITC::c_IRQ_Priority_15 ),

	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_PendSV          ,SF2_AITC::c_IRQ_Priority_15 ), //Mukundan
	DEFINE_IRQ( VectorIndex::c_IRQ_INDEX_SVCall        ,SF2_AITC::c_IRQ_Priority_5  ),
};

#undef DEFINE_IRQ

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif

////////////////////////////////////////////////////////////////////////////////

void SF2_AITC_Driver::Initialize()
{
    SF2_AITC& AITC = SF2::AITC();

	// Nived : Remains of stm's code here .. need to make this more elegant
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
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

BOOL SF2_AITC_Driver::ActivateInterrupt( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param)
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;

    {
        GLOBAL_LOCK(irq);

        SF2_AITC& AITC = SF2::AITC();

        // disable this interrupt while we change it

        AITC.DisableInterrupt(Irq_Index);


        // set the vector
        IsrVector->Handler.Initialize( ISR, ISR_Param );

        	// enable the interrupt if we have a vector
        AITC.EnableInterrupt(Irq_Index);

    }

    return TRUE;
}

BOOL SF2_AITC_Driver::DeactivateInterrupt( UINT32 Irq_Index )
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;
    {
        GLOBAL_LOCK(irq);
        SF2_AITC& AITC = SF2::AITC();

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
		SF2_AITC& AITC = SF2::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_SVCall]++;
#endif

		SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[VectorIndex::c_IRQ_INDEX_SVCall];

		// In case the interrupt was forced, remove the flag.
		AITC.RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();

		//ISR_PendSV_Handler(NULL);

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable

	}

	//This is needed to support realtime timer, this is incomplete
	void __irq PendSV_Handler()
	{
		//Below line is needed, but commented out
		//SCB->ICSR |= SCB_ICSR_PENDSVCLR;

		SF2_AITC& AITC = SF2::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_PendSV]++;
#endif

		SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[VectorIndex::c_IRQ_INDEX_PendSV];

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

	void __irq RTC_IRQHandler()
	{
		UINT32 index;

		SF2_AITC& AITC = SF2::AITC();

		GLOBAL_LOCK(irq);
		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		irq.Release();

#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_RTC]++;
#endif

		SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[VectorIndex::c_IRQ_INDEX_RTC];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		irq.Acquire();
		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq ENVM0_IRQHandler()
	{

		SF2_AITC& AITC = SF2::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_FLASH]++;
#endif

		SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[VectorIndex::c_IRQ_INDEX_ENVM0];

		// In case the interrupt was forced, remove the flag.
		AITC.RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq TIM1_CC_IRQHandler()
	{
		UINT32 index;
		SF2_AITC& AITC = SF2::AITC();
		GLOBAL_LOCK(irq);
		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM1_CC]++;
#endif

		SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[VectorIndex::c_IRQ_INDEX_TIM1_CC];
		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq  TIM2_IRQHandler()
	{
		UINT32 index;
		SF2_AITC& AITC = SF2::AITC();
		GLOBAL_LOCK(irq);
		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_TIM1_CC]++;
#endif

		SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[VectorIndex::c_IRQ_INDEX_TIM2];
		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}


	void __irq USART1_IRQHandler()
	{

		UINT32 index;

		SF2_AITC& AITC = SF2::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

#ifdef DEBUG_DOTNOW_ISR
	interrupt_count[c_IRQ_INDEX_USART1]++;
#endif

		SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[VectorIndex::c_IRQ_INDEX_USART1];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

void __irq USB_IRQHandler()
	{
		UINT32 index;

		SF2_AITC& AITC = SF2::AITC();
		//LED_GREEN();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_USB_LP_CAN_RX0]++;
#endif

		//SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[19];
		SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[VectorIndex::c_IRQ_INDEX_USB];


		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq DMA_IRQHandler()
	{
		UINT32 index;

		SF2_AITC& AITC = SF2::AITC();

		// set before jumping elsewhere or allowing other interrupts
		GLOBAL_LOCK(irq);
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		irq.Release();

#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL1]++;
#endif

		SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[VectorIndex::c_IRQ_INDEX_DMA];

		IsrVector->Handler.Execute();

		irq.Acquire();
		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		irq.Release();
	}


	void __irq ComBlk_IRQHandler()
	{

		SF2_AITC& AITC = SF2::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

#ifdef DEBUG_DOTNOW_ISR
		interrupt_count[c_IRQ_INDEX_ComBlk]++;
#endif

		SF2_AITC_Driver::IRQ_VECTORING* IsrVector = &SF2_AITC_Driver::s_IsrTable[VectorIndex::c_IRQ_INDEX_ComBlk];

		// In case the interrupt was forced, remove the flag.
		AITC.RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}


	void __irq  SPI0_IRQHandler()
	{

	}

	void __irq  SPI1_IRQHandler()
	{

	}

	void __irq  I2C0_IRQHandler()
	{

	}

	void __irq  I2C0_SMBAlert_IRQHandler()
	{

	}

	void __irq  I2C0_SMBus_IRQHandler()
	{

	}

	void __irq  I2C1_IRQHandler()
	{

	}

	void __irq  I2C1_SMBAlert_IRQHandler()
	{

	}

	void __irq  I2C1_SMBus_IRQHandler()
	{

	}

	void __irq  USART2_IRQHandler()
	{

	}

	void __irq  EthernetMAC_IRQHandler()
	{

	}


	void __irq  CAN_IRQHandler()
	{

	}

	void __irq  EVM0_IRQHandler()
	{

	}

	void __irq  EVM1_IRQHandler()
	{

	}

	void __irq  USB_DMA_IRQHandler()
	{

	}

	void __irq  PLL_Lock_IRQHandler()
	{

	}

	void __irq  PLL_LockLost_IRQHandler()
	{

	}

	void __irq  CommSwitchError_IRQHandler()
	{

	}

	void __irq  CacheError_IRQHandler()
	{

	}

	void __irq  DDR_IRQHandler()
	{

	}

	void __irq  HPDMA_Error_IRQHandler()
	{

	}

	void __irq  MDDR_IOCalib_IRQHandler()
	{

	}

	void __irq  FAB_PLL_Lock_IRQHandler()
	{

	}

	void __irq  FAB_PLL_LockLost_IRQHandler()
	{

	}

	void __irq  FIC64_IRQHandler()
	{

	}

	void __irq  FabricIrq0_IRQHandler()
	{

	}

	void __irq  FabricIrq1_IRQHandler()
	{

	}

	void __irq  FabricIrq2_IRQHandler()
	{

	}

	void __irq  FabricIrq3_IRQHandler()
	{

	}

	void __irq  FabricIrq4_IRQHandler()
	{

	}

	void __irq  FabricIrq5_IRQHandler()
	{

	}

	void __irq  FabricIrq6_IRQHandler()
	{

	}

	void __irq  FabricIrq7_IRQHandler()
	{

	}

	void __irq  FabricIrq8_IRQHandler()
	{

	}

	void __irq  FabricIrq9_IRQHandler()
	{

	}

	void __irq  FabricIrq10_IRQHandler()
	{

	}

	void __irq  FabricIrq11_IRQHandler()
	{

	}

	void __irq  FabricIrq12_IRQHandler()
	{

	}

	void __irq  FabricIrq13_IRQHandler()
	{

	}

	void __irq  FabricIrq14_IRQHandler()
	{

	}

	void __irq  FabricIrq15_IRQHandler()
	{

	}

	void __irq  GPIO0_IRQHandler()
	{

	}

	void __irq  GPIO1_IRQHandler()
	{

	}

	void __irq  GPIO2_IRQHandler()
	{

	}

	void __irq  GPIO3_IRQHandler()
	{

	}

	void __irq  GPIO4_IRQHandler()
	{

	}

	void __irq  GPIO5_IRQHandler()
	{

	}

	void __irq  GPIO6_IRQHandler()
	{

	}

	void __irq  GPIO7_IRQHandler()
	{

	}

	void __irq  GPIO8_IRQHandler()
	{

	}

	void __irq  GPIO9_IRQHandler()
	{

	}

	void __irq  GPIO10_IRQHandler()
	{

	}

	void __irq  GPIO11_IRQHandler()
	{

	}

	void __irq  GPIO12_IRQHandler()
	{

	}

	void __irq  GPIO13_IRQHandler()
	{

	}

	void __irq  GPIO14_IRQHandler()
	{

	}

	void __irq  GPIO15_IRQHandler()
	{

	}

	void __irq  GPIO16_IRQHandler()
	{

	}

	void __irq  GPIO17_IRQHandler()
	{

	}

	void __irq  GPIO18_IRQHandler()
	{

	}

	void __irq  GPIO19_IRQHandler()
	{

	}

	void __irq  GPIO20_IRQHandler()
	{

	}

	void __irq  GPIO21_IRQHandler()
	{

	}

	void __irq  GPIO22_IRQHandler()
	{

	}

	void __irq  GPIO23_IRQHandler()
	{

	}

	void __irq  GPIO24_IRQHandler()
	{

	}

	void __irq  GPIO25_IRQHandler()
	{

	}

	void __irq  GPIO26_IRQHandler()
	{

	}

	void __irq  GPIO27_IRQHandler()
	{

	}

	void __irq  GPIO28_IRQHandler()
	{

	}

	void __irq  GPIO29_IRQHandler()
	{

	}

	void __irq  GPIO30_IRQHandler()
	{

	}

	void __irq  GPIO31_IRQHandler()
	{

	}

}	//End extern C


BOOL SF2_AITC_Driver::InterruptEnable( UINT32 Irq_Index  )
{
    if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;
    SF2_AITC& AITC = SF2::AITC();
    GLOBAL_LOCK(irq);
    BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );
    AITC.EnableInterrupt(Irq_Index);
    return WasEnabled;
}


BOOL SF2_AITC_Driver::InterruptDisable( UINT32 Irq_Index )
{
    if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;

    SF2_AITC& AITC = SF2::AITC();

    GLOBAL_LOCK(irq);

    BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

    AITC.DisableInterrupt(Irq_Index);

    return WasEnabled;
}


BOOL SF2_AITC_Driver::InterruptEnableState( UINT32 Irq_Index )
{
    SF2_AITC& AITC = SF2::AITC();

    return AITC.IsInterruptEnabled( Irq_Index );
}


BOOL  SF2_AITC_Driver::InterruptState( UINT32 Irq_Index )
{
    SF2_AITC& AITC = SF2::AITC();
    return AITC.GetInterruptState( Irq_Index );
}

SF2_AITC_Driver::IRQ_VECTORING* SF2_AITC_Driver::IRQToIRQVector( UINT32 IRQ )
{
    IRQ_VECTORING* IsrVector = s_IsrTable;

    if (IRQ < c_VECTORING_GUARD)
    {
        return &IsrVector[IRQ];
    }
    return NULL;
}

void SF2_AITC_Driver::STUB_ISRVector( void* Param )
{
    lcd_printf( "\fSTUB_ISR %08x\r\n", (size_t)Param );
	//Nived :  Calling Default Interrupt Handler
	hal_printf(" Calling Default Interrupt Handler \n");
    HARD_BREAKPOINT();
}

