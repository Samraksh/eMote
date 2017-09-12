////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <max3263x.h>
#include <uart.h>
#include <ioman.h>
#include "sam_usart.h"
#include <gpio\netmf_gpio.h>

#define MAX_BAUDRATE 115200
#define MIN_BAUDRATE 0

#define BUFF_SIZE 32

#define IOMAN_UART_FUNC(obj, i, im, cm, rm, ien, cen, ren) {                                                   \
        obj.req_reg = (uint32_t*)((unsigned int)(&MXC_IOMAN->uart0_req) + (i * 2*sizeof(uint32_t)));    \
        obj.ack_reg = (uint32_t*)((unsigned int)(&MXC_IOMAN->uart0_ack) + (i * 2*sizeof(uint32_t)));    \
        obj.req_val.uart.io_map = im;                                                              \
        obj.req_val.uart.cts_map = cm;                                                             \
        obj.req_val.uart.rts_map = rm;                                                             \
        obj.req_val.uart.io_req = ien;                                                             \
        obj.req_val.uart.cts_io_req = cen;                                                         \
        obj.req_val.uart.rts_io_req = ren;		\
}



void USART0_Handler(void *args);
void USART1_Handler(void *args);
static int PORTS_IN_USE_MASK = 0;
//Max326_USART_Driver g_max326_usart_Driver;

uint8_t txdata_com0[BUFF_SIZE];
uint8_t txdata_com1[BUFF_SIZE];
uart_req_t write_req0,write_req1;

uint32_t com0_current_baud, com1_current_baud;
uint32_t com0_current_parity, com1_current_parity;
uint32_t com0_current_databits, com1_current_databits;
uint32_t com0_current_stopbits, com1_current_stopbits;
uint32_t com0_current_flowcontrol, com1_current_flowcontrol;

//ComHandle != ComPort.  COM1 is a handle with port=0. COM1=0x101 means port 0 on USART transport.  See platform_selector.h and tinyhal.h.

#define UART_ERRORS             (MXC_F_UART_INTEN_RX_FIFO_OVERFLOW  | \
                                MXC_F_UART_INTEN_RX_FRAMING_ERR | \
                                MXC_F_UART_INTEN_RX_PARITY_ERR)

#define UART_READ_INTS          (MXC_F_UART_INTEN_RX_FIFO_AF |  \
                                MXC_F_UART_INTEN_RX_FIFO_NOT_EMPTY | \
                                MXC_F_UART_INTEN_RX_STALLED | \
                                UART_ERRORS)

#define UART_WRITE_INTS         (MXC_F_UART_INTEN_TX_UNSTALLED | \
                                MXC_F_UART_INTEN_TX_FIFO_AE)

void USART0_Handler(void *args) {
	char buf[BUFF_SIZE];
	int bufIndex;
	int uart_num = 0;
	int bufSize = 0;
    uint32_t flags;
	mxc_uart_fifo_regs_t *fifo = MXC_UART0_FIFO;
	int avail;

	GLOBAL_LOCK(irq);

    flags = MXC_UART0->intfl;
    MXC_UART0->intfl = flags;

    if(flags & UART_READ_INTS) {
		// Disable interrupts
    	MXC_UART0->inten &= ~UART_READ_INTS;
		
    	avail = UART_NumReadAvail(MXC_UART0);
		bufSize = avail;
		if (avail < BUFF_SIZE) {
			bufIndex = 0;
			while(avail) {
				buf[bufIndex] = fifo->rx; 
				bufIndex++;
    	    	avail--;
    		}
			USART_AddToRxBuffer( ConvertCOM_ComPort(COM1), buf, bufSize);
    	    	
		} else if (avail >= BUFF_SIZE) {
			for (int i = 0; i < BUFF_SIZE; i++) {
				buf[i] = fifo->rx; 
				bufIndex++;
			}
			USART_AddToRxBuffer( ConvertCOM_ComPort(COM1), buf, BUFF_SIZE);
		}
		MXC_UART0->inten |= UART_READ_INTS;
    }

    // Figure out if this UART has an active Write request
    if((flags & (MXC_F_UART_INTEN_TX_UNSTALLED | MXC_F_UART_INTEN_TX_FIFO_AE))) {
        int avail, remain;

    	// Disable write interrupts
    	MXC_UART0->inten &= ~(UART_WRITE_INTS);

    	// Refill the TX FIFO
    	avail = UART_NumWriteAvail(MXC_UART0);
    	remain = write_req0.len - write_req0.num;

    	while(avail && remain) {

        	// Write the data to the FIFO
#if(MXC_UART_REV == 0)
        	MXC_UART0->intfl = MXC_F_UART_INTFL_TX_DONE;
#endif
        	fifo->tx = write_req0.data[write_req0.num++];
        	remain--;
        	avail--;
    	}

    	// All of the bytes have been written to the FIFO
    	if(!remain) {

        	int i=0;
			for (i=0; i < BUFF_SIZE; i++){
				char c;
				if(!USART_RemoveCharFromTxBuffer(ConvertCOM_ComPort(COM1), c)){
					break;
				}
				txdata_com0[i]=c;
			}

			if (i>0){
				write_req0.len=i; 
				write_req0.num=0;
				UART_WriteAsync(MXC_UART0, &write_req0);
			}
	    } else {

	        // Interrupt when there is one byte left in the TXFIFO
	        MXC_UART0->tx_fifo_ctrl = ((MXC_UART_FIFO_DEPTH - 1) << MXC_F_UART_TX_FIFO_CTRL_FIFO_AE_LVL_POS);

	        // Enable almost empty interrupt	
	        MXC_UART0->inten |= (UART_WRITE_INTS);
    	}
    }
}

