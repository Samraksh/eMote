// LCD.cpp

#include "LCD.h"
#include <i2c/stm32f10x_i2c.h>
#include <rcc/stm32f10x_rcc.h>
#include <gpio/stm32f10x_gpio.h>

const uint8_t LCD_NUM[43] = { 0xEE, 0xEF, 0x8D, 0x8F, 0x8E, 0xCD, 0x6E, 0x60, 0x65, 0x0D, 0xEC, 0xED, 0xAE, 0xCB, 0x6D, 0x6A, 0xE7, 0x4F, 0x07, 0x67, 0xAF, 0xEB, 0x4E, 0x40, 0x41, 0x60, 0x46, 0x47, 0xEA, 0x06, 0x0F, 0x45, 0xED, 0x60, 0xA7,0xE3, 0x6A, 0xCB, 0xCF, 0xE0, 0xEF, 0xEB, 0x00 };


EmoteDotNow_LCD_Driver::EmoteDotNow_LCD_Driver(){
}

bool EmoteDotNow_LCD_Driver::Initialize()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStruct;

	GPIO_PinRemapConfig(GPIO_Remap_FSMC_NADV, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  	I2C_InitStruct.I2C_ClockSpeed = 100000;
  	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  	I2C_InitStruct.I2C_OwnAddress1 = 0x33;

  	I2C_Cmd(I2C1, ENABLE);
  	I2C_Init(I2C1, &I2C_InitStruct); 

	return true;
}

bool EmoteDotNow_LCD_Driver::Uninitialize()
{
	I2C_Cmd(I2C1, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, DISABLE);
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
	return true;
}

bool EmoteDotNow_LCD_Driver::Write(int data4, int data3, int data2, int data1)
{
	int byte1, byte2, byte3, byte4;

	if ( (data1 & DISPLAY_DECIMAL_POINT) == DISPLAY_DECIMAL_POINT) {
		// If the DISPLAY_DECIMAL_POINT was AND'ed with the data then we strip it out, look up the 7-seg representation and then put back the 7-seg decimal point
		byte1 = LCD_NUM[(data1 & ~DISPLAY_DECIMAL_POINT)] | 0x10;
	} else {
		// There is not decimal point so we just look up the 7-seg representation
		byte1 = LCD_NUM[(data1 & ~DISPLAY_DECIMAL_POINT)];
	}
	if ( (data2 & DISPLAY_DECIMAL_POINT) == DISPLAY_DECIMAL_POINT) {
		// If the DISPLAY_DECIMAL_POINT was AND'ed with the data then we strip it out, look up the 7-seg representation and then put back the 7-seg decimal point
		byte2 = LCD_NUM[(data2 & ~DISPLAY_DECIMAL_POINT)] | 0x10;
	} else {
		// There is not decimal point so we just look up the 7-seg representation
		byte2 = LCD_NUM[(data2 & ~DISPLAY_DECIMAL_POINT)];
	}
	if ( (data3 & DISPLAY_DECIMAL_POINT) == DISPLAY_DECIMAL_POINT) {
		// If the DISPLAY_DECIMAL_POINT was AND'ed with the data then we strip it out, look up the 7-seg representation and then put back the 7-seg decimal point
		byte3 = LCD_NUM[(data3 & ~DISPLAY_DECIMAL_POINT)] | 0x10;
	} else {
		// There is not decimal point so we just look up the 7-seg representation
		byte3 = LCD_NUM[(data3 & ~DISPLAY_DECIMAL_POINT)];
	}
	if ( (data4 & DISPLAY_DECIMAL_POINT) == DISPLAY_DECIMAL_POINT) {
		// If the DISPLAY_DECIMAL_POINT was AND'ed with the data then we strip it out, look up the 7-seg representation and then put back the 7-seg decimal point
		byte4 = LCD_NUM[(data4 & ~DISPLAY_DECIMAL_POINT)] | 0x10;
	} else {
		// There is not decimal point so we just look up the 7-seg representation
		byte4 = LCD_NUM[(data4 & ~DISPLAY_DECIMAL_POINT)];
	}

	currentColumn1 = byte1;
	currentColumn2 = byte2;
	currentColumn3 = byte3;
	currentColumn4 = byte4;

	I2C_GenerateSTART(I2C1, ENABLE);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; }

  	I2C_Send7bitAddress(I2C1, 0x70, I2C_Direction_Transmitter);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) { ; }

  	I2C_SendData(I2C1, 0x80); // Load-data-pointer, reset to 0, 10000000
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, 0x48); // Mode Set, 01001000
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	// Start sending display bytes
  	I2C_SendData(I2C1, currentColumn4);
 	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, currentColumn3);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, currentColumn2);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, currentColumn1);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

	I2C_GenerateSTOP(I2C1, ENABLE);

	return true;
}

