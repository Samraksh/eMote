////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <stm32f10x.h>
#include <netmf_usart.h>
#include <gpio\stm32f10x_gpio.h>
#include <rcc\stm32f10x_rcc.h>
#include <intc\STM32.h>

#define COM1 0
#define COM2 1

uint8_t RX_BAD=2;

extern "C"
{
void USART1_Handler(void *args);
void USART2_Handler(void *args);
}


/*TODO 
	Add error handling
	Define the APIs completely
	#define the comPort
	Check how physical ports are mapped to logical ports
	Send data is non-blocking, skips data if continously transmitted
*/

BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{

	// Fix added to protect long range radio against usart power - ask Nathan.Stohs for reasons
	CPU_GPIO_EnableInputPin3((GPIO_PIN) 9, FALSE, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);

	if(CPU_GPIO_GetPinState((GPIO_PIN) 9) == TRUE)
	{
		// Lock the external radio seln pin
		if(!CPU_GPIO_ReservePin(89, TRUE))
			return FALSE;

		// Lock the external radio slptr pin
		if(!CPU_GPIO_ReservePin(27, TRUE))
			return FALSE;

		// Lock the external radio rstn pin
		if(!CPU_GPIO_ReservePin(90, TRUE))
			return FALSE;
	}


	// Reserve the two ports the TX/RX ports of usart 1/ COM1
	if(!CPU_GPIO_ReservePin(9, TRUE))
		return FALSE;

	if(!CPU_GPIO_ReservePin(10, TRUE))
		return FALSE;

  UINT32 interruptIndex = 0;
  HAL_CALLBACK_FPN callback = NULL;

  if(ComPortNum == 0) //CLR has it as 0
  {	
    ComPortNum = COM1;
    interruptIndex = STM32_AITC::c_IRQ_INDEX_USART1;
    callback = USART1_Handler;
  }
  else
  {
    ComPortNum = COM2;
    interruptIndex = STM32_AITC::c_IRQ_INDEX_USART2;
    callback = USART2_Handler;
  }

  // If unable to active the interrupt for the usart return false
  if(!CPU_INTC_ActivateInterrupt(interruptIndex, callback, NULL) ) return FALSE;



  // Init UART1

  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_ClockInitTypeDef USART_ClockInitStruct;


  USART_DeInit(USART1);
  USART_StructInit(&USART_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);

  USART_ClockStructInit(&USART_ClockInitStruct);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  // Configure USART Tx as alternate function push-pull
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure USART Rx as input floating
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_Init(USART1, &USART_InitStructure);
  USART_ClockInit(USART1, &USART_ClockInitStruct);
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  USART_Cmd(USART1, ENABLE);

  return TRUE;
}

// Not sure of a scenario where this fails
BOOL CPU_USART_Uninitialize( int ComPortNum )
{
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
		activeUsart = USART2;
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
	// Unsupported ComPortNum
	if(ComPortNum != 0 && ComPortNum != 1)
		return;
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
		activeUsart = USART2;
		break;
	}

	//Transmit data to USART specified
	USART_SendData(activeUsart, c);
	USART_ITConfig(activeUsart, USART_IT_TXE,  ENABLE); // Why does this want to fire immediately?
}


void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
	return; // Turned on dynamically.

	if(ComPortNum != 0 && ComPortNum != 1)
		return;

	// There is no way we have more than two usarts in the hardware, but need to check which usart is connected to
	// the connector
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
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
		activeUsart = USART2;
		break;
	}

	return (activeUsart->CR1 & 0x80);
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
	return; // always on
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
		activeUsart = USART2;
		break;
	}
	// the connector

	USART_ITConfig(activeUsart, USART_IT_RXNE, (FunctionalState) Enable);
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
	return TRUE; // always on
	USART_TypeDef* activeUsart;
	switch(ComPortNum)
	{
	case 0:
		activeUsart = USART1;
		break;
	case 1:
		activeUsart = USART2;
		break;
	}

	if (USART_GetITStatus(activeUsart, USART_IT_RXNE) == SET)
		return TRUE;
	else
		return FALSE;
}

BOOL CPU_USART_TxHandshakeEnabledState( int comPort )
{
	/*
	Check for the value of CTS Pin and return the result. 
	Need to determine if HandShakeMode is enabled or not
	*/

    return TRUE; 
}

