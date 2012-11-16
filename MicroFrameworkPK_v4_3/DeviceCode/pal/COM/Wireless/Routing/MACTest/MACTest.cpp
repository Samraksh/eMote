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

extern HALTimerManager gHalTimerManagerObject;
extern MACTest gMacTest;
extern UINT16 MF_NODE_ID;

void Timer_1_Handler(void * arg){
	CPU_GPIO_SetPinState((GPIO_PIN) 22, TRUE);
	gMacTest.Send();
	CPU_GPIO_SetPinState((GPIO_PIN) 22, FALSE);
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
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 22, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 23, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 10, FALSE);
	MAC_Initialize(&myEventHandler,&MacId, MyAppID, (void*) &Config);
	return TRUE;
}

BOOL MACTest::StartTest(){
	msg.MSGID=0;
	SendCount=0;
	RcvCount=0;
	//gHalTimerManagerObject.CreateTimer(1, 0, 1000000, FALSE, FALSE, Timer_1_Handler); //1 sec Timer in micro seconds
	return TRUE;
}

void MACTest::Receive(void *msg, UINT16 size){
	CPU_GPIO_SetPinState((GPIO_PIN) 8, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 8, FALSE);
	Payload_t *rcvmsg = (Payload_t *) msg;
	if(rcvmsg->MSGID!=RcvCount){
		//CPU_GPIO_SetPinState((GPIO_PIN) 0, TRUE);
	}
	RcvCount=rcvmsg->MSGID;
	CPU_GPIO_SetPinState((GPIO_PIN) 8, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 8, FALSE);
}

void MACTest::SendAck(void *msg, UINT16 size, NetOpStatus status){
	CPU_GPIO_SetPinState((GPIO_PIN) 10, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 10, FALSE);
	if(status==NO_Success){

	}else {

	}
}


BOOL MACTest::Send(){
	msg.MSGID=SendCount;
	msg.data[10]=10;
	CPU_GPIO_SetPinState((GPIO_PIN) 4, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 4, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 4, TRUE);
	Mac_Send(MacId, MAC_BROADCAST_ADDRESS, MFM_DATA, (void*) &msg.data, sizeof(Payload_t));
	SendCount++;
}

void MacLayerTest_Initialize(){
	gMacTest.Initialize();
	gMacTest.StartTest();
}

