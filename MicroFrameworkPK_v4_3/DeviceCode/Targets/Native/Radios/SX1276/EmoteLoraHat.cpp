/*
 * EmoteLoraHat.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: Bora
 */

#include "EmoteLoraHat.h"

namespace LoraHat {


bool LoraHardwareConfig::SpiInitialize() {
//	initSPI2();
//	 Calling mf spi initialize function

	m_spi_config.DeviceCS               = GPIO_LORA_CHIPSELECT ; //TODO - fix me..
	m_spi_config.CS_Active              = false;
	m_spi_config.CS_Setup_uSecs         = 0;
	m_spi_config.CS_Hold_uSecs          = 0;
	m_spi_config.MSK_IDLE               = false;
	m_spi_config.MSK_SampleEdge         = false;
	m_spi_config.Clock_RateKHz          = 16; // THIS IS IGNORED.
	m_spi_config.SPI_mod                = 0;\
	m_spi_config.MD_16bits = FALSE;

	// Enable the SPI depending on the radio who is the user
	CPU_SPI_Enable(m_spi_config);



	return TRUE;

}



LoraHardwareConfig::LoraHardwareConfig() {

	Initialize();
}



void LoraHardwareConfig::reset(){
//	CPU_GPIO_SetPinState(SX1276_pin_setup.reset_mf_pin, TRUE);
//	CPU_GPIO_SetPinState(SX1276_pin_setup.reset_mf_pin, FALSE);
}


void LoraHardwareConfig::init_pins(){
	SX1276_pin_setup.SX1276_interupt_pins.DIO0			= (GPIO_PIN)46; //P5_6
	SX1276_pin_setup.SX1276_interupt_pins.DIO1			= (GPIO_PIN)45;	//P5_5
	SX1276_pin_setup.SX1276_interupt_pins.DIO2			= (GPIO_PIN)26; //P3_2
	SX1276_pin_setup.SX1276_interupt_pins.DIO3			= (GPIO_PIN)44; //P5_4
	SX1276_pin_setup.SX1276_interupt_pins.DIO4			= (GPIO_PIN)43; //P5_3
	SX1276_pin_setup.SX1276_interupt_pins.DIO5			= (GPIO_PIN)27; //P3_3

	SX1276_pin_setup.reset_mf_pin		= (GPIO_PIN)120; //
}

void LoraHardwareConfig::Initialize(){

	SpiInitialize();
	init_pins();
	reset();
}

//void LoraHardwareConfig::initSPI2() {
/*	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned int baud;
	unsigned SpiBusClock;
	SPI_InitTypeDef SPI_InitStruct;

	RCC_APB1PeriphClockCmd(SX1276_pin_setup.spi_rcc,	ENABLE);
	SPI_I2S_DeInit(SX1276_pin_setup.spi_base);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin =  SX1276_pin_setup.sclk_pin | SX1276_pin_setup.mosi_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(SX1276_pin_setup.spi_port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = SX1276_pin_setup.miso_pin;
	GPIO_Init(SX1276_pin_setup.spi_port, &GPIO_InitStructure);

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;

	// FIX ME, TEMP
	if (SX1276_pin_setup.spi_base != SPI1)
		SpiBusClock = get_APB1_clock();
	else
		SpiBusClock = get_APB2_clock();

	switch(SPI_InitStruct.SPI_BaudRatePrescaler) {
		case SPI_BaudRatePrescaler_2:  baud = SpiBusClock / 2  / 1000; break;
		case SPI_BaudRatePrescaler_4:  baud = SpiBusClock / 4  / 1000; break;
		case SPI_BaudRatePrescaler_8:  baud = SpiBusClock / 8  / 1000; break;
		case SPI_BaudRatePrescaler_16: baud = SpiBusClock / 16 / 1000; break;
		default: baud = 0;
	}

	SPI_Init(SX1276_pin_setup.spi_base, &SPI_InitStruct);
	SPI_Cmd(SX1276_pin_setup.spi_base, ENABLE);

*/
//}

/*
void LoraHardwareConfig::spi_write_bytes(unsigned count,
		const uint8_t* buf)  {
	for(unsigned i=0; i<count; i++) {
		radio_spi_go(buf[i]);
	}
}

uint8_t LoraHardwareConfig::radio_spi_go(uint8_t data)  {
//	while( SPI_I2S_GetFlagStatus(SX1276_pin_setup.spi_base, SPI_I2S_FLAG_TXE) == RESET ) ; // spin
//	SPI_I2S_SendData(SX1276_pin_setup.spi_base, data);
//	while( SPI_I2S_GetFlagStatus(SX1276_pin_setup.spi_base, SPI_I2S_FLAG_RXNE) == RESET ) ; // spin
//	return SPI_I2S_ReceiveData(SX1276_pin_setup.spi_base);
}

void LoraHardwareConfig::radio_spi_sel_no_assert() {
	GPIO_WriteBit(SX1276_pin_setup.cs_port, SX1276_pin_setup.cs_pin, Bit_SET); // chip select
}

void LoraHardwareConfig::spi_read_bytes(unsigned count, uint8_t* buf)  {
	for(unsigned i=0; i<count; i++) {
		buf[i] = radio_spi_go(0);
	}
}
*/
/*
unsigned int LoraHardwareConfig::radio_comm_PollCTS(){
	unsigned timeout=0;

#ifdef SI446X_AGGRESSIVE_CTS
	if (ctsWentHigh) return 1;
#else
	ctsWentHigh = 0;
#endif

	while(ctsWentHigh == 0 && timeout < CTS_TIMEOUT) {
		radio_comm_GetResp(0, NULL);
		timeout++;
	}
	if (timeout == CTS_TIMEOUT) {
		ctsWentHigh = 0;
		LORA_ASSERT(0, "Fatal: CTS Timeout waiting for response\r\n");
		return 0;
	}

	return 1;
}

void LoraHardwareConfig::radio_spi_sel_assert() {
	GPIO_WriteBit(SX1276_pin_setup.cs_port, SX1276_pin_setup.cs_pin, Bit_RESET); // chip select
	__NOP();
}

uint8_t LoraHardwareConfig::radio_comm_GetResp(uint8_t byteCount,
		uint8_t* pData) {
	unsigned ctsVal;
	unsigned timeout=0;

	radio_spi_sel_assert();
	radio_spi_go(0x44); //read CMD buffer
	ctsVal = radio_spi_go(0);

	while(ctsVal != 0xFF && timeout++ <= CTS_TIMEOUT) {
		radio_spi_sel_no_assert();
		//for(unsigned i=0; i<CTS_WAIT; i++) ; // spin
		// Looking for at least 150ns, or likely even half that would be enough.
		__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
		radio_spi_sel_assert();
		radio_spi_go(0x44); //read CMD buffer
		ctsVal = radio_spi_go(0);
	}

	if (ctsVal != 0xFF) {
		radio_spi_sel_no_assert();
		LORA_ASSERT(0, "Fatal: CTS Timeout waiting for response\r\n");
		return 0;
	}
	else {
		ctsWentHigh = 1;
	}

	if (byteCount) {
		spi_read_bytes(byteCount, pData);
	}
	radio_spi_sel_no_assert();

	return ctsVal;
}
*/
} /* namespace LoraHat */
