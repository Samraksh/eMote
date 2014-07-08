@
@  File Name : TIAM3517_prestackinit.s
@
@  Author    : Nived.Sivadas@Samraksh.com
@
@  Description : Contains low level initialization of pll and clocks
@

#include "..\..\TIAM3517.h"

	.global PreStackInit

	.extern s_init

	.global mpu_36x_dpll_param

	.global iva_36x_dpll_param

	.global core_36x_dpll_param

	.global per_36x_dpll_param

	.global enableINTC


	.arm

macroIntc : .word 0x80

enableINTC:
	mrs r0, cpsr
	bic r1, r0,#0x80
	msr  cpsr, r1
   	mov  pc, lr


MPU_M_12_ES1: .word	0x0FE
 MPU_N_12_ES1: .word		0x07
 MPU_FSEL_12_ES1: .word		0x05
 MPU_M2_12_ES1: .word		0x01

 MPU_M_12_ES2: .word		0x0FA
 MPU_N_12_ES2: .word		0x05
 MPU_FSEL_12_ES2: .word		0x07
 MPU_M2_ES2: .word		0x01

 MPU_M_12: .word		0x085
 MPU_N_12: .word		0x05
 MPU_FSEL_12: .word		0x07
 MPU_M2_12: .word		0x01

 MPU_M_13_ES1: .word		0x17D
 MPU_N_13_ES1: .word		0x0C
 MPU_FSEL_13_ES1: .word		0x03
 MPU_M2_13_ES1: .word		0x01

 MPU_M_13_ES2: .word		0x1F4
 MPU_N_13_ES2: .word		0x0C
 MPU_FSEL_13_ES2: .word		0x03
 MPU_M2_13_ES2: .word		0x01

 MPU_M_13: .word		0x10A
 MPU_N_13: .word		0x0C
 MPU_FSEL_13: .word		0x03
 MPU_M2_13: .word		0x01

 MPU_M_19P2_ES1: .word		0x179
 MPU_N_19P2_ES1: .word		0x12
 MPU_FSEL_19P2_ES1: .word	0x04
 MPU_M2_19P2_ES1: .word		0x01

 MPU_M_19P2_ES2: .word		0x271
 MPU_N_19P2_ES2: .word		0x17
 MPU_FSEL_19P2_ES2: .word	0x03
 MPU_M2_19P2_ES2: .word		0x01

 MPU_M_19P2: .word		0x14C
 MPU_N_19P2: .word		0x17
 MPU_FSEL_19P2	: .word	0x03
 MPU_M2_19P2: .word		0x01

 MPU_M_26_ES1: .word		0x17D
 MPU_N_26_ES1: .word		0x19
 MPU_FSEL_26_ES1: .word		0x03
 MPU_M2_26_ES1: .word		0x01

 MPU_M_26_ES2: .word		0x0FA
 MPU_N_26_ES2: .word		0x0C
 MPU_FSEL_26_ES2: .word		0x07
 MPU_M2_26_ES2: .word		0x01

 MPU_M_26: .word		0x085
 MPU_N_26: .word		0x0C
 MPU_FSEL_26: .word		0x07
 MPU_M2_26: .word		0x01

 MPU_M_38P4_ES1: .word		0x1FA
 MPU_N_38P4_ES1: .word		0x32
 MPU_FSEL_38P4_ES1: .word	0x03
 MPU_M2_38P4_ES1: .word		0x01

 MPU_M_38P4_ES2: .word		0x271
 MPU_N_38P4_ES2: .word		0x2F
 MPU_FSEL_38P4_ES2: .word	0x03
 MPU_M2_38P4_ES2: .word		0x01

 MPU_M_38P4: .word		0x14C
 MPU_N_38P4: .word		0x2F
 MPU_FSEL_38P4: .word		0x03
 MPU_M2_38P4: .word		0x01

