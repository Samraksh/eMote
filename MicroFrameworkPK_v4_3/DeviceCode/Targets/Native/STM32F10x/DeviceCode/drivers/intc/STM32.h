/////////////////////////////////////////////////////////////////
// The Samraksh Company
//  Author      : Nived Sivadas
//  Description : Equivalent of stm32f10x.h
/////////////////////////////////////////////////////////////////

#ifndef _MYSTM32_HEADER_H_
#define _MYSTM32_HEADER_H_ 1

#include <cores\arm\include\cpu.h>
#include <stm32f10x.h>
// TODO: Namespace collision, both tinyhal.h (via cpu.h) and stm32f10x.h have FLASH defined
// Actually tinyhal.h only uses FLASH, for the life of me I can't find where defined.

#define BASE                  ((UINT32)0x40000000)
//0x40010400
//#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
//#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400)
#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)
#define SCS_BASE            (0xE000E000)                              /*!< System Control Space Base Address */

#ifndef __NVIC_PRIO_BITS
  #define __NVIC_PRIO_BITS    4               /*!< standard definition for NVIC Priority Bits */
#endif

#define NVIC_PriorityGroup_0         ((uint32_t)0x700) /*!< 0 bits for pre-emption priority */

#define NVIC_PriorityGroup_1         ((uint32_t)0x600) /*!< 1 bits for pre-emption priority
                                                            3 bits for subpriority */
#define NVIC_PriorityGroup_2         ((uint32_t)0x500) /*!< 2 bits for pre-emption priority
                                                            2 bits for subpriority */
#define NVIC_PriorityGroup_3         ((uint32_t)0x400) /*!< 3 bits for pre-emption priority
                                                            1 bits for subpriority */
#define NVIC_PriorityGroup_4         ((uint32_t)0x300) /*!< 4 bits for pre-emption priority
                                                            0 bits for subpriority */


extern "C"
{
void CPWAIT(int reg);
void CPU_CPWAIT();

void BOOT_CPWAIT();
void BOOT_FlushCaches();
}

//
// STM32 driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Interrupt Controller driver
//

struct STM32_AITC_Driver
{
    static const UINT32 c_VECTORING_GUARD = 62;

    //--//

    struct IRQ_VECTORING
    {
        UINT32       Priority;
        HAL_CALLBACK Handler;
    };

    //--//

    static IRQ_VECTORING s_IsrTable[];

    //--//

    static void Initialize();

    static BOOL ActivateInterrupt  ( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param);
    static BOOL DeactivateInterrupt( UINT32 Irq_Index );

    static BOOL InterruptEnable( UINT32 Irq_Index );

    static BOOL InterruptDisable( UINT32 Irq_Index );

    static BOOL InterruptEnableState( UINT32 Irq_Index );

    static BOOL InterruptState( UINT32 Irq_Index );

#if defined(PLATFORM_ARM)
    static void IRQ_Handler();
#endif

    static IRQ_VECTORING* IRQToIRQVector( UINT32 IRQ );

private:
    static void STUB_ISRVector( void* Param );
};

//
// Interrupt Controller driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
struct STM32_AITC
{
	// Base address of the INTC registers

	volatile UINT32 ISER[8];                      /*!< Offset: 0x000  Interrupt Set Enable Register           */
       UINT32 RESERVED0[24];
	volatile UINT32 ICER[8];                      /*!< Offset: 0x080  Interrupt Clear Enable Register         */
       UINT32 RSERVED1[24];
	volatile UINT32 ISPR[8];                      /*!< Offset: 0x100  Interrupt Set Pending Register          */
       UINT32 RESERVED2[24];
	volatile UINT32 ICPR[8];                      /*!< Offset: 0x180  Interrupt Clear Pending Register        */
       UINT32 RESERVED3[24];
	volatile UINT32 IABR[8];                      /*!< Offset: 0x200  Interrupt Active bit Register           */
       UINT32 RESERVED4[56];
	volatile UINT32  IP[240];                      /*!< Offset: 0x300  Interrupt Priority Register (8Bit wide) */
       UINT32 RESERVED5[644];
	volatile  UINT32 STIR;                         /*!< Offset: 0xE00  Software Trigger Interrupt Register     */


	// volatile UINT32 IMR;
	// volatile UINT32 EMR;
	// volatile UINT32 RTSR;
	// volatile UINT32 FTSR;
	// volatile UINT32 SWIER;
	// volatile UINT32 PR;

