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

// Not truly static, but passed by function pointers
/*static void EXTI0_IRQ_HANDLER(void *args);
static void EXTI1_IRQ_HANDLER(void *args);
static void EXTI2_IRQ_HANDLER(void *args);
static void EXTI3_IRQ_HANDLER(void *args);
static void EXTI4_IRQ_HANDLER(void *args);
static void EXTI9_5_IRQ_HANDLER(void *args);
static void EXTI15_10_IRQ_Handler(void *args);*/


//static int GPIO_GetExtrCR(unsigned int line);
uint16_t GPIO_GetPort(GPIO_PIN Pin);
uint16_t GPIO_GetPin(GPIO_PIN Pin); // this function will return a value of '1' shifted pin number times
uint16_t GPIO_GetPinNumber(GPIO_PIN Pin); // this function will return the pin number of the port (i.e. pin 3)

#define NUMBER_OF_EXTI_LINES 16

const uint GPIO_PORTS = 16;
const uint GPIO_PPP = 8; // pins per port
const uint GPIO_PINS = 128;


//GPIO_TypeDef* GPIO_PORT_ARRAY[GPIO_PORTS] = {PORT_0, PORT_1, PORT_2, PORT_3, PORT_4, PORT_5, PORT_6, PORT_7, PORT_8, PORT_9, PORT_10, PORT_11, PORT_12, PORT_13, PORT_14, PORT_15};
//UINT32 EXTILines[NUMBER_OF_EXTI_LINES] = {EXTI_Line0,EXTI_Line1,EXTI_Line2,EXTI_Line3,EXTI_Line4,EXTI_Line5,EXTI_Line6,EXTI_Line7,
//											EXTI_Line8,EXTI_Line9,EXTI_Line10,EXTI_Line11,EXTI_Line12,EXTI_Line13,EXTI_Line14,EXTI_Line15};

//static void (*callbacks[MXC_GPIO_NUM_PORTS][MXC_GPIO_MAX_PINS_PER_PORT])(void *);
static void (*callbacks[MXC_GPIO_NUM_PORTS][MXC_GPIO_MAX_PINS_PER_PORT])(GPIO_PIN Pin, BOOL PinState, void* Param );
static void *cbparam[MXC_GPIO_NUM_PORTS][MXC_GPIO_MAX_PINS_PER_PORT];
static uint8_t pin_reservations[GPIO_PORTS]; // 14*8 = 112

//Local Functions

/*static void handle_exti(unsigned int exti)
{
	if(EXTI_GetITStatus(exti) != RESET)
	{
		EXTI_ClearITPendingBit(exti);

		GPIO_INTERRUPT_SERVICE_ROUTINE my_isr;
		UINT32 line = GPIO_GetExtrCR(exti);
		UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7; // Assuming this is right, not double-checked --NPS
/		UINT32 pin = line + port * GPIO_PPP;
		void *parm;

		// The C# GPIO ISR will add this interrupt to g_CLR_HW_Hardware.m_interruptData.m_HalQueue via SaveToHALQueue(), which eventually gets added to m_interruptData.m_applicationQueue via CLR_HW_Hardware::TransferAllInterruptsToApplicationQueue() which eventually gets dispatched via CLR_HW_Hardware::SpawnDispatcher()
		my_isr = gpio_isr[pin];
		parm = gpio_parm[pin];
		if(my_isr != NULL)
		{
		    my_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)), parm);
		}
		// the code below was being triggered when a radio app was being deployed. During the app erase, the radio driver is uninitialized (including the GPIO interrupt line to the radio). The erase routine is
		// global locked and at some point the radio could request an interrupt which is not able to be serviced until after the erase is over. At that point the interupt fires but the GPIO interrupt line
		// has already been disabled and the callback function is now null. 
		// If problems occur from this code being commented out we can instead just clear the radio interrupt that is pending instead.
		//else
		//{
		    //GPIO_DEBUG_PRINT("%s",c_strGpioBadCallback);
		    //ASSERT(my_isr != NULL);
		//}
	}
}*/

/*static int GPIO_GetExtrCR(unsigned int line)
{
	if(line > (1 << NUMBER_OF_EXTI_LINES)) { return -1; }

	for(int i = 0; i < NUMBER_OF_EXTI_LINES; i++)
	{
		if(line & (1 << i)){
			return i;
		}
	}
	return -1;
}*/

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

/*uint16_t GPIO_GetExtiLine(GPIO_PIN Pin){
	return EXTILines[Pin % GPIO_PPP];
}*/

