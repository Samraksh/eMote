/*
 * Name : TIAM3517_bootstrap.cpp
 *
 * Author : Nived.Sivadas@Samraksh.com
 *
 * Description : Low level initializations of the PRCM etc are done here, contents of this file follow uboot\cpu\arm_co  * rtexa8\omap3\clock.c
 */

#include "..\TIAM3517.h"

void prcm_init(void);

void s_init(void)
{

	prcm_init();

	per_clocks_enable();

}

static void dpll3_init_36xx(u32 sil_index, u32 clk_index)
{
	dpll_param *ptr = (dpll_param *) get_36x_core_dpll_param();
	TIAM3517_CLOCK_MANAGER_PRCM &CMGRPRCM = TIAM3517::CMGRPRCM();
	void (*f_lock_pll) (u32, u32, u32, u32);
	int p0, p1, p2, p3;


	//xip_safe = is_running_in_sram();

	/* Moving it to the right sysclk base */
	ptr += clk_index;

	if (true) {
		/* CORE DPLL */

		/* Select relock bypass: CM_CLKEN_PLL[0:2] */
		sr32(CMGRPRCM.clken_pll, 0, 3, PLL_FAST_RELOCK_BYPASS);
		wait_on_value(ST_CORE_CLK, 0, CMGRPRCM.idlest_ckgen,
				LDELAY);

		/* CM_CLKSEL1_EMU[DIV_DPLL3] */
		sr32(CMGRPRCM.clksel1_emu, 16, 5, CORE_M3X2);

		/* M2 (CORE_DPLL_CLKOUT_DIV): CM_CLKSEL1_PLL[27:31] */
		sr32(CMGRPRCM.clksel1_pll, 27, 5, ptr->m2);

		/* M (CORE_DPLL_MULT): CM_CLKSEL1_PLL[16:26] */
		sr32(CMGRPRCM.clksel1_pll, 16, 11, ptr->m);

		/* N (CORE_DPLL_DIV): CM_CLKSEL1_PLL[8:14] */
		sr32(CMGRPRCM.clksel1_pll, 8, 7, ptr->n);

		/* Source is the CM_96M_FCLK: CM_CLKSEL1_PLL[6] */
		sr32(CMGRPRCM.clksel1_pll, 6, 1, 0);

		/* SSI */
		sr32(CMGRPRCM.clksel_core, 8, 4, CORE_SSI_DIV);
		/* FSUSB */
		sr32(CMGRPRCM.clksel_core, 4, 2, CORE_FUSB_DIV);
		/* L4 */
		sr32(CMGRPRCM.clksel_core, 2, 2, CORE_L4_DIV);
		/* L3 */
		sr32(CMGRPRCM.clksel_core, 0, 2, CORE_L3_DIV);
		/* GFX */
		sr32(CMGRPRCM.clksel_gfx,  0, 3, GFX_DIV);
		/* RESET MGR */
		sr32(CMGRPRCM.clksel_wkup, 1, 2, WKUP_RSM);
		/* FREQSEL (CORE_DPLL_FREQSEL): CM_CLKEN_PLL[4:7] */
		sr32(CMGRPRCM.clken_pll,   4, 4, ptr->fsel);
		/* LOCK MODE */
		sr32(CMGRPRCM.clken_pll,   0, 3, PLL_LOCK);

		wait_on_value(ST_CORE_CLK, 1, CMGRPRCM.idlest_ckgen,
				LDELAY);
	} 

	// Nived : We are running out of the RAM, the following piece of code is not necessary
	
	//else if (is_running_in_flash()) {
		/*
		 * if running from flash, jump to small relocated code
		 * area in SRAM.
		 */
	//	f_lock_pll = (void *) ((u32) &_end_vect - (u32) &_start +
	//			SRAM_VECT_CODE);

	//	p0 = readl(CMGRPRCM.clken_pll);
	//	sr32(&p0, 0, 3, PLL_FAST_RELOCK_BYPASS);
		/* FREQSEL (CORE_DPLL_FREQSEL): CM_CLKEN_PLL[4:7] */
	//	sr32(&p0, 4, 4, ptr->fsel);

	//	p1 = readl(CMGRPRCM.clksel1_pll);
		/* M2 (CORE_DPLL_CLKOUT_DIV): CM_CLKSEL1_PLL[27:31] */
	//	sr32(&p1, 27, 5, ptr->m2);
		/* M (CORE_DPLL_MULT): CM_CLKSEL1_PLL[16:26] */
	//	sr32(&p1, 16, 11, ptr->m);
		/* N (CORE_DPLL_DIV): CM_CLKSEL1_PLL[8:14] */
	//	sr32(&p1, 8, 7, ptr->n);
		/* Source is the CM_96M_FCLK: CM_CLKSEL1_PLL[6] */
	//	sr32(&p1, 6, 1, 0);

	//	p2 = readl(CMGRPRCM.clksel_core);
		/* SSI */
	//	sr32(&p2, 8, 4, CORE_SSI_DIV);
		/* FSUSB */
	//	sr32(&p2, 4, 2, CORE_FUSB_DIV);
		/* L4 */
	//	sr32(&p2, 2, 2, CORE_L4_DIV);
		/* L3 */
	//	sr32(&p2, 0, 2, CORE_L3_DIV);

	//	p3 = (u32)CMGRPRCM.idlest_ckgen;

	//	(*f_lock_pll) (p0, p1, p2, p3);
	//}
}

