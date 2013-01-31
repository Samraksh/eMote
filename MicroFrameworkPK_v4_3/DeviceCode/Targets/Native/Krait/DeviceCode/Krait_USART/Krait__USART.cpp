/*
 * 	Name 	:  Krait__USART.cpp
 *
 *  Author  :  nived.sivadas@samraksh.com
 *
 *  Description : contains the implementation of the usart driver
 */
#include <tinyhal.h>
#include <USART_decl.h>
#include "..\Krait.h"
#include "Krait__USART.h"
#include "gsbi.h"
#include "uart_dm.h"

extern Krait_USART_Driver g_Krait_USART_Driver;

extern unsigned int msm_boot_uart_dm_write(UINT8 id, char *data, unsigned int num_of_chars);
extern unsigned int msm_boot_uart_dm_gsbi_init(UINT8 id);
extern unsigned int msm_boot_uart_dm_reset(UINT8 id);
extern unsigned int msm_boot_uart_dm_init_rx_transfer(UINT8 id);
extern int uart_putc(int port, char c);

extern "C"
{

void USART_Handler(void *args)
{
	g_Krait_USART_Driver.HandleInterrupt(args);
}

}

static inline void msm_write(struct Krait_USART *port, UINT32 val, UINT32 off)
{
	__raw_writel(val, port->membase + off);
}


static inline UINT32 msm_read(struct Krait_USART *port, UINT32 off)
{
	return __raw_readl((volatile void *) port->membase + off);
}

// This function sets the baud rate of the interesting uart default value is 28800
// Similar to the function msm_hsl_set_baud_rate from the linux kernel
static void SetBaudRate(Krait_USART *port, UINT32 BaudRate)
{
	UINT32 baud_code, watermark;
	UINT32 rxstale;
	UINT32 vid;

	switch (BaudRate) {
		case 300:
			baud_code = UART_CSR_300;
			rxstale = 1;
			break;
		case 600:
			baud_code = UART_CSR_600;
			rxstale = 1;
			break;
		case 1200:
			baud_code = UART_CSR_1200;
			rxstale = 1;
			break;
		case 2400:
			baud_code = UART_CSR_2400;
			rxstale = 1;
			break;
		case 4800:
			baud_code = UART_CSR_4800;
			rxstale = 1;
			break;
		case 9600:
			baud_code = UART_CSR_9600;
			rxstale = 2;
			break;
		case 14400:
			baud_code = UART_CSR_14400;
			rxstale = 3;
			break;
		case 19200:
			baud_code = UART_CSR_19200;
			rxstale = 4;
			break;
		case 28800:
			baud_code = UART_CSR_28800;
			rxstale = 6;
			break;
		case 38400:
			baud_code = UART_CSR_38400;
			rxstale = 8;
			break;
		case 57600:
			baud_code = UART_CSR_57600;
			rxstale = 16;
			break;
		case 115200:
		default:
			baud_code = UART_CSR_115200;
			rxstale = 31;
			break;
		}

		msm_write(port, baud_code, UART_CSR);

		/* RX stale watermark */
		watermark = UART_IPR_STALE_LSB & rxstale;
		watermark |= UART_IPR_RXSTALE_LAST;
		watermark |= UART_IPR_STALE_TIMEOUT_MSB & (rxstale << 2);
		msm_write(port, watermark, UART_IPR);

		/* set RX watermark */
		watermark = (port->fifosize * 3) / 4;
		msm_write(port, watermark, UART_RFWR);

		/* set TX watermark */
		msm_write(port, 10, UART_TFWR);
}

static void msm_reset(struct Krait_USART *port)
{
	/* reset everything */
	msm_write(port, UART_CR_CMD_RESET_RX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_TX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_BREAK_INT, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_CTS, UART_CR);
	msm_write(port, UART_CR_CMD_SET_RFR, UART_CR);

}

