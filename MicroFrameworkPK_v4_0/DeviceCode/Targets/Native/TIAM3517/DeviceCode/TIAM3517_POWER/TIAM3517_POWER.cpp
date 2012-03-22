/*
	Microframework 4.0 HAL driver for basic power functions.
	For now just sleep. Add power levels.


	Nathan Stohs
	nathan.stohs@samraksh.com
	The Samraksh Company
	2012-03-20
*/

#include <tinyhal.h>
#include "..\TIAM3517.h"

static int disable_interrupts(void);
static void enable_interrupts(void);

// Sleep function in assembly.
extern "C" { extern void omap3517_cpu_suspend(u32 *addr, int save_state); }


/*
Uses WFI instruction (arm-v7a and later)
Interrupt source must be configured *and* unmasked in INTC

Note this will sleep SDRAM (memory is retained) and so
omap3517_cpu_suspend needs to live outside SDRAM (e.g. 64K on-chip memory)

Can use 'void __section(BootStrap) TIAM3517_POWER_Driver::Sleep()'
but only assembly is critical and should contain .section

Disable interrupts to the ARM core (but enable/unmask in INTC!) so
that the core doesn't try to service in the instruction after wfi,
as we must wait a few clocks and re-enable SDRAM etc.

Re-enable interrupts afterwards (if they were previously enabled)
*/

void TIAM3517_POWER_Driver::Sleep() {
	int enabled;
	
	enabled = disable_interrupts();
	omap3517_cpu_suspend(NULL, 0); 	// parameters are meaningless?, remove
	
	if (enabled)					// Re-enable interrupt servicing (if previously set)
		enable_interrupts();
}

// UNTESTED
void HAL_AssertEx() {
	// cause an abort and let the abort handler take over
    *((char*)0xFFFFFFFF) = 'a';
}

// Disable servicing of interrupts on the ARM core, (note distinct from INTC module)
// Code from INTC module, but didn't feel like sharing. Flag for clean-up.
static int disable_interrupts(void) {
        unsigned long old,temp;
        __asm__ __volatile__("mrs %0, cpsr\n"
                             "orr %1, %0, #0xc0\n"
                             "msr cpsr_c, %1"
                             : "=r" (old), "=r" (temp)
                             :
                             : "memory");
        return (old & 0x80) == 0;
}

// Enable servicing of interrupts on the ARM core, (note distinct from INTC module)
// Code from INTC module, but didn't feel like sharing. Flag for clean-up.
static void enable_interrupts(void) {
        unsigned long temp;
        __asm__ __volatile__("mrs %0, cpsr\n"
                             "bic %0, %0, #0x80\n"
                             "msr cpsr_c, %0"
                             : "=r" (temp)
                             :
                             : "memory");
}
