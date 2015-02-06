////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#define NVIC_ICER       0xE000E180
#define NVIC_ISER       0xE000E100
#define NVIC_ABR        0xE000E300

//--//
#define DISABLED_MASK 0x3

#define __ASM __asm
//--//

///////////////////////////////////////////////////////////////////
// For Thumb2 code, we need to declare some functions as extern 
// and implement them in assembly, as the RVDS3.1 compiler does
// not support inline assembly
// 
extern "C"
{
void   IRQ_LOCK_Probe_asm();
UINT32 IRQ_LOCK_Release_asm();
UINT32 IRQ_LOCK_GetState_asm();
UINT32 IRQ_LOCK_ForceDisabled_asm();
UINT32 IRQ_LOCK_ForceEnabled_asm();
UINT32 IRQ_LOCK_Disable_asm();
void   IRQ_LOCK_Restore_asm();
}
//
//
///////////////////////////////////////////////////////////////////


SmartPtr_IRQ::SmartPtr_IRQ(void* context)
{ 
    m_context = context; 
    Disable(); 
}

SmartPtr_IRQ::~SmartPtr_IRQ() 
{ 
	__ASM volatile("cpsie i");
}

BOOL SmartPtr_IRQ::WasDisabled()
{
    return (m_state & DISABLED_MASK) == DISABLED_MASK;
}

void SmartPtr_IRQ::Acquire()
{
    UINT32 Cp = m_state;

    if((Cp & DISABLED_MASK) == DISABLED_MASK)
    {
        Disable();
    }
}

void SmartPtr_IRQ::Release()
{
    UINT32 Cp = m_state;

    if((Cp & DISABLED_MASK) == 0)
    {
    	 UINT32 Cs;

    	        __asm
    	        (
    	           " MRS     %0, PSR\n"
    	            "BIC     %1, %2, #0x80\n"
    	            "cpsie i"
    	        		: "=r" (Cs), "=r" (Cp)
    	        		: "r" (Cs)
    	        );

    	 	 // Unable to read CPSR register using MSR or LDR instructrion.
    	        m_state = Cp;
    }
}

void SmartPtr_IRQ::Probe()
{
    UINT32 Cp = m_state;

    if((Cp & DISABLED_MASK) == 0)
    {
    	// Nived.Sivadas@samraksh.com Don't have quite the semantics of allowing one interrupt to be processed, so faking it here
    	__ASM volatile("cpsie i");
    	__ASM volatile("cpsid i");
    }
}

BOOL SmartPtr_IRQ::GetState(void* context)
{
	UINT32 Cp;


	    __asm volatile(
	        "MRS     %[tmp], PSR\n\t"
	        "MVN     %[tmp], %[tmp]\n\t"
	        "AND     %[tmp], %[tmp], #0x80"
	    		: [tmp] "=r" (Cp)
	    );


	    return (Cp == 0);
}

BOOL SmartPtr_IRQ::ForceDisabled(void* context)
{
	__ASM volatile("cpsid i");
	return TRUE;
}

BOOL SmartPtr_IRQ::ForceEnabled(void* context)
{
	__ASM volatile("cpsie i");
	return TRUE;
}

void SmartPtr_IRQ::Disable()
{
	__ASM volatile("cpsid i");
    m_state = 0x3;
}

void SmartPtr_IRQ::Restore()
{

    __ASM volatile("cpsie i");
}

