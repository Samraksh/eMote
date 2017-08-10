/*
 *
 * 		Name : Krait__USART_RegDef.h
 *
 *
 *
 */

#ifndef _KRAIT__USART_REGDEF_H_
#define _KRAIT__USART_REGDEF_H_

#include "..\Krait_INTC\Krait__INTC.h"
#include "..\Krait_Clock\Krait_Clock.h"

#define UART_MR1			0x0000

#define CS5  5
#define CS6	 6
#define CS7  7
#define CS8  8

#define GSBI5_UART_DM_BASE 0x16440000

#define MSM8960_GSBI1_UARTDM_IRQ		(GIC_SPI_START + 146)
#define MSM8960_GSBI1_QUP_IRQ			(GIC_SPI_START + 147)
#define MSM8960_GSBI2_UARTDM_IRQ		(GIC_SPI_START + 148)
#define MSM8960_GSBI2_QUP_IRQ		        (GIC_SPI_START + 149)
#define GSBI3_UARTDM_IRQ			(GIC_SPI_START + 150)
#define GSBI3_QUP_IRQ				(GIC_SPI_START + 151)
#define GSBI4_UARTDM_IRQ			(GIC_SPI_START + 152)
#define GSBI4_QUP_IRQ				(GIC_SPI_START + 153)
#define GSBI5_UARTDM_IRQ			(GIC_SPI_START + 154)
#define GSBI5_QUP_IRQ				(GIC_SPI_START + 155)
#define GSBI6_UARTDM_IRQ			(GIC_SPI_START + 156)
#define GSBI6_QUP_IRQ				(GIC_SPI_START + 157)
#define GSBI7_UARTDM_IRQ			(GIC_SPI_START + 158)
#define GSBI7_QUP_IRQ				(GIC_SPI_START + 159)
#define GSBI8_UARTDM_IRQ			(GIC_SPI_START + 160)
#define GSBI8_QUP_IRQ				(GIC_SPI_START + 161)

/* Address of GSBI blocks */
#define MSM_GSBI1_PHYS		0x16000000
#define MSM_GSBI2_PHYS		0x16100000
#define MSM_GSBI3_PHYS		0x16200000
#define MSM_GSBI4_PHYS		0x16300000
#define MSM_GSBI5_PHYS		0x16400000
#define MSM_GSBI6_PHYS		0x16500000
#define MSM_GSBI7_PHYS		0x16600000
#define MSM_GSBI8_PHYS		0x1A000000
#define MSM_GSBI9_PHYS		0x1A100000
#define MSM_GSBI10_PHYS		0x1A200000
#define MSM_GSBI11_PHYS		0x12440000
#define MSM_GSBI12_PHYS		0x12480000

#define MSM_UART2DM_PHYS	(MSM_GSBI2_PHYS + 0x40000)
#define MSM_UART5DM_PHYS	(MSM_GSBI5_PHYS + 0x40000)
#define MSM_UART6DM_PHYS	(MSM_GSBI6_PHYS + 0x40000)

#define MSM_UART1_BASE	0xA9A00000
#define MSM_UART2_BASE	0xA9B00000
#define MSM_UART3_BASE	0xA9C00000

#define MSM_DEBUG_UART_PHYS		0x16440000

#define MSM_UART_RXD 40
#define MSM_UART_TXD 42
#define MSM_USAR_CTS 38
#define MSM_UART_RTS 36

#define UART_MR1_AUTO_RFR_LEVEL0	0x3F
#define UART_MR1_AUTO_RFR_LEVEL1	0x3FF00
#define UART_MR1_RX_RDY_CTL    		(1 << 7)
#define UART_MR1_CTS_CTL       		(1 << 6)

#define UART_MR2			0x0004
#define UART_MR2_ERROR_MODE		(1 << 6)
#define UART_MR2_BITS_PER_CHAR		0x30
#define UART_MR2_BITS_PER_CHAR_5	(0x0 << 4)
#define UART_MR2_BITS_PER_CHAR_6	(0x1 << 4)
#define UART_MR2_BITS_PER_CHAR_7	(0x2 << 4)
#define UART_MR2_BITS_PER_CHAR_8	(0x3 << 4)
#define UART_MR2_STOP_BIT_LEN_ONE	(0x1 << 2)
#define UART_MR2_STOP_BIT_LEN_TWO	(0x3 << 2)
#define UART_MR2_PARITY_MODE_NONE	0x0
#define UART_MR2_PARITY_MODE_ODD	0x1
#define UART_MR2_PARITY_MODE_EVEN	0x2
#define UART_MR2_PARITY_MODE_SPACE	0x3
#define UART_MR2_PARITY_MODE		0x3

