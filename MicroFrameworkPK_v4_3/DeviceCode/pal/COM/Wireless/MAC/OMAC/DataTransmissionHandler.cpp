/*
 * DataTransmissionHandler.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan Sridharan
 *
 *  Modified on: Oct 30, 2015
 *  	Authors: Bora Karaoglu; Ananth Muralidharan
 *
 *  Copyright The Samraksh Company
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

const uint EXECUTE_WITH_CCA = 1;
const uint FAST_RECOVERY = 1;


void PublicDataTxCallback(void* param){
	g_omac_scheduler.m_DataTransmissionHandler.PostExecuteEvent();
}

void PublicDataTxExecEventCallback(void* param){
	g_omac_scheduler.m_DataTransmissionHandler.ExecuteEventHelper();
}

void PublicFastRecoveryCallback(void* param){
	g_omac_scheduler.m_DataTransmissionHandler.SendRetry();
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
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(DATATX_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(DATATX_DATA_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(DATARX_NEXTEVENT, TRUE);
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	CPU_GPIO_EnableOutputPin(HW_ACK_PIN, TRUE);
	CPU_GPIO_SetPinState( HW_ACK_PIN, FALSE );
	CPU_GPIO_SetPinState( DATARX_NEXTEVENT, FALSE );
	CPU_GPIO_EnableOutputPin(DATATX_POSTEXEC, TRUE);
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );

#endif

	isDataPacketScheduled = false;
	currentAttempt = 0;
	maxRetryAttempts = 2;
	//m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_TRANSMITTER, 0, MAX_PACKET_TX_DURATION_MICRO, TRUE, FALSE, PublicDataTxCallback); //1 sec Timer in micro seconds
	ASSERT_SP(rm == TimerSupported);
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_TX_EXECEVENT, 0, 0, TRUE, FALSE, PublicDataTxExecEventCallback); //1 sec Timer in micro seconds
	ASSERT_SP(rm == TimerSupported);
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_FAST_RECOVERY, 0, FAST_RECOVERY_WAIT_PERIOD, TRUE, FALSE, PublicFastRecoveryCallback); //1 sec Timer in micro seconds
	ASSERT_SP(rm == TimerSupported);
}


/*
 * This function returns the number of ticks until the transmission time
 */
UINT64 DataTransmissionHandler::NextEvent(){
	//in case the task delay is large and we are already pass
	//tx time, tx immediately
	CPU_GPIO_SetPinState( DATARX_NEXTEVENT, TRUE );

	if(ScheduleDataPacket()) {
		UINT16 dest = m_outgoingEntryPtr->GetHeader()->dest;
		UINT64 nextTXTicks = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(dest, g_NeighborTable.GetNeighborPtr(dest)->nextwakeupSlot * SLOT_PERIOD_TICKS);
		UINT64 curTicks = HAL_Time_CurrentTicks();
		//ASSERT_SP(nextTXTicks > curTicks);
		UINT64 remMicroSecnextTX = HAL_Time_TicksToTime(nextTXTicks - curTicks);
		//Wake up the transmitter a little early
		remMicroSecnextTX -= GUARDTIME_MICRO;

#ifdef OMAC_DEBUG_PRINTF
		hal_printf("DataTransmissionHandler::NextEvent curTicks: %llu; nextTXTicks: %llu; remMicroSecnextTX: %llu\n", curTicks, nextTXTicks, remMicroSecnextTX);
#ifdef def_Neighbor2beFollowed
		hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler::NextEvent() remMicroSecnextTX= %llu AbsnextWakeupTimeInMicSec= %llu - %lu m_neighborNextEventTimeinMicSec = %llu - %lu\n"
				, HAL_Time_TicksToTime(curTicks), g_omac_scheduler.GetSlotNumber(), HAL_Time_TicksToTime(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), g_omac_scheduler.GetSlotNumberfromTicks(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks))
				, remMicroSecnextTX, HAL_Time_TicksToTime(nextTXTicks), g_omac_scheduler.GetSlotNumberfromTicks(nextTXTicks), HAL_Time_TicksToTime(g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr->GetHeader()->dest)->nextwakeupSlot * SLOT_PERIOD_TICKS), g_omac_scheduler.GetSlotNumberfromTicks(g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr->GetHeader()->dest)->nextwakeupSlot * SLOT_PERIOD_TICKS) );