	static const UINT32 c_Base = (SCS_BASE +  0x0100);

	static const UINT32 c_IRQ_Priority__3  =  -3;
	static const UINT32 c_IRQ_Priority__2  =  -2;
	static const UINT32 c_IRQ_Priority__1  =  -1;
	static const UINT32 c_IRQ_Priority_0  =  0;
	static const UINT32 c_IRQ_Priority_1  =  1;
    static const UINT32 c_IRQ_Priority_2  =  2;
    static const UINT32 c_IRQ_Priority_3  =  3;
    static const UINT32 c_IRQ_Priority_4  =  4;
    static const UINT32 c_IRQ_Priority_5  =  5;
    static const UINT32 c_IRQ_Priority_6  =  6;
    static const UINT32 c_IRQ_Priority_7  =  7;
    static const UINT32 c_IRQ_Priority_8  =  8;
    static const UINT32 c_IRQ_Priority_9  =  9;
    static const UINT32 c_IRQ_Priority_10 = 10;
    static const UINT32 c_IRQ_Priority_11 = 11;
    static const UINT32 c_IRQ_Priority_12 = 12;
    static const UINT32 c_IRQ_Priority_13 = 13;
    static const UINT32 c_IRQ_Priority_14 = 14;
    static const UINT32 c_IRQ_Priority_15 = 15;


	static const UINT32 c_IRQ_INDEX_NonMaskableInt          = -14;
	static const UINT32 c_IRQ_INDEX_MemoryManagementInt          = -12;
	static const UINT32 c_IRQ_INDEX_BusFault          = -11;
	static const UINT32 c_IRQ_INDEX_UsageFault          = -10;
	static const UINT32 c_IRQ_INDEX_DebugMonitor          = -4;
	static const UINT32 c_IRQ_INDEX_SysTick          = -1;
	static const UINT32 c_IRQ_INDEX_WWDG          = 0;
	static const UINT32 c_IRQ_INDEX_PVD          = 1;
    static const UINT32 c_IRQ_INDEX_TAMPER       = 2;
    static const UINT32 c_IRQ_INDEX_RTC     = 3;
    static const UINT32 c_IRQ_INDEX_FLASH     = 4;
    static const UINT32 c_IRQ_INDEX_RCC    = 5;
    static const UINT32 c_IRQ_INDEX_EXTI0   = 6;
    static const UINT32 c_IRQ_INDEX_EXTI1      = 7;
	static const UINT32 c_IRQ_INDEX_EXTI2        = 8;
	static const UINT32 c_IRQ_INDEX_EXTI3        = 9;
	static const UINT32 c_IRQ_INDEX_EXTI4        = 10;
    static const UINT32 c_IRQ_INDEX_DMA_CHANNEL1     = 11;
	static const UINT32 c_IRQ_INDEX_DMA_CHANNEL2       = 12;
	static const UINT32 c_IRQ_INDEX_DMA_CHANNEL3       = 13;
	static const UINT32 c_IRQ_INDEX_DMA_CHANNEL4       = 14;
	static const UINT32 c_IRQ_INDEX_DMA_CHANNEL5      = 15;
	static const UINT32 c_IRQ_INDEX_DMA_CHANNEL6       = 16;
	static const UINT32 c_IRQ_INDEX_DMA_CHANNEL7       = 17;
	static const UINT32 c_IRQ_INDEX_ADC1_2       = 18;
	static const UINT32 c_IRQ_INDEX_USB_HP_CAN_TX    = 19;
	static const UINT32 c_IRQ_INDEX_USB_LP_CAN_RX0       = 20;
	static const UINT32 c_IRQ_INDEX_CAN_RX1       = 21;
	static const UINT32 c_IRQ_INDEX_CAN_SCE      = 22;
	static const UINT32 c_IRQ_INDEX_EXTI9_5      = 23;
	static const UINT32 c_IRQ_INDEX_TIM1_BRK_TIM9     = 24;
	static const UINT32 c_IRQ_INDEX_TIM1_UP_TIM10      = 25;
	static const UINT32 c_IRQ_INDEX_TIM1_TRG_COM_TIM11     = 26;
	static const UINT32 c_IRQ_INDEX_TIM1_CC       = 27;
	static const UINT32 c_IRQ_INDEX_TIM2       = 28;
	static const UINT32 c_IRQ_INDEX_TIM3       = 29;
	static const UINT32 c_IRQ_INDEX_TIM4       = 30;
	static const UINT32 c_IRQ_INDEX_I2C1_EV     = 31;
	static const UINT32 c_IRQ_INDEX_I2C1_ER      = 32;
	static const UINT32 c_IRQ_INDEX_I2C2_EV      = 33;
	static const UINT32 c_IRQ_INDEX_I2C2_ER      = 34;
	static const UINT32 c_IRQ_INDEX_SPI1       = 35;
	static const UINT32 c_IRQ_INDEX_SPI2       = 36;
	static const UINT32 c_IRQ_INDEX_USART1     = 37;
	static const UINT32 c_IRQ_INDEX_USART2      = 38;
	static const UINT32 c_IRQ_INDEX_USART3      = 39;
	static const UINT32 c_IRQ_INDEX_EXTI15_10    = 40;
	static const UINT32 c_IRQ_INDEX_RTCAlarm      = 41;
	static const UINT32 c_IRQ_INDEX_USBWakeUp     = 42;
	static const UINT32 c_IRQ_INDEX_TIM8_BRK_TIM12   = 43;
	static const UINT32 c_IRQ_INDEX_TIM8_UP    = 44;
	static const UINT32 c_IRQ_INDEX_TIM8_TRG_COM    = 45;
	static const UINT32 c_IRQ_INDEX_TIM8_CC      = 46;
	static const UINT32 c_IRQ_INDEX_ADC3       = 47;
    static const UINT32 c_IRQ_INDEX_FSMC       = 48;
    static const UINT32 c_IRQ_INDEX_SDIO       = 49;
	static const UINT32 c_IRQ_INDEX_TIM5        = 50;
	static const UINT32 c_IRQ_INDEX_SPI3        = 51;
	static const UINT32 c_IRQ_INDEX_USART4       = 52;
	static const UINT32 c_IRQ_INDEX_USART5       = 53;
	static const UINT32 c_IRQ_INDEX_TIM6        = 54;
	static const UINT32 c_IRQ_INDEX_TIM7       = 55;
	static const UINT32 c_IRQ_INDEX_DMA2_Channel1       = 56;
	static const UINT32 c_IRQ_INDEX_DMA2_Channel2       = 57;
	static const UINT32 c_IRQ_INDEX_DMA2_Channel3       = 58;
	static const UINT32 c_IRQ_INDEX_DMA2_Channel4_5     = 59;
	static const UINT32 c_IRQ_INDEX_PendSV          = 60;	//Used to be -2, Modified by Mukundan for Preemptive thread
	static const UINT32 c_IRQ_INDEX_SVCall          = 61;    // Used to -5, Modified by Nived for bottom half thread

