/*
 *  Name  : TIAM3517__AITC.cpp
 *
 *	Author : Nived.Sivadas@Samraksh.com
 *
 *  Description : Contains the core interrupt controller implementation
 *
 */

 #include <tinyhal.h>
 #include "..\TIAM3517.h"

#define DEFINE_IRQ(index, priority) { priority, { NULL, (void*)(size_t)index } }

extern void TIMER_2_ISR(void *Param);
extern void TIMER_1_ISR(void *Param);
extern void TIMER_0_ISR(void *Param);

extern "C"
{
	void myIRQ_Handler();
}

void enableINTC()
{
	asm(
"mrs r0, cpsr \n"
"bic r1, r0,#0x80 \n"
"msr  cpsr, r1 \n"
"mov  pc, lr \n"
);

}



TIAM3517_AITC_Driver::IRQ_VECTORING TIAM3517_AITC_Driver::s_IsrTable[] = {

	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_EMUINT , TIAM3517_AITC::c_IRQ_Priority_4),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_COMMTX , TIAM3517_AITC::c_IRQ_Priority_5),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_COMMRX , TIAM3517_AITC::c_IRQ_Priority_6),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_BENCH , TIAM3517_AITC::c_IRQ_Priority_8),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP2_ST_IRQ , TIAM3517_AITC::c_IRQ_Priority_9),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP3_ST_IRQ , TIAM3517_AITC::c_IRQ_Priority_10),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_Reserved0 , TIAM3517_AITC::c_IRQ_Priority_1),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_sysnirq , TIAM3517_AITC::c_IRQ_Priority_2),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_Reserved1 , TIAM3517_AITC::c_IRQ_Priority_3),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SMX_DBG , TIAM3517_AITC::c_IRQ_Priority_7),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SMX_APP , TIAM3517_AITC::c_IRQ_Priority_11),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_PRCM_MPU , TIAM3517_AITC::c_IRQ_Priority_12),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SDMA_0 , TIAM3517_AITC::c_IRQ_Priority_13),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SDMA_1 , TIAM3517_AITC::c_IRQ_Priority_14),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SDMA_2 , TIAM3517_AITC::c_IRQ_Priority_15),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SDMA_3 , TIAM3517_AITC::c_IRQ_Priority_16),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP1 , TIAM3517_AITC::c_IRQ_Priority_17),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP2 , TIAM3517_AITC::c_IRQ_Priority_18),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SR1 , TIAM3517_AITC::c_IRQ_Priority_19),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_Reserved2 , TIAM3517_AITC::c_IRQ_Priority_20),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPMC , TIAM3517_AITC::c_IRQ_Priority_21),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SGX , TIAM3517_AITC::c_IRQ_Priority_22),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP3 , TIAM3517_AITC::c_IRQ_Priority_23),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP4 , TIAM3517_AITC::c_IRQ_Priority_24),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_HECCOINT , TIAM3517_AITC::c_IRQ_Priority_25),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_DSS_IRQ , TIAM3517_AITC::c_IRQ_Priority_26),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_Reserved3 , TIAM3517_AITC::c_IRQ_Priority_27),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP5 , TIAM3517_AITC::c_IRQ_Priority_28),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_HECC1INT , TIAM3517_AITC::c_IRQ_Priority_29),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPIO1_MPU , TIAM3517_AITC::c_IRQ_Priority_30),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPIO2_MPU , TIAM3517_AITC::c_IRQ_Priority_31),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPIO3_MPU , TIAM3517_AITC::c_IRQ_Priority_32),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPIO4_MPU , TIAM3517_AITC::c_IRQ_Priority_33),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPIO5_MPU , TIAM3517_AITC::c_IRQ_Priority_34),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPIO6_MPU , TIAM3517_AITC::c_IRQ_Priority_35),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_Reserved4 , TIAM3517_AITC::c_IRQ_Priority_36),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_WDT3 , TIAM3517_AITC::c_IRQ_Priority_37),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT1 , TIAM3517_AITC::c_IRQ_Priority_38),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT2 , TIAM3517_AITC::c_IRQ_Priority_42),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT3 , TIAM3517_AITC::c_IRQ_Priority_41),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT4 , TIAM3517_AITC::c_IRQ_Priority_40),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT5 , TIAM3517_AITC::c_IRQ_Priority_42),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT6 , TIAM3517_AITC::c_IRQ_Priority_43),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT7 , TIAM3517_AITC::c_IRQ_Priority_44),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT8 , TIAM3517_AITC::c_IRQ_Priority_45),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT9 , TIAM3517_AITC::c_IRQ_Priority_46),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT10 , TIAM3517_AITC::c_IRQ_Priority_47),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT11 , TIAM3517_AITC::c_IRQ_Priority_48),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SPI4 , TIAM3517_AITC::c_IRQ_Priority_49),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SHA1MD5 , TIAM3517_AITC::c_IRQ_Priority_50),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_FPKA_IRQREADY_N , TIAM3517_AITC::c_IRQ_Priority_51),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SHA2MD5 , TIAM3517_AITC::c_IRQ_Priority_52),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_RNG , TIAM3517_AITC::c_IRQ_Priority_53),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_EMIF4ERR , TIAM3517_AITC::c_IRQ_Priority_54),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP4_TX , TIAM3517_AITC::c_IRQ_Priority_55),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP4_RX , TIAM3517_AITC::c_IRQ_Priority_56),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_I2C1 , TIAM3517_AITC::c_IRQ_Priority_57),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_I2C2 , TIAM3517_AITC::c_IRQ_Priority_58),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_HDQ , TIAM3517_AITC::c_IRQ_Priority_59),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP1_TX , TIAM3517_AITC::c_IRQ_Priority_60),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP1_RX , TIAM3517_AITC::c_IRQ_Priority_61),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_I2C3 , TIAM3517_AITC::c_IRQ_Priority_62),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP2_IRQ_TX ,TIAM3517_AITC::c_IRQ_Priority_63),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP2_IRQ_RX , TIAM3517_AITC::c_IRQ_Priority_64),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_FPKA_IRQERROR_N , TIAM3517_AITC::c_IRQ_Priority_65),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SP1 , TIAM3517_AITC::c_IRQ_Priority_66),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SP2 , TIAM3517_AITC::c_IRQ_Priority_67),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_CO_RX_THRESH_PULSE , TIAM3517_AITC::c_IRQ_Priority_68),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_CO_RX_PULSE , TIAM3517_AITC::c_IRQ_Priority_69),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_CO_TX_PULSE , TIAM3517_AITC::c_IRQ_Priority_70),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_CO_MISC_PULSE , TIAM3517_AITC::c_IRQ_Priority_71),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_USBOTG_INPUT , TIAM3517_AITC::c_IRQ_Priority_72),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_UART1 , TIAM3517_AITC::c_IRQ_Priority_73),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_UART2 , TIAM3517_AITC::c_IRQ_Priority_74),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_UART3 , TIAM3517_AITC::c_IRQ_Priority_75),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_PBIAS , TIAM3517_AITC::c_IRQ_Priority_76),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_OHCI , TIAM3517_AITC::c_IRQ_Priority_77),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_EHCI , TIAM3517_AITC::c_IRQ_Priority_78),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_TLL , TIAM3517_AITC::c_IRQ_Priority_79),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_PARTHASH , TIAM3517_AITC::c_IRQ_Priority_80),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_Reserved5 , TIAM3517_AITC::c_IRQ_Priority_81),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP5_TX , TIAM3517_AITC::c_IRQ_Priority_82),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP5_RX , TIAM3517_AITC::c_IRQ_Priority_83),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MMC1 , TIAM3517_AITC::c_IRQ_Priority_84),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_UART4 , TIAM3517_AITC::c_IRQ_Priority_85),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_Reserved6 , TIAM3517_AITC::c_IRQ_Priority_86),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MMC2 , TIAM3517_AITC::c_IRQ_Priority_87),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MPU_ICR , TIAM3517_AITC::c_IRQ_Priority_88),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_CCDC_VD0_INT , TIAM3517_AITC::c_IRQ_Priority_89),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP3_TX , TIAM3517_AITC::c_IRQ_Priority_90),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MCBSP3_RX , TIAM3517_AITC::c_IRQ_Priority_91),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_SPI3 , TIAM3517_AITC::c_IRQ_Priority_92),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_CCDC_VD1 , TIAM3517_AITC::c_IRQ_Priority_93),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_CCDC_VD2 , TIAM3517_AITC::c_IRQ_Priority_94),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_MMC3 , TIAM3517_AITC::c_IRQ_Priority_95),
	DEFINE_IRQ(TIAM3517_AITC::c_IRQ_INDEX_GPT12 , TIAM3517_AITC::c_IRQ_Priority_96),
	DEFINE_IRQ(TIAM3517_AITC_Driver::c_VECTORING_GUARD, TIAM3517_AITC::c_IRQ_Priority_97),


 };

