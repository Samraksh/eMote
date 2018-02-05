////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <stm32f10x.h>
#include "netmf_gpio.h"

//--//

const char* const c_strGpioBadCallback = "No GPIO callback defined";
const char* const c_strGpioBadPin = "Pin Number greater than max allowable pins";
const char* const c_strGpioBadSize = "size greater than max allowable pins";

//#define DEBUG_GPIO_VERBOSE 1

#if defined(DEBUG_GPIO_VERBOSE)
#define GPIO_DEBUG_PRINT(format, ...) hal_printf("[Native] [GPIO Driver] " format " at %d, %s \n", ##__VA_ARGS__, __LINE__, __FILE__)
#elif defined(DEBUG_GPIO)
#define GPIO_DEBUG_PRINT(format, ...) hal_printf("[Native] [GPIO Driver] " format "\n", ##__VA_ARGS__)
#else
#define GPIO_DEBUG_PRINT(format, ...)
#endif





// Not truly static, but passed by function pointers
static void EXTI0_IRQ_HANDLER(void *args);
static void EXTI1_IRQ_HANDLER(void *args);
static void EXTI2_IRQ_HANDLER(void *args);
static void EXTI3_IRQ_HANDLER(void *args);
static void EXTI4_IRQ_HANDLER(void *args);
static void EXTI9_5_IRQ_HANDLER(void *args);
static void EXTI15_10_IRQ_Handler(void *args);


static int GPIO_GetExtrCR(unsigned int line);
GPIO_TypeDef* GPIO_GetPortPtr(GPIO_PIN Pin);
uint16_t GPIO_GetPin(GPIO_PIN Pin);

#define NUMBER_OF_EXTI_LINES 16

const uint GPIO_PORTS = 7;
const uint GPIO_PPP = 16;
const uint GPIO_PINS = 112;


GPIO_TypeDef* GPIO_PORT_ARRAY[GPIO_PORTS] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};
UINT32 EXTILines[NUMBER_OF_EXTI_LINES] = {EXTI_Line0,EXTI_Line1,EXTI_Line2,EXTI_Line3,EXTI_Line4,EXTI_Line5,EXTI_Line6,EXTI_Line7,
											EXTI_Line8,EXTI_Line9,EXTI_Line10,EXTI_Line11,EXTI_Line12,EXTI_Line13,EXTI_Line14,EXTI_Line15};

static GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr[GPIO_PINS];
static void* gpio_parm[GPIO_PINS];
static uint8_t pin_reservations[14]; // 14*8 = 112

//Local Functions

static void handle_exti(unsigned int exti)
{
	if(EXTI_GetITStatus(exti) != RESET)
	{
		EXTI_ClearITPendingBit(exti);

		GPIO_INTERRUPT_SERVICE_ROUTINE my_isr;
		UINT32 line = GPIO_GetExtrCR(exti);
		UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7; // Assuming this is right, not double-checked --NPS
		UINT32 pin = line + port * GPIO_PPP;
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
}

static int GPIO_GetExtrCR(unsigned int line)
{
	if(line > (1 << NUMBER_OF_EXTI_LINES)) { return -1; }

	for(int i = 0; i < NUMBER_OF_EXTI_LINES; i++)
	{
		if(line & (1 << i)){
			return i;
		}
	}
	return -1;
}

static BOOL IsOutputPin(GPIO_PIN Pin)
{
	GPIO_TypeDef *port;
	unsigned int p;
	unsigned int mode;
	unsigned int mode_mask = 0x03;
	BOOL isOutput;

	if (Pin > GPIO_PINS) { return FALSE; }

	port = GPIO_GetPortPtr(Pin);
	p = GPIO_GetPin(Pin);

	if (p < 8) { // check mode in CRL
		mode = port->CRL;
		mode_mask = mode_mask << (p*4);
	}
	else { // check mode in CRH
		mode = port->CRH;
		mode_mask = mode_mask << (p*2);
	}

	if ( (mode & mode_mask) == 0 ) {
		isOutput = FALSE;
	}
	else {
		isOutput = TRUE;
	}
	return isOutput;
}

uint16_t GPIO_GetPin(GPIO_PIN Pin) {
	return (uint16_t)(0x1 << (Pin % GPIO_PPP));
}

uint8_t GPIO_GetPort(GPIO_PIN Pin) {
	return (uint8_t)(Pin / GPIO_PPP);
}

UINT32 GPIO_GetExtiLine(GPIO_PIN Pin){
	return EXTILines[Pin % GPIO_PPP];
}

GPIO_TypeDef* GPIO_GetPortPtr(GPIO_PIN Pin) {
	return GPIO_PORT_ARRAY[GPIO_GetPort(Pin)];
}

/**
 * stupid.
 * TODO: make this a giant macro.
 */
GPIO_PIN GPIO_GetNumber(GPIO_TypeDef* Port, uint16_t Pin) {
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
}

UINT32 GPIO_GetIRQNumber(GPIO_PIN Pin)
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
}

