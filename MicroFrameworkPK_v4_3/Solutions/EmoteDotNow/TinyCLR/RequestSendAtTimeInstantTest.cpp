/*
 * TimesyncTest.cpp
 */

/*
 * TestObject_t.cpp
 *
 *  Created on: Dec 24, 2015
 *      Author: Ananth Muralidharan
 *      A simple test to measure the pseudo randomness of the PRNG
 */

#include "RequestSendAtTimeInstantTest.h"
#include <Samraksh/VirtualTimer.h>

const UINT16 ONESEC_IN_MSEC = 1000;
const UINT16 ONEMSEC_IN_USEC = 1000;

TestObject_t g_TestObject_t;
extern EMOTE_SX1276_LORA::Samraksh_SX1276_hal gsx1276radio;
//extern EMOTE_SX1276_LORA::Samraksh_SX1276_hal gsx1276radio;


#define DEBUG_RadioTest 1
#define TEST_0A_TIMER1	7
#define TEST_0A_TIMER2	11
#define TIMER2_PERIOD 	4*ONESEC_IN_MSEC*ONEMSEC_IN_USEC
#define Test_0A_Timer_Pin (GPIO_PIN)25 //2



void TestObject_t:: TxDone( bool success){

	if(success){
		hal_printf("TestObject_t::TxDone success %d number_of_bytes_in_buffer = %d  \r\n" , success);
	}
	else{
		hal_printf("TestObject_t::TxDone success %d number_of_bytes_in_buffer = %d  \r\n" , success);
	}
//	g_TestObject_t.StartTest();
}
void TestObject_t:: PacketDetected(){

}
void TestObject_t:: RxDone (uint8_t *payload, uint16_t size ){

}
void TestObject_t:: CadDone ( bool channelActivityDetected ){

}
void TestObject_t:: DataStatusCallback( bool success, UINT16 number_of_bytes_in_buffer ){
	hal_printf("TestObject_t::DataStatusCallback success %d number_of_bytes_in_buffer = %d  \r\n" , success, number_of_bytes_in_buffer);
	if(success){

	}
	else{
		g_TestObject_t.delay = g_TestObject_t.delay*2;
//		g_TestObject_t.StartTest();
	}
}

void Test_0A_Timer1_Handler(void * arg){
	g_TestObject_t.ScheduleSendPacket();
}




void TestObject_t::ScheduleSendPacket()
{
	StartTest();
}

/*
 * Start TEST_0A_TIMER1 which keeps sending a packet until a response is received
 */
BOOL TestObject_t::StartTest()
{
	SamrakshRadio_I::ClockIdentifier_t ClockIdentifier = 4;
	SamrakshRadio_I::TimeVariable_t PacketTransmissionTime = HAL_Time_CurrentTicks() + delay;

	CPU_GPIO_SetPinState(Test_0A_Timer_Pin, FALSE);
	CPU_GPIO_SetPinState(Test_0A_Timer_Pin, TRUE);

	gsx1276radio.RequestSendAtTimeInstanst(static_cast<void*>(&msg), sizeof(Payload_t), PacketTransmissionTime, ClockIdentifier);

	return TRUE;
}

BOOL TestObject_t::Initialize()
{
	CPU_GPIO_EnableOutputPin(Test_0A_Timer_Pin, TRUE);
	CPU_GPIO_SetPinState(Test_0A_Timer_Pin, FALSE);
	CPU_GPIO_SetPinState(Test_0A_Timer_Pin, TRUE);

	delay = 10000;
	//radio_events = {TxDone, PacketDetected, RxDone, CadDone, DataStatusCallback};
	radio_events.TxDone = TxDone;
	radio_events.PacketDetected = PacketDetected;
	radio_events.RxDone = RxDone;
	radio_events.CadDone = CadDone;
	radio_events.DataStatusCallback = DataStatusCallback;

	hal_printf("TestObject_t::Initialize  \r\n");

	//	radio = dynamic_cast<SamrakshRadio_I*> (&gsx1276radio);
	//	DeviceStatus ds = radio->Initialize(radio_events);

	DeviceStatus ds = gsx1276radio.Initialize(radio_events);




	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(TEST_0A_TIMER1, 0, TIMER2_PERIOD, FALSE, FALSE, Test_0A_Timer1_Handler);
//		ASSERT(rm == TimerSupported);

	rm = VirtTimer_Start(TEST_0A_TIMER1);
//		ASSERT(rm == TimerSupported);


	return TRUE;
}

void Test_InitializeAndRun()
{
	g_TestObject_t.Initialize();

}