static void dpll4_init_36xx(u32 sil_index, u32 clk_index)
{
	struct dpll_per_36x_param *ptr;
	TIAM3517_CLOCK_MANAGER_PRCM &CMGRPRCM = TIAM3517::CMGRPRCM();

	ptr = (struct dpll_per_36x_param *)get_36x_per_dpll_param();

	/* Moving it to the right sysclk base */
	ptr += clk_index;

	/* EN_PERIPH_DPLL: CM_CLKEN_PLL[16:18] */
	sr32(CMGRPRCM.clken_pll, 16, 3, PLL_STOP);
	wait_on_value(ST_PERIPH_CLK, 0, CMGRPRCM.idlest_ckgen, LDELAY);

	/* M6 (DIV_DPLL4): CM_CLKSEL1_EMU[24:29] */
	sr32(CMGRPRCM.clksel1_emu, 24, 6, ptr->m6);

	/* M5 (CLKSEL_CAM): CM_CLKSEL1_EMU[0:5] */
	sr32(CMGRPRCM.clksel_cam, 0, 6, ptr->m5);

	/* M4 (CLKSEL_DSS1): CM_CLKSEL_DSS[0:5] */
	sr32(CMGRPRCM.clksel_dss, 0, 6, ptr->m4);

	/* M3 (CLKSEL_DSS1): CM_CLKSEL_DSS[8:13] */
	sr32(CMGRPRCM.clksel_dss, 8, 6, ptr->m3);

	/* M2 (DIV_96M): CM_CLKSEL3_PLL[0:4] */
	sr32(CMGRPRCM.clksel3_pll, 0, 5, ptr->m2);

	/* M (PERIPH_DPLL_MULT): CM_CLKSEL2_PLL[8:19] */
	sr32(CMGRPRCM.clksel2_pll, 8, 12, ptr->m);

	/* N (PERIPH_DPLL_DIV): CM_CLKSEL2_PLL[0:6] */
	sr32(CMGRPRCM.clksel2_pll, 0, 7, ptr->n);

	/* M2DIV (CLKSEL_96M): CM_CLKSEL_CORE[12:13] */
	sr32(CMGRPRCM.clksel_core, 12, 2, ptr->m2div);

	/* LOCK MODE (EN_PERIPH_DPLL): CM_CLKEN_PLL[16:18] */
	sr32(CMGRPRCM.clken_pll, 16, 3, PLL_LOCK);
	wait_on_value(ST_PERIPH_CLK, 2, CMGRPRCM.idlest_ckgen, LDELAY);
}

