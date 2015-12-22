/*
 * OMACTimeSync.cpp
 *
 *  Created on:
 *      Author: Bora Karaoglu
 *
 *  Modified on: Oct 30, 2015
 *  	Authors: Bora Karaoglu; Ananth Muralidharan
 *
 *  Copyright The Samraksh Company
 */

#include <Samraksh/Message.h>
#include <Samraksh/MAC/OMAC/DiscoveryHandler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/MAC/OMAC/OMACTimeSync.h>
//#include "NeighNodeIDDef.h"

#define FUDGEFACTOR 10000		//in 100ns, a value of 1000 =100 microseconds

#define LOCALSKEW 1
extern RadioControl_t g_omac_RadioControl;
extern OMACType g_OMAC;
extern NeighborTable g_NeighborTable;
extern OMACScheduler g_omac_scheduler;

INT64 GlobalTime::offset =0;
float GlobalTime::skew =0;
UINT16 GlobalTime::leader = 0xFFFF;
BOOL GlobalTime::synced=FALSE;

#define MIN_TICKS_DIFF_BTW_TSM 8000000




/*
 *
 */
//inline UINT64 DifferenceBetweenTimes(UINT64 X, UINT64 Y){
//	if(X>Y) return (X-Y);
//	else return( (MAX_UINT64 - Y) + X);
//}

/*
 *
 */
void OMACTimeSync::Initialize(UINT8 radioID, UINT8 macID){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(TIMESYNC_SENDPIN, FALSE);
	CPU_GPIO_EnableOutputPin(TIMESYNC_RECEIVEPIN, FALSE);
	CPU_GPIO_SetPinState(TIMESYNC_RECEIVEPIN, FALSE);
#endif

	RadioID = radioID;
	MacID = macID;

	m_messagePeriod = SENDER_CENTRIC_PROACTIVE_TIMESYNC_REQUEST;//Time period in ticks
	m_globalTime.Init();

}

UINT64 OMACTimeSync::NextEvent(){
	Neighbor_t* sn;
	UINT16 nextEventsSlot = 0;
	UINT64 nextEventsMicroSec = 0;
	nextEventsSlot = NextEventinSlots();
	while(nextEventsSlot == 0){
		sn = g_NeighborTable.GetCritalSyncNeighborWOldestSyncPtr(HAL_Time_CurrentTicks(),m_messagePeriod,FORCE_REQUESTTIMESYNC_INTICKS);
		if(sn != NULL) {
			Send(sn->MacAddress);
			nextEventsSlot = NextEventinSlots();
		}
	}


	nextEventsMicroSec = nextEventsSlot * SLOT_PERIOD_MILLI * MICSECINMILISEC;
	nextEventsMicroSec = nextEventsMicroSec + g_omac_scheduler.GetTimeTillTheEndofSlot();
	return(nextEventsMicroSec);
}
/*
 *
 */
UINT16 OMACTimeSync::NextEventinSlots(){
	UINT64 y = HAL_Time_CurrentTicks();
	UINT64 currentSlotNum = g_omac_scheduler.GetSlotNumber();
	Neighbor_t* sn = g_NeighborTable.GetCritalSyncNeighborWOldestSyncPtr(y, m_messagePeriod,FORCE_REQUESTTIMESYNC_INTICKS );
	if ( sn == NULL ) return ((UINT16) MAX_UINT32);

	else if( y - sn->LastTimeSyncSendTime >= m_messagePeriod) { //Already passed the time. schedule send immediately
		return 0;
	}
	else {
		UINT64 remslots = (m_messagePeriod - (y - sn->LastTimeSyncSendTime) ) / SLOT_PERIOD;
		return remslots;
	}
}


/*
 *
 */
void OMACTimeSync::ExecuteEvent(){
	//BK: This will be handled with the non_interrupt timer
//	Neighbor_t* sn;
//	while(NextEventinSlots() == 0){
//		sn = g_NeighborTable.GetCritalSyncNeighborWOldestSyncPtr(HAL_Time_CurrentTicks(),m_messagePeriod,FORCE_REQUESTTIMESYNC_INTICKS);
//		Send(sn->MacAddress);
//	}
	PostExecuteEvent();
}

/*
 *
 */
/*UINT8 OMACTimeSync::ExecuteEventDone()
{
	return 0;
}*/

/*
 *
 */
void OMACTimeSync::PostExecuteEvent(){
	g_omac_scheduler.PostExecution();
}



/*
 *
 */
