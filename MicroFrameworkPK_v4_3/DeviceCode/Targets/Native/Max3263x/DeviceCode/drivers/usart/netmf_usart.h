#ifndef _NETMF_USART_H_
#define _NETMF_USART_H_

#include <gpio\netmf_gpio.h>
#include <netmf_usart.h>
#include <Include/uart.h>

//TODO Calculate this based on the System Clock value.
#define MAX_BAUDRATE 115200
#define MIN_BAUDRATE 0

uint16_t USART_IT;

#define COMn                             2
/*
#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCC_APB2Periph_USART1
#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_APB2Periph_GPIOA
#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_APB2Periph_GPIOA
#define EVAL_COM1_IRQn                   USART1_IRQn


//@brief Definition for COM port2, connected to USART2 (USART2 pins remapped on GPIOD)
#define EVAL_COM2                        USART2
#define EVAL_COM2_CLK                    RCC_APB1Periph_USART2
#define EVAL_COM2_TX_PIN                 GPIO_Pin_2
#define EVAL_COM2_TX_GPIO_PORT           GPIOD
#define EVAL_COM2_TX_GPIO_CLK            RCC_APB2Periph_GPIOD
#define EVAL_COM2_RX_PIN                 GPIO_Pin_3
#define EVAL_COM2_RX_GPIO_PORT           GPIOD
#define EVAL_COM2_RX_GPIO_CLK            RCC_APB2Periph_GPIOD
#define EVAL_COM2_IRQn                   USART2_IRQn
*/

#define BUFF_SIZE 32

char txdata_com0[BUFF_SIZE];
char rxdata_com0[BUFF_SIZE];
char txdata_com1[BUFF_SIZE];
char rxdata_com1[BUFF_SIZE];
uart_req_t read_req0,read_req1;
uart_req_t write_req0,write_req1;
void InitBuffers(int comPort);
/*
struct Max326_USART_Driver
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
*/

#endif
	
//#endif //_STM32__USART_H_1
