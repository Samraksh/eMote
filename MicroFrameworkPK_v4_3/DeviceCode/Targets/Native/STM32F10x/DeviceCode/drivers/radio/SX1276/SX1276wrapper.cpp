/*
 * SX1276wrapper.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: Bora
 */


//#include <cstdint>
#include <Samraksh/VirtualTimer.h>

#include "SX1276wrapper.h"

#define SX1276M1BxASWrapper_debug_PIN (GPIO_PIN)120

#define READ_OPMODE() (Read( REG_OPMODE ) & ~RFLR_OPMODE_MASK)

SX1276M1BxASWrapper g_SX1276M1BxASWrapper;



//namespace SX1276_Semtech {


//const RadioRegisters_t SX1276MB1xAS::RadioRegsInit[] = RADIO_INIT_REGISTERS_VALUE;



#ifndef SX1276_NO_DEBUG_PRINT
static void SX1276_debug_print(int priority, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    if(priority >= si4468x_debug_level) {
		hal_vprintf(fmt, args);
	}

    va_end(args);
}
#else
// Compiler is smart enough to toss out debug strings even in debug mode
static void SX1276_debug_print(int priority, const char *fmt, ...) { return; }
#endif


// Temporary hack.
static unsigned get_APB1_clock() {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	return RCC_Clocks.PCLK1_Frequency;
}

static unsigned get_APB2_clock() {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	return RCC_Clocks.PCLK2_Frequency;
}


bool SX1276M1BxASWrapper::SpiInitialize() {
	initSPI2();
	// Calling mf spi initialize function
	//if(TRUE != CPU_SPI_Initialize())
	//	return FALSE;

//	config.DeviceCS               = 10; //TODO - fix me..
//	config.CS_Active              = false;
//	config.CS_Setup_uSecs         = 0;
//	config.CS_Hold_uSecs          = 0;
//	config.MSK_IDLE               = false;
//	config.MSK_SampleEdge         = false;
//	config.Clock_RateKHz          = 16; // THIS IS IGNORED.
//	if(this->GetRadioName() == RF231RADIO)
//	{
//		config.SPI_mod                = RF231_SPI_BUS;
//	}
//	else if(this->GetRadioName() == RF231RADIOLR)
//	{
//		config.SPI_mod 				  = RF231_LR_SPI_BUS;
//	}
//
//	config.MD_16bits = FALSE;
//
//
//	// Enable the SPI depending on the radio who is the user
//	CPU_SPI_Enable(config);



//	m_spi_config.DeviceCS               = GPIO_Pin_12; //TODO - fix me..
//	m_spi_config.CS_Active              = false;
//	m_spi_config.CS_Setup_uSecs         = 0;
//	m_spi_config.CS_Hold_uSecs          = 0;
//	m_spi_config.MSK_IDLE               = false;
//	m_spi_config.MSK_SampleEdge         = false;
//	m_spi_config.Clock_RateKHz          = 16; // THIS IS IGNORED.
//	m_spi_config.SPI_mod = SPIBUS2;
//	m_spi_config.MD_16bits = FALSE;
//
//	CPU_SPI_Initialize();
//	// Enable the SPI depending on the radio who is the user
//	CPU_SPI_Enable(m_spi_config);
//
//	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , FALSE);
//	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , TRUE);


	return TRUE;

}


void SX1276M1BxASWrapper::radio_comm_WriteData(uint8_t cmd, unsigned pollCts,
		uint8_t byteCount, uint8_t* pData) {
//    if(pollCts)
//    {
//        while(!SX1276M1BxASWrapper::ctsWentHigh)
//        {
//            radio_comm_PollCTS();
//        }
//    }
	radio_spi_sel_assert(); //cs pin chip select
	radio_spi_go(cmd);
	spi_write_bytes(byteCount, pData);
	radio_spi_sel_no_assert();

    //radio_hal_ClearNsel();
    //radio_hal_SpiWriteByte(cmd);
    //radio_hal_SpiWriteData(byteCount, pData);
    //radio_hal_SetNsel();

//    ctsWentHigh = 0;



}

void SX1276M1BxASWrapper::radio_comm_ReadData(uint8_t cmd, unsigned pollCts,
		uint8_t byteCount, uint8_t* pData) {
	if (pollCts) { // Not sure of case where you wouldn't want to do this
		radio_comm_PollCTS();
	}
	radio_spi_sel_assert();
	radio_spi_go(cmd);
	spi_read_bytes(byteCount, pData);
	radio_spi_sel_no_assert();
//	ctsWentHigh = 0;
}



void SX1276M1BxASWrapper::reset(){
	CPU_GPIO_SetPinState(SX1276_pin_setup.reset_mf_pin, TRUE);
	CPU_GPIO_SetPinState(SX1276_pin_setup.reset_mf_pin, FALSE);
}


