/*
 *  Name : TIAM3517.h
 *
 *  Author : Nived.Sivadas@Samraksh.com
 *
 *  Description : Contains structures of all drivers for the TIAM3517
 *
 *  Change History :
 *  			
 *  			v0.0 Seminal Version written for running initial MF testing on SOC8200 
 *
 */

#ifndef _TIAM3517_H_
#define _TIAM3517_H_

#include <tinyhal.h>

#define TIAM3517_INTC_IRQROUTE 0
#define TIAM3517_INTC_FIQROUTE 1
//#include <stdio.h>
//

#define MF_GPIO_OFFSET_GPIO1 0
#define MF_GPIO_OFFSET_GPIO2 4 * 1024 
#define MF_GPIO_OFFSET_GPIO3 8 * 1024 
#define MF_GPIO_OFFSET_GPIO4 12 * 1024
#define MF_GPIO_OFFSET_GPIO5 16 * 1024
#define MF_GPIO_OFFSET_GPIO6 20 * 1024

// Following macros copied from uboot\uboot\include\asm-arch\arch-omap3\clocks_omap3.h
#define PLL_STOP		1	/* PER & IVA */
#define PLL_LOW_POWER_BYPASS	5	/* MPU, IVA & CORE */
#define PLL_FAST_RELOCK_BYPASS	6	/* CORE */
#define PLL_LOCK		7	/* MPU, IVA, CORE & PER */


/* CORE DPLL */
#define CORE_M3X2	2	/* 332MHz : CM_CLKSEL1_EMU */
#define CORE_SSI_DIV	3	/* 221MHz : CM_CLKSEL_CORE */
#define CORE_FUSB_DIV	2	/* 41.5MHz: */
#define CORE_L4_DIV	2	/* 83MHz  : L4 */
#define CORE_L3_DIV	2	/* 166MHz : L3 {DDR} */
#define GFX_DIV		2	/* 83MHz  : CM_CLKSEL_GFX */
#define WKUP_RSM	2	/* 41.5MHz: CM_CLKSEL_WKUP */

/* PER DPLL */
#define PER_M6X2	3	/* 288MHz: CM_CLKSEL1_EMU */
#define PER_M5X2	4	/* 216MHz: CM_CLKSEL_CAM */
#define PER_M4X2	2	/* 432MHz: CM_CLKSEL_DSS-dss1 */
#define PER_M3X2	16	/* 54MHz : CM_CLKSEL_DSS-tv */

#define CLSEL1_EMU_VAL ((CORE_M3X2 << 16) | (PER_M6X2 << 24) | (0x0A50))

/* MPU DPLL */

#define MPU_M_12_ES1		0x0FE
#define MPU_N_12_ES1		0x07
#define MPU_FSEL_12_ES1		0x05
#define MPU_M2_12_ES1		0x01

#define MPU_M_12_ES2		0x0FA
#define MPU_N_12_ES2		0x05
#define MPU_FSEL_12_ES2		0x07
#define MPU_M2_ES2		0x01

#define MPU_M_12		0x085
#define MPU_N_12		0x05
#define MPU_FSEL_12		0x07
#define MPU_M2_12		0x01

#define MPU_M_13_ES1		0x17D
#define MPU_N_13_ES1		0x0C
#define MPU_FSEL_13_ES1		0x03
#define MPU_M2_13_ES1		0x01

#define MPU_M_13_ES2		0x1F4
#define MPU_N_13_ES2		0x0C
#define MPU_FSEL_13_ES2		0x03
#define MPU_M2_13_ES2		0x01

#define MPU_M_13		0x10A
#define MPU_N_13		0x0C
#define MPU_FSEL_13		0x03
#define MPU_M2_13		0x01

#define MPU_M_19P2_ES1		0x179
#define MPU_N_19P2_ES1		0x12
#define MPU_FSEL_19P2_ES1	0x04
#define MPU_M2_19P2_ES1		0x01

#define MPU_M_19P2_ES2		0x271
#define MPU_N_19P2_ES2		0x17
#define MPU_FSEL_19P2_ES2	0x03
#define MPU_M2_19P2_ES2		0x01

#define MPU_M_19P2		0x14C
#define MPU_N_19P2		0x17
#define MPU_FSEL_19P2		0x03
#define MPU_M2_19P2		0x01

#define MPU_M_26_ES1		0x17D
#define MPU_N_26_ES1		0x19
#define MPU_FSEL_26_ES1		0x03
#define MPU_M2_26_ES1		0x01

#define MPU_M_26_ES2		0x0FA
#define MPU_N_26_ES2		0x0C
#define MPU_FSEL_26_ES2		0x07
#define MPU_M2_26_ES2		0x01

#define MPU_M_26		0x085
#define MPU_N_26		0x0C
#define MPU_FSEL_26		0x07
#define MPU_M2_26		0x01

#define MPU_M_38P4_ES1		0x1FA
#define MPU_N_38P4_ES1		0x32
#define MPU_FSEL_38P4_ES1	0x03
#define MPU_M2_38P4_ES1		0x01

#define MPU_M_38P4_ES2		0x271
#define MPU_N_38P4_ES2		0x2F
#define MPU_FSEL_38P4_ES2	0x03
#define MPU_M2_38P4_ES2		0x01

#define MPU_M_38P4		0x14C
#define MPU_N_38P4		0x2F
#define MPU_FSEL_38P4		0x03
#define MPU_M2_38P4		0x01

/* IVA DPLL */

#define IVA_M_12_ES1		0x07D
#define IVA_N_12_ES1		0x05
#define IVA_FSEL_12_ES1		0x07
#define IVA_M2_12_ES1		0x01

#define IVA_M_12_ES2		0x0B4
#define IVA_N_12_ES2		0x05
#define IVA_FSEL_12_ES2		0x07
#define IVA_M2_12_ES2		0x01

#define IVA_M_12		0x085
#define IVA_N_12		0x05
#define IVA_FSEL_12		0x07
#define IVA_M2_12		0x01

#define IVA_M_13_ES1		0x0FA
#define IVA_N_13_ES1		0x0C
#define IVA_FSEL_13_ES1		0x03
#define IVA_M2_13_ES1		0x01

#define IVA_M_13_ES2		0x168
#define IVA_N_13_ES2		0x0C
#define IVA_FSEL_13_ES2		0x03
#define IVA_M2_13_ES2		0x01

#define IVA_M_13		0x10A
#define IVA_N_13		0x0C
#define IVA_FSEL_13		0x03
#define IVA_M2_13		0x01

#define IVA_M_19P2_ES1		0x082
#define IVA_N_19P2_ES1		0x09
#define IVA_FSEL_19P2_ES1	0x07
#define IVA_M2_19P2_ES1		0x01

#define IVA_M_19P2_ES2		0x0E1
#define IVA_N_19P2_ES2		0x0B
#define IVA_FSEL_19P2_ES2	0x06
#define IVA_M2_19P2_ES2		0x01

#define IVA_M_19P2		0x14C
#define IVA_N_19P2		0x17
#define IVA_FSEL_19P2		0x03
#define IVA_M2_19P2		0x01

#define IVA_M_26_ES1		0x07D
#define IVA_N_26_ES1		0x0C
#define IVA_FSEL_26_ES1		0x07
#define IVA_M2_26_ES1		0x01

#define IVA_M_26_ES2		0x0B4
#define IVA_N_26_ES2		0x0C
#define IVA_FSEL_26_ES2		0x07
#define IVA_M2_26_ES2		0x01

#define IVA_M_26		0x085
#define IVA_N_26		0x0C
#define IVA_FSEL_26		0x07
#define IVA_M2_26		0x01

#define IVA_M_38P4_ES1		0x13F
#define IVA_N_38P4_ES1		0x30
#define IVA_FSEL_38P4_ES1	0x03
#define IVA_M2_38P4_ES1		0x01

#define IVA_M_38P4_ES2		0x0E1
#define IVA_N_38P4_ES2		0x17
#define IVA_FSEL_38P4_ES2	0x06
#define IVA_M2_38P4_ES2		0x01

#define IVA_M_38P4		0x14C
#define IVA_N_38P4		0x2F
#define IVA_FSEL_38P4		0x03
#define IVA_M2_38P4		0x01

/* CORE DPLL */

#define CORE_M_12		0xA6
#define CORE_N_12		0x05
#define CORE_FSEL_12		0x07
#define CORE_M2_12		0x01	/* M3 of 2 */

#define CORE_M_12_ES1		0x19F
#define CORE_N_12_ES1		0x0E
#define CORE_FSL_12_ES1		0x03
#define CORE_M2_12_ES1		0x1	/* M3 of 2 */

#define CORE_M_13		0x14C
#define CORE_N_13		0x0C
#define CORE_FSEL_13		0x03
#define CORE_M2_13		0x01	/* M3 of 2 */

#define CORE_M_13_ES1		0x1B2
#define CORE_N_13_ES1		0x10
#define CORE_FSL_13_ES1		0x03
#define CORE_M2_13_ES1		0x01	/* M3 of 2 */

#define CORE_M_19P2		0x19F
#define CORE_N_19P2		0x17
#define CORE_FSEL_19P2		0x03
#define CORE_M2_19P2		0x01	/* M3 of 2 */

#define CORE_M_19P2_ES1		0x19F
#define CORE_N_19P2_ES1		0x17
#define CORE_FSL_19P2_ES1	0x03
#define CORE_M2_19P2_ES1	0x01	/* M3 of 2 */

#define CORE_M_26		0xA6
#define CORE_N_26		0x0C
#define CORE_FSEL_26		0x07
#define CORE_M2_26		0x01	/* M3 of 2 */

