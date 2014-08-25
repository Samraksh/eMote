/*
 *
 * 	Name : Krait_INTC.cpp
 *
 *
 *
 *
 *
 */

#ifndef SAM_FORCE_GCC_CMDLINE_OPTS
#pragma GCC optimize ("O3")
#endif

#include "Krait__INTC.h"
#include <tinyhal.h>
static struct ihandler handler[NR_IRQS];

extern "C"
{

/* Intialize distributor */
void qgic_dist_init(void)
{
	uint32_t i;
	uint32_t num_irq = 0;
	uint32_t cpumask = 1;

	cpumask |= cpumask << 8;
	cpumask |= cpumask << 16;

	/* Disabling GIC */
	writel(0, GIC_DIST_CTRL);

	/*
	 * Find out how many interrupts are supported.
	 */
	num_irq = readl(GIC_DIST_CTR) & 0x1f;
	num_irq = (num_irq + 1) * 32;

	/* Set each interrupt line to use N-N software model
	 * and edge sensitive, active high
	 */
	for (i = 32; i < num_irq; i += 16)
		writel(0xffffffff, GIC_DIST_CONFIG + i * 4 / 16);

	writel(0xffffffff, GIC_DIST_CONFIG + 4);

	/* Set up interrupts for this CPU */
	for (i = 32; i < num_irq; i += 4)
		writel(cpumask, GIC_DIST_TARGET + i * 4 / 4);

	/* Set priority of all interrupts */

	/*
	 * In bootloader we dont care about priority so
	 * setting up equal priorities for all
	 */
	for (i = 0; i < num_irq; i += 4)
		writel(0xa0a0a0a0, GIC_DIST_PRI + i * 4 / 4);

	/* Disabling interrupts */
	for (i = 0; i < num_irq; i += 32)
		writel(0xffffffff, GIC_DIST_ENABLE_CLEAR + i * 4 / 32);

	writel(0x0000ffff, GIC_DIST_ENABLE_SET);

	/*Enabling GIC */
	writel(1, GIC_DIST_CTRL);
}


/* Intialize cpu specific controller */
void qgic_cpu_init(void)
{
	writel(0xf0, GIC_CPU_PRIMASK);
	writel(1, GIC_CPU_CTRL);
}

// Initializes the interrupt controller
void qgic_init(void)
{
	qgic_dist_init();
	qgic_cpu_init();
}

/* Mask interrupt */
status_t mask_interrupt(unsigned int vector)
{
	uint32_t reg = GIC_DIST_ENABLE_CLEAR + (vector / 32) * 4;
	uint32_t bit = 1 << (vector & 31);

	writel(bit, reg);

	return 0;
}

/* Un-mask interrupt */
status_t unmask_interrupt(unsigned int vector)
{
	uint32_t reg = GIC_DIST_ENABLE_SET + (vector / 32) * 4;
	uint32_t bit = 1 << (vector & 31);

	writel(bit, reg);

	return 0;
}

/* Register interrupt handler */
void register_int_handler(unsigned int vector, int_handler func, void *arg)
{
	ASSERT(vector < NR_IRQS);

//	enter_critical_section();
	handler[vector].func = func;
	handler[vector].arg = arg;
//	exit_critical_section();
}


}

void IRQ_Handler()
{
	uint32_t num;
		enum handler_return ret;

		num = readl(GIC_CPU_INTACK);
		if (num > NR_IRQS)
			return;

		ret = handler[num].func(handler[num].arg);
		writel(num, GIC_CPU_EOI);

}


