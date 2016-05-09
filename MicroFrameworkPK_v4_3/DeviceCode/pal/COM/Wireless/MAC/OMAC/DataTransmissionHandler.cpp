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


extern OMACType g_OMAC;

const uint EXECUTE_WITH_CCA = 0;
const uint FAST_RECOVERY = 1;
const uint FAST_RECOVERY2 = 0;

void PublicDataTxCallback(void * param){
	if(	FAST_RECOVERY) {
		g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.SendRetry();
	}
	else{
		g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.PostExecuteEvent();
	}
}

void PublicFastRecoveryCallback(void* param){
	g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.SendRetry();
}

void ClearMsgContents(Message_15_4_t* msg){
	IEEE802_15_4_Header_t* header = msg->GetHeader();
	int length = header->length;
	header->fcf.fcfWordValue = 0;
	header->dest = 0;
	header->length = 0;
	header->payloadType = 0;
	header->flags = 0;

	DataMsg_t* data_msg = (DataMsg_t*)msg->GetPayload();
	//data_msg->msg_identifier = 0;
	int size = data_msg->size;
	data_msg->size = 0;
	for(int i = 0; i < length; i++){
		data_msg->payload[i] = 0;
	}

	IEEE802_15_4_Metadata_t* metadata = msg->GetMetaData();
	metadata->SetRetryAttempts(0);
	metadata->SetReceiveTimeStamp((INT64)0);
	metadata->SetReceiveTimeStamp((UINT32)0);
	metadata->SetRssi(0);
	metadata->SetLqi(0);
	metadata->SetNetwork(0);
}

/*
 *
 */
void DataTransmissionHandler::Initialize(){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(DATATX_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(DATATX_DATA_PIN, TRUE);
	CPU_GPIO_SetPinState( DATATX_DATA_PIN, FALSE );
	CPU_GPIO_EnableOutputPin(DATARX_NEXTEVENT, TRUE);
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );

	CPU_GPIO_EnableOutputPin(FAST_RECOVERY_SEND, TRUE);
	CPU_GPIO_SetPinState( FAST_RECOVERY_SEND, FALSE );
	CPU_GPIO_EnableOutputPin(DATATX_SEND_ACK_HANDLER, TRUE);
	CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
	//CPU_GPIO_EnableOutputPin(HW_ACK_PIN, TRUE);
	//CPU_GPIO_SetPinState( HW_ACK_PIN, FALSE );
	CPU_GPIO_SetPinState( DATARX_NEXTEVENT, FALSE );
	CPU_GPIO_EnableOutputPin(DATATX_POSTEXEC, TRUE);
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
	CPU_GPIO_EnableOutputPin(DATATX_RECV_HW_ACK, TRUE);
	CPU_GPIO_SetPinState( DATATX_RECV_HW_ACK, FALSE );
	CPU_GPIO_EnableOutputPin(DATATX_SCHED_DATA_PKT, TRUE);
	CPU_GPIO_SetPinState( DATATX_SCHED_DATA_PKT, FALSE );

	CPU_GPIO_EnableOutputPin(OMAC_RX_DATAACK_PIN, FALSE);
#endif

	isDataPacketScheduled = false;
	m_currentSlotRetryAttempt = 0;
	m_RANDOM_BACKOFF = 0;
	m_backoff_mask = 137 * 29 * (g_OMAC.GetMyAddress() + 1);
	m_backoff_seed = 119 * 119 * (g_OMAC.GetMyAddress() + 1); // The initial seed
	//m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_TRANSMITTER, 0, MAX_PACKET_TX_DURATION_MICRO, TRUE, FALSE, PublicDataTxCallback, OMACClockSpecifier); //1 sec Timer in micro seconds
	ASSERT_SP(rm == TimerSupported);

}