#define CORE_M_26_ES1		0x1B2
#define CORE_N_26_ES1		0x21
#define CORE_FSL_26_ES1		0x03
#define CORE_M2_26_ES1		0x01	/* M3 of 2 */

#define CORE_M_38P4		0x19F
#define CORE_N_38P4		0x2F
#define CORE_FSEL_38P4		0x03
#define CORE_M2_38P4		0x01	/* M3 of 2 */

#define CORE_M_38P4_ES1		0x19F
#define CORE_N_38P4_ES1		0x2F
#define CORE_FSL_38P4_ES1	0x03
#define CORE_M2_38P4_ES1	0x01	/* M3 of 2 */

/* PER DPLL */

#define PER_M_12		0xD8
#define PER_N_12		0x05
#define PER_FSEL_12		0x07
#define PER_M2_12		0x09

#define PER_M_13		0x1B0
#define PER_N_13		0x0C
#define PER_FSEL_13		0x03
#define PER_M2_13		0x09

#define PER_M_19P2		0xE1
#define PER_N_19P2		0x09
#define PER_FSEL_19P2		0x07
#define PER_M2_19P2		0x09

#define PER_M_26		0xD8
#define PER_N_26		0x0C
#define PER_FSEL_26		0x07
#define PER_M2_26		0x09

#define PER_M_38P4		0xE1
#define PER_N_38P4		0x13
#define PER_FSEL_38P4		0x07
#define PER_M2_38P4		0x09

/* 36XX PER DPLL */

#define PER_36XX_M_12		0x1B0
#define PER_36XX_N_12		0x05
#define PER_36XX_FSEL_12	0x07
#define PER_36XX_M2_12		0x09

#define PER_36XX_M_13		0x360
#define PER_36XX_N_13		0x0C
#define PER_36XX_FSEL_13	0x03
#define PER_36XX_M2_13		0x09

#define PER_36XX_M_19P2		0x1C2
#define PER_36XX_N_19P2		0x09
#define PER_36XX_FSEL_19P2	0x07
#define PER_36XX_M2_19P2	0x09

#define PER_36XX_M_26		0x1B0
#define PER_36XX_N_26		0x0C
#define PER_36XX_FSEL_26	0x07
#define PER_36XX_M2_26		0x09

#define PER_36XX_M_38P4		0x1C2
#define PER_36XX_N_38P4		0x13
#define PER_36XX_FSEL_38P4	0x07
#define PER_36XX_M2_38P4	0x09




// Smuggled from uboot\uboot\include\asm-arch\arch-omap3\clocks.h
typedef struct {
	unsigned int m;
	unsigned int n;
	unsigned int fsel;
	unsigned int m2;
} dpll_param;

struct dpll_per_36x_param {
        unsigned int sys_clk;
        unsigned int m;
        unsigned int n;
        unsigned int m2;
        unsigned int m3;
        unsigned int m4;
        unsigned int m5;
        unsigned int m6;
        unsigned int m2div;
};

/* Following functions are exported from PreStackInit.S */
extern UINT32 wait_on_value(UINT32, UINT32, void *, UINT32);
extern dpll_param *get_mpu_dpll_param(void);
extern dpll_param *get_iva_dpll_param(void);
extern dpll_param *get_core_dpll_param(void);
extern dpll_param *get_per_dpll_param(void);

extern dpll_param *get_36x_mpu_dpll_param(void);
extern dpll_param *get_36x_iva_dpll_param(void);
extern dpll_param *get_36x_core_dpll_param(void);
extern dpll_param *get_36x_per_dpll_param(void);

extern void *_end_vect, *_start;



// Smuggled from uboot\cpu\arm-cortexa8\omap3\syslib.c
/*****************************************************************
 * sr32 - clear & set a value in a bit range for a 32 bit address
 *****************************************************************/
void sr32(void *addr, u32 start_bit, u32 num_bits, u32 value)
{
	u32 tmp, msk = 0;
	msk = 1 << num_bits;
	--msk;
	tmp = readl((u32)addr) & ~(msk << start_bit);
	tmp |= value << start_bit;
	writel(tmp, (u32)addr);
}

UINT32 wait_on_value(UINT32 read_bit_mask, UINT32 match_value, void *read_addr,
		  UINT32 bound)
{
	UINT32 i = 0, val;
	do {
		++i;
		val = readl((UINT32)read_addr) & read_bit_mask;
		if (val == match_value)
			return 1;
		if (i == bound)
			return 0;
	} while (1);
}



// The three lines here are copied verbatim from Linux\source\u-boot-03.00.00.04\u-boot-03.00.00.04\include\asm-arm\arch-omap3\omap3.h
#define SRAM_OFFSET0			0x40000000
#define SRAM_OFFSET1			0x00200000
#define SRAM_OFFSET2			0x0000F800


#define __arch_putl(v,a)		(*(volatile unsigned int *)(a) = (v))
#define __arch_getl(a)			(*(volatile unsigned int *)(a))
#define __raw_readl(a)			__arch_getl(a)
#define __raw_writel(v,a)		__arch_putl(v,a)

// The next 215 lines have been copied from uboot\include\asm-arch\arch-omap3\omap3.h and need to be checked for ip // issues before going to market

/* Stuff on L3 Interconnect */
#define SMX_APE_BASE			0x68000000

/* GPMC */
#define OMAP34XX_GPMC_BASE		0x6E000000

/* SMS */
#define OMAP34XX_SMS_BASE		0x6C000000

/* SDRC */
#define OMAP34XX_SDRC_BASE		0x6D000000

/*
 * L4 Peripherals - L4 Wakeup and L4 Core now
 */
#define OMAP34XX_CORE_L4_IO_BASE	0x48000000
#define OMAP34XX_WAKEUP_L4_IO_BASE	0x48300000
#define OMAP34XX_ID_L4_IO_BASE		0x4830A200
#define OMAP34XX_L4_PER			0x49000000
#define OMAP34XX_L4_IO_BASE		OMAP34XX_CORE_L4_IO_BASE

/* CONTROL */
#define OMAP34XX_CTRL_BASE		(OMAP34XX_L4_IO_BASE + 0x2000)

/* UART */
#define OMAP34XX_UART1			(OMAP34XX_L4_IO_BASE + 0x6a000)
#define OMAP34XX_UART2			(OMAP34XX_L4_IO_BASE + 0x6c000)
#define OMAP34XX_UART3			(OMAP34XX_L4_PER + 0x20000)

/* General Purpose Timers */
#define OMAP34XX_GPT1			0x48318000
#define OMAP34XX_GPT2			0x49032000
#define OMAP34XX_GPT3			0x49034000
#define OMAP34XX_GPT4			0x49036000
#define OMAP34XX_GPT5			0x49038000
#define OMAP34XX_GPT6			0x4903A000
#define OMAP34XX_GPT7			0x4903C000
#define OMAP34XX_GPT8			0x4903E000
#define OMAP34XX_GPT9			0x49040000
#define OMAP34XX_GPT10			0x48086000
#define OMAP34XX_GPT11			0x48088000
#define OMAP34XX_GPT12			0x48304000

/* WatchDog Timers (1 secure, 3 GP) */
#define WD1_BASE			0x4830C000
#define WD2_BASE			0x48314000
#define WD3_BASE			0x49030000

/* 32KTIMER */
#define SYNC_32KTIMER_BASE		0x48320000

// The following lines have been copied from ubuntu\include\asm-arch\arch-omap3\gpio.h
#define OMAP24XX_GPIO_REVISION		0x0000
#define OMAP24XX_GPIO_SYSCONFIG		0x0010
#define OMAP24XX_GPIO_SYSSTATUS		0x0014
#define OMAP24XX_GPIO_IRQSTATUS1	0x0018
#define OMAP24XX_GPIO_IRQSTATUS2	0x0028
#define OMAP24XX_GPIO_IRQENABLE2	0x002c
#define OMAP24XX_GPIO_IRQENABLE1	0x001c
#define OMAP24XX_GPIO_WAKE_EN		0x0020
#define OMAP24XX_GPIO_CTRL		0x0030
#define OMAP24XX_GPIO_OE		0x0034
#define OMAP24XX_GPIO_DATAIN		0x0038
#define OMAP24XX_GPIO_DATAOUT		0x003c
#define OMAP24XX_GPIO_LEVELDETECT0	0x0040
#define OMAP24XX_GPIO_LEVELDETECT1	0x0044
#define OMAP24XX_GPIO_RISINGDETECT	0x0048
#define OMAP24XX_GPIO_FALLINGDETECT	0x004c
#define OMAP24XX_GPIO_DEBOUNCE_EN	0x0050
#define OMAP24XX_GPIO_DEBOUNCE_VAL	0x0054
#define OMAP24XX_GPIO_CLEARIRQENABLE1	0x0060
#define OMAP24XX_GPIO_SETIRQENABLE1	0x0064
#define OMAP24XX_GPIO_CLEARWKUENA	0x0080
#define OMAP24XX_GPIO_SETWKUENA		0x0084
#define OMAP24XX_GPIO_CLEARDATAOUT	0x0090
#define OMAP24XX_GPIO_SETDATAOUT	0x0094


#define METHOD_GPIO_24XX	4

struct gpio_bank {
	void *base;
	int method;
};


#ifndef __ASSEMBLY__

struct s32ktimer {
	unsigned char res[0x10];
	unsigned int s32k_cr;		/* 0x10 */
};

#endif /* __ASSEMBLY__ */

