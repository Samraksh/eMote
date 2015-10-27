/*
 * DataTransmissionHandler.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan Sridharan
 */

//#include <Samraksh/Neighbors.h>
#include <Samraksh/MAC/OMAC/DataTransmissionHandler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/MAC/OMAC/Scheduler.h>
#include <Samraksh/MAC/OMAC/RadioControl.h>


//#define DATATXPIN 29 //2

extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;
extern Buffer_15_4_t g_send_buffer;
//extern Buffer_15_4_t g_receive_buffer;
extern NeighborTable g_NeighborTable;
extern RadioControl_t g_omac_RadioControl;
DataTransmissionHandler g_DataTransmissionHandler;

static BOOL isDataPacketScheduled = false;

void PublicTXEndHCallback(void * param){
	g_omac_scheduler.m_DataTransmissionHandler.PostExecuteEvent();
}

UINT64 DataTransmissionHandler::GetTxTicks()
{
	//return m_nextTXTicks;
}

UINT32 DataTransmissionHandler::GetTxCounter()
{
	//return m_nextTXCounter;
}

void DataTransmissionHandler::SetTxTicks(UINT64 tmp_nextTXTicks)
{
	//m_nextTXTicks = tmp_nextTXTicks;
}

void DataTransmissionHandler::SetTxCounter(UINT32 tmp_nextTXCounter)
{
	//m_nextTXCounter = tmp_nextTXCounter;
}

INT64 DataTransmissionHandler::GetNeighborGlobalTime()
{
	return m_nbrGlobalTime;
}

UINT32 DataTransmissionHandler::GetNeighborWakeupSlot()
{
	return m_nbrWakeupSlot;
}

void DataTransmissionHandler::SetNeighborGlobalTime(INT64 tmp_nbrGlobalTime)
{
	m_nbrGlobalTime = tmp_nbrGlobalTime;
}

void DataTransmissionHandler::SetNeighborWakeupSlot(UINT32 tmp_nbrWakeupSlot)
{
	m_nbrWakeupSlot = tmp_nbrWakeupSlot;
}


/*
 *
 */
void DataTransmissionHandler::Initialize(){
	CPU_GPIO_EnableOutputPin(DATATX_PIN, TRUE);
	m_dsn = 0;
	m_retryCnt = m_dwellCnt=0;
	randVal = 0;
	m_nextDestination=0;
	m_receivedDataBeacon=0;
	m_busy =FALSE;   //indicates if radio is busy.

	//the # of times the current packet has been sent
	m_dataHeartBeats=0;
	//m_neighborNextEventTimeinTicks = 0;
	m_lastSlot=0;
	m_collisionCnt = 0;

	m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

	/*VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(HAL_TX_TIMER, 0, SLOT_PERIOD_MILLI * 1 * MICSECINMILISEC, TRUE, FALSE, PublicTXEndHCallback); //1 sec Timer in micro seconds*/

	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
}


/*
 * This function returns the number of ticks until the transmission time
 */
UINT64 DataTransmissionHandler::NextEvent(){
	//in case the task delay is large and we are already pass
	//tx time, tx immediately

	if(ScheduleDataPacket()) {
		UINT16 dest = m_outgoingEntryPtr->GetHeader()->dest;
		UINT64 nextTXTicks = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(dest, g_NeighborTable.GetNeighborPtr(dest)->nextwakeupSlot * SLOT_PERIOD_TICKS);
		UINT64 curTicks = HAL_Time_CurrentTicks();
		UINT64 remMicroSecnextTX = HAL_Time_TicksToTime(nextTXTicks - curTicks);
		hal_printf("DataTransmissionHandler::NextEvent curTicks: %llu; nextTXTicks: %llu; remMicroSecnextTX: %llu\n", curTicks, nextTXTicks, remMicroSecnextTX);
		//UINT64 curTicks = HAL_Time_CurrentTicks();
		hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler::NextEvent() remMicroSecnextTX= %llu AbsnextWakeupTimeInMicSec= %llu - %lu m_neighborNextEventTimeinMicSec = %llu - %lu\n"
				, HAL_Time_TicksToTime(curTicks), g_omac_scheduler.GetSlotNumber(), HAL_Time_TicksToTime(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, curTicks)), g_omac_scheduler.GetSlotNumberfromTicks(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, curTicks))
				, remMicroSecnextTX, HAL_Time_TicksToTime(nextTXTicks), g_omac_scheduler.GetSlotNumberfromTicks(nextTXTicks), HAL_Time_TicksToTime(g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr->GetHeader()->dest)->nextwakeupSlot * SLOT_PERIOD_TICKS), g_omac_scheduler.GetSlotNumberfromTicks(g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr->GetHeader()->dest)->nextwakeupSlot * SLOT_PERIOD_TICKS) );

		return remMicroSecnextTX;
	}
	else {
		//Either Dont have packet to send or missing timing for the destination
		return MAX_UINT64;
	}
}

