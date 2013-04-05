#ifndef _NETMF_GPIO_H_
#define _NETMF_GPIO_H_

#include <tinyhal.h>

struct STM32F10x_GPIO_Driver
{
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

#endif