/* IVA DPLL */

 IVA_M_12_ES1: .word		0x07D
 IVA_N_12_ES1: .word		0x05
 IVA_FSEL_12_ES1: .word		0x07
 IVA_M2_12_ES1: .word		0x01

 IVA_M_12_ES2: .word		0x0B4
 IVA_N_12_ES2: .word		0x05
 IVA_FSEL_12_ES2: .word		0x07
 IVA_M2_12_ES2: .word		0x01

 IVA_M_12: .word		0x085
 IVA_N_12: .word		0x05
 IVA_FSEL_12: .word		0x07
 IVA_M2_12: .word		0x01

 IVA_M_13_ES1: .word		0x0FA
 IVA_N_13_ES1: .word		0x0C
 IVA_FSEL_13_ES1: .word		0x03
 IVA_M2_13_ES1	: .word	0x01

 IVA_M_13_ES2: .word		0x168
 IVA_N_13_ES2: .word		0x0C
 IVA_FSEL_13_ES2: .word		0x03
 IVA_M2_13_ES2: .word		0x01

 IVA_M_13: .word		0x10A
 IVA_N_13: .word		0x0C
 IVA_FSEL_13: .word		0x03
 IVA_M2_13: .word		0x01

 IVA_M_19P2_ES1: .word		0x082
 IVA_N_19P2_ES1: .word		0x09
 IVA_FSEL_19P2_ES1: .word	0x07
 IVA_M2_19P2_ES1: .word		0x01

 IVA_M_19P2_ES2: .word		0x0E1
 IVA_N_19P2_ES2: .word		0x0B
 IVA_FSEL_19P2_ES2: .word	0x06
 IVA_M2_19P2_ES2: .word		0x01

 IVA_M_19P2: .word		0x14C
 IVA_N_19P2: .word		0x17
 IVA_FSEL_19P2: .word		0x03
 IVA_M2_19P2: .word		0x01

 IVA_M_26_ES1: .word		0x07D
 IVA_N_26_ES1: .word		0x0C
 IVA_FSEL_26_ES1: .word		0x07
 IVA_M2_26_ES1: .word		0x01

 IVA_M_26_ES2: .word		0x0B4
 IVA_N_26_ES2: .word		0x0C
 IVA_FSEL_26_ES2: .word		0x07
 IVA_M2_26_ES2: .word		0x01

 IVA_M_26: .word		0x085
 IVA_N_26: .word		0x0C
 IVA_FSEL_26: .word		0x07
 IVA_M2_26: .word		0x01

 IVA_M_38P4_ES1	: .word	0x13F
 IVA_N_38P4_ES1: .word		0x30
 IVA_FSEL_38P4_ES1: .word	0x03
 IVA_M2_38P4_ES1: .word		0x01

 IVA_M_38P4_ES2 : .word		0x0E1
 IVA_N_38P4_ES2	: .word	0x17
 IVA_FSEL_38P4_ES2: .word	0x06
 IVA_M2_38P4_ES2: .word		0x01

 IVA_M_38P4: .word		0x14C
 IVA_N_38P4: .word		0x2F
 IVA_FSEL_38P4: .word		0x03
 IVA_M2_38P4: .word		0x01

/* CORE DPLL */

 CORE_M_12: .word		0xA6
 CORE_N_12: .word		0x05
 CORE_FSEL_12: .word		0x07
 CORE_M2_12: .word		0x01	/* M3 of 2 */

 CORE_M_12_ES1: .word		0x19F
 CORE_N_12_ES1: .word		0x0E
 CORE_FSL_12_ES1: .word		0x03
 CORE_M2_12_ES1: .word		0x1	/* M3 of 2 */

 CORE_M_13: .word		0x14C
 CORE_N_13: .word		0x0C
 CORE_FSEL_13: .word		0x03
 CORE_M2_13: .word		0x01	/* M3 of 2 */

 CORE_M_13_ES1: .word		0x1B2
 CORE_N_13_ES1: .word		0x10
 CORE_FSL_13_ES1: .word		0x03
 CORE_M2_13_ES1: .word		0x01	/* M3 of 2 */

 CORE_M_19P2: .word		0x19F
 CORE_N_19P2: .word		0x17
 CORE_FSEL_19P2: .word		0x03
 CORE_M2_19P2: .word		0x01	/* M3 of 2 */

 CORE_M_19P2_ES1: .word		0x19F
 CORE_N_19P2_ES1: .word		0x17
 CORE_FSL_19P2_ES1: .word	0x03
 CORE_M2_19P2_ES1: .word	0x01	/* M3 of 2 */

 CORE_M_26: .word		0xA6
 CORE_N_26: .word		0x0C
 CORE_FSEL_26: .word		0x07
 CORE_M2_26: .word		0x01	/* M3 of 2 */

 CORE_M_26_ES1: .word		0x1B2
 CORE_N_26_ES1: .word		0x21
 CORE_FSL_26_ES1: .word		0x03
 CORE_M2_26_ES1: .word		0x01	/* M3 of 2 */

 CORE_M_38P4: .word		0x19F
 CORE_N_38P4: .word	0x2F
 CORE_FSEL_38P4: .word		0x03
 CORE_M2_38P4: .word		0x01	/* M3 of 2 */

 CORE_M_38P4_ES1: .word		0x19F
 CORE_N_38P4_ES1: .word		0x2F
 CORE_FSL_38P4_ES1: .word	0x03
 CORE_M2_38P4_ES1: .word	0x01	/* M3 of 2 */

