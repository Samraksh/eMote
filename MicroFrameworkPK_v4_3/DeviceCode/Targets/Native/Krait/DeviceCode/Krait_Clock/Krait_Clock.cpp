#include "..\Krait.h"
#include "Krait_Clock.h"
#include "..\Krait_MMC\Krait__MMC.h"

enum {
	PCOM_CMD_IDLE = 0x0,
	PCOM_CMD_DONE,
	PCOM_RESET_APPS,
	PCOM_RESET_CHIP,
	PCOM_CONFIG_NAND_MPU,
	PCOM_CONFIG_USB_CLKS,
	PCOM_GET_POWER_ON_STATUS,
	PCOM_GET_WAKE_UP_STATUS,
	PCOM_GET_BATT_LEVEL,
	PCOM_CHG_IS_CHARGING,
	PCOM_POWER_DOWN,
	PCOM_USB_PIN_CONFIG,
	PCOM_USB_PIN_SEL,
	PCOM_SET_RTC_ALARM,
	PCOM_NV_READ,
	PCOM_NV_WRITE,
	PCOM_GET_UUID_HIGH,
	PCOM_GET_UUID_LOW,
	PCOM_GET_HW_ENTROPY,
	PCOM_RPC_GPIO_TLMM_CONFIG_REMOTE,
	PCOM_CLKCTL_RPC_ENABLE,
	PCOM_CLKCTL_RPC_DISABLE,
	PCOM_CLKCTL_RPC_RESET,
	PCOM_CLKCTL_RPC_SET_FLAGS,
	PCOM_CLKCTL_RPC_SET_RATE,
	PCOM_CLKCTL_RPC_MIN_RATE,
	PCOM_CLKCTL_RPC_MAX_RATE,
	PCOM_CLKCTL_RPC_RATE,
	PCOM_CLKCTL_RPC_PLL_REQUEST,
	PCOM_CLKCTL_RPC_ENABLED,
	PCOM_VREG_SWITCH,
	PCOM_VREG_SET_LEVEL,
	PCOM_GPIO_TLMM_CONFIG_GROUP,
	PCOM_GPIO_TLMM_UNCONFIG_GROUP,
	PCOM_NV_READ_HIGH_BITS,
	PCOM_NV_WRITE_HIGH_BITS,
	PCOM_RPC_GPIO_TLMM_CONFIG_EX = 0x25,
	PCOM_NUM_CMDS,
	PCOM_KERNEL_SEC_BOOT = 0x7A,
};

enum {
	PCOM_INVALID_STATUS = 0x0,
	PCOM_READY,
	PCOM_CMD_RUNNING,
	PCOM_CMD_SUCCESS,
	PCOM_CMD_FAIL,
};


#define APP_COMMAND (MSM_SHARED_BASE + 0x00)
#define APP_STATUS  (MSM_SHARED_BASE + 0x04)
#define APP_DATA1   (MSM_SHARED_BASE + 0x08)
#define APP_DATA2   (MSM_SHARED_BASE + 0x0C)

#define MDM_COMMAND (MSM_SHARED_BASE + 0x10)
#define MDM_STATUS  (MSM_SHARED_BASE + 0x14)
#define MDM_DATA1   (MSM_SHARED_BASE + 0x18)
#define MDM_DATA2   (MSM_SHARED_BASE + 0x1C)

void notify_other_proc_comm(void)
{
	writel(1 << 6, (MSM_GCC_BASE + 0x8));
}

int msm_proc_comm(unsigned cmd, unsigned *data1, unsigned *data2)
{
	int ret = -1;
	unsigned status;

//      dprintf(INFO, "proc_comm(%d,%d,%d)\n",
//              cmd, data1 ? *data1 : 0, data2 ? *data2 : 0);
	while (readl(MDM_STATUS) != PCOM_READY) {
		/* XXX check for A9 reset */
	}

	if (data1)
		writel(*data1, APP_DATA1);
	if (data2)
		writel(*data2, APP_DATA2);

	/*
	 * As per the specs write data, cmd, interrupt for
	 * proc comm processing
	 */
	writel(cmd, APP_COMMAND);
//      dprintf(INFO, "proc_comm tx\n");
	notify_other_proc_comm();
	while (readl(APP_COMMAND) != PCOM_CMD_DONE) {
		/* XXX check for A9 reset */
	}

	status = readl(APP_STATUS);
//      dprintf(INFO, "proc_comm status %d\n", status);

	if (status != PCOM_CMD_FAIL) {
		if (data1)
			*data1 = readl(APP_DATA1);
		if (data2)
			*data2 = readl(APP_DATA2);
		ret = 0;
		/*
		 * Write command idle to indicate non HLOS that
		 * apps has finished reading the status & data
		 * of proc comm command
		 */
		writel(PCOM_CMD_IDLE, APP_COMMAND);
	}

	return ret;
}