#define UART_CSR	0x0008
#define UART_CSR_115200	0xFF
#define UART_CSR_57600	0xEE
#define UART_CSR_38400	0xDD
#define UART_CSR_28800	0xCC
#define UART_CSR_19200	0xBB
#define UART_CSR_14400	0xAA
#define UART_CSR_9600	0x99
#define UART_CSR_7200	0x88
#define UART_CSR_4800	0x77
#define UART_CSR_2400	0x55
#define UART_CSR_1200	0x44
#define UART_CSR_600	0x33
#define UART_CSR_300	0x22
#define UART_CSR_150	0x11
#define UART_CSR_75	0x00

#define UART_TF		0x000C
#define UARTDM_TF	0x0070

#define UART_CR				0x0010
#define UART_CR_CMD_NULL		(0 << 4)
#define UART_CR_CMD_RESET_RX		(1 << 4)
#define UART_CR_CMD_RESET_TX		(2 << 4)
#define UART_CR_CMD_RESET_ERR		(3 << 4)
#define UART_CR_CMD_RESET_BREAK_INT	(4 << 4)
#define UART_CR_CMD_START_BREAK		(5 << 4)
#define UART_CR_CMD_STOP_BREAK		(6 << 4)
#define UART_CR_CMD_RESET_CTS		(7 << 4)
#define UART_CR_CMD_RESET_STALE_INT	(8 << 4)
#define UART_CR_CMD_PACKET_MODE		(9 << 4)
#define UART_CR_CMD_MODE_RESET		(12 << 4)
#define UART_CR_CMD_SET_RFR		(13 << 4)
#define UART_CR_CMD_RESET_RFR		(14 << 4)
#define UART_CR_CMD_PROTECTION_EN	(16 << 4)
#define UART_CR_CMD_STALE_EVENT_ENABLE	(80 << 4)
#define UART_CR_TX_DISABLE		(1 << 3)
#define UART_CR_TX_ENABLE		(1 << 2)
#define UART_CR_RX_DISABLE		(1 << 1)
#define UART_CR_RX_ENABLE		(1 << 0)

#define UART_IMR		0x0014
#define UART_IMR_TXLEV		(1 << 0)
#define UART_IMR_RXSTALE	(1 << 3)
#define UART_IMR_RXLEV		(1 << 4)
#define UART_IMR_DELTA_CTS	(1 << 5)
#define UART_IMR_CURRENT_CTS	(1 << 6)

#define UART_IPR_RXSTALE_LAST		0x20
#define UART_IPR_STALE_LSB		0x1F
#define UART_IPR_STALE_TIMEOUT_MSB	0x3FF80

#define UART_IPR	0x0018
#define UART_TFWR	0x001C
#define UART_RFWR	0x0020
#define UART_HCR	0x0024

#define UART_MREG		0x0028
#define UART_NREG		0x002C
#define UART_DREG		0x0030
#define UART_MNDREG		0x0034
#define UART_IRDA		0x0038
#define UART_MISR_MODE		0x0040
#define UART_MISR_RESET		0x0044
#define UART_MISR_EXPORT	0x0048
#define UART_MISR_VAL		0x004C
#define UART_TEST_CTRL		0x0050

#define UART_SR			0x0008
#define UART_SR_HUNT_CHAR	(1 << 7)
#define UART_SR_RX_BREAK	(1 << 6)
#define UART_SR_PAR_FRAME_ERR	(1 << 5)
#define UART_SR_OVERRUN		(1 << 4)
#define UART_SR_TX_EMPTY	(1 << 3)
#define UART_SR_TX_READY	(1 << 2)
#define UART_SR_RX_FULL		(1 << 1)
#define UART_SR_RX_READY	(1 << 0)

#define UART_RF			0x000C
#define UARTDM_RF		0x0070
#define UART_MISR		0x0010
#define UART_ISR		0x0014
#define UART_ISR_TX_READY	(1 << 7)