/* OMAP3 GPIO registers */
#define OMAP34XX_GPIO1_BASE		0x48310000
#define OMAP34XX_GPIO2_BASE		0x49050000
#define OMAP34XX_GPIO3_BASE		0x49052000
#define OMAP34XX_GPIO4_BASE		0x49054000
#define OMAP34XX_GPIO5_BASE		0x49056000
#define OMAP34XX_GPIO6_BASE		0x49058000

#ifndef __ASSEMBLY__
struct gpio {
	unsigned char res1[0x34];
	unsigned int oe;		/* 0x34 */
	unsigned int datain;		/* 0x38 */
	unsigned char res2[0x54];
	unsigned int cleardataout;	/* 0x90 */
	unsigned int setdataout;	/* 0x94 */
};
#endif /* __ASSEMBLY__ */

#define GPIO0				(0x1 << 0)
#define GPIO1				(0x1 << 1)
#define GPIO2				(0x1 << 2)
#define GPIO3				(0x1 << 3)
#define GPIO4				(0x1 << 4)
#define GPIO5				(0x1 << 5)
#define GPIO6				(0x1 << 6)
#define GPIO7				(0x1 << 7)
#define GPIO8				(0x1 << 8)
#define GPIO9				(0x1 << 9)
#define GPIO10				(0x1 << 10)
#define GPIO11				(0x1 << 11)
#define GPIO12				(0x1 << 12)
#define GPIO13				(0x1 << 13)
#define GPIO14				(0x1 << 14)
#define GPIO15				(0x1 << 15)
#define GPIO16				(0x1 << 16)
#define GPIO17				(0x1 << 17)
#define GPIO18				(0x1 << 18)
#define GPIO19				(0x1 << 19)
#define GPIO20				(0x1 << 20)
#define GPIO21				(0x1 << 21)
#define GPIO22				(0x1 << 22)
#define GPIO23				(0x1 << 23)
#define GPIO24				(0x1 << 24)
#define GPIO25				(0x1 << 25)
#define GPIO26				(0x1 << 26)
#define GPIO27				(0x1 << 27)
#define GPIO28				(0x1 << 28)
#define GPIO29				(0x1 << 29)
#define GPIO30				(0x1 << 30)
#define GPIO31				(0x1 << 31)

/* base address for indirect vectors (internal boot mode) */
#define SRAM_OFFSET0			0x40000000
#define SRAM_OFFSET1			0x00200000
#define SRAM_OFFSET2			0x0000F800
#define SRAM_VECT_CODE			(SRAM_OFFSET0 | SRAM_OFFSET1 | \
					 SRAM_OFFSET2)

#define LOW_LEVEL_SRAM_STACK		0x4020FFFC

#define DEBUG_LED1			149	/* gpio */
#define DEBUG_LED2			150	/* gpio */

#define XDR_POP		5	/* package on package part */
#define SDR_DISCRETE	4	/* 128M memory SDR module */
#define DDR_STACKED	3	/* stacked part on 2422 */
#define DDR_COMBO	2	/* combo part on cpu daughter card */
#define DDR_DISCRETE	1	/* 2x16 parts on daughter card */

#define DDR_100		100	/* type found on most mem d-boards */
#define DDR_111		111	/* some combo parts */
#define DDR_133		133	/* most combo, some mem d-boards */
#define DDR_165		165	/* future parts */

/*
 * 343x real hardware:
 *  ES1     = rev 0
 *
 *  ES2 onwards, the value maps to contents of IDCODE register [31:28].
 *
 * Note : CPU_3XX_ES20 is used in cache.S.  Please review before changing.
 */
#define CPU_3XX_ES10		0
#define CPU_3XX_ES20		1
#define CPU_3XX_ES21		2
#define CPU_3XX_ES30		3
#define CPU_3XX_ES31		4
#define CPU_3XX_MAX_REV		(CPU_3XX_ES31 + 1)

#define CPU_3XX_ID_SHIFT	28

#define WIDTH_8BIT		0x0000
#define WIDTH_16BIT		0x1000	/* bit pos for 16 bit in gpmc */

/*
 * Hawkeye values
 */
#define HAWKEYE_OMAP34XX	0xb7ae
#define HAWKEYE_AM35XX		0xb868
#define HAWKEYE_OMAP36XX	0xb891

#define HAWKEYE_SHIFT		12

/*
 * Define CPU families
 */
#define CPU_OMAP34XX		0x3400	/* OMAP34xx/OMAP35 devices */
#define CPU_AM35XX		0x3500	/* AM35xx devices          */
#define CPU_OMAP36XX		0x3600	/* OMAP36xx devices        */

/*
 * Define CPUs
 */
#define OMAP3430		0x3430

#define OMAP3503		0x3503
#define OMAP3515		0x3515
#define OMAP3525		0x3525
#define OMAP3530		0x3530

#define AM3505			0x3505
#define AM3517			0x3517

#define OMAP3630		0x3630
#define OMAP3730		0x3730
/*
 * Control status register values corresponding to cpu variants
 */
#define CTRL_OMAP3503		0x5c00
#define CTRL_OMAP3515		0x1c00
#define CTRL_OMAP3525		0x4c00
#define CTRL_OMAP3530		0x0c00

#define CTRL_AM3505		0x5c00
#define CTRL_AM3517		0x1c00

#define CTRL_OMAP3730		0x0c00

#define MATCH_INTERRUPT_PENDING 0x1

struct TIAM3517_AITC;



struct TIAM3517_CLOCK_MANAGER_PRM
{

	static const UINT32 c_Base = 0x48306000;


	volatile UINT8 res1[0xd40];
	volatile UINT32 clksel;
	volatile UINT8 res2[0x50C];
	volatile UINT32 rstctrl;
	volatile UINT8 res3[0x1C];
	volatile UINT32 clksrc_ctrl;


};


struct TIAM3517_GPIO_Port
{

	static const UINT32 c_Base_gpio1 = 0x48310000;
	static const UINT32 c_Base_gpio2 = 0x49050000;
	static const UINT32 c_Base_gpio3 = 0x49052000;
	static const UINT32 c_Base_gpio4 = 0x49054000;
	static const UINT32 c_Base_gpio5 = 0x49056000;
	static const UINT32 c_Base_gpio6 = 0x49058000;


	volatile UINT32 GPIO_REVISION;
	volatile UINT32 GPIO_SYSCONFIG;
	volatile UINT32 GPIO_SYSSTATUS;
	volatile UINT32 GPIO_IRQSTATUS1;
	volatile UINT32 GPIO_IRQENABLE1;
	volatile UINT32 GPIO_WAKEUPENABLE;
	volatile UINT32 GPIO_IRQSTATUS2;
	volatile UINT32 GPIO_IRQENABLE2;
	volatile UINT32 GPIO_CTRL;
	volatile UINT32 GPIO_OE;
	volatile UINT32 GPIO_DATAIN;
	volatile UINT32 GPIO_DATAOUT;
	volatile UINT32 GPIO_LEVELDETECT0;
	volatile UINT32 GPIO_LEVELDETECT1;
	volatile UINT32 GPIO_RISINGDETECT;
	volatile UINT32 GPIO_FALLINGDETECT;
	volatile UINT32 GPIO_DEBOUNCEENABLE;
	volatile UINT32 GPIO_DEBOUNCINGTIME;
	volatile UINT32 GPIO_CLEARIRQENABLE1;
	volatile UINT32 GPIO_SETIRQENABLE1;
	volatile UINT32 GPIO_CLEARIRQENABLE2;
	volatile UINT32 GPIO_SETIRQENABLE2;
	volatile UINT32 GPIO_CLEARWKUENA;
	volatile UINT32 GPIO_SETWKUENA;
	volatile UINT32 GPIO_CLEARDATAOUT;
	volatile UINT32 GPIO_SETDATAOUT;
	


};