UINT64 DataTransmissionHandler::CalculateNextTxMicro(UINT16 dest){
	UINT64 nextTXTicks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(dest, g_NeighborTable.GetNeighborPtr(dest)->nextwakeupSlot * SLOT_PERIOD_TICKS);
	//UINT64 nextTXmicro = g_OMAC.m_Clock.ConvertTickstoMicroSecs(nextTXTicks) - PROCESSING_DELAY_BEFORE_TX_MICRO - RADIO_TURN_ON_DELAY_MICRO;
	UINT64 nextTXmicro = g_OMAC.m_Clock.SubstractMicroSeconds( g_OMAC.m_Clock.ConvertTickstoMicroSecs(nextTXTicks) , (RADIO_TURN_ON_DELAY_MICRO+PROCESSING_DELAY_BEFORE_TX_MICRO));
	if(EXECUTE_WITH_CCA){
		nextTXmicro -= CCA_PERIOD_MICRO;
	}
	if(FAST_RECOVERY){
		nextTXmicro -= GUARDTIME_MICRO;
	}
	if(FAST_RECOVERY2){
	}
	return nextTXmicro;
}

UINT64 DataTransmissionHandler::CalculateNextRXOpp(UINT16 dest){
	if(UpdateNeighborsWakeUpSlot(dest, 0)) {


		UINT64 nextTXmicro = CalculateNextTxMicro(dest);
		UINT64 curmicro =  g_OMAC.m_Clock.ConvertTickstoMicroSecs(g_OMAC.m_Clock.GetCurrentTimeinTicks());

		while(nextTXmicro  <= curmicro + OMAC_SCHEDULER_MIN_REACTION_TIME_IN_MICRO) {
			if(UpdateNeighborsWakeUpSlot(dest, 1)){
				nextTXmicro = CalculateNextTxMicro(dest);
				curmicro =  g_OMAC.m_Clock.ConvertTickstoMicroSecs(g_OMAC.m_Clock.GetCurrentTimeinTicks());
			}
			else{
				nextTXmicro = MAX_UINT64;
				break;
			}
		}
		ASSERT_SP(nextTXmicro > curmicro);
		UINT64 remMicroSecnextTX = nextTXmicro - curmicro;
		//Wake up the transmitter a little early
		//remMicroSecnextTX -= GUARDTIME_MICRO;


#ifdef OMAC_DEBUG_PRINTF
		hal_printf("DataTransmissionHandler::NextEvent curTicks: %llu; nextTXTicks: %llu; remMicroSecnextTX: %llu\n", curTicks, nextTXTicks, remMicroSecnextTX);
#ifdef def_Neighbor2beFollowed
		//hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler::NextEvent() remMicroSecnextTX= %llu AbsnextWakeupTimeInMicSec= %llu - %lu m_neighborNextEventTimeinMicSec = %llu - %lu\n"
		//		, g_OMAC.m_Clock.ConvertTickstoMicroSecs(curTicks), g_OMAC.m_omac_scheduler.GetSlotNumber(), g_OMAC.m_Clock.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks))
		//		, remMicroSecnextTX, g_OMAC.m_Clock.ConvertTickstoMicroSecs(nextTXTicks), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(nextTXTicks), g_OMAC.m_Clock.ConvertTickstoMicroSecs(g_NeighborTable.GetNeighborPtr(_m_outgoingEntryPtr->GetHeader()->dest)->nextwakeupSlot * SLOT_PERIOD_TICKS), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_NeighborTable.GetNeighborPtr(_m_outgoingEntryPtr->GetHeader()->dest)->nextwakeupSlot * SLOT_PERIOD_TICKS) );
#endif
#endif

		/*UINT64 neighborSlot = g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, g_OMAC.m_Clock.GetCurrentTimeinTicks()));
		Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(dest);
		UINT64 neighborwakeUpSlot = neigh_ptr->nextwakeupSlot;
		if(neighborwakeUpSlot - neighborSlot < 20 ){
			neighborwakeUpSlot = neighborwakeUpSlot+1;
		}*/
		CPU_GPIO_SetPinState( DATARX_NEXTEVENT, FALSE );
		return remMicroSecnextTX;
	}
	else {
		CPU_GPIO_SetPinState( DATARX_NEXTEVENT, FALSE );
		//Either Dont have packet to send or missing timing for the destination
		return MAX_UINT64;
	}

}

/*
 * This function returns the number of ticks until the transmission time
 */
