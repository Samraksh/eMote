////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "testMath.h"

#include <Samraksh\Message.h>
#include <Samraksh\Neighbors.h>
#include <Samraksh\MAC.h>
#include <Samraksh\Buffer.h>
#include <Samraksh\Radio_decl.h>
#include <Samraksh\RoutingEngine.h>
#include <..\Targets\Native\STM32F10x\DeviceCode\drivers\radio\RF231\RF231.h>

#define LEVEL_0A 0
#define LEVEL_0B 1
#define LEVEL_0C 2
#define LEVEL_1  3

#define INIT_STATE_CHECK()				UINT16 poll_counter, trx_status;

#define DID_STATE_CHANGE(x,y)				poll_counter = 0;               \
										do{ 							\
											if(poll_counter == 0xfffe)    \
											{  								\
												testObject.DisplayStats(FALSE,y, NULL,NULL); \
												return FALSE; 				\
											} 								\
											poll_counter++; 				\
										  }while(TRUE == x);							\


typedef Buffer_15_4<8> Buffer_15_4_t;

Message_15_4_t recv_mesg_carrier;
Message_15_4_t* recv_mesg_carrier_ptr;

typedef struct  {
	UINT16 MSGID;
	UINT8 data[5];
}Payload_t;

class RadioLRTest
{

	RadioEventHandler_t radioEventHandler;
	Buffer_15_4_t m_send_buffer;
	int numberOfEvents;
	testMath testMathInstance;
	Payload_t msg;
	Message_15_4_t msg_carrier;
	Message_15_4_t* msg_carrier_ptr;

public:
	UINT32 currentpackedId;
	UINT32 successfulTestCount;
	UINT8 radioID;
	UINT8 numberOfRadios;
	Ctp_Addr_T routing_id;
	static BOOL RadioAckPending;
	static BOOL RadioReceivedPending;

//--//
    //RadioLRTest ( int seedValue, int numberOfEvents );
	BOOL SendPacketSync(UINT16 dest, UINT8 dataType, void* msg, int Size);
	BOOL     Initialize(int seedValue, int numberOfEvents);
    BOOL     Execute( int testLevel );
	BOOL 	 Level_0A();
	BOOL 	 Level_0B();
	BOOL     Level_0C();
	BOOL 	 Level_1();
	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);
	Payload_t GetMsg() {return msg;};
};

BOOL RadioLRTest::RadioAckPending = FALSE;
BOOL RadioLRTest::RadioReceivedPending = FALSE;



