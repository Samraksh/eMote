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
    //m_context = context;
    //Disable();
	__asm volatile(
    "MRS      r2,PRIMASK\n"
    "CPSID    i\n"
    "STR      r2,[r0,#__cpp(offsetof(SmartPtr_IRQ, m_state))]\n"
    "BX       lr\n"
    );


}

SmartPtr_IRQ::~SmartPtr_IRQ()
{
    //Restore();
	__asm volatile(
    "LDR      r1,[r0,#__cpp(offsetof(SmartPtr_IRQ, m_state))]\n"
    "MSR      PRIMASK,r1\n"
    "BX       lr"
			);

}

BOOL SmartPtr_IRQ::WasDisabled()
{
	__asm volatile(
	    "MRS      r1,IPSR\n"
	    "LDR      r0,[r0,#__cpp(offsetof(SmartPtr_IRQ, m_state))]\n"
	    "CBZ      r1,L1\n"
	    "MOV      r0,#1\n"
	    "L1  BX       lr\n"
			);
}

void SmartPtr_IRQ::Acquire()
{
	__asm volatile
	(
	  "CPSID    i\n"
	  "BX       lr\n"
	 );
}

void SmartPtr_IRQ::Release()
{
	__asm volatile(
		"LDR      r1,[r0,#__cpp(offsetof(SmartPtr_IRQ, m_state))]\n"
	    "MSR      PRIMASK,r1\n"
	    "BX       lr\n"
	    );
}

void SmartPtr_IRQ::Probe()
{
	__asm volatile(
		"LDR      r1,[r0,#__cpp(offsetof(SmartPtr_IRQ, m_state))]\n"
	    "MRS      r2,PRIMASK\n"
	    "MSR      PRIMASK,r1\n"
	    "NOP\n"
	    "MSR      PRIMASK,r2\n"
	    "BX       lr\n"
		);
}

BOOL SmartPtr_IRQ::GetState(void* context)
{
	__asm volatile(
		"MRS      r0,PRIMASK\n"
	    "MRS      r1,IPSR\n"
	    "EOR      r0,r0,#1\n"
	    "CBZ      r1,L2\n"
	    "MOV      r0,#0\n"
	    "L2  BX       lr\n"
			);
}

BOOL SmartPtr_IRQ::ForceDisabled(void* context)
{
	__asm volatile
	(
		"MRS      r0,PRIMASK\n"
	    "CPSID    i\n"
	    "EOR      r0,r0,#1\n"
	    "BX       lr\n"
	);
	return TRUE;
}

BOOL SmartPtr_IRQ::ForceEnabled(void* context)
{
	__asm volatile
	(
		"MRS      r0,PRIMASK\n"
		"CPSIE    i\n"
		"EOR      r0,r0,#1\n"
		"BX       lr\n"
			);
	return TRUE;
}

void SmartPtr_IRQ::Disable()
{
	__asm volatile
	(
	    "MRS      r1,PRIMASK\n"
	    "CPSID    i\n"
	    "STR      r1,[r0,#__cpp(offsetof(SmartPtr_IRQ, m_state))]\n"
	    "BX       lr\n"
			);
}

void SmartPtr_IRQ::Restore()
{
	__asm volatile
	(
		"LDR      r1,[r0,#__cpp(offsetof(SmartPtr_IRQ, m_state))]\n"
	    "MSR      PRIMASK,r1\n"
	    "BX       lr\n"
			);
}

