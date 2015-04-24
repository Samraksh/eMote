#include "DiscoveryHandler.h"
#include <Samraksh/Message.h>
#include "RadioControl.h"
#include "CMaxTimeSync.h"
#include "PacketTimeSync_15_4.h"

#define DEBUG_TSYNC 1
#define FUDGEFACTOR 10000		//in 100ns, a value of 1000 =100 microseconds

#define LOCALSKEW 1
extern RadioControl_t g_omac_RadioControl;

INT64 GlobalTime::offset =0;
float GlobalTime::skew =0;
UINT16 GlobalTime::leader = 0xFFFF;
BOOL GlobalTime::synced=FALSE;




UINT32 CMaxTimeSync::NextSlot(UINT32 currSlot){

	return 1000; //Dont sent separate timesync
	//rollover
	/*if(currSlot< m_lastSlotExecuted){
		return m_messagePeriod - (0xFFFFFFFF + -  m_lastSlotExecuted + currSlot);
	}else {
		return m_messagePeriod - (currSlot -  m_lastSlotExecuted);
	}*/
}

void CMaxTimeSync::ExecuteSlot(UINT32 slotNum){
	m_lastSlotExecuted=slotNum;
	Send();
}

void CMaxTimeSync::PostExecuteSlot(){

}

void CMaxTimeSync::Initialize(UINT8 radioID, UINT8 macID){
	RadioID = radioID;
	MacID = macID;
	m_skew = 0;
	//m_leader=TRUE;
	m_localAverage = 0;
	m_offsetAverage = 0;
	m_messagePeriod = 10000/SLOT_PERIOD; //Time period in milliseconds
	m_timeSyncBufferPtr = &m_timeSyncMsgBuffer;
	m_globalTime.Init();
	m_state = E_Leader;

}


//DeviceStatus CMaxTimeSync::Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time){
DeviceStatus CMaxTimeSync::Send(){

#ifdef DEBUG_TSYNC
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, FALSE);
#endif

	IEEE802_15_4_Header_t * header = m_timeSyncBufferPtr->GetHeader();
	m_timeSyncMsg = (TimeSyncMsg *) m_timeSyncBufferPtr->GetPayload();
	INT64 x,y,d;
	x= 85899345921;
	y = HAL_Time_CurrentTime();
#ifndef LOCALSKEW
	x = m_globalTime.Local2Global(y);
#endif
	m_timeSyncMsg->localTime0 = (UINT32) y;
	m_timeSyncMsg->localTime1 = (UINT32) (y>>32);
	m_timeSyncMsg->globalTime0 = (UINT32) x;
	m_timeSyncMsg->globalTime1 = (UINT32) (x>>32);

	m_timeSyncMsg->skew = m_globalTime.GetSkew();
	m_timeSyncMsg->nodeID = CPU_Radio_GetAddress(RadioID);
	m_timeSyncMsg->seqNo = m_seqNo++;

	header->dest= RADIO_BROADCAST_ADDRESS;
	header->type=MFM_TIMESYNC;

	//d= x-y;

	g_omac_RadioControl.Send_TimeStamped(RADIO_BROADCAST_ADDRESS,m_timeSyncBufferPtr,sizeof(TimeSyncMsg),(UINT32) HAL_Time_CurrentTime());
	//hal_printf("TS Send: %d,  Gtime: %lld, LTime: %lld, diff: %lld \n",m_seqNo, x, y, d);
	hal_printf("TS Send: %d, LTime: %lld \n",m_seqNo, y);
#ifdef DEBUG_TSYNC
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, FALSE);
#endif

}

DeviceStatus CMaxTimeSync::Receive(Message_15_4_t* msg, void* payload, UINT8 len){

#ifdef DEBUG_TSYNC
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, FALSE);
#endif

	INT64 senderEventTime = PacketTimeSync_15_4::EventTime(msg,len);

	TimeSyncMsg* rcv_msg  = (TimeSyncMsg *) msg->GetPayload();
	UINT16 msg_src =  msg->GetHeader()->src;
