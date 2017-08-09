//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "SamrakshEmote.h"
#include "SamrakshEmote_Samraksh_eMote_RadarInterface.h"

using namespace Samraksh::eMote;

#ifndef PLATFORM_ARM_WLN // Code only active on WLN for now
INT8 RadarInterface::TurnOn( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 RadarInterface::TurnOff( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 RadarInterface::Blind( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

#else // #ifndef PLATFORM_ARM_WLN
#include <stm32f10x.h>
static bool isInit = false;

static void initRadarPins(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// PA11 --> TrigClockOn for Blinding
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET); // default ON
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA4 --> RF_PWM_EN for OFF
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET); // default ON (PWM mode)
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA1 --> ~MUX_EN for Blinding
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_WriteBit(GPIOA, GPIO_Pin_1,  Bit_RESET); // default ON (inverted logic)
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PB7 --> radar_ldo_en 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_WriteBit(GPIOB, GPIO_Pin_7,  Bit_SET); // default ON
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	isInit = true;
}

// Turn on the radar or un-blind
INT8 RadarInterface::TurnOn( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	if (!isInit) { initRadarPins(); } // return after init because on by default

	GPIO_WriteBit(GPIOA, GPIO_Pin_11,	Bit_SET); 	// TrigClockOn ENABLED
	GPIO_WriteBit(GPIOA, GPIO_Pin_4,	Bit_SET); 	// PWM mode ENABLED
	GPIO_WriteBit(GPIOA, GPIO_Pin_1,	Bit_RESET);	// MUX_EN ENABLED
	GPIO_WriteBit(GPIOB, GPIO_Pin_7,	Bit_SET); 	// radar_ldo_en ENABLED

	return 0;
}

// Disable radar and put its power supply into low power mode
INT8 RadarInterface::TurnOff( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	if (!isInit) { initRadarPins(); }

	GPIO_WriteBit(GPIOA, GPIO_Pin_11,	Bit_RESET); 	// TrigClockOn DISABLED
	GPIO_WriteBit(GPIOA, GPIO_Pin_4,	Bit_RESET); 	// PWM mode DISABLED
	GPIO_WriteBit(GPIOB, GPIO_Pin_7,	Bit_RESET); 	// radar_ldo_en DISABLED

	return 0;
}

// Disable radar but keep its power supply hot
INT8 RadarInterface::Blind( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	if (!isInit) { initRadarPins(); }

	GPIO_WriteBit(GPIOA, GPIO_Pin_1,	Bit_SET);		// MUX_EN DISABLED
	GPIO_WriteBit(GPIOA, GPIO_Pin_11,	Bit_RESET); 	// TrigClockOn DISABLED
	GPIO_WriteBit(GPIOA, GPIO_Pin_4,	Bit_SET); 		// PWM mode ENABLED

	return 0;
}
#endif // #ifndef PLATFORM_ARM_WLN
