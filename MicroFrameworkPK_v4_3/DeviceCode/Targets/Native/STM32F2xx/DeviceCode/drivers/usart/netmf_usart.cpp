////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <stm32f2xx.h>
#include <netmf_usart.h>
#include <gpio\stm32f2xx_gpio.h>
#include <rcc\stm32f2xx_rcc.h>
#include <intc\STM32.h>

//#define COM1 0
//#define COM2 1

USART_TypeDef* COM_USART[COMn] = {EVAL_COM1, EVAL_COM2};

GPIO_TypeDef* COM_TX_PORT[COMn] = {EVAL_COM1_TX_GPIO_PORT, EVAL_COM2_TX_GPIO_PORT};

GPIO_TypeDef* COM_RX_PORT[COMn] = {EVAL_COM1_RX_GPIO_PORT, EVAL_COM2_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {EVAL_COM1_CLK, EVAL_COM2_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {EVAL_COM1_TX_GPIO_CLK, EVAL_COM2_TX_GPIO_CLK};

const uint32_t COM_RX_PORT_CLK[COMn] = {EVAL_COM1_RX_GPIO_CLK, EVAL_COM2_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {EVAL_COM1_TX_PIN, EVAL_COM2_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {EVAL_COM1_RX_PIN, EVAL_COM2_RX_PIN};

const uint8_t COM_TX_PIN_SOURCE[COMn] = {EVAL_COM1_TX_SOURCE, EVAL_COM2_TX_SOURCE};

const uint8_t COM_RX_PIN_SOURCE[COMn] = {EVAL_COM1_RX_SOURCE,EVAL_COM2_RX_SOURCE};

const uint8_t COM_TX_AF[COMn] = {EVAL_COM1_TX_AF,EVAL_COM2_TX_AF};

const uint8_t COM_RX_AF[COMn] = {EVAL_COM1_RX_AF,EVAL_COM2_RX_AF};

const uint16_t COM_RTS_PIN[COMn] = {0x0B, 0x00};
const uint16_t COM_CTS_PIN[COMn] = {0x0C, 0x01};

char debugInput[150] = {0};
char debugOutput[150] = {0};

int debugOutCounter = 0;

int debugCounter = 0;

//#define USART_DEBUG_GPIO 1

//extern BOOL startRecord;

extern "C"
{
void USART1_Handler(void *args);
void USART2_Handler(void *args);
}


void COMInit(int COM, USART_InitTypeDef* USART_InitStruct)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIO clock
	RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

    if (COM == COM1)
    {
      // Enable UART clock
      RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
    }

    // Connect PXx to USARTx_Tx
    GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

    // Connect PXx to USARTx_Rx
    GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

    // Configure USART Tx as alternate function
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    // Configure USART Rx as alternate function
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    // Configure USART RTS as alternate function push-pull
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = COM_RTS_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    // Configure USART CTS as input floating
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin = COM_CTS_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    // USART configuration
    USART_Init(COM_USART[COM], USART_InitStruct);

    // Enable USART
    USART_Cmd(COM_USART[COM], ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, (FunctionalState) TRUE);
    // USART_ITConfig(USART1, USART_IT_TXE, (FunctionalState) TRUE);
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


  BOOL fRet = TRUE;
  UINT32 interruptIndex = 0;
  HAL_CALLBACK_FPN callback = NULL;
  //Initializing the USART structure for calling the Init on STM Driver
  USART_InitTypeDef USART_InitStructure; 

  //CPU_GPIO_EnableOutputPin((GPIO_PIN) 9, FALSE);

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
  

  USART_InitStructure.USART_BaudRate = BaudRate;

  USART_InitStructure.USART_WordLength = DataBits;

  USART_InitStructure.USART_StopBits = StopBits;

  USART_InitStructure.USART_Parity = Parity;

  USART_InitStructure.USART_HardwareFlowControl = FlowValue;

  USART_InitStructure.USART_Mode =  USART_Mode_Rx |  USART_Mode_Tx;
  
  // If unable to active the interrupt for the usart return false
  if(!CPU_INTC_ActivateInterrupt(interruptIndex, callback, NULL) ) return FALSE;



  //USART_ITConfig(USART1, USART_IT_ORE, (FunctionalState) TRUE);
  //USART_ITConfig(USART1, USART_IT_TXE, (FunctionalState) TRUE);

  COMInit( ComPortNum	, &USART_InitStructure);
#ifdef USART_DEBUG_GPIO
  CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, FALSE);
  CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, FALSE);
  CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);
  CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
#endif
  return fRet;
    
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
	case 2:
		activeUsart = USART3;
				break;
	}
	if(USART_GetFlagStatus(activeUsart, USART_SR_TXE)  == SET) return TRUE;
	else return FALSE;

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
	
	if(USART_GetFlagStatus(activeUsart, USART_SR_TC) == SET)
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
	
	//Transmit data to USART specified.
	USART_SendData(activeUsart, c);
    
}


