////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <cmsis/m2sxxx.h>
#include <netmf_usart.h>
//#include "sam_usart.h"

//ComHandle != ComPort.  COM1 is a handle with port=0. COM1=0x101 means port 0 on USART transport.  See platform_selector.h and tinyhal.h.

void USART2_Handler(void *args);

static USART_InitTypeDef USART1_InitStructure;
static USART_InitTypeDef USART2_InitStructure;
static int PORTS_IN_USE_MASK = 0;

/*TODO 
	Add error handling
	Define the APIs completely
	#define the comPort
	Check how physical ports are mapped to logical ports
	Send data is non-blocking, skips data if continuously transmitted
*/

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
		
		// Do the reset
		USART_DeInit(USART1);
		USART_Init(USART1, &USART1_InitStructure);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE); // Will cause bytes to start moving again.
		USART_Cmd(USART1, ENABLE);
	}

	if (PORTS_IN_USE_MASK & 0x2) {
		USART_DeInit(USART2);
		USART_Init(USART2, &USART2_InitStructure);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE); // Will cause bytes to start moving again.
		USART_Cmd(USART2, ENABLE);
	}
}

// Note that once this goes up, it stays up.
static void init_com0(void) {
	if(!CPU_GPIO_ReservePin(9, TRUE))  { return; }
	if(!CPU_GPIO_ReservePin(10, TRUE)) { return; }

	PORTS_IN_USE_MASK |= 1;

	NVIC_InitTypeDef NVIC_InitStructure;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
	USART_DeInit(USART1);
	USART_StructInit(&USART1_InitStructure);

	USART1_InitStructure.USART_BaudRate 			= 115200; // Default baud for MFDeploy and Visual Studio
	USART1_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART1_InitStructure.USART_StopBits 			= USART_StopBits_1;
	USART1_InitStructure.USART_Parity 				= USART_Parity_No;
	USART1_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART1_InitStructure.USART_Mode				 	= USART_Mode_Rx | USART_Mode_Tx;

	// Configure USART Tx as alternate function push-pull
	GPIO_ConfigurePin(GPIOA, GPIO_Pin_9, GPIO_Mode_AF_PP, GPIO_Speed_10MHz);

	// Configure USART Rx as input floating // Actually make it a pull-up for noise immunity. See #250.
	GPIO_ConfigurePin(GPIOA, GPIO_Pin_10, GPIO_Mode_IPU, GPIO_Speed_10MHz);

	USART_Init(USART1, &USART1_InitStructure);
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

static BOOL init_com1(int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue) {
	if(!CPU_GPIO_ReservePin(2, TRUE)) { return FALSE; }
	if(!CPU_GPIO_ReservePin(3, TRUE)) { return FALSE; }

	PORTS_IN_USE_MASK |= 2;

	UINT32 interruptIndex = 0;
	HAL_CALLBACK_FPN callback = NULL;

	interruptIndex = STM32_AITC::c_IRQ_INDEX_USART2;
	callback = USART2_Handler;
	if(!CPU_INTC_ActivateInterrupt(interruptIndex, callback, NULL) ) return FALSE;

	UINT32 my_baudrate = BaudRate;
	CPU_USART_IsBaudrateSupported( 1, my_baudrate );

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); // Assume this is already on from COM1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_DeInit(USART2);
	USART_StructInit(&USART2_InitStructure);

	USART2_InitStructure.USART_BaudRate 			= my_baudrate;
	USART2_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART2_InitStructure.USART_StopBits 			= USART_StopBits_1;
	USART2_InitStructure.USART_Parity 				= USART_Parity_No;
	USART2_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART2_InitStructure.USART_Mode 				= USART_Mode_Rx | USART_Mode_Tx;

	// Configure USART Tx as alternate function push-pull
	GPIO_ConfigurePin(GPIOA, GPIO_Pin_2, GPIO_Mode_AF_PP, GPIO_Speed_10MHz);

	// Configure USART Rx as input floating // Actually make it a pull-up for noise immunity. See #250.
	GPIO_ConfigurePin(GPIOA, GPIO_Pin_3, GPIO_Mode_IPU, GPIO_Speed_10MHz);

	USART_Init(USART2, &USART2_InitStructure);
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	return TRUE;
}

static void do_com0_attached(GPIO_PIN Pin, BOOL PinState, void* Param) {
	CPU_GPIO_DisablePin( (GPIO_PIN) 10, (GPIO_RESISTOR)0, 0, (GPIO_ALT_MODE)0 ); // Only first arg used, rest dummy.
	init_com0();
}

BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
	// Check to make sure not already up.
	if (ComPortNum == 0 && (PORTS_IN_USE_MASK&1)) return TRUE;
	if (ComPortNum == 1 && (PORTS_IN_USE_MASK&2)) return TRUE;

	// If COM0, don't initialize until we see something connected (i.e. usb-serial attached)
	if (ComPortNum == 0) {

		if(1) {
		//if (CPU_GPIO_GetPinState((GPIO_PIN)10) == TRUE) {
			do_com0_attached(0, FALSE, NULL);
			return TRUE;
		}

		CPU_GPIO_EnableInputPin( (GPIO_PIN) 10, FALSE, do_com0_attached, GPIO_INT_EDGE_HIGH, RESISTOR_PULLDOWN );
		return TRUE;
	}

	if (ComPortNum == 1) {
		return init_com1(BaudRate, Parity, DataBits, StopBits, FlowValue);
	}

	return FALSE;
}

