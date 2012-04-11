////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "../TIAM3517.h"

//#define AM3517_ENABLE_UART1
//#define AM3517_ENABLE_UART2
#define AM3517_ENABLE_UART3		// Recommend always enabled
//#define AM3517_ENABLE_UART4

/*
	Simple map of comPort to UART register base
*/
static int TIAM3517_getBase( int comPort ) {
	switch ( comPort ) {
	
		#ifdef AM3517_ENABLE_UART1
		case 1: return SAM_AM3517_UART1;
		#endif
		
		#ifdef AM3517_ENABLE_UART2
		case 2: return SAM_AM3517_UART2;
		#endif
		
		#ifdef AM3517_ENABLE_UART3
		case 3: return SAM_AM3517_UART3;
		#endif
		
		#ifdef AM3517_ENABLE_UART4
		case 4: return SAM_AM3517_UART4;
		#endif
		
		default: return 0;
	}
}

/*
	Software reset a UART module (3 by default). Ignores parameters.
	Defaults: UART3, 8 data bits, no parity, 115200 baud, 1 stop bit, software flow, 16x
	Returns TRUE
*/
BOOL TIAM3517_USART_Driver::Initialize( int comPort, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue ) {
	int base;
	
	base = TIAM3517_getBase(comPort);
	if (base == 0) return false;
	
	__raw_writeb(SAM_AM3517_UART_SYSC_SOFTRESET, base+SAM_AM3517_UART_SYSC);
	while ( __raw_readb(base+SAM_AM3517_UART_SYSS) == 0) { ; } // poll until reset
	
	// Disable UART, put into configuration mode
	__raw_writeb( 0x7 , base+SAM_AM3517_UART_MDR1 );
	
	// Switch to configuration mode B
	__raw_writeb( 0x00BF, base+SAM_AM3517_UART_LCR );
	
	// Write DLH, DLL. Default of 115200 baud
	__raw_writeb( 0x00, base+SAM_AM3517_UART_DLH );
	__raw_writeb( 0x1A, base+SAM_AM3517_UART_DLL );
	
	// Switch to operational mode and set protocol format to 8,N,1
	__raw_writeb( 0x03, base+SAM_AM3517_UART_LCR);
	
	// Load
	__raw_writeb( 0x0, base+SAM_AM3517_UART_MDR1 );
	
	return true;
}

/*
	TODO
	Returns TRUE
*/
BOOL TIAM3517_USART_Driver::Uninitialize( int comPort ) {
	return true;
}

/*
	Returns TRUE if the tx fifo is empty
*/
BOOL TIAM3517_USART_Driver::TxBufferEmpty( int comPort ) {
	int base;
	base = TIAM3517_getBase(comPort) + SAM_AM3517_UART_LSR;
	if (base == SAM_AM3517_UART_LSR) return false;
	
	return __raw_readb(base) & SAM_AM3517_UART_LSR_TX_FIFO_E;
}

/*
	Returns TRUE if the tx shift register is empty
	meaning the transmission is complete (as opposed to above)
*/
BOOL TIAM3517_USART_Driver::TxShiftRegisterEmpty( int comPort ) {
	int base;
	base = TIAM3517_getBase(comPort) + SAM_AM3517_UART_LSR;
	if (base == SAM_AM3517_UART_LSR) return false;
	
	return __raw_readb(base) & SAM_AM3517_UART_LSR_TX_SR_E;
}

/*
	Waits for tx fifo empty, write a character to the tx fifo
*/
void TIAM3517_USART_Driver::WriteCharToTxBuffer( int comPort, UINT8 c ) {
	int base, lsr, t, thr;
	
	base = TIAM3517_getBase(comPort);
	if (base == 0) return;
	
	lsr  = base + SAM_AM3517_UART_LSR;
	thr  = base + SAM_AM3517_UART_THR;
	
	// Spin until buffer ready
	while ( (__raw_readb(base) & SAM_AM3517_UART_LSR_TX_FIFO_E) == 0 ) { ; }
	
	__raw_writeb(c, thr);
}

/*
	TODO
*/
void TIAM3517_USART_Driver::TxBufferEmptyInterruptEnable( int comPort, BOOL enable ) {
	
}

/*
	TODO
*/
BOOL TIAM3517_USART_Driver::TxBufferEmptyInterruptState( int comPort ) {
    return FALSE;
}

/*
	TODO
*/
void TIAM3517_USART_Driver::RxBufferFullInterruptEnable( int comPort, BOOL Enable ) {

}

/*
	TODO
*/
BOOL TIAM3517_USART_Driver::RxBufferFullInterruptState( int comPort ) {
    return FALSE;
}
/*
	TODO
*/
BOOL TIAM3517_USART_Driver::TxHandshakeEnabledState(int comPort) {
    return TRUE;
}

/*
	TODO
*/
void TIAM3517_USART_Driver::USART_ISR( void* param ) {

}

/*
	Not implemented
*/
void TIAM3517_USART_Driver::ProtectPins( int comPort, BOOL on ) {

}

/*
	Not implemented
*/
void  TIAM3517_USART_Driver::GetPins( int comPort, GPIO_PIN& rxPin, GPIO_PIN& txPin, GPIO_PIN& ctsPin, GPIO_PIN& rtsPin ) {
    
}

void  TIAM3517_USART_Driver::BaudrateBoundary( int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz ) {
    maxBaudrateHz = 115200;
    minBaudrateHz = 115200;
}

BOOL TIAM3517_USART_Driver::IsBaudrateSupported( int ComPortNum, UINT32 & BaudrateHz ) {
	if (BaudrateHz > 115200) {
		BaudrateHz = 115200;
		return false;
	}
	
	if (BaudrateHz == 115200) {
		return true;
	}
	
	BaudrateHz = 0;
	return false;
}