void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
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


#if 0
	GLOBAL_LOCK(irq);
	char c;

	while(USART_RemoveCharFromTxBuffer(COM1, c))
	{
		USART_SendData(USART1, c);
#if 0
		if(startRecord == TRUE)
		{
			debugOutput[debugOutCounter++] = c;
			if(debugOutCounter == 140)
				debugOutCounter = 0;
		}
#endif
		//CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
		//CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);

		while(!(USART_GetFlagStatus(activeUsart, USART_SR_TXE) == SET));
		//while(!CPU_USART_TxShiftRegisterEmpty(COM1));
	}
#endif

	//USART_ITConfig(activeUsart, USART_IT_RXNE, (FunctionalState) Enable);
	USART_ITConfig(activeUsart, USART_IT_TXE, (FunctionalState) Enable);
	//USART_ITConfig(activeUsart, USART_IT_RXNE, (FunctionalState) !Enable);


	//USART_ITConfig(USART1, USART_IT_TC, (FunctionalState) Enable);
}

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

	if(USART_GetITStatus(activeUsart, USART_IT_TXE) == SET) return TRUE;
	else return FALSE;

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
		activeUsart = USART2;
		break;
	}
	// the connector

	USART_ITConfig(activeUsart, USART_IT_RXNE, (FunctionalState) Enable);
	//USART_ITConfig(activeUsart, USART_IT_TXE, (FunctionalState) Enable);
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
		activeUsart = USART2;
		break;
	}

	if(USART_GetITStatus(activeUsart, USART_CR1_RXNEIE) == SET) return TRUE;
		else return FALSE;

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

void USART1_Handler(void *args)
{
	volatile unsigned int ir = 0;
	char c;
	ir = USART1->SR;

	if(ir & USART_FLAG_RXNE)
	{
#ifdef USART_DEBUG_GPIO
		CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
#endif
		USART1->SR &= ~USART_FLAG_RXNE;
		if(!USART_AddCharToRxBuffer(COM1, (char) (USART1->DR & 0x1FF)))
		{

#ifdef USART_DEBUG_GPIO
			CPU_GPIO_SetPinState((GPIO_PIN) 31, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN) 31, FALSE);
#endif
		}
		Events_Set(SYSTEM_EVENT_FLAG_COM_IN);

#if 0
	//if(ir & USART_FLAG_RXNE)
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		GLOBAL_LOCK(irq);
		//USART1->SR &= ~USART_FLAG_RXNE;

		USART_ITConfig(USART1, USART_IT_RXNE, (FunctionalState) FALSE);

		while(USART_GetFlagStatus(USART1, USART_SR_RXNE) != RESET)
		{
			//USART1->SR &= ~USART_FLAG_RXNE;

			CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);

		//c = (char) (USART_ReceiveData(USART1) /* & 0x7F */);
			//c = (char) (USART1->DR & 0x1FF);

			USART_AddCharToRxBuffer(COM1, (char) (USART1->DR & 0x1FF));
		}
		Events_Set(SYSTEM_EVENT_FLAG_COM_IN);

		USART1->SR &= ~USART_FLAG_RXNE;
		USART_ITConfig(USART1, USART_IT_RXNE, (FunctionalState) TRUE);


		if((!(ir & USART_FLAG_RXNE)) || debugOutCounter == 150)
		{
			int temp = 0;
			while(debugOutCounter > 0)
			{
				USART_AddCharToRxBuffer(COM1, debugOutput[temp++]);
				debugOutCounter--;
			}
		}
