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
	struct PIN_ISR_DESCRIPTOR
	{
			 GPIO_PIN 						m_pin;
			 GPIO_INTERRUPT_SERVICE_ROUTINE m_isr;
			 void*                          m_param;
			 GPIO_INT_EDGE                  m_intEdge;

	};

	static const UINT32 c_MaxPins           = 112;
	static const UINT32 c_MaxPorts          =   7;
	static const UINT32 c_PinsPerPort       =  16;

	static const UINT8 c_Gpio_Attributes[c_MaxPins];

	static const UINT32 c_DirectionOut = 0;
	static const UINT32 c_DirectionIn = 1;

	// Call backs
	PIN_ISR_DESCRIPTOR m_pinIsr[c_MaxPins];

	// Store information regarding whether a pin is reserved or not
	UINT16 m_pinReservationInfo[c_MaxPorts];

	static const UINT8 c_NumberofResistorMode = 3;
    static const UINT8 c_GPIO_ResistorMode= (1<<RESISTOR_DISABLED) | (1<<RESISTOR_PULLUP) | (1 << RESISTOR_PULLDOWN);

    // applies to all GPIO pins
    static const UINT8 c_NumberofInterruptMode = 3;
    static const UINT8 c_GPIO_InterruptMode= (1<<GPIO_INT_EDGE_LOW) | (1<<GPIO_INT_EDGE_HIGH ) | (1<<GPIO_INT_EDGE_BOTH);
};

void GPIO_ConfigurePin( GPIO_TypeDef* GPIO_PortSource, GPIO_PIN Pin, GPIOMode_TypeDef mode = GPIO_Mode_IN_FLOATING, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);
void GPIO_ConfigurePin( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode = GPIO_Mode_IN_FLOATING, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);

//STM32F10x_GPIO_Driver g_STM32F10x_Gpio_Driver;

#endif
