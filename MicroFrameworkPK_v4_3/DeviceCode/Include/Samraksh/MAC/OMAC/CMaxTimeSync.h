/*
 * CMaxTimeSync.h
 *
 *  Created on: Sep 10, 2012
 *      Author: Mukundan
 */

#ifndef CMAXTIMESYNC_H_
#define CMAXTIMESYNC_H_

#include <Samraksh\PacketTimeSync_15_4.h>
//#include "PacketTimeSync_15_4.h"
#include <Samraksh/Message.h>
#include <Samraksh/Hal_util.h>
#include <Samraksh/GlobalTime.h>

/*
 *
 */
//typedef class CMaxTimeSync : public TimeSyncMessage_15_4  {
typedef class CMaxTimeSync : public EventHandler{
	UINT8 RadioID;
	UINT8 MacID;
	Message_15_4_t m_timeSyncMsgBuffer;
	TimeSyncMsg *m_timeSyncMsg;
	UINT64 m_messagePeriod;
	//PacketTimeSync_15_4 m_packetTimeSync;
	UINT32 m_seqNo;
public:
	GlobalTime m_globalTime;

	void Initialize(UINT8 radioID, UINT8 macID);
	UINT64 NextEvent();
  	UINT16 NextEventinSlots();
	//UINT64 NextEvent(UINT64 currentTicks);
	void ExecuteEvent();
	UINT8 ExecuteEventDone();
	void PostExecuteEvent();

	//UINT32 GetSyncPoint();
	DeviceStatus Receive(Message_15_4_t* msg, void* payload, UINT8 len);

	BOOL Send(RadioAddress_t address, bool request_TimeSync);

}CMaxTimeSync_t;


#endif /* CMAXTIMESYNC_H_ */
