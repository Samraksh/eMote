#include "DiscoveryHandler.h"
#include <Samraksh/Message.h>
#include "RadioControl.h"
#include "CMaxTimeSync.h"

extern RadioControl_t g_omac_RadioControl;

void CMaxTimeSync::Initialize(){
	m_skew = 0;
	m_localAverage = 0;
	m_offsetAverage = 0;
	//m_timeSyncMsg = (TimeSyncMsg_t *) m_timeSyncMsgBuffer.GetPayload();

	m_globalTime.Init();
}


//DeviceStatus CMaxTimeSync::Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time){
DeviceStatus CMaxTimeSync::Send(){
	IEEE802_15_4_Header_t * header = m_timeSyncMsgBuffer.GetHeader();
	m_timeSyncMsg = (TimeSyncMsg_t *) m_timeSyncMsgBuffer.GetPayload();
	m_timeSyncMsg->globalTime = m_globalTime.Read();
	m_timeSyncMsg->skew = m_globalTime.GetSkew();

	header->dest= RADIO_BROADCAST_ADDRESS;
	header->type=MFM_TIMESYNC;
	//UINT32 event_time = (UINT32) Time_GetLocalTime();
	g_omac_RadioControl.Send_TimeStamped(RADIO_BROADCAST_ADDRESS,&m_timeSyncMsgBuffer,sizeof(Message_15_4_t),(UINT32) Time_GetLocalTime());
}


DeviceStatus CMaxTimeSync::Receive(Message_15_4_t* msg, void* payload, UINT8 len){
	TimeSyncMsg_t* rcv_msg  = (TimeSyncMsg_t *) m_timeSyncMsgBuffer.GetPayload();
	UINT64 curr_gtime= m_globalTime.Read();
	//Adjust your globaltime if you received a new message
	if(rcv_msg->globalTime > curr_gtime + 20){
		m_globalTime.Adjust(rcv_msg->globalTime - curr_gtime, TRUE);
	}
	return DS_Success;
}





bool CMaxTimeSync::IsInTransitionPeriod(RadioAddress_t nodeID){
	if(State == E_Transition){
		return TRUE;
	}else {
		return FALSE;
	}

	/*	UINT8 idx;
		if (nbrToIdx(nodeID, &idx) == SUCCESS) {
			return ((m_beaconTable[idx].size < ENTRY_VALID_LIMIT) &&
			m_beaconTable[idx].isInTransition);
		} else {
			return FALSE;
		}
		*/
}

void CMaxTimeSync::SetCounterOffset(UINT16 counterOffset){
	//m_timeSyncBeacon->counterOffset = counterOffset;
}

bool CMaxTimeSync::IsSynced(RadioAddress_t nodeID) {
	if(State == E_Synced){
		return TRUE;
	}else {
		return FALSE;
	}
}




void SendAckHandler(Message_15_4_t* msg, UINT8 len, NetOpStatus success){

}


bool CMaxTimeSync::IsProccessingBeacon(){
	/*bool retVal;
	retVal = (m_processedMsg != NULL);
	return retVal;*/
	return FALSE;
}


void CMaxTimeSync::InsertBeacon(Message_15_4_t *ptr){
	/*Message_15_4_t *bufPtr;

	bufPtr = call MessagePool.get();
	if (bufPtr != NULL) {
		*bufPtr = *ptr;
		call MessageQueue.enqueue(bufPtr);
	}


#ifdef ORIGINAL_OMAC
		signal TimeSyncInfo.receivedBeaconAck(SUCCESS);
#endif

	m_receivedPiggybackBeacon = TRUE;
	*/
}


//GlobalTime interface implementation
	/*async command uint32_t GlobalTime.getLocalTime()
	{
		return call LocalTime.get();
	}

	async command error_t GlobalTime.getGlobalTime(uint32_t *time, am_addr_t nodeID)
	{
		*time = call GlobalTime.getLocalTime();
		if (nodeID == TOS_NODE_ID) {
			return SUCCESS;
		}
		else {
			return call GlobalTime.local2Global(time, nodeID);
		}
	}

	async command error_t GlobalTime.local2Global(uint32_t *time, am_addr_t nodeID)
	{
		error_t result = is_synced(nodeID);
		if (nodeID == TOS_NODE_ID) {
			return result;
		}
		// the input is the local time. If I'm synced, return the global time. Otherwise, do nothing
		if (result == SUCCESS) {
			uint8_t nbrIdx;
			nbrToIdx(nodeID, &nbrIdx);
			if (nbrIdx != INVALID_INDEX) {
				TableItem *ptr = &(m_beaconTable[nbrIdx]);
			//	printf("ofset=%ld lclAvg=%lu lcl=%lu, sk=%ld\n",
			//	    ptr->offsetAvg, ptr->localAvg, *time, (int32_t)(ptr->skew * 1000));
				*time += ptr->offsetAvg
					+ (int32_t)(ptr->skew * (int32_t)(*time - ptr->localAvg));
			} else {
				result = FAIL;
				printf("Error in local2Global. should not happen\n");
			}
		}

		return result;
	}

	async command error_t GlobalTime.global2Local(uint32_t *time, am_addr_t nodeID)
	{
		if (nodeID == TOS_NODE_ID) {
			printf("global2Local myself\n");
			return is_synced(nodeID);
		} else if (call TimeSyncInfo.isSynced(nodeID)) {
			uint8_t nbrIdx;
			nbrToIdx(nodeID, &nbrIdx);
			if (nbrIdx != INVALID_INDEX) {
				TableItem *ptr = &(m_beaconTable[nbrIdx]);
				uint32_t approxLocalTime = *time - ptr->offsetAvg;
				*time = approxLocalTime
					- (int32_t)(ptr->skew * (int32_t)(approxLocalTime - ptr->localAvg));
				return SUCCESS;
			}
			else {
				printf("Error in global2Local. should not happen\n");
				return FAIL;
			}
		} else {
			printf("global2Local failed\n");
			return FAIL;
		}
	}
*/
