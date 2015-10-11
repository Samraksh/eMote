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


UINT64 DataTransmissionHandler::GetTxTicks()
{
	return m_nextTXTicks;
}

UINT32 DataTransmissionHandler::GetTxCounter()
{
	return m_nextTXCounter;
}

void DataTransmissionHandler::SetTxTicks(UINT64 tmp_nextTXTicks)
{
	m_nextTXTicks = tmp_nextTXTicks;
}

void DataTransmissionHandler::SetTxCounter(UINT32 tmp_nextTXCounter)
{
	m_nextTXCounter = tmp_nextTXCounter;
}

INT64 DataTransmissionHandler::GetNbrGlobalTime()
{
	return m_nbrGlobalTime;
}

UINT32 DataTransmissionHandler::GetNbrWakeupSlot()
{
	return m_nbrWakeupSlot;
}

void DataTransmissionHandler::SetNbrGlobalTime(INT64 tmp_nbrGlobalTime)
{
	m_nbrGlobalTime = tmp_nbrGlobalTime;
}

void DataTransmissionHandler::SetNbrWakeupSlot(UINT32 tmp_nbrWakeupSlot)
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
	//m_nextTXCounter=0;
	//m_nextTXTicks=0;
	m_lastSlot=0;
	m_collisionCnt = 0;
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
}


/*
 * This function returns the number of ticks until the transmission time
 */
UINT64 DataTransmissionHandler::NextEvent(UINT32 currentSlotNum){
	m_lastSlot++;
	//hal_printf("DataTransmissionHandler::NextEvent currentSlotNum: %u\n", currentSlotNum);
	//UINT32 tmp_nextTXCounter = GetTxCounter();
	UINT64 tmp_nextTXTicks = GetTxTicks();
	//if (g_send_buffer.Size() > 0 && tmp_nextTXCounter == 0) {
	if(m_lastSlot % 500 == 0){
		hal_printf("----------->g_send_buffer size: %u<--------------\n", g_send_buffer.Size());
	}
	if (g_send_buffer.Size() > 0) {
		if(m_lastSlot % 500 == 0){
			hal_printf("----------->CALLING ScheduleDataPacket<--------------\n");
		}
		this->ScheduleDataPacket();
	}

	//in case the task delay is large and we are already pass
	//tx time, tx immediately
	if(tmp_nextTXTicks < HAL_Time_CurrentTicks()) {
		return 0xffffffffffffffff;
	}
	else {
		UINT64 ticksInMicroSecs = CPU_TicksToMicroseconds(tmp_nextTXTicks - HAL_Time_CurrentTicks());
		UINT64 curTime = HAL_Time_CurrentTicks() / (TICKS_PER_MILLI/MICSECINMILISEC);
		hal_printf("\n[LT: %llu NT: %llu] DataTransmissionHandler::NextEvent() nextWakeupTimeInMicSec= %llu AbsnextWakeupTimeInMicSec= %llu \n",curTime, m_TimeSyncHandler.m_globalTime.Local2NeighborTime(m_TimeSyncHandler.Neighbor2beFollowed, curTime), ticksInMicroSecs,curTime+ticksInMicroSecs );
		return ticksInMicroSecs;
	}
}

/*
 *
 */
void DataTransmissionHandler::ExecuteEvent(UINT32 currentSlotNum){
	//BK: At this point there should be some message to be sent in the m_outgoingEntryPtr
	hal_printf("\n[LT: %llu NT: %llu] DataTransmissionHandler:ExecuteEvent\n",HAL_Time_CurrentTime(), g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTime()));
	bool rv = Send();
	SetTxCounter(0);
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
	hal_printf("DataTransmissionHandler::PostExecuteEvent\n");
	//stop the radio
	g_omac_scheduler.PostExecution();
}

/*
 *
 */
void DataTransmissionHandler::SetWakeup(bool shldWakeup){
	hal_printf("DataTransmissionHandler::SetWakeup\n");
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

/*
 *
 */
bool DataTransmissionHandler::Send(){
	ASSERT(m_outgoingEntryPtr != NULL);

	//Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
	//m_outgoingEntryPtr->GetHeader()->dest = sn->MacAddress;
	IEEE802_15_4_Header_t * header = m_outgoingEntryPtr->GetHeader();
	header->type = MFM_DATA;

	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );

	if (m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp() == 0) {
		hal_printf("DataTransmissionHandler::Send calling Send\n");
		DeviceStatus rs = g_omac_RadioControl.Send(m_outgoingEntryPtr->GetHeader()->dest, m_outgoingEntryPtr, m_outgoingEntryPtr->GetMessageSize()  );
	}
	else {
		hal_printf("DataTransmissionHandler::Send calling Send_TimeStamped\n");
		DeviceStatus rs = g_omac_RadioControl.Send_TimeStamped(m_outgoingEntryPtr->GetHeader()->dest, m_outgoingEntryPtr , m_outgoingEntryPtr->GetMessageSize(), m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp()  );
	}
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	return true;
}

