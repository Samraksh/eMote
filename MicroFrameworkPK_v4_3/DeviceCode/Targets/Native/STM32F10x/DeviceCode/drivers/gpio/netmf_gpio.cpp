////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "stm32f10x_gpio.h"
#include <rcc/stm32f10x_rcc.h>
#include <exti/stm32f10x_exti.h>
#include <intc/STM32.h>
#include "netmf_gpio.h"

//--//

#define GPIO_PORTS	7
#define	GPIO_PPP	16
#define GPIO_PINS	(GPIO_PORTS * GPIO_PPP)	//112

BOOL pinState = FALSE;


extern "C"
{
void Default_EXTI_Handler(void *data);
void EXTI0_IRQ_HANDLER(void *args);
void EXTI1_IRQ_HANDLER(void *args);
void EXTI2_IRQ_HANDLER(void *args);
void EXTI3_IRQ_HANDLER(void *args);
void EXTI4_IRQ_HANDLER(void *args);
void EXTI9_5_IRQ_HANDLER(void *args);
void EXTI15_10_IRQ_Handler(void *args);
void STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param );
}

STM32F10x_GPIO_Driver g_STM32F10x_Gpio_Driver;

const UINT8 STM32F10x_GPIO_Driver::c_Gpio_Attributes[STM32F10x_GPIO_Driver::c_MaxPins] =
{
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   0
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   1
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   2
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   3
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   4
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   5
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   6
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   7
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   8
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   9
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  10
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  11
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  12
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  13
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  14
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  15
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  16
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  17
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  18
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  19
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  20
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  21
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  22
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  23
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  24
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  25
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  26
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  27
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  28
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  29
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  30
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  31
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  32
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  33
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  34
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  35
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  36
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  37
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  38
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  39
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  40
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  41
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  42
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  43
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  44
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  45
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  46
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  47
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  48
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  49
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  50
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  51
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  52
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  53
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  54
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  55
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  56
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  57
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  58
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  59
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  60
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  61
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  62
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  63
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  64
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  65
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  66
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  67
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  68
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  69
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  70
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  71
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  72
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  73
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  74
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  75
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  76
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  77
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  78
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  79
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  80
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  81
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  82
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  83
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  84
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  85
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  86
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  87
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  88
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  89
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  90
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  91
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  92
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  93
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  94
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  95
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  96
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  97
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  98
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  99
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 100
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 101
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 102
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 103
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 104
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 105
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 106
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 107
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 108
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 109
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 110
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 111


};


GPIO_InitTypeDef GPIO_Instances[GPIO_PINS];

GPIO_TypeDef* GPIO_PORT_ARRAY[GPIO_PORTS] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};

#define NUMBER_OF_EXTI_LINES 16

UINT32 EXTILines[16] = {EXTI_Line0,EXTI_Line1,EXTI_Line2,EXTI_Line3,EXTI_Line4,EXTI_Line5,EXTI_Line6,EXTI_Line7,EXTI_Line8,EXTI_Line9,EXTI_Line10,EXTI_Line11,EXTI_Line12,EXTI_Line13,EXTI_Line14,EXTI_Line15};
//Local Functions