void SX1276M1BxASWrapper::init_pins(){
	GPIO_InitTypeDef GPIO_InitStructure;

	SX1276_pin_setup_t *config = &SX1276_pin_setup;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =  config->reset_pin;
	GPIO_Init(config->reset_port, &GPIO_InitStructure);



	// NIRQ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = config->nirq_pin0;
	GPIO_Init(config->nirq_port0, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = config->nirq_pin1;
	GPIO_Init(config->nirq_port1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = config->nirq_pin2;
	GPIO_Init(config->nirq_port2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = config->nirq_pin3;
	GPIO_Init(config->nirq_port3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = config->nirq_pin4;
	GPIO_Init(config->nirq_port4, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = config->nirq_pin5;
	GPIO_Init(config->nirq_port5, &GPIO_InitStructure);

	// PA4 SPI chip select
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = config->cs_pin;
	GPIO_WriteBit(config->cs_port, config->cs_pin, Bit_SET); // Set
	GPIO_Init(config->cs_port, &GPIO_InitStructure);
}

void SX1276M1BxASWrapper::LoraHardwareConfigInitialize(){




	SX1276_pin_setup.nirq_port0			= GPIOA;
	SX1276_pin_setup.nirq_pin0			= GPIO_Pin_1;
	SX1276_pin_setup.nirq_mf_pin0		= (GPIO_PIN) 1;

	SX1276_pin_setup.nirq_port1			= GPIOA;
	SX1276_pin_setup.nirq_pin1			= GPIO_Pin_2;
	SX1276_pin_setup.nirq_mf_pin1		= (GPIO_PIN) 2;

	SX1276_pin_setup.nirq_port2			= GPIOA;
	SX1276_pin_setup.nirq_pin2			= GPIO_Pin_3;
	SX1276_pin_setup.nirq_mf_pin2		= (GPIO_PIN) 3;

	SX1276_pin_setup.nirq_port3			= GPIOA;
	SX1276_pin_setup.nirq_pin3			= GPIO_Pin_8;
	SX1276_pin_setup.nirq_mf_pin3		= (GPIO_PIN) 8;


	SX1276_pin_setup.nirq_port4			= GPIOA;
	SX1276_pin_setup.nirq_pin4			= GPIO_Pin_0; //GPIO_Pin_22; // TODO:
	SX1276_pin_setup.nirq_mf_pin4		= (GPIO_PIN) 22;

	SX1276_pin_setup.nirq_port5			= GPIOA;
	SX1276_pin_setup.nirq_pin5			= GPIO_Pin_0; //GPIO_Pin_23;
	SX1276_pin_setup.nirq_mf_pin5		= (GPIO_PIN) 23;

	SX1276_pin_setup.reset_port			= GPIOA; // ?
	SX1276_pin_setup.reset_pin			= GPIO_Pin_0; // GPIO_Pin_24;
	SX1276_pin_setup.reset_mf_pin		= (GPIO_PIN) 24;

	SX1276_pin_setup.spi_base 			= SPI2;
	SX1276_pin_setup.spi_port 			= GPIOB;
	SX1276_pin_setup.cs_port			= GPIOB;
	SX1276_pin_setup.cs_pin				= GPIO_Pin_9;  //MS changed this
	SX1276_pin_setup.sclk_pin			= GPIO_Pin_13; //15 BK: I think the default values are as commented. But SI radio was using a different pin setup
	SX1276_pin_setup.miso_pin			= GPIO_Pin_14; //14
	SX1276_pin_setup.mosi_pin			= GPIO_Pin_15; //13

	SX1276_pin_setup.spi_rcc			= RCC_APB1Periph_SPI2;


	SX1276_interupt_pins.DIO0			= SX1276_pin_setup.nirq_mf_pin0;
	SX1276_interupt_pins.DIO1			= SX1276_pin_setup.nirq_mf_pin1;
	SX1276_interupt_pins.DIO2			= SX1276_pin_setup.nirq_mf_pin2;
	SX1276_interupt_pins.DIO3			= SX1276_pin_setup.nirq_mf_pin3;
	SX1276_interupt_pins.DIO4			= SX1276_pin_setup.nirq_mf_pin4;
	SX1276_interupt_pins.DIO5			= SX1276_pin_setup.nirq_mf_pin5;


//	initSPI2();
	init_pins();
	//reset();





}

void SX1276M1BxASWrapper::initSPI2() {
	GPIO_InitTypeDef GPIO_InitStructure;
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
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
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

	if (SX1276_pin_setup.spi_base == SPI2) {
		SX1276_debug_print(SX1276DEBUG02,"SPI2 up CPOL: %d CPHA: %d Baud: %d kHz (%d kHz bus)\r\n",
			SPI_InitStruct.SPI_CPOL, SPI_InitStruct.SPI_CPHA, baud, SpiBusClock/1000);
	}
	else {
		SX1276_debug_print(SX1276DEBUG02,"SPI??? up CPOL: %d CPHA: %d Baud: %d kHz (%d kHz bus)\r\n",
			SPI_InitStruct.SPI_CPOL, SPI_InitStruct.SPI_CPHA, baud, SpiBusClock/1000);
	}
}

void SX1276M1BxASWrapper::spi_write_bytes(unsigned count,
		const uint8_t* buf)  {
	for(unsigned i=0; i<count; i++) {
		radio_spi_go(buf[i]);
	}
}

uint8_t SX1276M1BxASWrapper::radio_spi_go(uint8_t data)  {
	while( SPI_I2S_GetFlagStatus(SX1276_pin_setup.spi_base, SPI_I2S_FLAG_TXE) == RESET ) ; // spin
	SPI_I2S_SendData(SX1276_pin_setup.spi_base, data);
	while( SPI_I2S_GetFlagStatus(SX1276_pin_setup.spi_base, SPI_I2S_FLAG_RXNE) == RESET ) ; // spin
	return SPI_I2S_ReceiveData(SX1276_pin_setup.spi_base);
}

void SX1276M1BxASWrapper::radio_spi_sel_no_assert() {
	GPIO_WriteBit(SX1276_pin_setup.cs_port, SX1276_pin_setup.cs_pin, Bit_SET); // chip select
}

void SX1276M1BxASWrapper::spi_read_bytes(unsigned count, uint8_t* buf)  {
	for(unsigned i=0; i<count; i++) {
		buf[i] = radio_spi_go(0);
	}
}

unsigned int SX1276M1BxASWrapper::radio_comm_PollCTS(){
//	unsigned timeout=0;
//
//#ifdef SI446X_AGGRESSIVE_CTS
//	if (ctsWentHigh) return 1;
//#else
//	ctsWentHigh = 0;
//#endif
//
//	while(ctsWentHigh == 0 && timeout < CTS_TIMEOUT) {
//		radio_comm_GetResp(0, NULL);
//		timeout++;
//	}
//	if (timeout == CTS_TIMEOUT) {
//		ctsWentHigh = 0;
//		LORA_ASSERT(0, "Fatal: CTS Timeout waiting for response\r\n");
//		return 0;
//	}

	return 1;
}

void SX1276M1BxASWrapper::radio_spi_sel_assert() {
	GPIO_WriteBit(SX1276_pin_setup.cs_port, SX1276_pin_setup.cs_pin, Bit_RESET); // chip select
	__NOP();
}

uint8_t SX1276M1BxASWrapper::radio_comm_GetResp(uint8_t byteCount,
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
//		ctsWentHigh = 1;
	}

	if (byteCount) {
		spi_read_bytes(byteCount, pData);
	}
	radio_spi_sel_no_assert();

	return ctsVal;
}



const FskBandwidth_t SX1276M1BxASWrapper::FskBandwidths[] =
{
    { 2600  , 0x17 },
    { 3100  , 0x0F },
    { 3900  , 0x07 },
    { 5200  , 0x16 },
    { 6300  , 0x0E },
    { 7800  , 0x06 },
    { 10400 , 0x15 },
    { 12500 , 0x0D },
    { 15600 , 0x05 },
    { 20800 , 0x14 },
    { 25000 , 0x0C },
    { 31300 , 0x04 },
    { 41700 , 0x13 },
    { 50000 , 0x0B },
    { 62500 , 0x03 },
    { 83333 , 0x12 },
    { 100000, 0x0A },
    { 125000, 0x02 },
    { 166700, 0x11 },
    { 200000, 0x09 },
    { 250000, 0x01 },
    { 300000, 0x00 }, // Invalid Bandwidth
};


SX1276M1BxASWrapper::SX1276M1BxASWrapper() {


//	RadioRegsInit[0].Modem = RADIO_INIT_REGISTERS_VALUE0;
	Get_SX1276_RADIO_INIT_REGISTERS_VALUE(RadioRegsInit[0],0);

	IoIrqInit();

	InitializeTimers();
}

SX1276M1BxASWrapper::~SX1276M1BxASWrapper() {
}

bool SX1276M1BxASWrapper::CheckRfFrequency(
		uint32_t frequency) {
	return true;
}

void SX1276M1BxASWrapper::Write(uint8_t addr, uint8_t data) {
	 Write( addr, &data, 1 );
}

uint8_t SX1276M1BxASWrapper::Read(uint8_t addr) {
    uint8_t data;
    Read( addr, &data, 1 );
    return data;
}

void SX1276M1BxASWrapper::Write(uint8_t addr,
		uint8_t* buffer, uint8_t size) {
	radio_comm_WriteData(addr | 0x80, 0, size, buffer);

//	addr = addr | 0x80;
//	INT32 readsize = 0;
//	INT32 writesize = (INT32)size;
//
//	if(!CPU_SPI_Xaction_Start(m_spi_config)){
//		return;
//	}
//
//	SPI_XACTION_8 t;
//	{
//	t.Write8 = &addr;
//	t.WriteCount = 1;
//	t.Read8 = NULL;
//	t.ReadCount = 0;
//	t.ReadStartOffset = 0;
//	t.SPI_mod = m_spi_config.SPI_mod;
//	t.BusyPin = m_spi_config.BusyPin;
//	}
//	CPU_SPI_Xaction_nWrite8_nRead8(t);
//	{
//	t.Write8 = &addr;
//	t.WriteCount = size;
//	t.Read8 = NULL;
//	t.ReadCount = 0;
//	t.ReadStartOffset = 0;
//	t.SPI_mod = m_spi_config.SPI_mod;
//	t.BusyPin = m_spi_config.BusyPin;
//	}
//	CPU_SPI_Xaction_nWrite8_nRead8(t);
//	CPU_SPI_Xaction_Stop(m_spi_config);
}



void SX1276M1BxASWrapper::Read(uint8_t addr, uint8_t* buffer,
		uint8_t size) {

	radio_comm_ReadData(addr & 0x7F, 0, size, buffer);

//	addr = addr & 0x7F;
//	INT32 readsize = 0;
//	INT32 writesize = (INT32)size;
//
//	if(!CPU_SPI_Xaction_Start(m_spi_config)){
//		return;
//	}
//
//	SPI_XACTION_8 t;
//	{
//	t.Write8 = &addr;
//	t.WriteCount = 1;
//	t.Read8 = buffer;
//	t.ReadCount = size;
//	t.ReadStartOffset = 1;
//	t.SPI_mod = m_spi_config.SPI_mod;
//	t.BusyPin = m_spi_config.BusyPin;
//	}
//	if(!CPU_SPI_Xaction_nWrite8_nRead8(t)) return;
//
//	CPU_SPI_Xaction_Stop(m_spi_config);

}

void SX1276M1BxASWrapper::WriteFifo(uint8_t* buffer,
		uint8_t size) {
	Write( 0, buffer, size );
}

void SX1276M1BxASWrapper::ReadFifo(uint8_t* buffer,
		uint8_t size) {
	Read( 0, buffer, size );
}



void SX1276M1BxASWrapper::Reset() { //TODO: B
	reset_intiated = true;
	//Write a zero to the reset pin
	//and switch to reading in that pin and wait 6 ms

	CPU_GPIO_EnableOutputPin(SX1276_pin_setup.reset_mf_pin, TRUE);
	VirtTimer_SleepMicroseconds(VIRT_TIMER_SX1276_txTimeout, 10000 );
	CPU_GPIO_SetPinState( SX1276_pin_setup.reset_mf_pin, FALSE );
	VirtTimer_SleepMicroseconds(VIRT_TIMER_SX1276_txTimeout, 10000 );
	CPU_GPIO_EnableOutputPin(SX1276_pin_setup.reset_mf_pin, TRUE);
	VirtTimer_SleepMicroseconds(VIRT_TIMER_SX1276_txTimeout, 10000 );
	CPU_GPIO_SetPinState( SX1276_pin_setup.reset_mf_pin, FALSE );
	VirtTimer_SleepMicroseconds(VIRT_TIMER_SX1276_txTimeout, 10000 );
	//CPU_GPIO_EnableOutputPin(SX1276_pin_setup.reset_mf_pin, TRUE);
	//CPU_GPIO_SetPinState( SX1276_pin_setup.reset_mf_pin, FALSE );
	//VirtTimer_SleepMicroseconds(VIRT_TIMER_SX1276_txTimeout, 1000 );
	//CPU_GPIO_EnableInputPin(SX1276_pin_setup.reset_mf_pin, FALSE, SX1276M1BxASWrapper::SX1276_Reset_Pin_Interrupt_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	//VirtTimer_SleepMicroseconds(VIRT_TIMER_SX1276_txTimeout, 1000 );

	//while(reset_intiated){};

}

void SX1276M1BxASWrapper::IoInit() {
    SX1276M1BxASWrapper::AntSwInit( );
	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , FALSE);
	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , TRUE);
	this->SX1276M1BxASWrapper::SpiInit();
}

void SX1276M1BxASWrapper::RadioRegistersInit() {
    uint8_t i = 0;
    for( i = 0; i < sizeof( RadioRegsInit ) / sizeof( RadioRegisters_t ); i++ )
    {
    	Get_SX1276_RADIO_INIT_REGISTERS_VALUE(RadioRegsInit[i],0);
    	SetModem( RadioRegsInit[i].Modem );
        Write( RadioRegsInit[i].Addr, RadioRegsInit[i].Value );
    }
}

void SX1276M1BxASWrapper::SpiInit(void) {
	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , FALSE);
	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , TRUE);
	SX1276M1BxASWrapper::SpiInitialize();
}


void SX1276M1BxASWrapper::IoIrqInit() {
	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO0, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler0, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	EXTI_ClearITPendingBit(EXTI_Line1);
//	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO1, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler1, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
//	EXTI_ClearITPendingBit(EXTI_Line1);
//	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO2, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler2, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
//	EXTI_ClearITPendingBit(EXTI_Line1);
	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO3, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler3, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	EXTI_ClearITPendingBit(EXTI_Line3);
//	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO4, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler4, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
//	EXTI_ClearITPendingBit(EXTI_Line1);
//	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO5, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler5, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
//	EXTI_ClearITPendingBit(EXTI_Line1);
}

void SX1276M1BxASWrapper::IoDeInit() {
}

void SX1276M1BxASWrapper::SetRfTxPower(int8_t power) {
}

uint8_t SX1276M1BxASWrapper::GetPaSelect(uint32_t channel) {
}

void SX1276M1BxASWrapper::SetAntSwLowPower(bool status) {
}

void SX1276M1BxASWrapper::AntSwInit() {
}

void SX1276M1BxASWrapper::AntSwDeInit() {
}

void SX1276M1BxASWrapper::SetAntSw(uint8_t opMode) {
}


void SX1276M1BxASWrapper::InitializeTimers(){
	bool rv = VirtTimer_Initialize();
	//ASSERT_SP(rv);
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(VIRT_TIMER_SX1276_txTimeout, 0, 10, TRUE, FALSE, SX1276M1BxASWrapper::SX1276_Radio_OnTimeoutIrq);
	rm = VirtTimer_SetTimer(VIRT_TIMER_SX1276_rxTimeout, 0, 10, FALSE, FALSE, SX1276M1BxASWrapper::SX1276_Radio_OnTimeoutIrq );
	rm = VirtTimer_SetTimer(VIRT_TIMER_SX1276_rxTimeoutSyncWord, 0, 10, TRUE, FALSE, SX1276M1BxASWrapper::SX1276_Radio_OnTimeoutIrq);

}


UINT8 SX1276M1BxASWrapper::GetTimerID(TimeoutName_t ton){
	switch (ton){
	case txTimeoutTimer:
		return VIRT_TIMER_SX1276_txTimeout;
		break;
	case rxTimeoutTimer:
		return VIRT_TIMER_SX1276_rxTimeout;
		break;
	case rxTimeoutSyncWord:
		return VIRT_TIMER_SX1276_rxTimeoutSyncWord;
		break;
	default:
		//assert(0);
		return 0;
		break;
	}
	return 0;
}

void SX1276M1BxASWrapper::SetTimeoutTimer(TimeoutName_t ton, float delay) {
	VirtTimer_Stop(GetTimerID(ton));
	VirtTimer_Change(GetTimerID(ton), 0, delay, TRUE);
	VirtTimer_Start(GetTimerID(ton));
}
void SX1276M1BxASWrapper::CancelTimeoutTimer(TimeoutName_t ton) {
	VirtTimer_Stop(GetTimerID(ton));
}


void SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler0(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	g_SX1276M1BxASWrapper.OnDio0Irq();
}
void SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler1(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	g_SX1276M1BxASWrapper.OnDio1Irq();
}
void SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler2(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	g_SX1276M1BxASWrapper.OnDio2Irq();
}
void SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler3(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	g_SX1276M1BxASWrapper.OnDio3Irq();
}
void SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler4(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	g_SX1276M1BxASWrapper.OnDio4Irq();
}
void SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler5(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	g_SX1276M1BxASWrapper.OnDio5Irq();
}

