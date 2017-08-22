/*
 * 	Name 	:   netmf_i2c.cpp
 *
 *  Author  :   nived.sivadas@samraksh.com
 *
 *  Description : contains the hal i2c api functions interface with the stm32 i2c firmware library
 *
 *
 */

#include "netmf_i2c.h"

#define I2C_BUS_1 1

typedef struct
{
  uint16_t CR1;
  uint16_t  RESERVED0;
  uint16_t CR2;
  uint16_t  RESERVED1;
  uint16_t OAR1;
  uint16_t  RESERVED2;
  uint16_t OAR2;
  uint16_t  RESERVED3;
  uint16_t DR;
  uint16_t  RESERVED4;
  uint16_t SR1;
  uint16_t  RESERVED5;
  uint16_t SR2;
  uint16_t  RESERVED6;
  uint16_t CCR;
  uint16_t  RESERVED7;
  uint16_t TRISE;
  uint16_t  RESERVED8;
} I2C_TypeDef;

STM32F10x_I2C_Driver g_STM32F10x_i2c_driver;


I2C_TypeDef* I2C_BUS_ARRAY[I2C_BUSES] = {0, 0};

BOOL I2C_Internal_Initialize()
{

			return FALSE;
}

BOOL I2C_Internal_Uninitialize()
{

	return TRUE;
}

void I2C_Internal_XActionStart( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
}

void I2C_Internal_XActionStop()
{
}

void I2C_Internal_GetClockRate( UINT32 rateKhz, UINT8& clockRate, UINT8& clockRate2)
{

}

void I2C_Internal_GetPins(GPIO_PIN& scl, GPIO_PIN& sda)
{

}

extern "C"
{

void I2C_Event_Handler(void *param)
{



}

void I2C_Error_Handler(void  *param)
{

}

}


DeviceStatus STM32F10x_I2C_Driver::XActionStart(I2C_HAL_XACTION* xAction, bool repeatedStart)
{

}


DeviceStatus STM32F10x_I2C_Driver::XActionStop()
{

}

DeviceStatus STM32F10x_I2C_Driver::Initialize(I2CBus bus)
{



		return DS_Success;


}

DeviceStatus STM32F10x_I2C_Driver::UnInitialize()
{

	return DS_Success;

}
