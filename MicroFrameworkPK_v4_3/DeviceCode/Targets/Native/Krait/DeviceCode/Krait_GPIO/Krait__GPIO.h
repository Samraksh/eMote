/*
 * Krait__GPIO.h
 *
 *  Created on: Sep 17, 2012
 *      Author: Mukundan Sridharan
 */

#ifndef KRAIT_GPIO_H
#define KRAIT_GPIO_H

#include "../Krait.h"

////Below taken from platform/msm_shared/include/gsbi.h
/* GSBI Registers */
#define GSBI_CTRL_REG(id)        (GSBI_BASE(id) + 0x0)

#define GSBI_CTRL_REG_PROTOCOL_CODE_S   4
#define GSBI_PROTOCOL_CODE_I2C          0x2
#define GSBI_PROTOCOL_CODE_SPI          0x3
#define GSBI_PROTOCOL_CODE_UART_FLOW    0x4
#define GSBI_PROTOCOL_CODE_I2C_UART     0x6

#define GSBI_HCLK_CTL_S                 4
#define GSBI_HCLK_CTL_CLK_ENA           0x1

enum {
	GSBI_ID_1 = 1,
	GSBI_ID_2,
	GSBI_ID_3,
	GSBI_ID_4,
	GSBI_ID_5,
	GSBI_ID_6,
	GSBI_ID_7,
	GSBI_ID_8,
	GSBI_ID_9,
	GSBI_ID_10,
	GSBI_ID_11,
	GSBI_ID_12,
};

////Below taken from platfrom/msm8690/include/platform/gpio.h

// GPIO TLMM: Direction
#define GPIO_INPUT      0
#define GPIO_OUTPUT     1

// GPIO TLMM: Pullup/Pulldown
#define GPIO_NO_PULL    0
#define GPIO_PULL_DOWN  1
#define GPIO_KEEPER     2
#define GPIO_PULL_UP    3

// GPIO TLMM: Drive Strength
#define GPIO_2MA        0
#define GPIO_4MA        1
#define GPIO_6MA        2
#define GPIO_8MA        3
#define GPIO_10MA       4
#define GPIO_12MA       5
#define GPIO_14MA       6
#define GPIO_16MA       7

// GPIO TLMM: Status
#define GPIO_ENABLE     1
#define GPIO_DISABLE    0


struct Krait_GPIO_Driver
{
private:

public:
	static BOOL Initialize();
	static BOOL EnableInputPin(GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState );
	static void EnableOutputPin(GPIO_PIN pin, BOOL initialState);
	static UINT32 Attributes( GPIO_PIN Pin );
	static BOOL ReservePin( GPIO_PIN pin, BOOL fReserve );
	static void SetPinState(GPIO_PIN pin, BOOL pinState);
	static BOOL GetPinState(GPIO_PIN pin);
	static void DisablePin(GPIO_PIN pin, GPIO_RESISTOR resistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction);
};

#endif