void SX1276M1BxASWrapper::SX1276_Radio_OnTimeoutIrq(void* param) {
	g_SX1276M1BxASWrapper.OnTimeoutIrq();
}

void SX1276M1BxASWrapper::SX1276_Reset_Pin_Interrupt_Handler(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	g_SX1276M1BxASWrapper.reset_intiated = false;
}

void SX1276M1BxASWrapper::Initialize(SX1276RadioEvents_t *events) {
	rxtxBuffer = &(rxtxBufferstorage[0]);
	InitializeTimers();

	SX1276M1BxASWrapper::LoraHardwareConfigInitialize();
	//this->SX1276M1BxASWrapper::Reset();
	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , FALSE);
	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , TRUE);

	this->SX1276M1BxASWrapper::IoInit( );

	RxChainCalibration();
	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , FALSE);
	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , TRUE);




	SX1276M1BxASWrapper::SetOpMode( RF_OPMODE_SLEEP );
	SX1276M1BxASWrapper::IoIrqInit();
	SX1276M1BxASWrapper::RadioRegistersInit();


	Init(events);
    SetModem( MODEM_FSK );

    //SanityCheck
    uint8_t ver;
    Read(0x42, &ver, 1);
    if (ver != 0x12) {
    	while(1){
    		__asm__("BKPT"); // Something is terribly wrong. TODO: DELETE ME. SANITY CHECK FOR DEBUG.
    		hal_printf("SX1276M1BxASWrapper::Initialize: Something went terribly wrong in SPI Initialization\n\r");
    	}
    }

    this->settings.State = RF_IDLE ;

}


void SX1276M1BxASWrapper::Init( SX1276RadioEvents_t *events )
{
    this->RadioEvents = events;
}

RadioState SX1276M1BxASWrapper::GetStatus(  )
{
    return this->settings.State;
}

void SX1276M1BxASWrapper::SetChannel( uint32_t freq )
{
    this->settings.Channel = freq;
    freq = ( uint32_t )( ( double )freq / ( double )FREQ_STEP );
    Write( REG_FRFMSB, ( uint8_t )( ( freq >> 16 ) & 0xFF ) );
    Write( REG_FRFMID, ( uint8_t )( ( freq >> 8 ) & 0xFF ) );
    Write( REG_FRFLSB, ( uint8_t )( freq & 0xFF ) );
}

bool SX1276M1BxASWrapper::IsChannelFree( RadioModems_t modem, uint32_t freq, int16_t rssiThresh )
{
    int16_t rssi = 0;

    SetModem( modem );

    SetChannel( freq );

    SetOpMode( RF_OPMODE_RECEIVER );

    wait_ms( 1 );

    rssi = GetRssi( modem );

    Sleep( );

    if( rssi > rssiThresh )
    {
        return false;
    }
    return true;
}

uint32_t SX1276M1BxASWrapper::Random(  )
{
    uint8_t i;
    uint32_t rnd = 0;

    /*
     * Radio setup for random number generation
     */
    // Set LoRa modem ON
    SetModem( MODEM_LORA );

    // Disable LoRa modem interrupts
    Write( REG_LR_IRQFLAGSMASK, RFLR_IRQFLAGS_RXTIMEOUT |
                  RFLR_IRQFLAGS_RXDONE |
                  RFLR_IRQFLAGS_PAYLOADCRCERROR |
                  RFLR_IRQFLAGS_VALIDHEADER |
                  RFLR_IRQFLAGS_TXDONE |
                  RFLR_IRQFLAGS_CADDONE |
                  RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                  RFLR_IRQFLAGS_CADDETECTED );

    // Set radio in continuous reception
    SetOpMode( RF_OPMODE_RECEIVER );

    for( i = 0; i < 32; i++ )
    {
        wait_ms( 1 );
        // Unfiltered RSSI value reading. Only takes the LSB value
        rnd |= ( ( uint32_t )Read( REG_LR_RSSIWIDEBAND ) & 0x01 ) << i;
    }

    Sleep( );

    return rnd;
}

/*!
 * Performs the Rx chain calibration for LF and HF bands
 * \remark Must be called just after the reset so all registers are at their
 *         default values
 */
void SX1276M1BxASWrapper::RxChainCalibration( )
{
    uint8_t regPaConfigInitVal;
    uint32_t initialFreq;

	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , FALSE);
	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , TRUE);


    // Save context
    regPaConfigInitVal = this->Read( REG_PACONFIG );

	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , FALSE);
	CPU_GPIO_SetPinState( SX1276M1BxASWrapper_debug_PIN , TRUE);


    initialFreq = ( double )( ( ( uint32_t )this->Read( REG_FRFMSB ) << 16 ) |
                              ( ( uint32_t )this->Read( REG_FRFMID ) << 8 ) |
                              ( ( uint32_t )this->Read( REG_FRFLSB ) ) ) * ( double )FREQ_STEP;

    // Cut the PA just in case, RFO output, power = -1 dBm
    this->Write( REG_PACONFIG, 0x00 );

    // Launch Rx chain calibration for LF band
    Write ( REG_IMAGECAL, ( Read( REG_IMAGECAL ) & RF_IMAGECAL_IMAGECAL_MASK ) | RF_IMAGECAL_IMAGECAL_START );
    while( ( Read( REG_IMAGECAL ) & RF_IMAGECAL_IMAGECAL_RUNNING ) == RF_IMAGECAL_IMAGECAL_RUNNING )
    {
    }

    // Sets a Frequency in HF band
    SetChannel( 868000000 );

    // Launch Rx chain calibration for HF band
    Write ( REG_IMAGECAL, ( Read( REG_IMAGECAL ) & RF_IMAGECAL_IMAGECAL_MASK ) | RF_IMAGECAL_IMAGECAL_START );
    while( ( Read( REG_IMAGECAL ) & RF_IMAGECAL_IMAGECAL_RUNNING ) == RF_IMAGECAL_IMAGECAL_RUNNING )
    {
    }

    // Restore context
    this->Write( REG_PACONFIG, regPaConfigInitVal );
    SetChannel( initialFreq );
}

/*!
 * Returns the known FSK bandwidth registers value
 *
 * \param [IN] bandwidth Bandwidth value in Hz
 * \retval regValue Bandwidth register value.
 */
uint8_t SX1276M1BxASWrapper::GetFskBandwidthRegValue( uint32_t bandwidth )
{
    uint8_t i;

    for( i = 0; i < ( sizeof( FskBandwidths ) / sizeof( FskBandwidth_t ) ) - 1; i++ )
    {
        if( ( bandwidth >= FskBandwidths[i].bandwidth ) && ( bandwidth < FskBandwidths[i + 1].bandwidth ) )
        {
            return FskBandwidths[i].RegValue;
        }
    }
    // ERROR: Value not found
    while( 1 );
}

void SX1276M1BxASWrapper::SetRxConfig( RadioModems_t modem, uint32_t bandwidth,
                         uint32_t datarate, uint8_t coderate,
                         uint32_t bandwidthAfc, uint16_t preambleLen,
                         uint16_t symbTimeout, bool fixLen,
                         uint8_t payloadLen,
                         bool crcOn, bool freqHopOn, uint8_t hopPeriod,
                         bool iqInverted, bool rxContinuous )
{
    SetModem( modem );

    switch( modem )
    {
    case MODEM_FSK:
        {
            this->settings.Fsk.Bandwidth = bandwidth;
            this->settings.Fsk.Datarate = datarate;
            this->settings.Fsk.BandwidthAfc = bandwidthAfc;
            this->settings.Fsk.FixLen = fixLen;
            this->settings.Fsk.PayloadLen = payloadLen;
            this->settings.Fsk.CrcOn = crcOn;
            this->settings.Fsk.IqInverted = iqInverted;
            this->settings.Fsk.RxContinuous = rxContinuous;
            this->settings.Fsk.PreambleLen = preambleLen;
            this->settings.Fsk.RxSingleTimeout = symbTimeout * ( ( 1.0 / ( double )datarate ) * 8.0 ) * 1e3;

            datarate = ( uint16_t )( ( double )XTAL_FREQ / ( double )datarate );
            Write( REG_BITRATEMSB, ( uint8_t )( datarate >> 8 ) );
            Write( REG_BITRATELSB, ( uint8_t )( datarate & 0xFF ) );

            Write( REG_RXBW, GetFskBandwidthRegValue( bandwidth ) );
            Write( REG_AFCBW, GetFskBandwidthRegValue( bandwidthAfc ) );

            Write( REG_PREAMBLEMSB, ( uint8_t )( ( preambleLen >> 8 ) & 0xFF ) );
            Write( REG_PREAMBLELSB, ( uint8_t )( preambleLen & 0xFF ) );

            if( fixLen == 1 )
            {
                Write( REG_PAYLOADLENGTH, payloadLen );
            }
            else
            {
                Write( REG_PAYLOADLENGTH, 0xFF ); // Set payload length to the maximum
            }

            Write( REG_PACKETCONFIG1,
                         ( Read( REG_PACKETCONFIG1 ) &
                           RF_PACKETCONFIG1_CRC_MASK &
                           RF_PACKETCONFIG1_PACKETFORMAT_MASK ) |
                           ( ( fixLen == 1 ) ? RF_PACKETCONFIG1_PACKETFORMAT_FIXED : RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE ) |
                           ( crcOn << 4 ) );
            Write( REG_PACKETCONFIG2, ( Read( REG_PACKETCONFIG2 ) | RF_PACKETCONFIG2_DATAMODE_PACKET ) );
        }
        break;
    case MODEM_LORA:
        {
            if( bandwidth > 2 )
            {
                // Fatal error: When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
                while( 1 );
            }
            bandwidth += 7;
            this->settings.LoRa.Bandwidth = bandwidth;
            this->settings.LoRa.Datarate = datarate;
            this->settings.LoRa.Coderate = coderate;
            this->settings.LoRa.PreambleLen = preambleLen;
            this->settings.LoRa.FixLen = fixLen;
            this->settings.LoRa.PayloadLen = payloadLen;
            this->settings.LoRa.CrcOn = crcOn;
            this->settings.LoRa.FreqHopOn = freqHopOn;
            this->settings.LoRa.HopPeriod = hopPeriod;
            this->settings.LoRa.IqInverted = iqInverted;
            this->settings.LoRa.RxContinuous = rxContinuous;

            if( datarate > 12 )
            {
                datarate = 12;
            }
            else if( datarate < 6 )
            {
                datarate = 6;
            }

            if( ( ( bandwidth == 7 ) && ( ( datarate == 11 ) || ( datarate == 12 ) ) ) ||
                ( ( bandwidth == 8 ) && ( datarate == 12 ) ) )
            {
                this->settings.LoRa.LowDatarateOptimize = 0x01;
            }
            else
            {
                this->settings.LoRa.LowDatarateOptimize = 0x00;
            }

            Write( REG_LR_MODEMCONFIG1,
                         ( Read( REG_LR_MODEMCONFIG1 ) &
                           RFLR_MODEMCONFIG1_BW_MASK &
                           RFLR_MODEMCONFIG1_CODINGRATE_MASK &
                           RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) |
                           ( bandwidth << 4 ) | ( coderate << 1 ) |
                           fixLen );

            Write( REG_LR_MODEMCONFIG2,
                         ( Read( REG_LR_MODEMCONFIG2 ) &
                           RFLR_MODEMCONFIG2_SF_MASK &
                           RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK &
                           RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) |
                           ( datarate << 4 ) | ( crcOn << 2 ) |
                           ( ( symbTimeout >> 8 ) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) );

            Write( REG_LR_MODEMCONFIG3,
                         ( Read( REG_LR_MODEMCONFIG3 ) &
                           RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK ) |
                           ( this->settings.LoRa.LowDatarateOptimize << 3 ) );

            Write( REG_LR_SYMBTIMEOUTLSB, ( uint8_t )( symbTimeout & 0xFF ) );

            Write( REG_LR_PREAMBLEMSB, ( uint8_t )( ( preambleLen >> 8 ) & 0xFF ) );
            Write( REG_LR_PREAMBLELSB, ( uint8_t )( preambleLen & 0xFF ) );

            if( fixLen == 1 )
            {
                Write( REG_LR_PAYLOADLENGTH, payloadLen );
            }

            if( this->settings.LoRa.FreqHopOn == true )
            {
                Write( REG_LR_PLLHOP, ( Read( REG_LR_PLLHOP ) & RFLR_PLLHOP_FASTHOP_MASK ) | RFLR_PLLHOP_FASTHOP_ON );
                Write( REG_LR_HOPPERIOD, this->settings.LoRa.HopPeriod );
            }

            if( ( bandwidth == 9 ) && ( this->settings.Channel > RF_MID_BAND_THRESH ) )
            {
                // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
                Write( REG_LR_TEST36, 0x02 );
                Write( REG_LR_TEST3A, 0x64 );
            }
            else if( bandwidth == 9 )
            {
                // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
                Write( REG_LR_TEST36, 0x02 );
                Write( REG_LR_TEST3A, 0x7F );
            }
            else
            {
                // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
                Write( REG_LR_TEST36, 0x03 );
            }

            if( datarate == 6 )
            {
                Write( REG_LR_DETECTOPTIMIZE,
                             ( Read( REG_LR_DETECTOPTIMIZE ) &
                               RFLR_DETECTIONOPTIMIZE_MASK ) |
                               RFLR_DETECTIONOPTIMIZE_SF6 );
                Write( REG_LR_DETECTIONTHRESHOLD,
                             RFLR_DETECTIONTHRESH_SF6 );
            }
            else
            {
                Write( REG_LR_DETECTOPTIMIZE,
                             ( Read( REG_LR_DETECTOPTIMIZE ) &
                             RFLR_DETECTIONOPTIMIZE_MASK ) |
                             RFLR_DETECTIONOPTIMIZE_SF7_TO_SF12 );
                Write( REG_LR_DETECTIONTHRESHOLD,
                             RFLR_DETECTIONTHRESH_SF7_TO_SF12 );
            }
        }
        break;
    }
}

