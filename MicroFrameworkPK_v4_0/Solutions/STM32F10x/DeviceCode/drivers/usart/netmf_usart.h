//#ifndef _STM32__USART_H_1
//#define _STM32__USART_H_1   1

//TODO Calculate this based on the System Clock value.
#define MAX_BAUDRATE 115200
#define MIN_BAUDRATE 0

uint16_t USART_IT;

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
	
//#endif //_STM32__USART_H_1