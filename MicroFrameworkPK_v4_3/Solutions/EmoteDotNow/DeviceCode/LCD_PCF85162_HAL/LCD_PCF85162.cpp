// LCD_PCF85162.cpp
// History : Chris.Morse   - Initial Version
//			 Nived.Sivadas - Removed all infinite while loops and replaced them with timeout loops

#include <tinyhal.h>
#include "LCD_PCF85162.h"
#include <i2c/stm32f10x_i2c.h>
#include <rcc/stm32f10x_rcc.h>
#include <gpio/stm32f10x_gpio.h>

LCD_PCF85162_Driver glcdObject;

const uint8_t LCD_NUM[63] = { 0x00, 0xee, 0xef, 0x8d, 0xed, 0x8f, 0x8e, 0xcd, 0x6e, 0x60, 0x65, 0x6e, 0x0d, 0xc6, 0xec, 0xed, 0xae, 0xea, 0x8c, 0xcb, 0xe0, 0x6d, 0x29, 0xc5, 0x6e, 0x6a, 0xa7, 0xe7, 0x4f, 0x07, 0x67, 0xaf, 0x8e, 0xeb, 0x4e, 0x40, 0x41, 0x2f, 0x60, 0x8f, 0x46, 0x47, 0xae, 0xea, 0x06, 0xcb, 0x0f, 0x45, 0x45, 0xe3, 0x6e, 0x6b, 0xa7, 0xed, 0x60, 0xa7, 0xe3, 0x6a, 0xcb, 0xcf, 0xe0, 0xef, 0xeb };


LCD_PCF85162_Driver g_LCD_PCF85162_Driver;

LCD_PCF85162_Driver::LCD_PCF85162_Driver(){
}

bool LCD_PCF85162_Driver::Initialize()
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

	currentColumn1 = 0;
	currentColumn2 = 0;
	currentColumn3 = 0;
	currentColumn4 = 0;
	DP1 = false;
	DP2 = false;
	DP3 = false;
	DP4 = false;

	return true;
}

bool LCD_PCF85162_Driver::Uninitialize()
{
	I2C_Cmd(I2C1, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, DISABLE);
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
	return true;
}

bool LCD_PCF85162_Driver::Write(int data4, int data3, int data2, int data1)
{
	int byte1, byte2, byte3, byte4;
	INIT_I2C_STATE_CHECK();

	currentColumn1 = data1;
	currentColumn2 = data2;
	currentColumn3 = data3;
	currentColumn4 = data4;

	if ( DP1 == true) {
		byte1 = LCD_NUM[data1] | 0x10;
	} else {
		byte1 = LCD_NUM[data1];
	}
	if ( DP2 == true) {
		byte2 = LCD_NUM[data2] | 0x10;
	} else {
		byte2 = LCD_NUM[data2];
	}
	if ( DP3 == true) {
		byte3 = LCD_NUM[data3] | 0x10;
	} else {
		byte3 = LCD_NUM[data3];
	}
	if ( DP4 == true) {
		byte4 = LCD_NUM[data4] | 0x10;
	} else {
		byte4 = LCD_NUM[data4];
	}

	// Nived.Sivadas - Changing all infinite whiles to timer based ones
	I2C_GenerateSTART(I2C1, ENABLE);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; }
	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_MODE_SELECT);

  	I2C_Send7bitAddress(I2C1, 0x70, I2C_Direction_Transmitter);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

  	I2C_SendData(I2C1, 0x80); // Load-data-pointer, reset to 0, 10000000
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, 0x48); // Mode Set, 01001000
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	// Start sending display bytes
  	I2C_SendData(I2C1, byte4);
 	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, byte3);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, byte2);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, byte1);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

	I2C_GenerateSTOP(I2C1, ENABLE);

	return true;
}

bool LCD_PCF85162_Driver :: SetDP(bool dp4, bool dp3, bool dp2, bool dp1)
{
	DP1 = dp1;
	DP2 = dp2;
	DP3 = dp3;
	DP4 = dp4;

	Write(currentColumn4, currentColumn3, currentColumn2, currentColumn1);

	return true;
}