UINT64 DataTransmissionHandler::NextEvent(){
	//in case the task delay is large and we are already pass
	//tx time, tx immediately
	CPU_GPIO_SetPinState( DATARX_NEXTEVENT, TRUE );


// Check all elements in the buffer
//	UINT64 remMicroSecnextTX = MAX_UINT64;
//	m_outgoingEntryPtr_pos = Buffer_15_4_t_SIZE;
//	m_outgoingEntryPtr_dest = 0;
//	UINT8 curpos = g_send_buffer.GetFirstFullBufferPosition();
//	for(UINT8 i = 0; i < g_send_buffer.GetNumberMessagesInBuffer(); ++i){
//		if(g_send_buffer.PeekElementAtPosition(m_outgoingEntryPtr_pos)->GetMetaData()->GetRetryAttempts() < FRAMERETRYMAXATTEMPT){
//			if( (m_outgoingEntryPtr_pos == Buffer_15_4_t_SIZE || m_outgoingEntryPtr_dest != g_send_buffer.PeekElementAtPosition(m_outgoingEntryPtr_pos)->GetHeader()->dest)
//			&& CalculateNextRXOpp(g_send_buffer.PeekElementAtPosition(curpos)) < remMicroSecnextTX
//			){
//				m_outgoingEntryPtr_pos = curpos;
//				m_outgoingEntryPtr_dest = g_send_buffer.PeekElementAtPosition(m_outgoingEntryPtr_pos)->GetHeader()->dest;
//			}
//		}
//		else{
//			g_send_buffer.DropElementAtPosition(curpos);
//		}
//		++curpos;
//	}

	//Check all Neighbors that have a packet in the queue
	isDataPacketScheduled = false;
	m_outgoingEntryPtr_dest = 0;
	UINT64 cur_remMicroSecnextTX, remMicroSecnextTX = MAX_UINT64;
	for(UINT8 i = 0; i < MAX_NEIGHBORS ; ++i){
		if(g_NeighborTable.Neighbor[i].Status != Dead){
			// Readjust the neighbors queues
			while(g_NeighborTable.Neighbor[i].tsr_send_buffer.GetNumberMessagesInBuffer() > 0
					&& g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts() > FRAMERETRYMAXATTEMPT // This can be handled more gracefully
				  ){
				ClearMsgContents(g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval());
				g_NeighborTable.Neighbor[i].tsr_send_buffer.DropOldest(1);
			}
			while(g_NeighborTable.Neighbor[i].send_buffer.GetNumberMessagesInBuffer() > 0
					&& g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts() > FRAMERETRYMAXATTEMPT
				  ){
				ClearMsgContents(g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval());
				g_NeighborTable.Neighbor[i].send_buffer.DropOldest(1);
			}


			if(g_NeighborTable.Neighbor[i].send_buffer.GetNumberMessagesInBuffer() > 0 ) {
				cur_remMicroSecnextTX = CalculateNextRXOpp(g_NeighborTable.Neighbor[i].MacAddress);
				if(cur_remMicroSecnextTX < remMicroSecnextTX){
					remMicroSecnextTX = cur_remMicroSecnextTX;
					m_outgoingEntryPtr_dest = g_NeighborTable.Neighbor[i].MacAddress;
					isDataPacketScheduled = true;
				}
			}
			else if(g_NeighborTable.Neighbor[i].tsr_send_buffer.GetNumberMessagesInBuffer() > 0 ){
				cur_remMicroSecnextTX = CalculateNextRXOpp(g_NeighborTable.Neighbor[i].MacAddress);
				if(cur_remMicroSecnextTX < remMicroSecnextTX){
					remMicroSecnextTX = cur_remMicroSecnextTX;
					m_outgoingEntryPtr_dest = g_NeighborTable.Neighbor[i].MacAddress;
					isDataPacketScheduled = true;
				}
			}
		}
	}

	//At this point we have decided to send the packet to m_outgoingEntryPtr_dest
	if(isDataPacketScheduled) {
		return CalculateNextRXOpp(m_outgoingEntryPtr_dest);
	}
	else {
		return MAX_UINT64;
	}
}

