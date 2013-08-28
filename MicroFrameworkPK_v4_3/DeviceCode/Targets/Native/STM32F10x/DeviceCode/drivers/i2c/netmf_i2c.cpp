/*
 * 	Name 	:   netmf_i2c.cpp
 *
 *  Author  :   nived.sivadas@samraksh.com
 *
 *  Description : contains the hal i2c api functions interface with the stm32 i2c firmware library
 *
 *
 */

#include "..\gpio\netmf_gpio.h"
#include "netmf_i2c.h"

#define I2C_BUS_1 1

STM32F10x_I2C_Driver g_STM32F10x_i2c_driver;

STM32F10x_GPIO_Driver g_STM32F10x_Gpio_Driver;

I2C_TypeDef* I2C_BUS_ARRAY[I2C_BUSES] = {I2C1, I2C2};

BOOL I2C_Internal_Initialize()
{
#if defined(I2C_BUS_1)
		if(g_STM32F10x_i2c_driver.Initialize(I2CBus1) == DS_Success)
			return TRUE;
		else
			return FALSE;
#else
		if(g_STM32F10x_i2c_driver.Initialize(I2CBus2) == DS_Success)
			return TRUE;
		else
			return FALSE;
#endif
}

BOOL I2C_Internal_Uninitialize()
{
	if(g_STM32F10x_i2c_driver.UnInitialize() != DS_Success)
		return FALSE;

	return TRUE;
}

void I2C_Internal_XActionStart( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
	g_STM32F10x_i2c_driver.XActionStart(xAction, repeatedStart);
}

void I2C_Internal_XActionStop()
{
	g_STM32F10x_i2c_driver.XActionStop();
}

void I2C_Internal_GetClockRate( UINT32 rateKhz, UINT8& clockRate, UINT8& clockRate2)
{

}

void I2C_Internal_GetPins(GPIO_PIN& scl, GPIO_PIN& sda)
{
#if defined(I2C_BUS_1)
    scl = I2C1_SCL;
    sda = I2C1_SDA;
#else
    scl = I2C2_SCL;
    sda = I2C2_SDA;
#endif
}

void I2C_Event_Handler(void *param)
{
	I2C_HAL_XACTION* xAction = g_STM32F10x_i2c_driver.getCurrentXAction();
	I2C_HAL_XACTION_UNIT* unit = g_STM32F10x_i2c_driver.getCurrentXActionUnit();

	// Work to do
	int todo = unit->m_bytesToTransfer;

	int sr1 = I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->SR1;
	int sr2 = I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->SR2;
	int cr1 = I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR1;

	if(unit->IsReadXActionUnit())
	{
		if(sr1 & I2C_SR1_SB)
		{
			if(todo == 1)
			{
				I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR1 = (cr1 &= ~I2C_CR1_ACK);
			}
			else if(todo == 2)
			{
				I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR1 = (cr1 |= I2C_CR1_POS);
			}
			UINT8 addr = xAction->m_address << 1;
			I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->DR = addr + 1;
		}
		else
		{
			if(sr1 & I2C_SR1_ADDR)
			{
				if(todo == 1)
				{
					I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR1 = (cr1 |= I2C_CR1_STOP); // send stop after single byte
				}
				else if(todo == 2)
				{
					I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR1 = (cr1 &= ~I2C_CR1_ACK); // last byte nack
				}
			}
			else
			{
				while(sr1 & I2C_SR1_RXNE)
				{
					if(todo == 2)
					{
						I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR1 = (cr1 |= I2C_CR1_STOP); // stop after last byte
					}
					else if(todo == 3)
					{
						 if (!(sr1 & I2C_SR1_BTF)) break; // assure 2 bytes are received
						 I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR1 = (cr1 &= ~I2C_CR1_ACK); // last byte nack
					}

					UINT8 data = I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->DR;
					*(unit->m_dataQueue.Push()) =data;
					unit->m_bytesTransferred++;
					unit->m_bytesToTransfer == --todo;
					sr1 = I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->SR1;
				}
			}
			if (todo == 1)
			{
				I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR2 |= I2C_CR2_ITBUFEN; // enable I2C_SR1_RXNE interrupt
			}
		}
	}
	else
	{
		if(sr1 & I2C_SR1_SB)
		{
			UINT8 addr = xAction->m_address << 1;
			I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->DR = addr;
		}else
		{
			while(todo && (sr1 & I2C_SR1_TXE))
			{
				I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->DR = *(unit->m_dataQueue.Pop());
				unit->m_bytesTransferred++;
				unit->m_bytesToTransfer = --todo;
				sr1 = I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->SR1;
			}
			if (!(sr1 & I2C_SR1_BTF)) todo++; // last byte not yet sent
		}
	}


}

void I2C_Error_Handler(void  *param)
{
	I2C_HAL_XACTION* xAction = g_STM32F10x_i2c_driver.getCurrentXAction();

	I2C_ClearFlag(I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus], I2C_FLAG_SMBALERT | I2C_FLAG_TIMEOUT | I2C_FLAG_PECERR | I2C_FLAG_OVR | I2C_FLAG_AF | I2C_FLAG_ARLO | I2C_FLAG_BERR);

	xAction->Signal(I2C_HAL_XACTION::c_Status_Aborted);

}


