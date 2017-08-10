#ifndef _KRAIT__USART_H_
#define _KRAIT__USART_H_
#include <tinyhal.h>
#include "Krait__USART_RegDef.h"
#include "..\Krait_Clock\Krait_Clock.h"

extern void __rcg_clk_enable_reg(struct rcg_clk *clk);

static unsigned uart_base = MSM_GSBI5_PHYS;

#define uwr(v,a) writel(v, uart_base + (a))

extern int clock_enable(unsigned id);

struct uart_icount {
	UINT32	cts;
	UINT32	dsr;
	UINT32	rng;
	UINT32	dcd;
	UINT32	rx;
	UINT32	tx;
	UINT32	frame;
	UINT32	overrun;
	UINT32	parity;
	UINT32	brk;
	UINT32	buf_overrun;
};

#if 0
struct uart_port {
	spinlock_t		lock;			/* port lock */
	unsigned long		iobase;			/* in/out[bwl] */
	unsigned char __iomem	*membase;		/* read/write[bwl] */
	unsigned int		(*serial_in)(struct uart_port *, int);
	void			(*serial_out)(struct uart_port *, int, int);
	void			(*set_termios)(struct uart_port *,
				               struct ktermios *new,
				               struct ktermios *old);
	void			(*pm)(struct uart_port *, unsigned int state,
				      unsigned int old);
	unsigned int		irq;			/* irq number */
	unsigned long		irqflags;		/* irq flags  */
	unsigned int		uartclk;		/* base uart clock */
	unsigned int		fifosize;		/* tx fifo size */
	unsigned char		x_char;			/* xon/xoff char */
	unsigned char		regshift;		/* reg offset shift */
	unsigned char		iotype;			/* io access style */
	unsigned char		unused1;

#define UPIO_PORT		(0)
#define UPIO_HUB6		(1)
#define UPIO_MEM		(2)
#define UPIO_MEM32		(3)
#define UPIO_AU			(4)			/* Au1x00 type IO */
#define UPIO_TSI		(5)			/* Tsi108/109 type IO */
#define UPIO_DWAPB		(6)			/* DesignWare APB UART */
#define UPIO_RM9000		(7)			/* RM9000 type IO */
#define UPIO_DWAPB32		(8)			/* DesignWare APB UART (32 bit accesses) */

	unsigned int		read_status_mask;	/* driver specific */
	unsigned int		ignore_status_mask;	/* driver specific */
	struct uart_state	*state;			/* pointer to parent state */
	struct uart_icount	icount;			/* statistics */

	struct console		*cons;			/* struct console, if any */
#if defined(CONFIG_SERIAL_CORE_CONSOLE) || defined(SUPPORT_SYSRQ)
	unsigned long		sysrq;			/* sysrq timeout */
#endif

	upf_t			flags;

#define UPF_FOURPORT		((__force upf_t) (1 << 1))
#define UPF_SAK			((__force upf_t) (1 << 2))
#define UPF_SPD_MASK		((__force upf_t) (0x1030))
#define UPF_SPD_HI		((__force upf_t) (0x0010))
#define UPF_SPD_VHI		((__force upf_t) (0x0020))
#define UPF_SPD_CUST		((__force upf_t) (0x0030))
#define UPF_SPD_SHI		((__force upf_t) (0x1000))
#define UPF_SPD_WARP		((__force upf_t) (0x1010))
#define UPF_SKIP_TEST		((__force upf_t) (1 << 6))
#define UPF_AUTO_IRQ		((__force upf_t) (1 << 7))
#define UPF_HARDPPS_CD		((__force upf_t) (1 << 11))
#define UPF_LOW_LATENCY		((__force upf_t) (1 << 13))
#define UPF_BUGGY_UART		((__force upf_t) (1 << 14))
#define UPF_NO_TXEN_TEST	((__force upf_t) (1 << 15))
#define UPF_MAGIC_MULTIPLIER	((__force upf_t) (1 << 16))
#define UPF_CONS_FLOW		((__force upf_t) (1 << 23))
#define UPF_SHARE_IRQ		((__force upf_t) (1 << 24))
/* The exact UART type is known and should not be probed.  */
#define UPF_FIXED_TYPE		((__force upf_t) (1 << 27))
#define UPF_BOOT_AUTOCONF	((__force upf_t) (1 << 28))
#define UPF_FIXED_PORT		((__force upf_t) (1 << 29))
#define UPF_DEAD		((__force upf_t) (1 << 30))
#define UPF_IOREMAP		((__force upf_t) (1 << 31))

#define UPF_CHANGE_MASK		((__force upf_t) (0x17fff))
#define UPF_USR_MASK		((__force upf_t) (UPF_SPD_MASK|UPF_LOW_LATENCY))