static void mpu_init_36xx(u32 sil_index, u32 clk_index)
{
	dpll_param *ptr = (dpll_param *) get_36x_mpu_dpll_param();
	TIAM3517_CLOCK_MANAGER_PRCM &CMGRPRCM = TIAM3517::CMGRPRCM();

	/* Moving to the right sysclk */
	ptr += clk_index;

	/* MPU DPLL (unlocked already */

	/* M2 (MPU_DPLL_CLKOUT_DIV) : CM_CLKSEL2_PLL_MPU[0:4] */
	sr32(CMGRPRCM.clksel2_pll_mpu, 0, 5, ptr->m2);

	/* M (MPU_DPLL_MULT) : CM_CLKSEL2_PLL_MPU[8:18] */
	sr32(CMGRPRCM.clksel1_pll_mpu, 8, 11, ptr->m);

	/* N (MPU_DPLL_DIV) : CM_CLKSEL2_PLL_MPU[0:6] */
	sr32(CMGRPRCM.clksel1_pll_mpu, 0, 7, ptr->n);
}

static void iva_init_36xx(u32 sil_index, u32 clk_index)
{
	dpll_param *ptr = (dpll_param *)get_36x_iva_dpll_param();
	TIAM3517_CLOCK_MANAGER_PRCM &CMGRPRCM = TIAM3517::CMGRPRCM();

	/* Moving to the right sysclk */
	ptr += clk_index;

	/* IVA DPLL */
	/* EN_IVA2_DPLL : CM_CLKEN_PLL_IVA2[0:2] */
	sr32(CMGRPRCM.clken_pll_iva2, 0, 3, PLL_STOP);
	wait_on_value(ST_IVA2_CLK, 0, CMGRPRCM.idlest_pll_iva2, LDELAY);

	/* M2 (IVA2_DPLL_CLKOUT_DIV) : CM_CLKSEL2_PLL_IVA2[0:4] */
	sr32(CMGRPRCM.clksel2_pll_iva2, 0, 5, ptr->m2);

	/* M (IVA2_DPLL_MULT) : CM_CLKSEL1_PLL_IVA2[8:18] */
	sr32(CMGRPRCM.clksel1_pll_iva2, 8, 11, ptr->m);

	/* N (IVA2_DPLL_DIV) : CM_CLKSEL1_PLL_IVA2[0:6] */
	sr32(CMGRPRCM.clksel1_pll_iva2, 0, 7, ptr->n);

	/* LOCK (MODE (EN_IVA2_DPLL) : CM_CLKEN_PLL_IVA2[0:2] */
	sr32(CMGRPRCM.clken_pll_iva2, 0, 3, PLL_LOCK);

	wait_on_value(ST_IVA2_CLK, 1, CMGRPRCM.idlest_pll_iva2, LDELAY);
}



