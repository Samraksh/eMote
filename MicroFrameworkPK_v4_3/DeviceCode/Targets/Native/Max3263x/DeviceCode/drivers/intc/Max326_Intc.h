/////////////////////////////////////////////////////////////////
// The Samraksh Company
//  Author      : Nived Sivadas
//  Description : Equivalent of MAX326f10x.h
/////////////////////////////////////////////////////////////////

#ifndef _MAX326_INTC_HEADER_H_
#define _MAX326_INTC_HEADER_H_ 1

#include <cores\arm\include\cpu.h>
#include <core_cm4.h>
//#include <MAX326f10x.h>

//#define BASE                  ((UINT32)0x40000000)
//0x40010400
//#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
//#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400)
//#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)
//#define SCS_BASE            (0xE000E000)                              /*!< System Control Space Base Address */

/*
#ifndef __NVIC_PRIO_BITS
  #define __NVIC_PRIO_BITS    4               //!< standard definition for NVIC Priority Bits
#endif

#define NVIC_PriorityGroup_0         ((uint32_t)0x700) //!< 0 bits for pre-emption priority
#define NVIC_PriorityGroup_1         ((uint32_t)0x600) //!< 1 bits for pre-emption priority, 3 bits for subpriority
#define NVIC_PriorityGroup_2         ((uint32_t)0x500) //!< 2 bits for pre-emption priority, 2 bits for subpriority
#define NVIC_PriorityGroup_3         ((uint32_t)0x400) //!< 3 bits for pre-emption priority, 1 bits for subpriority
#define NVIC_PriorityGroup_4         ((uint32_t)0x300) //!< 4 bits for pre-emption priority, 0 bits for subpriority


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
extern volatile NVIC_Type* pInterruptType;
extern volatile MPU_Type*           pMPU;
extern volatile CoreDebug_Type*     pCoreDebug;
#endif

}
*/

#define ASSERT_IRQ(IRQn) ASSERT(IRQn <= c_MaxInterruptIndex || (IRQn >= 2147483648 && IRQn <= 2147483663))

struct IRQ_VECTORING
{
	UINT32       Priority;
	HAL_CALLBACK Handler;
};

struct MAX326_AITC_Driver
{
    static const UINT32 c_VECTORING_GUARD = 62;
    static IRQ_VECTORING s_IsrTable[];

#if defined(PLATFORM_ARM)
    static void IRQ_Handler();
#endif

    static void Initialize();
    static BOOL ActivateInterrupt  ( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param);
    static BOOL DeactivateInterrupt( UINT32 Irq_Index );
    static BOOL InterruptEnable( UINT32 Irq_Index );
    static BOOL InterruptDisable( UINT32 Irq_Index );
    static BOOL InterruptEnableState( UINT32 Irq_Index );
    static BOOL InterruptState( UINT32 Irq_Index );
    static IRQ_VECTORING* IRQToIRQVector( UINT32 IRQ );

private:
    static void STUB_ISRVector( void* Param );
};


// Interrupt Controller driver
struct MAX326_AITC
{
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

	static const UINT32 c_MaxInterruptIndex = 51;
	static const INT32 c_MinInterruptIndex = -15;

	void DisableInterrupt( UINT32 Irq_Index )
	{
		IRQn_Type IRQn= Irq_Index;
		ASSERT_IRQ(Irq_Index);
		NVIC_DisableIRQ(IRQn);
	}

    void EnableInterrupt( UINT32 Irq_Index )
    {
    	IRQn_Type IRQn= Irq_Index;
    	ASSERT_IRQ(Irq_Index);
        NVIC_EnableIRQ(IRQn);
    }

    BOOL SetInterrupt(UINT32 Irq_Index)
    {
    	IRQn_Type IRQn= Irq_Index;
    	ASSERT_IRQ(Irq_Index);
    	NVIC_SetPendingIRQ(IRQn);
    	return TRUE;
    }

    BOOL ClearInterrupt(UINT32 Irq_Index)
    {
    	IRQn_Type IRQn= Irq_Index;
    	ASSERT_IRQ(Irq_Index);
		NVIC_ClearPendingIRQ(IRQn);
		return TRUE;
    }


	//! Checks if the interrupt has been enabled for the given irq channel by checking the bit value of ISER
    BOOL IsInterruptEnabled(UINT32 Irq_Index )
    {
    	IRQn_Type IRQn= Irq_Index;
    	ASSERT_IRQ(Irq_Index);
        return NVIC_GetActive(IRQn);
    }

	//! Returns the state of the interrupt, is the interrrupt pending?
    BOOL GetInterruptState( UINT32 Irq_Index )
    {

    	ASSERT_IRQ(Irq_Index);
    	IRQn_Type IRQn= Irq_Index;
		return NVIC_GetPendingIRQ(IRQn);
    }

    void SetPriority(UINT32 Irq_Index, UINT32 priority )
	{
		IRQn_Type IRQn= Irq_Index;
		ASSERT_IRQ(Irq_Index);
		NVIC_SetPriority(IRQn, priority);
	}

	UINT32 GetPriority( UINT32 Irq_Index)
	{
		IRQn_Type IRQn= Irq_Index;
		ASSERT_IRQ(Irq_Index);
		return NVIC_GetPriority(IRQn);
	}

    //! Stubbed out Cortex - M4 calls the interrupt handlers by itself and there is no concept fo checking the pending interrupts at the software level
    UINT32 NormalInterruptPending() {  return 0; }

	//! Stubbed out Cortex - M4 calls the interrupt handlers by itself and there is no concept fo checking the pending interrupts at the software level
    UINT32 FastInterruptPending() { return 0; }

	//! Stubbed out Cortex - M3 doesnot require querying of interrupt pending registers
    BOOL IsInterruptPending() { return FALSE; }

	//! Stubbed out Cortex - M4 doesnot support Fast and Normal Interrupt Channels, hence there is no need to set the type of the interrupt
    void SetType(UINT32 Irq_Index, BOOL Fast ) { return; }

    void ForceInterrupt( UINT32 Irq_Index ) {
        // TBD
        ASSERT(FALSE);      // Interrupts may not be forced for this processor (at this time?)
    }

    void RemoveForcedInterrupt( UINT32 Irq_Index )
    {
        // TBD
    }
};

/*
struct MAX326_USART
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


struct MAX326
{
	static MAX326_AITC &AITC() { return *(MAX326_AITC*)(size_t)(MAX326_AITC::c_Base); }
	static MAX326_USART &USART(int port) {
			if (port == MAX326_USART::USART_PORT_1)
					return *(MAX326_USART *)(size_t)(MAX326_USART::c_Base_usart1);
			else
					return *(MAX326_USART *)(size_t)(MAX326_USART::c_Base_usart2);
		}

};
*/


#endif //end of header file