void DataTransmissionHandler::DropPacket(){
	//Packet will have to be dropped if retried max attempts
#ifdef OMAC_DEBUG_PRINTF
	hal_printf("dropping packet\n");
#endif
	Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr_dest);
	if(neigh_ptr->send_buffer.GetNumberMessagesInBuffer() > 0 && m_outgoingEntryPtr == neigh_ptr->send_buffer.GetOldestwithoutRemoval() ) {
		ClearMsgContents(neigh_ptr->send_buffer.GetOldestwithoutRemoval());
		neigh_ptr->send_buffer.DropOldest(1);

		if((neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0)	&& (m_outgoingEntryPtr->GetHeader()->flags & MFM_TIMESYNC_FLAG)
		){ //This is flushing the time sync message queue if the previous message was successful
			ClearMsgContents(neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval());
			neigh_ptr->tsr_send_buffer.DropOldest(1);
		}
	}
	else if(neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0 && m_outgoingEntryPtr == neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval() ){
		ClearMsgContents(neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval());
		neigh_ptr->tsr_send_buffer.DropOldest(1);
		//neigh_ptr->tsr_send_buffer.ClearBuffer();
	}
	else{ // The packet is gone
		ASSERT_SP(0);
	}

	isDataPacketScheduled = false;

	m_currentSlotRetryAttempt = 0;
}

void DataTransmissionHandler::SendRetry(){ // BK: This function is called to retry in the case of FAST_RECOVERY
	VirtualTimerReturnMessage rm;
	m_currentSlotRetryAttempt++;

	if(FAST_RECOVERY && txhandler_state == DTS_WAITING_FOR_ACKS && m_currentSlotRetryAttempt < SLOTRETRYMAXATTEMPT){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( FAST_RECOVERY_SEND, TRUE );
	CPU_GPIO_SetPinState( FAST_RECOVERY_SEND, FALSE );
#endif
		//Enable m_RANDOM_BACKOFF for retries
		m_RANDOM_BACKOFF = true;
		ExecuteEventHelper();
	}
	else{
		PostExecuteEvent();
	}
}

void DataTransmissionHandler::ExecuteEventHelper() { // BK: This function starts sending routine for a packet
	bool canISend = true;
	UINT64 y;
	DeviceStatus DS = DS_Success;
	VirtualTimerReturnMessage rm;

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
#endif

	//The number 500 was chosen arbitrarily. In reality it should be the sum of backoff period + CCA period + guard band.
	//For GUARDTIME_MICRO period check the channel before transmitting
	//140 usec is the time taken for CCA to return a result
	if(EXECUTE_WITH_CCA) y = g_OMAC.m_Clock.GetCurrentTimeinTicks();

	while(EXECUTE_WITH_CCA){
		//If retrying, don't do CCA, but perform random backoff and transmit
		if(m_currentSlotRetryAttempt > 0){
			break;
		}
		//Check CCA only for DATA packets
		DS = CPU_Radio_ClearChannelAssesment(g_OMAC.radioName);

		if(DS != DS_Success){
			hal_printf("transmission detected!\n");
			//i = GUARDTIME_MICRO/140;
			canISend = false;
			break;
		}
		canISend = true;

		if(m_currentSlotRetryAttempt == 0){
			if( g_OMAC.m_Clock.ConvertTickstoMicroSecs(g_OMAC.m_Clock.GetCurrentTimeinTicks() - y) > CCA_PERIOD_MICRO){
				break;
			}
		}
		else{
			if( g_OMAC.m_Clock.ConvertTickstoMicroSecs(g_OMAC.m_Clock.GetCurrentTimeinTicks() - y) > CCA_PERIOD_FRAME_RETRY_MICRO){
				break;
			}
		}
	}

	//Perform CCA for random backoff period (only for retries)
	if(m_RANDOM_BACKOFF){
		UINT16 randVal = g_OMAC.m_omac_scheduler.m_seedGenerator.RandWithMask(&m_backoff_seed, m_backoff_mask);
		m_backoff_seed = randVal;
		int i = 0;
		int finalBackoffValue = randVal % RANDOM_BACKOFF_COUNT_MAX;
		//hal_printf("rand value is %d\n", (randVal % RANDOM_BACKOFF_COUNT));
		while(i <= (randVal % RANDOM_BACKOFF_COUNT_MAX)){
			++i;
			DS = CPU_Radio_ClearChannelAssesment(g_OMAC.radioName);
			if(DS != DS_Success){
				//hal_printf("transmission detected (inside backoff)!\n");
				canISend = false;
				break;
			}
		}
	}
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
#endif
	//Transmit
	if(canISend){
		//resendSuccessful = false;

		bool rv = Send();
		if(rv) {
			if(!SOFTWARE_ACKS && !HARDWARE_ACKS){
				DropPacket();
			}
		}
		else{
#ifdef OMAC_DEBUG_GPIO
		//hal_printf("DataTransmissionHandler::ExecuteEventHelper Toggling\n");
		CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
		CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
		CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
		CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
#endif
		}
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, MAX_PACKET_TX_DURATION_MICRO, TRUE, OMACClockSpecifier );
		//rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, FAST_RECOVERY_WAIT_PERIOD_MICRO, TRUE, OMACClockSpecifier );
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
	else{ //I cannot send stop execution
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 0, TRUE, OMACClockSpecifier );
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
#endif

}