void msm_init_clock(struct Krait_USART *usartport)
{
	//Krait_USART *usartport = &g_Krait_USART_Driver.usart[ComPortNum];

	usartport->ClockEnable();

	//clk_enable(usartport->clk);

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	msm_port->clk_state = MSM_CLK_ON;
#endif

	if (usartport->uartclk == 19200000) {
		/* clock is TCXO (19.2MHz) */
		msm_write(usartport, 0x06, UART_MREG);
		msm_write(usartport, 0xF1, UART_NREG);
		msm_write(usartport, 0x0F, UART_DREG);
		msm_write(usartport, 0x1A, UART_MNDREG);
	} else {
		/* clock must be TCXO/4 */
		msm_write(usartport, 0x18, UART_MREG);
		msm_write(usartport, 0xF6, UART_NREG);
		msm_write(usartport, 0x0F, UART_DREG);
		msm_write(usartport, 0x0A, UART_MNDREG);
	}
}


BOOL Krait_USART_Driver::Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{

		CPU_USART_ProtectPins(ComPortNum, TRUE);

		Krait_USART *usartport = &usart[ComPortNum];

		UINT32 id = GSBI_ID_5;

		usartport->SetIndex(ComPortNum);

		//uart_init(GSBI_ID_5);
		msm_boot_uart_dm_gsbi_init(id);

		// Set the flow control
		writel(FlowValue, MSM_BOOT_UART_DM_MR1(GSBI_ID_5));

		if(DataBits == 8 && Parity == 0 && StopBits == 1)
		{
			/* 8-N-1 configuration: 8 data bits - No parity - 1 stop bit */
			writel(MSM_BOOT_UART_DM_8_N_1_MODE, MSM_BOOT_UART_DM_MR2(GSBI_ID_5));
		}
		else
		{
			UINT32 val = readl(MSM_BOOT_UART_DM_MR2(GSBI_ID_5));
			UINT32 reg;

			switch(DataBits)
			{
			case 5:
				reg = MSM_BOOT_UART_DM_5_BPS;
				break;
			case 6:
				reg = MSM_BOOT_UART_DM_6_BPS;
				break;
			case 7:
				reg = MSM_BOOT_UART_DM_7_BPS;
				break;
			case 8:
				reg = MSM_BOOT_UART_DM_8_BPS;
				break;
			default:
				ASSERT(0);
				break;
			}

			switch(StopBits)
			{
			case USART_STOP_BITS_ONE:
				reg |= MSM_BOOT_UART_DM_SBL_1;
				break;
			case USART_STOP_BITS_TWO:
				reg |= MSM_BOOT_UART_DM_SBL_2;
				break;
			default:
				ASSERT(0);
				break;
			}

			switch(Parity)
			{
			case USART_PARITY_NONE:
				reg |= MSM_BOOT_UART_DM_NO_PARITY;
				break;
			case USART_PARITY_ODD:
				reg |= MSM_BOOT_UART_DM_ODD_PARITY;
				break;
			case USART_PARITY_EVEN:
				reg |= MSM_BOOT_UART_DM_EVEN_PARITY;
				break;
			case USART_PARITY_MARK:
				break;
			case USART_PARITY_SPACE:
				reg |= MSM_BOOT_UART_DM_SPACE_PARITY;
				break;
			default:
				ASSERT(0);
				break;
			}

			writel((reg | val), MSM_BOOT_UART_DM_MR2(GSBI_ID_5));
		}

		/* Configure Interrupt Mask register IMR */
		//writel(MSM_BOOT_UART_DM_IMR_ENABLED, MSM_BOOT_UART_DM_IMR(GSBI_ID_5));
		writel(MSM_BOOT_UART_DM_IMR_ENABLED, MSM_BOOT_UART_DM_IMR(GSBI_ID_5));

		/* Configure Tx and Rx watermarks configuration registers */
		/* TX watermark value is set to 0 - interrupt is generated when
		 * FIFO level is less than or equal to 0 */
		writel(MSM_BOOT_UART_DM_TFW_VALUE, MSM_BOOT_UART_DM_TFWR(id));

		/* RX watermark value */
		writel(MSM_BOOT_UART_DM_RFW_VALUE, MSM_BOOT_UART_DM_RFWR(id));

		/* Configure Interrupt Programming Register */
		/* Set initial Stale timeout value */
		writel(MSM_BOOT_UART_DM_STALE_TIMEOUT_LSB, MSM_BOOT_UART_DM_IPR(id));

		/* Configure IRDA if required */
		/* Disabling IRDA mode */
		writel(0x0, MSM_BOOT_UART_DM_IRDA(id));

		/* Configure and enable sim interface if required */

		/* Configure hunt character value in HCR register */
		/* Keep it in reset state */
		writel(0x0, MSM_BOOT_UART_DM_HCR(id));

		/* Configure Rx FIFO base address */
		/* Both TX/RX shares same SRAM and default is half-n-half.
		 * Sticking with default value now.
		 * As such RAM size is (2^RAM_ADDR_WIDTH, 32-bit entries).
		 * We have found RAM_ADDR_WIDTH = 0x7f */

		/* Issue soft reset command */
		msm_boot_uart_dm_reset(id);

		/* Enable/Disable Rx/Tx DM interfaces */
		/* Data Mover not currently utilized. */
		writel(0x0, MSM_BOOT_UART_DM_DMEN(id));

		/* Enable transmitter and receiver */
		writel(MSM_BOOT_UART_DM_CR_RX_ENABLE, MSM_BOOT_UART_DM_CR(id));
		writel(MSM_BOOT_UART_DM_CR_TX_ENABLE, MSM_BOOT_UART_DM_CR(id));
		/* Initialize Receive Path */
		msm_boot_uart_dm_init_rx_transfer(id);

		CPU_INTC_ActivateInterrupt((UINT32) GSBI5_UARTDM_IRQ, USART_Handler, (void *) usartport->index);
		CPU_INTC_InterruptEnable( (UINT32) GSBI5_UARTDM_IRQ );


		hal_printf("Microframework 4.3 - UART_DM Initialized from USART PAL!!!\n");

		return TRUE;

#if 0
	UINT32 mr;

	// Get a pointer to the interesting port number
	Krait_USART *usartport = &usart[ComPortNum];

	usartport->SetIndex(ComPortNum);

	//usartport->uartclk = 19200000;

	usartport->SetUartClock(19200000);

	usartport->Initialize();

	msm_init_clock(usartport);

	usartport->ClockEnable();

	// Sets baud rate and water mark both RX and TX
	SetBaudRate(usartport, BaudRate);

	mr = msm_read(usartport, UART_MR2);

	mr &= ~UART_MR2_PARITY_MODE;

	// Return if the value of parity is invalid
	if(Parity > UART_MR2_PARITY_MODE_SPACE)
		return FALSE;

	switch(Parity)
	{
	case USART_PARITY_NONE:
		break;
	case USART_PARITY_ODD:
		mr |= UART_MR2_PARITY_MODE_ODD;
		break;
	case USART_PARITY_EVEN:
		mr |= UART_MR2_PARITY_MODE_EVEN;
		break;
	case USART_PARITY_MARK:
		break;
	case USART_PARITY_SPACE:
		mr |= UART_MR2_PARITY_MODE_SPACE;
		break;
	}

	mr &= ~UART_MR2_BITS_PER_CHAR;

	switch (DataBits)
	{
		case CS5:
			mr |= UART_MR2_BITS_PER_CHAR_5;
			break;
		case CS6:
			mr |= UART_MR2_BITS_PER_CHAR_6;
			break;
		case CS7:
			mr |= UART_MR2_BITS_PER_CHAR_7;
			break;
		case CS8:
		default:
			mr |= UART_MR2_BITS_PER_CHAR_8;
			break;
	}

	mr &= ~(UART_MR2_STOP_BIT_LEN_ONE | UART_MR2_STOP_BIT_LEN_TWO);

	switch(StopBits)
	{
		case USART_STOP_BITS_ONE:
			mr |= UART_MR2_STOP_BIT_LEN_ONE;
			break;
		case USART_STOP_BITS_TWO:
			mr |= UART_MR2_STOP_BIT_LEN_TWO;
			break;
	}

	msm_write(usartport, mr, UART_MR2);

	mr = msm_read(usartport, UART_MR1);
	mr &= ~(UART_MR1_CTS_CTL | UART_MR1_RX_RDY_CTL);

	// Not super excited by this but, since there is no documentation if the code is not expressive enough there
	// are no more moves at this point
	if(FlowValue)
	{
		mr |= UART_MR1_CTS_CTL;
		mr |= UART_MR1_RX_RDY_CTL;
		msm_write(usartport, mr, UART_MR1);
	}

	CPU_INTC_ActivateInterrupt((UINT32) MSM8960_GSBI2_UARTDM_IRQ, USART_Handler, (void *) usartport->index);
	CPU_INTC_InterruptEnable( (UINT32) MSM8960_GSBI2_UARTDM_IRQ );

	// Reset the port
	msm_reset(usartport);

	msm_write(usartport, 0x05, UART_CR);	/* enable TX & RX */

	/* turn on RX and CTS interrupts */
	usartport->imr = UART_IMR_RXLEV | UART_IMR_RXSTALE | UART_IMR_CURRENT_CTS;

	msm_write(usartport, usartport->imr, UART_IMR);

	usartport->ClockDisable();

	return TRUE;
#endif
}

