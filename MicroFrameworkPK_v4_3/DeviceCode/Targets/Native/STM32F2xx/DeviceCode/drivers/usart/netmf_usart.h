//#ifndef _STM32__USART_H_1
//#define _STM32__USART_H_1   1
#ifndef _NETMF_USART_H_
#define _NETMF_USART_H_
#include "STM32F2xx_usart.h"
#include "..\intc\STM32.h"

//TODO Calculate this based on the System Clock value.
#define MAX_BAUDRATE 115200
#define MIN_BAUDRATE 0

uint16_t USART_IT;

#define COMn                             2

#define EVAL_COM1                        USART3
#define EVAL_COM1_CLK                    RCC_APB1Periph_USART3
#define EVAL_COM1_TX_PIN                 GPIO_Pin_10
#define EVAL_COM1_TX_GPIO_PORT           GPIOC
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_TX_AF                  GPIO_AF_USART3
#define EVAL_COM1_RX_PIN                 GPIO_Pin_11
#define EVAL_COM1_RX_GPIO_PORT           GPIOC
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource11
#define EVAL_COM1_RX_AF                  GPIO_AF_USART3
#define EVAL_COM1_IRQn                   USART3_IRQn


/**
 * @brief Definition for COM port2, connected to USART2 (USART2 pins remapped on GPIOD)
 */
#define EVAL_COM2                        USART2
#define EVAL_COM2_CLK                    RCC_APB1Periph_USART2
#define EVAL_COM2_TX_PIN                 GPIO_Pin_2
#define EVAL_COM2_TX_GPIO_PORT           GPIOD
#define EVAL_COM2_TX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM2_TX_SOURCE              GPIO_PinSource2
#define EVAL_COM2_TX_AF                  GPIO_AF_USART2
#define EVAL_COM2_RX_PIN                 GPIO_Pin_3
#define EVAL_COM2_RX_GPIO_PORT           GPIOD
#define EVAL_COM2_RX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM2_RX_SOURCE              GPIO_PinSource3
#define EVAL_COM2_RX_AF                  GPIO_AF_USART2
#define EVAL_COM2_IRQn                   USART2_IRQn


struct STM32_USART_Driver
{	
 BOOL Initialize( int comPort, int baudRate, int Parity, int DataBits, int StopBits, int flowValue );
 BOOL Uninitialize( int comPort );
 BOOL TxBufferEmpty( int comPort );
 BOOL TxShiftRegisterEmpty( int comPort );
 void WriteCharToTxBuffer( int comPort,  UINT8 c );
 void TxBufferEmptyInterruptEnable( int comPort, BOOL enable );
 BOOL TxBufferEmptyInterruptState( int comPort );
 void RxBufferFullInterruptEnable( int comPort, BOOL enable );
 BOOL RxBufferFullInterruptState( int comPort );
// UINT8 CPU_USART_getRxBuffer(int ComPortNum);
 BOOL TxHandshakeEnabledState( int comPort );
 void ProtectPins( int comPort, BOOL on );
 void USART_ISR( void* Param );
 void GetPins( int comPort,  GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin );    
 void BaudrateBoundary( int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz );
 BOOL IsBaudrateSupported( int ComPortNum, UINT32& BaudrateHz );

};

#endif
	
//#endif //_STM32__USART_H_1
