////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <max3263x.h>
#include "netmf_gpio.h"
#include <gpio.h>

//--//

#if defined(DEBUG_GPIO_VERBOSE)
#define GPIO_DEBUG_PRINT(format, ...) hal_printf("[Native] [GPIO Driver] " format " at %d, %s \n", ##__VA_ARGS__, __LINE__, __FILE__)
#elif defined(DEBUG_GPIO)
#define GPIO_DEBUG_PRINT(format, ...) hal_printf("[Native] [GPIO Driver] " format "\n", ##__VA_ARGS__)
#else
#define GPIO_DEBUG_PRINT(format, ...)
#endif

//static int GPIO_GetExtrCR(unsigned int line);
uint16_t GPIO_GetPort(GPIO_PIN Pin);
uint16_t GPIO_GetPin(GPIO_PIN Pin); // this function will return a value of '1' shifted pin number times
uint16_t GPIO_GetPinNumber(GPIO_PIN Pin); // this function will return the pin number of the port (i.e. pin 3)

#define NUMBER_OF_EXTI_LINES 16

const uint GPIO_PORTS = 16;
const uint GPIO_PPP = 8; // pins per port
const uint GPIO_PINS = 128;

//static void (*callbacks[MXC_GPIO_NUM_PORTS][MXC_GPIO_MAX_PINS_PER_PORT])(void *);
static void (*callbacks[MXC_GPIO_NUM_PORTS][MXC_GPIO_MAX_PINS_PER_PORT])(GPIO_PIN Pin, BOOL PinState, void* Param );
static void *cbparam[MXC_GPIO_NUM_PORTS][MXC_GPIO_MAX_PINS_PER_PORT];
static uint8_t pin_reservations[GPIO_PORTS]; // 14*8 = 112

//Local Functions