#define GSBI_CONTROL		0x0
#define GSBI_PROTOCOL_CODE	0x30
#define GSBI_PROTOCOL_UART	0x40
#define GSBI_PROTOCOL_IDLE	0x0

#define UARTDM_DMRX		0x34
#define UARTDM_NCF_TX		0x40
#define UARTDM_RX_TOTAL_SNAP	0x38

#define IOMEM(x)	x

#define MSM_CLK_CTL_BASE	IOMEM(0xFA010000)

#define REG(off)	(MSM_CLK_CTL_BASE + (off))
#define REG_MM(off)	(MSM_MMSS_CLK_CTL_BASE + (off))
#define REG_LPA(off)	(MSM_LPASS_CLK_CTL_BASE + (off))
#define REG_GCC(off)	(MSM_APCS_GCC_BASE + (off))

#define BM(msb, lsb)	(((((uint32_t)-1) << (31-msb)) >> (31-msb+lsb)) << lsb)


#define BIT(nr)			(1UL << (nr))

#define GSBIn_UART_APPS_NS_REG(n)		REG(0x29D4+(0x20*((n)-1)))
#define GSBIn_UART_APPS_MD_REG(n)		REG(0x29D0+(0x20*((n)-1)))
#define GSBIn_RESET_REG(n)			REG(0x29DC+(0x20*((n)-1)))

#define CLK_HALT_CFPB_STATEB_REG		REG(0x2FD0)

#define F_GSBI_UART(f, s, d, m, n) \
	{ \
		.freq_hz = f, \
		.src_clk = &s##_clk.c, \
		.md_val = MD16(m, n), \
		.ns_val = NS(31, 16, n, m, 5, 4, 3, d, 2, 0, s##_to_bb_mux), \
		.mnd_en_mask = BIT(8) * !!(n), \
	}

#if 0

static struct clk_freq_tbl clk_tbl_gsbi_uart[] = {
	F_GSBI_UART(       0, gnd,  1,  0,   0),
	F_GSBI_UART( 1843200, pll8, 1,  3, 625),
	F_GSBI_UART( 3686400, pll8, 1,  6, 625),
	F_GSBI_UART( 7372800, pll8, 1, 12, 625),
	F_GSBI_UART(14745600, pll8, 1, 24, 625),
	F_GSBI_UART(16000000, pll8, 4,  1,   6),
	F_GSBI_UART(24000000, pll8, 4,  1,   4),
	F_GSBI_UART(32000000, pll8, 4,  1,   3),
	F_GSBI_UART(40000000, pll8, 1,  5,  48),
	F_GSBI_UART(46400000, pll8, 1, 29, 240),
	F_GSBI_UART(48000000, pll8, 4,  1,   2),
	F_GSBI_UART(51200000, pll8, 1,  2,  15),
	F_GSBI_UART(56000000, pll8, 1,  7,  48),
	F_GSBI_UART(58982400, pll8, 1, 96, 625),
	F_GSBI_UART(64000000, pll8, 2,  1,   3),
	F_END
};

#define CLK_GSBI_UART(i, n, h_r, h_b) \
	struct rcg_clk i##_clk = { \
		.b = { \
			.ctl_reg = GSBIn_UART_APPS_NS_REG(n), \
			.en_mask = BIT(9), \
			.reset_reg = GSBIn_RESET_REG(n), \
			.reset_mask = BIT(0), \
			.halt_reg = h_r, \
			.halt_bit = h_b, \
		}, \
		.ns_reg = GSBIn_UART_APPS_NS_REG(n), \
		.md_reg = GSBIn_UART_APPS_MD_REG(n), \
		.root_en_mask = BIT(11), \
		.ns_mask = (BM(31, 16) | BM(6, 0)), \
		.set_rate = set_rate_mnd, \
		.freq_tbl = clk_tbl_gsbi_uart, \
		.current_freq = &rcg_dummy_freq, \
		.c = { \
			.dbg_name = #i "_clk", \
			.ops = &clk_ops_rcg_8960, \
			VDD_DIG_FMAX_MAP2(LOW, 32000000, NOMINAL, 64000000), \
			CLK_INIT(i##_clk.c), \
		}, \
	}

#endif
#endif