/* PER DPLL */

 PER_M_12: .word		0xD8
 PER_N_12: .word		0x05
 PER_FSEL_12: .word		0x07
 PER_M2_12: .word		0x09

 PER_M_13: .word		0x1B0
 PER_N_13: .word		0x0C
 PER_FSEL_13: .word		0x03
 PER_M2_13: .word		0x09

 PER_M_19P2: .word		0xE1
 PER_N_19P2: .word		0x09
 PER_FSEL_19P2: .word		0x07
 PER_M2_19P2: .word		0x09

 PER_M_26: .word		0xD8
 PER_N_26: .word		0x0C
 PER_FSEL_26: .word		0x07
 PER_M2_26: .word		0x09

 PER_M_38P4: .word		0xE1
 PER_N_38P4: .word		0x13
 PER_FSEL_38P4: .word		0x07
 PER_M2_38P4: .word		0x09

/* 36XX PER DPLL */

 PER_36XX_M_12: .word		0x1B0
 PER_36XX_N_12: .word		0x05
 PER_36XX_FSEL_12: .word	0x07
 PER_36XX_M2_12	: .word	0x09

 PER_36XX_M_13: .word		0x360
 PER_36XX_N_13: .word		0x0C
 PER_36XX_FSEL_13: .word	0x03
 PER_36XX_M2_13	: .word	0x09

 PER_36XX_M_19P2: .word		0x1C2
 PER_36XX_N_19P2: .word		0x09
 PER_36XX_FSEL_19P2: .word	0x07
 PER_36XX_M2_19P2: .word	0x09

 PER_36XX_M_26: .word		0x1B0
 PER_36XX_N_26: .word		0x0C
 PER_36XX_FSEL_26: .word	0x07
 PER_36XX_M2_26	: .word	0x09

 PER_36XX_M_38P4: .word		0x1C2
 PER_36XX_N_38P4: .word		0x13
 PER_36XX_FSEL_38P4: .word	0x07
 PER_36XX_M2_38P4: .word	0x09


mpu_dpll_param:
/* 12MHz */
/* ES1 */
.word MPU_M_12_ES1, MPU_N_12_ES1, MPU_FSEL_12_ES1, MPU_M2_12_ES1
/* ES2 */
.word MPU_M_12_ES2, MPU_N_12_ES2, MPU_FSEL_12_ES2, MPU_M2_ES2
/* 3410 */
.word MPU_M_12, MPU_N_12, MPU_FSEL_12, MPU_M2_12

/* 13MHz */
/* ES1 */
.word MPU_M_13_ES1, MPU_N_13_ES1, MPU_FSEL_13_ES1, MPU_M2_13_ES1
/* ES2 */
.word MPU_M_13_ES2, MPU_N_13_ES2, MPU_FSEL_13_ES2, MPU_M2_13_ES2
/* 3410 */
.word MPU_M_13, MPU_N_13, MPU_FSEL_13, MPU_M2_13

/* 19.2MHz */
/* ES1 */
.word MPU_M_19P2_ES1, MPU_N_19P2_ES1, MPU_FSEL_19P2_ES1, MPU_M2_19P2_ES1
/* ES2 */
.word MPU_M_19P2_ES2, MPU_N_19P2_ES2, MPU_FSEL_19P2_ES2, MPU_M2_19P2_ES2
/* 3410 */
.word MPU_M_19P2, MPU_N_19P2, MPU_FSEL_19P2, MPU_M2_19P2

/* 26MHz */
/* ES1 */
.word MPU_M_26_ES1, MPU_N_26_ES1, MPU_FSEL_26_ES1, MPU_M2_26_ES1
/* ES2 */
.word MPU_M_26_ES2, MPU_N_26_ES2, MPU_FSEL_26_ES2, MPU_M2_26_ES2
/* 3410 */
.word MPU_M_26, MPU_N_26, MPU_FSEL_26, MPU_M2_26

/* 38.4MHz */
/* ES1 */
.word MPU_M_38P4_ES1, MPU_N_38P4_ES1, MPU_FSEL_38P4_ES1, MPU_M2_38P4_ES1
/* ES2 */
.word MPU_M_38P4_ES2, MPU_N_38P4_ES2, MPU_FSEL_38P4_ES2, MPU_M2_38P4_ES2
/* 3410 */
.word MPU_M_38P4, MPU_N_38P4, MPU_FSEL_38P4, MPU_M2_38P4


.global get_mpu_dpll_param
get_mpu_dpll_param:
	adr	r0, mpu_dpll_param
	mov	pc, lr