void P0_HANDLER(void *args)
{
	GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr;
	uint8_t intfl;
	uint32_t port = 0;
	uint32_t port_pin_number = 0;
	uint32_t port_read;
	BOOL port_read_bool;
	void *param;

	// Read and clear enabled interrupts.
    intfl = MXC_GPIO->intfl[port];
    intfl &= MXC_GPIO->inten[port];
	MXC_GPIO->intfl[port] = intfl;

	// Process each pins' interrupt
    for (port_pin_number = 0; port_pin_number < MXC_GPIO_MAX_PINS_PER_PORT; port_pin_number++) {
        if ((intfl & (1 << port_pin_number))) {
            gpio_isr = callbacks[port][port_pin_number];
			param = cbparam[port][port_pin_number];
			if(gpio_isr != NULL)
			{
				port_read = MXC_GPIO->in_val[port];
				port_read_bool = (BOOL)((port_read >> port_pin_number) & 0x1);
	    		gpio_isr(port_pin_number, port_read_bool, param);
			}
        }
    }
}
void P1_HANDLER(void *args)
{
	GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr;
	uint8_t intfl;
	uint32_t port = 1;
	uint32_t port_pin_number = 0;
	uint32_t port_read;
	BOOL port_read_bool;
	void *param;

	// Read and clear enabled interrupts.
    intfl = MXC_GPIO->intfl[port];
    intfl &= MXC_GPIO->inten[port];
	MXC_GPIO->intfl[port] = intfl;

	// Process each pins' interrupt
    for (port_pin_number = 0; port_pin_number < MXC_GPIO_MAX_PINS_PER_PORT; port_pin_number++) {
        if ((intfl & (1 << port_pin_number))) {
            gpio_isr = callbacks[port][port_pin_number];
			param = cbparam[port][port_pin_number];
			if(gpio_isr != NULL)
			{
				port_read = MXC_GPIO->in_val[port];
				port_read_bool = (BOOL)((port_read >> port_pin_number) & 0x1);
	    		gpio_isr(port_pin_number, port_read_bool, param);
			}
        }
    }
}
void P2_HANDLER(void *args)
{
	GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr;
	uint8_t intfl;
	uint32_t port = 2;
	uint32_t port_pin_number = 0;
	uint32_t port_read;
	BOOL port_read_bool;
	void *param;

	// Read and clear enabled interrupts.
    intfl = MXC_GPIO->intfl[port];
    intfl &= MXC_GPIO->inten[port];
	MXC_GPIO->intfl[port] = intfl;

	// Process each pins' interrupt
    for (port_pin_number = 0; port_pin_number < MXC_GPIO_MAX_PINS_PER_PORT; port_pin_number++) {
        if ((intfl & (1 << port_pin_number))) {
            gpio_isr = callbacks[port][port_pin_number];
			param = cbparam[port][port_pin_number];
			if(gpio_isr != NULL)
			{
				port_read = MXC_GPIO->in_val[port];
				port_read_bool = (BOOL)((port_read >> port_pin_number) & 0x1);
	    		gpio_isr(port_pin_number, port_read_bool, param);
			}
        }
    }
}
void P3_HANDLER(void *args)
{
	GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr;
	uint8_t intfl;
	uint32_t port = 3;
	uint32_t port_pin_number = 0;
	uint32_t port_read;
	BOOL port_read_bool;
	void *param;

	// Read and clear enabled interrupts.
    intfl = MXC_GPIO->intfl[port];
    intfl &= MXC_GPIO->inten[port];
	MXC_GPIO->intfl[port] = intfl;

	// Process each pins' interrupt
    for (port_pin_number = 0; port_pin_number < MXC_GPIO_MAX_PINS_PER_PORT; port_pin_number++) {
        if ((intfl & (1 << port_pin_number))) {
            gpio_isr = callbacks[port][port_pin_number];
			param = cbparam[port][port_pin_number];
			if(gpio_isr != NULL)
			{
				port_read = MXC_GPIO->in_val[port];
				port_read_bool = (BOOL)((port_read >> port_pin_number) & 0x1);
	    		gpio_isr(port_pin_number, port_read_bool, param);
			}
        }
    }
}
void P4_HANDLER(void *args)
{
	GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr;
	uint8_t intfl;
	uint32_t port = 4;
	uint32_t port_pin_number = 0;
	uint32_t port_read;
	BOOL port_read_bool;
	void *param;

	// Read and clear enabled interrupts.
    intfl = MXC_GPIO->intfl[port];
    intfl &= MXC_GPIO->inten[port];
	MXC_GPIO->intfl[port] = intfl;

	// Process each pins' interrupt
    for (port_pin_number = 0; port_pin_number < MXC_GPIO_MAX_PINS_PER_PORT; port_pin_number++) {
        if ((intfl & (1 << port_pin_number))) {
            gpio_isr = callbacks[port][port_pin_number];
			param = cbparam[port][port_pin_number];
			if(gpio_isr != NULL)
			{
				port_read = MXC_GPIO->in_val[port];
				port_read_bool = (BOOL)((port_read >> port_pin_number) & 0x1);
	    		gpio_isr(port_pin_number, port_read_bool, param);
			}
        }
    }
}
void P5_HANDLER(void *args)
{
	GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr;
	uint8_t intfl;
	uint32_t port = 5;
	uint32_t port_pin_number = 0;
	uint32_t port_read;
	BOOL port_read_bool;
	void *param;

	// Read and clear enabled interrupts.
    intfl = MXC_GPIO->intfl[port];
    intfl &= MXC_GPIO->inten[port];
	MXC_GPIO->intfl[port] = intfl;

	// Process each pins' interrupt
    for (port_pin_number = 0; port_pin_number < MXC_GPIO_MAX_PINS_PER_PORT; port_pin_number++) {
        if ((intfl & (1 << port_pin_number))) {
            gpio_isr = callbacks[port][port_pin_number];
			param = cbparam[port][port_pin_number];
			if(gpio_isr != NULL)
			{
				port_read = MXC_GPIO->in_val[port];
				port_read_bool = (BOOL)((port_read >> port_pin_number) & 0x1);
	    		gpio_isr(port_pin_number, port_read_bool, param);
			}
        }
    }
}

