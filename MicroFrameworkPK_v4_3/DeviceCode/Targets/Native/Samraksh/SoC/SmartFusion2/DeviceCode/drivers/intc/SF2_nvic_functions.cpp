////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
//////////////////////////////////////////////////////////////////////

#include "SF2.h"

void CPU_INTC_Initialize()
{
    SF2_AITC_Driver::Initialize();
}

BOOL CPU_INTC_ActivateInterrupt( UINT32 Irq_Index, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
    return SF2_AITC_Driver::ActivateInterrupt( Irq_Index, FALSE, ISR, ISR_Param);
}

BOOL CPU_INTC_DeactivateInterrupt( UINT32 Irq_Index )
{
    return SF2_AITC_Driver::DeactivateInterrupt( Irq_Index );
}

BOOL CPU_INTC_InterruptEnable( UINT32 Irq_Index )
{
    return SF2_AITC_Driver::InterruptEnable( Irq_Index );
}

BOOL CPU_INTC_InterruptDisable( UINT32 Irq_Index )
{
    return SF2_AITC_Driver::InterruptDisable( Irq_Index );
}

BOOL CPU_INTC_InterruptEnableState( UINT32 Irq_Index )
{
    return SF2_AITC_Driver::InterruptEnableState( Irq_Index );
}

BOOL CPU_INTC_InterruptState( UINT32 Irq_Index )
{
    return SF2_AITC_Driver::InterruptState( Irq_Index );
}