/*
struct TIAM3517_GPIO2
{

	static const UINT32 c_Base = 0x49050000;


	volatile UINT32 GPIO_REVISION;
	volatile UINT32 GPIO_SYSCONFIG;
	volatile UINT32 GPIO_SYSSTATUS;
	volatile UINT32 GPIO_IRQSTATUS1;
	volatile UINT32 GPIO_IRQENABLE1;
	volatile UINT32 GPIO_WAKEUPENABLE;
	volatile UINT32 GPIO_IRQSTATUS2;
	volatile UINT32 GPIO_IRQENABLE2;
	volatile UINT32 GPIO_CTRL;
	volatile UINT32 GPIO_OE;
	volatile UINT32 GPIO_DATAIN;
	volatile UINT32 GPIO_DATAOUT;
	volatile UINT32 GPIO_LEVELDETECT0;
	volatile UINT32 GPIO_LEVELDETECT1;
	volatile UINT32 GPIO_RISINGDETECT;
	volatile UINT32 GPIO_FALLINGDETECT;
	volatile UINT32 GPIO_DEBOUNCEENABLE;
	volatile UINT32 GPIO_DEBOUNCINGTIME;
	volatile UINT32 GPIO_CLEARIRQENABLE1;
	volatile UINT32 GPIO_SETIRQENABLE1;
	volatile UINT32 GPIO_CLEARIRQENABLE2;
	volatile UINT32 GPIO_SETIRQENABLE2;
	volatile UINT32 GPIO_CLEARWKUENA;
	volatile UINT32 GPIO_SETWKUENA;
	volatile UINT32 GPIO_CLEARDATAOUT;
	volatile UINT32 GPIO_SETDATAOUT;
	


};

struct TIAM3517_GPIO3
{

	static const UINT32 c_Base = 0x49052000;


	volatile UINT32 GPIO_REVISION;
	volatile UINT32 GPIO_SYSCONFIG;
	volatile UINT32 GPIO_SYSSTATUS;
	volatile UINT32 GPIO_IRQSTATUS1;
	volatile UINT32 GPIO_IRQENABLE1;
	volatile UINT32 GPIO_WAKEUPENABLE;
	volatile UINT32 GPIO_IRQSTATUS2;
	volatile UINT32 GPIO_IRQENABLE2;
	volatile UINT32 GPIO_CTRL;
	volatile UINT32 GPIO_OE;
	volatile UINT32 GPIO_DATAIN;
	volatile UINT32 GPIO_DATAOUT;
	volatile UINT32 GPIO_LEVELDETECT0;
	volatile UINT32 GPIO_LEVELDETECT1;
	volatile UINT32 GPIO_RISINGDETECT;
	volatile UINT32 GPIO_FALLINGDETECT;
	volatile UINT32 GPIO_DEBOUNCEENABLE;
	volatile UINT32 GPIO_DEBOUNCINGTIME;
	volatile UINT32 GPIO_CLEARIRQENABLE1;
	volatile UINT32 GPIO_SETIRQENABLE1;
	volatile UINT32 GPIO_CLEARIRQENABLE2;
	volatile UINT32 GPIO_SETIRQENABLE2;
	volatile UINT32 GPIO_CLEARWKUENA;
	volatile UINT32 GPIO_SETWKUENA;
	volatile UINT32 GPIO_CLEARDATAOUT;
	volatile UINT32 GPIO_SETDATAOUT;
	


};

struct TIAM3517_GPIO4
{

	static const UINT32 c_Base = 0x49054000;


	volatile UINT32 GPIO_REVISION;
	volatile UINT32 GPIO_SYSCONFIG;
	volatile UINT32 GPIO_SYSSTATUS;
	volatile UINT32 GPIO_IRQSTATUS1;
	volatile UINT32 GPIO_IRQENABLE1;
	volatile UINT32 GPIO_WAKEUPENABLE;
	volatile UINT32 GPIO_IRQSTATUS2;
	volatile UINT32 GPIO_IRQENABLE2;
	volatile UINT32 GPIO_CTRL;
	volatile UINT32 GPIO_OE;
	volatile UINT32 GPIO_DATAIN;
	volatile UINT32 GPIO_DATAOUT;
	volatile UINT32 GPIO_LEVELDETECT0;
	volatile UINT32 GPIO_LEVELDETECT1;
	volatile UINT32 GPIO_RISINGDETECT;
	volatile UINT32 GPIO_FALLINGDETECT;
	volatile UINT32 GPIO_DEBOUNCEENABLE;
	volatile UINT32 GPIO_DEBOUNCINGTIME;
	volatile UINT32 GPIO_CLEARIRQENABLE1;
	volatile UINT32 GPIO_SETIRQENABLE1;
	volatile UINT32 GPIO_CLEARIRQENABLE2;
	volatile UINT32 GPIO_SETIRQENABLE2;
	volatile UINT32 GPIO_CLEARWKUENA;
	volatile UINT32 GPIO_SETWKUENA;
	volatile UINT32 GPIO_CLEARDATAOUT;
	volatile UINT32 GPIO_SETDATAOUT;
	


};

struct TIAM3517_GPIO5
{

	static const UINT32 c_Base = 0x49056000;


	volatile UINT32 GPIO_REVISION;
	volatile UINT32 GPIO_SYSCONFIG;
	volatile UINT32 GPIO_SYSSTATUS;
	volatile UINT32 GPIO_IRQSTATUS1;
	volatile UINT32 GPIO_IRQENABLE1;
	volatile UINT32 GPIO_WAKEUPENABLE;
	volatile UINT32 GPIO_IRQSTATUS2;
	volatile UINT32 GPIO_IRQENABLE2;
	volatile UINT32 GPIO_CTRL;
	volatile UINT32 GPIO_OE;
	volatile UINT32 GPIO_DATAIN;
	volatile UINT32 GPIO_DATAOUT;
	volatile UINT32 GPIO_LEVELDETECT0;
	volatile UINT32 GPIO_LEVELDETECT1;
	volatile UINT32 GPIO_RISINGDETECT;
	volatile UINT32 GPIO_FALLINGDETECT;
	volatile UINT32 GPIO_DEBOUNCEENABLE;
	volatile UINT32 GPIO_DEBOUNCINGTIME;
	volatile UINT32 GPIO_CLEARIRQENABLE1;
	volatile UINT32 GPIO_SETIRQENABLE1;
	volatile UINT32 GPIO_CLEARIRQENABLE2;
	volatile UINT32 GPIO_SETIRQENABLE2;
	volatile UINT32 GPIO_CLEARWKUENA;
	volatile UINT32 GPIO_SETWKUENA;
	volatile UINT32 GPIO_CLEARDATAOUT;
	volatile UINT32 GPIO_SETDATAOUT;
	


};

struct TIAM3517_GPIO6
{

	static const UINT32 c_Base = 0x49058000;


	volatile UINT32 GPIO_REVISION;
	volatile UINT32 GPIO_SYSCONFIG;
	volatile UINT32 GPIO_SYSSTATUS;
	volatile UINT32 GPIO_IRQSTATUS1;
	volatile UINT32 GPIO_IRQENABLE1;
	volatile UINT32 GPIO_WAKEUPENABLE;
	volatile UINT32 GPIO_IRQSTATUS2;
	volatile UINT32 GPIO_IRQENABLE2;
	volatile UINT32 GPIO_CTRL;
	volatile UINT32 GPIO_OE;
	volatile UINT32 GPIO_DATAIN;
	volatile UINT32 GPIO_DATAOUT;
	volatile UINT32 GPIO_LEVELDETECT0;
	volatile UINT32 GPIO_LEVELDETECT1;
	volatile UINT32 GPIO_RISINGDETECT;
	volatile UINT32 GPIO_FALLINGDETECT;
	volatile UINT32 GPIO_DEBOUNCEENABLE;
	volatile UINT32 GPIO_DEBOUNCINGTIME;
	volatile UINT32 GPIO_CLEARIRQENABLE1;
	volatile UINT32 GPIO_SETIRQENABLE1;
	volatile UINT32 GPIO_CLEARIRQENABLE2;
	volatile UINT32 GPIO_SETIRQENABLE2;
	volatile UINT32 GPIO_CLEARWKUENA;
	volatile UINT32 GPIO_SETWKUENA;
	volatile UINT32 GPIO_CLEARDATAOUT;
	volatile UINT32 GPIO_SETDATAOUT;
	


};

struct TIAM3517_GPIO
{
	static UINT32 max_pins = 19
	static TIAM3517_GPIO &GPIO1() { return *(TIAM3517_GPIO*)(size_t)(TIAM3517_GPIO::c_Base_gpio1);}
	static TIAM3517_GPIO &GPIO2() { return *(TIAM3517_GPIO*)(size_t)(TIAM3517_GPIO::c_Base_gpio2);}
	static TIAM3517_GPIO &GPIO3() { return *(TIAM3517_GPIO*)(size_t)(TIAM3517_GPIO::c_Base_gpio3);}
	static TIAM3517_GPIO &GPIO4() { return *(TIAM3517_GPIO*)(size_t)(TIAM3517_GPIO::c_Base_gpio4);}
	static TIAM3517_GPIO &GPIO5() { return *(TIAM3517_GPIO*)(size_t)(TIAM3517_GPIO::c_Base_gpio5);}
	static TIAM3517_GPIO &GPIO6() { return *(TIAM3517_GPIO*)(size_t)(TIAM3517_GPIO::c_Base_gpio6);}


	// Accesses the revision registers and returns the revision
	UINT32 getRevision(GPIO_BANK GPIO)
	{

	}

	UINT32 getGPIOBank(GPIO_PIN PIN)
	{

		ASSERT((int) PIN <= 192);

		return ((int) PIN >> 5);

	}

	void setDataOut(GPIO_PIN PIN)
	{
		ASSERT((int)PIN <= max_pins);

		UINT32 bank = getGPIOBank(PIN);

		UINT32 bankpin = PIN - (bank - 1) * 32;

		switch(bank)
		{
			case 1: 
				GPIO1().GPIO_SETDATAOUT |= 1 << (bankpin - 1);	
				break;
			case 2:
				GPIO2().GPIO_SETDATAOUT |= 1 << (bankpin - 1);
				break;
			case 3:
				GPIO3().GPIO_SETDATAOUT |= 1 << (bankpin - 1);
				break;
			case 4:
				GPIO4().GPIO_SETDATAOUT |= 1 << (bankpin - 1);
				break;
			case 5:
				GPIO5().GPIO_SETDATAOUT |= 1 << (bankpin - 1);
				break;
			case 6:
				GPIO5().GPIO_SETDATAOUT |= 1 << (bankpin - 1);
				break;

		}

	}


};

*/

struct OCP_System_Reg_PRM
{
	volatile UINT32 REVISION;
	volatile UINT32 SYSCONFIG;
	volatile UINT32 IRQSTATUS_MPU;
	volatile UINT32 IRQENABLE_MPU;
};

struct OCP_System_Reg_CM
{
	volatile UINT32 REVISION;
	volatile UINT32 padding1[2];
	volatile BOOL padding2[2];
	volatile UINT32 SYSCONFIG;
	
};

struct MPU_PRM
{
	
	volatile UINT32	RM_RSTST_MPU;
	volatile UINT32	PM_EVGENCTRL_MPU;
	volatile UINT32	PM_EVGENONTIM_MPU;
	volatile UINT32	PM_EVGENOFFTIM_MPU;
	volatile UINT32	PM_PWSTCTRL_MPU;
	volatile UINT32	PM_PWSTST_MPU;
	volatile UINT32	PM_PREPWSTST_MPU;
};