void P6_HANDLER(void *args)
{
	GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr;
	uint8_t intfl;
	uint32_t port = 6;
	uint32_t port_pin_number = 0;
	uint32_t port_read;
	BOOL port_read_bool;
	void *param;

	// Read and clear enabled interrupts.
    intfl = MXC_GPIO->intfl[port];
    intfl &= MXC_GPIO->inten[port];
	MXC_GPIO->intfl[port] = intfl;

	// Process each pins' interrupt
    for (port_pin_number = 0; port_pin_number < MXC_GPIO_MAX_PINS_PER_PORT; port_pin_number++) {
        if ((intfl & (1 << port_pin_number))) {
            gpio_isr = callbacks[port][port_pin_number];
			param = cbparam[port][port_pin_number];
			if(gpio_isr != NULL)
			{
				port_read = MXC_GPIO->in_val[port];
				port_read_bool = (BOOL)((port_read >> port_pin_number) & 0x1);
	    		gpio_isr(port_pin_number, port_read_bool, param);
			}
        }
    }
}

static BOOL IsOutputPin(GPIO_PIN Pin)
{
	uint16_t port;
	uint16_t port_pin_number;
	BOOL isOutput;

	if (Pin > GPIO_PINS) { return FALSE; }

	port = GPIO_GetPort(Pin);
	port_pin_number = GPIO_GetPinNumber(Pin);

	uint32_t out_mode = MXC_GPIO->out_mode[port];
	uint32_t func_sel = MXC_GPIO->func_sel[port];

	out_mode = (out_mode >> (4 * port_pin_number)) & 0xf;
	func_sel = (func_sel >> (4 * port_pin_number)) & 0xf;

	isOutput = FALSE;
	if ((out_mode == GPIO_PAD_NORMAL) | (out_mode == GPIO_PAD_SLOW) | (out_mode == GPIO_PAD_FAST))
		isOutput = TRUE;
	else
		isOutput = FALSE;

	if ((func_sel == GPIO_FUNC_GPIO) & (isOutput == TRUE ))
		return TRUE;
	else
		return FALSE;
}

uint16_t GPIO_GetPin(GPIO_PIN Pin) {
	return (uint16_t)(0x1 << (Pin % GPIO_PPP));
}

uint16_t GPIO_GetPinNumber(GPIO_PIN Pin) {
	return (uint16_t)(Pin % GPIO_PPP);
}

uint16_t GPIO_GetPort(GPIO_PIN Pin) {
	return (uint16_t)(Pin / GPIO_PPP);
}

uint32_t GPIO_GetIRQNumber(uint16_t port)
{
	UINT32 irq_number;

	switch(port)
	{
	case	0:
		irq_number = GPIO_P0_IRQn;
		break;
	case 	1:
		irq_number = GPIO_P1_IRQn;
		break;
	case 	2:
		irq_number = GPIO_P2_IRQn;
		break;
	case    3:
		irq_number = GPIO_P3_IRQn;
		break;
	case    4:
		irq_number = GPIO_P4_IRQn;
		break;
	case	5:
		irq_number = GPIO_P5_IRQn;
		break;
	case	6:
		irq_number = GPIO_P6_IRQn;
		break;
	default:
		irq_number = NULL;
		break;
	}

	return irq_number;
}

HAL_CALLBACK_FPN GPIO_GetCallBack(uint16_t port)
{
	HAL_CALLBACK_FPN callback;

	switch(port)
	{
	case	0:
		callback = P0_HANDLER;
		break;
	case 	1:
		callback = P1_HANDLER;
		break;
	case 	2:
		callback = P2_HANDLER;
		break;
	case    3:
		callback = P3_HANDLER;
		break;
	case    4:
		callback = P4_HANDLER;
		break;
	case	5:
		callback = P5_HANDLER;
		break;
	case	6:
		callback = P6_HANDLER;
		break;
	default:
		callback = NULL;
		break;
	}

	return callback;
}