void SX1276M1BxASWrapper::SetTxConfig( RadioModems_t modem, int8_t power, uint32_t fdev,
                        uint32_t bandwidth, uint32_t datarate,
                        uint8_t coderate, uint16_t preambleLen,
                        bool fixLen, bool crcOn, bool freqHopOn,
                        uint8_t hopPeriod, bool iqInverted, uint32_t timeout )
{
    SetModem( modem );

    SetRfTxPower( power );

    switch( modem )
    {
    case MODEM_FSK:
        {
            this->settings.Fsk.Power = power;
            this->settings.Fsk.Fdev = fdev;
            this->settings.Fsk.Bandwidth = bandwidth;
            this->settings.Fsk.Datarate = datarate;
            this->settings.Fsk.PreambleLen = preambleLen;
            this->settings.Fsk.FixLen = fixLen;
            this->settings.Fsk.CrcOn = crcOn;
            this->settings.Fsk.IqInverted = iqInverted;
            this->settings.Fsk.TxTimeout = timeout;

            fdev = ( uint16_t )( ( double )fdev / ( double )FREQ_STEP );
            Write( REG_FDEVMSB, ( uint8_t )( fdev >> 8 ) );
            Write( REG_FDEVLSB, ( uint8_t )( fdev & 0xFF ) );

            datarate = ( uint16_t )( ( double )XTAL_FREQ / ( double )datarate );
            Write( REG_BITRATEMSB, ( uint8_t )( datarate >> 8 ) );
            Write( REG_BITRATELSB, ( uint8_t )( datarate & 0xFF ) );

            Write( REG_PREAMBLEMSB, ( preambleLen >> 8 ) & 0x00FF );
            Write( REG_PREAMBLELSB, preambleLen & 0xFF );

            Write( REG_PACKETCONFIG1,
                         ( Read( REG_PACKETCONFIG1 ) &
                           RF_PACKETCONFIG1_CRC_MASK &
                           RF_PACKETCONFIG1_PACKETFORMAT_MASK ) |
                           ( ( fixLen == 1 ) ? RF_PACKETCONFIG1_PACKETFORMAT_FIXED : RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE ) |
                           ( crcOn << 4 ) );
            Write( REG_PACKETCONFIG2, ( Read( REG_PACKETCONFIG2 ) | RF_PACKETCONFIG2_DATAMODE_PACKET ) );
        }
        break;
    case MODEM_LORA:
        {
            this->settings.LoRa.Power = power;
            if( bandwidth > 2 )
            {
                // Fatal error: When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
                while( 1 );
            }
            bandwidth += 7;
            this->settings.LoRa.Bandwidth = bandwidth;
            this->settings.LoRa.Datarate = datarate;
            this->settings.LoRa.Coderate = coderate;
            this->settings.LoRa.PreambleLen = preambleLen;
            this->settings.LoRa.FixLen = fixLen;
            this->settings.LoRa.FreqHopOn = freqHopOn;
            this->settings.LoRa.HopPeriod = hopPeriod;
            this->settings.LoRa.CrcOn = crcOn;
            this->settings.LoRa.IqInverted = iqInverted;
            this->settings.LoRa.TxTimeout = timeout;

            if( datarate > 12 )
            {
                datarate = 12;
            }
            else if( datarate < 6 )
            {
                datarate = 6;
            }
            if( ( ( bandwidth == 7 ) && ( ( datarate == 11 ) || ( datarate == 12 ) ) ) ||
                ( ( bandwidth == 8 ) && ( datarate == 12 ) ) )
            {
                this->settings.LoRa.LowDatarateOptimize = 0x01;
            }
            else
            {
                this->settings.LoRa.LowDatarateOptimize = 0x00;
            }

            if( this->settings.LoRa.FreqHopOn == true )
            {
                Write( REG_LR_PLLHOP, ( Read( REG_LR_PLLHOP ) & RFLR_PLLHOP_FASTHOP_MASK ) | RFLR_PLLHOP_FASTHOP_ON );
                Write( REG_LR_HOPPERIOD, this->settings.LoRa.HopPeriod );
            }

            Write( REG_LR_MODEMCONFIG1,
                         ( Read( REG_LR_MODEMCONFIG1 ) &
                           RFLR_MODEMCONFIG1_BW_MASK &
                           RFLR_MODEMCONFIG1_CODINGRATE_MASK &
                           RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) |
                           ( bandwidth << 4 ) | ( coderate << 1 ) |
                           fixLen );

            Write( REG_LR_MODEMCONFIG2,
                         ( Read( REG_LR_MODEMCONFIG2 ) &
                           RFLR_MODEMCONFIG2_SF_MASK &
                           RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK ) |
                           ( datarate << 4 ) | ( crcOn << 2 ) );

            Write( REG_LR_MODEMCONFIG3,
                         ( Read( REG_LR_MODEMCONFIG3 ) &
                           RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK ) |
                           ( this->settings.LoRa.LowDatarateOptimize << 3 ) );

            Write( REG_LR_PREAMBLEMSB, ( preambleLen >> 8 ) & 0x00FF );
            Write( REG_LR_PREAMBLELSB, preambleLen & 0xFF );

            if( datarate == 6 )
            {
                Write( REG_LR_DETECTOPTIMIZE,
                             ( Read( REG_LR_DETECTOPTIMIZE ) &
                               RFLR_DETECTIONOPTIMIZE_MASK ) |
                               RFLR_DETECTIONOPTIMIZE_SF6 );
                Write( REG_LR_DETECTIONTHRESHOLD,
                             RFLR_DETECTIONTHRESH_SF6 );
            }
            else
            {
                Write( REG_LR_DETECTOPTIMIZE,
                             ( Read( REG_LR_DETECTOPTIMIZE ) &
                             RFLR_DETECTIONOPTIMIZE_MASK ) |
                             RFLR_DETECTIONOPTIMIZE_SF7_TO_SF12 );
                Write( REG_LR_DETECTIONTHRESHOLD,
                             RFLR_DETECTIONTHRESH_SF7_TO_SF12 );
            }
        }
        break;
    }
}

uint32_t SX1276M1BxASWrapper::TimeOnAir( RadioModems_t modem, uint8_t pktLen )
{
    uint32_t airTime = 0;

    switch( modem )
    {
    case MODEM_FSK:
        {
            airTime = rint( ( 8 * ( this->settings.Fsk.PreambleLen +
                                     ( ( Read( REG_SYNCCONFIG ) & ~RF_SYNCCONFIG_SYNCSIZE_MASK ) + 1 ) +
                                     ( ( this->settings.Fsk.FixLen == 0x01 ) ? 0.0 : 1.0 ) +
                                     ( ( ( Read( REG_PACKETCONFIG1 ) & ~RF_PACKETCONFIG1_ADDRSFILTERING_MASK ) != 0x00 ) ? 1.0 : 0 ) +
                                     pktLen +
                                     ( ( this->settings.Fsk.CrcOn == 0x01 ) ? 2.0 : 0 ) ) /
                                     this->settings.Fsk.Datarate ) * 1e3 );
        }
        break;
    case MODEM_LORA:
        {
            double bw = 0.0;
            // REMARK: When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
            switch( this->settings.LoRa.Bandwidth )
            {
            //case 0: // 7.8 kHz
            //    bw = 78e2;
            //    break;
            //case 1: // 10.4 kHz
            //    bw = 104e2;
            //    break;
            //case 2: // 15.6 kHz
            //    bw = 156e2;
            //    break;
            //case 3: // 20.8 kHz
            //    bw = 208e2;
            //    break;
            //case 4: // 31.2 kHz
            //    bw = 312e2;
            //    break;
            //case 5: // 41.4 kHz
            //    bw = 414e2;
            //    break;
            //case 6: // 62.5 kHz
            //    bw = 625e2;
            //    break;
            case 7: // 125 kHz
                bw = 125e3;
                break;
            case 8: // 250 kHz
                bw = 250e3;
                break;
            case 9: // 500 kHz
                bw = 500e3;
                break;
            }

            // Symbol rate : time for one symbol (secs)
            double rs = bw / ( 1 << this->settings.LoRa.Datarate );
            double ts = 1 / rs;
            // time of preamble
            double tPreamble = ( this->settings.LoRa.PreambleLen + 4.25 ) * ts;
            // Symbol length of payload and time
            double tmp = ceil( ( 8 * pktLen - 4 * this->settings.LoRa.Datarate +
                                 28 + 16 * this->settings.LoRa.CrcOn -
                                 ( this->settings.LoRa.FixLen ? 20 : 0 ) ) /
                                 ( double )( 4 * ( this->settings.LoRa.Datarate -
                                 ( ( this->settings.LoRa.LowDatarateOptimize > 0 ) ? 2 : 0 ) ) ) ) *
                                 ( this->settings.LoRa.Coderate + 4 );
            double nPayload = 8 + ( ( tmp > 0 ) ? tmp : 0 );
            double tPayload = nPayload * ts;
            // Time on air
            double tOnAir = tPreamble + tPayload;
            // return ms secs
            airTime = floor( tOnAir * 1e3 + 0.999 );
        }
        break;
    }
    return airTime;
}

