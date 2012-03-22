/* linux/arch/arm/mach-omap2/sleep3517.S
 *
 * AM3505/3517 Sleep Code.
 * Ranjith Lohithakshan <ranjithl@ti.com>
 *
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * Forces OMAP into idle state
 *
 * omap34xx_suspend() - This bit of code just executes the WFI
 * for normal idles.
 *
 * Note: This code get's copied to internal SRAM at boot. When the OMAP
 *	 wakes up it continues execution at the point it went to sleep.
 */
.global omap3517_cpu_suspend
.section BootStrap,       "a", %progbits
omap3517_cpu_suspend:
	stmfd	sp!, {r0-r12, lr}		@ save registers on stack
	
	/* Put EMIF in self-refresh */
	ldr     r4, emif_pm_ctrl
	ldr     r5, [r4]
	orr     r5, r5, #0x200
	str     r5, [r4]

	/* Disable SDRC and Control Module */
	ldr     r4, cm_iclken1_core
	ldr     r5, [r4]
	str     r5, iclk_core_enable
	ldr     r4, cm_iclken1_core
	ldr     r5, clk_core_disable
	str     r5, [r4]
wait_sdrc_ok:
	ldr     r4, cm_idlest1_core
	ldr     r5, [r4]
	and     r5, r5, #0x2
	cmp     r5, #0x2
	bne     wait_sdrc_ok

	/* Gate DDR Phy clock */
	ldr     r4, omap3517_conf1
	ldr     r5, emif_phy_gate
	str     r5, [r4]

	/* Data memory barrier and Data sync barrier */
	mov	r1, #0
	mcr	p15, 0, r1, c7, c10, 4
	mcr	p15, 0, r1, c7, c10, 5

	wfi

	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	/* Enable SDRC and Control Module */
	ldr     r4, cm_iclken1_core
	ldr     r5, iclk_core_enable
	str     r5, [r4]

	/* Enable DDR Phy Clock */
	ldr     r4, omap3517_conf1
	ldr     r5, emif_phy_enable
	str     r5, [r4]

	/* Take EMIF out of self-refresh */
	ldr     r4, emif_pm_ctrl
	ldr     r5, [r4]
	bic     r5, r5, #0x200
	str     r5, [r4]

	ldmfd	sp!, {r0-r12, pc}		@ restore regs and return

clk_core_disable:
	.word   0x0
iclk_core_enable:
	.word   0x0
emif_phy_gate:
	.word   0x2620
emif_phy_enable:
	.word   0x8620
cm_idlest1_core:
	.word 0x48004a20
	/*.word   CM_IDLEST1_CORE_V*/
cm_clkst_core:
	.word 0x48004a4c
	/*.word   CM_CLKST_CORE_V*/
emif_pm_ctrl:
	.word	0x6d000038
	/*.word   EMIF_PM_CTR_V*/
cm_iclken1_core:
	.word 0x48004a10
	/*.word   CM_ICLKEN1_CORE_V*/
omap3517_conf1:
	.word	0x48002584
	/*.word   OMAP3517_CONF1_REG_V*/