/*
 *
 */
void DataTransmissionHandler::ExecuteEvent(){
#ifdef OMAC_DEBUG_PRINTF
#ifdef def_Neighbor2beFollowed
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler:ExecuteEvent\n"
			, g_OMAC.m_Clock.ConvertTickstoMicroSecs(g_OMAC.m_Clock.GetCurrentTimeinTicks()), g_OMAC.m_omac_scheduler.GetSlotNumber(), g_OMAC.m_Clock.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, g_OMAC.m_Clock.GetCurrentTimeinTicks())), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, g_OMAC.m_Clock.GetCurrentTimeinTicks())) );
#endif
#endif

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
#endif

	VirtualTimerReturnMessage rm;
	//When starting a new send, reset attempt to 0
	m_currentSlotRetryAttempt = 0;
	m_RANDOM_BACKOFF = false;

	txhandler_state = DTS_EXECUTE_START;
	DeviceStatus e = DS_Fail;
	e = g_OMAC.m_omac_RadioControl.StartRx();

	if(e == DS_Success){
		this->ExecuteEventHelper();
		txhandler_state = DTS_WAITING_FOR_ACKS;
	}
	else{
		hal_printf("Radio not in RX state\n");
		txhandler_state = DTS_RADIO_START_FAILED;
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 0, TRUE, OMACClockSpecifier );
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
}

void DataTransmissionHandler::SendACKHandler(Message_15_4_t* rcv_msg, UINT8 radioAckStatus){
	txhandler_state = DTS_SEND_FINISHED;
	RadioAddress_t src = 0;
	RadioAddress_t myID = g_OMAC.GetMyAddress();
	VirtualTimerReturnMessage rm;

	if(HARDWARE_ACKS){
		//resendSuccessful = true;
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RX_DATAACK_PIN, TRUE);
		//CPU_GPIO_SetPinState( HW_ACK_PIN, TRUE );
#endif
		if(radioAckStatus == TRAC_STATUS_SUCCESS || radioAckStatus == TRAC_STATUS_SUCCESS_DATA_PENDING){
			CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
			CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
			//Drop data packets only if send was successful
			DropPacket();
			//set flag to false after packet has been sent and ack received

			m_currentSlotRetryAttempt = 0;
			//m_currentFrameRetryAttempt = 0;
			src = rcv_msg->GetHeader()->src;
			if(src != myID){
				DeviceStatus ds = g_NeighborTable.RecordLastHeardTime(src,g_OMAC.m_Clock.GetCurrentTimeinTicks());
				if(ds != DS_Success){
					hal_printf("DataTransmissionHandler::SendACKHandler RecordLastHeardTime failure; address: %d; line: %d\n", src, __LINE__);
				}
			}

			txhandler_state = DTS_RECEIVEDDATAACK;
			rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
			rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 0, TRUE, OMACClockSpecifier );
			rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RX_DATAACK_PIN, FALSE);
		//CPU_GPIO_SetPinState( HW_ACK_PIN, TRUE );