/*uint16_t GPIO_GetPortPtr(GPIO_PIN Pin) {
	return GPIO_PORT_ARRAY[GPIO_GetPort(Pin)];
}*/

/**
 * stupid.
 * TODO: make this a giant macro.
 */
/*GPIO_PIN GPIO_GetNumber(GPIO_TypeDef* Port, uint16_t Pin) {
    unsigned itr_port;
    for(itr_port=0; itr_port < GPIO_PORTS; ++itr_port) {
        if(GPIO_PORT_ARRAY[itr_port] == Port) {
            break;
        }
    }
    ASSERT(itr_port < GPIO_PORTS);
    ASSERT(Pin < (1 << GPIO_PPP));
    unsigned itr_pin;
    for(itr_pin=0; itr_pin < GPIO_PPP; ++itr_pin) {
        if((Pin >> itr_pin) == 0x1) {
            break;
        }
    }
    ASSERT(itr_pin < GPIO_PPP);
    return ((itr_port * GPIO_PPP) + itr_pin);
}*/

/*UINT32 GPIO_GetIRQNumber(GPIO_PIN Pin)
{
	UINT32 line = Pin % GPIO_PPP;
	UINT32 irq_number;

	switch(line)
	{
	case	0:
		irq_number = EXTI0_IRQn;
		break;
	case 	1:
		irq_number = EXTI1_IRQn;
		break;
	case 	2:
		irq_number = EXTI2_IRQn;
		break;
	case    3:
		irq_number = EXTI3_IRQn;
		break;
	case    4:
		irq_number = EXTI4_IRQn;
		break;
	case	5:
	case	6:
	case    7:
	case    8:
	case    9:
		irq_number = EXTI9_5_IRQn;
		break;
	case   10:
	case   11:
	case   12:
	case   13:
	case   14:
	case   15:
		irq_number = EXTI15_10_IRQn;
		break;

	}

	return irq_number;
}*/

/*HAL_CALLBACK_FPN GPIO_GetCallBack(GPIO_PIN Pin)
{
	UINT32 line = Pin % GPIO_PPP;
	HAL_CALLBACK_FPN callback;

	switch(line)
	{
	case	0:
		callback = EXTI0_IRQ_HANDLER;
		break;
	case 	1:
		callback = EXTI1_IRQ_HANDLER;
		break;
	case 	2:
		callback = EXTI2_IRQ_HANDLER;
		break;
	case    3:
		callback = EXTI3_IRQ_HANDLER;
		break;
	case    4:
		callback = EXTI4_IRQ_HANDLER;
		break;
	case	5:
	case	6:
	case    7:
	case    8:
	case    9:
		callback = EXTI9_5_IRQ_HANDLER;
		break;
	case   10:
	case   11:
	case   12:
	case   13:
	case   14:
	case   15:
		callback = EXTI15_10_IRQ_Handler;
		break;

	}

	return callback;
}*/

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
	gpio_cfg_t pin = { port, port_pin, GPIO_FUNC_GPIO, GPIO_PAD_OPEN_DRAIN };

	GPIO_Config(&pin);

	// Remove any user interrupts
	//gpio_isr[Pin] = NULL;
	//gpio_parm[Pin] = NULL;
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

	//EXTI_InitTypeDef EXTI_InitStructure;

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
		NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(pinCfg.port));

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



/*
void EXTI0_IRQ_HANDLER(void *args)
{
	handle_exti(EXTI_Line0);
}

void EXTI1_IRQ_HANDLER(void *args)
{
	GLOBAL_LOCK(EXTI1);
	handle_exti(EXTI_Line1);
}

void EXTI2_IRQ_HANDLER(void *args)
{
	handle_exti(EXTI_Line2);
}

void EXTI3_IRQ_HANDLER(void *args)
{
	handle_exti(EXTI_Line3);
}

void EXTI4_IRQ_HANDLER(void *args)
{
	handle_exti(EXTI_Line4);
}

void EXTI9_5_IRQ_HANDLER(void *args)
{
	handle_exti(EXTI_Line5);
	handle_exti(EXTI_Line6);
	handle_exti(EXTI_Line7);
	handle_exti(EXTI_Line8);
	handle_exti(EXTI_Line9);
}

void EXTI15_10_IRQ_Handler(void *args)
{
	handle_exti(EXTI_Line10);
	handle_exti(EXTI_Line11);
	handle_exti(EXTI_Line12);
	handle_exti(EXTI_Line13);
	handle_exti(EXTI_Line14);
	handle_exti(EXTI_Line15);
}*/
