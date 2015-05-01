#include "DiscoveryHandler.h"
#include <Samraksh/Message.h>
#include "RadioControl.h"
#include "CMaxTimeSync.h"


#define DEBUG_TSYNC 1
#define FUDGEFACTOR 10000		//in 100ns, a value of 1000 =100 microseconds

#define LOCALSKEW 1
extern RadioControl_t g_omac_RadioControl;

INT64 GlobalTime::offset =0;
float GlobalTime::skew =0;
UINT16 GlobalTime::leader = 0xFFFF;
BOOL GlobalTime::synced=FALSE;

#define TIMESYNCSENDPIN 2
#define TIMESYNCRECEIVEPIN 23
#define NBRCLOCKMONITORPIN 29
#define LOCALCLOCKMONITORPIN 24

#define LocalClockMonitor_TIMER 32
#define NbrClockMonitor_TIMER 33

#define USEONESHOTTIMER FALSE

#define NBCLOCKMONITORPERIOD 100000
#define INITIALDELAY 100000

#define TXNODEID 30906
#define RXNODEID 4028



void CMaxTSLocalClockMonitorTimerHandler(void * arg) {
	//Toggle Pin State for monitoring with Logic Analyzer
	if(CPU_GPIO_GetPinState((GPIO_PIN) LOCALCLOCKMONITORPIN)){
		CPU_GPIO_SetPinState((GPIO_PIN) LOCALCLOCKMONITORPIN, false);
	}
	else {
		CPU_GPIO_SetPinState((GPIO_PIN) LOCALCLOCKMONITORPIN, true);
	}


}

void CMaxTSNeighborClockMonitorTimerHandler(void * arg) {
	//Toggle Pin State for monitoring with Logic Analyzer
	if(CPU_GPIO_GetPinState((GPIO_PIN) NBRCLOCKMONITORPIN)){
	//if(gSimpleTimesyncTest.NeighClkMonitorPinState) {
	//	gSimpleTimesyncTest.NeighClkMonitorPinState = false;
		CPU_GPIO_SetPinState((GPIO_PIN) NBRCLOCKMONITORPIN, false);
	}
	else {
	//	gSimpleTimesyncTest.NeighClkMonitorPinState = true;
		CPU_GPIO_SetPinState((GPIO_PIN) NBRCLOCKMONITORPIN, true);
	}
}

UINT32 CMaxTimeSync::NextSlot(UINT32 currSlot){

	//BK: Return periods until next timesync
	if( (currSlot > m_lastSlotExecuted)  ) {
		if ( (currSlot-m_lastSlotExecuted) >= m_messagePeriod  )
			return(0); //This is the case for triggerting
		else
			return (  m_messagePeriod - (currSlot-m_lastSlotExecuted) );
	}
	else{
		if( ( (0xFFFFFFFF -  m_lastSlotExecuted) + currSlot ) >= m_messagePeriod)
			return(0);
		else
			return(m_messagePeriod-( (0xFFFFFFFF -  m_lastSlotExecuted) + currSlot ));
	}
}

void CMaxTimeSync::ExecuteSlot(UINT32 slotNum){
	m_lastSlotExecuted=slotNum;
	Send();
}

void CMaxTimeSync::PostExecuteSlot(){

}

void CMaxTimeSync::Initialize(UINT8 radioID, UINT8 macID){
	VirtualTimerReturnMessage rm;

	CPU_GPIO_EnableOutputPin((GPIO_PIN) NBRCLOCKMONITORPIN, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) LOCALCLOCKMONITORPIN, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) TIMESYNCSENDPIN, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) TIMESYNCRECEIVEPIN, TRUE);

	CPU_GPIO_SetPinState((GPIO_PIN) TIMESYNCRECEIVEPIN, FALSE);

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

#ifdef DEBUG_TSYNC

	if(g_OMAC.MyID == RXNODEID) {
		Nbr2beFollowed = TXNODEID;
	}
	else {
		Nbr2beFollowed = RXNODEID;
	}

	rm = VirtTimer_SetTimer(LocalClockMonitor_TIMER, INITIALDELAY, (UINT32) NBCLOCKMONITORPERIOD, USEONESHOTTIMER, FALSE, CMaxTSLocalClockMonitorTimerHandler);
	rm = VirtTimer_SetTimer(NbrClockMonitor_TIMER, INITIALDELAY, (UINT32) NBCLOCKMONITORPERIOD, USEONESHOTTIMER, FALSE, CMaxTSNeighborClockMonitorTimerHandler);
#endif


}


