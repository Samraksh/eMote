

#include <Samraksh/Message.h>
#include <Samraksh/MAC/OMAC/DiscoveryHandler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/MAC/OMAC/CMaxTimeSync.h>


#define DEBUG_TSYNC 1
#define FUDGEFACTOR 10000		//in 100ns, a value of 1000 =100 microseconds

#define LOCALSKEW 1
extern RadioControl_t g_omac_RadioControl;
extern OMACTypeBora g_OMAC;
extern NeighborTable g_NeighborTable;

INT64 GlobalTime::offset =0;
float GlobalTime::skew =0;
UINT16 GlobalTime::leader = 0xFFFF;
BOOL GlobalTime::synced=FALSE;

#define MIN_TICKS_DIFF_BTW_TSM 8000000
#define TIMESYNCSENDPIN 0 // 3 // PA3 J11-6
#define TIMESYNCRECEIVEPIN 31 // 23 //PB7 J11-10

//#define TXNODEID 2703
//#define RXNODEID 2491

#define TXNODEID 3505
#define RXNODEID 6846

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
	CPU_GPIO_EnableOutputPin((GPIO_PIN) TIMESYNCSENDPIN, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) TIMESYNCRECEIVEPIN, TRUE);

	CPU_GPIO_SetPinState((GPIO_PIN) TIMESYNCRECEIVEPIN, FALSE);

	RadioID = radioID;
	MacID = macID;
	m_skew = 0;
	//m_leader=TRUE;
	m_localAverage = 0;
	m_offsetAverage = 0;
	//m_messagePeriod = 10000/SLOT_PERIOD; //Time period in milliseconds
	m_messagePeriod = 10000 * TICKS_PER_MILLI;//Time period in ticks
	m_timeSyncBufferPtr = &m_timeSyncMsgBuffer;
	m_globalTime.Init();
	m_state = E_Leader;

#ifdef DEBUG_TSYNC

	if(g_OMAC.MyID == RXNODEID) {
		Neighbor2beFollowed = TXNODEID;
	}
	else {
		Neighbor2beFollowed = RXNODEID;
	}
#endif
}

/*
 *
 */