int disable_interrupts (void)
{
        unsigned long old,temp;
        __asm__ __volatile__("mrs %0, cpsr\n"
                             "orr %1, %0, #0xc0\n"
                             "msr cpsr_c, %1"
                             : "=r" (old), "=r" (temp)
                             :
                             : "memory");
        return (old & 0x80) == 0;
}

void enable_interrupts (void)
{
        unsigned long temp;
        __asm__ __volatile__("mrs %0, cpsr\n"
                             "bic %0, %0, #0x80\n"
                             "msr cpsr_c, %0"
                             : "=r" (temp)
                             :
                             : "memory");
}

void data_synchronization_barrier(void)
{
	__asm__ __volatile__("mov r0, #0\n"
						 "mcr p15, #0, r0, c7, c10, #4\n");
}

void restore_critical_context(void)
{
	__asm__ __volatile__("msr spsr, r11\n"
						 "ldmfd sp!, {r0 - r12, lr}\n"
						 "subs pc, lr, #4\n");
}

void TIAM3517_AITC_Driver::Initialize()
{
#ifdef _DEBUG_NESTED_INTERRUPTS
	CPU_GPIO_EnableOutputPin(152, FALSE);
#endif

	disable_interrupts();

	enable_interrupts();

	TIAM3517_CLOCK_MANAGER_MPU_CM &MPUCM = TIAM3517::CMGRMPUCM();

	UINT32 tmp = 0;
	// Enabling clock to the intc dpll1_fclk
//	MPUCM.CM_CLKEN_PLL_MPU |= 7; // Shouldn't be necessary -- NPS

//	enableINTC();

	TIAM3517_AITC& AITC = TIAM3517::AITC();


	tmp = AITC.padding0;
	// Resetting the INTC module and setting AUTOIDLE or automatic gating strategy for interface clk
	//AITC.INTCPS_SYSCONFIG |= 2;
	//AITC.INTCPS_SYSCONFIG |= 1;
	tmp = AITC.INTCPS_SYSCONFIG;
	tmp |= 1 << 1;

	AITC.INTCPS_SYSCONFIG = tmp;

	tmp = AITC.INTCPS_IDLE;

	tmp |= 1;

	AITC.INTCPS_IDLE = tmp;
	// Waiting for module reset to complete
	while(!(AITC.INTCPS_SYSSTATUS & 0x1));




	AITC.Reg[0].INTCPS_MIR_SET |= 2;

	// Resetting FIQ output and IRQ generation
	AITC.INTCPS_CONTROL |= 3;





	IRQ_VECTORING* IsrVector = s_IsrTable;


	for(UINT32 index = 0 ; index <= c_VECTORING_GUARD ; index++)
	{
		AITC.SetPriority(index,IsrVector[index].Priority);
		IsrVector->Handler.Initialize(STUB_ISRVector, (void *)(size_t) IsrVector->Priority);
	}




}