	unsigned int		mctrl;			/* current modem ctrl settings */
	unsigned int		timeout;		/* character-based timeout */
	unsigned int		type;			/* port type */
	const struct uart_ops	*ops;
	unsigned int		custom_divisor;
	unsigned int		line;			/* port index */
	resource_size_t		mapbase;		/* for ioremap */
	struct device		*dev;			/* parent device */
	unsigned char		hub6;			/* this should be in the 8250 driver */
	unsigned char		suspended;
	unsigned char		irq_wake;
	unsigned char		unused[2];
	void			*private_data;		/* generic platform data pointer */
};
#endif

#define MSM_UART_RXD 40
#define MSM_UART_TXD 42
#define MSM_USAR_CTS 38
#define MSM_UART_RTS 36




#define regbase MSM_GSBI5_PHYS

class Krait_USART
{

public:

	//char	*name;
	UINT32	 ver_id;
	UINT32   tx_timeout;
	GPIO_PIN rx_pin;
	GPIO_PIN tx_pin;
	GPIO_PIN rts_pin;
	GPIO_PIN cts_pin;
	UINT32   membase;
	UINT32   fifosize;
	// Statistic information
	struct uart_icount icount;
	UINT32 index;
	UINT32 imr;
	UINT32 uartclk;
	struct rcg_clk clkconfig;

	// Need to understand how this happens
	char x_char;
	BOOL clkenable;



	BOOL InitializePins()
	{
		if(index == 0)
		{
			rx_pin = (GPIO_PIN) MSM_UART_RXD;
			tx_pin = (GPIO_PIN) MSM_UART_TXD;
			rts_pin = (GPIO_PIN) MSM_UART_RTS;
			cts_pin = (GPIO_PIN) MSM_USAR_CTS;
			// Unsure if this the base address of the uart
		}
	}

	BOOL SetMemBase()
	{
		if(index == 0)
		membase = MSM_GSBI5_PHYS;
	}

	void SetUartClock(int val)
	{
		uartclk = val;
	}

	void InitializeClockConfig()
	{
		clkconfig.ns_mask = (BM(31, 16) | BM(6, 0));
		clkconfig.ns_reg = (void *) GSBIn_UART_APPS_NS_REG(5);
		clkconfig.root_en_mask = BIT(11);
		//clkconfig.ns_val = NS(31, 16, n, m, 5, 4, 3, d, 2, 0, s##_to_bb_mux);
		// Initializing this to 0 for now, lets see if this works
		clkconfig.current_freq->ns_val = 0;
		UINT32 lctl_reg = GSBIn_UART_APPS_NS_REG(5);
		clkconfig.b.ctl_reg = (void *) lctl_reg;
		clkconfig.b.en_mask = BIT(9);
		UINT32 lreset_reg = GSBIn_RESET_REG(5);
		clkconfig.b.reset_reg = (void *) lreset_reg;
		clkconfig.b.reset_mask = BIT(0);
		UINT32 lhalt_reg = CLK_HALT_CFPB_STATEB_REG;
		clkconfig.b.halt_reg = (void *) lhalt_reg;
		clkconfig.b.halt_bit = 22;
	}

	BOOL Initialize()
	{

		InitializePins();

		SetMemBase();

		SetUartClock(19200000);

		InitializeClockConfig();

		fifosize = 512;

		clkenable = false;

		imr = 0;

	}

	void SetIndex(int index)
	{
		this->index = index;
	}

	void ClockEnable()
	{
		if(!clkenable)
		{
			//uwr(0xC0, UART_MREG);
			//uwr(0xAF, UART_NREG);
			//uwr(0x80, UART_DREG);
			//uwr(0x19, UART_MNDREG);
			//clock_enable(32);
			//__rcg_clk_enable_reg(&clkconfig);

			clkenable = TRUE;
		}

	}

	void ClockDisable()
	{

	}

};

class Krait_USART_Driver
{
public:
	Krait_USART usart[2];

	BOOL Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue );
	BOOL Uninitialize( int ComPortNum);
	BOOL TxBufferEmpty( int ComPortNum);
	BOOL TxShiftRegisterEmpty( int ComPortNum);
	void WriteCharToTxBuffer( int ComPortNum, UINT8 c );
	void TxBufferEmptyInterruptEnable(int ComPortNum,BOOL Enable );
	BOOL TxBufferEmptyInterruptState(  int ComPortNum );
	void RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable );
	BOOL RxBufferFullInterruptState( int ComPortNum );
	BOOL TxHandshakeEnabledState(int ComPortNum);
	BOOL ProtectPins( int ComPortNum, BOOL On );
	void GetPins(int ComPortNum,  GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin);
	void BaudrateBoundary(int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz );
	BOOL IsBaudrateSupported( int ComPortNum, UINT32& BaudrateHz );

	BOOL HandleInterrupt(void *args);
};


#endif