struct TIAM3517_CLOCK_MANAGER_MPU_CM
{
	
	static const UINT32 c_Base = 0x48004900;

	volatile UINT32 CM_CLKEN_PLL_MPU;
	volatile UINT32 CM_IDLEST_MPU;   
	volatile UINT32 CM_IDLEST_PLL_MPU;	
	volatile UINT32 CM_AUTOIDLE_PLL_MPU;
	volatile UINT32 CM_CLKSEL1_PLL_MPU;
	volatile UINT32 CM_CLKSEL2_PLL_MPU;
	volatile UINT32 CM_CLKSTCTRL_MPU;
	volatile UINT32 CM_CLKSTST_MPU;



};

struct CORE_PRM
{
	volatile UINT32	RM_RSTST_CORE;;
	volatile UINT32	PM_WKEN1_CORE;
	volatile UINT32	PM_MPUGRPSEL1_CORE;
	volatile UINT32	Reserved1;
	volatile UINT32	PM_WKST1_CORE;
	volatile UINT32	PM_WKST3_CORE;
	volatile UINT32	PM_PWSTCTRL_CORE;
	volatile UINT32	PM_PWSTST_CORE;
	volatile UINT32	PM_PREPWSTST_CORE;
	volatile UINT32	PM_WKEN3_CORE;
	volatile UINT32	Reserved2;
	volatile UINT32	PM_MPUGRPSEL3_CORE;


};

struct CORE_CM
{
	volatile UINT32 CM_FCLKEN1_CORE;
	volatile UINT32 CM_FCLKEN3_CORE;
	volatile UINT32 CM_ICLKEN1_CORE;
	volatile UINT32 CM_ICLKEN2_CORE;
	volatile UINT32 CM_ICLKEN3_CORE;
	volatile UINT32 CM_IDLEST1_CORE;
	volatile UINT32 CM_IDLEST2_CORE;
	volatile UINT32 CM_IDLEST3_CORE;
	volatile UINT32 CM_AUTOIDLE1_CORE;
	volatile UINT32 CM_AUTOIDLE2_CORE;
	volatile UINT32 CM_AUTOIDLE3_CORE;
	volatile UINT32 CM_CLKSEL_CORE;
	volatile UINT32 CM_CLKSTCTRL_CORE;
	volatile UINT32 CM_CLKSTST_CORE;

};


struct SGX_PRM
{

	volatile UINT32	RM_RSTST_SGX;
	volatile UINT32	PM_WKDEP_SGX;
	volatile UINT32	PM_PWSTCTRL_SGX;
	volatile UINT32	PM_PWSTST_SGX;
	volatile UINT32	PM_PREPWSTCTRL_SGX;


};

struct SGX_CM
{
	volatile UINT32 CM_FCLKEN_SGX;
	volatile UINT32 CM_ICLKEN_SGX;
	volatile UINT32 CM_IDLEST_SGX;
	volatile UINT32 CM_CLKSEL_SG;
	volatile UINT32 CM_SLEEPDEP_SGX;
	volatile UINT32 CM_CLKSTCTRL_SGX;
	volatile UINT32 CM_CLKSTST_SGX;
	
};

struct WKUP_PRM
{
	volatile UINT32 PM_WKEN_WKUP;
	volatile UINT32 PM_MPUGRPSEL_WKUP;
	volatile UINT32 Reserved;
	volatile UINT32 PM_WKST_WKUP;

};

struct TIAM3517_CLOCK_MANAGER_WKUP_CM
{

	static const UINT32 c_Base = 0x48004C00;
	
	volatile UINT32 CM_FCLKEN_WKUP;
	volatile UINT32 CM_ICLKEN_WKUP;
	volatile UINT32 CM_IDLEST_WKUP;
	volatile UINT32 CM_AUTOIDLE_WKUP;
	volatile UINT32 CM_CLKSEL_WKUP;

};


struct Clock_Control_Reg_PRM
{
	volatile UINT32 PRM_CLKSEL;
	volatile UINT32 PRM_CLKOUT_CTRL;

};

struct Clock_Control_Reg_CM
{
	volatile UINT32 CM_CLKEN_PLL;
	volatile UINT32 CM_CLKEN2_PLL;
	volatile UINT32 CM_IDLEST_CKGEN;
	volatile UINT32 CM_IDLEST2_CKGEN;
	volatile UINT32 CM_AUTOIDLE_PLL;
	volatile UINT32 CM_AUTOIDLE2_PLL;
	volatile UINT32 CM_CLKSEL1_PLL;
	volatile UINT32 CM_CLKSEL2_PLL;
	volatile UINT32 CM_CLKSEL3_PLL;
	volatile UINT32 CM_CLKSEL4_PLL;
	volatile UINT32 CM_CLKSEL5_PLL;
	volatile UINT32 CM_CLKOUT_CTRL;
	


};

struct DSS_PRM
{
	volatile UINT32 RM_RSTST_DSS;
	volatile UINT32	PM_WKEN_DSS;
	volatile UINT32	PM_WKDEP_DSS;
	volatile UINT32	PM_PWSTCTRL_DSS;
	volatile UINT32	PM_PWSTST_DSS;
	volatile UINT32	PM_PREPWSTST_DSS;


};

struct DSS_CM
{
	volatile UINT32 CM_FCLKEN_DSS;
	volatile UINT32 CM_ICLKEN_DSS;
	volatile UINT32 CM_IDLEST_DSS;
	volatile UINT32 CM_AUTOIDLE_DSS;
	volatile UINT32 CM_CLKSEL_DSS;
	volatile UINT32 CM_SLEEPDEP_DSS;
	volatile UINT32 CM_CLKSTCTRL_DSS;
	volatile UINT32 CM_CLKSTST_DSS;
	


};


struct Reserved
{



};


struct PER_PRM
{

	volatile UINT32	RM_RSTST_PER;
	volatile UINT32	PM_WKEN_PER;
	volatile UINT32	PM_MPUGRPSEL_PER;
	volatile UINT32	Reserved;
	volatile UINT32	PM_WKST_PER;
	volatile UINT32	PM_WKDEP_PER;
	volatile UINT32	PM_PWSTCTRL_PER;
	volatile UINT32	PM_PWSTST_PER;
	volatile UINT32	PM_PREPWSTST_PER;

};

struct TIAM3517_CLOCK_MANAGER_PER_CM
{

	static const UINT32 c_Base = 0x48005000;
	
	volatile UINT32 CM_FCLKEN_PER;
	volatile UINT32 padding1;
	volatile BOOL padding2[2];
	volatile UINT32 CM_ICLKEN_PER;
	volatile UINT32 padding3;
	volatile BOOL padding4[2];
	volatile UINT32 CM_IDLEST_PER;
	volatile UINT32 padding5;
	volatile BOOL padding6[2];
	volatile UINT32 CM_AUTOIDLE_PER;
	volatile UINT32 padding7;
	volatile BOOL padding8[2];
	volatile UINT32 CM_CLKSEL_PER;
	volatile UINT32 CM_SLEEPDEP_PER;
	volatile UINT32 CM_CLKSTCTRL_PER;
	volatile UINT32 CM_CLKSTST_PER;


};


struct EMU_PRM
{
	volatile UINT32 RM_RSTST_EMU;
	volatile UINT32 Reserved;
};

struct EMU_CM
{
	volatile UINT32 CM_CLKSEL1_EMU;
	volatile UINT32 CM_CLKSTCTRL_EMU;
	volatile UINT32 CM_CLKSTST_EMU;
	volatile UINT32 CM_CLKSEL2_EMU;
	volatile UINT32 CM_CLKSEL3_EMU;
	

};

struct Global_Reg_PRM
{
	volatile UINT32 Reserved1;
	volatile UINT32 Reserved2;
	volatile UINT32 Reserved3;
	volatile UINT32 Reserved4;
	volatile UINT32 Reserved5;
	volatile UINT32 Reserved6;
	volatile UINT32 Reserved7;
	volatile UINT32 Reserved8;
	volatile UINT32 PRM_RSTCTRL;
	volatile UINT32 PRM_RSTTIME;
	volatile UINT32 PRM_RSTST;
	volatile UINT32 Reserved9;
	volatile UINT32 Reserved10;
	volatile UINT32 PRM_CLKSRC_CTRL;
	volatile UINT32 PRM_OBS;
	volatile UINT32 Reserved11;
	volatile UINT32 Reserved12;
	volatile UINT32 PRM_CLKSETUP;
	volatile UINT32 PRM_POLCTRL;
	volatile UINT32 Reserved13;

};

struct Global_Reg_CM
{
	volatile UINT32 CM_POLCTRL;
};

struct NEON_PRM
{
	volatile UINT32 RM_RSTST_NEON;
	volatile UINT32 PM_WKDEP_NEON;
	volatile UINT32 PM_PWSTCTRL_NEON;
	volatile UINT32 PM_PWSTST_NEON;
	volatile UINT32 PM_PREPWSTST_NEON;
};

struct NEON_CM
{
	volatile UINT32 CM_IDLEST_NEON;
	volatile UINT32 CM_CLKSTCTRL_NEON;

};

struct USBHOST_PRM
{
	volatile UINT32 RM_RSTST_USBHOST;
	volatile UINT32 PM_WKEN_USBHOST;
	volatile UINT32 PM_MPUGRPSEL_USBHOST;
	volatile UINT32 Reserved1;
	volatile UINT32 PM_WKST_USBHOST;
	volatile UINT32 PM_WKDEP_USBHOST;
	volatile UINT32 PM_PWSTCTRL_USBHOST;
	volatile UINT32 PM_PWSTST_USBHOST;
	volatile UINT32 PM_PREPWSTST_USBHOST;
};