//Exported Functions


// Initialize the ports GPIOA .... GPIOG of the Emote
BOOL CPU_GPIO_Initialize()
{
	

	return TRUE;
}

BOOL CPU_GPIO_Uninitialize()
{
	
	return TRUE;
}

BOOL CPU_GPIO_TogglePinState(GPIO_PIN Pin)
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return FALSE;
	}

	uint16_t port;
	uint16_t port_pin;
	port = GPIO_GetPort(Pin);
	port_pin = GPIO_GetPin(Pin);


	gpio_cfg_t pin = { port, port_pin, GPIO_FUNC_GPIO, GPIO_PAD_NORMAL };

	BOOL pinState = CPU_GPIO_GetPinState(Pin);
	pinState = !pinState;
	CPU_GPIO_SetPinState(Pin, pinState);

	return pinState;
}

/**
 * Note: Microsoft.SPOT>Hardware.Port uses CPU_GPIO_Attributes to do basic verification that C# may touch a port.
 */
UINT32 CPU_GPIO_Attributes( GPIO_PIN Pin )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return 0;
	}
	return (GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT);
}

void GPIO_ConfigurePin( uint16_t port, uint16_t Pin, gpio_func_t function, gpio_pad_t pad)
{
	gpio_cfg_t pin = { port, Pin, function, pad };

	GPIO_Config(&pin);
}


void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return;
	}

	uint32_t port = GPIO_GetPort(Pin);
	uint32_t port_pin = GPIO_GetPin(Pin);
	uint16_t port_pin_number = GPIO_GetPinNumber(Pin);
	gpio_cfg_t pin = { port, port_pin, GPIO_FUNC_GPIO, GPIO_PAD_OPEN_DRAIN };

	GPIO_Config(&pin);

	// Remove any user interrupts
	callbacks[port][port_pin_number] = NULL;
    cbparam[port][port_pin_number] = NULL;
}

// Configure the pin as an output pin, strange that this does not have a return type
void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
	// Check if the input pin is less than max pins
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return;
	}

	uint16_t port = GPIO_GetPort(Pin);
	uint16_t pin = GPIO_GetPin(Pin);
	CPU_GPIO_SetPinState(Pin, InitialState);
	GPIO_ConfigurePin(port, pin, GPIO_FUNC_GPIO, GPIO_PAD_NORMAL);
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return FALSE;
	}

	gpio_pad_t mode;
	if (ResistorState == RESISTOR_DISABLED) {
		mode = GPIO_PAD_INPUT;
	} else if(ResistorState == RESISTOR_PULLDOWN) {
		mode = GPIO_PAD_INPUT_PULLDOWN;
	} else if(ResistorState == RESISTOR_PULLUP) {
		mode = GPIO_PAD_INPUT_PULLUP;
	} else {
		GPIO_DEBUG_PRINT("Invalid resistor configuration");
		return FALSE;
	}

	// Short circuit for simple inputs.
	if ( PIN_ISR == NULL && IntEdge == GPIO_INT_NONE && GlitchFilterEnable == false ) {
		uint16_t port = GPIO_GetPort(Pin);
		uint16_t pin = GPIO_GetPin(Pin);
		GPIO_ConfigurePin(port, pin, GPIO_FUNC_GPIO, mode);
		return TRUE;
	}

	return CPU_GPIO_EnableInputPin2(Pin, GlitchFilterEnable, PIN_ISR, NULL, IntEdge, ResistorState );
}

