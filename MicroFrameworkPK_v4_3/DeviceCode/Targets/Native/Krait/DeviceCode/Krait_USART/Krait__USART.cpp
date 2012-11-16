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



		Krait_USART *usartport = &usart[ComPortNum];

		usartport->SetIndex(ComPortNum);

		CPU_INTC_ActivateInterrupt((UINT32) GSBI5_UARTDM_IRQ, USART_Handler, (void *) usartport->index);
		CPU_INTC_InterruptEnable( (UINT32) GSBI5_UARTDM_IRQ );

		uart_init(GSBI_ID_5);

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
	while(USART_RemoveCharFromTxBuffer(port->index, c))
	{
		uart_putc(GSBI_ID_5, c);
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
	return TRUE;
}


BOOL Krait_USART_Driver::TxBufferEmpty( int ComPortNum)
{
	UINT32 status;

	if(ComPortNum > 2)
		return FALSE;

	Krait_USART *usartport = &usart[ComPortNum];

	if((status = msm_read(usartport, UART_SR)) & UART_SR_TX_READY)
	{
		return TRUE;
	}

	return FALSE;
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

}



void Krait_USART_Driver::WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
	if(ComPortNum > 1)
		return;

	Krait_USART *usartport = &usart[ComPortNum];

	msm_write(usartport, c, UARTDM_TF);

}

void Krait_USART_Driver::TxBufferEmptyInterruptEnable(int ComPortNum,BOOL Enable )
{
	Krait_USART *usartport = &usart[ComPortNum];

	char c;
#if 0
	usartport->ClockEnable();

	if(Enable)
	{
		writel(MSM_BOOT_UART_DM_TX_READY | MSM_BOOT_UART_DM_TX_DONE | MSM_BOOT_UART_DM_TXLEV, MSM_BOOT_UART_DM_IMR(GSBI_ID_5));
	}
	else
	{
		UINT32 mask = readl(MSM_BOOT_UART_DM_IMR(GSBI_ID_5));
		mask &= ~(MSM_BOOT_UART_DM_TX_READY | MSM_BOOT_UART_DM_TX_DONE | MSM_BOOT_UART_DM_TXLEV);
		writel(mask,MSM_BOOT_UART_DM_IMR(GSBI_ID_5));
	}

	writel(1, MSM_BOOT_UART_DM_NO_CHARS_FOR_TX(GSBI_ID_5));

	/* Clear TX_READY interrupt */
	writel(MSM_BOOT_UART_DM_GCMD_RES_TX_RDY_INT, MSM_BOOT_UART_DM_CR(GSBI_ID_5));
#endif

	if(USART_RemoveCharFromTxBuffer(usartport->index, c))
		uart_putc(GSBI_ID_5, c);

	usartport->ClockDisable();

}

BOOL Krait_USART_Driver::TxBufferEmptyInterruptState(  int ComPortNum )
{
	Krait_USART *usartport = &usart[ComPortNum];

	UINT32 misr =  msm_read(usartport, UART_IMR);

	if(misr & UART_IMR_TXLEV)
		return TRUE;
	else
		return FALSE;
}

void Krait_USART_Driver::RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
	Krait_USART *usartport = &usart[ComPortNum];

	usartport->ClockEnable();

	if(Enable)
	{
		usartport->imr |= UART_IMR_RXLEV;
	}
	else
	{
		usartport->imr &= ~UART_IMR_RXLEV;
	}

	msm_write(usartport, usartport->imr, UART_IMR);

	usartport->ClockDisable();


}

BOOL Krait_USART_Driver::RxBufferFullInterruptState( int ComPortNum )
{
	Krait_USART *usartport = &usart[ComPortNum];

	UINT32 misr =  msm_read(usartport, UART_IMR);

	if(misr & UART_IMR_RXLEV)
		return TRUE;
	else
		return FALSE;
}

BOOL Krait_USART_Driver::TxHandshakeEnabledState(int ComPortNum)
{

}

BOOL Krait_USART_Driver::ProtectPins( int ComPortNum, BOOL On )
{

}

void Krait_USART_Driver::GetPins(int ComPortNum,  GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin)
{

}

void Krait_USART_Driver::BaudrateBoundary(int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz )
{

}

BOOL Krait_USART_Driver::IsBaudrateSupported( int ComPortNum, UINT32& BaudrateHz )
{

}