UINT16 CMaxTimeSync::NextEvent(UINT32 currentSlotNum){
	//return MAX_UINT32; //BK: WILD HACK. Disable the independent sending of the messages. TimeSync relies on the discovery alone.
	Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
	if ( sn == NULL ) return ((UINT32) MAX_UINT32);
	else if( DifferenceBetweenTimes(HAL_Time_CurrentTicks(), sn->LastTimeSyncTime) >= m_messagePeriod) { //Already passed the time. schedule send immediately
		if(sn->LastTimeSyncRequestTime == 0 || DifferenceBetweenTimes(HAL_Time_CurrentTicks(), sn->LastTimeSyncRequestTime) > MIN_TICKS_DIFF_BTW_TSM ) {
			return 0;
		}
		else {
			return ((UINT32) MAX_UINT32);
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
void CMaxTimeSync::ExecuteEvent(UINT32 currentSlotNum){
	////hal_printf("start CMaxTimeSync::ExecuteSlot\n");
	m_lastSlotExecuted = currentSlotNum;
	Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
	Send(sn->MacAddress,true);
	////hal_printf("end CMaxTimeSync::ExecuteSlot\n");
}

/*
 *
 */
UINT8 CMaxTimeSync::ExecuteEventDone()
{
	return 0;
}

/*
 *
 */
void CMaxTimeSync::PostExecuteEvent(){
	hal_printf("CMaxTimeSync::PostExecuteEvent\n");
}

/*
 *
 */
void CMaxTimeSync::SetWakeup(bool shldWakeup)
{
	hal_printf("CMaxTimeSync::SetWakeup\n");
}

/*
 *
 */
//DeviceStatus CMaxTimeSync::Send(RadioAddress_t address, Message_15_4_t  * msg, UINT16 size, UINT64 event_time){
BOOL CMaxTimeSync::Send(RadioAddress_t address, bool request_TimeSync){
	////GLOBAL_LOCK(irq);
	////VirtTimer_Stop(HAL_SLOT_TIMER);
	////hal_printf("start CMaxTimeSync::Send\n");
#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCSENDPIN, TRUE );
#endif

	IEEE802_15_4_Header_t * header = m_timeSyncBufferPtr->GetHeader();
	m_timeSyncMsg = (TimeSyncMsg *) m_timeSyncBufferPtr->GetPayload();
	//INT64 x,y,d;
	//x= 85899345921;
	UINT64 y;
	y = HAL_Time_CurrentTicks();
#ifndef LOCALSKEW
	x = m_globalTime.Local2Global(y);
#endif
	m_timeSyncMsg->localTime0 = (UINT32) y;
	m_timeSyncMsg->localTime1 = (UINT32) (y>>32);
	m_timeSyncMsg->request_TimeSync = request_TimeSync;

	//m_timeSyncMsg->globalTime0 = (UINT32) x;
	//m_timeSyncMsg->globalTime1 = (UINT32) (x>>32);

	//m_timeSyncMsg->skew = m_globalTime.GetSkew();
	//m_timeSyncMsg->nodeID = CPU_Radio_GetAddress(RadioID);
	//m_timeSyncMsg->seqNo = m_seqNo++;

	//header->dest= RADIO_BROADCAST_ADDRESS;
	header->type = MFM_TIMESYNC;

	/**/
	/*//IEEE802_15_4_Header_t *header = msg_carrier->GetHeader();
	header->length = sizeof(TimeSyncMsg) + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest = address;
	header->src = CPU_Radio_GetAddress(0);
	header->network = 138;
	header->mac_id = OMAC;
	header->type = MFM_TIMESYNC;

	m_timeSyncBufferPtr->GetMetaData()->SetReceiveTimeStamp(y);*/

	/*UINT8* lmsg = (UINT8 *) msg;
	UINT8* payload =  m_timeSyncBufferPtr->GetPayload();

	for(UINT8 i = 0 ; i < sizeof(TimeSyncMsg); i++) {
		payload[i] = lmsg[i];
	}*/
	/**/

	//d= x-y;

	//DeviceStatus rs = g_omac_RadioControl.Send_TimeStamped(RADIO_BROADCAST_ADDRESS,m_timeSyncBufferPtr,sizeof(TimeSyncMsg), (UINT32) (y & (~(UINT32) 0)) );
	BOOL rs = g_OMAC.SendTimeStamped(address,MFM_TIMESYNC, m_timeSyncBufferPtr,sizeof(TimeSyncMsg), (UINT32) (y & (~(UINT32) 0)) );
	g_NeighborTable.RecordTimeSyncRequestSent(address,(UINT32) (y & (~(UINT32) 0)));

	/*////DeviceStatus rs = g_omac_RadioControl.Send_TimeStamped(address,m_timeSyncBufferPtr,sizeof(TimeSyncMsg), (UINT32) (y & (~(UINT32) 0)) );
	timeSyncSendFlag = TRUE;
	hal_printf("setting timeSyncSendFlag %d\n", timeSyncSendFlag);
	BOOL rs = g_OMAC.SendTimeStamped(address, MFM_TIMESYNC, m_timeSyncBufferPtr, sizeof(TimeSyncMsg), (UINT32) (y & (~(UINT32) 0)) );
	g_NeighborTable.RecordTimeSyncSent(address,(UINT32) (y & (~(UINT32) 0)));*/
	//hal_printf("TS Send: %d,  Gtime: %lld, LTime: %lld, diff: %lld \n",m_seqNo, x, y, d);
	hal_printf("TS Send: %d, LTime: %lld \n\n",m_seqNo, y);
#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCSENDPIN, FALSE );
#endif
	////hal_printf("end CMaxTimeSync::Send\n");
	return rs;
}

/*
 *
 */
DeviceStatus CMaxTimeSync::Receive(Message_15_4_t* msg, void* payload, UINT8 len){
	////GLOBAL_LOCK(irq);
	bool TimerReturn;
	RadioAddress_t msg_src = msg->GetHeader()->src;

	////UINT16 msg_src =  msg->GetHeader()->src;
	////CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, TRUE );

#ifdef DEBUG_TSYNC
	if (msg_src == Neighbor2beFollowed ){
		if (m_globalTime.regressgt2.NumberOfRecordedElements(msg_src) >=2  ){
			CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, TRUE );
		}
	}
#endif

	UINT64 EventTime = PacketTimeSync_15_4::EventTime(msg,len);
	TimeSyncMsg* rcv_msg = (TimeSyncMsg *) msg->GetPayload();


	UINT64 rcv_ltime;
	INT64 l_offset;
	rcv_ltime=  (((UINT64)rcv_msg->localTime1) <<32) + rcv_msg->localTime0;
	l_offset = rcv_ltime - EventTime;

	m_globalTime.regressgt2.Insert(msg_src, rcv_ltime, l_offset);
	g_NeighborTable.RecordTimeSyncRecv(msg_src,EventTime);


	//Determine if timesync is requested, schedule sending a message back to the source
	if(rcv_msg->request_TimeSync){
		this->Send(msg_src,false);
	}
#ifdef DEBUG_TSYNC
	//if (Nbr2beFollowed==0){ Nbr2beFollowed = msg_src; }
	if (msg_src == Neighbor2beFollowed ){
		if (m_globalTime.regressgt2.NumberOfRecordedElements(msg_src) >=2  ){
			CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, FALSE );
		}
	}
#endif
	////CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, FALSE );
	////VirtTimer_Start(HAL_SLOT_TIMER);
	return DS_Success;
}

/*
 *
 */
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

/*
 *
 */
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

/*
 *
 */
void SendAckHandler(Message_15_4_t* msg, UINT8 len, NetOpStatus success){

}

/*
 *
 */
bool CMaxTimeSync::IsProccessingBeacon(){
	/*bool retVal;
	retVal = (m_processedMsg != NULL);
	return retVal;*/
	return FALSE;
}

/*
 *
 */
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