bool SX1276M1BxASWrapper::Send( uint8_t *buffer, uint8_t size )
{
	return SendTS(buffer,size,0);
/*
	uint32_t txTimeout = 0;
    switch( this->settings.Modem )
    {
    case MODEM_FSK:
        {
            this->settings.FskPacketHandler.NbBytes = 0;
            this->settings.FskPacketHandler.Size = size;

            if( this->settings.Fsk.FixLen == false )
            {
                WriteFifo( ( uint8_t* )&size, 1 );
            }
            else
            {
                Write( REG_PAYLOADLENGTH, size );
            }

            if( ( size > 0 ) && ( size <= 64 ) )
            {
                this->settings.FskPacketHandler.ChunkSize = size;
            }
            else
            {
                for(uint8_t i = 0; i < size; ++i){
                	rxtxBufferstorage[i] = *(buffer + i);
                };
//                memcpy( rxtxBuffer, buffer, size );
                this->settings.FskPacketHandler.ChunkSize = 32;
            }

            // Write payload buffer
            WriteFifo( buffer, this->settings.FskPacketHandler.ChunkSize );
            this->settings.FskPacketHandler.NbBytes += this->settings.FskPacketHandler.ChunkSize;
            txTimeout = this->settings.Fsk.TxTimeout;
        }
        break;
    case MODEM_LORA:
        {
            if( this->settings.LoRa.IqInverted == true )
            {
                Write( REG_LR_INVERTIQ, ( ( Read( REG_LR_INVERTIQ ) & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK ) | RFLR_INVERTIQ_RX_OFF | RFLR_INVERTIQ_TX_ON ) );
                Write( REG_LR_INVERTIQ2, RFLR_INVERTIQ2_ON );
            }
            else
            {
                Write( REG_LR_INVERTIQ, ( ( Read( REG_LR_INVERTIQ ) & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK ) | RFLR_INVERTIQ_RX_OFF | RFLR_INVERTIQ_TX_OFF ) );
                Write( REG_LR_INVERTIQ2, RFLR_INVERTIQ2_OFF );
            }

            this->settings.LoRaPacketHandler.Size = size;

            // Initializes the payload size
            Write( REG_LR_PAYLOADLENGTH, size );

            // Full buffer used for Tx
            Write( REG_LR_FIFOTXBASEADDR, 0 );
            Write( REG_LR_FIFOADDRPTR, 0 );

            // FIFO operations can not take place in Sleep mode
            uint8_t mode= READ_OPMODE();
            switch (mode){
				case RF_OPMODE_SLEEP:
					do {
						Standby( );
						wait_ms( 1 );
						mode= READ_OPMODE();
					}while(mode == RF_OPMODE_SLEEP);
					break;
				case RF_OPMODE_RECEIVER:
					hal_printf("SX1276:Send: Error: In receive mode, cant send now \n \r ");
					//return;
				default:
					hal_printf("SX1276: Send: Not sure what mode i am in \n \r ");
            }
            // Write payload buffer
            WriteFifo( buffer, size );
            txTimeout = this->settings.LoRa.TxTimeout;
        }
        break;
    }
    Tx( txTimeout );*/
}

bool SX1276M1BxASWrapper::SendTS( uint8_t *buffer, uint8_t size ,  UINT32 eventTime) //BK:Artificial injection due to compliance with mf_radio. TODO:The interface should be changed as it breaks layering.
{
    uint32_t txTimeout = 0;

    switch( this->settings.Modem )
    {
    case MODEM_FSK:
        {
            this->settings.FskPacketHandler.NbBytes = 0;
            this->settings.FskPacketHandler.Size = size+4;

            if( this->settings.Fsk.FixLen == false )
            {
                WriteFifo( ( uint8_t* )&size+4, 1 );
            }
            else
            {
                Write( REG_PAYLOADLENGTH, size+4 );
            }

            if( ( size > 0 ) && ( size <= (64-4) ) )
            {
                this->settings.FskPacketHandler.ChunkSize = size+4;
                for(uint8_t i = 0; i < size; ++i){
                	rxtxBufferstorage[i] = *(buffer + i);
                };
                UINT32 timestamp = HAL_Time_CurrentTicks() & 0xFFFFFFFF; // Lower bits only
                UINT32 eventOffset = timestamp - eventTime;
                memcpy(static_cast<void*>(&(rxtxBufferstorage[size])), static_cast<void*>(&eventOffset), sizeof(UINT32) );
            }
            else
            {
                for(uint8_t i = 0; i < size; ++i){
                	rxtxBufferstorage[i] = *(buffer + i);
                };
                UINT32 timestamp = HAL_Time_CurrentTicks() & 0xFFFFFFFF; // Lower bits only
                UINT32 eventOffset = timestamp - eventTime;
                memcpy(static_cast<void*>(&(rxtxBufferstorage[size])), static_cast<void*>(&eventOffset), sizeof(UINT32) );
                //for(uint8_t i = size; i < size+4; ++i){
				//	rxtxBufferstorage[i] = *(buffer + i);
				//};

            }

            // Write payload buffer
            //WriteFifo( buffer, this->settings.FskPacketHandler.ChunkSize );
            WriteFifo( rxtxBufferstorage, this->settings.FskPacketHandler.ChunkSize );
            this->settings.FskPacketHandler.NbBytes += this->settings.FskPacketHandler.ChunkSize;

            //CPU_GPIO_SetPinState( RF231_TX_TIMESTAMP, TRUE );
            //UINT32 timestamp = HAL_Time_CurrentTicks() & 0xFFFFFFFF; // Lower bits only
            //UINT32 eventOffset = timestamp - eventTime;
        	//WriteFifo( buffer, eventOffset );

            txTimeout = this->settings.Fsk.TxTimeout;
        }
        break;
    case MODEM_LORA:
        {
            if( this->settings.LoRa.IqInverted == true )
            {
                Write( REG_LR_INVERTIQ, ( ( Read( REG_LR_INVERTIQ ) & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK ) | RFLR_INVERTIQ_RX_OFF | RFLR_INVERTIQ_TX_ON ) );
                Write( REG_LR_INVERTIQ2, RFLR_INVERTIQ2_ON );
            }
            else
            {
                Write( REG_LR_INVERTIQ, ( ( Read( REG_LR_INVERTIQ ) & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK ) | RFLR_INVERTIQ_RX_OFF | RFLR_INVERTIQ_TX_OFF ) );
                Write( REG_LR_INVERTIQ2, RFLR_INVERTIQ2_OFF );
            }

            this->settings.LoRaPacketHandler.Size = size+4;

            // Initializes the payload size
            Write( REG_LR_PAYLOADLENGTH, this->settings.LoRaPacketHandler.Size );

            // Full buffer used for Tx
            Write( REG_LR_FIFOTXBASEADDR, 0 );
            Write( REG_LR_FIFOADDRPTR, 0 );

            // FIFO operations can not take place in Sleep mode
            uint8_t mode= READ_OPMODE();
            switch (mode){
            	case RFLR_OPMODE_STANDBY:
            		break;
				case RFLR_OPMODE_SLEEP:
					do {
						Standby( );
						wait_ms( 1 );
						mode= READ_OPMODE();
					}while(mode == RFLR_OPMODE_SLEEP);
					break;
				case RFLR_OPMODE_RECEIVER:
				case RFLR_OPMODE_RECEIVER_SINGLE:
					//hal_printf("SX1276: SendTS: Error: In receive mode, cant send now \n \r ");
					break;
				case RFLR_OPMODE_SYNTHESIZER_TX:
				case RFLR_OPMODE_TRANSMITTER:
					hal_printf("SX1276: SendTS: Error: Already transmitting something , cant send now \n \r ");
					return false;
				default:
					hal_printf("SX1276: SendTS: Not sure what mode radio in: %d \n \r ", mode);
            }

            //Do timestamping if a eventime is given
            if(eventTime>0){
				//CPU_GPIO_SetPinState( RF231_TX_TIMESTAMP, TRUE );
				UINT32 timestamp = HAL_Time_CurrentTicks() & 0xFFFFFFFF; // Lower bits only
				UINT32 eventOffset = timestamp - eventTime;
				memcpy(buffer+size, static_cast<void*>(&eventOffset), sizeof(UINT32) );
				//WriteFifo( reinterpret_cast<uint8_t*>(eventOffset), 4 );
				size = size+4;
				// Write payload buffer
				//hal_printf("SX1276: SendTS: Sending with timestamp: %d \n \r ", eventTime);
            }
            WriteFifo( buffer, size );
            txTimeout = this->settings.LoRa.TxTimeout;
        }
        break;
    }

    Tx( txTimeout );
    return true;
}


void SX1276M1BxASWrapper::Sleep(  )
{
    //txTimeoutTimer.detach( );
 CancelTimeoutTimer(txTimeoutTimer);
    //rxTimeoutTimer.detach( );
 CancelTimeoutTimer(rxTimeoutTimer);

    SetOpMode( RF_OPMODE_SLEEP );
    this->settings.State = RF_IDLE;
}

void SX1276M1BxASWrapper::Standby(  )
{
    //txTimeoutTimer.detach( );
 CancelTimeoutTimer(txTimeoutTimer);
    //rxTimeoutTimer.detach( );
 CancelTimeoutTimer(rxTimeoutTimer);

    SetOpMode( RF_OPMODE_STANDBY );
    this->settings.State = RF_IDLE;
}