	static const UINT32 c_MaxInterruptIndex = 61;


	//
	void DisableInterrupt( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);

        ICER[Irq_Index >> 0x05] = (UINT32) (0x01 << (Irq_Index & (UINT8)0x1F));


    }

    void EnableInterrupt( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);


        ISER[Irq_Index >> 0x05] = (UINT32)0x01 << (Irq_Index & (UINT8)0x1F);
    }

    BOOL SetInterrupt(UINT32 Irq_Index)
    {
    	 ASSERT(Irq_Index < c_MaxInterruptIndex);

    	 ISPR[Irq_Index >> 0x05] = (UINT32)0x01 << (Irq_Index & (UINT8)0x1F);;
    }


    BOOL ClearInterrupt(UINT32 Irq_Index)
    {
    	ASSERT(Irq_Index < c_MaxInterruptIndex);

    	 ICPR[Irq_Index >> 0x05] = (UINT32)0x01 << (Irq_Index & (UINT8)0x1F);;

    }


	//! Checks if the interrupt has been enabled for the given irq channel by checking the bit value of ISER
    BOOL IsInterruptEnabled( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);

        return (ISER[Irq_Index >> 0x05]) >> (Irq_Index & (uint8_t)0x1F);
    }

	//! Returns the state of the interrupt, internally calls the IsInterruptEnabled function
    BOOL GetInterruptState( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);

		return IsInterruptEnabled(Irq_Index);
   }

    //! Stubbed out Cortex - M3 calls the interrupt handlers by itself and there is no concept fo checking the pending interrupts at the software level
    UINT32 NormalInterruptPending()
    {
       return 0;
    }

	//! Stubbed out Cortex - M3 calls the interrupt handlers by itself and there is no concept fo checking the pending interrupts at the software level
    UINT32 FastInterruptPending()
    {
		return 0;
    }

	//! Stubbed out Cortex - M3 doesnot require querying of interrupt pending registers
    BOOL IsInterruptPending()
    {
        return FALSE;
    }

	//! Stubbed out Cortex - M3 doesnot support Fast and Normal Interrupt Channels, hence there is no need to set the type of the interrupt
    void SetType( UINT32 Irq_Index, BOOL Fast )
    {
       return;
	}

    // Nived.Sivadas - this function is configured for priority group 4 because sub priority is defaulted to 0
    void SetPriority( UINT32 Irq_Index, UINT32 priority )
    {

    	// Coded assert
    	if(Irq_Index > c_MaxInterruptIndex)
    	{
    		while(TRUE);
    	}

    	// Coded assert
    	if(priority > 16)
    	{
    		while(TRUE);
    	}

    	UINT32 tmppriority = 0x0;
    	UINT32 tmppre = 0x0;
    	UINT32 tmpsub = 0x0F;

    	tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    	tmppre = (0x4 - tmppriority);
    	tmpsub = tmpsub >> tmppriority;

    	tmppriority = (UINT32) (priority << tmppre);
    	tmppriority |=  (0 & tmpsub);
    	tmppriority = (tmppriority << 0x04);

    	NVIC->IP[(UINT32)Irq_Index] = tmppriority;
    	// Nived.Sivadas - discovered difference between stm32 m3 and official cortex m3
    	// the stm32 firmware uses only 4 bits of the 8 to set priorities
    	// and hence supports only 16 priority levels as opposed to 255
    	// making changes here to reflect this
#if 0
        ASSERT(Irq_Index < c_MaxInterruptIndex);
        ASSERT(priority < 255);

		if(Irq_Index > 0 && Irq_Index != c_IRQ_INDEX_SVCall)
			IP[(UINT32)Irq_Index] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);
		else if(Irq_Index == c_IRQ_INDEX_SVCall)
		{
			// Setting SVCall priority to 1 above the interrupt currently being running
			//SCB->SHP[7] = 1;

			*(__IO UINT32 *) 0xE000ED1C |= (1 << 24);
			// Nived : Need to implement SCB Driver for interrupts < 0
			//SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff); } /* set Priority for Cortex-M3 System Interrupts */
		}
