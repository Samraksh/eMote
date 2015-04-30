/*
 * OMACTest.h
 */

#ifndef OMACTest_H_
#define OMACTest_H_

#include <tinyhal.h>
#include <Samraksh\Mac_decl.h>
#include "../OMAC/PacketTimeSync_15_4.h"
#include <Samraksh/GlobalTime.h>

//typedef Buffer_15_4<8> Buffer_15_4_t;
#define MAX_PACKET_SIZE_SIMPLETIMESYNC 100

typedef struct  {
	UINT16 MSGID;
	UINT8 data[5];
}SimplePayload_t;

#ifndef SimpleTimeSyncMsg_H_
#define SimpleTimeSyncMsg_H_
struct SimpleTimeSyncMsg
{
  /*
   * After TEP 133, the message timestamp contains the difference between
   * event time and the time the message was actually sent out. TimeSyncP
   * sends the local time associated with this globalTime to the
   * TimeStamping mechanism, which then calculates the difference.
   *
   * On the receiving side, the difference is applied to the local
   * timestamp. The receiving timestamp thus represents the time on the
   * receiving clock when the remote globalTime was taken.
   */
  //the time to startup the radio could be different for different nodes.
  //use this neighbor info along with local info to compute this difference
  //UINT16 radioStartDelay;
	UINT8 testnumber00;
	UINT8 testnumber01;
	UINT8 testnumber02;
	UINT8 testnumber03;
	UINT32 testnumberlong;
  UINT32 localTime0;
  UINT32 localTime1;
	UINT8 testnumber10;
	UINT8 testnumber11;
	UINT8 testnumber12;
	UINT8 testnumber13;
  UINT16 nodeID;
  UINT32 seqNo;
	UINT8 testnumber20;
	UINT8 testnumber21;
	UINT8 testnumber22;
	UINT8 testnumber23;

	UINT8 unusuedpadding[30];
};
#endif /* SimpleTimeSyncMsg_H_ */


class OMACTest{
public:
	OMACTest();
	GlobalTime m_globalTime;
	UINT8 MyAppID;
	Message_15_4_t  msg_15_4;
	UINT8 msg_in_buffer[MAX_PACKET_SIZE_SIMPLETIMESYNC];
	MacEventHandler myEventHandler;
	MacConfig Config;
	UINT8 MacId;
	UINT16 SendCount;
	UINT16 RcvCount;
	UINT16 Nbr2beFollowed;
	bool LocalClkMonitorPinState;
	bool NeighClkMonitorPinState;


	BOOL Initialize();
	BOOL StartTest();
};

OMACTest gOMACTest;

void OMACTest_Initialize();

void LocalClockMonitorTimerHandler(void * arg);
void LocalClockMonitorTimerScheduler();

void NbrClockMonitorTimerHandler(void * arg);
void NbrClockMonitorTimerScheduler();

#endif /* OMACTest_H_ */


