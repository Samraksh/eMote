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


extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;
extern Buffer_15_4_t g_send_buffer;
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




/*
 *
 */
void DataTransmissionHandler::Initialize(){
	CPU_GPIO_EnableOutputPin(DATATX_PIN, TRUE);

	//m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

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

#ifdef def_Neighbor2beFollowed
		hal_printf("DataTransmissionHandler::NextEvent curTicks: %llu; nextTXTicks: %llu; remMicroSecnextTX: %llu\n", curTicks, nextTXTicks, remMicroSecnextTX);
		hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler::NextEvent() remMicroSecnextTX= %llu AbsnextWakeupTimeInMicSec= %llu - %lu m_neighborNextEventTimeinMicSec = %llu - %lu\n"
				, HAL_Time_TicksToTime(curTicks), g_omac_scheduler.GetSlotNumber(), HAL_Time_TicksToTime(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), g_omac_scheduler.GetSlotNumberfromTicks(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks))
				, remMicroSecnextTX, HAL_Time_TicksToTime(nextTXTicks), g_omac_scheduler.GetSlotNumberfromTicks(nextTXTicks), HAL_Time_TicksToTime(g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr->GetHeader()->dest)->nextwakeupSlot * SLOT_PERIOD_TICKS), g_omac_scheduler.GetSlotNumberfromTicks(g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr->GetHeader()->dest)->nextwakeupSlot * SLOT_PERIOD_TICKS) );
#endif
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
#ifdef def_Neighbor2beFollowed
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler:ExecuteEvent\n"
			, HAL_Time_TicksToTime(HAL_Time_CurrentTicks()), g_omac_scheduler.GetSlotNumber(), HAL_Time_TicksToTime(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, HAL_Time_CurrentTicks())), g_omac_scheduler.GetSlotNumberfromTicks(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, HAL_Time_CurrentTicks())) );
#endif
	DeviceStatus e = DS_Fail;
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
	bool rv = Send();
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
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

	DeviceStatus rs;

	//m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

	//Send only when packet has been scheduled
	if(isDataPacketScheduled){
		UINT16 dest = m_outgoingEntryPtr->GetHeader()->dest;
		UINT16 msgsize = m_outgoingEntryPtr->GetMessageSize();
		UINT64 time_stamp = m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp() ;

		if (m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp() == 0) {
			rs = g_omac_RadioControl.Send(dest, m_outgoingEntryPtr, m_outgoingEntryPtr->GetHeaderSize()+m_outgoingEntryPtr->GetPayloadSize(), 0 );
		}
		else {
			rs = g_omac_RadioControl.Send(dest, m_outgoingEntryPtr, m_outgoingEntryPtr->GetHeaderSize()+m_outgoingEntryPtr->GetPayloadSize(), time_stamp  );
		}

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
				g_omac_scheduler.m_DataReceptionHandler.UpdateSeedandCalculateWakeupSlot(neighborEntry->nextwakeupSlot, neighborEntry->nextSeed, neighborEntry->mask, neighborEntry->seedUpdateIntervalinSlots, g_omac_scheduler.GetSlotNumberfromTicks(neighborTimeinTicks) );
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