struct USBHOST_CM
{
	volatile UINT32 CM_FCLKEN_USBHOST;
	volatile UINT32 CM_ICLKEN_USBHOST;
	volatile UINT32 CM_IDLEST_USBHOST;
	volatile UINT32 CM_AUTOIDLE_USBHOST;
	volatile UINT32 CM_SLEEPDEP_USBHOST;
	volatile UINT32 CM_CLKSTCTRL_USBHOST;
	volatile UINT32 CM_CLKSTST_USBHOST;

};

/*
struct TIAM3517_CLOCK_MANAGER_PRM
{
     static const UINT32 c_Base = 0x48306000;

     volatile Reserved r1;
     volatile OCP_System_Reg_PRM OSR;
     volatile MPU_PRM MPU;
     volatile CORE_PRM CORE;
     volatile SGX_PRM SGX;
     volatile WKUP_PRM WKUP;
     volatile Clock_Control_Reg_PRM CCR;
     volatile DSS_PRM DSS;
     volatile Reserved r2;
     volatile PER_PRM PER;
     volatile EMU_PRM EMU;
     volatile Global_Reg_PRM GR;
     volatile NEON_PRM NEON;
     volatile USBHOST_PRM USBHOST;



};
*/

/*
struct TIAM3517_CLOCK_MANAGER_CM
{
	static const UINT32 c_Base = 0x48004000;
	
	volatile Reserved R1;
	volatile OCP_System_Reg_CM OSR;
     volatile MPU_CM MPU;
     volatile CORE_CM CORE;
     volatile SGX_CM SGX;
     volatile WKUP_CM WKUP;
     volatile Clock_Control_Reg_CM CCR;
     volatile DSS_CM DSS;
     volatile Reserved r2;
     volatile PER_CM PER;
     volatile EMU_CM EMU;
     volatile Global_Reg_CM GR;
     volatile NEON_CM NEON;
     volatile USBHOST_CM USBHOST;
	

};
*/

struct TIAM3517_AITC_Driver
{

	static const UINT32 c_VECTORING_GUARD = 97; 

	struct IRQ_VECTORING
	{
		UINT32 Priority;
		HAL_CALLBACK Handler;
	};

	static IRQ_VECTORING s_IsrTable[];

	static void Initialize();

	static BOOL ActivateInterrupt(UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param);

	static BOOL InterruptEnable(UINT32 Irq_Index);

	static BOOL InterruptDisable(UINT32 Irq_Index);

	static BOOL InterruptEnableState(UINT32 Irq_Index);

	static BOOL InterruptState(UINT32 Irq_Index);

	static IRQ_VECTORING* IRQToIRQVector(UINT32 IRQ);

	static BOOL DeactivateInterrupt(UINT32 IRQ);


private:

	static void STUB_ISRVector(void * Param);

};

//static UINT32 c_Base;

struct TIAM3517_TIMER
{
	//static UINT32 c_Base[] = {0x48318000,0x49032000,0x49034000,0x49036000,0x49038000,0x4903A000,0x4903C000,0x4903E000,0x49040000,0x48086000,0x48088000};
	public:
	//static const UINT32 c_Base[0] = 1;

	
	 volatile UINT32 TIDR;     // contains the IP Revision code
	 volatile UINT32 padding1[3];
	 volatile UINT32 TIOCP_CFG; // controls the parameters of the GP timer L4 interface
	 volatile UINT32 TISTAT;  // provides status information of the module
	 volatile UINT32 TISR; // shows which interrupts are pending inside the module
	 volatile UINT32 TIER; // controls (enables/disable)  the interrupt events
	 volatile UINT32 TWER;
	 volatile UINT32 TCLR;
	 volatile UINT32 TCRR;
	 volatile UINT32 TLDR;
	 volatile UINT32 TTGR;
	 volatile UINT32 TWPS;
	 volatile UINT32 TMAR;
	 volatile UINT32 TCAR1;
	 volatile UINT32 TSICR;
	 volatile UINT32 TCAR2;
	 volatile UINT32 TPIR;
	 volatile UINT32 TNIR;
	 volatile UINT32 TCVR;
	 volatile UINT32 TOCR;
	 volatile UINT32 TOWR;
	
	
	
};


// Removed from TIAM3517_TIMER 
static UINT32 getBase(int Timer)
{

	  //c_Base[] = {0x48318000,0x49032000,0x49034000,0x49036000,0x49038000,0x4903A000,0x4903C000,0x4903E000,0x49040000,0x48086000,0x48088000};
		int c_Base;
		
		switch(Timer)
		{
			case 0:
				c_Base = 0x48318000;
				break;
			case 1:
				c_Base = 0x49032000;
				break;
			case 2:
				c_Base = 0x49034000;
				break;
			case 3:
				c_Base = 0x49036000;
				break;
			case 4:
				c_Base = 0x49038000;
				break;
			case 5:
				c_Base = 0x4903A000;
				break;
			case 6:
				c_Base = 0x4903C000;
				break;
			case 7:
				c_Base = 0x4903E000;
				break;
			case 8:
				c_Base = 0x49040000;
				break;
			case 9:
				c_Base = 0x48086000;
				break;
			case 10:
				c_Base = 0x48088000;
				break;
		}
		
		return c_Base;
}	


struct TIAM3517_TIMER_Driver
{
	
	static const UINT32 c_Max_Timers = 11;

	static BOOL Initialize	(UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 externalSync, HAL_CALLBACK_FPN ISR, void* ISR_Param);
	
	static BOOL Uninitialize(UINT32 Timer);
	
	static UINT32 ReadCounter(UINT32 Timer);
	
	static void EnableCompareInterrupt(UINT32 Timer)
	{
		ASSERT(Timer <= c_Max_Timers);

		TIAM3517_TIMER &TIMER = getTimer(Timer);

		TIMER.TIER |= 1;

	}

	static void DisableCompareInterrupt(UINT32 Timer)
	{
		ASSERT(Timer <= c_Max_Timers);

		TIAM3517_TIMER &TIMER = getTimer(Timer);

		TIMER.TIER &= ~1;
	
	}

	static void ForceInterrupt(UINT32 Timer)
	{


	}

	static void SetCompare(UINT32 Timer, UINT32 Compare)
	{

		ASSERT(Timer <= c_Max_Timers);

		TIAM3517_TIMER &TIMER = getTimer(Timer);
		
		TIMER.TMAR = Compare;

	}

	static UINT32 GetCompare(UINT32 Timer)
	{

		ASSERT(Timer <= c_Max_Timers);

		TIAM3517_TIMER &TIMER = getTimer(Timer);
		
		return TIMER.TMAR;

	}

	static void SetCounter(UINT32 Timer, UINT32 Count)
	{

		ASSERT(Timer <= c_Max_Timers);

		TIAM3517_TIMER &TIMER = getTimer(Timer);

		TIMER.TCRR = Count;

	}

	static BOOL DidCompareHit(UINT32 Timer)
	{

		ASSERT(Timer <= c_Max_Timers);

		TIAM3517_TIMER &TIMER = getTimer(Timer);

		if((TIMER.TISR & 1) == MATCH_INTERRUPT_PENDING)
			return TRUE;

		return FALSE;


	}

	static void ResetCompareHit(UINT32 Timer)
	{

		ASSERT(Timer <= c_Max_Timers);

		TIAM3517_TIMER &TIMER = getTimer(Timer);

		TIMER.TISR |= 1;

	}


	private:

		struct Descriptors
		{
			HAL_CALLBACK_FPN isr;
			void* arg;
			BOOL configured;
		};
		Descriptors m_descriptors[c_Max_Timers];

		static void ISR_Default(void* param);


		static TIAM3517_TIMER& getTimer(UINT32 Timer)
		{
			return *(TIAM3517_TIMER *) (size_t)(getBase(Timer));
			//return;
		}
	
};

extern TIAM3517_TIMER_Driver g_TIAM3517_TIMER_Driver;



struct TIAM3517_AITC_RegisterGroup
{
	volatile UINT32 INTCPS_ITR;
	volatile UINT32 INTCPS_MIR;
	volatile UINT32 INTCPS_MIR_CLEAR;
	volatile UINT32 INTCPS_MIR_SET;
	volatile UINT32 INTCPS_ISR_SET;
	volatile UINT32 INTCPS_ISR_CLEAR;
	volatile UINT32 INTCPS_PENDING_IRQ;
	volatile UINT32 INTCPS_PENDING_FIQ;


};

struct TIAM3517_AITC
{
	
	volatile UINT32 padding0;
	volatile UINT16 padding1;
	volatile BOOL padding10[2];
	volatile UINT32 INTCPS_SYSCONFIG;
	volatile UINT32 INTCPS_SYSSTATUS;
	volatile UINT32 padding2[10];
	volatile UINT32 INTCPS_SIR_IRQ;
	volatile UINT32 INTCPS_SIR_FIQ;
	volatile UINT32 INTCPS_CONTROL;
	volatile UINT32 INTCPS_PROTECTION;
	volatile UINT32 INTCPS_IDLE;
	volatile UINT32 padding3[3];
	volatile UINT32 INTCPS_IRQ_PRIORITY;
	volatile UINT32 INTCPS_FIQ_PRIORITY;
	volatile UINT32 INTCPS_THRESHOLD;
	volatile UINT32 padding4[3];
	volatile BOOL padding5[2];
	volatile TIAM3517_AITC_RegisterGroup Reg[3];
	volatile UINT32 padding6[8];
	volatile UINT32 INTCPS_ILR[95];
	volatile UINT32 INTC_INIT_REGISTER1;
	volatile UINT32 INTC_INIT_REGISTER2;



