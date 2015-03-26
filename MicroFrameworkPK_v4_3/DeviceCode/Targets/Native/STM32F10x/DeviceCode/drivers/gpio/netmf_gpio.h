#ifndef _NETMF_GPIO_H_
#define _NETMF_GPIO_H_


#include <tinyhal.h>
#include <stm32f10x.h>

#define DEBUG_GPIO 1

#if defined(DEBUG_GPIO)
#define GPIO_DEBUG_PRINT1(x) hal_printf(x)
#define GPIO_DEBUG_PRINT2(x,y,z) hal_printf(x,y,z)
#else
#define GPIO_DEBUG_PRINT1(x)
#define GPIO_DEBUG_PRINT2(x,y,z)
#endif

struct STM32F10x_GPIO_Driver
{
public:

	static const UINT32 c_MaxPins           = 112;
	static const UINT32 c_MaxPorts          =   7;
	static const UINT32 c_PinsPerPort       =  16;

	static const UINT32 c_DirectionOut = 0;
	static const UINT32 c_DirectionIn = 1;
};

void GPIO_ConfigurePin( GPIO_TypeDef* GPIO_PortSource, GPIO_PIN Pin, GPIOMode_TypeDef mode = GPIO_Mode_IN_FLOATING, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);
void GPIO_ConfigurePin( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode = GPIO_Mode_IN_FLOATING, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);


#endif