void SX1276M1BxASWrapper::Rx( uint32_t timeout )
{
    bool rxContinuous = false;

    //hal_printf("SX1276M1BxASWrapper::Rx:: Called with timeout %u ========\n", timeout);
    switch( this->settings.Modem )
    {
    case MODEM_FSK:
        {
            rxContinuous = this->settings.Fsk.RxContinuous;

            // DIO0=PayloadReady
            // DIO1=FifoLevel
            // DIO2=SyncAddr
            // DIO3=FifoEmpty
            // DIO4=Preamble
            // DIO5=ModeReady
            Write( REG_DIOMAPPING1, ( Read( REG_DIOMAPPING1 ) & RF_DIOMAPPING1_DIO0_MASK &
                                                                            RF_DIOMAPPING1_DIO1_MASK &
                                                                            RF_DIOMAPPING1_DIO2_MASK ) |
                                                                            RF_DIOMAPPING1_DIO0_00 |
                                                                            RF_DIOMAPPING1_DIO1_00 |
                                                                            RF_DIOMAPPING1_DIO2_11 );

            Write( REG_DIOMAPPING2, ( Read( REG_DIOMAPPING2 ) & RF_DIOMAPPING2_DIO4_MASK &
                                                                            RF_DIOMAPPING2_MAP_MASK ) |
                                                                            RF_DIOMAPPING2_DIO4_11 |
                                                                            RF_DIOMAPPING2_MAP_PREAMBLEDETECT );

            this->settings.FskPacketHandler.FifoThresh = Read( REG_FIFOTHRESH ) & 0x3F;

            Write( REG_RXCONFIG, RF_RXCONFIG_AFCAUTO_ON | RF_RXCONFIG_AGCAUTO_ON | RF_RXCONFIG_RXTRIGER_PREAMBLEDETECT );

            this->settings.FskPacketHandler.PreambleDetected = false;
            this->settings.FskPacketHandler.SyncWordDetected = false;
            this->settings.FskPacketHandler.NbBytes = 0;
            this->settings.FskPacketHandler.Size = 0;
        }
        break;
    case MODEM_LORA:
        {
            if( this->settings.LoRa.IqInverted == true )
            {
                Write( REG_LR_INVERTIQ, ( ( Read( REG_LR_INVERTIQ ) & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK ) | RFLR_INVERTIQ_RX_ON | RFLR_INVERTIQ_TX_OFF ) );
                Write( REG_LR_INVERTIQ2, RFLR_INVERTIQ2_ON );
            }
            else
            {
                Write( REG_LR_INVERTIQ, ( ( Read( REG_LR_INVERTIQ ) & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK ) | RFLR_INVERTIQ_RX_OFF | RFLR_INVERTIQ_TX_OFF ) );
                Write( REG_LR_INVERTIQ2, RFLR_INVERTIQ2_OFF );
            }

            // ERRATA 2.3 - Receiver Spurious Reception of a LoRa Signal
            if( this->settings.LoRa.Bandwidth < 9 )
            {
                Write( REG_LR_DETECTOPTIMIZE, Read( REG_LR_DETECTOPTIMIZE ) & 0x7F );
                Write( REG_LR_TEST30, 0x00 );
                switch( this->settings.LoRa.Bandwidth )
                {
                case 0: // 7.8 kHz
                    Write( REG_LR_TEST2F, 0x48 );
                    SetChannel(this->settings.Channel + 7.81e3 );
                    break;
                case 1: // 10.4 kHz
                    Write( REG_LR_TEST2F, 0x44 );
                    SetChannel(this->settings.Channel + 10.42e3 );
                    break;
                case 2: // 15.6 kHz
                    Write( REG_LR_TEST2F, 0x44 );
                    SetChannel(this->settings.Channel + 15.62e3 );
                    break;
                case 3: // 20.8 kHz
                    Write( REG_LR_TEST2F, 0x44 );
                    SetChannel(this->settings.Channel + 20.83e3 );
                    break;
                case 4: // 31.2 kHz
                    Write( REG_LR_TEST2F, 0x44 );
                    SetChannel(this->settings.Channel + 31.25e3 );
                    break;
                case 5: // 41.4 kHz
                    Write( REG_LR_TEST2F, 0x44 );
                    SetChannel(this->settings.Channel + 41.67e3 );
                    break;
                case 6: // 62.5 kHz
                    Write( REG_LR_TEST2F, 0x40 );
                    break;
                case 7: // 125 kHz
                    Write( REG_LR_TEST2F, 0x40 );
                    break;
                case 8: // 250 kHz
                    Write( REG_LR_TEST2F, 0x40 );
                    break;
                }
            }
            else
            {
                Write( REG_LR_DETECTOPTIMIZE, Read( REG_LR_DETECTOPTIMIZE ) | 0x80 );
            }

            rxContinuous = this->settings.LoRa.RxContinuous;

            if( this->settings.LoRa.FreqHopOn == true )
            {
                Write( REG_LR_IRQFLAGSMASK, //RFLR_IRQFLAGS_RXTIMEOUT |
                                                  //RFLR_IRQFLAGS_RXDONE |
                                                  //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                                  //RFLR_IRQFLAGS_VALIDHEADER |
                                                  RFLR_IRQFLAGS_TXDONE |
                                                  RFLR_IRQFLAGS_CADDONE |
                                                  //RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                                  RFLR_IRQFLAGS_CADDETECTED );

                // DIO0=RxDone, DIO2=FhssChangeChannel
                Write( REG_DIOMAPPING1, ( Read( REG_DIOMAPPING1 ) & RFLR_DIOMAPPING1_DIO0_MASK & RFLR_DIOMAPPING1_DIO2_MASK  ) | RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO2_00 );
            }
            else
            {
                Write( REG_LR_IRQFLAGSMASK, //RFLR_IRQFLAGS_RXTIMEOUT |
                                                  //RFLR_IRQFLAGS_RXDONE |
                                                  //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                                  //RFLR_IRQFLAGS_VALIDHEADER |
                                                  RFLR_IRQFLAGS_TXDONE |
                                                  RFLR_IRQFLAGS_CADDONE |
                                                  RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                                  RFLR_IRQFLAGS_CADDETECTED );

                // DIO0=RxDone
                Write( REG_DIOMAPPING1, ( Read( REG_DIOMAPPING1 ) & RFLR_DIOMAPPING1_DIO0_MASK ) | RFLR_DIOMAPPING1_DIO0_00 );
            }
            Write( REG_LR_FIFORXBASEADDR, 0 );
            Write( REG_LR_FIFOADDRPTR, 0 );
        }
        break;
    }

    for(uint8_t i = 0; i < rxtxBufferSize; ++i){
    	rxtxBufferstorage[i] = 0;
    };
    //memset( rxtxBuffer, 0, ( size_t )RX_BUFFER_SIZE );

    this->settings.State = RF_RX_RUNNING;
    if( timeout != 0 )
    {
        //rxTimeoutTimer.attach_us( mbed::callback( this, &SX1276M1BxASWrapper::OnTimeoutIrq ), timeout * 1e3 );
    	SetTimeoutTimer(rxTimeoutTimer, timeout * 1e3 );
    }

    if( this->settings.Modem == MODEM_FSK )
    {
        SetOpMode( RF_OPMODE_RECEIVER );

        if( rxContinuous == false )
        {
            //rxTimeoutSyncWord.attach_us( mbed::callback( this, &SX1276M1BxASWrapper::OnTimeoutIrq ),
        		//this->settings.Fsk.RxSingleTimeout * 1e3 );
        	SetTimeoutTimer(rxTimeoutSyncWord,this->settings.Fsk.RxSingleTimeout * 1e3 );
        }
    }
    else
    {
        if( rxContinuous == true )
        {
            SetOpMode( RFLR_OPMODE_RECEIVER );
        }
        else
        {
            SetOpMode( RFLR_OPMODE_RECEIVER_SINGLE );
        }
    }
}

void SX1276M1BxASWrapper::Tx( uint32_t timeout )
{

    switch( this->settings.Modem )
    {
    case MODEM_FSK:
        {
            // DIO0=PacketSent
            // DIO1=FifoEmpty
            // DIO2=FifoFull
            // DIO3=FifoEmpty
            // DIO4=LowBat
            // DIO5=ModeReady
            Write( REG_DIOMAPPING1, ( Read( REG_DIOMAPPING1 ) & RF_DIOMAPPING1_DIO0_MASK &
                                                                            RF_DIOMAPPING1_DIO1_MASK &
                                                                            RF_DIOMAPPING1_DIO2_MASK ) |
                                                                            RF_DIOMAPPING1_DIO1_01 );

            Write( REG_DIOMAPPING2, ( Read( REG_DIOMAPPING2 ) & RF_DIOMAPPING2_DIO4_MASK &
                                                                            RF_DIOMAPPING2_MAP_MASK ) );
            this->settings.FskPacketHandler.FifoThresh = Read( REG_FIFOTHRESH ) & 0x3F;
        }
        break;
    case MODEM_LORA:
        {
            if( this->settings.LoRa.FreqHopOn == true )
            {
                Write( REG_LR_IRQFLAGSMASK, RFLR_IRQFLAGS_RXTIMEOUT |
                                                  RFLR_IRQFLAGS_RXDONE |
                                                  RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                                  RFLR_IRQFLAGS_VALIDHEADER |
                                                  //RFLR_IRQFLAGS_TXDONE |
                                                  RFLR_IRQFLAGS_CADDONE |
                                                  //RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                                  RFLR_IRQFLAGS_CADDETECTED );

                // DIO0=TxDone, DIO2=FhssChangeChannel
                Write( REG_DIOMAPPING1, ( Read( REG_DIOMAPPING1 ) & RFLR_DIOMAPPING1_DIO0_MASK & RFLR_DIOMAPPING1_DIO2_MASK ) | RFLR_DIOMAPPING1_DIO0_01 | RFLR_DIOMAPPING1_DIO2_00 );
            }
            else
            {
                Write( REG_LR_IRQFLAGSMASK, RFLR_IRQFLAGS_RXTIMEOUT |
                                                  RFLR_IRQFLAGS_RXDONE |
                                                  RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                                  RFLR_IRQFLAGS_VALIDHEADER |
                                                  //RFLR_IRQFLAGS_TXDONE |
                                                  RFLR_IRQFLAGS_CADDONE |
                                                  RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                                  RFLR_IRQFLAGS_CADDETECTED );

                // DIO0=TxDone
                Write( REG_DIOMAPPING1, ( Read( REG_DIOMAPPING1 ) & RFLR_DIOMAPPING1_DIO0_MASK ) | RFLR_DIOMAPPING1_DIO0_01 );
            }
        }
        break;
    }

    this->settings.State = RF_TX_RUNNING;
    //txTimeoutTimer.attach_us( mbed::callback( this, &SX1276M1BxASWrapper::OnTimeoutIrq ), timeout * 1e3 );
    SetTimeoutTimer(txTimeoutTimer, timeout * 1e3 );
    SetOpMode( RF_OPMODE_TRANSMITTER );
}

void SX1276M1BxASWrapper::StartCad(  )
{
    switch( this->settings.Modem )
    {
    case MODEM_FSK:
        {

        }
        break;
    case MODEM_LORA:
        {
            Write( REG_LR_IRQFLAGSMASK, RFLR_IRQFLAGS_RXTIMEOUT |
                                        RFLR_IRQFLAGS_RXDONE |
                                        RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                        RFLR_IRQFLAGS_VALIDHEADER |
                                        RFLR_IRQFLAGS_TXDONE |
                                        //RFLR_IRQFLAGS_CADDONE |
                                        RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL // |
                                        //RFLR_IRQFLAGS_CADDETECTED
                                        );

            // DIO3=CADDone
            Write( REG_DIOMAPPING1, ( Read( REG_DIOMAPPING1 ) & RFLR_DIOMAPPING1_DIO3_MASK ) | RFLR_DIOMAPPING1_DIO3_00 );

            this->settings.State = RF_CAD;
            SetOpMode( RFLR_OPMODE_CAD );
        }
        break;
    default:
        break;
    }
}

void SX1276M1BxASWrapper::SetTxContinuousWave( uint32_t freq, int8_t power, uint16_t time )
{
    uint32_t timeout = ( uint32_t )( time * 1e6 );

    SetChannel( freq );

    SetTxConfig( MODEM_FSK, power, 0, 0, 4800, 0, 5, false, false, 0, 0, 0, timeout );

    Write( REG_PACKETCONFIG2, ( Read( REG_PACKETCONFIG2 ) & RF_PACKETCONFIG2_DATAMODE_MASK ) );
    // Disable radio interrupts
    Write( REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_11 | RF_DIOMAPPING1_DIO1_11 );
    Write( REG_DIOMAPPING2, RF_DIOMAPPING2_DIO4_10 | RF_DIOMAPPING2_DIO5_10 );

    this->settings.State = RF_TX_RUNNING;
    //txTimeoutTimer.attach_us( mbed::callback( this, &SX1276M1BxASWrapper::OnTimeoutIrq ), timeout );
 SetTimeoutTimer(txTimeoutTimer, timeout );
    SetOpMode( RF_OPMODE_TRANSMITTER );
}

int16_t SX1276M1BxASWrapper::GetRssi( RadioModems_t modem )
{
    int16_t rssi = 0;

    switch( modem )
    {
    case MODEM_FSK:
        rssi = -( Read( REG_RSSIVALUE ) >> 1 );
        break;
    case MODEM_LORA:
        if( this->settings.Channel > RF_MID_BAND_THRESH )
        {
            rssi = RSSI_OFFSET_HF + Read( REG_LR_RSSIVALUE );
        }
        else
        {
            rssi = RSSI_OFFSET_LF + Read( REG_LR_RSSIVALUE );
        }
        break;
    default:
        rssi = -1;
        break;
    }
    return rssi;
}

void SX1276M1BxASWrapper::SetOpMode( uint8_t opMode )
{
    if( opMode == RF_OPMODE_SLEEP )
    {
        SetAntSwLowPower( true );
    }
    else
    {
        SetAntSwLowPower( false );
        SetAntSw( opMode );
    }
    Write( REG_OPMODE, ( Read( REG_OPMODE ) & RF_OPMODE_MASK ) | opMode );
}

void SX1276M1BxASWrapper::SetModem( RadioModems_t modem )
{
    if( ( Read( REG_OPMODE ) & RFLR_OPMODE_LONGRANGEMODE_ON ) != 0 )
    {
        this->settings.Modem = MODEM_LORA;
    }
    else
    {
        this->settings.Modem = MODEM_FSK;
    }

    if( this->settings.Modem == modem )
    {
        return;
    }

    this->settings.Modem = modem;
    switch( this->settings.Modem )
    {
    default:
    case MODEM_FSK:
        Sleep( );
        Write( REG_OPMODE, ( Read( REG_OPMODE ) & RFLR_OPMODE_LONGRANGEMODE_MASK ) | RFLR_OPMODE_LONGRANGEMODE_OFF );

        Write( REG_DIOMAPPING1, 0x00 );
        Write( REG_DIOMAPPING2, 0x30 ); // DIO5=ModeReady
        break;
    case MODEM_LORA:
        Sleep( );
        Write( REG_OPMODE, ( Read( REG_OPMODE ) & RFLR_OPMODE_LONGRANGEMODE_MASK ) | RFLR_OPMODE_LONGRANGEMODE_ON );

        Write( REG_DIOMAPPING1, 0x00 );
        Write( REG_DIOMAPPING2, 0x00 );
        break;
    }
}

void SX1276M1BxASWrapper::SetMaxPayloadLength( RadioModems_t modem, uint8_t max )
{
    this->SetModem( modem );

    switch( modem )
    {
    case MODEM_FSK:
        if( this->settings.Fsk.FixLen == false )
        {
            this->Write( REG_PAYLOADLENGTH, max );
        }
        break;
    case MODEM_LORA:
        this->Write( REG_LR_PAYLOADMAXLENGTH, max );
        break;
    }
}

void SX1276M1BxASWrapper::SetPublicNetwork( bool enable )
{
    SetModem( MODEM_LORA );
    this->settings.LoRa.PublicNetwork = enable;
    if( enable == true )
    {
        // Change LoRa modem SyncWord
        Write( REG_LR_SYNCWORD, LORA_MAC_PUBLIC_SYNCWORD );
    }
    else
    {
        // Change LoRa modem SyncWord
        Write( REG_LR_SYNCWORD, LORA_MAC_PRIVATE_SYNCWORD );
    }
}

void SX1276M1BxASWrapper::OnTimeoutIrq(  )
{
    switch( this->settings.State )
    {
    case RF_RX_RUNNING:
        if( this->settings.Modem == MODEM_FSK )
        {
            this->settings.FskPacketHandler.PreambleDetected = false;
            this->settings.FskPacketHandler.SyncWordDetected = false;
            this->settings.FskPacketHandler.NbBytes = 0;
            this->settings.FskPacketHandler.Size = 0;

            // Clear Irqs
            Write( REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI |
                                        RF_IRQFLAGS1_PREAMBLEDETECT |
                                        RF_IRQFLAGS1_SYNCADDRESSMATCH );
            Write( REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN );

            if( this->settings.Fsk.RxContinuous == true )
            {
                // Continuous mode restart Rx chain
                Write( REG_RXCONFIG, Read( REG_RXCONFIG ) | RF_RXCONFIG_RESTARTRXWITHOUTPLLLOCK );
                //rxTimeoutSyncWord.attach_us( mbed::callback( this, &SX1276M1BxASWrapper::OnTimeoutIrq ),
//                                             this->settings.Fsk.RxSingleTimeout * 1e3 );
 SetTimeoutTimer(rxTimeoutSyncWord,
                                             this->settings.Fsk.RxSingleTimeout * 1e3 );
            }
            else
            {
                this->settings.State = RF_IDLE;
                //rxTimeoutSyncWord.detach( );
 CancelTimeoutTimer(rxTimeoutSyncWord);
            }
        }
        if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->RxTimeout != NULL ) )
        {
            this->RadioEvents->RxTimeout( );
        }
        break;
    case RF_TX_RUNNING:
        // Tx timeout shouldn't happen.
        // But it has been observed that when it happens it is a result of a corrupted SPI transfer
        // it depends on the platform design.
        //
        // The workaround is to put the radio in a known state. Thus, we re-initialize it.

        // BEGIN WORKAROUND

        // Reset the radio
        Reset( );

        // Calibrate Rx chain
        RxChainCalibration( );

        // Initialize radio default values
        SetOpMode( RF_OPMODE_SLEEP );

        RadioRegistersInit( );

        SetModem( MODEM_FSK );

        // Restore previous network type setting.
        SetPublicNetwork( this->settings.LoRa.PublicNetwork );
        // END WORKAROUND

        this->settings.State = RF_IDLE;
        if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->TxTimeout != NULL ) )
        {
            this->RadioEvents->TxTimeout( );
        }
        break;
    default:
        break;
    }
}