BOOL TIAM3517_AITC_Driver::ActivateInterrupt(UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param)
{
	IRQ_VECTORING* Isr_Vector = IRQToIRQVector(Irq_Index); if(!Isr_Vector) return FALSE;

	{
		GLOBAL_LOCK(irq);

		TIAM3517_AITC& AITC = TIAM3517::AITC();

		AITC.DisableInterrupt(Irq_Index);

		AITC.SetType( Irq_Index, Fast );

		Isr_Vector->Handler.Initialize( ISR, ISR_Param );

		AITC.EnableInterrupt(Irq_Index);

	}

	return TRUE;

}

BOOL TIAM3517_AITC_Driver::DeactivateInterrupt(UINT32 Irq_Index)
{

	IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;

	{
		GLOBAL_LOCK(irq);

		TIAM3517_AITC& AITC = TIAM3517::AITC();

		AITC.DisableInterrupt(Irq_Index);

		IsrVector->Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector->Priority );
	}

	return TRUE;
}

extern "C"
{

void IRQ_Handler() {

}

void myIRQ_Handler()
{
#ifdef _DEBUG_NESTED_INTERRUPTS
	CPU_GPIO_SetPinState(152, FALSE);
#endif

	//UINT32 temp_assm;

	UINT32 index;

	TIAM3517_AITC& AITC = TIAM3517::AITC();

#ifdef _ENABLE_NESTED_INTERRUPTS
	volatile UINT32 OLD_PRIORITY_THRESHOLD = AITC.INTCPS_THRESHOLD;
#endif

	// set before jumping elsewhere or allowing other interrupts
	//SystemState_SetNoLock( SYSTEM_STATE_ISR              );
	//SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

	if((index = AITC.NormalInterruptPending()) <= AITC.c_Max_InterruptIndex)
	{
		TIAM3517_AITC_Driver::IRQ_VECTORING* IsrVector = &TIAM3517_AITC_Driver::s_IsrTable[ index ];

#ifdef _ENABLE_NESTED_INTERRUPTS
		volatile UINT32 temp = AITC.INTCPS_IRQ_PRIORITY;
		AITC.INTCPS_THRESHOLD = AITC.INTCPS_IRQ_PRIORITY;
		temp = AITC.INTCPS_THRESHOLD;
		AITC.INTCPS_CONTROL = 1;
		data_synchronization_barrier();
		enable_interrupts();

#endif



	    // In case the interrupt was forced, remove the flag.
	    //AITC.RemoveForcedInterrupt( index );

	    IsrVector->Handler.Execute();


#ifndef _ENABLE_NESTED_INTERRUPTS
	    AITC.INTCPS_CONTROL = 1;
#endif

#ifdef _ENABLE_NESTED_INTERRUPTS
	    disable_interrupts();
	    AITC.INTCPS_THRESHOLD = OLD_PRIORITY_THRESHOLD;
	    temp = AITC.INTCPS_THRESHOLD;
	    temp++;
#endif

	    // Resetting the NEWIRQAGR bit to enable processing of subsequent pending IRQs

	}


	//SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
	//SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
#ifdef _DEBUG_NESTED_INTERRUPTS
	//DoNothing();
	CPU_GPIO_SetPinState(152, TRUE);
#endif
	//restore_critical_context();
	//enable_interrupts();
	//__asm__ __volatile__("mov pc,lr\n");
	//__asm__ __volatile__("add lr,lr, #4\n");
	//__asm__ __volatile__("b IRQ_SubHandlerReturn\n");
	//enable_interrupts();
}

}