BOOL CPU_GPIO_EnableInputPin3( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return FALSE;
	}

	gpio_pad_t mode;

	if (ResistorState == RESISTOR_DISABLED) {
		mode = GPIO_PAD_INPUT;
	} else if(ResistorState == RESISTOR_PULLDOWN) {
		mode = GPIO_PAD_INPUT_PULLDOWN;
	} else if(ResistorState == RESISTOR_PULLUP) {
		mode = GPIO_PAD_INPUT_PULLUP;
	} else {
		GPIO_DEBUG_PRINT("Invalid resistor configuration");
		return FALSE;
	}

	uint16_t port = GPIO_GetPort(Pin);
	uint16_t pin = GPIO_GetPin(Pin);
	GPIO_ConfigurePin(port, pin, GPIO_FUNC_GPIO, mode);
	
	return TRUE;
}

static void IntConfig(unsigned int port, unsigned int pin, gpio_int_mode_t mode)
{
    uint32_t int_mode = MXC_GPIO->int_mode[port];
    int_mode &= ~(0xF << (pin*4));
    int_mode |= (mode << (pin*4));
    MXC_GPIO->int_mode[port] = int_mode;
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return FALSE;
	}

	gpio_pad_t mode;

	if (ResistorState == RESISTOR_DISABLED) {
		mode = GPIO_PAD_INPUT;
	} else if(ResistorState == RESISTOR_PULLDOWN) {
		mode = GPIO_PAD_INPUT_PULLDOWN;
	} else if(ResistorState == RESISTOR_PULLUP) {
		mode = GPIO_PAD_INPUT_PULLUP;
	} else {
		GPIO_DEBUG_PRINT("Invalid resistor configuration");
		return FALSE;
	}

	gpio_int_mode_t interruptMode;
	if (IntEdge == GPIO_INT_NONE) {
		interruptMode = GPIO_INT_DISABLE;
	} else if(IntEdge == GPIO_INT_EDGE_LOW) {
		interruptMode = GPIO_INT_FALLING_EDGE;
	} else if(IntEdge == GPIO_INT_EDGE_HIGH) {
		interruptMode = GPIO_INT_RISING_EDGE;
	} else if(IntEdge == GPIO_INT_EDGE_BOTH) {
		interruptMode = GPIO_INT_ANY_EDGE;	 
	} else if(IntEdge == GPIO_INT_LEVEL_HIGH) {
		interruptMode = GPIO_INT_HIGH_LEVEL;
	} else if(IntEdge == GPIO_INT_LEVEL_LOW) {
		interruptMode = GPIO_INT_LOW_LEVEL;
	} else {
		GPIO_DEBUG_PRINT("Invalid interruptMode configuration");
		return FALSE;
	}

	uint16_t port = GPIO_GetPort(Pin);
	uint16_t pin = GPIO_GetPin(Pin);
	uint16_t port_pin_number = GPIO_GetPinNumber(Pin);
	GPIO_ConfigurePin(port, pin, GPIO_FUNC_GPIO, mode);

	if (PIN_ISR)
	{
		IntConfig(port, port_pin_number, interruptMode);
	  	callbacks[port][port_pin_number] = PIN_ISR;
        cbparam[port][port_pin_number] = ISR_Param;
		gpio_cfg_t pinCfg = { port, pin, GPIO_FUNC_GPIO, mode };
		GPIO_IntEnable(&pinCfg);
		CPU_INTC_ActivateInterrupt(GPIO_GetIRQNumber(port), GPIO_GetCallBack(port), NULL);
	} else {
		callbacks[port][port_pin_number] = NULL;
        cbparam[port][port_pin_number] = NULL;
	}

	return TRUE;
}