#endif
#endif

		/*UINT64 neighborSlot = g_omac_scheduler.GetSlotNumberfromTicks(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, HAL_Time_CurrentTicks()));
		Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(dest);
		UINT64 neighborwakeUpSlot = neigh_ptr->nextwakeupSlot;
		if(neighborwakeUpSlot - neighborSlot < 20 ){
			neighborwakeUpSlot = neighborwakeUpSlot+1;
		}*/

		return remMicroSecnextTX;
	}
	else {
		//Either Dont have packet to send or missing timing for the destination
		return MAX_UINT64;
	}

	CPU_GPIO_SetPinState( DATARX_NEXTEVENT, FALSE );
}

void DataTransmissionHandler::HardwareACKHandler(){
	if(FAST_RECOVERY){
		//When a hardware ack is received, stop the 1-shot timer and drop the packet
		VirtualTimerReturnMessage rm = VirtTimer_Stop(VIRT_TIMER_OMAC_FAST_RECOVERY);
		ASSERT_SP(rm == TimerSupported);
		//hal_printf("DataTransmissionHandler::HardwareACKHandler - dropping packet\n");
		g_send_buffer.DropOldest(1);
	}
	CPU_GPIO_SetPinState( HW_ACK_PIN, TRUE );
	currentAttempt = 0;
	CPU_GPIO_SetPinState( HW_ACK_PIN, FALSE );

	VirtualTimerReturnMessage rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 0, TRUE );
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
		PostExecuteEvent();
	}
}

void DataTransmissionHandler::SendRetry(){
	currentAttempt++;
	//hal_printf("currentAttempt: %d\n", currentAttempt);
	//If retry count goes beyond the limit, packet could not reach dest, so drop it
	if(currentAttempt >= maxRetryAttempts){
		//hal_printf("Packet could not reach dest after max attempts. Dropping packet\n");
		currentAttempt = 0;
		g_send_buffer.DropOldest(1);
	}
}

void DataTransmissionHandler::ExecuteEventHelper()
{
	bool canISend = true;
	DeviceStatus DS = DS_Success;
	VirtualTimerReturnMessage rm;

	//The number 500 was chosen arbitrarily. In reality it should be the sum of backoff period + CCA period + guard band.
	//For GUARDTIME_MICRO period check the channel before transmitting
	//140 usec is the time taken for CCA to return a result
	//Do an extra count of CCA if using "Time optimized frame transmit procedure", as it is not possible
	// to check CCA before tx in that procedure.
	for(int i = 0; i < (GUARDTIME_MICRO/140); i++){
		if(EXECUTE_WITH_CCA){
			//Check CCA only for DATA packets
			if(m_outgoingEntryPtr->GetHeader()->dsn != OMAC_DISCO_SEQ_NUMBER){
				DS = CPU_Radio_ClearChannelAssesment(g_OMAC.radioName);
			}
			else{
				DS = DS_Success;
				HAL_Time_Sleep_MicroSeconds(140);
			}

			if(DS != DS_Success){
				hal_printf("transmission detected!\n");
				i = GUARDTIME_MICRO/140;
				canISend = false;
				break;
			}
			else{
				canISend = true;
			}
		}
		else{
			HAL_Time_Sleep_MicroSeconds(140);
		}
	}

#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
#endif
		if(canISend && currentAttempt < maxRetryAttempts){
			bool rv = Send();
			if(rv) {
				if(FAST_RECOVERY){
					//If send is successful, start timer for hardware ACK.
					//If hardware ack is received within stipulated period, drop the oldest packet.
					//Else retry
					rm = VirtTimer_Start(VIRT_TIMER_OMAC_FAST_RECOVERY);
					ASSERT_SP(rm == TimerSupported);
				}
				else{
					g_send_buffer.DropOldest(1);
				}
			}
			else{
#ifdef OMAC_DEBUG_GPIO
			hal_printf("DataTransmissionHandler::ExecuteEvent Toggling\n");
			CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
			CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
			CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
			CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
#endif
			}
		}
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
#endif

	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, DATATX_POST_EXEC_DELAY, TRUE );
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
		PostExecuteEvent();
	}
}