void handle_rx(Krait_USART* port)
{
	UINT32 sr;

	// Handle Overrun
	if ((readl(MSM_BOOT_UART_DM_SR(GSBI_ID_5)) & MSM_BOOT_UART_DM_SR_UART_OVERRUN)) {
			port->icount.overrun++;
			USART_AddCharToRxBuffer(port->index, 0);
			writel(MSM_BOOT_UART_DM_CMD_RESET_RX, MSM_BOOT_UART_DM_CR(GSBI_ID_5));
	}

	while((sr = readl(MSM_BOOT_UART_DM_SR(GSBI_ID_5))) & MSM_BOOT_UART_DM_SR_RXRDY)
	{
		UINT32 c;

		c = readl(MSM_BOOT_UART_DM_RF(GSBI_ID_5, 0));

		if (sr & MSM_BOOT_UART_DM_RX_BREAK)
		{
			port->icount.brk++;
			//if (uart_handle_break(port))
			//		continue;
		}
		else if (sr & MSM_BOOT_UART_DM_SR_PAR_FRAME_ERR)
		{
			port->icount.frame++;

		}
		else
		{
			port->icount.rx++;
		}

		USART_AddCharToRxBuffer(port->index, c);
	}

}

void handle_tx(Krait_USART* port)
{
	char c;
	//char txdata[200];
	//int counter = 0;
	if(USART_RemoveCharFromTxBuffer(port->index, c))
	{
		//if(USART_BytesInBuffer(port->index, false) == 1)
		//{
		UINT32 reg = readl(MSM_BOOT_UART_DM_IMR(GSBI_ID_5));
		UINT32 mask = ~(MSM_BOOT_UART_DM_TXLEV);
		writel((reg & mask), MSM_BOOT_UART_DM_IMR(GSBI_ID_5));
		//}
		msm_boot_uart_dm_write(GSBI_ID_5, &c, 1);
	}
}

