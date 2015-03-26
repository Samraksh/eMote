////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <stm32f10x.h>
#include "netmf_gpio.h"

//--//

#define NUMBER_OF_EXTI_LINES 16
#define GPIO_PORTS	7
#define	GPIO_PPP	16
#define GPIO_PINS	(GPIO_PORTS * GPIO_PPP)	//112

// Not truly static, but passed by function pointers
static void EXTI0_IRQ_HANDLER(void *args);
static void EXTI1_IRQ_HANDLER(void *args);
static void EXTI2_IRQ_HANDLER(void *args);
static void EXTI3_IRQ_HANDLER(void *args);
static void EXTI4_IRQ_HANDLER(void *args);
static void EXTI9_5_IRQ_HANDLER(void *args);
static void EXTI15_10_IRQ_Handler(void *args);
//static void Default_EXTI_Handler(void *data);
//static void STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param );

//--//

//Local Functions
static GPIO_InitTypeDef GPIO_Instances[GPIO_PINS];
static GPIO_TypeDef* GPIO_PORT_ARRAY[GPIO_PORTS] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};
static UINT32 EXTILines[16] = {EXTI_Line0,EXTI_Line1,EXTI_Line2,EXTI_Line3,EXTI_Line4,EXTI_Line5,EXTI_Line6,EXTI_Line7,EXTI_Line8,EXTI_Line9,EXTI_Line10,EXTI_Line11,EXTI_Line12,EXTI_Line13,EXTI_Line14,EXTI_Line15};
static GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr[GPIO_PINS]; 	// UGH, my stack =(
static void* gpio_parm[GPIO_PINS]; 							// UGH
static uint8_t pin_reservations[14]; // 14*8 = 112

static int GPIO_GetExtrCR(unsigned int line);
static GPIO_TypeDef* GPIO_GetPortPtr(GPIO_PIN Pin);
static unsigned int GPIO_GetPin(GPIO_PIN Pin);
static BOOL IsOutputPin(GPIO_PIN Pin);

//--//

// Core interrupt handler
static void handle_exti(unsigned int exti) {
	if(EXTI_GetITStatus(exti) != RESET) {
		EXTI_ClearITPendingBit(exti);

		GPIO_INTERRUPT_SERVICE_ROUTINE my_isr;
		UINT32 line = GPIO_GetExtrCR(exti);
		UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7; // Assuming this is right, not double-checked --NPS
		UINT32 pin = line + port * GPIO_PPP;
		void *parm;

		// Shouldn't this be a HAL_CONTINUATION??? TODO --NPS
		my_isr = gpio_isr[pin];
		parm = gpio_parm[pin];
		my_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)), parm);
	}
}

static int GPIO_GetExtrCR(unsigned int line)
{
	if(line > (1 << NUMBER_OF_EXTI_LINES)) { return -1; }

	for(int i = 0; i < NUMBER_OF_EXTI_LINES; i++)
	{
		if(line & (1 << i))
			return i;
	}

	return -1;
}

static unsigned int GPIO_GetPin(GPIO_PIN Pin) {
	return (0x1 << (Pin % GPIO_PPP)); // Hopefully this gets optimized to mask, somebody should check.
}

static unsigned int GPIO_GetPort(GPIO_PIN Pin) {
	return (Pin / GPIO_PPP);
}

static unsigned int GPIO_GetExtiLine(GPIO_PIN Pin){
	return EXTILines[Pin % GPIO_PPP];
}

static GPIO_TypeDef* GPIO_GetPortPtr(GPIO_PIN Pin) {
	return GPIO_PORT_ARRAY[GPIO_GetPort(Pin)];
}

static unsigned int GPIO_GetIRQNumber(GPIO_PIN Pin)
{
	unsigned int line = Pin % GPIO_PPP;
	unsigned int irq_number;

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

static BOOL IsOutputPin(GPIO_PIN Pin) {
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

static HAL_CALLBACK_FPN GPIO_GetCallBack(GPIO_PIN Pin)
{
	unsigned int line = Pin % GPIO_PPP;
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

// Initialize the ports GPIOA .... GPIOG of the Emote
BOOL CPU_GPIO_Initialize()
{
	// Configure clock source for all gpio ports
	// To conserve power this really should only be done as needed, TODO. --NPS
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
							RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);
	return TRUE;
}

// Note that this will kill ALL I/O peripherals (UART, SPI, etc), not just GPIO.
// Not changing for now but TODO. --NPS
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
	//AFIO
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
	return TRUE;
}

BOOL CPU_GPIO_TogglePinState(GPIO_PIN Pin)
{
	BOOL pinState = CPU_GPIO_GetPinState(Pin);

	if(!pinState)
	{
		CPU_GPIO_SetPinState(Pin, TRUE);
	}
	else
	{
		CPU_GPIO_SetPinState(Pin, FALSE);
	}
	return TRUE;
}

// Not sure what this is supposed to do. --NPS
UINT32 CPU_GPIO_Attributes( GPIO_PIN Pin )
{
	/*
	if(Pin < g_STM32F10x_Gpio_Driver.c_MaxPins)
	{
		return g_STM32F10x_Gpio_Driver.c_Gpio_Attributes[Pin];
	}
	*/
	return GPIO_ATTRIBUTE_NONE;
}

// Disable means set as input pin --NPS
void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned int p;

	if (Pin > GPIO_PINS) { return; }

	p = GPIO_GetPin(Pin);

	GPIO_InitStructure.GPIO_Pin = p;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIO_GetPortPtr(Pin), &GPIO_InitStructure);

	// Remove any user interrupts
	gpio_isr[Pin] = NULL;
	gpio_parm[Pin] = NULL;

	// TODO DISABLE INTERRUPT ITSELF --NPS
}

