/*
 * CMaxTimeSync.cpp
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
#include <Samraksh/MAC/OMAC/CMaxTimeSync.h>
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
inline UINT64 DifferenceBetweenTimes(UINT64 X, UINT64 Y){
	if(X>Y) return (X-Y);
	else return( (MAX_UINT64 - Y) + X);
}

/*
 *
 */
void CMaxTimeSync::Initialize(UINT8 radioID, UINT8 macID){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(TIMESYNC_SENDPIN, TRUE);
	CPU_GPIO_EnableOutputPin(TIMESYNC_RECEIVEPIN, TRUE);
	CPU_GPIO_SetPinState(TIMESYNC_RECEIVEPIN, FALSE);
#endif

	RadioID = radioID;
	MacID = macID;

	m_messagePeriod = 10000 * TICKS_PER_MILLI;//Time period in ticks
	m_globalTime.Init();
}

UINT64 CMaxTimeSync::NextEvent(){
	UINT16 nextEventsSlot = 0;
	UINT64 nextEventsMicroSec = 0;
	nextEventsSlot = NextEventinSlots();
	if(nextEventsSlot == 0) return(nextEventsMicroSec-1);//BK: Current slot is already too late. Hence return a large number back
	nextEventsMicroSec = nextEventsSlot * SLOT_PERIOD_MILLI * MICSECINMILISEC;
	nextEventsMicroSec = nextEventsMicroSec + g_omac_scheduler.GetTimeTillTheEndofSlot();
	return(nextEventsMicroSec);
}
/*
 *
 */
UINT16 CMaxTimeSync::NextEventinSlots(){
	//return MAX_UINT32; //BK: WILD HACK. Disable the independent sending of the messages. TimeSync relies on the discovery alone.
	UINT64 currentSlotNum = g_omac_scheduler.GetSlotNumber();
	Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
	if ( sn == NULL ) return ((UINT16) MAX_UINT32);
	else if( DifferenceBetweenTimes(HAL_Time_CurrentTicks(), sn->LastTimeSyncTime) >= m_messagePeriod) { //Already passed the time. schedule send immediately
		if(sn->LastTimeSyncRequestTime == 0 || DifferenceBetweenTimes(HAL_Time_CurrentTicks(), sn->LastTimeSyncRequestTime) > MIN_TICKS_DIFF_BTW_TSM ) {
			return 0;
		}
		else {
			return ((UINT16) MAX_UINT32);
		}
	}
	else {
		UINT64 remslots = (m_messagePeriod - DifferenceBetweenTimes(HAL_Time_CurrentTicks(), sn->LastTimeSyncTime) ) / SLOT_PERIOD;
		return remslots;
	}
}

/*UINT32 CMaxTimeSync::NextSlot(UINT32 currSlot){
	////hal_printf("start CMaxTimeSync::NextSlot\n");
	//return MAX_UINT32; //BK: WILD HACK. Disable the independent sending of the messages. TimeSync relies on the discovery alone.
	Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
	if ( sn == NULL ) return ((UINT32) MAX_UINT32);
	else if( (HAL_Time_CurrentTicks() - sn->LastTimeSyncTime) >= m_messagePeriod) { //Already passed the time. schedule send immediately
		Send(sn ->MacAddress);
		////hal_printf("end send CMaxTimeSync::NextSlot\n");
		return 0;
	}
	else {
		UINT64 remslots = (m_messagePeriod - (HAL_Time_CurrentTicks() - sn->LastTimeSyncTime) ) / SLOT_PERIOD;
		if ( remslots < 2 ) {
			hal_printf("end remslots 2 CMaxTimeSync::NextSlot\n");
			return 0;
		}
		else {
			////hal_printf("end remslots %d CMaxTimeSync::NextSlot\n", remslots);
			return remslots;
		}
	}
}*/


	//BK: Return periods until next timesync
