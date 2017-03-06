/////////////////////////////////////////////////////////////////
// The Samraksh Company
//  Author      : Nived Sivadas
//  Description : Equivalent of SF2f10x.h
/////////////////////////////////////////////////////////////////

#ifndef _MYSF2_HEADER_H_
#define _MYSF2_HEADER_H_ 1

#include <cores\arm\include\cpu.h>
#include <cmsis/m2sxxx.h>

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

#if defined(DEBUG)
extern volatile NVIC_Type*          pNVIC;
extern volatile SCB_Type*           pSCB;
extern volatile SysTick_Type*       pSysTick;
extern volatile ITM_Type*           pITM;
extern volatile InterruptType_Type* pInterruptType;
extern volatile MPU_Type*           pMPU;
extern volatile CoreDebug_Type*     pCoreDebug;
#endif

}

//
// SF2 driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Interrupt Controller driver
//

struct SF2_AITC_Driver
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
struct SF2_AITC
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

    /******  Cortex-M3 Processor Exceptions Numbers *********************************************************/
	static const UINT32 c_IRQ_INDEX_NonMaskableInt           = -14;
	static const UINT32 c_IRQ_INDEX_HardFault_IRQn           = -13;
	static const UINT32 c_IRQ_INDEX_MemoryManagementInt    	 = -12;
	static const UINT32 c_IRQ_INDEX_BusFault        		 = -11;
	static const UINT32 c_IRQ_INDEX_UsageFault          	 = -10;
	static const UINT32 c_IRQ_INDEX_DebugMonitor          	 = -4;
	static const UINT32 c_IRQ_INDEX_SysTick          = -1;
	static const UINT32 c_IRQ_INDEX_PendSV          = 82;	//Used to be -2, Modified by Mukundan for Preemptive thread
	static const UINT32 c_IRQ_INDEX_SVCall          = 83;    // Used to -5, Modified by Nived for bottom half thread

	/******  SmartFusion2 specific Interrupt Numbers *********************************************************/
	static const UINT32 c_IRQ_INDEX_WWDG          = 0;
	static const UINT32 c_IRQ_INDEX_RTC          = 1;
    static const UINT32 c_IRQ_INDEX_SPI0       = 2;
    static const UINT32 c_IRQ_INDEX_SPI1     = 3;
    static const UINT32 c_IRQ_INDEX_I2C0     = 4;
    static const UINT32 c_IRQ_INDEX_I2C0_SMBAlert    = 5;
    static const UINT32 c_IRQ_INDEX_I2C0_SMBus   = 6;
    static const UINT32 c_IRQ_INDEX_I2C1      = 7;
	static const UINT32 c_IRQ_INDEX_I2C1_SMBAlert        = 8;
	static const UINT32 c_IRQ_INDEX_I2C1_SMBus        = 9;
	static const UINT32 c_IRQ_INDEX_USART1        = 10;
    static const UINT32 c_IRQ_INDEX_USART2     = 11;
	static const UINT32 c_IRQ_INDEX_EthernetMAC       = 12;
	static const UINT32 c_IRQ_INDEX_DMA       = 13;
	static const UINT32 c_IRQ_INDEX_TIM1_CC       = 14;
	static const UINT32 c_IRQ_INDEX_TIM2      = 15;
	static const UINT32 c_IRQ_INDEX_CAN       = 16;
	static const UINT32 c_IRQ_INDEX_ENVM0       = 17;
	static const UINT32 c_IRQ_INDEX_ENVM1       = 18;
	static const UINT32 c_IRQ_INDEX_ComBlk    = 19;
	static const UINT32 c_IRQ_INDEX_USB       = 20;
	static const UINT32 c_IRQ_INDEX_USB_DMA       = 21;
	static const UINT32 c_IRQ_INDEX_PLL_Lock      = 22;
	static const UINT32 c_IRQ_INDEX_PLL_LockLost      = 23;
	static const UINT32 c_IRQ_INDEX_CommSwitchError     = 24;
	static const UINT32 c_IRQ_INDEX_CacheError     = 25;
	static const UINT32 c_IRQ_INDEX_DDR      = 26;
	static const UINT32 c_IRQ_INDEX_HPDMA_Complete     = 27;
	static const UINT32 c_IRQ_INDEX_HPDMA_Error       = 28;
	static const UINT32 c_IRQ_INDEX_ECC_Error       = 29;
	static const UINT32 c_IRQ_INDEX_MDDR_IOCalib       = 30;
	static const UINT32 c_IRQ_INDEX_FAB_PLL_Lock     = 31;
	static const UINT32 c_IRQ_INDEX_FAB_PLL_LockLost      = 32;
	static const UINT32 c_IRQ_INDEX_FIC64      = 33;
	static const UINT32 c_IRQ_INDEX_FabricIrq0      = 34;
	static const UINT32 c_IRQ_INDEX_FabricIrq1       = 35;
	static const UINT32 c_IRQ_INDEX_FabricIrq2       = 36;
	static const UINT32 c_IRQ_INDEX_FabricIrq3     = 37;
	static const UINT32 c_IRQ_INDEX_FabricIrq4      = 38;
	static const UINT32 c_IRQ_INDEX_FabricIrq5      = 39;
	static const UINT32 c_IRQ_INDEX_FabricIrq6    = 40;
	static const UINT32 c_IRQ_INDEX_FabricIrq7      = 41;
	static const UINT32 c_IRQ_INDEX_FabricIrq8     = 42;
	static const UINT32 c_IRQ_INDEX_FabricIrq9   = 43;
	static const UINT32 c_IRQ_INDEX_FabricIrq10    = 44;
	static const UINT32 c_IRQ_INDEX_FabricIrq11    = 45;
	static const UINT32 c_IRQ_INDEX_FabricIrq12      = 46;
	static const UINT32 c_IRQ_INDEX_FabricIrq13       = 47;
    static const UINT32 c_IRQ_INDEX_FabricIrq14       = 48;
    static const UINT32 c_IRQ_INDEX_FabricIrq15       = 49;
	static const UINT32 c_IRQ_INDEX_GPIO0       = 50;
	static const UINT32 c_IRQ_INDEX_GPIO1       = 51;
	static const UINT32 c_IRQ_INDEX_GPIO2       = 52;
	static const UINT32 c_IRQ_INDEX_GPIO3       = 53;
	static const UINT32 c_IRQ_INDEX_GPIO4       = 54;
	static const UINT32 c_IRQ_INDEX_GPIO5       = 55;
	static const UINT32 c_IRQ_INDEX_GPIO6       = 56;
	static const UINT32 c_IRQ_INDEX_GPIO7       = 57;
	static const UINT32 c_IRQ_INDEX_GPIO8       = 58;
	static const UINT32 c_IRQ_INDEX_GPIO9 		= 59;
	static const UINT32 c_IRQ_INDEX_GPIO10       = 60;
	static const UINT32 c_IRQ_INDEX_GPIO11       = 61;
	static const UINT32 c_IRQ_INDEX_GPIO12       = 62;
	static const UINT32 c_IRQ_INDEX_GPIO13       = 63;
	static const UINT32 c_IRQ_INDEX_GPIO14       = 64;
	static const UINT32 c_IRQ_INDEX_GPIO15       = 65;
	static const UINT32 c_IRQ_INDEX_GPIO16       = 66;
	static const UINT32 c_IRQ_INDEX_GPIO17       = 67;
	static const UINT32 c_IRQ_INDEX_GPIO18       = 68;
	static const UINT32 c_IRQ_INDEX_GPIO19     	 = 69;
	static const UINT32 c_IRQ_INDEX_GPIO20       = 70;
	static const UINT32 c_IRQ_INDEX_GPIO21       = 71;
	static const UINT32 c_IRQ_INDEX_GPIO22       = 72;
	static const UINT32 c_IRQ_INDEX_GPIO23       = 73;
	static const UINT32 c_IRQ_INDEX_GPIO24       = 74;
	static const UINT32 c_IRQ_INDEX_GPIO25       = 75;
	static const UINT32 c_IRQ_INDEX_GPIO26       = 76;
	static const UINT32 c_IRQ_INDEX_GPIO27       = 77;
	static const UINT32 c_IRQ_INDEX_GPIO28       = 78;
	static const UINT32 c_IRQ_INDEX_GPIO29       = 79;
	static const UINT32 c_IRQ_INDEX_GPIO30       = 80;
	static const UINT32 c_IRQ_INDEX_GPIO31       = 81;

	//82&83 are PendSV and SVCall. See up.
	static const UINT32 c_MaxInterruptIndex = 83;

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

	// ! assumes the PROGROUP bits in AIRCR[10:8] have already been initialized by calling NVIC_PriorityGroupConfig in SF2_AITC_Driver::Initialize()
	// ! assumes caller knew the setting to specify acceptable priority
	// ! FYI, In SF2_AITC_Driver, SCB->AIRCR[10:8] set to NVIC_PriorityGroup_4 which uses 4 bits for priority and 0 bits for subgroup.
    void SetPriority( UINT32 Irq_Index, UINT32 priority )
    {


    	// Coded assert
		if(Irq_Index > c_MaxInterruptIndex)
    	{
			#if defined(DEBUG)
				while(TRUE);
			#else
				return;
			#endif
    	}

    	UINT32 tmppriority = 0x0;
    	UINT32 tmppre = 0x0;
    	UINT32 tmpsub = 0x0F;

		tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08; // 0x4 bits for priorities.
		tmppre = (0x04 - tmppriority); // number bits to shift priority.
		tmpsub = tmpsub >> tmppriority; // number of sub-group bits to clear out.

/*#if defined(DEBUG)
		// Coded assert
		if(priority > (0xF >> tmppre))
		{
			while(TRUE);
		}
#endif*/
		tmppriority = (UINT32) (priority << tmppre); // assume caller knows how many priority bits exist, because it uses the lower bits of priority.
		tmppriority |=  (0 & tmpsub);                // clear out subgroup.
		tmppriority = (tmppriority << 0x04);         // STMicro PM0056 pg125.

		NVIC->IP[(UINT32)Irq_Index] = (UINT8)tmppriority;
		// Nived.Sivadas - discovered difference between SF2 m3 and official cortex m3
		// the SF2 firmware uses only 4 bits of the 8 to set priorities
		// and hence supports only 16 priority levels as opposed to 255
		// ... 16 priority levels at most.
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

struct SF2_USART
{
	static const UINT32 c_Base_usart1 = UART0_BASE;
	static const UINT32 c_Base_usart2 = UART1_BASE;

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


struct SF2
{
	    static SF2_AITC &AITC() { return *(SF2_AITC*)(size_t)(SF2_AITC::c_Base); }
	    static SF2_USART &USART(int port) {
	    										if (port == SF2_USART::USART_PORT_1)
	    												return *(SF2_USART *)(size_t)(SF2_USART::c_Base_usart1);
	    										else
	    												return *(SF2_USART *)(size_t)(SF2_USART::c_Base_usart2);
	    										}

};

#endif