bool LCD_PCF85162_Driver :: WriteN(int column, int data)
{
	int byte1, byte2, byte3, byte4;
	INIT_I2C_STATE_CHECK();

	switch (column){
		case 1:
			currentColumn1 = data;
			break;
		case 2:
			currentColumn2 = data;			
			break;
		case 3:
			currentColumn3 = data;
			break;
		case 4:
			currentColumn4 = data;
			break;
		default:
			currentColumn4 = data;
			break;
	}

	if ( DP1 == true) {
		byte1 = LCD_NUM[currentColumn1] | 0x10;
	} else {
		byte1 = LCD_NUM[currentColumn1];
	}
	if ( DP2 == true) {
		byte2 = LCD_NUM[currentColumn2] | 0x10;
	} else {
		byte2 = LCD_NUM[currentColumn2];
	}
	if ( DP3 == true) {
		byte3 = LCD_NUM[currentColumn3] | 0x10;
	} else {
		byte3 = LCD_NUM[currentColumn3];
	}
	if ( DP4 == true) {
		byte4 = LCD_NUM[currentColumn4] | 0x10;
	} else {
		byte4 = LCD_NUM[currentColumn4];
	}

	I2C_GenerateSTART(I2C1, ENABLE);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; }
	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_MODE_SELECT);

  	I2C_Send7bitAddress(I2C1, 0x70, I2C_Direction_Transmitter);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

  	I2C_SendData(I2C1, 0x80); // Load-data-pointer, reset to 0, 10000000
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

  	I2C_SendData(I2C1, 0x48); // Mode Set, 01001000
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	// Start sending display bytes
  	I2C_SendData(I2C1, byte4);
 	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, byte3);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, byte2);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, byte1);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

	I2C_GenerateSTOP(I2C1, ENABLE);

	return true;
}

bool LCD_PCF85162_Driver :: WriteRawBytes(int data4, int data3, int data2, int data1)
{
	INIT_I2C_STATE_CHECK();

	currentColumn1 = data1 & 0xff;
	currentColumn2 = data2 & 0xff;
	currentColumn3 = data3 & 0xff;
	currentColumn4 = data4 & 0xff;

	I2C_GenerateSTART(I2C1, ENABLE);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; }
	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_MODE_SELECT);

  	I2C_Send7bitAddress(I2C1, 0x70, I2C_Direction_Transmitter);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

  	I2C_SendData(I2C1, 0x80); // Load-data-pointer, reset to 0, 10000000
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, 0x48); // Mode Set, 01001000
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	// Start sending display bytes
  	I2C_SendData(I2C1, currentColumn4);
 	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, currentColumn3);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, currentColumn2);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  	I2C_SendData(I2C1, currentColumn1);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

	I2C_GenerateSTOP(I2C1, ENABLE);

	return true;
}

bool LCD_PCF85162_Driver :: Blink(int blinkType){

	INIT_I2C_STATE_CHECK();

	I2C_GenerateSTART(I2C1, ENABLE);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; }
	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_MODE_SELECT);

  	I2C_Send7bitAddress(I2C1, 0x70, I2C_Direction_Transmitter);
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

  	I2C_SendData(I2C1, 0x80); // Load-data-pointer, reset to 0, 10000000
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
  	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

	switch (blinkType){
		case (BLINK_OFF):
			I2C_SendData(I2C1, 0xF0); 
			break;
		case (BLINK_SLOW):
			I2C_SendData(I2C1, 0xF3); 
			break;
		case (BLINK_MEDIUM):
			I2C_SendData(I2C1, 0xF2); 
			break;
		case(BLINK_FAST):
			I2C_SendData(I2C1, 0xF1); 
			break;
		default:
			I2C_SendData(I2C1, 0xF0); 
			break;
	}
  	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
	DID_I2C_STATE_CHANGE(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);

	I2C_GenerateSTOP(I2C1, ENABLE);
	return true;
}
