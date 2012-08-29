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


//Mukundan
//#define EXTI_SCALER 5
//int Exti7_Count=0;

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
}

// Absence of a global gpio class forcing creation of a separate function to map callbacks
PIN_ISR_DESCRIPTOR gpinIsr[GPIO_PINS];

GPIO_InitTypeDef GPIO_Instances[GPIO_PINS];
uint16_t GPIO_Reserve[GPIO_PORTS];
GPIO_TypeDef* GPIO_PORT_ARRAY[GPIO_PORTS] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG};

UINT32 EXTILines[16] = {EXTI_Line0,EXTI_Line1,EXTI_Line2,EXTI_Line3,EXTI_Line4,EXTI_Line5,EXTI_Line6,EXTI_Line7,EXTI_Line8,EXTI_Line9,EXTI_Line10,EXTI_Line11,EXTI_Line12,EXTI_Line13,EXTI_Line14,EXTI_Line15};
//Local Functions

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

BOOL CPU_GPIO_Initialize()
{
	UINT8 ik;
	for(ik = 0; ik < GPIO_PINS; ik++) {
		GPIO_StructInit(&GPIO_Instances[ik]);
	}
	for(ik = 0; ik < GPIO_PORTS; ik++) {
		GPIO_Reserve[ik] = 0x00000000U;
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

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
	/*if		(IS_GPIO_MODE_IN(GPIO_Instances[Pin].GPIO_Mode)) {
		return GPIO_ATTRIBUTE_INPUT;
	}else if(IS_GPIO_MODE_OUT(GPIO_Instances[Pin].GPIO_Mode)) {
		return GPIO_ATTRIBUTE_OUTPUT;
	}else {
		return GPIO_ATTRIBUTE_NONE;
	}*/
	
	return GPIO_ATTRIBUTE_OUTPUT;
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
	return CPU_GPIO_EnableInputPin2(Pin, GlitchFilterEnable, PIN_ISR, NULL, IntEdge, ResistorState );
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

	if(PIN_ISR)
	{
		PIN_ISR_DESCRIPTOR& pinIsr = gpinIsr[Pin];

		pinIsr.m_isr     = PIN_ISR;
		pinIsr.m_param   = ISR_Param;

		CPU_INTC_ActivateInterrupt(GPIO_GetIRQNumber(Pin), (HAL_CALLBACK_FPN) GPIO_GetCallBack(Pin), NULL);
	}

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

void EXTI1_IRQ_HANDLER(void *args)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
		PIN_ISR_DESCRIPTOR& pinIsr = gpinIsr[17];

		if(pinIsr.m_isr)
			pinIsr.m_isr(17, TRUE,  args);
		else
			Default_EXTI_Handler(args);
	}

}

extern "C"
{

void EXTI0_IRQ_HANDLER(void *args)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
	  EXTI_ClearITPendingBit(EXTI_Line0);
	  //Handles only PA0
	  PIN_ISR_DESCRIPTOR& pinIsr = gpinIsr[0];
	  if(pinIsr.m_isr)
		  pinIsr.m_isr(0, TRUE,  args);
	  else
		  Default_EXTI_Handler(args);
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
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{

		EXTI_ClearITPendingBit(EXTI_Line2);
	}

}

void EXTI3_IRQ_HANDLER(void *args)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{

		EXTI_ClearITPendingBit(EXTI_Line3);
	}


}

void EXTI4_IRQ_HANDLER(void *args)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		//Handles only PA4
		PIN_ISR_DESCRIPTOR& pinIsr = gpinIsr[4];
		if(pinIsr.m_isr)
			pinIsr.m_isr(4, TRUE,  args);
		else
			Default_EXTI_Handler(args);
	}


}

void EXTI9_5_IRQ_HANDLER(void *args)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	else if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
		//Handles only PC6
		PIN_ISR_DESCRIPTOR& pinIsr = gpinIsr[38];
		if(pinIsr.m_isr){
			BOOL state=CPU_GPIO_GetPinState(38);
			pinIsr.m_isr(38, state , pinIsr.m_param);
		}
		else{
			Default_EXTI_Handler(args);
		}
	}
	else if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line7);

		//Handles only PB7
		//if( Exti7_Count == EXTI_SCALER){
			PIN_ISR_DESCRIPTOR& pinIsr = gpinIsr[23];
			if(pinIsr.m_isr){
				BOOL state=CPU_GPIO_GetPinState(23);
				pinIsr.m_isr(23, state, pinIsr.m_param);
			}
			else{
				Default_EXTI_Handler(args);
			}
			//Exti7_Count=-1;
		//}
		//Exti7_Count++;
	}
	else if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	else if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
	}

}

void EXTI15_10_IRQ_Handler(void *args)
{
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line10);
		}
		else if(EXTI_GetITStatus(EXTI_Line11) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line11);
		}
		else if(EXTI_GetITStatus(EXTI_Line12) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line12);
		}
		else if(EXTI_GetITStatus(EXTI_Line13) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line13);
		}
		else if(EXTI_GetITStatus(EXTI_Line14) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line14);
		}
		else if(EXTI_GetITStatus(EXTI_Line15) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line15);
		}


}
}