/*
 *
 */
void DataTransmissionHandler::ExecuteEvent(){
	//BK: At this point there should be some message to be sent in the m_outgoingEntryPtr
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler:ExecuteEvent\n"
			, HAL_Time_TicksToTime(HAL_Time_CurrentTicks()), g_omac_scheduler.GetSlotNumber(), HAL_Time_TicksToTime(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTicks())), g_omac_scheduler.GetSlotNumberfromTicks(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTicks())) );
	DeviceStatus e = DS_Fail;
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
	//e = g_omac_RadioControl.StartRx();
	bool rv = Send();
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	//m_neighborNextEventTimeinTicks = 0;
	PostExecuteEvent();
}

/*
 *
 */
/*UINT8 DataTransmissionHandler::ExecuteEventDone(){
	return 0;
}*/

/*
 *
 */
void DataTransmissionHandler::PostExecuteEvent(){
	//hal_printf("DataTransmissionHandler::PostExecuteEvent\n");
	g_omac_RadioControl.Stop();
	g_omac_scheduler.PostExecution();
}



/*
 *
 */
void DataTransmissionHandler::DataBeaconReceive(UINT8 type, Message_15_4_t *msg, UINT8 size){
	/*
	IEEE802_15_4_Header_t * headerPtr = msg->GetHeader();
	 // For sender, make sure that
	 // 1) we're waiting for a beacon : m_state == S_DATA_PRELOADED
	 // 2) the message is actually a beacon and not an ack:
	 //    type == OMAC_DATA_BEACON_TYPE && dest == BCAST
	 // 3) that it's from the receiver we're expecting
	switch (g_OMAC.m_omac_scheduler.ProtoState.GetState()) {
	  case S_DATA_PRELOADED:
		if (headerPtr->src == m_nextDestination && type == OMAC_DATA_BEACON_TYPE) {
		  if (g_OMAC.m_omac_scheduler.InputState.IsState(I_DWELL_SEND) && headerPtr->dest == RADIO_BROADCAST_ADDRESS) {
			 m_receivedDataBeacon = TRUE;
#ifdef OMAC_DEBUG
			hal_printf("tx1\n");
#endif
			call FIFOPreload.txPacket();
			atomic call PreloadTimer.stop();

			//the second condition is important. ControlBeaconHandlerC directly operates on
			//m_piggybackBeaconCopy. So, do not modify it while it is being processed.
#ifndef ORIGINAL_OMAC
			if (headerPtr->type == AM_TIMESYNCMSG) {
			  call TimeSyncInfo.insertBeacon(msg);
			  hal_printf("piggy1\n");
			}
#endif
		  }
		  //if we find out that some receiver is in the process of receiving,
		  //then cancel the current transmission
		  else if (headerPtr->dest != CPU_Radio_GetAddress(this->radioName)) {
			hal_printf("cancel2\n");
			cancelTX();
			call FIFOPreload.txPacket();
			atomic call PreloadTimer.stop();
		  }
		}
		break;
	  case S_SENDING_DATA:
		// if we receive another data beacon after receiving our data beacon
		// cancel the tx as it might lead to collisions
#ifdef ORIGINAL_OMAC
		if (type == IEEE154_TYPE_DATA && headerPtr->dest == TOS_NODE_ID
			&& headerPtr->type == AM_TIMESYNCMSG) {

		  call TimeSyncInfo.insertBeacon(msg);
		  hal_printf("piggy2\n");
		}
#endif
		if (type == OMAC_DATA_BEACON_TYPE) {
		  //ack for others. cancel our transmission and go to sleep
		  hal_printf("cancel3\n");
		  cancelTX();
		}
		break;
	  default:
		break;
		//for receiver: we are waiting for data packet after our beacon
	}
*/
}

typedef struct  {
	UINT32 MSGID;
	char* msgContent;
}Payload_t_ping;

/*
 *
 */
