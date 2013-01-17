/*
 * OMACTest.h
 *
 *  Created on: Aug 31, 2012
 *      Author: Mukundan
 */

#ifndef OMACTEST_H_
#define OMACTEST_H_

#include <tinyhal.h>
#include <Samraksh\Mac_decl.h>
#include "../../Mac/CSMAMAC/csmaMAC.h"

typedef struct  {
	UINT16 MSGID;
	UINT8 data[5];
}Payload_t;

class MACTest{
public:
	UINT8 MyAppID;
	Payload_t msg;
	MacEventHandler myEventHandler;
	MacConfig Config;
	UINT8 MacId;
	UINT16 SendCount;
	UINT16 RcvCount;

	BOOL Initialize();
	BOOL StartTest();
	void Receive(void *msg, UINT16 size);
	BOOL Send();
	void SendAck(void *msg, UINT16 size, NetOpStatus status);
};

MACTest gMacTest;

void MacLayerTest_Initialize();

#endif /* OMACTEST_H_ */
