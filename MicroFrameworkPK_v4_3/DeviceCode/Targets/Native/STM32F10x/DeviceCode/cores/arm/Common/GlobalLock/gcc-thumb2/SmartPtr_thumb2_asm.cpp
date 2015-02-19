////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#if defined(SAMRAKSH_RTOS_EXT)
#warning "GLOBAL_LOCK may interfere with real time. Need a new version of SmartPtr that utilizes the BASEPRI register to disable all interrupts except realtime."
#endif

//--//

#define __ASM __asm
//--//


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
	    "CBZ      r1,L1\n"/* if we are currently disabled, return m_state*/
	    "MOV      r0,#1\n"/* if we are in an interrupt and are currently disabled, then return true.*/
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