/*
 *
 */
void DataTransmissionHandler::ScheduleDataPacket()
{
	// do not schedule a new tx if
	// 1) we've already scheduled an tx task (m_nextTXTicks >= now), or
	// 2) we have nothing to send
	//3) when the radio is idle.

	static bool allNeighborFlag = false;
	UINT64 tmp_nextTXTicks = GetTxTicks();

	if ( ( (tmp_nextTXTicks + (SLOT_PERIOD_MILLI * TICKS_PER_MILLI)) < HAL_Time_CurrentTicks() ) && g_send_buffer.GetNumberMessagesInBuffer() > 0){
		UINT16 dest;
		Neighbor_t* neighborEntry;

		// Don't reschedule another tx for packets that are being sent
		if (g_omac_scheduler.InputState.IsState(I_DATA_SEND_PENDING)) {
			//hal_printf("DataTransmissionHandler::ScheduleDataPacket input state is I_DATA_SEND_PENDING. returning...\n");
			return;
		}

		if (m_outgoingEntryPtr == NULL) {
			//m_outgoingEntryPtr = g_OMAC.FindFirstSyncedNbrMessage();
			m_outgoingEntryPtr = g_send_buffer.GetOldest();
			//m_outgoingEntryPtr = g_send_buffer.GetNextFreeBuffer();
			if (m_outgoingEntryPtr == NULL) {
				//hal_printf("m_outgoingEntryPtr is null\n");
				return;
			}
		}

		//Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
		//m_outgoingEntryPtr->GetHeader()->dest = sn->MacAddress;
		dest = m_outgoingEntryPtr->GetHeader()->dest;
		//hal_printf("DataTransmissionHandler::ScheduleDataPacket dest is: %d\n", dest);
		neighborEntry =  g_NeighborTable.GetNeighborPtr(dest);
		hal_printf("-----------------\n");
		hal_printf("neighborEntry->LastHeardTime %llu\n", neighborEntry->LastHeardTime);
		hal_printf("neighborEntry->MacAddress %u\n", neighborEntry->MacAddress);
		hal_printf("-----------------\n");

		if (neighborEntry != NULL) {
			if (neighborEntry->MacAddress != dest) {
				hal_printf("incorrect neighbor returned\n");
				tmp_nextTXTicks = 0;
			}
			hal_printf("------------>NEIGHBORENTRY IS NOT NULL<----------------\n");


			UINT64 y = HAL_Time_CurrentTicks();
			// 1st,  compute the current localTime of the neighbor
			UINT64 neighborTime = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, HAL_Time_CurrentTicks());
			// 2nd, compute neighbor's current slotNumber value. The start of the slotNumber
			// is later than the start of the clock. So, a slotNumber offset should be used
			//slotNumber = (nbrGlobalTime - neighborEntry->counterOffset) >> SLOT_PERIOD_BITS;
			// BK: The slot number now depends only on the CurrentTicks. This design reduces data exchange and also deals with cases where data slot updates are missed due to some reason.
			UINT64 NextEventTime = ( ( neighborTime/( (UINT64) WAKEUPPERIOD) ) + 1 ) * ((UINT64)WAKEUPPERIOD);
			UINT64 TicksTillNextEvent = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(Nbr2beFollowed, NextEventTime) - y;
			UINT64 MicSTillNextEvent = (HAL_Time_TicksToTime(TicksTillNextEvent));
			UINT64 ProcessingLatency = (HAL_Time_TicksToTime( HAL_Time_CurrentTicks() - y));
			tmp_nextTXTicks =  MicSTillNextEvent+ProcessingLatency;
			SetTxTicks(tmp_nextTXTicks);
		}
		else if(dest == 0xFFFF || dest == 0xC000){	//TODO (Ananth): where does c000 come from?
			if(!allNeighborFlag){
				hal_printf("Cannot find nbr %u\n", dest);
				allNeighborFlag = true;
			}
			SetTxTicks(0);
		}
		else {
			hal_printf("Cannot find nbr %u\n", dest);
			SetTxTicks(0);
		}
	}
}