#endif

	}

    UINT32 GetPriority( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < c_MaxInterruptIndex);

		if(Irq_Index < 0) {

		    	return 0;
				// Nived : Need to implement SCB Driver for interrupts < 0
				//return((uint32_t)(SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] >> (8 - __NVIC_PRIO_BITS)));  
				} /* get priority for Cortex-M3 system interrupts */
		else {

				return((UINT32)(NVIC->IP[(UINT32)(Irq_Index)]           >> (8 - __NVIC_PRIO_BITS)));

			 } /* get priority for device specific interrupts  */
    }


    void ForceInterrupt( UINT32 Irq_Index )
    {
        // TBD
        ASSERT(FALSE);      // Interrupts may not be forced for this processor (at this time?)
    }

    void RemoveForcedInterrupt( UINT32 Irq_Index )
    {
        // TBD
    }


};

struct STM32_USART
{
	static const UINT32 c_Base_usart1 = USART1_BASE;
	static const UINT32 c_Base_usart2 = USART2_BASE;

	static const UINT32 USART_PORT_1 = 0;
	static const UINT32 USART_PORT_2 = 1;

	// Need to calculate this for the emote (TODO)
	static const UINT32 c_MAX_BAUDRATE = 115200;
	static const UINT32 c_MIN_BAUDRATE = 300;

	volatile UINT32 SR;
	volatile UINT32 DR;
	volatile UINT32 BRR;
	volatile UINT32 CR1;
	volatile UINT32 CR2;
	volatile UINT32 CR3;
	volatile UINT32 GTPR;

};


struct STM32
{
	    static STM32_AITC &AITC() { return *(STM32_AITC*)(size_t)(STM32_AITC::c_Base); }
	    static STM32_USART &USART(int port) {
	    										if (port == STM32_USART::USART_PORT_1)
	    												return *(STM32_USART *)(size_t)(STM32_USART::c_Base_usart1);
	    										else
	    												return *(STM32_USART *)(size_t)(STM32_USART::c_Base_usart2);
	    										}

};

#endif
