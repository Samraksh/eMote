/*
 * RadioTest.h
 */

#ifndef RadioTest_H_
#define RadioTest_H_

#include <tinyhal.h>
//#include <Samraksh/Radio.h>
#include <Targets/Native/STM32F10x/DeviceCode/drivers/radio/SX1276/SamrakshSX1276hal.h>

#define PAYLOAD_SIZE	5

struct Payload_t {
	UINT16 MSGID;
	UINT8 data[PAYLOAD_SIZE];
};


class TestObject_t	{
public:
	UINT64 delay;
	static void TxDone( bool success);
	static void PacketDetected();
	static void RxDone (uint8_t *payload, uint16_t size );
	static void CadDone ( bool channelActivityDetected );
	static void DataStatusCallback( bool success, UINT16 number_of_bytes_in_buffer );
	SamrakshRadio_I::RadioEvents_t radio_events;

//	static void Test_0A_Timer1_Handler(void * arg);

	/*
	 * Alternates between sending packets and sleeping
	 */
//	static void Test_0A_Timer2_Handler(void * arg);
public:
	Payload_t msg;
	SamrakshRadio_I* radio;
//	RadioEvents_t radio_events;

	BOOL Initialize();
	BOOL StartTest();
	void ScheduleSendPacket();

};

//extern RadioTest g_RadioTest;

void Test_InitializeAndRun();

#endif /* RadioTest_H_ */