void SX1276M1BxASWrapper::OnDio0Irq(  )
{
    volatile uint8_t irqFlags = 0;

    switch( this->settings.State )
    {
        case RF_RX_RUNNING:
            //TimerStop( &RxTimeoutTimer );
            // RxDone interrupt
            switch( this->settings.Modem )
            {
            case MODEM_FSK:
                if( this->settings.Fsk.CrcOn == true )
                {
                    irqFlags = Read( REG_IRQFLAGS2 );
                    if( ( irqFlags & RF_IRQFLAGS2_CRCOK ) != RF_IRQFLAGS2_CRCOK )
                    {
                        // Clear Irqs
                        Write( REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI |
                                                    RF_IRQFLAGS1_PREAMBLEDETECT |
                                                    RF_IRQFLAGS1_SYNCADDRESSMATCH );
                        Write( REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN );

                        //rxTimeoutTimer.detach( );
                        CancelTimeoutTimer(rxTimeoutTimer);

                        if( this->settings.Fsk.RxContinuous == false )
                        {
                            //rxTimeoutSyncWord.detach( );
                        	CancelTimeoutTimer(rxTimeoutSyncWord);
                            this->settings.State = RF_IDLE;
                        }
                        else
                        {
                            // Continuous mode restart Rx chain
                            Write( REG_RXCONFIG, Read( REG_RXCONFIG ) | RF_RXCONFIG_RESTARTRXWITHOUTPLLLOCK );
                            //rxTimeoutSyncWord.attach_us( mbed::callback( this, &SX1276M1BxASWrapper::OnTimeoutIrq ),
                            //      this->settings.Fsk.RxSingleTimeout * 1e3 );
                            SetTimeoutTimer(rxTimeoutSyncWord, this->settings.Fsk.RxSingleTimeout * 1e3 );
                        }

                        if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->RxError != NULL ) )
                        {
                            this->RadioEvents->RxError( );
                        }
                        this->settings.FskPacketHandler.PreambleDetected = false;
                        this->settings.FskPacketHandler.SyncWordDetected = false;
                        this->settings.FskPacketHandler.NbBytes = 0;
                        this->settings.FskPacketHandler.Size = 0;
                        break;
                    }
                }

                // Read received packet size
                if( ( this->settings.FskPacketHandler.Size == 0 ) && ( this->settings.FskPacketHandler.NbBytes == 0 ) )
                {
                    if( this->settings.Fsk.FixLen == false )
                    {
                        ReadFifo( ( uint8_t* )&this->settings.FskPacketHandler.Size, 1 );
                    }
                    else
                    {
                        this->settings.FskPacketHandler.Size = Read( REG_PAYLOADLENGTH );
                    }
                    ReadFifo( rxtxBuffer + this->settings.FskPacketHandler.NbBytes, this->settings.FskPacketHandler.Size - this->settings.FskPacketHandler.NbBytes );
                    this->settings.FskPacketHandler.NbBytes += ( this->settings.FskPacketHandler.Size - this->settings.FskPacketHandler.NbBytes );
                }
                else
                {
                    ReadFifo( rxtxBuffer + this->settings.FskPacketHandler.NbBytes, this->settings.FskPacketHandler.Size - this->settings.FskPacketHandler.NbBytes );
                    this->settings.FskPacketHandler.NbBytes += ( this->settings.FskPacketHandler.Size - this->settings.FskPacketHandler.NbBytes );
                }

                //rxTimeoutTimer.detach( );
                CancelTimeoutTimer(rxTimeoutTimer);

                if( this->settings.Fsk.RxContinuous == false )
                {
                    this->settings.State = RF_IDLE;
                    //rxTimeoutSyncWord.detach( );
                    CancelTimeoutTimer(rxTimeoutSyncWord);
                }
                else
                {
                    // Continuous mode restart Rx chain
                    Write( REG_RXCONFIG, Read( REG_RXCONFIG ) | RF_RXCONFIG_RESTARTRXWITHOUTPLLLOCK );
                    //rxTimeoutSyncWord.attach_us( mbed::callback( this, &SX1276M1BxASWrapper::OnTimeoutIrq ),
                    		//this->settings.Fsk.RxSingleTimeout * 1e3 );
                    SetTimeoutTimer(rxTimeoutSyncWord, this->settings.Fsk.RxSingleTimeout * 1e3 );
                }

                if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->RxDone != NULL ) )
                {
                    this->RadioEvents->RxDone( rxtxBuffer, this->settings.FskPacketHandler.Size, this->settings.FskPacketHandler.RssiValue, 0 );
                }
                this->settings.FskPacketHandler.PreambleDetected = false;
                this->settings.FskPacketHandler.SyncWordDetected = false;
                this->settings.FskPacketHandler.NbBytes = 0;
                this->settings.FskPacketHandler.Size = 0;
                break;
            case MODEM_LORA:
                {
                    int8_t snr = 0;

                    // Clear Irq
                    Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXDONE );

                    irqFlags = Read( REG_LR_IRQFLAGS );
                    if( ( irqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK ) == RFLR_IRQFLAGS_PAYLOADCRCERROR )
                    {
                        // Clear Irq
                        Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR );

                        if( this->settings.LoRa.RxContinuous == false )
                        {
                            this->settings.State = RF_IDLE;
                        }
                        //rxTimeoutTimer.detach( );
                        CancelTimeoutTimer(rxTimeoutTimer);

                        if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->RxError != NULL ) )
                        {
                            this->RadioEvents->RxError( );
                        }
                        break;
                    }

                    this->settings.LoRaPacketHandler.SnrValue = Read( REG_LR_PKTSNRVALUE );
                    if( this->settings.LoRaPacketHandler.SnrValue & 0x80 ) // The SNR sign bit is 1
                    {
                        // Invert and divide by 4
                        snr = ( ( ~this->settings.LoRaPacketHandler.SnrValue + 1 ) & 0xFF ) >> 2;
                        snr = -snr;
                    }
                    else
                    {
                        // Divide by 4
                        snr = ( this->settings.LoRaPacketHandler.SnrValue & 0xFF ) >> 2;
                    }

                    int16_t rssi = Read( REG_LR_PKTRSSIVALUE );
                    if( snr < 0 )
                    {
                        if( this->settings.Channel > RF_MID_BAND_THRESH )
                        {
                            this->settings.LoRaPacketHandler.RssiValue = RSSI_OFFSET_HF + rssi + ( rssi >> 4 ) +
                                                                          snr;
                        }
                        else
                        {
                            this->settings.LoRaPacketHandler.RssiValue = RSSI_OFFSET_LF + rssi + ( rssi >> 4 ) +
                                                                          snr;
                        }
                    }
                    else
                    {
                        if( this->settings.Channel > RF_MID_BAND_THRESH )
                        {
                            this->settings.LoRaPacketHandler.RssiValue = RSSI_OFFSET_HF + rssi + ( rssi >> 4 );
                        }
                        else
                        {
                            this->settings.LoRaPacketHandler.RssiValue = RSSI_OFFSET_LF + rssi + ( rssi >> 4 );
                        }
                    }
                    //Keep checking for packets until we have read all get the packet
                    LoraRcvPkt();
                }
                break;
            default:
                break;
            }
            break;
        case RF_TX_RUNNING:
            //txTimeoutTimer.detach( );
        	CancelTimeoutTimer(txTimeoutTimer);
            // TxDone interrupt
            switch( this->settings.Modem )
            {
            case MODEM_LORA:
                // Clear Irq
                Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE );
                // Intentional fall through
            case MODEM_FSK:
            default:
                this->settings.State = RF_IDLE;
                if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->TxDone != NULL ) )
                {
                    this->RadioEvents->TxDone( );
                }
                break;
            }
            break;
        default:
            break;
    }
}