iva_dpll_param:
/* 12MHz */
/* ES1 */
.word IVA_M_12_ES1, IVA_N_12_ES1, IVA_FSEL_12_ES1, IVA_M2_12_ES1
/* ES2 */
.word IVA_M_12_ES2, IVA_N_12_ES2, IVA_FSEL_12_ES2, IVA_M2_12_ES2
/* 3410 */
.word IVA_M_12, IVA_N_12, IVA_FSEL_12, IVA_M2_12

/* 13MHz */
/* ES1 */
.word IVA_M_13_ES1, IVA_N_13_ES1, IVA_FSEL_13_ES1, IVA_M2_13_ES1
/* ES2 */
.word IVA_M_13_ES2, IVA_N_13_ES2,  IVA_FSEL_13_ES2, IVA_M2_13_ES2
/* 3410 */
.word IVA_M_13, IVA_N_13, IVA_FSEL_13, IVA_M2_13

/* 19.2MHz */
/* ES1 */
.word IVA_M_19P2_ES1, IVA_N_19P2_ES1, IVA_FSEL_19P2_ES1, IVA_M2_19P2_ES1
/* ES2 */
.word IVA_M_19P2_ES2, IVA_N_19P2_ES2, IVA_FSEL_19P2_ES2, IVA_M2_19P2_ES2
/* 3410 */
.word IVA_M_19P2, IVA_N_19P2, IVA_FSEL_19P2, IVA_M2_19P2

/* 26MHz */
/* ES1 */
.word IVA_M_26_ES1, IVA_N_26_ES1, IVA_FSEL_26_ES1, IVA_M2_26_ES1
/* ES2 */
.word IVA_M_26_ES2, IVA_N_26_ES2, IVA_FSEL_26_ES2, IVA_M2_26_ES2
/* 3410 */
.word IVA_M_26, IVA_N_26, IVA_FSEL_26, IVA_M2_26

/* 38.4MHz */
/* ES1 */
.word IVA_M_38P4_ES1, IVA_N_38P4_ES1, IVA_FSEL_38P4_ES1, IVA_M2_38P4_ES1
/* ES2 */
.word IVA_M_38P4_ES2, IVA_N_38P4_ES2, IVA_FSEL_38P4_ES2, IVA_M2_38P4_ES2
/* 3410 */
.word IVA_M_38P4, IVA_N_38P4, IVA_FSEL_38P4, IVA_M2_38P4


.global get_iva_dpll_param
get_iva_dpll_param:
	adr	r0, iva_dpll_param
	mov	pc, lr

/* Core DPLL targets for L3 at 166 & L133 */
core_dpll_param:
/* 12MHz */
/* ES1 */
.word CORE_M_12_ES1, CORE_N_12_ES1, CORE_FSL_12_ES1, CORE_M2_12_ES1
/* ES2 */
.word CORE_M_12, CORE_N_12, CORE_FSEL_12, CORE_M2_12
/* 3410 */
.word CORE_M_12, CORE_N_12, CORE_FSEL_12, CORE_M2_12

/* 13MHz */
/* ES1 */
.word CORE_M_13_ES1, CORE_N_13_ES1, CORE_FSL_13_ES1, CORE_M2_13_ES1
/* ES2 */
.word CORE_M_13, CORE_N_13, CORE_FSEL_13, CORE_M2_13
/* 3410 */
.word CORE_M_13, CORE_N_13, CORE_FSEL_13, CORE_M2_13

/* 19.2MHz */
/* ES1 */
.word CORE_M_19P2_ES1, CORE_N_19P2_ES1, CORE_FSL_19P2_ES1, CORE_M2_19P2_ES1
/* ES2 */
.word CORE_M_19P2, CORE_N_19P2, CORE_FSEL_19P2, CORE_M2_19P2
/* 3410 */
.word CORE_M_19P2, CORE_N_19P2, CORE_FSEL_19P2, CORE_M2_19P2

/* 26MHz */
/* ES1 */
.word CORE_M_26_ES1, CORE_N_26_ES1, CORE_FSL_26_ES1, CORE_M2_26_ES1
/* ES2 */
.word CORE_M_26, CORE_N_26, CORE_FSEL_26, CORE_M2_26
/* 3410 */
.word CORE_M_26, CORE_N_26, CORE_FSEL_26, CORE_M2_26

