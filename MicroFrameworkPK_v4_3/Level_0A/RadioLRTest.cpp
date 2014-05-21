////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "RadioLRTest.h"
#include <tinyhal.h>
#include <Samraksh\HALTimer.h>
#include <Samraksh\HAL_util.h>
#include <..\..\radio\RF231\RF231.h>
//---//

RadioLRTest testObject;
extern HALTimerManager gHalTimerManagerObject;		//ref: \MicroFrameworkPK_v4_3\DeviceCode\pal\COM\Wireless\MAC\CSMAMAC\csmaMAC.cpp
UINT32 pkt_id = 0;

extern "C"
{

	void sendmsg(void* arg){
		if(RoutingEngine::IsRoot()||!RoutingEngine::HasValidRoute())
			return;
		/*BOOL SendResult;
		if(RoutingEngine::msgqueue.datapoint==-1)
			RoutingEngine::msgqueue.datapoint=1;*/
		
		CtpDataMessage msg;
		msg.Sender = RoutingEngine::GetSelfAddress();
		msg.SeqNo = pkt_id;
		//msg.datapoint = 1;
		//Try serializing a structure and sending it --Added by Dhrubo
		if(RoutingEngine::SendData(msg)!=DS_Success)
			hal_printf("\r\t\nPacket# %d send failed.\n",pkt_id);
		else
			hal_printf("\r\t\nSent Packet# %d.\n",pkt_id);
		pkt_id++;
	}
	
	BOOL RadioLRTest::Initialize(int seedValue, int numberOfEvents)
	{	
		//CPU_GPIO_Initialize();
		//CPU_SPI_Initialize();
		
		CtpRoutingBeaconMessage beacon;
		//radioID = RF231RADIOLR;
		radioID = RF231RADIO;
		routing_id = 0;
		DeviceStatus result;
	
		result = RoutingEngine::InitializeRoutingLayer(&beacon, routing_id, radioID);
		if(result != DS_Success)
		{
				DisplayStats(FALSE,"ERROR: Routing Layer Initialization failed", NULL, NULL);
				return FALSE;
		}
		return TRUE;
	}

}

BOOL RadioLRTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	hal_printf("\r\nresult=%s\r\n", (result) ? "PASS":"FAIL");
	hal_printf("\r\naccuracy=%d\r\n", accuracy);
	hal_printf("\r\nresultParameter1=%s\r\n", resultParameter1);
	hal_printf("\r\nresultParameter2=%s\r\n", resultParameter2);
	hal_printf("\r\nresultParameter3=%s\r\n", "null");
	hal_printf("\r\nresultParameter4=%s\r\n", "null");
	hal_printf("\r\nresultParameter5=%s\r\n", "null");

	return TRUE;
}

// This test only checks if the manufacture id can be read
BOOL RadioLRTest::Level_0A()
{
	if(!RoutingEngine::IsRoot()){
		gHalTimerManagerObject.Initialize();
		if(!gHalTimerManagerObject.CreateTimer(5, 0, 6000000, FALSE, FALSE, sendmsg)){ //6-second Timer in micro seconds
			DisplayStats(FALSE,"HAL Timer initialization failed", NULL, NULL);
			return FALSE;
		}
		gHalTimerManagerObject.StartTimer(5);
	}
	return TRUE;
}

// This test writes data and then verifies the write has been successful
BOOL RadioLRTest::Level_0B()
{
	return TRUE;

}

BOOL RadioLRTest::Level_1()
{
	return TRUE;
}

// Tests the buffer write and read interface
BOOL RadioLRTest::Level_0C()
{
	return TRUE;
}


BOOL RadioLRTest::Execute( int testLevel )
{
	BOOL result;

	switch(testLevel)
	{
	case 0:
		result = Level_0A();
		break;
	case 1:
		result = Level_0B();
		break;
	case 2:
		result = Level_0C();
		break;
	case 3:
		result = Level_1();
		break;
	}


	return result;



} //Execute

