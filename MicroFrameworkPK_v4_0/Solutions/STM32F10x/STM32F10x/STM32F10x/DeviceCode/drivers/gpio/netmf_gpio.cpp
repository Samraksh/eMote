////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "stm32f10x_gpio.h"
#include <rcc/stm32f10x_rcc.h>

//--//

#define GPIO_PORTS	7
#define	GPIO_PPP	16
#define GPIO_PINS	(GPIO_PORTS * GPIO_PPP)	//112

GPIO_InitTypeDef GPIO_Instances[GPIO_PINS];
uint16_t GPIO_Reserve[GPIO_PORTS];
GPIO_TypeDef* GPIO_PORT_ARRAY[GPIO_PORTS] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};

//Local Functions

uint16_t GPIO_GetPin(GPIO_PIN Pin) {
	return (uint16_t)(0x1 << (Pin % GPIO_PPP));
}

uint8_t GPIO_GetPort(GPIO_PIN Pin) {
	return (uint8_t)(Pin / GPIO_PPP);
}

GPIO_TypeDef* GPIO_GetPortPtr(GPIO_PIN Pin) {
	return GPIO_PORT_ARRAY[GPIO_GetPort(Pin)];
}

//Exported Functions

BOOL CPU_GPIO_Initialize()
{
	UINT8 ik;
	for(ik = 0; ik < GPIO_PINS; ik++) {
		GPIO_StructInit(&GPIO_Instances[ik]);
	}
	for(ik = 0; ik < GPIO_PORTS; ik++) {
		GPIO_Reserve[ik] = 0x00000000U;
	}
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

UINT32 CPU_GPIO_Attributes( GPIO_PIN Pin )
{
/*	if		(IS_GPIO_MODE_IN(GPIO_Instances[Pin].GPIO_Mode)) {
		return GPIO_ATTRIBUTE_INPUT;
	}else if(IS_GPIO_MODE_OUT(GPIO_Instances[Pin].GPIO_Mode)) {
		return GPIO_ATTRIBUTE_OUTPUT;
	}else {
		return GPIO_ATTRIBUTE_NONE;
	}
*/
}

void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
	GPIO_StructInit(&GPIO_Instances[Pin]);
	GPIO_Instances[Pin].GPIO_Pin = GPIO_GetPin(Pin);
	GPIO_Init(GPIO_GetPortPtr(Pin), &GPIO_Instances[Pin]);
}

void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
	GPIO_Instances[Pin].GPIO_Pin = GPIO_GetPin(Pin);
	GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_GetPortPtr(Pin), &GPIO_Instances[Pin]);
	CPU_GPIO_SetPinState(Pin, InitialState);
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    GPIO_Instances[Pin].GPIO_Pin = GPIO_GetPin(Pin);
	if		(ResistorState == RESISTOR_DISABLED) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}else if(ResistorState == RESISTOR_PULLDOWN) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IPD;
	}else if(ResistorState == RESISTOR_PULLUP) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IPU;
	}
	GPIO_Init(GPIO_GetPortPtr(Pin), &GPIO_Instances[Pin]);
	return TRUE;
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    GPIO_Instances[Pin].GPIO_Pin = GPIO_GetPin(Pin);
	if		(ResistorState == RESISTOR_DISABLED) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}else if(ResistorState == RESISTOR_PULLDOWN) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IPD;
	}else if(ResistorState == RESISTOR_PULLUP) {
		GPIO_Instances[Pin].GPIO_Mode = GPIO_Mode_IPU;
	}
	GPIO_Init(GPIO_GetPortPtr(Pin), &GPIO_Instances[Pin]);
	return TRUE;
}

BOOL CPU_GPIO_GetPinState( GPIO_PIN Pin )
{
	if		(CPU_GPIO_Attributes(Pin) == GPIO_ATTRIBUTE_INPUT) {
		return (BOOL)GPIO_ReadInputDataBit(GPIO_GetPortPtr(Pin), GPIO_GetPin(Pin));
	}else if(CPU_GPIO_Attributes(Pin) == GPIO_ATTRIBUTE_OUTPUT) {
		return (BOOL)GPIO_ReadOutputDataBit(GPIO_GetPortPtr(Pin), GPIO_GetPin(Pin));
	}
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
	if(PinState) {
		GPIO_WriteBit(GPIO_GetPortPtr(Pin), GPIO_GetPin(Pin), Bit_SET);
	}else {
		GPIO_WriteBit(GPIO_GetPortPtr(Pin), GPIO_GetPin(Pin), Bit_RESET);
	}
}

BOOL CPU_GPIO_PinIsBusy( GPIO_PIN Pin )
{
    return (BOOL)READ_BIT(GPIO_Reserve[GPIO_GetPort(Pin)], (0x1 << GPIO_GetPin(Pin)));
}

BOOL CPU_GPIO_ReservePin( GPIO_PIN Pin, BOOL fReserve )
{
    if(fReserve) {
		SET_BIT(GPIO_Reserve[GPIO_GetPort(Pin)], (0x1 << GPIO_GetPin(Pin)));
	}else {
		CLEAR_BIT(GPIO_Reserve[GPIO_GetPort(Pin)], (0x1 << GPIO_GetPin(Pin)));
	}
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
    UINT8 ik;
	for(ik = 0; ik < size; ik++) {
		pins[ik] = (UINT8)CPU_GPIO_Attributes((GPIO_PIN)ik);
	}
}

UINT8 CPU_GPIO_GetSupportedResistorModes( GPIO_PIN Pin )
{
	switch(GPIO_Instances[Pin].GPIO_Mode) {
		case(GPIO_Mode_IPD):
			return RESISTOR_PULLDOWN;
			break;
		case(GPIO_Mode_IPU):
			return RESISTOR_PULLUP;
			break;
		default:
			return RESISTOR_DISABLED;
			break;
	}
}
UINT8 CPU_GPIO_GetSupportedInterruptModes( GPIO_PIN pin )
{
    // as it is stub, return 0;
    return 0;
}