void USART1_Handler(void *args)
{
	char buf[BUFF_SIZE];
	int bufIndex;
	int uart_num = 1;
	int bufSize = 0;
    uint32_t flags;
	mxc_uart_fifo_regs_t *fifo = MXC_UART1_FIFO;
	int avail;

	GLOBAL_LOCK(irq);

    flags = MXC_UART1->intfl;
    MXC_UART1->intfl = flags;

    if(flags & UART_READ_INTS) {
		// Disable interrupts
    	MXC_UART1->inten &= ~UART_READ_INTS;
		
    	avail = UART_NumReadAvail(MXC_UART1);
		bufSize = avail;
		if (avail < BUFF_SIZE) {
			bufIndex = 0;
			while(avail) {
				buf[bufIndex] = fifo->rx; 
				bufIndex++;
    	    	avail--;
    		}
			USART_AddToRxBuffer( ConvertCOM_ComPort(COM1), buf, bufSize);
    	    	
		} else if (avail >= BUFF_SIZE) {
			for (int i = 0; i < BUFF_SIZE; i++) {
				buf[i] = fifo->rx; 
				bufIndex++;
			}
			USART_AddToRxBuffer( ConvertCOM_ComPort(COM1), buf, BUFF_SIZE);
		}
		MXC_UART1->inten |= UART_READ_INTS;
    }

	// Figure out if this UART has an active Write request
    if((flags & (MXC_F_UART_INTEN_TX_UNSTALLED | MXC_F_UART_INTEN_TX_FIFO_AE))) {
        int avail, remain;

    	// Disable write interrupts
    	MXC_UART1->inten &= ~(UART_WRITE_INTS);

    	// Refill the TX FIFO
    	avail = UART_NumWriteAvail(MXC_UART1);
    	remain = write_req1.len - write_req1.num;

    	while(avail && remain) {

        	// Write the data to the FIFO
#if(MXC_UART_REV == 0)
        	MXC_UART1->intfl = MXC_F_UART_INTFL_TX_DONE;
#endif
        	fifo->tx = write_req1.data[write_req1.num++];
        	remain--;
        	avail--;
    	}

    	// All of the bytes have been written to the FIFO
    	if(!remain) {

        	int i=0;
			for (i=0; i < BUFF_SIZE; i++){
				char c;
				if(!USART_RemoveCharFromTxBuffer(ConvertCOM_ComPort(COM1), c)){
					break;
				}
				txdata_com0[i]=c;
			}

			if (i>0){
				write_req1.len=i; 
				write_req1.num=0;
				UART_WriteAsync(MXC_UART1, &write_req1);
			}
	    } else {

	        // Interrupt when there is one byte left in the TXFIFO
	        MXC_UART1->tx_fifo_ctrl = ((MXC_UART_FIFO_DEPTH - 1) << MXC_F_UART_TX_FIFO_CTRL_FIFO_AE_LVL_POS);

	        // Enable almost empty interrupt	
	        MXC_UART1->inten |= (UART_WRITE_INTS);
    	}
    }
}


	//////////////////////Non extern C functions ///////////////