int16_t GPIO_GetExtrCR(UINT32 line)
{
	if(line > (1 << NUMBER_OF_EXTI_LINES))
		return -1;

	for(UINT16 i = 0; i < NUMBER_OF_EXTI_LINES; i++)
	{
		if(line & (1 << i))
			return i;
	}

	return -1;

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

UINT32 GPIO_GetIRQNumber(GPIO_PIN Pin)
{
	UINT32 line = Pin % GPIO_PPP;
	UINT32 irq_number;

	switch(line)
	{
	case	0:
		irq_number = STM32_AITC::c_IRQ_INDEX_EXTI0;
		break;
	case 	1:
		irq_number = STM32_AITC::c_IRQ_INDEX_EXTI1;
		break;
	case 	2:
		irq_number = STM32_AITC::c_IRQ_INDEX_EXTI2;
		break;
	case    3:
		irq_number = STM32_AITC::c_IRQ_INDEX_EXTI3;
		break;
	case    4:
		irq_number = STM32_AITC::c_IRQ_INDEX_EXTI4;
		break;
	case	5:
	case	6:
	case    7:
	case    8:
	case    9:
		irq_number = STM32_AITC::c_IRQ_INDEX_EXTI9_5;
		break;
	case   10:
	case   11:
	case   12:
	case   13:
	case   14:
	case   15:
		irq_number = STM32_AITC::c_IRQ_INDEX_EXTI15_10;
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

	UINT16 port;
	UINT16 pin;
	STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR* pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr;
	// Initialize all the pins in all the ports to the following configuration
	// - Input Floating
	// - 2 MHZ (consumes the least power)
	int ik = 0;

	for(port = 0; port < g_STM32F10x_Gpio_Driver.c_MaxPorts; port++)
	{
		g_STM32F10x_Gpio_Driver.m_pinReservationInfo[port] = 0;
	}

	// Initialize the isr structure for each of the pins
	for(pin = 0; pin < g_STM32F10x_Gpio_Driver.c_MaxPins; pin++)
	{
		GPIO_StructInit(&GPIO_Instances[pin]);
		pinIsr->m_pin     = pin;
		pinIsr->m_intEdge = GPIO_INT_NONE;
		pinIsr->m_isr     = STUB_GPIOISRVector;
		pinIsr->m_param   = NULL;

		pinIsr++;

		CPU_GPIO_EnableOutputPin((GPIO_PIN) pin, FALSE);
	}


	// Configure clock source for all gpio ports
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);

	return TRUE;

#if 0
	UINT8 ik;

	for(ik = 0; ik < GPIO_PINS; ik++) {
		GPIO_StructInit(&GPIO_Instances[ik]);
	}

	for(ik = 0; ik < GPIO_PORTS; ik++) {
		GPIO_Reserve[ik] = 0x00000000U;
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	return TRUE;
#endif

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
	if(pinState == FALSE)
	{
		CPU_GPIO_SetPinState(Pin, TRUE);
		pinState = TRUE;
	}
	else
	{
		CPU_GPIO_SetPinState(Pin, FALSE);
		pinState = FALSE;
	}
}

UINT32 CPU_GPIO_Attributes( GPIO_PIN Pin )
{
	if(Pin < g_STM32F10x_Gpio_Driver.c_MaxPins)
	{
		return g_STM32F10x_Gpio_Driver.c_Gpio_Attributes[Pin];
	}
	
	return GPIO_ATTRIBUTE_NONE;
}

// Debugging the i2c driver, this function is not doing what it is supposed to be doing
void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinRemapConfig(GPIO_Remap_FSMC_NADV, ENABLE);

	//GPIO_InitStructure.GPIO_Pin = GPIO_GetPin(Pin);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;

	//GPIO_Init(GPIO_GetPortPtr(Pin), &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

// Configure the pin as an output pin, strange that this does not have a return type
void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
	// Check if the input pin is less than max pins
	if(Pin > g_STM32F10x_Gpio_Driver.c_MaxPins)
	{
		GPIO_DEBUG_PRINT2("[Native] [GPIO Driver] Pin Number greater than max allowable pins at %s, %s \n", __LINE__, __FILE__);
		return;
	}

	GPIO_Instances[Pin].GPIO_Pin = GPIO_GetPin(Pin);
	GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_GetPortPtr(Pin), &GPIO_Instances[Pin]);
	CPU_GPIO_SetPinState(Pin, InitialState);
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	return CPU_GPIO_EnableInputPin2(Pin, GlitchFilterEnable, PIN_ISR, NULL, IntEdge, ResistorState );
}

BOOL CPU_GPIO_EnableInputPin3( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{

	if(Pin > g_STM32F10x_Gpio_Driver.c_MaxPins)
	{
		GPIO_DEBUG_PRINT2("[Native] [GPIO Driver] Pin Number greater than max allowable pins at %s, %s \n", __LINE__, __FILE__);
		return FALSE;
	}

	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_Instances[Pin].GPIO_Pin = GPIO_GetPin(Pin);
	if		(ResistorState == RESISTOR_DISABLED) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}else if(ResistorState == RESISTOR_PULLDOWN) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IPD;
	}else if(ResistorState == RESISTOR_PULLUP) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IPU;
	}
	else
	{
		GPIO_DEBUG_PRINT2("[Native] [GPIO Driver] Invalid resistor configuration at %s, %s \n", __LINE__, __FILE__);
		return FALSE;
	}

	GPIO_Init(GPIO_GetPortPtr(Pin), &GPIO_Instances[Pin]);
	
	return TRUE;

}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{

	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_Instances[Pin].GPIO_Pin = GPIO_GetPin(Pin);
	if		(ResistorState == RESISTOR_DISABLED) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}else if(ResistorState == RESISTOR_PULLDOWN) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IPD;
	}else if(ResistorState == RESISTOR_PULLUP) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IPU;
	}
	GPIO_Init(GPIO_GetPortPtr(Pin), &GPIO_Instances[Pin]);

	// Nived.Sivadas - adding interrupt support to the gpio pins
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


		STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[Pin];

		pinIsr.m_isr     = PIN_ISR;
		pinIsr.m_param   = ISR_Param;
		pinIsr.m_intEdge = IntEdge;

		CPU_INTC_ActivateInterrupt(GPIO_GetIRQNumber(Pin), (HAL_CALLBACK_FPN) GPIO_GetCallBack(Pin), NULL);
	}

	return TRUE;
}