#endif
			if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
				PostExecuteEvent();
			}
		}
		else if(radioAckStatus == TRAC_STATUS_NO_ACK || radioAckStatus == TRAC_STATUS_FAIL_TO_SEND){
			//Drop timesync packets irrespective of whether send was successful or not.
			//Don't retry a TS packet (for now)
			/*if(rcv_msg->GetHeader()->payloadType == MFM_TIMESYNCREQ){
				m_currentSlotRetryAttempt = 0;
				m_currentFrameRetryAttempt = 0;
				//isDataPacketScheduled = false;
				//m_outgoingEntryPtr = NULL;
				g_send_buffer.DropOldest(1);
			}
			else if(m_outgoingEntryPtr->GetHeader()->payloadType == MFM_DATA){
				m_currentSlotRetryAttempt++;
			}*/

			if(radioAckStatus == TRAC_STATUS_NO_ACK){
				if(FAST_RECOVERY2){
					g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.UnsuccessfulTransmission(m_outgoingEntryPtr_dest);
				}
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
			}
			else if(radioAckStatus == TRAC_STATUS_FAIL_TO_SEND){
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
			}
			txhandler_state = DTS_WAITING_FOR_ACKS;
			rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
			rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 0, TRUE, OMACClockSpecifier );
			rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
			//if(rm == TimerSupported) txhandler_state = DTS_WAITING_FOR_ACKS;
			if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
				PostExecuteEvent();
			}
		}
		/*else if(radioAckStatus == TRAC_STATUS_FAIL_TO_SEND){
			CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
			CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
			CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
			CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
			CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
			CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
			txhandler_state = DTS_WAITING_FOR_ACKS;
			rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
			rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 2000, TRUE, OMACClockSpecifier );
			rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
			//if(rm == TimerSupported) txhandler_state = DTS_WAITING_FOR_ACKS;
			if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
				PostExecuteEvent();
			}
		}*/
		else{
			hal_printf("radioAckStatus: %d\n", radioAckStatus);
			ASSERT_SP(0);
		}
	}
	else if(SOFTWARE_ACKS){
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, ACK_RX_MAX_DURATION_MICRO, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm == TimerSupported) txhandler_state = DTS_WAITING_FOR_ACKS;
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
	else {
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 0, TRUE, OMACClockSpecifier ); //Set up a timer with 0 microsecond delay (that is ideally 0 but would not make a difference)
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
}

void DataTransmissionHandler::ReceiveDATAACK(UINT16 address){
	txhandler_state = DTS_RECEIVEDDATAACK;
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RX_DATAACK_PIN, TRUE);
		//CPU_GPIO_SetPinState( HW_ACK_PIN, TRUE );
#endif
	VirtualTimerReturnMessage rm;


	if(SOFTWARE_ACKS){
		g_NeighborTable.RecordLastHeardTime(address,g_OMAC.m_Clock.GetCurrentTimeinTicks());
		DropPacket(); // The decision for dropping the packet depends on the outcome of the data reception
	}

	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm == TimerSupported){
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 0, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RX_DATAACK_PIN, FALSE);
		//CPU_GPIO_SetPinState( HW_ACK_PIN, FALSE );
#endif
}


/*
 *
 */
void DataTransmissionHandler::PostExecuteEvent(){
	txhandler_state = DTS_POSTEXECUTION;
	//Scheduler's PostExecution stops the radio
	g_OMAC.m_omac_RadioControl.Stop();
#ifdef OMAC_DEBUG_GPIO
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
#endif
	g_OMAC.m_omac_scheduler.PostExecution();
}

void DataTransmissionHandler::FailsafeStop(){
	VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
}



/*
 *
 */