int clock_enable(unsigned id)
{
	return msm_proc_comm(PCOM_CLKCTL_RPC_ENABLE, &id, 0);
}

int clock_disable(unsigned id)
{
	return msm_proc_comm(PCOM_CLKCTL_RPC_DISABLE, &id, 0);
}

int clock_set_rate(unsigned id, unsigned rate)
{
	return msm_proc_comm(PCOM_CLKCTL_RPC_SET_RATE, &id, &rate);
}

int clock_get_rate(unsigned id)
{
	if (msm_proc_comm(PCOM_CLKCTL_RPC_RATE, &id, 0)) {
		return -1;
	} else {
		return (int)id;
	}
}

struct branch_clk *to_branch_clk(struct clk *clk)
{
	return NULL;
}

struct rcg_clk *to_rcg_clk(struct clk *clk)
{
	return container_of(clk, struct rcg_clk, c);
}

int branch_in_hwcg_mode(const struct branch *b)
{
	if (!b->hwcg_mask)
		return 0;

	return !!(readl(b->hwcg_reg) & b->hwcg_mask);
}

void __branch_clk_enable_reg(const struct branch *clk, const char *name)
{
	UINT32 reg_val;

	if (clk->en_mask) {
		reg_val = readl(clk->ctl_reg);
		reg_val |= clk->en_mask;
		writel(reg_val, clk->ctl_reg);
	}

	/*
	 * Use a memory barrier since some halt status registers are
	 * not within the same 1K segment as the branch/root enable
	 * registers.  It's also needed in the udelay() case to ensure
	 * the delay starts after the branch enable.
	 */
	mb();

	/* Skip checking halt bit if the clock is in hardware gated mode */
	if (branch_in_hwcg_mode(clk))
		return;

	for(int i = 0; i < 10000; i++);
#if 0
	/* Wait for clock to enable before returning. */
	if (clk->halt_check == DELAY)
		udelay(HALT_CHECK_DELAY_US);

	else if (clk->halt_check == ENABLE || clk->halt_check == HALT
			|| clk->halt_check == ENABLE_VOTED
			|| clk->halt_check == HALT_VOTED) {
		int count;
		/* Wait up to HALT_CHECK_MAX_LOOPS for clock to enable. */
		for (count = HALT_CHECK_MAX_LOOPS; branch_clk_is_halted(clk)
					&& count > 0; count--)
			udelay(1);
		WARN(count == 0, "%s status stuck at 'off'", name);

	}
#endif
}


UINT32 __branch_clk_disable_reg(const struct branch *clk, const char *name)
{
	UINT32 reg_val;

	reg_val = readl(clk->ctl_reg);
	if (clk->en_mask) {
		reg_val &= ~(clk->en_mask);
		writel(reg_val, clk->ctl_reg);
	}

	/*
	 * Use a memory barrier since some halt status registers are
	 * not within the same K segment as the branch/root enable
	 * registers.  It's also needed in the udelay() case to ensure
	 * the delay starts after the branch disable.
	 */
	mb();

	/* Skip checking halt bit if the clock is in hardware gated mode */
	if (branch_in_hwcg_mode(clk))
		return reg_val;

	/* Wait for clock to disable before continuing. */
	if (clk->halt_check == DELAY || clk->halt_check == ENABLE_VOTED
				     || clk->halt_check == HALT_VOTED)
		//udelay(HALT_CHECK_DELAY_US);
		// Nived.Sivadas
		for(int i = 0; i < 10000; i++);
	else if (clk->halt_check == ENABLE || clk->halt_check == HALT) {
		int count;

#if 0
		/* Wait up to HALT_CHECK_MAX_LOOPS for clock to disable. */
		for (count = HALT_CHECK_MAX_LOOPS; !branch_clk_is_halted(clk)
					&& count > 0; count--)
			udelay(1);
		WARN(count == 0, "%s status stuck at 'on'", name);

#endif
	}
	return reg_val;
}


/* Perform any register operations required to disable the generator. */
void __rcg_clk_disable_reg(struct rcg_clk *clk)
{
	void __iomem *const reg = clk->b.ctl_reg;
	UINT32 reg_val;

	reg_val = __branch_clk_disable_reg(&clk->b, clk->c.dbg_name);
	/* Disable root. */
	if (clk->root_en_mask) {
		reg_val &= ~(clk->root_en_mask);
		writel(reg_val, reg);
	}
	/* Disable MN counter, if applicable. */
	if (clk->current_freq->mnd_en_mask) {
		reg_val &= ~(clk->current_freq->mnd_en_mask);
		writel(reg_val, reg);
	}
	/*
	 * Program NS register to low-power value with an un-clocked or
	 * slowly-clocked source selected.
	 */
	if (clk->ns_mask) {
		reg_val = readl(clk->ns_reg);
		reg_val &= ~(clk->ns_mask);
		reg_val |= (clk->freq_tbl->ns_val & clk->ns_mask);
		writel(reg_val, clk->ns_reg);
	}
}


