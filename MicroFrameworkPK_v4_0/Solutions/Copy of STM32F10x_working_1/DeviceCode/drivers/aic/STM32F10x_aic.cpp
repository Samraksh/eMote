////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

void __irq IRQ_Handler()
{
	while(1); // preserve state for debugger
}

void CPU_INTC_Initialize()
{
	// No init necessary
}

BOOL CPU_INTC_ActivateInterrupt( UINT32 Irq_Index, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
	NVIC_EnableIRQ((IRQn_Type)Irq_Index);
    return TRUE;	// note that we are NOT adding the callback because we have static handlers
}

BOOL CPU_INTC_DeactivateInterrupt( UINT32 Irq_Index )
{
	NVIC_DisableIRQ((IRQn_Type)Irq_Index);
    return FALSE;
}

BOOL CPU_INTC_InterruptEnable( UINT32 Irq_Index )
{
	NVIC_EnableIRQ((IRQn_Type)Irq_Index);
    return TRUE;
}

BOOL CPU_INTC_InterruptDisable( UINT32 Irq_Index )
{
	NVIC_ClearPendingIRQ((IRQn_Type)Irq_Index);
    return TRUE;
}

BOOL CPU_INTC_InterruptEnableState( UINT32 Irq_Index )
{
	NVIC_SetPendingIRQ((IRQn_Type)Irq_Index);
    return TRUE;
}

BOOL CPU_INTC_InterruptState( UINT32 Irq_Index )
{
    return (BOOL)(NVIC_GetPendingIRQ((IRQn_Type)Irq_Index));
}