void handle_delta_cts(Krait_USART *port)
{
	msm_write(port, UART_CR_CMD_RESET_CTS, UART_CR);
	port->icount.cts++;
	//wake_up_interruptible(&port->state->port.delta_msr_wait);
}


BOOL Krait_USART_Driver::HandleInterrupt(void *args)
{
	UINT32 misr;
	// Need a better system than this
	Krait_USART *usartport = &usart[0];

	//usartport->ClockEnable();

	misr = readl(MSM_BOOT_UART_DM_ISR(GSBI_ID_5)) ;

	//msm_write(usartport, 0 , UART_IMR);
	writel(0, MSM_BOOT_UART_DM_IMR(GSBI_ID_5));

	if (misr & (MSM_BOOT_UART_DM_RXLEV | MSM_BOOT_UART_DM_RXSTALE))
		handle_rx(usartport);
	if (misr & MSM_BOOT_UART_DM_TXLEV)
		handle_tx(usartport);
	if (misr & MSM_BOOT_UART_DM_DELTA_CTS)
		handle_delta_cts(usartport);

	writel(MSM_BOOT_UART_DM_IMR_ENABLED, MSM_BOOT_UART_DM_IMR(GSBI_ID_5));
	//msm_write(usartport, usartport->imr, UART_IMR); /* restore interrupt */

	usartport->ClockDisable();

}


BOOL Krait_USART_Driver::Uninitialize( int ComPortNum)
{
	CPU_USART_ProtectPins(ComPortNum, FALSE);
	return TRUE;
}


BOOL Krait_USART_Driver::TxBufferEmpty( int ComPortNum)
{
#if 0
	UINT32 status;

	if(ComPortNum > 2)
		return FALSE;

	Krait_USART *usartport = &usart[ComPortNum];

	if((status = msm_read(usartport, UART_SR)) & UART_SR_TX_READY)
	{
		return TRUE;
	}
#endif
	return TRUE;
}

