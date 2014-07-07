#ifndef _NETMF_I2C_H_
#define _NETMF_I2C_H_

#include <tinyhal.h>
#include <Samraksh/Hal_util.h>
#include <stm32f10x.h>
#include "../intc/STM32.h"

#define I2C1_SDA 23
#define I2C1_SCL 22

#define I2C2_SDA 26
#define I2C2_SCL 27

#define I2C_DEBUG 1

#define I2C_BUSES 2



#if defined(I2C_DEBUG)
#define I2C_DEBUG_PRINT(x) hal_printf(x)
#else
#define I2C_DEBUG_PRINT(x)
#endif

// Enum defining the number of buses supported by the stm32f10x processor
enum I2CBus
{
	I2CBus1,
	I2CBus2,
};


class STM32F10x_I2C_Driver
{
	// Stores the clock address
	static UINT32 I2CClock[2];

	// Stores the pin configurations for the two busses
	static UINT32 GpioPinsBus1[2];
	static UINT32 GpioPinsBus2[2];



	I2C_HAL_XACTION* currentI2CXAction;
	I2C_HAL_XACTION_UNIT* currentI2CUnit;


public:

	static BOOL IsInitialized;

	UINT32 currentActiveBus;

	DeviceStatus Initialize(I2CBus bus);

	DeviceStatus UnInitialize();

	I2C_HAL_XACTION* getCurrentXAction()
	{
		return currentI2CXAction;
	}

	void setCurrentXAction(I2C_HAL_XACTION* currentI2CXAction)
	{
		this->currentI2CXAction = currentI2CXAction;
	}

	I2C_HAL_XACTION_UNIT* getCurrentXActionUnit()
	{
		return currentI2CUnit;
	}

	void setCurrentXActionUnit(I2C_HAL_XACTION_UNIT* currentI2CUnit)
	{
		this->currentI2CUnit = currentI2CUnit;
	}

	DeviceStatus XActionStart(I2C_HAL_XACTION* xAction, bool repeatedStart);

	DeviceStatus XActionStop();

	//void I2C_Event_Handler(void *param);

	//void I2C_Error_Handler(void *param);

};


UINT32 STM32F10x_I2C_Driver::I2CClock[2] = {RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2};

UINT32 STM32F10x_I2C_Driver::GpioPinsBus1[2] = {I2C1_SCL,I2C1_SDA};
UINT32 STM32F10x_I2C_Driver::GpioPinsBus2[2] = {I2C2_SCL,I2C2_SDA};

BOOL STM32F10x_I2C_Driver::IsInitialized = FALSE;


#endif
