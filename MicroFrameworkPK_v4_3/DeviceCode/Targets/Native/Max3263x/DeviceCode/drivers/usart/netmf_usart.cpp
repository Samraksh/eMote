////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <max3263x.h>
//#include <netmf_usart.h>
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
uint8_t rxdata_com0[BUFF_SIZE];
uint8_t txdata_com1[BUFF_SIZE];
uint8_t rxdata_com1[BUFF_SIZE];
uart_req_t read_req0,read_req1;
uart_req_t write_req0,write_req1;

//ComHandle != ComPort.  COM1 is a handle with port=0. COM1=0x101 means port 0 on USART transport.  See platform_selector.h and tinyhal.h.

#ifdef __cplusplus
extern "C" {
#endif

void read_cb(uart_req_t* req, int error, int comPort){
	if(error!=E_NO_ERROR){
		return;
	}
	if(req->num > 0){
		for (int i=0 ; i< req->num; i++){
			USART_AddCharToRxBuffer(ConvertCOM_ComPort(comPort), req->data[i]);
		}
	}
}

void write_cb(uart_req_t* req, int error, int comPort){

	/*if(error!=E_NO_ERROR){
		return;
	}*/
	bool preWriteIncomplete=false;
	if(req->num < req->len){
		preWriteIncomplete=true;
		CPU_GPIO_TogglePinState(24);
	}

	//read bytes from transmit buffer -> FALSE, true means rx buffer
/*	int charPending=USART_BytesInBuffer(comPort,FALSE);

	int i=0;
	for (i=0; i < charPending && i < BUFF_SIZE; i++){
		char c;
		if(!USART_RemoveCharFromTxBuffer(ConvertCOM_ComPort(comPort), c)){
			break;
		}
		if(comPort==0)  rxdata_com0[i]=c;
		else if(comPort==1) rxdata_com1[i]=c;
	}

	if(i>0){
		req->len=charPending; req->num=0;
		if(comPort==0) UART_WriteAsync(MXC_UART0, &write_req0);
		if(comPort==1) UART_WriteAsync(MXC_UART1, &write_req1);
	}
*/
}


void read_cb0(uart_req_t* req, int error){read_cb(req,error,0);}
void read_cb1(uart_req_t* req, int error){read_cb(req,error,1);}

void write_cb0(uart_req_t* req, int error){write_cb(req,error,0);}
void write_cb1(uart_req_t* req, int error){write_cb(req,error,1);}



//void Max326_USART_Driver::InitBuffers(int comPort){
void InitBuffers(int comPort){
	int error;
	if(comPort==0){
		 read_req0.data = rxdata_com0;
		 read_req0.len = BUFF_SIZE;
		 read_req0.callback = read_cb0;

		 write_req0.data = txdata_com0;
		 write_req0.len = BUFF_SIZE;
		 write_req0.callback = write_cb0;

		error = UART_ReadAsync(MXC_UART0, &read_req0);
		if(error != E_NO_ERROR) {
			debug_printf("Error starting async read %d\n", error);
		}

		error = UART_WriteAsync(MXC_UART0, &write_req0);
		if(error != E_NO_ERROR) {
			debug_printf("Error starting async write %d\n", error);
		}
	}
	if(comPort==1){
		 read_req1.data = rxdata_com1;
		 //read_req1.len = BUFF_SIZE;
		 read_req1.callback = read_cb1;

		 write_req1.data = txdata_com1;
		 //write_req1.len = BUFF_SIZE;
		 write_req1.callback = write_cb1;

		/*error = UART_ReadAsync(MXC_UART1, &read_req1);
		if(error != E_NO_ERROR) {
			debug_printf("Error starting async read %d\n", error);
		}

		error = UART_WriteAsync(MXC_UART1, &write_req1);
		if(error != E_NO_ERROR) {
			debug_printf("Error starting async write %d\n", error);
		}*/
	}
}

#ifdef __cplusplus
}
#endif

void USART0_Handler(void *args) {
	/*GLOBAL_LOCK(irq);
	SystemState_SetNoLock( SYSTEM_STATE_ISR              );
	SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
*/
	UART_Handler(MXC_UART0);
	/*irq.Acquire();
	SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
	SystemState_ClearNoLock( SYSTEM_STATE_ISR              );
	//CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
	return;*/
}

void USART1_Handler(void *args)
{
	UART_Handler(MXC_UART1);
}


	//////////////////////Non extern C functions ///////////////


