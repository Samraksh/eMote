/*
 * MACTest.cpp
 *
 *  Created on: Oct 8, 2012
 *      Author: Mukundan Sridharan
 *      A simple Common test for all MACs
 */
#include <tinyhal.h>
#include <Samraksh/HALTimer.h>
#include "MACTest.h"
#include <Samraksh\Message.h>

//#define DEBUG_MACTEST 1

extern HALTimerManager gHalTimerManagerObject;
extern MACTest gMacTest;
extern UINT16 MF_NODE_ID;

void Timer_1_Handler(void * arg){
#ifdef DEBUG_MACTEST
	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
#endif
	gMacTest.Send();
#ifdef DEBUG_MACTEST
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
#endif
}

// Typedef defining the signature of the receive function
void ReceiveHandler (void *msg, UINT16 Size){
	return gMacTest.Receive(msg,Size);
}

void SendAckHandler (void* msg, UINT16 size, NetOpStatus status){
	gMacTest.SendAck(msg,size,status);
}

BOOL MACTest::Initialize(){
	MyAppID=3; //pick a number less than MAX_APPS currently 4.
	//MF_NODE_ID=10;	//Set you address
	Config.Network = 138;
	myEventHandler.SetRecieveHandler(&ReceiveHandler);
	myEventHandler.SetSendAckHandler(&SendAckHandler);

	gHalTimerManagerObject.Initialize();
#ifdef DEBUG_MACTEST
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, FALSE);
#endif
	MAC_Initialize(&myEventHandler,&MacId, MyAppID, (void*) &Config);
	return TRUE;
}

BOOL MACTest::StartTest(){
	msg.MSGID=0;
	SendCount=0;
	RcvCount=0;
	gHalTimerManagerObject.CreateTimer(1, 0, 1000000, FALSE, FALSE, Timer_1_Handler); //1 sec Timer in micro seconds
	/*while(TRUE){
		HAL_Time_Sleep_MicroSeconds(5000000);
        MACTest::Send();
	}*/
	return TRUE;
}

void MACTest::Receive(void *msg, UINT16 size){
#ifdef DEBUG_MACTEST
	CPU_GPIO_SetPinState((GPIO_PIN)30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)30, FALSE);
#endif
	Payload_t *rcvmsg = (Payload_t *) msg;
	if(rcvmsg->MSGID!=RcvCount){
		//CPU_GPIO_SetPinState((GPIO_PIN) 0, TRUE);
	}
	RcvCount=rcvmsg->MSGID;
#ifdef DEBUG_MACTEST
	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
#endif
}

void MACTest::SendAck(void *msg, UINT16 size, NetOpStatus status){
#ifdef DEBUG_MACTEST
	CPU_GPIO_SetPinState((GPIO_PIN) 31, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 31, FALSE);
#endif
	if(status==NO_Success){

	}else {

	}
}


BOOL MACTest::Send(){
	msg.MSGID=SendCount;
	msg.data[10]=10;
#ifdef DEBUG_MACTEST
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
#endif
	Mac_Send(MacId, MAC_BROADCAST_ADDRESS, MFM_DATA, (void*) &msg.data, sizeof(Payload_t));
	SendCount++;
}

void MacLayerTest_Initialize(){
	gMacTest.Initialize();
	gMacTest.StartTest();
}

