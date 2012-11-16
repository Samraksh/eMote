#ifndef _KRAIT_TIMER_REG_DEF_H_
#define _KRAIT_TIMER_REG_DEF_H_


typedef UINT64 cycle_t;

#define CLOCK_SOURCE_IS_CONTINUOUS		0x01

#define CLOCK_EVT_FEAT_PERIODIC			0x000001
#define CLOCK_EVT_FEAT_ONESHOT			0x000002

#define SZ_4K							0x00001000

#define IOMEM(x)	                	((void *)(x))

#define MSM_CSR_BASE			        IOMEM(0xFA001000)
#define MSM_CSR_PHYS         			0xC0100000
#define MSM_CSR_SIZE          			SZ_4K

#define MSM_TMR_PHYS         			MSM_CSR_PHYS
//#define MSM_TMR_BASE          			MSM_CSR_BASE
#define MSM_TMR_SIZE          			SZ_4K

#define MSM_TMR0_BASE         			MSM_TMR_BASE

#define DG_TIMER_RATING 				300

#define GIC_PPI_START 					16
#define GIC_SPI_START 					32

#define INT_DEBUG_TIMER_EXP				(GIC_PPI_START + 1)
#define INT_GP_TIMER_EXP				(GIC_PPI_START + 2)
#define INT_GP_TIMER2_EXP				(GIC_PPI_START + 3)


#define CLOCKSOURCE_MASK(bits) (cycle_t)((bits) < 64 ? ((1ULL<<(bits))-1) : -1)

#endif