// Note that once this goes up, it stays up.
static BOOL init_com0(int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue) {
	// Initialize the UART
	uart_cfg_t cfg;
	cfg.parity = UART_PARITY_DISABLE; // no parity
	cfg.size = UART_DATA_SIZE_8_BITS; //8 data bits
	cfg.extra_stop = 0; 
	cfg.cts = 0;  //No hardware flow control
	cfg.rts = 0;
	cfg.baud = BaudRate; 

	sys_cfg_uart_t sys_cfg;
	sys_cfg.clk_scale = CLKMAN_SCALE_AUTO;
	IOMAN_UART_FUNC(sys_cfg.io_cfg, 0, IOMAN_MAP_A, IOMAN_MAP_UNUSED, IOMAN_MAP_UNUSED, 1, 0, 0);

	while(UART_Busy(MXC_UART0)){}
	while(UART_Busy(MXC_UART1)){}

	uint32_t error = UART_Init(MXC_UART0, &cfg, &sys_cfg);
	
	if(error != E_NO_ERROR) {
		return FALSE;
	}

	write_req0.data = txdata_com0;
	write_req0.len = 0;

	if(!CPU_INTC_ActivateInterrupt(UART0_IRQn, USART0_Handler, NULL) ) return FALSE;

	PORTS_IN_USE_MASK |= 1;

	com0_current_baud = BaudRate;
	com0_current_parity = Parity;
	com0_current_databits = DataBits;
	com0_current_stopbits = StopBits;
	com0_current_flowcontrol = FlowValue;

	// enabling read interrupts
	MXC_UART0->inten |= UART_READ_INTS;

	return TRUE;
}

static BOOL init_com1(int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue) {
	// Initialize the UART
	uart_cfg_t cfg;
	cfg.parity = UART_PARITY_DISABLE; // no parity
	cfg.size = UART_DATA_SIZE_8_BITS; //8 data bits
	cfg.extra_stop = 0; 
	cfg.cts = 0;  //No hardware flow control
	cfg.rts = 0;
	cfg.baud = BaudRate; 

	sys_cfg_uart_t sys_cfg;
	sys_cfg.clk_scale = CLKMAN_SCALE_AUTO;
	IOMAN_UART_FUNC(sys_cfg.io_cfg, 1, IOMAN_MAP_A,  IOMAN_MAP_A, IOMAN_MAP_A, 1, 1, 1);

	while(UART_Busy(MXC_UART0)){}
	while(UART_Busy(MXC_UART1)){}

	uint32_t error = UART_Init(MXC_UART1, &cfg, &sys_cfg);
	if(error != E_NO_ERROR) {
		return FALSE;
	}
	write_req1.data = txdata_com1;
	write_req1.len = 0;

	if(!CPU_INTC_ActivateInterrupt(UART1_IRQn, USART1_Handler, NULL) ) return FALSE;

	PORTS_IN_USE_MASK |= 2;

	com1_current_baud = BaudRate;
	com1_current_parity = Parity;
	com1_current_databits = DataBits;
	com1_current_stopbits = StopBits;
	com1_current_flowcontrol = FlowValue;

	return TRUE;
}

void USART_pause(void) {
	if (PORTS_IN_USE_MASK & 0x1) {
		CPU_USART_TxBufferEmptyInterruptEnable(0, false);
		while( CPU_USART_TxShiftRegisterEmpty(0) == FALSE );
	}

	if (PORTS_IN_USE_MASK & 0x2) {
		CPU_USART_TxBufferEmptyInterruptEnable(1, false);
		while( CPU_USART_TxShiftRegisterEmpty(1) == FALSE );
	}
}

// Re-init the UART.
// Necessary when changing power levels, for example.
void USART_reinit(void) {
	if (PORTS_IN_USE_MASK & 0x1) {
		CPU_USART_Uninitialize(0);
		init_com0(com0_current_baud, com0_current_parity, com0_current_databits, com0_current_stopbits, com0_current_flowcontrol);
	}

	if (PORTS_IN_USE_MASK & 0x2) {
		CPU_USART_Uninitialize(1);
		init_com1(com1_current_baud, com1_current_parity, com1_current_databits, com1_current_stopbits, com1_current_flowcontrol);
	}
}




BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
	// Check to make sure not already up.
	if (ComPortNum == 0 && (PORTS_IN_USE_MASK&1)) return TRUE;
	if (ComPortNum == 1 && (PORTS_IN_USE_MASK&2)) return TRUE;

	// If COM0, don't initialize until we see something connected (i.e. usb-serial attached)
	if (ComPortNum == 0) {
		return init_com0(BaudRate, Parity, DataBits, StopBits, FlowValue);

	}

	if (ComPortNum == 1) {
		return init_com1(BaudRate, Parity, DataBits, StopBits, FlowValue);
	}

	return FALSE;
}

// Not sure of a scenario where this fails
BOOL CPU_USART_Uninitialize( int ComPortNum )
{
	switch(ComPortNum)
	{
	case 0:
		PORTS_IN_USE_MASK &= ~(0x1);
		CPU_INTC_DeactivateInterrupt(UART0_IRQn);
		UART_Shutdown(MXC_UART_GET_UART(ComPortNum));
		break;
	case 1:
	default:
		PORTS_IN_USE_MASK &= ~(0x2);
		CPU_INTC_DeactivateInterrupt(UART1_IRQn);
		UART_Shutdown(MXC_UART_GET_UART(ComPortNum));
		break;
	}
	
	
	return TRUE;
}


// check if tx buffer is empty
BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
	return (UART_NumWriteAvail(MXC_UART_GET_UART(ComPortNum))==BUFF_SIZE) ;
}

// if transmission is complete the Shift Register is empty
BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
	return (UART_NumWriteAvail(MXC_UART_GET_UART(ComPortNum))==BUFF_SIZE) ;
}

// Write char into the data register
void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
	if(ComPortNum==0){ 
		write_req0.len=1;
		write_req0.num=0;
		txdata_com0[0]=c;
		UART_WriteAsync(MXC_UART_GET_UART(ComPortNum) , &write_req0);
	} else if(ComPortNum==1){ 
		write_req1.len=1;
		write_req1.num=0;
		txdata_com1[0]=c;
		UART_WriteAsync(MXC_UART_GET_UART(ComPortNum) , &write_req1);
	}
}


void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
	if (Enable){
		// this function basically is having us check to see if there is data to be sent out
		while (CPU_USART_TxBufferRoomAvailable(ComPortNum)) {
			char c;
			GLOBAL_LOCK(irq);
			if (USART_RemoveCharFromTxBuffer( ComPortNum, c )) {
				CPU_USART_WriteCharToTxBuffer( ComPortNum, c );				
			} else {
				return;
			}
		}
	}


	/*mxc_uart_regs_t *uart;
	if(ComPortNum==0){ 
		uart = MXC_UART_GET_UART(ComPortNum);
		// Enable almost empty interrupt
		uart->inten |= (MXC_F_UART_INTEN_TX_FIFO_AE);
	} else if(ComPortNum==1){ 
		uart = MXC_UART_GET_UART(ComPortNum);
		// Enable almost empty interrupt
		uart->inten |= (MXC_F_UART_INTEN_TX_FIFO_AE);
	}*/
}

BOOL CPU_USART_TxBufferRoomAvailable(int ComPortNum)
{
	return (UART_NumWriteAvail(MXC_UART_GET_UART(ComPortNum)) > 0);
}

// Returns TRUE if interrupt is enabled, NOT their current state.
BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{

	return (UART_NumWriteAvail(MXC_UART_GET_UART(ComPortNum))==0) ;
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
	return (UART_NumReadAvail(MXC_UART_GET_UART(ComPortNum))==BUFF_SIZE) ;
}

BOOL CPU_USART_TxHandshakeEnabledState( int comPort )
{
	// No handshake
    return FALSE;
}

void CPU_USART_ProtectPins( int ComPortNum, BOOL On )
{
	// Nothing needed here.
}

UINT32 CPU_USART_PortsCount()
{
    return TOTAL_USART_PORT;
}

void CPU_USART_GetPins( int ComPortNum, GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{   
}

BOOL CPU_USART_SupportNonStandardBaudRate (int ComPortNum )
{
    return FALSE;
}

void CPU_USART_GetBaudrateBoundary( int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz )
{
    maxBaudrateHz = MAX_BAUDRATE;
	minBaudrateHz = MIN_BAUDRATE;
}

BOOL CPU_USART_IsBaudrateSupported( int ComPortNum, UINT32& BaudrateHz )
{
  if (BaudrateHz <= 115200)
  {
    return true;
  }
  else
  {
    BaudrateHz = MAX_BAUDRATE;
    return false;
  }
}


