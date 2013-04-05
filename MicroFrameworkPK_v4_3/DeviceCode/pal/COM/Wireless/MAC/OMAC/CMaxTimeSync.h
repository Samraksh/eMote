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
#include "GlobalTime.h"

enum CMaxTSyncState{
	E_Synced,
	E_Transition,
	E_Init
};

//typedef class CMaxTimeSync : public TimeSyncMessage_15_4  {
typedef class CMaxTimeSync {
	float m_skew;
	UINT32 m_offsetAverage;
	UINT32 m_localAverage;
	CMaxTSyncState State;
	GlobalTime m_globalTime;

	Message_15_4_t m_timeSyncMsgBuffer;
	//Message_15_4_t *m_processedMsg;
	TimeSyncMsg_t *m_timeSyncMsg;
	//PacketTimeSync_15_4 m_packetTimeSync;

public:
	void Initialize();
	float GetSkew(){
		return m_skew;
	}
	UINT32 GetOffset(){
		return m_offsetAverage;
	}
	//UINT32 GetSyncPoint();
	DeviceStatus Receive(Message_15_4_t* msg, void* payload, UINT8 len);
	void SendAckHandler(Message_15_4_t* msg, UINT8 len, NetOpStatus success);
	DeviceStatus Send(RadioAddress_t address, Message_15_4_t * msg, UINT16 size, UINT64 event_time);
	DeviceStatus Send();
	bool IsInTransitionPeriod(RadioAddress_t nodeID);
	void SetCounterOffset(UINT16 counterOffset);
	bool IsSynced(RadioAddress_t nodeID);
	void InsertBeacon(Message_15_4_t *ptr);
	UINT64 GetGlobalTime(){
		return 	m_globalTime.Read();
	}
	bool IsProccessingBeacon();
}CMaxTimeSync_t;



#endif /* CMAXTIMESYNC_H_ */