// Not sure of a scenario where this fails
BOOL CPU_USART_Uninitialize( int ComPortNum )
{
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		CPU_GPIO_ReservePin(9, FALSE);
		CPU_GPIO_ReservePin(10, FALSE);
		PORTS_IN_USE_MASK &= ~(0x1);
		break;
	case 1:
	default:
		activeUsart = USART2;
		CPU_GPIO_ReservePin(2, FALSE);
		CPU_GPIO_ReservePin(3, FALSE);
		PORTS_IN_USE_MASK &= ~(0x2);
		break;
	}
	
	USART_DeInit(activeUsart);

	return TRUE;
}

// check if tx buffer is empty
BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
	default:
		activeUsart = USART2;
		break;
	}

	if (USART_GetFlagStatus(activeUsart, USART_FLAG_TXE) == SET)
		return TRUE;
	else
		return FALSE;
}

// if transmission is complete the Shift Register is empty
BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
	default:
		activeUsart = USART2;
		break;
	}

	if (USART_GetFlagStatus(activeUsart, USART_FLAG_TC) == SET)
		return TRUE;
	else
		return FALSE;
}

// Write char into the data register
void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
	default:
		activeUsart = USART2;
		break;
	}

	USART_SendData(activeUsart, c);
}


void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
	default:
		activeUsart = USART2;
		break;
	}

	USART_ITConfig(activeUsart, USART_IT_TXE, (FunctionalState) Enable);
}

// Returns TRUE if interrupt is enabled, NOT their current state.
BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
	default:
		activeUsart = USART2;
		break;
	}

	return (activeUsart->CR1 & 0x80);
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
	default:
		activeUsart = USART2;
		break;
	}

	USART_ITConfig(activeUsart, USART_IT_RXNE, (FunctionalState) Enable);
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
	default:
		activeUsart = USART2;
		break;
	}

	return (activeUsart->CR1 & 0x20);

}

BOOL CPU_USART_TxHandshakeEnabledState( int comPort )
{
	/*
	Check for the value of CTS Pin and return the result. 
	Need to determine if HandShakeMode is enabled or not
	*/

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
    //TODO Return appropriate pins.
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

#define RX_HAL_BUF_SIZE 8  // Input buffer will flush after this size or IDLE interrupt

extern "C" {
void __irq USART1_IRQHandler() {
	static char buf[RX_HAL_BUF_SIZE];
	static int idx;
	unsigned int status;
	volatile unsigned int dummy __attribute__ ((unused));

	//CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	GLOBAL_LOCK(irq);
	SystemState_SetNoLock( SYSTEM_STATE_ISR              );
	SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

	
	status = USART1->SR; // check status reg
	
	if (status & (USART_FLAG_FE|USART_FLAG_PE|USART_FLAG_NE)) {
		// clearing interrupt flag
		dummy = USART1->DR;
	} else if (status & USART_FLAG_RXNE){
		// we got data
		char c = USART_ReceiveData(USART1)&0xFF; // Also clears status

		if (idx < RX_HAL_BUF_SIZE) {
			// temporarily storing data into batches of RX_HAL_BUF_SIZE
			buf[idx++] = c;
		}
		if (idx >= RX_HAL_BUF_SIZE) {
			// we have enough data so we do the time intesive task of putting it into software receive buffer
			USART_AddToRxBuffer( ConvertCOM_ComPort(COM1), buf, idx <= RX_HAL_BUF_SIZE ? idx : RX_HAL_BUF_SIZE);
			idx = 0;
		}
		if (status & USART_FLAG_ORE){
			c = USART_ReceiveData(USART1)&0xFF; // Also clears status

			if (idx < RX_HAL_BUF_SIZE) {
				buf[idx++] = c;
			}
			if (idx >= RX_HAL_BUF_SIZE) {
				USART_AddToRxBuffer( ConvertCOM_ComPort(COM1), buf, idx <= RX_HAL_BUF_SIZE ? idx : RX_HAL_BUF_SIZE);
				idx = 0;
			}
		}
	} else if (status & USART_FLAG_IDLE){
		dummy = USART1->DR;
		if (idx > 0) {
			USART_AddToRxBuffer( ConvertCOM_ComPort(COM1), buf, idx <= RX_HAL_BUF_SIZE ? idx : RX_HAL_BUF_SIZE);
			idx=0;
		}
	}
	irq.Release();

	// MF signals if there is TX work by toggling the interrupt enable
	// So we check it and don't do anything if it isn't set
	if (USART1->CR1 & 0x80) {
		if (USART_GetITStatus(USART1, USART_IT_TXE)  == SET) {
			char c;
			// USART_IT_TXE pending bit only cleared by write
			if ( USART_RemoveCharFromTxBuffer(ConvertCOM_ComPort(COM1), c) ) {
				USART_SendData(USART1, c);
			}
			else {
				USART_ITConfig(USART1, USART_IT_TXE,  DISABLE);
			}
		}
	}
	
	irq.Acquire();
	SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS );
	SystemState_ClearNoLock( SYSTEM_STATE_ISR              );
	//CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
	return;
}
} // extern C

void USART2_Handler(void *args)
{
	int err __attribute__ ((unused));
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		  char c = (char) USART_ReceiveData(USART2); // read RX data
		  USART_AddCharToRxBuffer(ConvertCOM_ComPort(COM2), c);
	}
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{
		 char c;
		    if (USART_RemoveCharFromTxBuffer(ConvertCOM_ComPort(COM2), c)) {
		    	USART_SendData(USART2, c);
		    } else {
		    	USART_ITConfig(USART2, USART_IT_TXE, (FunctionalState) FALSE);
		    }
		return;
		    ////Events_Set(SYSTEM_EVENT_FLAG_COM_OUT);
	}
	err = USART2->SR;
	err = USART2->DR;
}