DeviceStatus STM32F10x_I2C_Driver::XActionStart(I2C_HAL_XACTION* xAction, bool repeatedStart)
{

	setCurrentXAction(xAction);

	setCurrentXActionUnit(xAction->m_xActionUnits[ xAction->m_current++ ]);

	if(currentActiveBus == I2CBus1)
	{
		I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
		I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE);
		I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);

		I2C_GenerateSTART(I2C1, ENABLE);
	}
	else
	{
		I2C_ITConfig(I2C2, I2C_IT_EVT, ENABLE);
		I2C_ITConfig(I2C2, I2C_IT_ERR, ENABLE);
		I2C_ITConfig(I2C2, I2C_IT_BUF, ENABLE);

		I2C_GenerateSTART(I2C2, ENABLE);
	}
}


DeviceStatus STM32F10x_I2C_Driver::XActionStop()
{
	if (I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->SR2 & I2C_SR2_BUSY && !(I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR1 & I2C_CR1_STOP)) {
		I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR1 |= I2C_CR1_STOP; // send stop
	    }
		I2C_BUS_ARRAY[g_STM32F10x_i2c_driver.currentActiveBus]->CR2 &= ~(I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN); // disable interrupts
	    currentI2CXAction = NULL;
	    currentI2CUnit = NULL;
}

DeviceStatus STM32F10x_I2C_Driver::Initialize(I2CBus bus)
{

	if(IsInitialized)
			return DS_Success;

	I2C_InitTypeDef I2C_InitStruct;
	// Initialize the i2c clock
	//RCC_APB1PeriphClockCmd(I2CClock[bus], ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	if(bus == I2CBus1)
	{
		// Check if the pins are busy before enabling them
		if(CPU_GPIO_PinIsBusy(I2C1_SCL) || CPU_GPIO_PinIsBusy(I2C1_SDA))
		{
			I2C_DEBUG_PRINT("[Native] [I2C Driver] The I2C Bus1 Pins are busy \n");
			return DS_Busy;
		}
		else
		{
			// Enable the pins in alternate mode configuration
			CPU_GPIO_DisablePin(I2C1_SCL, RESISTOR_DISABLED, g_STM32F10x_Gpio_Driver.c_DirectionOut , GPIO_ALT_MODE_1);
			CPU_GPIO_DisablePin(I2C1_SDA, RESISTOR_DISABLED, g_STM32F10x_Gpio_Driver.c_DirectionOut , GPIO_ALT_MODE_1);
		}
	}
	else
	{
		// Check if the pins are busy before enabling them
		if(CPU_GPIO_PinIsBusy(I2C2_SCL) || CPU_GPIO_PinIsBusy(I2C2_SDA))
		{
			I2C_DEBUG_PRINT("[Native] [I2C Driver] The I2C Bus2 Pins are busy \n");
			return DS_Busy;
		}
		else
		{
			// Enable the pins in alternate mode configuration
			CPU_GPIO_DisablePin(I2C2_SCL, RESISTOR_DISABLED, g_STM32F10x_Gpio_Driver.c_DirectionOut , GPIO_ALT_MODE_1);
			CPU_GPIO_DisablePin(I2C2_SDA, RESISTOR_DISABLED, g_STM32F10x_Gpio_Driver.c_DirectionOut , GPIO_ALT_MODE_1);
		}
	}

	currentActiveBus = bus;

	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1 = 0x33;



	if(bus == I2CBus1)
	{
		if(!CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_I2C1_EV, I2C_Event_Handler, NULL ))
			return DS_Fail;

		if(!CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_I2C1_ER, I2C_Error_Handler, NULL ))
			return DS_Fail;


		if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_I2C1_EV))
			return DS_Fail;

		if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_I2C1_ER))
			return DS_Fail;
	}
	else
	{
		if(!CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_I2C2_EV, I2C_Event_Handler, NULL))
			return DS_Fail;

		if(!CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_I2C2_ER, I2C_Error_Handler, NULL ))
			return DS_Fail;

		if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_I2C2_EV))
			return DS_Fail;

		if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_I2C2_ER))
			return DS_Fail;
	}

	if(bus == I2CBus1)
	{
		I2C_Init(I2C1, &I2C_InitStruct);
		I2C_Cmd(I2C1, ENABLE);

	}
	else
	{
		I2C_Init(I2C2, &I2C_InitStruct);
		I2C_Cmd(I2C2, ENABLE);

	}

	IsInitialized = TRUE;

	return DS_Success;

}

DeviceStatus STM32F10x_I2C_Driver::UnInitialize()
{
	if(currentActiveBus == I2CBus1)
	{
		I2C_DeInit(I2C1);
		I2C_SoftwareResetCmd(I2C1, ENABLE);
		for(UINT32 i = 0; i < 100; i++) {}
		I2C_SoftwareResetCmd(I2C1, DISABLE);
	}
	else
	{
		I2C_DeInit(I2C2);
		I2C_SoftwareResetCmd(I2C2, ENABLE);
		for(UINT32 i = 0; i < 100; i++) {}
		I2C_SoftwareResetCmd(I2C2, DISABLE);

	}

	return DS_Success;

}