//DeviceStatus OMACTimeSync::Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time){
BOOL OMACTimeSync::Send(RadioAddress_t address){
	 bool request_TimeSync;
	UINT64 lastTimeSyncRecv = g_NeighborTable.GetLastTimeSyncRecv(address);

	TimeSyncRequestMsg * tsreqmsg;
	BOOL rs = false;
	UINT64 y = HAL_Time_CurrentTicks();

	ASSERT_SP( y > lastTimeSyncRecv); //Ensure no rollover

	 if( y - lastTimeSyncRecv > FORCE_REQUESTTIMESYNC_INTICKS ){
		 request_TimeSync = true;
	 }
	 else{
		 request_TimeSync = false;
	 }

	if (m_globalTime.regressgt2.NumberOfRecordedElements(address) >=2  ){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( TIMESYNC_SENDPIN, TRUE );
#endif
		IEEE802_15_4_Header_t * header = m_timeSyncMsgBuffer.GetHeader();
		tsreqmsg = (TimeSyncRequestMsg *) m_timeSyncMsgBuffer.GetPayload();
		tsreqmsg->request_TimeSync = request_TimeSync;

		rs = g_OMAC.Send(address, MFM_TIMESYNCREQ, tsreqmsg, sizeof(TimeSyncRequestMsg));
#ifdef OMAC_DEBUG_PRINTF
		hal_printf("TS Send: %d, LTime: %lld \n\n",m_seqNo, y);
#endif
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( TIMESYNC_SENDPIN, FALSE );
#endif
	}

	g_NeighborTable.RecordTimeSyncRequestSent(address, y);
	return rs;
}

DeviceStatus OMACTimeSync::ReceiveTSReq(RadioAddress_t msg_src, TimeSyncRequestMsg* rcv_msg){

	//Determine if timesync is requested, schedule sending a message back to the source
	if(rcv_msg->request_TimeSync){
		hal_printf("OMACTimeSync::Receive. Sending\n");
		this->Send(msg_src);
	}
	return DS_Success;
}



void OMACTimeSync::CreateMessage(TimeSyncMsg* timeSyncMsg, UINT64 curticks, bool request_TimeSync) {
	timeSyncMsg->timesyncIdentifier = 50529027; //0x03030303
	timeSyncMsg->localTime0 = (UINT32) curticks;
	timeSyncMsg->localTime1 = (UINT32) (curticks>>32);
	timeSyncMsg->request_TimeSync = request_TimeSync;
	timeSyncMsg->seqNo = m_seqNo++;
}

/*
 *
 */
DeviceStatus OMACTimeSync::Receive(RadioAddress_t msg_src, TimeSyncMsg* rcv_msg, INT64 EventTime){
	bool TimerReturn;
	//RadioAddress_t msg_src = msg->GetHeader()->src;
	UINT64 y,neighborscurtime;

	if(rcv_msg->timesyncIdentifier != 50529027 ){
		y = HAL_Time_CurrentTicks();
		neighborscurtime = m_globalTime.Neighbor2LocalTime(msg_src,y);
		ASSERT_SP(0);
	}
	//UINT64 EventTime = PacketTimeSync_15_4::EventTime(msg,len);
	//TimeSyncMsg* rcv_msg = (TimeSyncMsg *) msg->GetPayload();
	UINT64 rcv_ltime;
	INT64 l_offset;
	rcv_ltime = (((UINT64)rcv_msg->localTime1) <<32) + rcv_msg->localTime0;
	l_offset = (INT64)rcv_ltime - (INT64)EventTime;

	if((m_globalTime.regressgt2.LastRecordedTime(msg_src) >= rcv_ltime)){
		return DS_Fail;
	}

	m_globalTime.regressgt2.Insert(msg_src, rcv_ltime, l_offset);
	g_NeighborTable.RecordTimeSyncRecv(msg_src,EventTime);

#ifdef def_Neighbor2beFollowed
	if (msg_src == g_OMAC.Neighbor2beFollowed ){
		if (m_globalTime.regressgt2.NumberOfRecordedElements(msg_src) >=2  ){
			TimerReturn = false;
		}
#ifdef DEBUG_TSYNC_PIN
	CPU_GPIO_SetPinState( TIMESYNC_RECEIVEPIN, TRUE );
#endif
	}
#endif

#ifdef def_Neighbor2beFollowed
	if (msg_src == g_OMAC.Neighbor2beFollowed ){
		if (m_globalTime.regressgt2.NumberOfRecordedElements(msg_src) >=2  ){
			y = HAL_Time_CurrentTicks();
			neighborscurtime = m_globalTime.Neighbor2LocalTime(msg_src,rcv_ltime);
		}
	}
#endif

#ifdef def_Neighbor2beFollowed
	if (msg_src == g_OMAC.Neighbor2beFollowed ){
		if (m_globalTime.regressgt2.NumberOfRecordedElements(msg_src) >= 2  ){

		}
#ifdef DEBUG_TSYNC_PIN
	CPU_GPIO_SetPinState( TIMESYNC_RECEIVEPIN, FALSE );
#endif
	}
#endif
	return DS_Success;
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