// Return the state of the pin
// In case the pin is configured as one of the input modes, read the input data register.
// If the pin is configured in one of the output modes, read the output data register
BOOL CPU_GPIO_GetPinState( GPIO_PIN Pin )
{
	if((GPIO_Instances[Pin].GPIO_Mode == GPIO_Mode_AIN) || (GPIO_Instances[Pin].GPIO_Mode == GPIO_Mode_IN_FLOATING) || (GPIO_Instances[Pin].GPIO_Mode == GPIO_Mode_IPD) || (GPIO_Instances[Pin].GPIO_Mode == GPIO_Mode_IPU))
		return (BOOL)GPIO_ReadInputDataBit(GPIO_GetPortPtr(Pin), GPIO_GetPin(Pin));
	else
		return (BOOL)GPIO_ReadOutputDataBit(GPIO_GetPortPtr(Pin), GPIO_GetPin(Pin));
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{

	if(PinState) {
		GPIO_WriteBit(GPIO_GetPortPtr(Pin), GPIO_GetPin(Pin), Bit_SET);
	}else {
		GPIO_WriteBit(GPIO_GetPortPtr(Pin), GPIO_GetPin(Pin), Bit_RESET);
	}
}

// Check if the pin is busy or in use, takes the Pin number as the input argument
BOOL CPU_GPIO_PinIsBusy( GPIO_PIN Pin )
{
    return  ((g_STM32F10x_Gpio_Driver.m_pinReservationInfo[GPIO_GetPort(Pin)] & GPIO_GetPin(Pin)) > 0 ? TRUE : FALSE);
}

// This function is used to reserve and unreserve pins to avoid gpio resource conflict issues
BOOL CPU_GPIO_ReservePin( GPIO_PIN Pin, BOOL fReserve )
{
	// Check if the pin you are about to configure is busy or already in use, if so return false
	if(CPU_GPIO_PinIsBusy(Pin) && fReserve)
	{
		GPIO_DEBUG_PRINT1("[Native] [GPIO Driver] The Pin is busy\n");
		return FALSE;
	}

    if(fReserve) {
		SET_BIT(g_STM32F10x_Gpio_Driver.m_pinReservationInfo[GPIO_GetPort(Pin)], GPIO_GetPin(Pin));
	}else {
		CLEAR_BIT(g_STM32F10x_Gpio_Driver.m_pinReservationInfo[GPIO_GetPort(Pin)], GPIO_GetPin(Pin));
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
    return g_STM32F10x_Gpio_Driver.c_MaxPins;
} 

void CPU_GPIO_GetPinsMap( UINT8* pins, size_t size )
{
    UINT8 ik;
	for(ik = 0; ik < size; ik++) {
		pins[ik] = (UINT8)CPU_GPIO_Attributes((GPIO_PIN)ik);
	}
}

UINT8 CPU_GPIO_GetSupportedResistorModes( GPIO_PIN Pin )
{
	return g_STM32F10x_Gpio_Driver.c_GPIO_ResistorMode;

}
UINT8 CPU_GPIO_GetSupportedInterruptModes( GPIO_PIN pin )
{
    return g_STM32F10x_Gpio_Driver.c_GPIO_InterruptMode;
}

void EXTI1_IRQ_HANDLER(void *args)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);

		UINT32 line = GPIO_GetExtrCR(EXTI_Line1);

		UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

		UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

		STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

		if(pinIsr.m_isr)
			pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);
	}

}