//DeviceStatus CMaxTimeSync::Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time){
DeviceStatus CMaxTimeSync::Send(){
	DeviceStatus rs;
#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCSENDPIN, TRUE );
	VirtTimer_Stop(LocalClockMonitor_TIMER);
#endif

	IEEE802_15_4_Header_t * header = m_timeSyncBufferPtr->GetHeader();
	m_timeSyncMsg = (TimeSyncMsg *) m_timeSyncBufferPtr->GetPayload();
	INT64 x,y,d;
	x= 85899345921;
	y = HAL_Time_CurrentTicks();
#ifndef LOCALSKEW
	x = m_globalTime.Local2Global(y);
#endif
	m_timeSyncMsg->localTime0 = (UINT32) y;
	m_timeSyncMsg->localTime1 = (UINT32) (y>>32);
	//m_timeSyncMsg->globalTime0 = (UINT32) x;
	//m_timeSyncMsg->globalTime1 = (UINT32) (x>>32);

	//m_timeSyncMsg->skew = m_globalTime.GetSkew();
	//m_timeSyncMsg->nodeID = CPU_Radio_GetAddress(RadioID);
	//m_timeSyncMsg->seqNo = m_seqNo++;

	//header->dest= RADIO_BROADCAST_ADDRESS;
	header->type=MFM_TIMESYNC;

	//d= x-y;

	rs = g_omac_RadioControl.Send_TimeStamped(RADIO_BROADCAST_ADDRESS,m_timeSyncBufferPtr,sizeof(TimeSyncMsg), (UINT32) (y & (~(UINT32) 0)) );
	//hal_printf("TS Send: %d,  Gtime: %lld, LTime: %lld, diff: %lld \n",m_seqNo, x, y, d);
	hal_printf("TS Send: %d, LTime: %lld \n\n",m_seqNo, y);
#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCSENDPIN, FALSE );
	VirtTimer_Start(LocalClockMonitor_TIMER);
#endif
	return rs;

}

DeviceStatus CMaxTimeSync::Receive(Message_15_4_t* msg, void* payload, UINT8 len){
	bool TimerReturn;
	UINT16 msg_src =  msg->GetHeader()->src;

#ifdef DEBUG_TSYNC
	if (msg_src == Nbr2beFollowed ){
		if (m_globalTime.regressgt2.NumberOfRecordedElements(msg_src) >=2  ){

			CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, TRUE );
		}
	}
#endif

	UINT64 EventTime = PacketTimeSync_15_4::EventTime(msg,len);
	TimeSyncMsg* rcv_msg  = (TimeSyncMsg *) msg->GetPayload();


	UINT64 rcv_ltime;
	INT64 l_offset;
	rcv_ltime=  (((UINT64)rcv_msg->localTime1) <<32) + rcv_msg->localTime0;
	l_offset = rcv_ltime - EventTime;

	m_globalTime.regressgt2.Insert(msg_src, rcv_ltime, l_offset);

#ifdef DEBUG_TSYNC
	//if (Nbr2beFollowed==0){ Nbr2beFollowed = msg_src; }
	if (msg_src == Nbr2beFollowed ){
		if (m_globalTime.regressgt2.NumberOfRecordedElements(msg_src) >=2  ){

			CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, TRUE );

			TimerReturn = VirtTimer_Stop(NbrClockMonitor_TIMER);
			// RcvCount = 30; //This is to ensure preventing overflow on the RcvCount
			//float relfreq = 1.0;
			float relfreq = m_globalTime.regressgt2.FindRelativeFreq(msg_src);
			if (relfreq > 1.3){
				relfreq = 1.3;
			}
			else if (relfreq < 0.7){
				relfreq = 0.7;
			}
			UINT32 NeighborsPeriodLength = (UINT32) (((float) NBCLOCKMONITORPERIOD)*relfreq); ///m_globalTime.regressgt.samples[nbrIndex].relativeFreq;
			UINT64 y = HAL_Time_CurrentTicks();
			//INT64 start_delay = (y - EventTime); // Attempt to compansate for the difference
			INT64 start_delay = HAL_Time_TicksToTime((y - EventTime));
			//
//			UINT8 size = len;
//            UINT8 * rcv_msg2 =  msg->GetPayload();
//            UINT32 * senderEventTime = (UINT32 *)((UINT32)rcv_msg2 + size + TIMESTAMP_OFFSET);
//
//            UINT64 rcv_ts = msg->GetMetaData()->GetReceiveTimeStamp();
//
//            UINT32 sender_delay = *senderEventTime;
//			//
			start_delay = (INT64) (((float) INITIALDELAY)*relfreq) - start_delay;
			if (start_delay > (1.5*INITIALDELAY)) {
				start_delay = (1.5*INITIALDELAY);
			}
			else if(start_delay<(0.5*INITIALDELAY)){
				start_delay = (0.5*INITIALDELAY);
			}

			if (NeighborsPeriodLength > (1.5*NBCLOCKMONITORPERIOD)) {
				start_delay =  (1.5*NBCLOCKMONITORPERIOD) ;
			}
			else if(NeighborsPeriodLength <  (0.5*NBCLOCKMONITORPERIOD) ){
				start_delay =  (0.5*NBCLOCKMONITORPERIOD);
			}
			VirtTimer_Change(NbrClockMonitor_TIMER, start_delay, NeighborsPeriodLength, USEONESHOTTIMER);

			TimerReturn = VirtTimer_Start(NbrClockMonitor_TIMER);
		}
	}
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, FALSE );
#endif

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