// Configure the pin as an output pin.
void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned int p;

	if (Pin > GPIO_PINS) { return; }

	p = GPIO_GetPin(Pin);

	GPIO_InitStructure.GPIO_Pin = p;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; // Default to 10 MHz. 50 MHz only supported >= 2.7v
	GPIO_Init(GPIO_GetPortPtr(Pin), &GPIO_InitStructure);

	CPU_GPIO_SetPinState(Pin, InitialState);
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	//return CPU_GPIO_EnableInputPin2(Pin, GlitchFilterEnable, PIN_ISR, NULL, IntEdge, ResistorState );
	return CPU_GPIO_EnableInputPin2(Pin, FALSE, PIN_ISR, NULL, IntEdge, ResistorState ); // We don't support GlitchFilter. --NPS
}

BOOL CPU_GPIO_EnableInputPin3( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	return CPU_GPIO_EnableInputPin2(Pin, FALSE, NULL, NULL, IntEdge, ResistorState);
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_TypeDef *port = GPIO_GetPortPtr(Pin);
	unsigned int p;

	if (Pin > GPIO_PINS) { return FALSE; }

	p = GPIO_GetPin(Pin);

	GPIO_InitStructure.GPIO_Pin = p;

	switch (ResistorState) {
		case RESISTOR_PULLDOWN:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
			break;
		case RESISTOR_PULLUP:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
			break;
		case RESISTOR_DISABLED:
		default:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			break;
	}

	GPIO_Init(port, &GPIO_InitStructure);

	if(PIN_ISR != NULL)
	{
		GPIO_EXTILineConfig(GPIO_GetPort(Pin), (Pin % GPIO_PPP));
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
	else {
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
	GPIO_TypeDef *port;
	unsigned int p;

	if (Pin > GPIO_PINS) { return FALSE; }

	port = GPIO_GetPortPtr(Pin);
	p = GPIO_GetPin(Pin);

	if (IsOutputPin(Pin)) {
		return (BOOL)GPIO_ReadOutputDataBit(port, p);
	}
	else {
		return (BOOL)GPIO_ReadInputDataBit(port, p);
	}
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
	if (Pin > GPIO_PINS) { return; }
	if (!IsOutputPin(Pin)) { return; }

	if(PinState) {
		GPIO_WriteBit(GPIO_GetPortPtr(Pin), GPIO_GetPin(Pin), Bit_SET);
	}else {
		GPIO_WriteBit(GPIO_GetPortPtr(Pin), GPIO_GetPin(Pin), Bit_RESET);
	}
}

// Check if the pin is busy or in use, takes the Pin number as the input argument
BOOL CPU_GPIO_PinIsBusy( GPIO_PIN Pin )
{
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
	for(int i=0; i < size; i++) {
		pins[i] = GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT; // ??? TODO FIX ME. --NPS
	}
	/*
    UINT8 ik;
	for(ik = 0; ik < size; ik++) {
		pins[ik] = (UINT8)CPU_GPIO_Attributes((GPIO_PIN)ik);
	}
	*/
}

UINT8 CPU_GPIO_GetSupportedResistorModes( GPIO_PIN Pin )
{
	return (1<<RESISTOR_DISABLED) | (1<<RESISTOR_PULLUP) | (1 << RESISTOR_PULLDOWN);

}
UINT8 CPU_GPIO_GetSupportedInterruptModes( GPIO_PIN pin )
{
	return (1<<GPIO_INT_EDGE_LOW) | (1<<GPIO_INT_EDGE_HIGH ) | (1<<GPIO_INT_EDGE_BOTH);
}

void EXTI0_IRQ_HANDLER(void *args)
{
	handle_exti(EXTI_Line0);
}

void EXTI1_IRQ_HANDLER(void *args)
{
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

/*
void STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param )
{

}

void Default_EXTI_Handler(void *data)
{

}
*/