// Return the state of the pin
// In case the pin is configured as one of the input modes, read the input data register.
// If the pin is configured in one of the output modes, read the output data register
BOOL CPU_GPIO_GetPinState( GPIO_PIN Pin )
{
	uint16_t port;
	uint16_t port_pin_number;
	BOOL pinState = FALSE;

	if (Pin > GPIO_PINS) { return FALSE; }

	port = GPIO_GetPort(Pin);
	port_pin_number = GPIO_GetPinNumber(Pin);

	uint32_t out_mode = MXC_GPIO->out_mode[port];
	uint32_t func_sel = MXC_GPIO->func_sel[port];
	uint32_t port_value;

	out_mode = (out_mode >> (4 * port_pin_number)) & 0xf;
	func_sel = (func_sel >> (4 * port_pin_number)) & 0xf;

	if (func_sel != GPIO_FUNC_GPIO)
		return FALSE;

	if ((out_mode == GPIO_PAD_NORMAL) | (out_mode == GPIO_PAD_SLOW) | (out_mode == GPIO_PAD_FAST)){
		port_value = MXC_GPIO->out_val[port];
		port_value = (port_value >>  port_pin_number) & 0x1;
	} else if ((out_mode == GPIO_PAD_INPUT) | (out_mode == GPIO_PAD_INPUT_PULLUP) | (out_mode == GPIO_PAD_INPUT_PULLDOWN)){
		port_value = MXC_GPIO->in_val[port];
		port_value = (port_value >> port_pin_number) & 0x1;
	} else {
		return FALSE;
	}

	pinState = port_value;

	return pinState;
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return;
	}

	uint16_t port;
	uint16_t port_pin;
	port = GPIO_GetPort(Pin);
	port_pin = GPIO_GetPin(Pin);

	gpio_cfg_t pin = { port, port_pin, GPIO_FUNC_GPIO, GPIO_PAD_NORMAL };

	if(PinState) {
		GPIO_OutSet(&pin);
	} else {
		GPIO_OutClr(&pin);
	}
}


// Check if the pin is busy or in use, takes the Pin number as the input argument
BOOL CPU_GPIO_PinIsBusy( GPIO_PIN Pin )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return FALSE;
	}

	unsigned int bit = 1 << (Pin % 8);
	unsigned int byte = Pin / 8;
	if (pin_reservations[byte] & bit) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// This function is used to reserve and unreserve pins to avoid gpio resource conflict issues
BOOL CPU_GPIO_ReservePin( GPIO_PIN Pin, BOOL fReserve )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return FALSE;
	}

	unsigned int bit = 1 << (Pin % 8);
	unsigned int byte = Pin / 8;
	if (pin_reservations[byte] & bit) {
		if (fReserve) { return FALSE; } // Can't reserve it, busy, fail.
		else { pin_reservations[byte] &= ~bit; } // Un-reserve a busy pin.
	}
	else {
		if (fReserve) { pin_reservations[byte] |= bit; } // Reserve it.
		// Nothing to do for case where un-reserving a non-busy pin
	}
	return TRUE;
}

//STUB
UINT32 CPU_GPIO_GetDebounce()
{
    return 0;
}

//STUB
BOOL CPU_GPIO_SetDebounce( INT64 debounceTimeMilliseconds )
{
    return FALSE;
}

INT32 CPU_GPIO_GetPinCount()
{
    return GPIO_PINS;
} 

void CPU_GPIO_GetPinsMap( UINT8* pins, size_t size )
{
	if(size > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadSize);
		return;
	}
    UINT8 i;
	for(i = 0; i < size; i++) {
		pins[i] = CPU_GPIO_Attributes(i);	//GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT;
	}
}

UINT8 CPU_GPIO_GetSupportedResistorModes( GPIO_PIN Pin )
{
	return ((1<<RESISTOR_DISABLED) | (1<<RESISTOR_PULLUP) | (1 << RESISTOR_PULLDOWN));
}

UINT8 CPU_GPIO_GetSupportedInterruptModes( GPIO_PIN pin )
{
    return ((1<<GPIO_INT_EDGE_LOW) | (1<<GPIO_INT_EDGE_HIGH ) | (1<<GPIO_INT_EDGE_BOTH));
}