void __rcg_clk_enable_reg(struct rcg_clk *clk)
{
	UINT32 reg_val;
	void __iomem *const reg = clk->b.ctl_reg;


	/*
	 * Program the NS register, if applicable. NS registers are not
	 * set in the set_rate path because power can be saved by deferring
	 * the selection of a clocked source until the clock is enabled.
	 */
	if (clk->ns_mask) {
		reg_val = readl(clk->ns_reg);
		reg_val &= ~(clk->ns_mask);
		reg_val |= (clk->current_freq->ns_val & clk->ns_mask);
		writel(reg_val, clk->ns_reg);
	}

	/* Enable MN counter, if applicable. */
	reg_val = readl(reg);
	if (clk->current_freq->mnd_en_mask) {
		reg_val |= clk->current_freq->mnd_en_mask;
		writel(reg_val, reg);
	}
	/* Enable root. */
	if (clk->root_en_mask) {
		reg_val |= clk->root_en_mask;
		writel(reg_val, reg);
	}
	__branch_clk_enable_reg(&clk->b, clk->c.dbg_name);
}


int rcg_clk_enable(struct clk *c)
{
	unsigned long flags;
	struct rcg_clk *clk = to_rcg_clk(c);

	//spin_lock_irqsave(&local_clock_reg_lock, flags);
	__rcg_clk_enable_reg(clk);
	clk->enabled = true;
	//spin_unlock_irqrestore(&local_clock_reg_lock, flags);

	return 0;
}

/* Disable a rate-settable clock. */
void rcg_clk_disable(struct clk *c)
{
	unsigned long flags;
	struct rcg_clk *clk = to_rcg_clk(c);

	//spin_lock_irqsave(&local_clock_reg_lock, flags);
	__rcg_clk_disable_reg(clk);
	clk->enabled = false;
	//spin_unlock_irqrestore(&local_clock_reg_lock, flags);
}

/* Set rate and enable the clock */
void clock_config(UINT32 ns, UINT32 md, UINT32 ns_addr, UINT32 md_addr)
{
	unsigned int val = 0;

	/* Activate the reset for the M/N Counter */
	val = 1 << 7;
	writel(val, ns_addr);

	/* Write the MD value into the MD register */
	if (md_addr != 0x0)
		writel(md, md_addr);

	/* Write the ns value, and active reset for M/N Counter, again */
	val = 1 << 7;
	val |= ns;
	writel(val, ns_addr);

	/* De-activate the reset for M/N Counter */
	val = 1 << 7;
	val = ~val;
	val = val & readl(ns_addr);
	writel(val, ns_addr);

	/* Enable the Clock Root */
	val = 1 << 11;
	val = val | readl(ns_addr);
	writel(val, ns_addr);

	/* Enable the Clock Branch */
	val = 1 << 9;
	val = val | readl(ns_addr);
	writel(val, ns_addr);

	/* Enable the M/N Counter */
	val = 1 << 8;
	val = val | readl(ns_addr);
	writel(val, ns_addr);
}


/* Configure MMC clock */
void clock_config_mmc(UINT32 interface, UINT32 freq)
{
	UINT32 reg = 0;
	
	hal_printf("YOU DUN GOOF'D, SEE __FILE__  __LINE__\r\n");
	ASSERT(0); // THIS FUNCTION IS BROKEN, MMC_BOOT_MCI_CLK IS UNDEFINED
	// FIX AND UNCOMMENT BELOW

	switch (freq) {
	case MMC_CLK_400KHZ:
		clock_config(SDC_CLK_NS_400KHZ,
			     SDC_CLK_MD_400KHZ,
			     SDC_NS(interface), SDC_MD(interface));
		break;
	case MMC_CLK_48MHZ:
	case MMC_CLK_50MHZ:	/* Max supported is 48MHZ */
		clock_config(SDC_CLK_NS_48MHZ,
			     SDC_CLK_MD_48MHZ,
			     SDC_NS(interface), SDC_MD(interface));
		break;
	default:
		ASSERT(0);
		break;

	}

	reg |= MMC_BOOT_MCI_CLK_ENABLE;
	reg |= MMC_BOOT_MCI_CLK_ENA_FLOW;
	reg |= MMC_BOOT_MCI_CLK_IN_FEEDBACK;
	//writel(reg, MMC_BOOT_MCI_CLK);
}

/* Configure UART clock - based on the gsbi id */
void clock_config_uart_dm(UINT8 id)
{
	/* Enable gsbi_uart_clk */
	clock_config(UART_DM_CLK_NS_115200,
		     UART_DM_CLK_MD_115200,
		     GSBIn_UART_APPS_NS(id), GSBIn_UART_APPS_MD(id));

	/* Enable gsbi_pclk */
	writel(GSBI_HCLK_CTL_CLK_ENA << GSBI_HCLK_CTL_S, GSBIn_HCLK_CTL(id));
}

