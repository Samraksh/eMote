
#include <tinyhal.h>
#include "netmf_usart.h"

STM32_USART_Driver g_stm32_usart_driver;

// Initial the USART Com port
BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
	 return g_stm32_usart_driver.Initialize( ComPortNum, BaudRate, Parity, DataBits, StopBits, FlowValue);
}

BOOL CPU_USART_Uninitialize( int ComPortNum )
{
	return g_stm32_usart_driver.Uninitialize(ComPortNum);
}


BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
	return g_stm32_usart_driver.TxBufferEmpty(ComPortNum);
}


BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
	return g_stm32_usart_driver.TxShiftRegisterEmpty(ComPortNum);
}

// Write char into the data register
void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
	g_stm32_usart_driver.WriteCharToTxBuffer(ComPortNum, c);
}

void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
	g_stm32_usart_driver.TxBufferEmptyInterruptEnable(ComPortNum, Enable);
}

BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{
	return g_stm32_usart_driver.TxBufferEmptyInterruptState(ComPortNum);
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
	g_stm32_usart_driver.RxBufferFullInterruptEnable(ComPortNum, Enable);
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
	g_stm32_usart_driver.RxBufferFullInterruptState(ComPortNum);
}

BOOL CPU_USART_TxHandshakeEnabledState( int comPort )
{
    return g_stm32_usart_driver.TxHandshakeEnabledState(comPort);
}

void CPU_USART_ProtectPins( int ComPortNum, BOOL On )
{
    return g_stm32_usart_driver.ProtectPins(ComPortNum, On);
}

// The EmoteDotNow has only one usart port
UINT32 CPU_USART_PortsCount()
{
    return 1;
}

void CPU_USART_GetPins( int ComPortNum, GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{

}

BOOL CPU_USART_SupportNonStandardBaudRate (int ComPortNum )
{

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

BOOL STM32_USART_Driver::Initialize(int comPort, int baudRate, int Parity, int DataBits, int StopBits, int flowValue)
{

	if(comPort > STM32_USART::USART_PORT_2)
		return FALSE;

	STM32_USART &USART = STM32::USART(comPort);

	UINT32 regValue = USART.CR2;


}

BOOL STM32_USART_Driver::Uninitialize(int comPort)
{
}

BOOL STM32_USART_Driver::TxBufferEmpty(int comPort)
{
}

void STM32_USART_Driver::TxBufferEmptyInterruptEnable(int comPort, BOOL enable)
{

}

BOOL STM32_USART_Driver::TxBufferEmptyInterruptState(int  comPort)
{

}

BOOL STM32_USART_Driver::TxHandshakeEnabledState(int comPort)
{
}

BOOL STM32_USART_Driver::TxShiftRegisterEmpty(int comPort)
{

}

void STM32_USART_Driver::WriteCharToTxBuffer(int comPort, UINT8 c)
{
}


void STM32_USART_Driver::RxBufferFullInterruptEnable(int comPort, BOOL enable)
{

}