/*
 *
 */
void DataTransmissionHandler::ExecuteEvent(){
#ifdef OMAC_DEBUG_PRINTF
#ifdef def_Neighbor2beFollowed
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler:ExecuteEvent\n"
			, HAL_Time_TicksToTime(HAL_Time_CurrentTicks()), g_omac_scheduler.GetSlotNumber(), HAL_Time_TicksToTime(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, HAL_Time_CurrentTicks())), g_omac_scheduler.GetSlotNumberfromTicks(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, HAL_Time_CurrentTicks())) );
#endif
#endif

	VirtualTimerReturnMessage rm;
	DeviceStatus DS = DS_Success;
	IEEE802_15_4_Header_t* header = m_outgoingEntryPtr->GetHeader();

	DeviceStatus e = DS_Fail;
	e = g_omac_RadioControl.StartRx();

	if(e == DS_Success){
		this->ExecuteEventHelper();
		/*rm = VirtTimer_Start(VIRT_TIMER_OMAC_TX_EXECEVENT);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}*/
	}
	else{
		hal_printf("Radio not in RX state\n");
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, DATATX_POST_EXEC_DELAY, TRUE );
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}

#if 0
	//An alternate arrangement for the non availability of CCA in the radio driver
	//The number 500 was chosen arbitrarily. In reality it should be the sum of backoff period + CCA period + guard band.

	//HAL_Time_Sleep_MicroSeconds(500);
	//Start CCA only after the initial normal DISCO period
	//if(g_DiscoveryHandler.highdiscorate == false){
		//For GUARDTIME_MICRO period check the channel before transmitting
		//150 usec is the time taken for CCA to return a result
		for(int i = 0; i < (GUARDTIME_MICRO/150)+1; i++){
			DS = CPU_Radio_ClearChannelAssesment(g_OMAC.radioName);
			if(DS != DS_Success){
				hal_printf("Cannot transmit right now!\n");
				break;
			}
		}
	//}

	if (e == DS_Success){
		#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
		#endif
			bool rv = Send();
			if(rv) {
				/*if(header->type == MFM_DATA){
					hal_printf("DataTransmissionHandler::ExecuteEvent Dropping oldest packet\n");
				}*/
				g_send_buffer.DropOldest(1);
			}
			else{
		#ifdef OMAC_DEBUG_GPIO
			hal_printf("DataTransmissionHandler::ExecuteEvent Toggling\n");
			CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
			CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
			CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
			CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
		#endif
			}
		#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
		#endif
	}
	else{
		hal_printf("Radio not in RX state\n");
	}

	rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
		PostExecuteEvent();
	}
	//PostExecuteEvent();
#endif
}

/*
 *
 */
/*UINT8 DataTransmissionHandler::ExecuteEventDone(){
	return 0;
}*/

void DataTransmissionHandler::SendACKHandler(){
	/*VirtualTimerReturnMessage rm;
	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm == TimerSupported){
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, DATATX_POST_EXEC_DELAY, TRUE ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
	else{		//Could not stop timer just wait for it
	}*/
}


/*
 *
 */
