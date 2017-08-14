////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Samraksh Company
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
//#include <mxc_config.h>
#include <string.h>
//#include <nvic_table.h>
//#include "Max326_Intc.h"

/*
void MAX326_AITC_Driver::Initialize()
{
}

BOOL MAX326_AITC_Driver::ActivateInterrupt( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param)
{
    return TRUE;
}

BOOL MAX326_AITC_Driver::DeactivateInterrupt( UINT32 Irq_Index )
{
    return TRUE;
}


BOOL MAX326_AITC_Driver::InterruptEnable( UINT32 Irq_Index  )
{

    if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;

    MAX326_AITC& AITC = MAX326::AITC();

    GLOBAL_LOCK(irq);

    BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

    AITC.EnableInterrupt(Irq_Index);

    return WasEnabled;
}


BOOL MAX326_AITC_Driver::InterruptDisable( UINT32 Irq_Index )
{
    if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;

    MAX326_AITC& AITC = MAX326::AITC();

    GLOBAL_LOCK(irq);

    BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

    AITC.DisableInterrupt(Irq_Index);

    return WasEnabled;
}


BOOL MAX326_AITC_Driver::InterruptEnableState( UINT32 Irq_Index )
{
    MAX326_AITC& AITC = MAX326::AITC();

    return AITC.IsInterruptEnabled( Irq_Index );
}


BOOL  MAX326_AITC_Driver::InterruptState( UINT32 Irq_Index )
{
    MAX326_AITC& AITC = MAX326::AITC();

    return AITC.GetInterruptState( Irq_Index );
}

MAX326_AITC_Driver::IRQ_VECTORING* MAX326_AITC_Driver::IRQToIRQVector( UINT32 IRQ )
{
    IRQ_VECTORING* IsrVector = s_IsrTable;



    if (IRQ < c_VECTORING_GUARD)
    {
        return &IsrVector[IRQ];

    }

    return NULL;
}

void MAX326_AITC_Driver::STUB_ISRVector( void* Param )
{
    lcd_printf( "\fSTUB_ISR %08x\r\n", (size_t)Param );
	//Nived :  Calling Default Interrupt Handler
	hal_printf(" Calling Default Interrupt Handler \n");
    HARD_BREAKPOINT();

}

*/

struct IRQ_VECTORING
{
	UINT32       Priority;
	HAL_CALLBACK Handler;
};

static IRQ_VECTORING s_IsrTable[64];

//__isr_vector[64];


extern "C"
{


void HardFault_Display()
{
}

//__attribute__((optimize("O0")))
void HardFault_HandlerC(unsigned long *hardfault_args)
{
	while (1){}
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


void __irq SVC_Handler(){}

void __irq PendSV_Handler(){}

// Stub here to save potential confusion later.
void __irq SysTick_Handler()
{
	//HARD_BREAKPOINT();
}

void __irq Default_Handler()
{
	//HARD_BREAKPOINT();
}

void __irq PVD_IRQHandler(){}

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