// Note that once this goes up, it stays up.
static BOOL init_com0(int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue) {
	PORTS_IN_USE_MASK |= 1;

	UINT32 interruptIndex = UART0_IRQn;
	HAL_CALLBACK_FPN callback = NULL;

	// Initialize the UART
	uart_cfg_t cfg;
	cfg.parity = UART_PARITY_DISABLE; // no parity
	cfg.size = UART_DATA_SIZE_8_BITS; //8 data bits
	cfg.extra_stop = 1; //no stop bits
	cfg.cts = 0;  //No hardware flow control
	cfg.rts = 0;
	cfg.baud = 57600; // Default baud for MFDeploy and Visual Studio

	sys_cfg_uart_t sys_cfg;
	//sys_cfg.clk_scale = CLKMAN_SCALE_DIV_4; //CLKMAN_SCALE_AUTO;
	sys_cfg.clk_scale = CLKMAN_SCALE_AUTO;
	IOMAN_UART_FUNC(sys_cfg.io_cfg, 0, IOMAN_MAP_A, IOMAN_MAP_A, IOMAN_MAP_A, 1, 1, 1);

	while(UART_Busy(MXC_UART0)){}
	while(UART_Busy(MXC_UART1)){}

	int error = UART_Init(MXC_UART0, &cfg, &sys_cfg);
	if(error != E_NO_ERROR) {
		return FALSE;
	} else {
		InitBuffers(0);
	}

	interruptIndex = UART0_IRQn;
	callback = USART0_Handler;
	if(!CPU_INTC_ActivateInterrupt(interruptIndex, callback, NULL) ) return FALSE;

/*mxc_uart_fifo_regs_t *fifo;
fifo = MXC_UART0_FIFO;
fifo->tx = 'a';
fifo->tx = 'b';
fifo->tx = 'c';
CPU_GPIO_TogglePinState(8);
CPU_GPIO_TogglePinState(8);
CPU_USART_WriteCharToTxBuffer(0, 'd');
CPU_USART_WriteCharToTxBuffer(0, 'e');
CPU_USART_WriteCharToTxBuffer(0, 'f');
debug_printf("T");
debug_printf("Works?");*/

	return TRUE;
}

static BOOL init_com1(int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue) {
	PORTS_IN_USE_MASK |= 2;

	UINT32 interruptIndex = UART1_IRQn;
	HAL_CALLBACK_FPN callback = NULL;


	// Initialize the UART
	uart_cfg_t cfg;
	cfg.parity = UART_PARITY_DISABLE; // no parity
	cfg.size = UART_DATA_SIZE_8_BITS; //8 data bits
	cfg.extra_stop = 1; //no stop bits
	cfg.cts = 0;  //No hardware flow control
	cfg.rts = 0;
	cfg.baud = 57600; // Default baud for MFDeploy and Visual Studio

	sys_cfg_uart_t sys_cfg;
	sys_cfg.clk_scale = CLKMAN_SCALE_AUTO;
	IOMAN_UART_FUNC(sys_cfg.io_cfg,1, IOMAN_MAP_A,  IOMAN_MAP_A, IOMAN_MAP_A, 1, 1, 1);

	while(UART_Busy(MXC_UART0)){}
	while(UART_Busy(MXC_UART1)){}

	int error = UART_Init(MXC_UART1, &cfg, &sys_cfg);
	if(error != E_NO_ERROR) {
		return FALSE;
	} else {
		InitBuffers(1);
	}

	interruptIndex = UART1_IRQn;
	callback = USART1_Handler;
	if(!CPU_INTC_ActivateInterrupt(interruptIndex, callback, NULL) ) return FALSE;

/*mxc_uart_fifo_regs_t *fifo;
fifo = MXC_UART1_FIFO;
fifo->tx = 'a';
fifo->tx = 'b';
fifo->tx = 'c';
CPU_GPIO_TogglePinState(8);
CPU_GPIO_TogglePinState(8);*/


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
		UART_Shutdown(MXC_UART_GET_UART(0));
		UART_Enable(MXC_UART_GET_UART(0));
	}

	if (PORTS_IN_USE_MASK & 0x2) {
		UART_Shutdown(MXC_UART_GET_UART(1));
		UART_Enable(MXC_UART_GET_UART(1));
	}
}




BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
	// Check to make sure not already up.
	//if (ComPortNum == 0 && (PORTS_IN_USE_MASK&1)) return TRUE;
	//if (ComPortNum == 1 && (PORTS_IN_USE_MASK&2)) return TRUE;

	// If COM0, don't initialize until we see something connected (i.e. usb-serial attached)
	//if (ComPortNum == 0) {
		return init_com0(BaudRate, Parity, DataBits, StopBits, FlowValue);
	/*}

	if (ComPortNum == 1) {
		return init_com1(BaudRate, Parity, DataBits, StopBits, FlowValue);
	}

	return FALSE;*/
}

// Not sure of a scenario where this fails
BOOL CPU_USART_Uninitialize( int ComPortNum )
{
	switch(ComPortNum)
	{
	case 0:
		PORTS_IN_USE_MASK &= ~(0x1);
		break;
	case 1:
	default:
		PORTS_IN_USE_MASK &= ~(0x2);
		break;
	}
	
	UART_Shutdown(MXC_UART_GET_UART(ComPortNum));
	return TRUE;
}

// check if tx buffer is empty
BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
	return (UART_NumWriteAvail(MXC_UART_GET_UART(ComPortNum))==0) ;
}

// if transmission is complete the Shift Register is empty
BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
	return FALSE;
}


// Write char into the data register
void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
	uart_req_t *req;
	if(ComPortNum==0){ req= &write_req0;}
	if(ComPortNum==1){ req= &write_req1;}

	req->len=1;
	req->num=0;
	req->data[0]=c;
	UART_WriteAsync(MXC_UART_GET_UART(ComPortNum) , req);
}


void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
	mxc_uart_regs_t *uart = MXC_UART_GET_UART(ComPortNum);
	// Enable almost empty interrupt
	uart->inten |= (MXC_F_UART_INTEN_TX_FIFO_AE);
}

// Returns TRUE if interrupt is enabled, NOT their current state.
BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{
	//mxc_uart_regs_t *uart = MXC_UART_GET_UART(ComPortNum);
	// Enable almost empty interrupt
	//return uart->inten & MXC_F_UART_INTEN_TX_FIFO_AE;
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