// The kernel code indicates that not all msm uarts have a TXDONE available or in other words there is not
// feature of the hardware that you can check this by. They have used a timer to accomplish this
// Implementing this function by using a timer or a slow down for loop
BOOL Krait_USART_Driver::TxShiftRegisterEmpty( int ComPortNum)
{

	UINT32 status;

	// I want to sleep for 1ms this is roughly the number indicated in the linux kernel code
	// This function is dummy since the timer module  is still under construction
	HAL_Time_Sleep_MicroSeconds(1000);

	// This may require some tuning and should be removed when the timer comes into play
	for(int i = 0; i < 10000; i++);

	return TRUE;
}



void Krait_USART_Driver::WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
	char inp = (char) c;
	msm_boot_uart_dm_write(GSBI_ID_5, &inp, 1);
}

void Krait_USART_Driver::TxBufferEmptyInterruptEnable(int ComPortNum,BOOL Enable )
{
	Krait_USART *usartport = &usart[ComPortNum];

/*
	char c;

	if(USART_RemoveCharFromTxBuffer(usartport->index, c))
		msm_boot_uart_dm_write(GSBI_ID_5, &c, 1);
*/
	UINT32 reg = readl(MSM_BOOT_UART_DM_IMR(GSBI_ID_5));
	UINT32 mask = MSM_BOOT_UART_DM_TXLEV;
	writel((reg | mask), MSM_BOOT_UART_DM_IMR(GSBI_ID_5));


}

BOOL Krait_USART_Driver::TxBufferEmptyInterruptState(  int ComPortNum )
{
	Krait_USART *usartport = &usart[ComPortNum];

	if(readl(MSM_BOOT_UART_DM_IMR(GSBI_ID_5)) & MSM_BOOT_UART_DM_TX_READY)
		return TRUE;
	else
		return FALSE;
}

void Krait_USART_Driver::RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
	Krait_USART *usartport = &usart[ComPortNum];

	UINT32 id = GSBI_ID_5;

	if(Enable)
	{
		writel(MSM_BOOT_UART_DM_RXLEV, MSM_BOOT_UART_DM_IMR(id));
	}
	else
	{
		UINT32 reg = readl(MSM_BOOT_UART_DM_IMR(id));
		UINT32 mask = ~(MSM_BOOT_UART_DM_RXLEV);
		writel((reg & mask), MSM_BOOT_UART_DM_IMR(id));
	}


}

BOOL Krait_USART_Driver::RxBufferFullInterruptState( int ComPortNum )
{
	UINT32 id = GSBI_ID_5;

	// Not sure if RxBufferFull is same RXRDY
	if(readl(MSM_BOOT_UART_DM_SR(id)) & MSM_BOOT_UART_DM_SR_RXRDY)
		return TRUE;
	else
		return FALSE;

}

BOOL Krait_USART_Driver::TxHandshakeEnabledState(int ComPortNum)
{

}

BOOL Krait_USART_Driver::ProtectPins( int ComPortNum, BOOL On )
{
	UINT32 id;
	if(ComPortNum == 0)
		id = GSBI_ID_5;

	if(id == GSBI_ID_5)
	{
		if(On)
		{
			CPU_GPIO_ReservePin((GPIO_PIN)22, TRUE);
			CPU_GPIO_ReservePin((GPIO_PIN)23, TRUE);
		}
		else
		{
			CPU_GPIO_ReservePin((GPIO_PIN)22, FALSE);
			CPU_GPIO_ReservePin((GPIO_PIN)23, FALSE);
		}
	}
}

void Krait_USART_Driver::GetPins(int ComPortNum,  GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin)
{
	UINT32 id;

	if(ComPortNum == 0)
		id = GSBI_ID_5;

	if(id == GSBI_ID_5)
	{
		rxPin = 23;
		txPin = 22;
		// the ctsPin and rtsPin information are not available at this point
	}

}

void Krait_USART_Driver::BaudrateBoundary(int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz )
{
	if(ComPortNum == 0)
	{
		maxBaudrateHz = 115200;
		minBaudrateHz = 57600;
	}

}

BOOL Krait_USART_Driver::IsBaudrateSupported( int ComPortNum, UINT32& BaudrateHz )
{
	if(BaudrateHz == 57600)
		return TRUE;
	else
		return FALSE;
}