/*	if( (currSlot > m_lastSlotExecuted)  ) {
		if ( (currSlot-m_lastSlotExecuted) >= m_messagePeriod  )
			return(0); //This is the case for triggerting
		else
			return (  m_messagePeriod - (currSlot-m_lastSlotExecuted) );
	}
	else{
		if( ( (MAX_UINT32 -  m_lastSlotExecuted) + currSlot ) >= m_messagePeriod)
			return(0);
		else
			return(m_messagePeriod-( (MAX_UINT32 -  m_lastSlotExecuted) + currSlot ));
	}
}*/

/*
 *
 */
void CMaxTimeSync::ExecuteEvent(){
	Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
	Send(sn->MacAddress, true);
	PostExecuteEvent();
}

/*
 *
 */
/*UINT8 CMaxTimeSync::ExecuteEventDone()
{
	return 0;
}*/

/*
 *
 */
void CMaxTimeSync::PostExecuteEvent(){
	g_omac_scheduler.PostExecution();
}



/*
 *
 */
//DeviceStatus CMaxTimeSync::Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time){
BOOL CMaxTimeSync::Send(RadioAddress_t address, bool request_TimeSync){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( TIMESYNC_SENDPIN, TRUE );
#endif

	IEEE802_15_4_Header_t * header = m_timeSyncMsgBuffer.GetHeader();
	m_timeSyncMsg = (TimeSyncMsg *) m_timeSyncMsgBuffer.GetPayload();
	UINT64 y = HAL_Time_CurrentTicks();
	CreateMessage(m_timeSyncMsg, y, request_TimeSync);
	BOOL rs = g_OMAC.SendTimeStamped(address, MFM_TIMESYNC, &m_timeSyncMsgBuffer, sizeof(TimeSyncMsg), (UINT32) (y & (~(UINT32) 0)) );
	g_NeighborTable.RecordTimeSyncRequestSent(address, (UINT32) (y & (~(UINT32) 0)));
	hal_printf("TS Send: %d, LTime: %lld \n\n",m_seqNo, y);
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( TIMESYNC_SENDPIN, FALSE );
#endif
	return rs;
}

void CMaxTimeSync::CreateMessage(TimeSyncMsg* timeSyncMsg, UINT64 curticks, bool request_TimeSync) {
	timeSyncMsg->localTime0 = (UINT32) curticks;
	timeSyncMsg->localTime1 = (UINT32) (curticks>>32);
	timeSyncMsg->request_TimeSync = request_TimeSync;
	timeSyncMsg->seqNo = m_seqNo++;
}

/*
 *
 */
DeviceStatus CMaxTimeSync::Receive(RadioAddress_t msg_src, TimeSyncMsg* rcv_msg, UINT64 EventTime){
	bool TimerReturn;
	//RadioAddress_t msg_src = msg->GetHeader()->src;

#ifdef def_Neighbor2beFollowed
	if (msg_src == g_OMAC.Neighbor2beFollowed ){
		if (m_globalTime.regressgt2.NumberOfRecordedElements(msg_src) >=2  ){
#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState( TIMESYNC_RECEIVEPIN, TRUE );
#endif
		}
	}
#endif

	//UINT64 EventTime = PacketTimeSync_15_4::EventTime(msg,len);
	//TimeSyncMsg* rcv_msg = (TimeSyncMsg *) msg->GetPayload();

	UINT64 rcv_ltime;
	INT64 l_offset;
	rcv_ltime = (((UINT64)rcv_msg->localTime1) <<32) + rcv_msg->localTime0;
	l_offset = rcv_ltime - EventTime;

	m_globalTime.regressgt2.Insert(msg_src, rcv_ltime, l_offset);
	g_NeighborTable.RecordTimeSyncRecv(msg_src,EventTime);

	//Determine if timesync is requested, schedule sending a message back to the source
	if(rcv_msg->request_TimeSync){
		hal_printf("CMaxTimeSync::Receive. Sending\n");
		this->Send(msg_src, false);
	}
#ifdef def_Neighbor2beFollowed
	if (msg_src == g_OMAC.Neighbor2beFollowed ){
		if (m_globalTime.regressgt2.NumberOfRecordedElements(msg_src) >= 2  ){
#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState( TIMESYNC_RECEIVEPIN, FALSE );
#endif
		}
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