#endif
#if 0
		USART_ITConfig(USART1, USART_IT_RXNE, (FunctionalState) FALSE);

		do
		{
			 CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
			 CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
			USART1->SR &= ~USART_FLAG_RXNE;

			c = (char) (USART_ReceiveData(USART1) /* & 0x7F */);

			USART_AddCharToRxBuffer(COM1, c);

		}while(USART_GetFlagStatus(USART1, USART_SR_RXNE) != RESET);

		USART1->SR &= ~USART_FLAG_RXNE;

		USART_ITConfig(USART1, USART_IT_RXNE, (FunctionalState) TRUE);
		//Events_Set(SYSTEM_EVENT_FLAG_COM_IN);
#endif
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
#if 0
				CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
				CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
#endif
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

#if 0
void USART1_Handler(void *args)
{


	if(USART_GetFlagStatus(USART1, USART_SR_RXNE) != RESET)
	{
		GLOBAL_LOCK(irq);
		//USART_ITConfig(USART1, USART_IT_RXNE, (FunctionalState) FALSE);

		  //CPU_GPIO_SetPinState((GPIO_PIN) 9, TRUE);
		  //CPU_GPIO_SetPinState((GPIO_PIN) 9, FALSE);
		  //while(USART_GetITStatus(USART1, USART_SR_RXNE) != RESET)
		 while(USART_GetFlagStatus(USART1, USART_SR_RXNE) != RESET)
		 {
			  char c = (char) (USART_ReceiveData(USART1) /* & 0x7F */); // read RX data

			  CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
			  CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
		  //debugInput[debugCounter++] = c;

		  //for(int i = 0; i < 1000; i++);

		  //if(debugCounter == 150)
		  //	  debugCounter = 0;

			  USART_AddCharToRxBuffer(COM1, c);
		  }
		  Events_Set(SYSTEM_EVENT_FLAG_COM_IN);
		  //USART_ITConfig(USART1, USART_IT_RXNE, (FunctionalState) FALSE);
		  //USART_ITConfig(USART1, USART_IT_RXNE, (FunctionalState) TRUE);

	}
#if 0
	//if(USART_GetITStatus(USART1, USART_IT_TC) == SET)
	else if(USART_GetFlagStatus(USART1, USART_SR_TXE) != RESET)
	{
		//GLOBAL_LOCK(irq);
		//USART_ITConfig(USART1, USART_IT_TXE, (FunctionalState) FALSE);
		 char c;

		 //USART_ITConfig(USART1, USART_IT_TXE, (FunctionalState) FALSE);

		 while(USART_RemoveCharFromTxBuffer(COM1, c))
		 {
			 CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
			 CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);

			 USART_SendData(USART1, c);
			 while(!(USART_GetFlagStatus(USART1, USART_SR_TXE) == SET));
			 //while(!CPU_USART_TxShiftRegisterEmpty(COM1));
			 Events_Set(SYSTEM_EVENT_FLAG_COM_OUT);
		 }
		 //else
		 //{
			 //USART_ITConfig(USART1, USART_IT_TXE, (FunctionalState) FALSE);
			 //USART_ITConfig(USART1, USART_IT_RXNE, (FunctionalState) TRUE);
		 //}


#if 0
		    if (USART_RemoveCharFromTxBuffer(COM1, c))
		    {
		    		debugOutput[debugOutCounter++] = c;

		    		if(debugOutCounter == 150)
		    			debugOutCounter = 0;


		    		USART_SendData(USART1, c);
		    		// Some pal layer functionality is not doing its job making this necessary to slow down transmission
		    		// The question is why is the transmission getting corrupted when the shift register is not checked, technically
		    		// you dont need to do this
		    		while(!CPU_USART_TxShiftRegisterEmpty(COM1));
		    }
		    else
		    {
		    	//while(!CPU_USART_TxShiftRegisterEmpty(COM1));
		    	//Turn off TXE interrupt when you are done sending and turn  on RXNE interrupt
		    	// you may get something
		    	USART_ITConfig(USART1, USART_IT_TXE, (FunctionalState) FALSE);
		    	USART_ITConfig(USART1, USART_IT_RXNE, (FunctionalState) TRUE);
		    }
#endif
		    Events_Set(SYSTEM_EVENT_FLAG_COM_OUT);

	}
#endif
	else if(USART_GetITStatus(USART1, USART_IT_ORE) != RESET)
	{
			//CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
			//CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
	}

}
#endif

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