#ifdef LOCALSKEW
	INT64 my_nbrEst= m_globalTime.Local2NbrTime(msg_src,senderEventTime);
#else
	INT64 my_gtime= m_globalTime.Local2Global(senderEventTime);
#endif

	INT64 my_ltime= senderEventTime;
	UINT64 rcv_ltime, rcv_gtime;
	INT64 g_offset, my_goffset;
	INT64 l_offset, nbr_loffset;
	rcv_gtime=  (((UINT64)rcv_msg->globalTime1) <<32) + rcv_msg->globalTime0;
	rcv_ltime=  (((UINT64)rcv_msg->localTime1) <<32) + rcv_msg->localTime0;

	nbr_loffset = rcv_ltime - my_nbrEst;
	l_offset = rcv_ltime - my_ltime;

#ifdef LOCALSKEW

	//hal_printf("RCV: %d, My LTime: %lld, Sender Ltime : %lld, My Est snder: %llu, diff: %lld \n", \
			rcv_msg->seqNo, my_ltime, rcv_ltime, my_nbrEst, nbr_loffset);

	//local pair-wise skew logic: Do skew between your nbr localtime and your localtime
	m_globalTime.regressgt2.Insert(msg_src, rcv_ltime,l_offset);
	//hal_printf("RCV: %d, My LTime: %lld, Sender Ltime : %lld, My Est snder: %llu, diff: %lld \n\n", \
				rcv_msg->seqNo, my_ltime, rcv_ltime, my_nbrEst, nbr_loffset);

#else
	g_offset = rcv_gtime - my_gtime;
	//hal_printf("RCV: %d, My Gtime: %llu , My LTime: %lld, Sender Gtime : %lld, LTime: %llu, diff: %lld \n",rcv_msg->seqNo, my_gtime, my_ltime, rcv_gtime, rcv_ltime, g_offset);
	//global skew logic: Do skew between your localtime and network globaltime

	if(g_offset - FUDGEFACTOR > 0){
		m_globalTime.Adjust(g_offset,my_ltime,TRUE);
		if(g_offset - FUDGEFACTOR > 0xFFFFFF){
		}else {
			m_globalTime.regress.Insert(msg_src, my_ltime, g_offset, senderEventTime);
		}
	}
	else {
		m_globalTime.regress.Insert(msg_src, my_ltime, 0, senderEventTime);
	}


	/*if(g_offset > 0 ){
		m_globalTime.leader = msg_src;
		m_state = E_Follower;
	}else {
		if (m_globalTime.leader == msg_src) {
			m_state = E_Leader;
			m_globalTime.leader = CPU_Radio_GetAddress(this->radioName);
		}

	}*/

	//Regress on global max clock
	/*if(g_offset > 0){
		m_globalTime.regress.Insert(msg_src, my_ltime,g_offset, senderEventTime);
	}else {
		m_globalTime.regress.Insert(msg_src, my_ltime,0, senderEventTime);
	}
	//}
	if((rcv1 > myg1) || (rcv1==myg1 && rcv0 > myg0)){
		m_globalTime.Adjust(g_offset, my_ltime, TRUE);
		m_globalTime.synced=TRUE;
		m_globalTime.leader = msg_src;
		m_isLeader = FALSE;
		hal_printf("Timesync: Syncing with leader, Adjusting global offset, old: %llu, new: %llu \n", my_gtime, rcv_gtime);

		if(m_globalTime.synced && g_offset > 0xFFFFFFFF) {
				hal_printf("What happened?? Fell out of sync\n");
				m_globalTime.synced=FALSE;
				m_globalTime.regress.Clean(m_globalTime.leader);
		}
	}
	*/
#endif

#ifdef DEBUG_TSYNC
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, FALSE);
#endif
	//hal_printf("receive 3");
	return DS_Success;
}


bool CMaxTimeSync::IsInTransitionPeriod(RadioAddress_t nodeID){
	if(m_state == E_Transition){
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

/*
bool CMaxTimeSync::IsSynced(RadioAddress_t nodeID) {
	if(State == E_Synced){
		return TRUE;
	}else {
		return FALSE;
	}
}
*/



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