bool DataTransmissionHandler::Send(){
	ASSERT(m_outgoingEntryPtr != NULL);

	DeviceStatus rs;

	//Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
	//m_outgoingEntryPtr->GetHeader()->dest = sn->MacAddress;

	m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;
	Message_15_4_t* msgPtr = m_outgoingEntryPtr;

	//Send only when packet has been scheduled
	if(isDataPacketScheduled){
		UINT16 dest = m_outgoingEntryPtr->GetHeader()->dest;
		UINT16 msgsize = m_outgoingEntryPtr->GetMessageSize();
		UINT64 time_stamp = m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp() ;

		/*IEEE802_15_4_Header_t * header = m_outgoingEntryPtr->GetHeader();
		header->dest = dest;
		header->type = MFM_DATA;

		CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
		//if(dest == g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed) {
		//	DeviceStatus ds = g_omac_scheduler.m_DiscoveryHandler.Beacon(dest, &(g_omac_scheduler.m_DiscoveryHandler.m_discoveryMsgBuffer));
		//}
		DeviceStatus rs = g_omac_RadioControl.Send_TimeStamped(dest, msgPtr, msgsize,  time_stamp );
		CPU_GPIO_SetPinState( DATATX_PIN, FALSE );

		return true;*/

		////hal_printf("DataTransmissionHandler::Send elements in sendBuffer: %d\n", g_send_buffer.GetNumberMessagesInBuffer());
		/*Payload_t_ping* pingPayload = (Payload_t_ping*)m_outgoingEntryPtr->GetPayload();
		if(pingPayload->MSGID > 29){
			hal_printf("Reached 75\n");
			Payload_t_ping* pingPayload = (Payload_t_ping*)m_outgoingEntryPtr->GetPayload();
		}*/

		if (m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp() == 0) {
			////hal_printf("DataTransmissionHandler::Send payload size %u\n", m_outgoingEntryPtr->GetPayloadSize());
			rs = g_omac_RadioControl.Send(dest, m_outgoingEntryPtr, m_outgoingEntryPtr->GetHeaderSize()+m_outgoingEntryPtr->GetPayloadSize(), 0 );
		}
		else {
			////hal_printf("DataTransmissionHandler::Send payload size %u\n", m_outgoingEntryPtr->GetPayloadSize());
			rs = g_omac_RadioControl.Send(dest, m_outgoingEntryPtr, m_outgoingEntryPtr->GetHeaderSize()+m_outgoingEntryPtr->GetPayloadSize(), time_stamp  );
		}
		//}

		//set flag to false after packet has been sent
		isDataPacketScheduled = false;

		if(rs != DS_Success)
			return false;
		else
			return true;
	}
	else{
		return false;
	}
}

/*
 * Schedule a data packet only if a neighbor is found or there are msgs in the buffer
 */
BOOL DataTransmissionHandler::ScheduleDataPacket()
{
	// do not schedule a new tx if
	// 1) we've already scheduled an tx task (m_nextTXTicks >= now), or
	// 2) we have nothing to send
	//3) when the radio is idle.

	if ( g_send_buffer.GetNumberMessagesInBuffer() > 0 ){
		UINT16 dest;
		Neighbor_t* neighborEntry;
		m_outgoingEntryPtr = g_send_buffer.GetOldest();
		if (m_outgoingEntryPtr == NULL) {
			//hal_printf("m_outgoingEntryPtr is null\n");
			return FALSE;
		}
		dest = m_outgoingEntryPtr->GetHeader()->dest;
		neighborEntry =  g_NeighborTable.GetNeighborPtr(dest);
		if (neighborEntry != NULL) {
			if (neighborEntry->MacAddress != dest) {
				hal_printf("DataTransmissionHandler::ScheduleDataPacket() incorrect neighbor returned\n");
				return FALSE;
			}
			UINT64 y = HAL_Time_CurrentTicks();
			UINT64 neighborTimeinTicks = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, HAL_Time_CurrentTicks());
			if (neighborTimeinTicks == 0){
				hal_printf("DataTransmissionHandler::ScheduleDataPacket() neighbor time is not known!!!\n");
				return FALSE;
			}
			UINT32 neighborSlot = g_omac_scheduler.GetSlotNumberfromTicks(neighborTimeinTicks);
			if(neighborSlot >= neighborEntry->nextwakeupSlot) {
				g_omac_scheduler.m_DataReceptionHandler.UpdateSeedandCalculateWakeupSlot(neighborEntry->nextwakeupSlot, neighborEntry->nextSeed, neighborEntry->mask, (UINT32)neighborEntry->seedUpdateIntervalinSlots, g_omac_scheduler.GetSlotNumberfromTicks(neighborTimeinTicks) );
			}
			isDataPacketScheduled = true;
			return TRUE;
		}
		else {
			hal_printf("Cannot find nbr %u\n", dest);
			return FALSE;
		}
	}
	else{
		return FALSE;
	}
}

