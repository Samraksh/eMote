/*
	Microframework 4.0 HAL driver for basic power functions.
	For now just sleep etc.


	Nathan Stohs
	nathan.stohs@samraksh.com
	The Samraksh Company
	2012-03-20
*/

#include <tinyhal.h>
#include "..\TIAM3517.h"

extern void omap3517_cpu_suspend(u32 *addr, int save_state);

/*
Uses WFI.
Interrupt source must be configured *and* unmasked in INTC

Note this will sleep SDRAM (memory is retained).
omap3517_cpu_suspend must live in 64K on-chip memory

disable_interrupts will still allow an interrupt to wake the core.
Must run wake-up code before servicing the next interrupt however.
*/

//TIAM3517_POWER_Driver tiam3517_g;
static int disable_interrupts(void);
static void enable_interrupts(void);

void TIAM3517_POWER_Driver::Sleep() {
	disable_interrupts();
	omap3517_cpu_suspend(NULL, 0);
	enable_interrupts();
}

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

static void enable_interrupts(void) {
        unsigned long temp;
        __asm__ __volatile__("mrs %0, cpsr\n"
                             "bic %0, %0, #0x80\n"
                             "msr cpsr_c, %0"
                             : "=r" (temp)
                             :
                             : "memory");
}