bool EmoteDotNow_LCD_Driver :: WriteN(int column, int data)
{
	int byte;

	data = data & 0xff;

	if ( (data & DISPLAY_DECIMAL_POINT) == DISPLAY_DECIMAL_POINT) {
		// If the DISPLAY_DECIMAL_POINT was AND'ed with the data then we strip it out, look up the 7-seg representation and then put back the 7-seg decimal point
		byte = LCD_NUM[(data & ~DISPLAY_DECIMAL_POINT)] | 0x10;
	} else {
		// There is not decimal point so we just look up the 7-seg representation
		byte = LCD_NUM[(data & ~DISPLAY_DECIMAL_POINT)];
	}
	switch (column){
		case 1:
			currentColumn1 = byte;
			break;
		case 2:
			currentColumn2 = byte;
			break;
		case 3:
			currentColumn3 = byte;
			break;
		case 4:
			currentColumn4 = byte;
			break;
		default:
			currentColumn4 = byte;
			break;
	}

	I2C_GenerateSTART(I2C1, ENABLE);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; }

  	I2C_Send7bitAddress(I2C1, 0x70, I2C_Direction_Transmitter);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) { ; }

  	I2C_SendData(I2C1, 0x80); // Load-data-pointer, reset to 0, 10000000
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, 0x48); // Mode Set, 01001000
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	// Start sending display bytes
  	I2C_SendData(I2C1, currentColumn4);
 	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, currentColumn3);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, currentColumn2);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, currentColumn1);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

	I2C_GenerateSTOP(I2C1, ENABLE);

	return true;
}

bool EmoteDotNow_LCD_Driver :: WriteRawBytes(int data4, int data3, int data2, int data1)
{
	currentColumn1 = data1 & 0xff;
	currentColumn2 = data2 & 0xff;
	currentColumn3 = data3 & 0xff;
	currentColumn4 = data4 & 0xff;

	I2C_GenerateSTART(I2C1, ENABLE);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; }

  	I2C_Send7bitAddress(I2C1, 0x70, I2C_Direction_Transmitter);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) { ; }

  	I2C_SendData(I2C1, 0x80); // Load-data-pointer, reset to 0, 10000000
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, 0x48); // Mode Set, 01001000
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	// Start sending display bytes
  	I2C_SendData(I2C1, currentColumn4);
 	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, currentColumn3);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, currentColumn2);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  	I2C_SendData(I2C1, currentColumn1);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

	I2C_GenerateSTOP(I2C1, ENABLE);

	return true;
}

bool EmoteDotNow_LCD_Driver :: Blink(int blinkType){
	I2C_GenerateSTART(I2C1, ENABLE);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; }

  	I2C_Send7bitAddress(I2C1, 0x70, I2C_Direction_Transmitter);
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) { ; }

  	I2C_SendData(I2C1, 0x80); // Load-data-pointer, reset to 0, 10000000
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

	switch (blinkType){
		case (0):
			I2C_SendData(I2C1, 0xF0); 
			break;
		case (1):
			I2C_SendData(I2C1, 0xF3); 
			break;
		case (2):
			I2C_SendData(I2C1, 0xF2); 
			break;
		case(3):
			I2C_SendData(I2C1, 0xF1); 
			break;
		default:
			I2C_SendData(I2C1, 0xF0); 
			break;
	}
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

	I2C_GenerateSTOP(I2C1, ENABLE);
	return true;
}
