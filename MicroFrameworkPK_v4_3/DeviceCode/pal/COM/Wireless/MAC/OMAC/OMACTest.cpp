/*
 * TimesyncTest.cpp
 */
#include <tinyhal.h>
//#include <Samraksh/HALTimer.h>
#include <Samraksh/VirtualTimer.h>
#include "OMACTest.h"


//extern HALTimerManager gHalTimerManagerObject;
//extern VirtualTimer gVirtualTimerObject;
//extern VirtualTimerMapper gVirtualTimerMapperObject;
extern OMACTest gOMACTest;
extern UINT16 MF_NODE_ID;
//extern Buffer_15_4_t m_receive_buffer;

void OMACTest_Send_Timer_Handler(void * arg){
}

void OMACTest_ReceiveHandler (UINT16 NumOfPendingPackets){
}

void OMACTest_SendAckHandler (void* msg, UINT16 size, NetOpStatus status){
}

void SimpleLocalClockMonitorTimerHandler(void * arg) {
}

void SimpleNeighborClockMonitorTimerHandler(void * arg) {
}


// TIMESYNCTEST

BOOL OMACTest::Initialize(){
	MyAppID = 3; //pick a number less than MAX_APPS currently 4.
	Config.Network = 138;
	myEventHandler.SetRecieveHandler(OMACTest_ReceiveHandler);
	myEventHandler.SetSendAckHandler(OMACTest_SendAckHandler);
	VirtTimer_Initialize();

	MacId = OMAC;
	Mac_Initialize(&myEventHandler,MacId, MyAppID, Config.RadioID, (void*) &Config);

	return TRUE;
}

BOOL OMACTest::StartTest(){
	int a;
	VirtTimer_Start(OMACTest_Send_TIMER);
	while(1){
	}
	return TRUE;
}

void OMACTest_Initialize(){
	BOOL ret;
	ret = gOMACTest.Initialize();
	ret = gOMACTest.StartTest();
}