void DataTransmissionHandler::PostExecuteEvent(){
	//Commenting out below as g_omac_scheduler.PostExecution() also stops radio
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
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
		  else if (headerPtr->dest != g_OMAC.GetMyAddress()) {
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
	ASSERT_SP(m_outgoingEntryPtr != NULL);

	DeviceStatus rs;

	//m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

	//Send only when packet has been scheduled
	if(m_outgoingEntryPtr != NULL && isDataPacketScheduled){
		//UINT16 fcf = m_outgoingEntryPtr->GetHeader()->fcf;
		//hal_printf("DataTransmissionHandler::Send - fcf is %d\n", fcf);
		UINT16 dest = m_outgoingEntryPtr->GetHeader()->dest;
		IEEE802_15_4_Header_t* header = m_outgoingEntryPtr->GetHeader();
		IEEE802_15_4_Metadata* metadata = m_outgoingEntryPtr->GetMetaData();
		CPU_GPIO_SetPinState( DATATX_DATA_PIN, TRUE );
		rs = g_omac_RadioControl.Send(dest, m_outgoingEntryPtr, header->length);
		CPU_GPIO_SetPinState( DATATX_DATA_PIN, FALSE );

		if(rs != DS_Success){
			return false;
		}
		else{
			//set flag to false after packet has been sent
			isDataPacketScheduled = false;
			m_outgoingEntryPtr = NULL;
			return true;
		}
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
	// do not schedule a packet if
	// 1) Case for no data packets in line
	// 2) Case : destination does not exist in the neighbor table
	//	3) Case: No timing info is available for the destination
	if (m_outgoingEntryPtr == NULL && g_send_buffer.GetNumberMessagesInBuffer() > 0 ) {//If we already have a packet
		m_outgoingEntryPtr = g_send_buffer.GetOldestwithoutRemoval();
		if (m_outgoingEntryPtr == NULL) {
			return FALSE;
		}
	}

	if ( m_outgoingEntryPtr != NULL ){
		UINT16 dest;
		Neighbor_t* neighborEntry;
		dest = m_outgoingEntryPtr->GetHeader()->dest;
		neighborEntry =  g_NeighborTable.GetNeighborPtr(dest);
		if (neighborEntry != NULL) {
			if (neighborEntry->MacAddress != dest) {
				hal_printf("DataTransmissionHandler::ScheduleDataPacket() incorrect neighbor returned\n");
				ASSERT_SP(neighborEntry->MacAddress == dest);
				isDataPacketScheduled = false;
				return FALSE;
			}
			UINT64 y = HAL_Time_CurrentTicks();
			UINT64 neighborTimeinTicks = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, HAL_Time_CurrentTicks());
			if (neighborTimeinTicks == 0){ //Case: No timing info is available for the destination
				//Keep the packet but do not schedule the data packet
				hal_printf("DataTransmissionHandler::ScheduleDataPacket() neighbor time is not known!!!\n");
				isDataPacketScheduled = false;
				return FALSE;
			}
			UINT64 neighborSlot = g_omac_scheduler.GetSlotNumberfromTicks(neighborTimeinTicks);
			if(neighborSlot >= neighborEntry->nextwakeupSlot) {
				UINT64 currentSlotNum = g_omac_scheduler.GetSlotNumberfromTicks(neighborTimeinTicks);
				g_omac_scheduler.m_DataReceptionHandler.UpdateSeedandCalculateWakeupSlot(neighborEntry->nextwakeupSlot, neighborEntry->nextSeed, neighborEntry->mask, neighborEntry->seedUpdateIntervalinSlots, currentSlotNum );
			}
			isDataPacketScheduled = true;
			return TRUE;
		}
		else { //Case : destination does not exist in the neighbor table
			//Keep the packet
			isDataPacketScheduled = false;
			hal_printf("Cannot find nbr %u\n", dest);
			return FALSE;
		}
	}
	else{ //Case for no data packets in line
		return FALSE;
	}
}