/* 38.4MHz */
/* ES1 */
.word CORE_M_38P4_ES1, CORE_N_38P4_ES1, CORE_FSL_38P4_ES1, CORE_M2_38P4_ES1
/* ES2 */
.word CORE_M_38P4, CORE_N_38P4, CORE_FSEL_38P4, CORE_M2_38P4
/* 3410 */
.word CORE_M_38P4, CORE_N_38P4, CORE_FSEL_38P4, CORE_M2_38P4

.global get_core_dpll_param
get_core_dpll_param:
	adr	r0, core_dpll_param
	mov	pc, lr

/* PER DPLL values are same for both ES1 and ES2 */
per_dpll_param:
/* 12MHz */
.word PER_M_12, PER_N_12, PER_FSEL_12, PER_M2_12

/* 13MHz */
.word PER_M_13, PER_N_13, PER_FSEL_13, PER_M2_13

/* 19.2MHz */
.word PER_M_19P2, PER_N_19P2, PER_FSEL_19P2, PER_M2_19P2

/* 26MHz */
.word PER_M_26, PER_N_26, PER_FSEL_26, PER_M2_26

/* 38.4MHz */
.word PER_M_38P4, PER_N_38P4, PER_FSEL_38P4, PER_M2_38P4

.globl get_per_dpll_param
get_per_dpll_param:
	adr	r0, per_dpll_param
	mov	pc, lr

mpu_36x_dpll_param:
.word 600, 12, 0, 1
.word 600, 12, 0, 1
.word 600, 12, 0, 1
.word 600, 12, 0, 1

iva_36x_dpll_param:
.word 20, 0, 0, 1
.word 20, 0, 0, 1
.word 20, 0, 0, 1
.word 20, 0, 0, 1

core_36x_dpll_param:
.word 400, 12, 0, 1
.word 400, 12, 0, 1
.word 400, 12, 0, 1
.word 400, 12, 0, 1

per_36x_dpll_param:
/*    SYSCLK    M       N      M2      M3      M4     M5      M6      m2DIV */
.word 13000,    864,   12,     9,      16,     9,     4,      3,      1
.word 13000,    864,   12,     9,      16,     9,     4,      3,      1
.word 13000,    864,   12,     9,      16,     9,     4,      3,      1
.word 13000,    864,   12,     9,      16,     9,     4,      3,      1

.globl get_36x_mpu_dpll_param
get_36x_mpu_dpll_param:
	adr	r0, mpu_36x_dpll_param
	mov	pc, lr

.globl get_36x_iva_dpll_param
get_36x_iva_dpll_param:
	adr	r0, iva_36x_dpll_param
	mov	pc, lr

.globl get_36x_core_dpll_param
get_36x_core_dpll_param:
	adr	r0, core_36x_dpll_param
	mov	pc, lr

.globl get_36x_per_dpll_param
get_36x_per_dpll_param:
	adr	r0, per_36x_dpll_param
	mov	pc, lr




@LOW_LEVEL_SRAM_STACK: .word  0x4020FFFC
LOW_LEVEL_SRAM_STACK: .word  0x83000000
@LOW_LEVEL_SRAM_STACK: .word  0x8150FFFC

ARM_VECTOR_BASE: .word 0x80400010

SRAM_STACK:
	.word LOW_LEVEL_SRAM_STACK


PreStackInit:

	@
	@  Invalidate L1 I/D
	@

	mov	r0, #0			@ set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	@ clear bits 13 (--V-)
	bic	r0, r0, #0x00000007	@ clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00000002	@ set bit 1 (--A-) Align
	orr	r0, r0, #0x00000800	@ set bit 12 (Z---) BTB
	mcr	p15, 0, r0, c1, c0, 0

	/*
	 * Nived Move base address of the exception vectors
	 */

	 @ldr r0, ARM_VECTOR_BASE
	 @mcr p15, 0, r0, c12, c0, 0

	/*
	 * Jump to board specific initialization...
	 * The Mask ROM will have already initialized
	 * basic memory. Go here to bump up clock rate and handle
	 * wake up conditions.
	 */
	@mov	ip, lr			@ persevere link reg across call
	@bl	lowlevel_init		@ go setup pll,mux,memory
	@mov	lr, ip			@ restore link
	@mov	pc, lr			@ back to my caller
	b lowlevel_init

lowlevel_init:
	ldr	sp, SRAM_STACK
	@str	ip, [sp]	/* stash old link register */
	@mov	ip, lr		/* save link reg across call */
	@bl	s_init		/* go setup pll, mux, memory */
	@ldr	ip, [sp]	/* restore save ip */
	@mov	lr, ip		/* restore link reg */

	/* back to arch calling code */
	@mov	pc, lr

	/* the literal pools origin */
	@.ltorg


PreStackEnd:
    B     PreStackInit_Exit_Pointer
