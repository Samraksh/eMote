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


#include "KiwiSensorBoard.h"
#include "KiwiSensorBoard_Samraksh_eMote_SensorBoard_Buzzer.h"

#include "stm32f10x.h"
#include "pwr/netmf_pwr_wakelock.h"

using namespace Samraksh::eMote::SensorBoard;

static void Buzzer_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void Buzzer_GPIO_Deinit(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static unsigned get_APB1_clock() {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	return RCC_Clocks.PCLK1_Frequency;
}

// static unsigned get_APB2_clock() {
	// RCC_ClocksTypeDef RCC_Clocks;
	// RCC_GetClocksFreq(&RCC_Clocks);
	// return RCC_Clocks.PCLK2_Frequency;
// }

// Lets use PA1 which is TIM5_CH2

INT8 Buzzer::On( HRESULT &hr )
{
	uint16_t PrescalerValue;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	unsigned SystemClock = get_APB1_clock();
	
	// If the divider is 2 or more, TIM units have double frequency
	// Only high power uses Div2
	if (SystemClock > 24000000)
		SystemClock = SystemClock * 2;

	/* System Clocks Configuration */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	/* GPIO Configuration */
	Buzzer_GPIO_Configuration();

	/* -----------------------------------------------------------------------
	TIM1 Configuration: generate 4 PWM signals with 4 different duty cycles:
	The TIM1CLK frequency is set to SystemCoreClock (Hz), to get TIM1 counter
	clock at 24 MHz the Prescaler is computed as following:
	 - Prescaler = (TIM1CLK / TIM1 counter clock) - 1
	SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
	and Connectivity line devices and to 24 MHz for Low-Density Value line and
	Medium-Density Value line devices

	The TIM1 is running at 36 KHz: TIM1 Frequency = TIM1 counter clock/(ARR + 1)
												  = 24 MHz / 666 = 36 KHz
	TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 = 50%
	TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR)* 100 = 37.5%
	TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR)* 100 = 25%
	TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR)* 100 = 12.5%
	----------------------------------------------------------------------- */

	TIM_DeInit(TIM5);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemClock / 8000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 2000-1; // 50% duty cycle @ 4 kHz
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1000-1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	//TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	//TIM_ARRPreloadConfig(TIM5, ENABLE);

	/* TIM1 enable counter */
	TIM_Cmd(TIM5, ENABLE);
	TIM_CtrlPWMOutputs(TIM5, ENABLE);

	WakeLock(KIWI_WAKELOCK);

    return 1;
}

INT8 Buzzer::Off( HRESULT &hr )
{
	TIM_Cmd(TIM5, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE);
	Buzzer_GPIO_Deinit();
	WakeUnlock(KIWI_WAKELOCK);
    return 1;
}

