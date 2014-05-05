/*
 * 	Name 	: Krait_INTC.h
 *
 *  Description : contains the macros for the interuppt controller
 *
 *
 *
 */

#ifndef _KRAIT_INTC_H_
#define _KRAIT_INTC_H_

#include "..\Krait.h"

#define MSM_GIC_DIST_BASE   0x02000000
#define MSM_GIC_CPU_BASE    0x02002000

typedef enum handler_return (*int_handler)(void *arg);

#define GIC_CPU_REG(off)            (MSM_GIC_CPU_BASE  + (off))
#define GIC_DIST_REG(off)           (MSM_GIC_DIST_BASE + (off))

#define GIC_CPU_CTRL                GIC_CPU_REG(0x00)
#define GIC_CPU_PRIMASK             GIC_CPU_REG(0x04)
#define GIC_CPU_BINPOINT            GIC_CPU_REG(0x08)
#define GIC_CPU_INTACK              GIC_CPU_REG(0x0c)
#define GIC_CPU_EOI                 GIC_CPU_REG(0x10)
#define GIC_CPU_RUNNINGPRI          GIC_CPU_REG(0x14)
#define GIC_CPU_HIGHPRI             GIC_CPU_REG(0x18)

#define GIC_DIST_CTRL               GIC_DIST_REG(0x000)
#define GIC_DIST_CTR                GIC_DIST_REG(0x004)
#define GIC_DIST_ENABLE_SET         GIC_DIST_REG(0x100)
#define GIC_DIST_ENABLE_CLEAR       GIC_DIST_REG(0x180)
#define GIC_DIST_PENDING_SET        GIC_DIST_REG(0x200)
#define GIC_DIST_PENDING_CLEAR      GIC_DIST_REG(0x280)
#define GIC_DIST_ACTIVE_BIT         GIC_DIST_REG(0x300)
#define GIC_DIST_PRI                GIC_DIST_REG(0x400)
#define GIC_DIST_TARGET             GIC_DIST_REG(0x800)
#define GIC_DIST_CONFIG             GIC_DIST_REG(0xc00)
#define GIC_DIST_SOFTINT            GIC_DIST_REG(0xf00)

#define GIC_PPI_START 16
#define GIC_SPI_START 32

#define INT_DEBUG_TIMER_EXP     (GIC_PPI_START + 1)

#define USB1_HS_BAM_IRQ         (GIC_SPI_START + 94)
#define USB1_HS_IRQ             (GIC_SPI_START + 100)
#define USB2_IRQ                (GIC_SPI_START + 141)
#define USB1_IRQ                (GIC_SPI_START + 142)

#define GSBI_QUP_IRQ(id)       ((id) <= 8 ? (GIC_SPI_START + 145 + 2*((id))) : \
                                            (GIC_SPI_START + 187 + 2*((id)-8)))

/* Retrofit universal macro names */
#define INT_USB_HS                  USB1_HS_IRQ

// Nived.Sivadas : This was initially 256, 173 and 0 but the kernel is suggesting different values for the same

#define NR_MSM_IRQS                 288
#define NR_GPIO_IRQS                152
#define NR_PM8921_IRQS 256
#define NR_PM8821_IRQS 64
#define NR_TABLA_IRQS 49
#define NR_GPIO_EXPANDER_IRQS 8
#define NR_BOARD_IRQS (NR_PM8921_IRQS + NR_PM8821_IRQS + \
                NR_TABLA_IRQS + NR_GPIO_EXPANDER_IRQS)

#define NR_IRQS (NR_MSM_IRQS + NR_GPIO_IRQS + NR_BOARD_IRQS)

// Nived.Sivadas : This was also in the kernel
#define NR_TLMM_MSM_DIR_CONN_IRQ 8 /*Need to Verify this Count*/
#define NR_MSM_GPIOS NR_GPIO_IRQS

#define MSM_GPIO_TO_INT(n) (NR_MSM_IRQS + (n))
#define FIRST_GPIO_IRQ MSM_GPIO_TO_INT(0)
#define MSM_INT_TO_REG(base, irq) (base + irq / 32)



extern "C"
{
void qgic_init(void);
void register_int_handler(unsigned int vector, int_handler handler, void *arg);
status_t mask_interrupt(unsigned int vector);
status_t unmask_interrupt(unsigned int vector);
}

struct ihandler {
	int_handler func;
	void *arg;
};

#endif
