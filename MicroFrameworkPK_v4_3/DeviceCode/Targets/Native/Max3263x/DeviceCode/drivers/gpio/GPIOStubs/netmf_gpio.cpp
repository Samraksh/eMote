////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "netmf_gpio.h"

//--//

const char* const c_strGpioBadCallback = "No GPIO callback defined";
const char* const c_strGpioBadPin = "Pin Number greater than max allowable pins";
const char* const c_strGpioBadSize = "size greater than max allowable pins";




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


GPIO_TypeDef* GPIO_PORT_ARRAY[GPIO_PORTS] = {0, 0, 0, 0, 0, 0, 0};
UINT32 EXTILines[NUMBER_OF_EXTI_LINES] = {0,0,0,0,0,0,0,0,
											0,0,0,0,0,0,0,0};

static GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr[GPIO_PINS];
static void* gpio_parm[GPIO_PINS];
static uint8_t pin_reservations[14]; // 14*8 = 112

//Local Functions

static void handle_exti(unsigned int exti)
{

}

static int GPIO_GetExtrCR(unsigned int line)
{

	return -1;
}

static BOOL IsOutputPin(GPIO_PIN Pin)
{
	BOOL isOutput = false;

	return isOutput;
}

uint16_t GPIO_GetPin(GPIO_PIN Pin) {
	return 0;
}

uint8_t GPIO_GetPort(GPIO_PIN Pin) {
	return 0;
}

UINT32 GPIO_GetExtiLine(GPIO_PIN Pin){
	return 0;
}

GPIO_TypeDef* GPIO_GetPortPtr(GPIO_PIN Pin) {
	return GPIO_PORT_ARRAY[GPIO_GetPort(Pin)];
}

GPIO_PIN GPIO_GetNumber(GPIO_TypeDef* Port, uint16_t Pin) {

    return 0;
}

UINT32 GPIO_GetIRQNumber(GPIO_PIN Pin)
{
	UINT32 irq_number = 0;



	return irq_number;
}

HAL_CALLBACK_FPN GPIO_GetCallBack(GPIO_PIN Pin)
{


	return EXTI0_IRQ_HANDLER;
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

	return false;
}


UINT32 CPU_GPIO_Attributes( GPIO_PIN Pin )
{

	return (GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT);
}

//TODO: just call DisablePin...
void GPIO_ConfigurePinC( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed) {

}

//TODO: keep track of resources.  Otherwise this function is pointless and clients should use the MSFT CPU_GPIO API.
void GPIO_ConfigurePin( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed)
{

}


void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{

}

// Configure the pin as an output pin, strange that this does not have a return type
void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{

}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{

		return TRUE;

}

BOOL CPU_GPIO_EnableInputPin3( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	
	return TRUE;
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{


	return TRUE;
}

// Return the state of the pin
// In case the pin is configured as one of the input modes, read the input data register.
// If the pin is configured in one of the output modes, read the output data register
BOOL CPU_GPIO_GetPinState( GPIO_PIN Pin )
{

		return false;
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{

}


// Check if the pin is busy or in use, takes the Pin number as the input argument
BOOL CPU_GPIO_PinIsBusy( GPIO_PIN Pin )
{

		return FALSE;
}

// This function is used to reserve and unreserve pins to avoid gpio resource conflict issues
BOOL CPU_GPIO_ReservePin( GPIO_PIN Pin, BOOL fReserve )
{

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

}

UINT8 CPU_GPIO_GetSupportedResistorModes( GPIO_PIN Pin )
{
	return 0;
}

UINT8 CPU_GPIO_GetSupportedInterruptModes( GPIO_PIN pin )
{
	return 0;
}




void EXTI0_IRQ_HANDLER(void *args)
{
}

void EXTI1_IRQ_HANDLER(void *args)
{
}

void EXTI2_IRQ_HANDLER(void *args)
{
}

void EXTI3_IRQ_HANDLER(void *args)
{
}

void EXTI4_IRQ_HANDLER(void *args)
{
}

void EXTI9_5_IRQ_HANDLER(void *args)
{

}

void EXTI15_10_IRQ_Handler(void *args)
{

}