bool SX1276M1BxASWrapper::LoraRcvPkt(){
    this->settings.LoRaPacketHandler.Size = Read( REG_LR_RXNBBYTES );
    if(this->settings.LoRaPacketHandler.Size > 0){
    	hal_printf("LoraRcvPkt: Got a pkt readig  %d bytes\n\r", this->settings.LoRaPacketHandler.Size);
    	hal_printf("\r");
    	ReadFifo( rxtxBuffer, this->settings.LoRaPacketHandler.Size );

		if( this->settings.LoRa.RxContinuous == false )
		{
			this->settings.State = RF_IDLE;
		}
		//rxTimeoutTimer.detach( );
		CancelTimeoutTimer(rxTimeoutTimer);

		if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->RxDone != NULL ) )
		{
			this->RadioEvents->RxDone( rxtxBuffer, this->settings.LoRaPacketHandler.Size, this->settings.LoRaPacketHandler.RssiValue, this->settings.LoRaPacketHandler.SnrValue );
		}
		return true;
    }
    else {
    	hal_printf("Got a interrupt: but couldnt read pkt: %d \n", this->settings.LoRaPacketHandler.Size);
    	return false;
    }
}


void SX1276M1BxASWrapper::OnDio1Irq(  )
{
    switch( this->settings.State )
    {
        case RF_RX_RUNNING:
            switch( this->settings.Modem )
            {
            case MODEM_FSK:
                // FifoLevel interrupt
                // Read received packet size
                if( ( this->settings.FskPacketHandler.Size == 0 ) && ( this->settings.FskPacketHandler.NbBytes == 0 ) )
                {
                    if( this->settings.Fsk.FixLen == false )
                    {
                        ReadFifo( ( uint8_t* )&this->settings.FskPacketHandler.Size, 1 );
                    }
                    else
                    {
                        this->settings.FskPacketHandler.Size = Read( REG_PAYLOADLENGTH );
                    }
                }

                if( ( this->settings.FskPacketHandler.Size - this->settings.FskPacketHandler.NbBytes ) > this->settings.FskPacketHandler.FifoThresh )
                {
                    ReadFifo( ( rxtxBuffer + this->settings.FskPacketHandler.NbBytes ), this->settings.FskPacketHandler.FifoThresh );
                    this->settings.FskPacketHandler.NbBytes += this->settings.FskPacketHandler.FifoThresh;
                }
                else
                {
                    ReadFifo( ( rxtxBuffer + this->settings.FskPacketHandler.NbBytes ), this->settings.FskPacketHandler.Size - this->settings.FskPacketHandler.NbBytes );
                    this->settings.FskPacketHandler.NbBytes += ( this->settings.FskPacketHandler.Size - this->settings.FskPacketHandler.NbBytes );
                }
                break;
            case MODEM_LORA:
                // Sync time out
                //rxTimeoutTimer.detach( );
 CancelTimeoutTimer(rxTimeoutTimer);
                // Clear Irq
                Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXTIMEOUT );

                this->settings.State = RF_IDLE;
                if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->RxTimeout != NULL ) )
                {
                    this->RadioEvents->RxTimeout( );
                }
                break;
            default:
                break;
            }
            break;
        case RF_TX_RUNNING:
            switch( this->settings.Modem )
            {
            case MODEM_FSK:
                // FifoEmpty interrupt
                if( ( this->settings.FskPacketHandler.Size - this->settings.FskPacketHandler.NbBytes ) > this->settings.FskPacketHandler.ChunkSize )
                {
                    WriteFifo( ( rxtxBuffer + this->settings.FskPacketHandler.NbBytes ), this->settings.FskPacketHandler.ChunkSize );
                    this->settings.FskPacketHandler.NbBytes += this->settings.FskPacketHandler.ChunkSize;
                }
                else
                {
                    // Write the last chunk of data
                    WriteFifo( rxtxBuffer + this->settings.FskPacketHandler.NbBytes, this->settings.FskPacketHandler.Size - this->settings.FskPacketHandler.NbBytes );
                    this->settings.FskPacketHandler.NbBytes += this->settings.FskPacketHandler.Size - this->settings.FskPacketHandler.NbBytes;
                }
                break;
            case MODEM_LORA:
                break;
            default:
                break;
            }
            break;
        default:
            break;
    }
}

void SX1276M1BxASWrapper::OnDio2Irq(  )
{
    switch( this->settings.State )
    {
        case RF_RX_RUNNING:
            switch( this->settings.Modem )
            {
            case MODEM_FSK:
                // Checks if DIO4 is connected. If it is not PreambleDtected is set to true.
                if(0)//if( this->dioIrq[4] == NULL )
                {
                    this->settings.FskPacketHandler.PreambleDetected = true;
                }

                if( ( this->settings.FskPacketHandler.PreambleDetected == true ) && ( this->settings.FskPacketHandler.SyncWordDetected == false ) )
                {
                    //rxTimeoutSyncWord.detach( );
 CancelTimeoutTimer(rxTimeoutSyncWord);

                    this->settings.FskPacketHandler.SyncWordDetected = true;

                    this->settings.FskPacketHandler.RssiValue = -( Read( REG_RSSIVALUE ) >> 1 );

                    this->settings.FskPacketHandler.AfcValue = ( int32_t )( double )( ( ( uint16_t )Read( REG_AFCMSB ) << 8 ) |
                                                                           ( uint16_t )Read( REG_AFCLSB ) ) *
                                                                           ( double )FREQ_STEP;
                    this->settings.FskPacketHandler.RxGain = ( Read( REG_LNA ) >> 5 ) & 0x07;
                }
                break;
            case MODEM_LORA:
                if( this->settings.LoRa.FreqHopOn == true )
                {
                    // Clear Irq
                    Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );

                    if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->FhssChangeChannel != NULL ) )
                    {
                        this->RadioEvents->FhssChangeChannel( ( Read( REG_LR_HOPCHANNEL ) & RFLR_HOPCHANNEL_CHANNEL_MASK ) );
                    }
                }
                break;
            default:
                break;
            }
            break;
        case RF_TX_RUNNING:
            switch( this->settings.Modem )
            {
            case MODEM_FSK:
                break;
            case MODEM_LORA:
                if( this->settings.LoRa.FreqHopOn == true )
                {
                    // Clear Irq
                    Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );

                    if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->FhssChangeChannel != NULL ) )
                    {
                        this->RadioEvents->FhssChangeChannel( ( Read( REG_LR_HOPCHANNEL ) & RFLR_HOPCHANNEL_CHANNEL_MASK ) );
                    }
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
    }
}

void SX1276M1BxASWrapper::OnDio3Irq(  )
{
    switch( this->settings.Modem )
    {
    case MODEM_FSK:
        break;
    case MODEM_LORA:
    	/*if( this->settings.State == RF_RX_RUNNING){ //BK: Adding interrupt for packet detected
    		if( ( Read( REG_LR_IRQFLAGS ) & RFLR_IRQFLAGS_VALIDHEADER ) == RFLR_IRQFLAGS_VALIDHEADER ){ //BK:Adding callback for
    			// Clear Irq
    		        	 Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_VALIDHEADER | RFLR_IRQFLAGS_VALIDHEADER );
    		             if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->ValidHeaderDetected != NULL ) )
    		             {
    		                 this->RadioEvents->ValidHeaderDetected( );
    		             }
    		        }
    	}*/
        if( ( Read( REG_LR_IRQFLAGS ) & RFLR_IRQFLAGS_CADDETECTED ) == RFLR_IRQFLAGS_CADDETECTED )
        {
            // Clear Irq
            Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_CADDETECTED | RFLR_IRQFLAGS_CADDONE );
            if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->CadDone != NULL ) )
            {
                this->RadioEvents->CadDone( true );
            }
        }
        else
        {
            // Clear Irq
            Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_CADDONE );
            if( ( this->RadioEvents != NULL ) && ( this->RadioEvents->CadDone != NULL ) )
            {
                this->RadioEvents->CadDone( false );
            }
        }
        break;
    default:
        break;
    }
}

void SX1276M1BxASWrapper::OnDio4Irq(  )
{
    switch( this->settings.Modem )
    {
    case MODEM_FSK:
        {
            if( this->settings.FskPacketHandler.PreambleDetected == false )
            {
                this->settings.FskPacketHandler.PreambleDetected = true;
            }
        }
        break;
    case MODEM_LORA:
        break;
    default:
        break;
    }
}

void SX1276M1BxASWrapper::OnDio5Irq(  )
{
    switch( this->settings.Modem )
    {
    case MODEM_FSK:
        break;
    case MODEM_LORA:
        break;
    default:
        break;
    }
}

void SX1276M1BxASWrapper::wait_ms(  UINT32 x){
	Events_WaitForEvents(0,x);
}

//} namespace SX1276_Semtech