extern "C"
{

void EXTI0_IRQ_HANDLER(void *args)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
	  EXTI_ClearITPendingBit(EXTI_Line0);

		UINT32 line = GPIO_GetExtrCR(EXTI_Line0);

			UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

			UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

			STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

			if(pinIsr.m_isr)
				pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);

  }
}



extern "C"
{
void Default_EXTI_Handler(void *data)
{

}
}

void EXTI2_IRQ_HANDLER(void *args)
{
	int line = 0;

	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{

		EXTI_ClearITPendingBit(EXTI_Line2);

		UINT32 line = GPIO_GetExtrCR(EXTI_Line2);

			UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

			UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

			STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

			if(pinIsr.m_isr)
				pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);

	}

}

void EXTI3_IRQ_HANDLER(void *args)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{

		EXTI_ClearITPendingBit(EXTI_Line3);

		UINT32 line = GPIO_GetExtrCR(EXTI_Line3);

			UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

			UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

			STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

			if(pinIsr.m_isr)
				pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);
	}
}

void EXTI4_IRQ_HANDLER(void *args)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		//Handles only PA4
		UINT32 line = GPIO_GetExtrCR(EXTI_Line4);

		UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

		UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

		STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

		if(pinIsr.m_isr)
			pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);
	}


}

void EXTI9_5_IRQ_HANDLER(void *args)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);

		UINT32 line = GPIO_GetExtrCR(EXTI_Line5);

		UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

		UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

		STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

		if(pinIsr.m_isr)
			pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);

	}
	else if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
		//Handles only PC6
		UINT32 line = GPIO_GetExtrCR(EXTI_Line6);

		UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

		UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

		STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

		if(pinIsr.m_isr)
			pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);
	}
	else if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line7);

		UINT32 line = GPIO_GetExtrCR(EXTI_Line7);

		UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

		UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

		STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

		if(pinIsr.m_isr)
			pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);
	}
	else if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);

		UINT32 line = GPIO_GetExtrCR(EXTI_Line8);

		UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

		UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

		STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

		if(pinIsr.m_isr)
			pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);

	}
	else if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);

		UINT32 line = GPIO_GetExtrCR(EXTI_Line9);

		UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

		UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

		STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

		if(pinIsr.m_isr)
			pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);

	}

}

void EXTI15_10_IRQ_Handler(void *args)
{
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line10);

			UINT32 line = GPIO_GetExtrCR(EXTI_Line10);

					UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

					UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

					STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

					if(pinIsr.m_isr)
						pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);

		}
		else if(EXTI_GetITStatus(EXTI_Line11) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line11);

			UINT32 line = GPIO_GetExtrCR(EXTI_Line11);

					UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

					UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

					STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

					if(pinIsr.m_isr)
						pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);
		}
		else if(EXTI_GetITStatus(EXTI_Line12) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line12);

			UINT32 line = GPIO_GetExtrCR(EXTI_Line12);

					UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

					UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

					STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

					if(pinIsr.m_isr)
						pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);

		}
		else if(EXTI_GetITStatus(EXTI_Line13) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line13);

			UINT32 line = GPIO_GetExtrCR(EXTI_Line13);

					UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

					UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

					STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

					if(pinIsr.m_isr)
						pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);

		}
		else if(EXTI_GetITStatus(EXTI_Line14) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line14);

			UINT32 line = GPIO_GetExtrCR(EXTI_Line14);

					UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

					UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

					STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

					if(pinIsr.m_isr)
						pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);

		}
		else if(EXTI_GetITStatus(EXTI_Line15) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line15);

			UINT32 line = GPIO_GetExtrCR(EXTI_Line15);

					UINT32 port = ((AFIO->EXTICR[line >> 2]) >> ( 4 * (line % 4))) & 0x7;

					UINT32 pin = line + port * g_STM32F10x_Gpio_Driver.c_PinsPerPort;

					STM32F10x_GPIO_Driver::PIN_ISR_DESCRIPTOR& pinIsr = g_STM32F10x_Gpio_Driver.m_pinIsr[pin];

					if(pinIsr.m_isr)
						pinIsr.m_isr(pin, GPIO_ReadInputDataBit(GPIO_GetPortPtr(pin),GPIO_GetPin(pin)),  pinIsr.m_param);
		}


}
}

void STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param )
{

}


