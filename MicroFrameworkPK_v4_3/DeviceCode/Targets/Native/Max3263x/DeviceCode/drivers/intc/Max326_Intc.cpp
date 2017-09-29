////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Samraksh Company
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <mxc_config.h>
#include <string.h>
//#include <nvic_table.h>
#include "Max326_Intc.h"

////////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS
#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

////////////////////////////////////////////////////////////////////////////////


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "s_IsrTable_MAX326"
#endif

#define DEFINE_IRQ(index, priority) { priority, { NULL, (void*)(size_t)index } }

#ifdef DEBUG_MAX326_ISR
int interrupt_count[MXC_IRQ_COUNT];
#endif


IRQ_VECTORING __section(rwdata) MAX326_AITC_Driver::s_IsrTable[MXC_IRQ_COUNT] =
{
	DEFINE_IRQ(NonMaskableInt_IRQn, MAX326_AITC::c_IRQ_Priority_5  ), //    = -14, ARM Core : Non-maskable IRQ
	DEFINE_IRQ(HardFault_IRQn , MAX326_AITC::c_IRQ_Priority_5  ), //< ARM Core : Hard Fault IRQ
	DEFINE_IRQ(MemoryManagement_IRQn , MAX326_AITC::c_IRQ_Priority_5  ), //< ARM Core : Memory Management IRQ
	DEFINE_IRQ(BusFault_IRQn , MAX326_AITC::c_IRQ_Priority_5  ), //< ARM Core : Bus Fault IRQ
	DEFINE_IRQ(UsageFault_IRQn , MAX326_AITC::c_IRQ_Priority_5  ), //< ARM Core : Usage Fault IRQ
	DEFINE_IRQ(SVCall_IRQn , MAX326_AITC::c_IRQ_Priority_5  ),  //< ARM Core : SVCall IRQ
	DEFINE_IRQ(DebugMonitor_IRQn , MAX326_AITC::c_IRQ_Priority_5  ),  //< ARM Core : Debug Monitor IRQ
	DEFINE_IRQ(PendSV_IRQn , MAX326_AITC::c_IRQ_Priority_5  ),  //< ARM Core : PendSV IRQ
	DEFINE_IRQ(SysTick_IRQn , MAX326_AITC::c_IRQ_Priority_5  ),  //< ARM Core : SysTick IRQ
	DEFINE_IRQ(CLKMAN_IRQn , MAX326_AITC::c_IRQ_Priority_5  ),          //< CLKMAN
	DEFINE_IRQ(	    PWRMAN_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< PWRMAN
	DEFINE_IRQ(	    FLC_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                     //< Flash Controller
	DEFINE_IRQ(	    RTC0_IRQn, MAX326_AITC::c_IRQ_Priority_0  ),                    //< RTC Counter match with Compare 0
	DEFINE_IRQ(	    RTC1_IRQn, MAX326_AITC::c_IRQ_Priority_0  ),                    //< RTC Counter match with Compare 1
	DEFINE_IRQ(	    RTC2_IRQn, MAX326_AITC::c_IRQ_Priority_0  ),                    //< RTC Prescaler interval compare match
	DEFINE_IRQ(	    RTC3_IRQn, MAX326_AITC::c_IRQ_Priority_0  ),                    //< RTC Overflow
	DEFINE_IRQ(	    PMU_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                     //< Peripheral Management Unit (PMU/DMA)
	DEFINE_IRQ(	    USB_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                     //< USB
	DEFINE_IRQ(	    AES_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                     //< AES
	DEFINE_IRQ(	    MAA_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                     //< MAA
	DEFINE_IRQ(	    WDT0_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                    //< Watchdog 0 timeout
	DEFINE_IRQ(	    WDT0_P_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Watchdog 0 pre-window (fed too early)
	DEFINE_IRQ(	    WDT1_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                    //< Watchdog 1 timeout
	DEFINE_IRQ(	    WDT1_P_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Watchdog 1 pre-window (fed too early)
	DEFINE_IRQ(	    GPIO_P0_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                 //< GPIO Port 0
	DEFINE_IRQ(	    GPIO_P1_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                 //< GPIO Port 1
	DEFINE_IRQ(		GPIO_P2_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                 //< GPIO Port 2
	DEFINE_IRQ(	    GPIO_P3_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                 //< GPIO Port 3
	DEFINE_IRQ(	    GPIO_P4_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                 //< GPIO Port 4
	DEFINE_IRQ(	    GPIO_P5_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                 //< GPIO Port 5
	DEFINE_IRQ(	    GPIO_P6_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                 //< GPIO Port 6
	DEFINE_IRQ(	    TMR0_0_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 0 (32-bit, 16-bit #0)
	DEFINE_IRQ(	    TMR0_1_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 0 (16-bit #1)
	DEFINE_IRQ(	    TMR1_0_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 1 (32-bit, 16-bit #0)
	DEFINE_IRQ(	    TMR1_1_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 1 (16-bit #1)
	DEFINE_IRQ(	    TMR2_0_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 2 (32-bit, 16-bit #0)
	DEFINE_IRQ(	    TMR2_1_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 2 (16-bit #1)
	DEFINE_IRQ(	    TMR3_0_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 3 (32-bit, 16-bit #0)
	DEFINE_IRQ(	    TMR3_1_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 3 (16-bit #1)
	DEFINE_IRQ(	    TMR4_0_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 4 (32-bit, 16-bit #0)
	DEFINE_IRQ(	    TMR4_1_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 4 (16-bit #1)
	DEFINE_IRQ(	    TMR5_0_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 5 (32-bit, 16-bit #0)
	DEFINE_IRQ(	    TMR5_1_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                  //< Timer 5 (16-bit #1)
	DEFINE_IRQ(	    UART0_IRQn, MAX326_AITC::c_IRQ_Priority_1  ),                   //< UART 0
	DEFINE_IRQ(	    UART1_IRQn, MAX326_AITC::c_IRQ_Priority_1  ),                   //< UART 1
	DEFINE_IRQ(	    UART2_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                   //< UART 2
	DEFINE_IRQ(	    UART3_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                   //< UART 3
	DEFINE_IRQ(	    PT_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                      //< Pulse Trains
	DEFINE_IRQ(	    I2CM0_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                   //< I2C Master 0
	DEFINE_IRQ(	    I2CM1_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                   //< I2C Master 1
	DEFINE_IRQ(	    I2CM2_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                   //< I2C Master 2
	DEFINE_IRQ(	    I2CS_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                    //< I2C Slave
	DEFINE_IRQ(	    SPIM0_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                   //< SPI Master 0
	DEFINE_IRQ(	    SPIM1_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                   //< SPI Master 1
	DEFINE_IRQ(	    SPIM2_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                   //< SPI Master 2
	DEFINE_IRQ(	    SPIB_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                    //< SPI Bridge
	DEFINE_IRQ(	    OWM_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                     //< 1-Wire Master
	DEFINE_IRQ(	    AFE_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                     //< ADC
	DEFINE_IRQ(	    SPIS_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                    //< SPI Slave
	DEFINE_IRQ(	    GPIO_P7_IRQn, MAX326_AITC::c_IRQ_Priority_5  ),                 //< GPIO Port 7
	DEFINE_IRQ(	    GPIO_P8_IRQn,  MAX326_AITC::c_IRQ_Priority_5  ),               //< GPIO Port 8
};

#undef DEFINE_IRQ

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif

////////////////////////////////////////////////////////////////////////////////

void MAX326_AITC_Driver::Initialize()
{

	for(INT32 i = MAX326_AITC::c_MinInterruptIndex ; i <= MAX326_AITC::c_MaxInterruptIndex;i++)
	{
		MAX326_AITC::ClearInterrupt((UINT32)i);
	}

    // // set all priorities to the defaults specified
     IRQ_VECTORING* IsrVector = s_IsrTable;

#ifdef DEBUG_MAX326_ISR
	memset(interrupt_count, 0, sizeof(int)*MXC_IRQ_COUNT);
#endif

    // // set the priority level for each IRQ and stub the IRQ callback
     for (int i=0; i<c_VECTORING_GUARD;i++)
     {
         MAX326_AITC::SetPriority(i, IsrVector[i].Priority );
         IsrVector->Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector->Priority);
     }

}

BOOL MAX326_AITC_Driver::ActivateInterrupt( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param)
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;
    {
        GLOBAL_LOCK(irq);
        // disable this interrupt while we change it
        MAX326_AITC::DisableInterrupt(Irq_Index);

        // set the vector
        IsrVector->Handler.Initialize( ISR, ISR_Param );
        	// enable the interrupt if we have a vector
        MAX326_AITC::EnableInterrupt(Irq_Index);
    }
    return TRUE;
}

BOOL MAX326_AITC_Driver::DeactivateInterrupt( UINT32 Irq_Index )
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;
    {
        GLOBAL_LOCK(irq);
        //MAX326_AITC& AITC = MAX326::AITC();
        // disable this interrupt while we change it
        MAX326_AITC::DisableInterrupt(Irq_Index);
        // as it is stub, just put the Priority to the ISR parameter
        IsrVector->Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector->Priority );
    }
    return TRUE;
}

BOOL MAX326_AITC_Driver::InterruptEnable( UINT32 Irq_Index  )
{
    if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;
    //MAX326_AITC& AITC = MAX326::AITC();
    GLOBAL_LOCK(irq);
    BOOL WasEnabled = MAX326_AITC::IsInterruptEnabled( Irq_Index );
    MAX326_AITC::EnableInterrupt(Irq_Index);
    return WasEnabled;
}

BOOL MAX326_AITC_Driver::InterruptDisable( UINT32 Irq_Index )
{
    if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;
    //MAX326_AITC& AITC = MAX326::AITC();
    GLOBAL_LOCK(irq);
    BOOL WasEnabled = MAX326_AITC::IsInterruptEnabled( Irq_Index );
    MAX326_AITC::DisableInterrupt(Irq_Index);
    return WasEnabled;
}

BOOL MAX326_AITC_Driver::InterruptEnableState( UINT32 Irq_Index )
{
    //MAX326_AITC& AITC = MAX326::AITC();
    return MAX326_AITC::IsInterruptEnabled( Irq_Index );
}

BOOL  MAX326_AITC_Driver::InterruptState( UINT32 Irq_Index )
{
    //MAX326_AITC& AITC = MAX326::AITC();
    return MAX326_AITC::GetInterruptState( Irq_Index );
}

IRQ_VECTORING* MAX326_AITC_Driver::IRQToIRQVector( UINT32 IRQn )
{
    IRQ_VECTORING* IsrVector = s_IsrTable;
    INT32 irq = (INT32)IRQn;
    irq = irq + MAX326_AITC::c_IRQToIndexOffset;
    if (irq < c_VECTORING_GUARD) { return &IsrVector[irq]; }
    return NULL;
}

void MAX326_AITC_Driver::STUB_ISRVector( void* Param )
{
    lcd_printf( "\fSTUB_ISR %08x\r\n", (size_t)Param );
	//Nived :  Calling Default Interrupt Handler
	hal_printf(" Calling Default Interrupt Handler \n");
    HARD_BREAKPOINT();
}




/*
extern "C"
{

	
	void __irq TAMPER_IRQHandler()
	{

		//MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TAMPER]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TAMPER];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq RTC_IRQHandler()
	{
		UINT32 index;

		//MAX326_AITC& AITC = MAX326::AITC();

		GLOBAL_LOCK(irq);
		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		irq.Release();

#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_RTC]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_RTC];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		irq.Acquire();
		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq FLASH_IRQHandler()
	{

		//MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_FLASH]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_FLASH];

		// In case the interrupt was forced, remove the flag.
		MAX326_AITC::RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq RCC_IRQHandler()
	{

		//MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_RCC]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_RCC];

		// In case the interrupt was forced, remove the flag.
		MAX326_AITC::RemoveForcedInterrupt( 0 );

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq EXTI1_IRQHandler()
	{
		UINT32 index;

		//MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		GLOBAL_LOCK(irq);
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		irq.Release();
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_EXTI1]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_EXTI1];

		IsrVector->Handler.Execute();

		irq.Acquire();
		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	

	void __irq EXTI2_IRQHandler()
	{
		UINT32 index;

		//MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_EXTI2]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_EXTI2];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq EXTI3_IRQHandler()
	{
		UINT32 index;

		//MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_EXTI3]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_EXTI3];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq EXTI4_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_EXTI4]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_EXTI4];

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq DMA1_Channel1_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		GLOBAL_LOCK(irq);
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		irq.Release();
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL1]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA_CHANNEL1];

		IsrVector->Handler.Execute();

		irq.Acquire();
		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		irq.Release();
	}
	
	void __irq DMA1_Channel2_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL2]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA_CHANNEL2];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA1_Channel3_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL3]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA_CHANNEL3];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
		
		void __irq DMA1_Channel4_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL4]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA_CHANNEL4];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq DMA1_Channel5_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL5]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA_CHANNEL5];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA1_Channel6_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL6]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA_CHANNEL6];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
		void __irq DMA1_Channel7_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA_CHANNEL7]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA_CHANNEL7];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq ADC1_2_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_ADC1_2]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_ADC1_2];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq USB_HP_CAN1_TX_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_USB_HP_CAN_TX]++;
#endif

		//MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[18];
		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_USB_HP_CAN_TX];
		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
		void __irq USB_LP_CAN1_RX0_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();
		//LED_GREEN();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_USB_LP_CAN_RX0]++;
#endif

		//MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[19];
		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_USB_LP_CAN_RX0];


		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq CAN1_RX1_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_CAN_RX1]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_CAN_RX1];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq CAN1_SCE_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_CAN_SCE]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_CAN_SCE];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq EXTI9_5_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		GLOBAL_LOCK(irq);
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		// do not release irq. *SPOT_Hardware_Port::IsrProcedure() wants IRQ off.
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_EXTI9_5]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_EXTI9_5];

		IsrVector->Handler.Execute();

		irq.Acquire();
		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM1_BRK_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM1_BRK_TIM9]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM1_BRK_TIM9];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM1_UP_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM1_UP_TIM10]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM1_UP_TIM10];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM1_TRG_COM_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM1_TRG_COM_TIM11]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM1_TRG_COM_TIM11];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM1_CC_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();
		GLOBAL_LOCK(irq);
		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM1_CC]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM1_CC];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	

	void __irq TIM3_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM3]++;
#endif

			//MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[28];
			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM3];

			// In case the interrupt was forced, remove the flag.
			IsrVector->Handler.Execute();
			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}


	void __irq TIM4_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM4]++;
#endif

			//MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[29];
			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM4];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}

	

	void __irq I2C1_EV_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			GLOBAL_LOCK(irq);
			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
			irq.Release();
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_I2C1_EV]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_I2C1_EV];

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

			MAX326_AITC& AITC = MAX326::AITC();

			GLOBAL_LOCK(irq);
			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
			irq.Release();
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_I2C1_ER]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_I2C1_ER];

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

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_I2C2_EV]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_I2C2_EV];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
	void __irq I2C2_ER_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_I2C2_ER]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_I2C2_ER];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
	
		void __irq SPI1_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_SPI1]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_SPI1];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq SPI2_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_SPI2]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_SPI2];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
#if !defined(PLATFORM_ARM_EmoteDotNow) && !defined(PLATFORM_ARM_WLN)
		void __irq USART1_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_USART1]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_USART1];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
#endif
		
		void __irq USART2_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_USART2]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_USART2];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq USART3_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_USART3]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_USART3];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq EXTI15_10_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();
			GLOBAL_LOCK(irq);
			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
			irq.Release();
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_EXTI15_10]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_EXTI15_10];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();
			irq.Acquire();
			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}

		void __irq RTCAlarm_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_RTCAlarm]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_RTCAlarm];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}

		void __irq USBWakeUp_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_USBWakeUp]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_USBWakeUp];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq TIM8_BRK_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM8_BRK_TIM12]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM8_BRK_TIM12];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq USART4_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_USART4]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_USART4];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq USART5_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_USART5]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_USART5];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq TIM8_CC_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM8_CC]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM8_CC];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		
		void __irq ADC3_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_ADC3]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_ADC3];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}
		

		void __irq FSMC_IRQHandler()
		{

			UINT32 index;

			MAX326_AITC& AITC = MAX326::AITC();

			// set before jumping elsewhere or allowing other interrupts
			SystemState_SetNoLock( SYSTEM_STATE_ISR              );
			SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_FSMC]++;
#endif

			MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_FSMC];

			// In case the interrupt was forced, remove the flag.

			IsrVector->Handler.Execute();

			SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
			SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
		}


		

		void __irq SDIO_IRQHandler()
	{

		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_SDIO]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_SDIO];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM5_IRQHandler()
	{

		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM5]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM5];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq SPI3_IRQHandler()
	{

		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_SPI3]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_SPI3];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	

	void __irq TIM6_IRQHandler()
	{

		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM6]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM6];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq TIM7_IRQHandler()
	{

		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM7]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM7];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA2_Channel1_IRQHandler()
	{

		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA2_Channel1]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA2_Channel1];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA2_Channel2_IRQHandler()
	{

		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA2_Channel2]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA2_Channel2];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA2_Channel3_IRQHandler()
	{

		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA2_Channel3]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA2_Channel3];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}
	
	void __irq DMA2_Channel4_5_IRQHandler()
	{

		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_DMA2_Channel4_5]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_DMA2_Channel4_5];

		// In case the interrupt was forced, remove the flag.

		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}



	void __irq EXTI0_IRQHandler()
	{
		UINT32 index;

		MAX326_AITC& AITC = MAX326::AITC();

		// set before jumping elsewhere or allowing other interrupts
		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_EXTI0]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_EXTI0];

		IsrVector->Handler.Execute();


		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

	void __irq TIM2_IRQHandler()
	{
		UINT32 index;
		MAX326_AITC& AITC = MAX326::AITC();
		GLOBAL_LOCK(irq);

		SystemState_SetNoLock( SYSTEM_STATE_ISR              );
		SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
		
#ifdef DEBUG_MAX326_ISR
		interrupt_count[c_IRQ_INDEX_TIM2]++;
#endif

		MAX326_AITC_Driver::IRQ_VECTORING* IsrVector = &MAX326_AITC_Driver::s_IsrTable[MAX326_AITC::c_IRQ_INDEX_TIM2];
		IsrVector->Handler.Execute();

		SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
		SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
	}

}
*/


