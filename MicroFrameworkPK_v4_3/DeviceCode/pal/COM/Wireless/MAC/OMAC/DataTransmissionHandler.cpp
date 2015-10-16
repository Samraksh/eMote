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

void PublicTXEndHCallback(void * param){
	g_omac_scheduler.m_DataTransmissionHandler.PostExecuteEvent();
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
	m_nextTXCounter=0;
	m_nextTXTicks=0;
	m_lastSlot=0;
	m_collisionCnt = 0;

	m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(HAL_TX_TIMER, 0, SLOT_PERIOD_MILLI * 1 * MICSECINMILISEC, TRUE, FALSE, PublicTXEndHCallback); //1 sec Timer in micro seconds

	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
}


/*
 * This function returns the number of ticks until the transmission time
 */
UINT64 DataTransmissionHandler::NextEvent(UINT32 currentSlotNum){

	//in case the task delay is large and we are already pass
	//tx time, tx immediately
	if(m_nextTXTicks < HAL_Time_CurrentTicks()) {
		ScheduleDataPacket();
	}
	if(m_nextTXTicks < HAL_Time_CurrentTicks()) {
		return 0xffffffffffffffff;
	}
	else {
		UINT64 ticksInMicroSecs = CPU_TicksToMicroseconds(m_nextTXTicks - HAL_Time_CurrentTicks());
		UINT64 curTime = HAL_Time_CurrentTicks() / (TICKS_PER_MILLI/MICSECINMILISEC);
		hal_printf("\n[LT: %llu NT: %llu] DataTransmissionHandler::NextEvent() nextWakeupTimeInMicSec= %llu AbsnextWakeupTimeInMicSec= %llu \n",curTime, g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, curTime), ticksInMicroSecs,curTime+ticksInMicroSecs );
		return ticksInMicroSecs;
	}
}

/*
 *
 */
void DataTransmissionHandler::ExecuteEvent(UINT32 currentSlotNum){
	//BK: At this point there should be some message to be sent in the m_outgoingEntryPtr
	hal_printf("\n[LT: %llu NT: %llu] DataTransmissionHandler:ExecuteEvent\n",HAL_Time_CurrentTime(), g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTime()));
	DeviceStatus e = DS_Fail;
	e = g_omac_RadioControl.StartRx();
	bool rv = Send();
	m_nextTXTicks = 0;
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

/*
 *
 */
bool DataTransmissionHandler::Send(){
	ASSERT(m_outgoingEntryPtr != NULL);

	//Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
	//m_outgoingEntryPtr->GetHeader()->dest = sn->MacAddress;

	m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;
	Message_15_4_t* msgPtr = m_outgoingEntryPtr;

	UINT16 dest = m_outgoingEntryPtr->GetHeader()->dest;
	UINT16 msgsize = m_outgoingEntryPtr->GetMessageSize();
	UINT64 time_stamp = m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp() ;

	IEEE802_15_4_Header_t * header = m_outgoingEntryPtr->GetHeader();
	header->dest = dest;
	header->type = MFM_DATA;

	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );

	DeviceStatus ds = g_omac_scheduler.m_DiscoveryHandler.Beacon(dest, &(g_omac_scheduler.m_DiscoveryHandler.m_discoveryMsgBuffer));
	DeviceStatus rs = g_omac_RadioControl.Send_TimeStamped(dest, msgPtr, msgsize,  time_stamp );

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

	if ( ( (m_nextTXTicks + (SLOT_PERIOD_MILLI * TICKS_PER_MILLI)) < HAL_Time_CurrentTicks() )
			&& g_send_buffer.GetNumberMessagesInBuffer() > 0
		){
		UINT16 dest;
		Neighbor_t* neighborEntry;


		if (m_outgoingEntryPtr == NULL) {
			//m_outgoingEntryPtr = g_OMAC.FindFirstSyncedNbrMessage();
			m_outgoingEntryPtr = g_send_buffer.GetOldest();
			//m_outgoingEntryPtr = g_send_buffer.GetNextFreeBuffer();
			if (m_outgoingEntryPtr == NULL) {
				//hal_printf("m_outgoingEntryPtr is null\n");
				return;
			}
		}
		dest = m_outgoingEntryPtr->GetHeader()->dest;
		neighborEntry =  g_NeighborTable.GetNeighborPtr(dest);
		if (neighborEntry != NULL) {
			if (neighborEntry->MacAddress != dest) {
				hal_printf("DataTransmissionHandler::ScheduleDataPacket() incorrect neighbor returned\n");
			}

			UINT64 y = HAL_Time_CurrentTicks();
			UINT64 neighborTime = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, HAL_Time_CurrentTicks());
			UINT32 neighborSlot = g_omac_scheduler.GetSlotNumber(neighborTime);
			//TODO: This part needs to be changed to reflect the chnages in wakeup scheduler
			if(neighborSlot >= neighborEntry->nextwakeupSlot) {
				g_omac_scheduler.m_DataReceptionHandler.UpdateSeedandCalculateWakeupSlot(neighborEntry->nextwakeupSlot, neighborEntry->nextSeed, neighborEntry->mask, neighborEntry->seedUpdateIntervalinSlots,  g_omac_scheduler.GetSlotNumber(neighborTime) );
			}

			// 2nd, compute neighbor's current slotNumber value. The start of the slotNumber
			// is later than the start of the clock. So, a slotNumber offset should be used
			//slotNumber = (nbrGlobalTime - neighborEntry->counterOffset) >> SLOT_PERIOD_BITS;
			// BK: The slot number now depends only on the CurrentTicks. This design reduces data exchange and also deals with cases where data slot updates are missed due to some reason.
			UINT64 NextEventTime = neighborEntry->nextwakeupSlot * SLOT_PERIOD_TICKS;
			//UINT64 NextEventTime = ( ( neighborTime/( (UINT64) WAKEUPPERIODINTICKS) ) + 1 ) * ((UINT64)WAKEUPPERIODINTICKS);
			UINT64 TicksTillNextEvent = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, NextEventTime) - y;
			m_nextTXTicks =  TicksTillNextEvent + HAL_Time_CurrentTicks();

			//END OF TOD O
		}
		else {
			hal_printf("Cannot find nbr %u\n", dest);
			m_nextTXTicks = 0;
		}
	}
	else{
		m_nextTXTicks = 0;
	}
}