	static const UINT32 c_Base = 0x48200000;

	static const UINT32 c_IRQ_INDEX_EMUINT  =  0;
	static const UINT32 c_IRQ_INDEX_COMMTX = 1;
	static const UINT32 c_IRQ_INDEX_COMMRX = 2;
	static const UINT32 c_IRQ_INDEX_BENCH = 3;
	static const UINT32 c_IRQ_INDEX_MCBSP2_ST_IRQ = 4; 
	static const UINT32 c_IRQ_INDEX_MCBSP3_ST_IRQ = 5;
	static const UINT32 c_IRQ_INDEX_Reserved0  = 6;
	static const UINT32 c_IRQ_INDEX_sysnirq  =  7;
	static const UINT32 c_IRQ_INDEX_Reserved1  =  8;
	static const UINT32 c_IRQ_INDEX_SMX_DBG  = 9;
	static const UINT32 c_IRQ_INDEX_SMX_APP  = 10;
	static const UINT32 c_IRQ_INDEX_PRCM_MPU  =  11;
	static const UINT32 c_IRQ_INDEX_SDMA_0  =  12;
	static const UINT32 c_IRQ_INDEX_SDMA_1  =  13;
	static const UINT32 c_IRQ_INDEX_SDMA_2  =  14;
	static const UINT32 c_IRQ_INDEX_SDMA_3  =  15;
	static const UINT32 c_IRQ_INDEX_MCBSP1  =  16;
	static const UINT32 c_IRQ_INDEX_MCBSP2  =  17;
	static const UINT32 c_IRQ_INDEX_SR1  = 18;
	static const UINT32 c_IRQ_INDEX_Reserved2  = 19;
	static const UINT32 c_IRQ_INDEX_GPMC = 20;
	static const UINT32 c_IRQ_INDEX_SGX = 21;
	static const UINT32 c_IRQ_INDEX_MCBSP3  = 22;
	static const UINT32 c_IRQ_INDEX_MCBSP4  = 23;
	static const UINT32 c_IRQ_INDEX_HECCOINT  = 24; 
	static const UINT32 c_IRQ_INDEX_DSS_IRQ  = 25;
	static const UINT32 c_IRQ_INDEX_Reserved3  = 26;
	static const UINT32 c_IRQ_INDEX_MCBSP5  = 27;
	static const UINT32 c_IRQ_INDEX_HECC1INT  = 28;
	static const UINT32 c_IRQ_INDEX_GPIO1_MPU  = 29;
	static const UINT32 c_IRQ_INDEX_GPIO2_MPU  = 30;
	static const UINT32 c_IRQ_INDEX_GPIO3_MPU  = 31;
	static const UINT32 c_IRQ_INDEX_GPIO4_MPU  = 32;
	static const UINT32 c_IRQ_INDEX_GPIO5_MPU  = 33;
	static const UINT32 c_IRQ_INDEX_GPIO6_MPU  = 34;
	static const UINT32 c_IRQ_INDEX_Reserved4  = 35;
	static const UINT32 c_IRQ_INDEX_WDT3  = 36;
	static const UINT32 c_IRQ_INDEX_GPT1  = 37;
	static const UINT32 c_IRQ_INDEX_GPT2  = 38;
	static const UINT32 c_IRQ_INDEX_GPT3  = 39;
	static const UINT32 c_IRQ_INDEX_GPT4  = 40;
	static const UINT32 c_IRQ_INDEX_GPT5  = 41;
	static const UINT32 c_IRQ_INDEX_GPT6  = 42;
	static const UINT32 c_IRQ_INDEX_GPT7  = 43;
	static const UINT32 c_IRQ_INDEX_GPT8  = 44;
	static const UINT32 c_IRQ_INDEX_GPT9  = 45;
	static const UINT32 c_IRQ_INDEX_GPT10  = 46;
	static const UINT32 c_IRQ_INDEX_GPT11  = 47;
	static const UINT32 c_IRQ_INDEX_SPI4  = 48;
	static const UINT32 c_IRQ_INDEX_SHA1MD5  = 49;
	static const UINT32 c_IRQ_INDEX_FPKA_IRQREADY_N  = 50;
	static const UINT32 c_IRQ_INDEX_SHA2MD5  = 51;
	static const UINT32 c_IRQ_INDEX_RNG  = 52;
	static const UINT32 c_IRQ_INDEX_EMIF4ERR  = 53;
	static const UINT32 c_IRQ_INDEX_MCBSP4_TX  = 54;
	static const UINT32 c_IRQ_INDEX_MCBSP4_RX  = 55;
	static const UINT32 c_IRQ_INDEX_I2C1  = 56;
	static const UINT32 c_IRQ_INDEX_I2C2  = 57; 
	static const UINT32 c_IRQ_INDEX_HDQ  = 58;
	static const UINT32 c_IRQ_INDEX_MCBSP1_TX  = 59;
	static const UINT32 c_IRQ_INDEX_MCBSP1_RX  = 60;
	static const UINT32 c_IRQ_INDEX_I2C3 = 61;
	static const UINT32 c_IRQ_INDEX_MCBSP2_IRQ_TX = 62;
	static const UINT32 c_IRQ_INDEX_MCBSP2_IRQ_RX = 63;
	static const UINT32 c_IRQ_INDEX_FPKA_IRQERROR_N  = 64;
	static const UINT32 c_IRQ_INDEX_SP1  = 65;
	static const UINT32 c_IRQ_INDEX_SP2  = 66;
	static const UINT32 c_IRQ_INDEX_CO_RX_THRESH_PULSE  = 67;
	static const UINT32 c_IRQ_INDEX_CO_RX_PULSE  = 68;
	static const UINT32 c_IRQ_INDEX_CO_TX_PULSE  = 69;
	static const UINT32 c_IRQ_INDEX_CO_MISC_PULSE  = 70;
	static const UINT32 c_IRQ_INDEX_USBOTG_INPUT  = 71;
	static const UINT32 c_IRQ_INDEX_UART1  = 72;
	static const UINT32 c_IRQ_INDEX_UART2  = 73;
	static const UINT32 c_IRQ_INDEX_UART3  = 74;
	static const UINT32 c_IRQ_INDEX_PBIAS  = 75;
	static const UINT32 c_IRQ_INDEX_OHCI  =  76;
	static const UINT32 c_IRQ_INDEX_EHCI  =  77;
	static const UINT32 c_IRQ_INDEX_TLL  =   78;
	static const UINT32 c_IRQ_INDEX_PARTHASH  = 79;
	static const UINT32 c_IRQ_INDEX_Reserved5  = 80;
	static const UINT32 c_IRQ_INDEX_MCBSP5_TX  = 81;
	static const UINT32 c_IRQ_INDEX_MCBSP5_RX  = 82;
	static const UINT32 c_IRQ_INDEX_MMC1  = 83;
	static const UINT32 c_IRQ_INDEX_UART4  = 84;
	static const UINT32 c_IRQ_INDEX_Reserved6  = 85;
	static const UINT32 c_IRQ_INDEX_MMC2  = 86;
	static const UINT32 c_IRQ_INDEX_MPU_ICR  = 87;
	static const UINT32 c_IRQ_INDEX_CCDC_VD0_INT  = 88;
	static const UINT32 c_IRQ_INDEX_MCBSP3_TX  = 89;
	static const UINT32 c_IRQ_INDEX_MCBSP3_RX  = 90;
	static const UINT32 c_IRQ_INDEX_SPI3  = 91;
	static const UINT32 c_IRQ_INDEX_CCDC_VD1  = 92;
	static const UINT32 c_IRQ_INDEX_CCDC_VD2  = 93;
	static const UINT32 c_IRQ_INDEX_MMC3  = 94;
	static const UINT32 c_IRQ_INDEX_GPT12  = 95;