HAL_CALLBACK_FPN GPIO_GetCallBack(GPIO_PIN Pin)
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
}

//Exported Functions


// Initialize the ports GPIOA .... GPIOG of the Emote
BOOL CPU_GPIO_Initialize()
{
	// Configure clock source for all gpio ports
#ifndef PLATFORM_ARM_AUSTERE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
										RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);
#else
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); // Austere only has/uses GPIOs ABC
#endif // PLATFORM_ARM_AUSTERE

	return TRUE;
}

BOOL CPU_GPIO_Uninitialize()
{
	//GPIOA
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
	//GPIOB
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, DISABLE);
	//GPIOC
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, DISABLE);
#ifndef PLATFORM_ARM_AUSTERE
	//GPIOD
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, DISABLE);
	//GPIOE
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, DISABLE);
	//GPIOF
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, DISABLE);
	//GPIOG
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, DISABLE);
#endif
	//AFIO
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
	return TRUE;
}

BOOL CPU_GPIO_TogglePinState(GPIO_PIN Pin)
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return FALSE;
	}

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

//TODO: just call DisablePin...
void GPIO_ConfigurePinC( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed) {
    GPIO_ConfigurePin( GPIO_PortSource, Pin, mode, speed);
}

//TODO: keep track of resources.  Otherwise this function is pointless and clients should use the MSFT CPU_GPIO API.
void GPIO_ConfigurePin( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = Pin;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_Speed = speed;
	GPIO_Init(GPIO_PortSource, &GPIO_InitStructure);
}


