@
@  File Name : TIAM3517_prestackinit.s
@
@  Author    : Nived.Sivadas@Samraksh.com
@
@  Description : Contains low level initialization of pll and clocks
@


	.global PreStackInit


	.arm


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




LOW_LEVEL_SRAM_STACK: .word  0x4020FFFC

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
	 * Jump to board specific initialization...
	 * The Mask ROM will have already initialized
	 * basic memory. Go here to bump up clock rate and handle
	 * wake up conditions.
	 */
	mov	ip, lr			@ persevere link reg across call
	bl	lowlevel_init		@ go setup pll,mux,memory
	mov	lr, ip			@ restore link
	mov	pc, lr			@ back to my caller


lowlevel_init:
	ldr	sp, SRAM_STACK
	str	ip, [sp]	/* stash old link register */
	mov	ip, lr		/* save link reg across call */
	bl	s_init		/* go setup pll, mux, memory */
	ldr	ip, [sp]	/* restore save ip */
	mov	lr, ip		/* restore link reg */

	/* back to arch calling code */
	mov	pc, lr

	/* the literal pools origin */
	.ltorg
	
SRAM_STACK:
	.word LOW_LEVEL_SRAM_STACK
	