	static const UINT32 c_IRQ_Priority_0 = 0;
	static const UINT32 c_IRQ_Priority_1 = 1;
	static const UINT32 c_IRQ_Priority_2 = 2;
	static const UINT32 c_IRQ_Priority_3 = 3;
	static const UINT32 c_IRQ_Priority_4 = 4;
	static const UINT32 c_IRQ_Priority_5 = 5;
	static const UINT32 c_IRQ_Priority_6 = 6;
	static const UINT32 c_IRQ_Priority_7 = 7;
	static const UINT32 c_IRQ_Priority_8 = 8;
	static const UINT32 c_IRQ_Priority_9 = 9;
	static const UINT32 c_IRQ_Priority_10 = 10;
	static const UINT32 c_IRQ_Priority_11 = 11;
	static const UINT32 c_IRQ_Priority_12 = 12;
	static const UINT32 c_IRQ_Priority_13 = 13;
	static const UINT32 c_IRQ_Priority_14 = 14;
	static const UINT32 c_IRQ_Priority_15 = 15;
	static const UINT32 c_IRQ_Priority_16 = 16;
	static const UINT32 c_IRQ_Priority_17 = 17;
	static const UINT32 c_IRQ_Priority_18 = 18;
	static const UINT32 c_IRQ_Priority_19 = 19;
	static const UINT32 c_IRQ_Priority_20 = 20;
	static const UINT32 c_IRQ_Priority_21 = 21;
	static const UINT32 c_IRQ_Priority_22 = 22;
	static const UINT32 c_IRQ_Priority_23 = 23;
	static const UINT32 c_IRQ_Priority_24 = 24;
	static const UINT32 c_IRQ_Priority_25 = 25;
	static const UINT32 c_IRQ_Priority_26 = 26;
	static const UINT32 c_IRQ_Priority_27 = 27;
	static const UINT32 c_IRQ_Priority_28 = 28;
	static const UINT32 c_IRQ_Priority_29 = 29;
	static const UINT32 c_IRQ_Priority_30 = 30;
	static const UINT32 c_IRQ_Priority_31 = 31;
	static const UINT32 c_IRQ_Priority_32 = 32;
	static const UINT32 c_IRQ_Priority_33 = 33;
	static const UINT32 c_IRQ_Priority_34 = 34;
	static const UINT32 c_IRQ_Priority_35 = 35;
	static const UINT32 c_IRQ_Priority_36 = 36;
	static const UINT32 c_IRQ_Priority_37 = 37;
	static const UINT32 c_IRQ_Priority_38 = 38;
	static const UINT32 c_IRQ_Priority_39 = 39;
	static const UINT32 c_IRQ_Priority_40 = 40;
	static const UINT32 c_IRQ_Priority_41 = 41;
	static const UINT32 c_IRQ_Priority_42 = 42;
	static const UINT32 c_IRQ_Priority_43 = 43;
	static const UINT32 c_IRQ_Priority_44 = 44;
	static const UINT32 c_IRQ_Priority_45 = 45;
	static const UINT32 c_IRQ_Priority_46 = 46;
	static const UINT32 c_IRQ_Priority_47 = 47;
	static const UINT32 c_IRQ_Priority_48 = 48;
	static const UINT32 c_IRQ_Priority_49 = 49;
	static const UINT32 c_IRQ_Priority_50 = 50;
	static const UINT32 c_IRQ_Priority_51 = 51;
	static const UINT32 c_IRQ_Priority_52 = 52;
	static const UINT32 c_IRQ_Priority_53 = 53;
	static const UINT32 c_IRQ_Priority_54 = 54;
	static const UINT32 c_IRQ_Priority_55 = 55;
	static const UINT32 c_IRQ_Priority_56 = 56;
	static const UINT32 c_IRQ_Priority_57 = 57;
	static const UINT32 c_IRQ_Priority_58 = 58;
	static const UINT32 c_IRQ_Priority_59 = 59;
	static const UINT32 c_IRQ_Priority_60 = 60;
	static const UINT32 c_IRQ_Priority_61 = 61;
	static const UINT32 c_IRQ_Priority_62 = 62;
	static const UINT32 c_IRQ_Priority_63 = 63;
	static const UINT32 c_IRQ_Priority_64 = 64;
	static const UINT32 c_IRQ_Priority_65 = 65;
	static const UINT32 c_IRQ_Priority_66 = 66;
	static const UINT32 c_IRQ_Priority_67 = 67;
	static const UINT32 c_IRQ_Priority_68 = 68;
	static const UINT32 c_IRQ_Priority_69 = 69;
	static const UINT32 c_IRQ_Priority_70 = 70;
	static const UINT32 c_IRQ_Priority_71 = 71;
	static const UINT32 c_IRQ_Priority_72 = 72;
	static const UINT32 c_IRQ_Priority_73 = 73;
	static const UINT32 c_IRQ_Priority_74 = 74;
	static const UINT32 c_IRQ_Priority_75 = 75;
	static const UINT32 c_IRQ_Priority_76 = 76;
	static const UINT32 c_IRQ_Priority_77 = 77;
	static const UINT32 c_IRQ_Priority_78 = 78;
	static const UINT32 c_IRQ_Priority_79 = 79;
	static const UINT32 c_IRQ_Priority_80 = 80;
	static const UINT32 c_IRQ_Priority_81 = 81;
	static const UINT32 c_IRQ_Priority_82 = 82;
	static const UINT32 c_IRQ_Priority_83 = 83;
	static const UINT32 c_IRQ_Priority_84 = 84;
	static const UINT32 c_IRQ_Priority_85 = 85;
	static const UINT32 c_IRQ_Priority_86 = 86;
	static const UINT32 c_IRQ_Priority_87 = 87;
	static const UINT32 c_IRQ_Priority_88 = 88;
	static const UINT32 c_IRQ_Priority_89 = 89;
	static const UINT32 c_IRQ_Priority_90 = 90;
	static const UINT32 c_IRQ_Priority_91 = 91;
	static const UINT32 c_IRQ_Priority_92 = 92;
	static const UINT32 c_IRQ_Priority_93 = 93;
	static const UINT32 c_IRQ_Priority_94 = 94;
	static const UINT32 c_IRQ_Priority_95 = 95;
	static const UINT32 c_IRQ_Priority_96 = 96;
	static const UINT32 c_IRQ_Priority_97 = 97;

	static const UINT32 c_Max_InterruptIndex = 97;


	void DisableInterrupt(UINT32 Irq_Index)
	{
		ASSERT(Irq_Index <= c_Max_InterruptIndex);
		Reg[(Irq_Index >> 5) - 1].INTCPS_MIR |= (1 << (Irq_Index & 31));	
	}

	void EnableInterrupt(UINT32 Irq_Index)
	{
		ASSERT(Irq_Index <= c_Max_InterruptIndex);
		Reg[(Irq_Index >> 5) - 1].INTCPS_MIR_CLEAR |= (1 << (Irq_Index & 31));	
	}

	BOOL IsInterruptEnabled(UINT32 Irq_Index)
	{
		
		ASSERT(Irq_Index <= c_Max_InterruptIndex);
		if(Reg[(Irq_Index >> 5)].INTCPS_MIR & (1 << (Irq_Index & 31)))
				return TRUE;

		return FALSE;
	}

	BOOL GetInterruptState(UINT32 Irq_Index)
	{
		ASSERT(Irq_Index <= c_Max_InterruptIndex);
		if(Reg[(Irq_Index >> 5)].INTCPS_PENDING_IRQ & (1 << (Irq_Index & 31)))
			return TRUE;
		return FALSE;

	}

	UINT32 NormalInterruptPending()
	{
		return (INTCPS_SIR_IRQ & 127);
	}

	UINT32 FastInterruptPending()
	{

		return (INTCPS_SIR_FIQ & 127);

	}
	
	BOOL IsInterruptPending()
    	{
		if((NormalInterruptPending() <= c_Max_InterruptIndex))
			return TRUE;

		return FALSE;
    	}

    	void SetType( UINT32 Irq_Index, BOOL Fast )
    	{
		if(TRUE == Fast)
		{
			INTCPS_ILR[Irq_Index] |= TIAM3517_INTC_FIQROUTE;
		}
		else
			INTCPS_ILR[Irq_Index] |= TIAM3517_INTC_IRQROUTE;

	}

    	void SetPriority( UINT32 Irq_Index, UINT32 priority )
    	{
		INTCPS_ILR[Irq_Index] |= (Irq_Index << 2);

	}

        UINT32 GetPriority( UINT32 Irq_Index )
    	{
		return INTCPS_ILR[Irq_Index];
      
        }
    


   	void ForceInterrupt( UINT32 Irq_Index )
    	{
		ASSERT(Irq_Index <= c_Max_InterruptIndex);
		Reg[(Irq_Index >> 5)].INTCPS_ISR_SET |= (1 << (Irq_Index & 31));

    	}

   	void RemoveForcedInterrupt( UINT32 Irq_Index )
    	{

		ASSERT(Irq_Index <= c_Max_InterruptIndex);
		Reg[(Irq_Index >> 5)].INTCPS_ISR_CLEAR |= (1 << (Irq_Index & 31));
    	}
		

};

// 
// Name :  GPIO Driver
//
// Description : Contains the initialize and setpinstate functions

struct TIAM3517_GPIO_Driver
{

	/*
	
	struct PIN_ISR_DESCRIPTOR
	{

		 HAL_COMPLETION                 m_completion;
		 GPIO_INTERRUPT_SERVICE_ROUTINE m_isr;
		 void*                          m_param;
		 GPIO_PIN                       m_pin;
		 GPIO_INT_EDGE                  m_intEdge;
		 UINT8                          m_flags;
		 UINT8                          m_status;
		



	};
	
    */

	static BOOL Initialize();

	static void SetPinState(GPIO_PIN Pin, BOOL PinState);

	static void EnableOutputPin(GPIO_PIN Pin, BOOL initialState);

};


struct TIAM3517
{
	static TIAM3517_AITC &AITC () { return *(TIAM3517_AITC *) (size_t)(TIAM3517_AITC::c_Base);}

	static TIAM3517_CLOCK_MANAGER_PRM &CMGRP () { return *(TIAM3517_CLOCK_MANAGER_PRM *) (size_t)(TIAM3517_CLOCK_MANAGER_PRM::c_Base);}
	//static TIAM3517_CLOCK_MANAGER_CM &CMGRC () { return *(TIAM3517_CLOCK_MANAGER_CM *) (size_t)(TIAM3517_CLOCK_MANAGER_CM::c_Base);}
	static TIAM3517_CLOCK_MANAGER_PER_CM &CMGRPCM () { return *(TIAM3517_CLOCK_MANAGER_PER_CM *) (size_t)(TIAM3517_CLOCK_MANAGER_PER_CM::c_Base); }
	static TIAM3517_CLOCK_MANAGER_WKUP_CM &CMGRWCM () { return *(TIAM3517_CLOCK_MANAGER_WKUP_CM *) (size_t) (TIAM3517_CLOCK_MANAGER_WKUP_CM::c_Base); }
	static TIAM3517_CLOCK_MANAGER_MPU_CM &CMGRMPUCM () { return *(TIAM3517_CLOCK_MANAGER_MPU_CM *) (size_t) (TIAM3517_CLOCK_MANAGER_MPU_CM::c_Base); }

};
#endif
