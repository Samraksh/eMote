#include "STM32F10x_usart"
#include "stm32_usart.h"
#include "stm32_eval.h"


/* Depends on 
	- stm32_eval.c
	- stm32f103x_usart.c
*/

STM32_USART_Driver g_STM32_USART_Driver;


BOOL Initialize( int comPort, int baudRate, int Parity, int DataBits, int StopBits, int flowValue )
{
  /*TODO 
	Add error handling.
	Define the APIs completely
  */
  BOOL fRet = TRUE;
  //Initializing the USART structure for calling the Init on STM Driver
  USART_InitTypeDef USART_InitStructure; 
  
  
  if(comPort == 1)
  {
    comPort = COM1;
  }
  
  else
  {
    comPort = COM2;
  }
  
  //TODO Switch among allowed values of baudrate and select or else error
  USART_InitStructure.USART_BaudRate = baudRate;
  //TODO Switch among allowed values of databits and select or else error
  USART_InitStructure.USART_WordLength = DataBits;
  //TODO Switch among allowed values of stopbits and select or else error
  USART_InitStructure.USART_StopBits = StopBits;
  //TODO Switch among allowed values of parity and select or else error
  USART_InitStructure.USART_Parity = Parity;
  //TODO Switch among allowed values of flow value and select or else error
  USART_InitStructure.USART_HardwareFlowControl = flowValue;
  //TODO Define Mode selection
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  
  STM_EVAL_COMInit(comPort, &USART_InitStructure);    
  
  return fRet;
  
}

BOOL Uninitialize( int comPort )
{
  if(comPort == 1)
  {
    USART_DeInit(USART1);
  }
  
  else if(comPort == 2)
  {
    USART_DeInit(USART2);
  }
  
  else if(comPort == 3)
  {
    USART_DeInit(USART3);
  }
  
  else if(comPort == 4)
  {
    USART_DeInit(UART4);
  }
  
  else
  {
    if(comPort == 5)
	{
	  USART_DeInit(UART5);
	}
  }
}

BOOL TxBufferEmpty( int comPort )
{

}

BOOL TxShiftRegisterEmpty( int comPort )
{

}

void WriteCharToTxBuffer( int comPort,  UINT8 c )
{

}

void TxBufferEmptyInterruptEnable( int comPort, BOOL enable )
{

}

BOOL TxBufferEmptyInterruptState( int comPort )
{

}

void RxBufferFullInterruptEnable( int comPort, BOOL enable )
{

}

BOOL RxBufferFullInterruptState( int comPort )
{

}

BOOL TxHandshakeEnabledState( int comPort )
{

}

void ProtectPins( int comPort, BOOL on )
{

}

void USART_ISR( void* Param )
{

}

void GetPins( int comPort,  GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{

}

void BaudrateBoundary( int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz )
{
  maxBaudrateHz = MAX_BAUDRATE;
  minBaudrateHz = MIN_BAUDRATE;

}

BOOL IsBaudrateSupported( int ComPortNum, UINT32 & BaudrateHz )
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