extern "C"
{
void __irq FIQ_Handler()
{
		UINT32 index;

		TIAM3517_AITC& AITC = TIAM3517::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

		// protect from spurious interrupts
		if((index = AITC.FastInterruptPending()) <= AITC.c_Max_InterruptIndex)
		{
		    TIAM3517_AITC_Driver::IRQ_VECTORING* IsrVector = &TIAM3517_AITC_Driver::s_IsrTable[ index ];

		    // In case the interrupt was forced, remove the flag.
		    AITC.RemoveForcedInterrupt( index );

		    IsrVector->Handler.Execute();

			// Resetting the NEWIRQAGR bit to enable processing of subsequent pending FIQs
			AITC.INTCPS_CONTROL = (1 << 1);

		}

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
}

void __irq NotUsed_Handler()
{
	while(1);
}

}

BOOL TIAM3517_AITC_Driver::InterruptEnable(UINT32 Irq_Index)
{
	if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;

	TIAM3517_AITC& AITC = TIAM3517::AITC();

	GLOBAL_LOCK(irq);

	BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

	AITC.EnableInterrupt(Irq_Index);

	return WasEnabled;


}

BOOL TIAM3517_AITC_Driver::InterruptDisable(UINT32 Irq_Index)
{
	if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;

	TIAM3517_AITC& AITC = TIAM3517::AITC();

	GLOBAL_LOCK(irq);

	BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

	AITC.DisableInterrupt(Irq_Index);

	return WasEnabled;

}


BOOL TIAM3517_AITC_Driver::InterruptEnableState( UINT32 Irq_Index )
{
   	 TIAM3517_AITC& AITC = TIAM3517::AITC();

        return AITC.IsInterruptEnabled( Irq_Index );


}


BOOL  TIAM3517_AITC_Driver::InterruptState( UINT32 Irq_Index )
{
	TIAM3517_AITC& AITC = TIAM3517::AITC();

	return AITC.GetInterruptState( Irq_Index );


}

TIAM3517_AITC_Driver::IRQ_VECTORING* TIAM3517_AITC_Driver::IRQToIRQVector( UINT32 IRQ )
{
	 IRQ_VECTORING* IsrVector = s_IsrTable;



	 if (IRQ < c_VECTORING_GUARD)
	 {
	     return &IsrVector[IRQ];

	 }

	 return NULL;

}

void TIAM3517_AITC_Driver::STUB_ISRVector( void* Param )
{
	while(1);
}