void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return;
	}

	GPIO_TypeDef* port = GPIO_GetPortPtr(Pin);
	uint16_t pinInHex = GPIO_GetPin(Pin);
	GPIO_ConfigurePin(port, pinInHex ); // AIN disables digital input so should prevent interrupts etc.
	CPU_GPIO_SetPinState(Pin, FALSE);

	if ( gpio_isr[Pin] != NULL ) { // Need to deconfigure its interrupt as well.
		// Disable in NVIC
		CPU_INTC_InterruptDisable(GPIO_GetIRQNumber(Pin));

		// Kill EXTI interrupt source
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_StructInit(&EXTI_InitStructure);
		EXTI_InitStructure.EXTI_Line = GPIO_GetExtiLine(Pin);
		EXTI_Init(&EXTI_InitStructure);
	}

	// Remove any user interrupts
	gpio_isr[Pin] = NULL;
	gpio_parm[Pin] = NULL;
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

	GPIO_TypeDef* port = GPIO_GetPortPtr(Pin);
	uint16_t pinInHex = GPIO_GetPin(Pin);
	CPU_GPIO_SetPinState(Pin, InitialState);
	GPIO_ConfigurePin(port, pinInHex, GPIO_Mode_Out_PP);
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return FALSE;
	}

	// Short circuit for simple inputs.
	if ( PIN_ISR == NULL && IntEdge == GPIO_INT_NONE && ResistorState == RESISTOR_DISABLED && GlitchFilterEnable == false ) {
		GPIOMode_TypeDef mode = GPIO_Mode_IN_FLOATING;
		GPIO_TypeDef* port = GPIO_GetPortPtr(Pin);
		uint16_t pinInHex = GPIO_GetPin(Pin);
		GPIO_ConfigurePin(port, pinInHex, mode);
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

	GPIOMode_TypeDef mode;

	if (ResistorState == RESISTOR_DISABLED) {
		mode = GPIO_Mode_IN_FLOATING;
	} else if(ResistorState == RESISTOR_PULLDOWN) {
		mode = GPIO_Mode_IPD;
	} else if(ResistorState == RESISTOR_PULLUP) {
		mode = GPIO_Mode_IPU;
	} else {
		GPIO_DEBUG_PRINT("Invalid resistor configuration");
		return FALSE;
	}

	GPIO_TypeDef* port = GPIO_GetPortPtr(Pin);
	uint16_t pinInHex = GPIO_GetPin(Pin);
	GPIO_ConfigurePin(port, pinInHex, mode);
	
	return TRUE;
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return FALSE;
	}

	EXTI_InitTypeDef EXTI_InitStructure;

	GPIOMode_TypeDef mode;
	if (ResistorState == RESISTOR_DISABLED) {
		mode = GPIO_Mode_IN_FLOATING;
	} else if(ResistorState == RESISTOR_PULLDOWN) {
		mode = GPIO_Mode_IPD;
	} else if(ResistorState == RESISTOR_PULLUP) {
		mode = GPIO_Mode_IPU;
	} else mode = GPIO_Mode_IN_FLOATING;

	GPIO_TypeDef* port = GPIO_GetPortPtr(Pin);
	uint16_t pinInHex = GPIO_GetPin(Pin);
	GPIO_ConfigurePin(port, pinInHex, mode);

	// Interrupt support for the gpio pins
	GPIO_EXTILineConfig(GPIO_GetPort(Pin), (Pin % GPIO_PPP));

	if(PIN_ISR)
	{
		EXTI_InitStructure.EXTI_Line = GPIO_GetExtiLine(Pin);

		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

		if(GPIO_INT_EDGE_LOW == IntEdge)
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		else if(GPIO_INT_EDGE_HIGH == IntEdge)
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		else if(GPIO_INT_EDGE_BOTH == IntEdge)
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		// Not supported on the device
		else if(GPIO_INT_LEVEL_HIGH == IntEdge || GPIO_INT_LEVEL_LOW == IntEdge)
			return FALSE;

		// Initialize external interrupts
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		gpio_isr[Pin] = PIN_ISR;
		gpio_parm[Pin] = ISR_Param;

		CPU_INTC_ActivateInterrupt(GPIO_GetIRQNumber(Pin), (HAL_CALLBACK_FPN) GPIO_GetCallBack(Pin), NULL);
	}
	else
	{
		gpio_isr[Pin] = NULL;
		gpio_parm[Pin] = NULL;
	}

	return TRUE;
}

// Return the state of the pin
// In case the pin is configured as one of the input modes, read the input data register.
// If the pin is configured in one of the output modes, read the output data register
BOOL CPU_GPIO_GetPinState( GPIO_PIN Pin )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return FALSE;
	}

	GPIO_TypeDef* port = GPIO_GetPortPtr(Pin);
	uint16_t pinInHex = GPIO_GetPin(Pin);
	if(IsOutputPin(Pin)){
		return (BOOL)GPIO_ReadOutputDataBit(port, pinInHex);
	} else	{
		return (BOOL)GPIO_ReadInputDataBit(port, pinInHex);
	}
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
	if(Pin > GPIO_PINS)
	{
		GPIO_DEBUG_PRINT("%s", c_strGpioBadPin);
		return;
	}

	//If it isn't an output pin, then its not valid to Set(). EnableOutputPin() first.
	//If you call Set() and it isn't an output, nothing will happen.
	//if (!IsOutputPin(Pin)) { return; }

	GPIO_TypeDef* port = GPIO_GetPortPtr(Pin);
	uint16_t pinInHex = GPIO_GetPin(Pin);
	if(PinState) {
		GPIO_WriteBit(port, pinInHex, Bit_SET);
	} else {
		GPIO_WriteBit(port, pinInHex, Bit_RESET);
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
}
