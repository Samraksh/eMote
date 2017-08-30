/*
 * SX1276wrapper.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: Bora
 */


//#include <cstdint>
#include <Samraksh/VirtualTimer.h>

#include "SX1276wrapper.h"


SX1276_Semtech::SX1276M1BxASWrapper g_SX1276M1BxASWrapper;

namespace SX1276_Semtech {


//const RadioRegisters_t SX1276MB1xAS::RadioRegsInit[] = RADIO_INIT_REGISTERS_VALUE;




SX1276M1BxASWrapper::SX1276M1BxASWrapper() :  SX1276(){


//	RadioRegsInit[0].Modem = RADIO_INIT_REGISTERS_VALUE0;
	SX1276_Semtech::Get_SX1276_RADIO_INIT_REGISTERS_VALUE(RadioRegsInit[0],0);

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
}

void SX1276M1BxASWrapper::Read(uint8_t addr, uint8_t* buffer,
		uint8_t size) {

	radio_comm_ReadData(addr & 0x7F, 0, size, buffer);
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
	//Write a zero to the reset pin
	//and switch to reading in that pin and wait 6 ms
	CPU_GPIO_EnableOutputPin(SX1276_pin_setup.reset_mf_pin, TRUE);
	CPU_GPIO_SetPinState( SX1276_pin_setup.reset_mf_pin, FALSE );
	VirtTimer_SleepMicroseconds(VIRT_TIMER_SX1276_txTimeout, 1000 );
	CPU_GPIO_EnableInputPin(SX1276_pin_setup.reset_mf_pin, FALSE, SX1276M1BxASWrapper::SX1276_Reset_Pin_Interrupt_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	VirtTimer_SleepMicroseconds(VIRT_TIMER_SX1276_txTimeout, 1000 );

	while(reset_intiated){
		reset_intiated = true;
	};

}

void SX1276M1BxASWrapper::IoInit() {
//    AntSwInit( );
	CPU_GPIO_SetPinState( (GPIO_PIN)25 , FALSE);
	CPU_GPIO_SetPinState( (GPIO_PIN)25 , TRUE);
	this->SpiInit();
}

void SX1276M1BxASWrapper::RadioRegistersInit() {
    uint8_t i = 0;
    for( i = 0; i < sizeof( RadioRegsInit ) / sizeof( RadioRegisters_t ); i++ )
    {
    	SX1276_Semtech::Get_SX1276_RADIO_INIT_REGISTERS_VALUE(RadioRegsInit[i],0);
    	SetModem( RadioRegsInit[i].Modem );
        Write( RadioRegsInit[i].Addr, RadioRegsInit[i].Value );
    }
}

void SX1276M1BxASWrapper::SpiInit(void) {
	CPU_GPIO_SetPinState( (GPIO_PIN)25 , FALSE);
	CPU_GPIO_SetPinState( (GPIO_PIN)25 , TRUE);
	this->SpiInitialize();
}


void SX1276M1BxASWrapper::IoIrqInit() {
	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO0, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler0, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	EXTI_ClearITPendingBit(EXTI_Line1);
	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO1, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler1, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	EXTI_ClearITPendingBit(EXTI_Line1);
	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO2, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler2, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	EXTI_ClearITPendingBit(EXTI_Line1);
	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO3, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler3, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	EXTI_ClearITPendingBit(EXTI_Line1);
	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO4, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler4, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	EXTI_ClearITPendingBit(EXTI_Line1);
	CPU_GPIO_EnableInputPin(SX1276_interupt_pins.DIO5, FALSE, SX1276M1BxASWrapper::SX1276_Radio_Interrupt_Handler5, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	EXTI_ClearITPendingBit(EXTI_Line1);
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

void SX1276M1BxASWrapper::Initialize(SX1276_Semtech::RadioEvents_t *events) {

	//this->SX1276M1BxASWrapper::Reset();
	CPU_GPIO_SetPinState( (GPIO_PIN)25 , FALSE);
	CPU_GPIO_SetPinState( (GPIO_PIN)25 , TRUE);

	this->SX1276M1BxASWrapper::IoInit( );

	RxChainCalibration();
	CPU_GPIO_SetPinState( (GPIO_PIN)25 , FALSE);
	CPU_GPIO_SetPinState( (GPIO_PIN)25 , TRUE);

	SetOpMode( RF_OPMODE_SLEEP );
	IoIrqInit();
	RadioRegistersInit();

	InitializeTimers();
	this->SX1276::Init(events);

    SetModem( MODEM_FSK );

    this->settings.State = RF_IDLE ;

}

}