void prcm_init(void)
{
	
	UINT32 osc_clk = get_osc_clk_speed();
	TIAM3517_CLOCK_MANAGER_PRCM &CMGRPRCM = TIAM3517::CMGRPRCM();
	TIAM3517_CLOCK_MANAGER_PRM &CMGRPRM = TIAM3517::CMGRP();

	sr32(CMGRPRM.clksel, 0, 3, sys_clkin_sel);

	if (sys_clkin_sel > 2) {
		/* input clock divider */
		sr32(CMGRPRM.clksrc_ctrl, 6, 2, 2);
		clk_index = sys_clkin_sel / 2;
	} else {
		/* input clock divider */
		sr32(CMGRPRM.clksrc_ctrl, 6, 2, 1);
		clk_index = sys_clkin_sel;
	}

	if (is_cpu_family(CPU_OMAP36XX)) {
		/* Unlock MPU DPLL (slows things down, and needed later) */
		sr32(CMGRPRCM.clken_pll_mpu, 0, 3, PLL_LOW_POWER_BYPASS);
		wait_on_value(ST_MPU_CLK, 0, CMGRPRCM.idlest_pll_mpu,
				LDELAY);

		dpll3_init_36xx(0, clk_index);
		dpll4_init_36xx(0, clk_index);
		iva_init_36xx(0, clk_index);
		mpu_init_36xx(0, clk_index);

		/* Lock MPU DPLL to set frequency */
		sr32(CMGRPRCM.clken_pll_mpu, 0, 3, PLL_LOCK);
		wait_on_value(ST_MPU_CLK, 1, CMGRPRCM.idlest_pll_mpu,
				LDELAY);
	} else {
		/*
		 * The DPLL tables are defined according to sysclk value and
		 * silicon revision. The clk_index value will be used to get
		 * the values for that input sysclk from the DPLL param table
		 * and sil_index will get the values for that SysClk for the
		 * appropriate silicon rev.
		 */
		if ((is_cpu_family(CPU_OMAP34XX)
				&& (get_cpu_rev() >= CPU_3XX_ES20)) ||
			(is_cpu_family(CPU_AM35XX)))
			sil_index = 1;

		/* Unlock MPU DPLL (slows things down, and needed later) */
		sr32(CMGRPRCM.clken_pll_mpu, 0, 3, PLL_LOW_POWER_BYPASS);
		wait_on_value(ST_MPU_CLK, 0, CMGRPRCM.idlest_pll_mpu,
				LDELAY);

		dpll3_init_34xx(sil_index, clk_index);
		dpll4_init_34xx(sil_index, clk_index);
		if (!is_cpu_family(CPU_AM35XX))
			iva_init_34xx(sil_index, clk_index);
		mpu_init_34xx(sil_index, clk_index);

		/* Lock MPU DPLL to set frequency */
		sr32(CMGRPRCM.clken_pll_mpu, 0, 3, PLL_LOCK);
		wait_on_value(ST_MPU_CLK, 1, CMGRPRCM.idlest_pll_mpu,
				LDELAY);
	}

	/* Set up GPTimers to sys_clk source only */
	sr32(CMGRPRCM.clksel_per, 0, 8, 0xff);
	sr32(CMGRPRCM.clksel_wkup, 0, 1, 1);
	

}


UINT32 get_osc_clk_speed(void)
{

	UINT32 val;
	UINT32 start;
	
	TIAM3517_CLOCK_MANAGER_PRM &CMGRPRM = TIAM3517::CMGRP();
	TIAM3517_CLOCK_MANAGER_PRCM &CMGRPRCM = TIAM3517::CMGRPRCM();
	TIAM3517_TIMER &TIMER = getTimer(0);

	val = CMGRPRM.clksrc_ctrl;

	if(val & SYSCLKDIV_2)
		cdiv = 2;
	else if(val & SYSCLKDIV_1)
		cdiv = 1;
	else
		cdiv = 1;


	val = CMGRPRCM.clksel_wkup | CKLSEL_GPT1;


	CMGRPRCM.clksel_wkup = val;


	val = CMGRPRCM.iclken_wkup | EN_GPT1 | EN_32KSYNC;


	CMGRPRCM.iclken_wkup = val;

	
	val = CMGRPRCM.fclken_wkup | EN_GPT1;

	CMGRPRCM.fclken_wkup = val;

	TIMER.TLDR = 0;
	TIMER.TCLR = GPT_EN;

	
	/* start time in 20 cycles */
	start = 20 + readl(&s32k_base->s32k_cr);

	/* dead loop till start time */
	while (readl(&s32k_base->s32k_cr) < start);

	/* get start sys_clk count */
	cstart = readl(&gpt1_base->tcrr);

	/* wait for 40 cycles */
	while (readl(&s32k_base->s32k_cr) < (start + 20)) ;
	cend = readl(&gpt1_base->tcrr);		/* get end sys_clk count */
	cdiff = cend - cstart;			/* get elapsed ticks */
	
	if(cdiv == 2)
	{
		cdiff *= 2;
	}


	if (cdiff > 19000)
		return S38_4M;
	else if (cdiff > 15200)
		return S26M;
	else if (cdiff > 13000)
		return S24M;
	else if (cdiff > 9000)
		return S19_2M;
	else if (cdiff > 7600)
		return S13M;
	else
		return S12M;
	


}
