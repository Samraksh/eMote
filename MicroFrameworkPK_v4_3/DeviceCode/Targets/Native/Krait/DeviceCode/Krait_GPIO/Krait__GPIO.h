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


//AnanthAtSamraksh

#define MSM_GPIO_52					((UINT16)0x0001)  /*!< Pin 0 selected */
#define MSM_GPIO_53					((UINT16)0x0002)  /*!< Pin 0 selected */
#define MSM_GPIO_55					((UINT16)0x0004)  /*!< Pin 0 selected */
#define MSM_GPIO_58					((UINT16)0x0008)  /*!< Pin 0 selected */

#define GPIO_Pin_0                 ((UINT16)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((UINT16)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((UINT16)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((UINT16)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((UINT16)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((UINT16)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((UINT16)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((UINT16)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((UINT16)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((UINT16)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((UINT16)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((UINT16)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((UINT16)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((UINT16)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((UINT16)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((UINT16)0x8000)  /*!< Pin 15 selected */
#define GPIO_Pin_All               ((UINT16)0xFFFF)  /*!< All pins selected */


/**
  * @brief  Output Maximum frequency selection
  */

typedef enum
{
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz,
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Speed_10MHz) || ((SPEED) == GPIO_Speed_2MHz) || \
                              ((SPEED) == GPIO_Speed_50MHz))

/**
  * @brief  Configuration Mode enumeration
  */

typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;

/**
  * @brief  GPIO Init structure definition
  */

typedef struct
{
  UINT16 GPIO_Pin;             /*!< Specifies the GPIO pins to be configured.
                                      This parameter can be any value of @ref GPIO_pins_define */

  GPIOSpeed_TypeDef GPIO_Speed;  /*!< Specifies the speed for the selected pins.
                                      This parameter can be a value of @ref GPIOSpeed_TypeDef */

  GPIOMode_TypeDef GPIO_Mode;    /*!< Specifies the operating mode for the selected pins.
                                      This parameter can be a value of @ref GPIOMode_TypeDef */
}GPIO_InitTypeDef;

//AnanthAtSamraksh


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
