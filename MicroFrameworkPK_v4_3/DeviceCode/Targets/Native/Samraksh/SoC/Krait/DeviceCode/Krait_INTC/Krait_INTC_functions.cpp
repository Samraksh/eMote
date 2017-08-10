/*
 *  Name : Krait_INTC_functions.cpp
 *
 *  Author : Nived.Sivadas@Samraksh.com
 *
 *  Description : contains the intc api required by the .net mf
 *
 */

#include <cores\arm\include\cpu.h>
#include "Krait__INTC.h"

#if 0
extern void qgic_init();
extern void register_int_handler(unsigned int vector, int_handler func, void *arg);
extern status_t unmask_interrupt(unsigned int vector);
extern status_t mask_interrupt(unsigned int vector);
#endif

void CPU_INTC_Initialize()
{
	qgic_init();
}

BOOL CPU_INTC_ActivateInterrupt( UINT32 Irq_Index, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{

	register_int_handler(Irq_Index, (int_handler) ISR, ISR_Param);
}

BOOL CPU_INTC_DeactivateInterrupt( UINT32 Irq_Index )
{

}

BOOL CPU_INTC_InterruptEnable( UINT32 Irq_Index )
{
	unmask_interrupt(Irq_Index);
}

BOOL CPU_INTC_InterruptDisable( UINT32 Irq_Index )
{
	mask_interrupt(Irq_Index);
}

BOOL CPU_INTC_InterruptEnableState( UINT32 Irq_Index )
{

}

BOOL CPU_INTC_InterruptState( UINT32 Irq_Index )
{

}

