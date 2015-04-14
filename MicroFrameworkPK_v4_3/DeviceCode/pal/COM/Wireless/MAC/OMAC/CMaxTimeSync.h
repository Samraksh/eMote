/*
 * CMaxTimeSync.h
 *
 *  Created on: Sep 10, 2012
 *      Author: Mukundan
 */

#ifndef CMAXTIMESYNC_H_
#define CMAXTIMESYNC_H_

//#include "PacketTimeSync_15_4.h"
#include <Samraksh/Message.h>
#include <Samraksh/Hal_util.h>
#include <Samraksh/GlobalTime.h>


enum CMaxTSyncState{
	E_Follower,
	E_Transition,
	E_Leader,
};

//typedef class CMaxTimeSync : public TimeSyncMessage_15_4  {
typedef class CMaxTimeSync : public SlotHandler{
	Message_15_4_t m_timeSyncMsgBuffer;
	Message_15_4_t *m_timeSyncBufferPtr;
	TimeSyncMsg *m_timeSyncMsg;
	//PacketTimeSync_15_4 m_packetTimeSync;
	UINT32 m_seqNo;
	float m_skew;
	UINT32 m_offsetAverage;
	UINT32 m_localAverage;
	CMaxTSyncState m_state;
	UINT32 m_messagePeriod;
	UINT32 m_lastSlotExecuted;

public:
	GlobalTime m_globalTime;
	BOOL m_isLeader;
	//UINT16 m_leader;


	void Initialize();
	UINT32 NextSlot(UINT32 currSlot);
	void ExecuteSlot(UINT32 slotNum);
	void PostExecuteSlot();

	//UINT32 GetSyncPoint();
	DeviceStatus Receive(Message_15_4_t* msg, void* payload, UINT8 len);
	void SendAckHandler(Message_15_4_t* msg, UINT8 len, NetOpStatus success);
	DeviceStatus Send(RadioAddress_t address, Message_15_4_t * msg, UINT16 size, UINT64 event_time);
	DeviceStatus Send();
	bool IsInTransitionPeriod(RadioAddress_t nodeID);
	void SetCounterOffset(UINT16 counterOffset);
	bool IsSynced(RadioAddress_t nodeID){
		return m_globalTime.synced;
	}
	void InsertBeacon(Message_15_4_t *ptr);
	bool IsProccessingBeacon();
	INT64 GetGlobalTime(){
		return 	m_globalTime.Read();
	}

	float GetSkew(){
		return m_skew;
	}
	UINT32 GetOffset(){
		return m_offsetAverage;
	}
}CMaxTimeSync_t;


#endif /* CMAXTIMESYNC_H_ */