bool DataTransmissionHandler::Send(){
	//ASSERT_SP(m_outgoingEntryPtr != NULL);

	DeviceStatus rs;
	Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr_dest);
	if(neigh_ptr == NULL){
		return false;
	}

	m_outgoingEntryPtr = NULL;
	if(neigh_ptr->send_buffer.GetNumberMessagesInBuffer() > 0 ) {
		m_outgoingEntryPtr = neigh_ptr->send_buffer.GetOldestwithoutRemoval();
	}
	else if(neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0 ){
		m_outgoingEntryPtr = neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval();
	}
	else{
		ASSERT_SP(0);
	}

	//Send only when packet has been scheduled
	if(m_outgoingEntryPtr != NULL && isDataPacketScheduled){
		UINT16 dest = m_outgoingEntryPtr->GetHeader()->dest;
		IEEE802_15_4_Header_t* header = m_outgoingEntryPtr->GetHeader();
		IEEE802_15_4_Metadata* metadata = m_outgoingEntryPtr->GetMetaData();
		g_OMAC.senderSequenceNumber = header->dsn; //BK: I don't know what this is. Seems like residue from incorrect HW ACK implementation. Ananth should check
		metadata->SetRetryAttempts(metadata->GetRetryAttempts()+1);
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATATX_DATA_PIN, TRUE );
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
#endif

		rs = g_OMAC.m_omac_RadioControl.Send(dest, m_outgoingEntryPtr, header->length);
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATATX_DATA_PIN, FALSE );
#endif

		//m_outgoingEntryPtr = NULL;
		if(rs != DS_Success){
			return false;
		}
		else{
			return true;
		}
	}
	else{
		return false;
	}
}

/* Schedule a data packet only if a neighbor is found or there are msgs in the buffer

BOOL DataTransmissionHandler::ScheduleDataPacket(UINT8 _skipperiods, UINT16 dest)
{
	CPU_GPIO_SetPinState( DATATX_SCHED_DATA_PKT, TRUE );
	CPU_GPIO_SetPinState( DATATX_SCHED_DATA_PKT, FALSE );
	// do not schedule a packet if
	// 1) Case for no data packets in line
	// 2) Case : destination does not exist in the neighbor table
	//	3) Case: No timing info is available for the destination


}
*/


BOOL DataTransmissionHandler::UpdateNeighborsWakeUpSlot(UINT16 dest, UINT8 _skipperiods){
	bool rv;
	Neighbor_t* neighborEntry =  g_NeighborTable.GetNeighborPtr(dest);
	if (neighborEntry != NULL) {
		if (neighborEntry->MacAddress != dest) {
			hal_printf("DataTransmissionHandler::ScheduleDataPacket() incorrect neighbor returned\n");
			return FALSE;
		}
		UINT64 y = g_OMAC.m_Clock.GetCurrentTimeinTicks();
		UINT64 neighborTimeinTicks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, g_OMAC.m_Clock.GetCurrentTimeinTicks());
		if (neighborTimeinTicks == 0){ //Case: No timing info is available for the destination
			//Keep the packet but do not schedule the data packet
			hal_printf("DataTransmissionHandler::ScheduleDataPacket() neighbor time is not known!!!\n");
			return FALSE;
		}
		UINT64 neighborSlot = g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(neighborTimeinTicks);
		if(neighborSlot >= neighborEntry->nextwakeupSlot) {
			rv = g_OMAC.m_omac_scheduler.m_DataReceptionHandler.UpdateSeedandCalculateWakeupSlot(neighborEntry->nextwakeupSlot, neighborEntry->nextSeed, neighborEntry->mask, neighborEntry->seedUpdateIntervalinSlots, neighborSlot );
			if (!rv) { // rv update failed something is wrong with the neighbor, refrain from sending to the neighbor
				return FALSE;
			}
		}
		while(_skipperiods > 0) {
			--_skipperiods;
			neighborSlot = neighborEntry->nextwakeupSlot;
			g_OMAC.m_omac_scheduler.m_DataReceptionHandler.UpdateSeedandCalculateWakeupSlot(neighborEntry->nextwakeupSlot, neighborEntry->nextSeed, neighborEntry->mask, neighborEntry->seedUpdateIntervalinSlots, neighborSlot );
		}
		return TRUE;
	}
	else { //Case : destination does not exist in the neighbor table
		hal_printf("Cannot find nbr %u\n", dest);
		return FALSE;
	}
}