void CPU_USART_ProtectPins( int ComPortNum, BOOL On )
{
    /*
	Maybe we can skip this as STM_EVAL_COMInit sets the 
	appropriate GPIO pins.
	*/
}

UINT32 CPU_USART_PortsCount()
{
    return TOTAL_USART_PORT;
}

void CPU_USART_GetPins( int ComPortNum, GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{   
    //Return appropriate pins.
}

BOOL CPU_USART_SupportNonStandardBaudRate (int ComPortNum )
{
    return TRUE;
}

void CPU_USART_GetBaudrateBoundary( int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz )
{
    maxBaudrateHz = MAX_BAUDRATE;
	minBaudrateHz = MIN_BAUDRATE;
}

BOOL CPU_USART_IsBaudrateSupported( int ComPortNum, UINT32& BaudrateHz )
{
  if (BaudrateHz <= MAX_BAUDRATE)
  {
    return true;
  }
  else
  {
    BaudrateHz = MAX_BAUDRATE;
    return false;
  }
}

extern "C"
{


void USART1_Handler(void *args) {
	int err;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		char c;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		c = USART_ReceiveData(USART1)&0xFF;
		
		// Fix for strangeness at 8 MHz... disabled for now while we run at 48 MHz.
		//if ( (RX_BAD-- > 0) && c != 'M' && c != 'S') { return; }
		
		USART_AddCharToRxBuffer(COM1, c);
		return;
	}

	if (USART_GetITStatus(USART1, USART_IT_TXE)  == SET) {
		char c;
		// USART_IT_TXE pending bit only cleared by write
		if ( USART_RemoveCharFromTxBuffer(COM1, c) ) {
			USART_SendData(USART1, c);
		}
		else {
			USART_ITConfig(USART1, USART_IT_TXE,  DISABLE);
		}
		return;
	}

	// How did we get here? Overruns, that's how.
	// Seems to happen when a timer interrupt hits.
	// Clear by read to SR and then DR
	err = USART1->SR;
	err = USART1->DR;
}



/*
void USART1_Handler(void *args)
{
	volatile unsigned int ir = 0;
	char c;
	ir = USART1->SR;

	if(ir & USART_FLAG_RXNE)
	{
		USART1->SR &= ~USART_FLAG_RXNE;
		c = (char) USART_ReceiveData(USART1);
		USART_AddCharToRxBuffer(COM1, c);
		//if(!USART_AddCharToRxBuffer(COM1, (char) (USART1->DR & 0x1FF)))
		{

		}
		Events_Set(SYSTEM_EVENT_FLAG_COM_IN);

	}

	if(ir & USART_FLAG_TXE)
	//if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		GLOBAL_LOCK(irq);

		if(USART_RemoveCharFromTxBuffer(COM1, c))
	    {
			//USART_ITConfig(USART1, USART_IT_TXE, (FunctionalState) FALSE);
			//do
			//{
			//if(USART_BytesInBuffer(COM1, false) == 1)
			//{
			//	USART_ITConfig(USART1, USART_IT_TXE, (FunctionalState) FALSE);
			//}

			 USART_SendData(USART1, c);
			 while(!(USART_GetFlagStatus(USART1, USART_SR_TXE) == SET));
			//}while(USART_RemoveCharFromTxBuffer(COM1, c));

			 Events_Set(SYSTEM_EVENT_FLAG_COM_OUT);
		}
		else
		{

			USART_ITConfig(USART1, USART_IT_TXE, (FunctionalState) FALSE);
			//USART_SendData(USART1, 0);
			//while(!(USART_GetFlagStatus(USART1, USART_SR_TXE) == SET));

		}
	}

}
*/


void USART2_Handler(void *args)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		  char c = (char) USART_ReceiveData(USART2); // read RX data
		  USART_AddCharToRxBuffer(COM2, c);
		 // Events_Set(SYSTEM_EVENT_FLAG_COM_IN);
	}
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{
		 char c;
		    if (USART_RemoveCharFromTxBuffer(COM2, c)) {
		    	USART_SendData(USART2, c);
		    } else {
		    	USART_ITConfig(USART2, USART_IT_TXE, (FunctionalState) FALSE);
		    }
		    Events_Set(SYSTEM_EVENT_FLAG_COM_OUT);
	}
}

}
