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

#if defined(OMAC_DEBUG_PRINTF) || defined(DEBUG_OMAC_UPDATENEIGHBORSWAKEUPSLOT)
#define DEBUG_OMAC_UNWUS_PRINTF(...) hal_printf( __VA_ARGS__ )
#else
#define DEBUG_OMAC_UNWUS_PRINTF(...) (void)0
#endif



void PublicDataTxCallback(void * param){
	if(	FAST_RECOVERY) {
		g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.SendRetry();
	}
	else{
		g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.PostExecuteEvent();
	}
}

/*void PublicDataTxPostExecCallback(void * param){
	g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.PostExecuteEvent();
}*/

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

	CPU_GPIO_EnableOutputPin(DATATX_TIMING_ERROR_PIN_TOGGLER, TRUE);
	CPU_GPIO_SetPinState( DATATX_TIMING_ERROR_PIN_TOGGLER, FALSE );

	CPU_GPIO_EnableOutputPin(DATATX_CCA_PIN_TOGGLER, TRUE);
	CPU_GPIO_SetPinState( DATATX_CCA_PIN_TOGGLER, FALSE );


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

	CPU_GPIO_EnableOutputPin(DATATX_NEXT_EVENT, TRUE);
	CPU_GPIO_SetPinState( DATATX_NEXT_EVENT, FALSE );

	CPU_GPIO_EnableOutputPin(DATATX_RECV_SW_ACK, TRUE);
	CPU_GPIO_SetPinState( DATATX_RECV_SW_ACK, FALSE );

	CPU_GPIO_EnableOutputPin(OMAC_RX_DATAACK_PIN, FALSE);
#endif

	isDataPacketScheduled = false;
	m_currentSlotRetryAttempt = 0;
	m_RANDOM_BACKOFF = 0;
	m_backoff_mask = 137 * 29 * (g_OMAC.GetMyAddress() + 1);
	m_backoff_seed = 119 * 119 * (g_OMAC.GetMyAddress() + 1); // The initial seed
	//m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_TRANSMITTER, 0, g_OMAC.MAX_PACKET_TX_DURATION_MICRO, TRUE, FALSE, PublicDataTxCallback, OMACClockSpecifier); //1 sec Timer in micro seconds
	ASSERT_SP(rm == TimerSupported);
	//rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_TRANSMITTER_POST_EXEC, 0, ACK_RX_MAX_DURATION_MICRO, TRUE, FALSE, PublicDataTxPostExecCallback, OMACClockSpecifier);
	ASSERT_SP(rm == TimerSupported);
}

UINT64 DataTransmissionHandler::CalculateNextTxMicro(UINT16 dest){
	UINT64 nextTXmicro = MAX_UINT64;
	Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(dest);
	if(neigh_ptr == NULL) {
		return nextTXmicro;
	}
	m_scheduledTXTime_in_neigh_clock_ticks = neigh_ptr->nextwakeupSlot * SLOT_PERIOD_TICKS;
	m_scheduledTXTime_in_own_clock_ticks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(dest, m_scheduledTXTime_in_neigh_clock_ticks);
	//UINT64 nextTXmicro = g_OMAC.m_Clock.ConvertTickstoMicroSecs(nextTXTicks) - PROCESSING_DELAY_BEFORE_TX_MICRO - RADIO_TURN_ON_DELAY_MICRO;

	nextTXmicro = g_OMAC.m_Clock.SubstractMicroSeconds( g_OMAC.m_Clock.ConvertTickstoMicroSecs(m_scheduledTXTime_in_own_clock_ticks) , (g_OMAC.RADIO_TURN_ON_DELAY_TX+g_OMAC.DELAY_FROM_OMAC_TX_TO_RADIO_DRIVER_TX));
	if(EXECUTE_WITH_CCA){
		if(nextTXmicro > g_OMAC.CCA_PERIOD_ACTUAL) {
			nextTXmicro -= g_OMAC.CCA_PERIOD_ACTUAL ;
		}
	}
	if(FAST_RECOVERY){
		if(nextTXmicro > GUARDTIME_MICRO) {
			nextTXmicro -= GUARDTIME_MICRO;
		}
	}
	if(FAST_RECOVERY2){
	}
	m_scheduledFUTime_in_own_clock_micro = nextTXmicro;
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
		OMAC_HAL_PRINTF("DataTransmissionHandler::NextEvent curmicro: %llu; nextTXmicro: %llu; remMicroSecnextTX: %llu \n", curmicro, nextTXmicro, remMicroSecnextTX);
#ifdef def_Neighbor2beFollowed
		//OMAC_HAL_PRINTF("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler::NextEvent() remMicroSecnextTX= %llu AbsnextWakeupTimeInMicSec= %llu - %lu m_neighborNextEventTimeinMicSec = %llu - %lu\n"
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
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATARX_NEXTEVENT, FALSE );
#endif
		return remMicroSecnextTX;
	}
	else {
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATARX_NEXTEVENT, FALSE );
#endif
		//Either Dont have packet to send or missing timing for the destination
		return MAX_UINT64;
	}

}

void DataTransmissionHandler::SendACKToUpperLayers(Message_15_4_t* msg, UINT16 Size, NetOpStatus status, UINT8 radioAckStatus){
	SendAckFuncPtrType m_txAckHandler = NULL;
	MACEventHandler* mac_event_handler_ptr = NULL;

	if (msg != NULL && msg->GetHeader()){
		if(msg->GetHeader()->payloadType != MFM_OMAC_TIMESYNCREQ){
			if(IsValidNativeAppIdOffset(msg->GetHeader()->payloadType) ){
				mac_event_handler_ptr = g_OMAC.GetNativeAppHandler(msg->GetHeader()->payloadType);
				if(mac_event_handler_ptr != NULL) {
					m_txAckHandler = mac_event_handler_ptr->GetSendAckHandler();
				}
			}
			if(m_txAckHandler == NULL){
				m_txAckHandler = g_OMAC.m_txAckHandler;
			}
			if(m_txAckHandler != NULL){
				(m_txAckHandler)(msg, Size, status, radioAckStatus);
			}
		}
	}
}

/*
 * This function returns the number of ticks until the transmission time
 */
UINT64 DataTransmissionHandler::NextEvent(){
	//in case the task delay is large and we are already pass
	//tx time, tx immediately
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATARX_NEXTEVENT, TRUE );
	CPU_GPIO_SetPinState( DATATX_NEXT_EVENT, TRUE );
#endif



	//Check all Neighbors that have a packet in the queue
	isDataPacketScheduled = false;
	m_outgoingEntryPtr_dest = 0;
	UINT64 cur_remMicroSecnextTX, remMicroSecnextTX = MAX_UINT64;
	for(UINT8 i = 0; i < MAX_NEIGHBORS ; ++i){
		if(g_NeighborTable.Neighbor[i].neighborStatus != Dead){
			if( g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.IsNeighborTimeAvailable(g_NeighborTable.Neighbor[i].MACAddress)){
				// Readjust the neighbors queues and drop packets that have failed FRAMERETRYMAXATTEMPT

				while(g_NeighborTable.Neighbor[i].tsr_send_buffer.GetNumberMessagesInBuffer() > 0
						&& g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts() > FRAMERETRYMAXATTEMPT // This can be handled more gracefully
				){
#if defined(OMAC_DEBUG_PRINTF)
					if(g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts() > FRAMERETRYMAXATTEMPTWARNINGLEVEL){

						OMAC_HAL_PRINTF("FRAMERETRYMAXATTEMPTWARNINGLEVEL tsr_send_buffer dest = %u, payloadType = %u, GetRetryAttempts = %u \n"
								, g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->dest
								, g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->payloadType
								, g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts()
						);
					}
#endif
#if OMAC_DEBUG_PRINTF_PACKETDROP_MAXTRIES
					hal_printf("Dropping TSR Packet FRAMERETRYMAXATTEMPT dest= %u payloadType= %u, flags = %u, Retry Attempts = %u \n"
							, g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->dest
							, g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->payloadType
							, g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->flags
							, g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts());
#endif
					ClearMsgContents(g_NeighborTable.Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval());
					g_NeighborTable.Neighbor[i].tsr_send_buffer.DropOldest(1);
				}
				while(g_NeighborTable.Neighbor[i].send_buffer.GetNumberMessagesInBuffer() > 0
						&& g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts() > FRAMERETRYMAXATTEMPT
				){
#if defined(OMAC_DEBUG_PRINTF)
					if(g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts() > FRAMERETRYMAXATTEMPTWARNINGLEVEL){

						OMAC_HAL_PRINTF("FRAMERETRYMAXATTEMPTWARNINGLEVEL send_buffer dest = %u, payloadType = %u, GetRetryAttempts = %u \n"
								, g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetHeader()->dest
								, g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetHeader()->payloadType
								, g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts()
						);
					}
#endif
#if OMAC_DEBUG_PRINTF_PACKETDROP_MAXTRIES
					hal_printf("Dropping Data Packet FRAMERETRYMAXATTEMPT dest= %u payloadType= %u, flags = %u, Retry Attempts = %u \n"
							, g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetHeader()->dest
							, g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetHeader()->payloadType
							, g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetHeader()->flags
							, g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts());
#endif
					if(true){ //if(g_OMAC.m_txAckHandler != NULL){ //If user is interested in ACKS, send negative acknowledgements for packets that are getting dropped due to exceeding number of retries
						Message_15_4_t* msg = g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval();
						SendACKToUpperLayers(msg, sizeof(Message_15_4_t), NetworkOperations_SendFailed, TRAC_STATUS_NO_ACK);
#if OMAC_DTH_DEBUG_ReceiveDATAACK_PRINTOUT
				hal_printf("NextEvent:NetworkOperations_SendFailed dest = %u \r\n", msg->GetHeader()->dest);
#endif
					}
					ClearMsgContents(g_NeighborTable.Neighbor[i].send_buffer.GetOldestwithoutRemoval());
					g_NeighborTable.Neighbor[i].send_buffer.DropOldest(1);
				}


				if(g_NeighborTable.Neighbor[i].send_buffer.GetNumberMessagesInBuffer() > 0 ) {
					cur_remMicroSecnextTX = CalculateNextRXOpp(g_NeighborTable.Neighbor[i].MACAddress);
					if(cur_remMicroSecnextTX < remMicroSecnextTX){
						remMicroSecnextTX = cur_remMicroSecnextTX;
						m_outgoingEntryPtr_dest = g_NeighborTable.Neighbor[i].MACAddress;
						isDataPacketScheduled = true;
					}
				}
				else if(g_NeighborTable.Neighbor[i].tsr_send_buffer.GetNumberMessagesInBuffer() > 0 ){
					cur_remMicroSecnextTX = CalculateNextRXOpp(g_NeighborTable.Neighbor[i].MACAddress);
					if(cur_remMicroSecnextTX < remMicroSecnextTX){
						remMicroSecnextTX = cur_remMicroSecnextTX;
						m_outgoingEntryPtr_dest = g_NeighborTable.Neighbor[i].MACAddress;
						isDataPacketScheduled = true;
					}
				}
			}
		}
	}

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATATX_NEXT_EVENT, FALSE );
#endif
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
	//However this function is only used for successfull packet drops
#ifdef OMAC_DEBUG_PRINTF
	OMAC_HAL_PRINTF("dropping packet\n");
#endif
#if OMAC_DEBUG_PRINTF_PACKETDROP_SUCESS
#endif
	Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr_dest);
	if(neigh_ptr == NULL) {
		ASSERT_SP(0);
	}
	else {
		neigh_ptr->random_back_off_window_size = INITIAL_RETRY_BACKOFF_WINDOW_SIZE;
		if(neigh_ptr->IsInitializationTimeSamplesNeeded() && neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0 && m_outgoingEntryPtr == neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval() ) {
#if OMAC_DEBUG_PRINTF_PACKETDROP_SUCESS
			hal_printf("Dropping TSR Packet SUCCESS NumTimeSyncMessagesSent = %u < INITIAL_RETRY_BACKOFF_WINDOW_SIZE dest= %u payloadType= %u, flags = %u, Retry Attempts = %u \n"
					, neigh_ptr->NumTimeSyncMessagesSent
					, neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->dest
					, neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->payloadType
					, neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->flags
					, neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts());
#endif
			ClearMsgContents(neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval());
			neigh_ptr->tsr_send_buffer.DropOldest(1);
			neigh_ptr->IncrementNumTimeSyncMessagesSent();

#if OMAC_DEBUG_PRINTF_TSREQ_TX
		g_OMAC.is_print_neigh_table = true;
#endif

		}
		else if(neigh_ptr->send_buffer.GetNumberMessagesInBuffer() > 0 && m_outgoingEntryPtr == neigh_ptr->send_buffer.GetOldestwithoutRemoval() ) {
#if OMAC_DEBUG_PRINTF_PACKETDROP_SUCESS
			hal_printf("Dropping Data Packet SUCCESS dest= %u payloadType= %u, flags = %u, Retry Attempts = %u \n"
					, neigh_ptr->send_buffer.GetOldestwithoutRemoval()->GetHeader()->dest
					, neigh_ptr->send_buffer.GetOldestwithoutRemoval()->GetHeader()->payloadType
					, neigh_ptr->send_buffer.GetOldestwithoutRemoval()->GetHeader()->flags
					, neigh_ptr->send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts());
#endif
			if(true){ //if(g_OMAC.m_txAckHandler != NULL){
				Message_15_4_t* msg = neigh_ptr->send_buffer.GetOldestwithoutRemoval();
				SendACKToUpperLayers(msg, sizeof(Message_15_4_t), NetworkOperations_Success, TRAC_STATUS_SUCCESS);
#if OMAC_DTH_DEBUG_ReceiveDATAACK_PRINTOUT
				hal_printf("DropPacket:NetworkOperations_Success dest = %u \r\n", m_outgoingEntryPtr->GetHeader()->dest);
#endif
			}

			ClearMsgContents(neigh_ptr->send_buffer.GetOldestwithoutRemoval());
			neigh_ptr->send_buffer.DropOldest(1);

			if((neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0)	&& (m_outgoingEntryPtr->GetHeader()->flags & MFM_TIMESYNC_FLAG)
			){ //This is flushing the time sync message queue if the previous message was successful
				ClearMsgContents(neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval());
				neigh_ptr->tsr_send_buffer.DropOldest(1);
				neigh_ptr->IncrementNumTimeSyncMessagesSent();

#if OMAC_DEBUG_PRINTF_TSREQ_TX
		g_OMAC.is_print_neigh_table = true;
#endif

			}
		}
		else if(neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0 && m_outgoingEntryPtr == neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval() ){
#if OMAC_DEBUG_PRINTF_PACKETDROP_SUCESS
			hal_printf("Dropping TSR Packet SUCCESS DestID dest= %u payloadType= %u, flags = %u, Retry Attempts = %u \n"
					, neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->dest
					, neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->payloadType
					, neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval()->GetHeader()->flags
					, neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts());
#endif
			ClearMsgContents(neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval());
			neigh_ptr->tsr_send_buffer.DropOldest(1);
			neigh_ptr->IncrementNumTimeSyncMessagesSent();

#if OMAC_DEBUG_PRINTF_TSREQ_TX
		g_OMAC.is_print_neigh_table = true;
#endif

			//neigh_ptr->tsr_send_buffer.ClearBuffer();
		}
		else{ // The packet is gone
			//ASSERT_SP(0);
		}
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
		//VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER_POST_EXEC);
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

#if OMAC_DTH_DEBUG_CCA
	if(DATATX_CCA_PIN_TOGGLER != DISABLED_PIN){
		CPU_GPIO_SetPinState( DATATX_CCA_PIN_TOGGLER, !CPU_GPIO_GetPinState(DATATX_CCA_PIN_TOGGLER) );
		CPU_GPIO_SetPinState( DATATX_CCA_PIN_TOGGLER, !CPU_GPIO_GetPinState(DATATX_CCA_PIN_TOGGLER) );
	}
#endif
		DS = CPU_Radio_ClearChannelAssesment(g_OMAC.radioName);

#if OMAC_DTH_DEBUG_CCA
		if(DATATX_CCA_PIN_TOGGLER != DISABLED_PIN){
			CPU_GPIO_SetPinState( DATATX_CCA_PIN_TOGGLER, !CPU_GPIO_GetPinState(DATATX_CCA_PIN_TOGGLER) );
			CPU_GPIO_SetPinState( DATATX_CCA_PIN_TOGGLER, !CPU_GPIO_GetPinState(DATATX_CCA_PIN_TOGGLER) );
		}
#endif


		if(DS != DS_Success){
#ifdef OMAC_DEBUG_PRINTF
			OMAC_HAL_PRINTF("transmission detected!\n");
#endif
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
		int finalBackoffValue = randVal % g_OMAC.RANDOM_BACKOFF_COUNT_MAX;
		//OMAC_HAL_PRINTF("rand value is %d\n", (randVal % RANDOM_BACKOFF_COUNT));
		while(i <= (randVal % g_OMAC.RANDOM_BACKOFF_COUNT_MAX)){
			++i;
			DS = CPU_Radio_ClearChannelAssesment(g_OMAC.radioName);
			if(DS != DS_Success){
				//OMAC_HAL_PRINTF("transmission detected (inside backoff)!\n");
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
		txhandler_state = DTS_CCA_CLEAR;
		//resendSuccessful = false;

#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATATX_DATA_PIN, TRUE );
		CPU_GPIO_SetPinState( DATATX_DATA_PIN, FALSE );
#endif



		bool rv = Send();

		if(rv) {

#if OMAC_DEBUG_PRINTF_TXATTEMPT_SUCCESS
			if(m_outgoingEntryPtr != NULL){
				hal_printf("TXATTEMPT_SUCCESSL dest= %u payloadType= %u, flags = %u, Retry Attempts = %u \n"
						, m_outgoingEntryPtr->GetHeader()->dest
						, m_outgoingEntryPtr->GetHeader()->payloadType
						, m_outgoingEntryPtr->GetHeader()->flags
						, m_outgoingEntryPtr->GetMetaData()->GetRetryAttempts());
			}
			else{
				hal_printf("TXATTEMPT_SUCCESS NO m_outgoingEntryPtr \n");
			}
#endif


			txhandler_state = DTS_SEND_INITIATION_SUCCESS;
			if(CPU_Radio_GetRadioAckType() == NO_ACK){
				DropPacket();
			}
		}
		else{
			txhandler_state = DTS_SEND_INITIATION_FAIL;
#ifdef OMAC_DEBUG_GPIO
			//OMAC_HAL_PRINTF("DataTransmissionHandler::ExecuteEventHelper Toggling\n");
			CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
			CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
			CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
			CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
#endif
		}
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, g_OMAC.MAX_PACKET_TX_DURATION_MICRO, TRUE, OMACClockSpecifier );
		//rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, FAST_RECOVERY_WAIT_PERIOD_MICRO, TRUE, OMACClockSpecifier );
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
	else{ //I cannot send
#if OMAC_DEBUG_PRINTF_CCA_TXFAIL
		hal_printf("CCA_TXFAIL dest= %u \n",m_outgoingEntryPtr_dest);
#endif

		txhandler_state = DTS_CCA_BUSY;



		//Increment number of retries for the current packet
		m_outgoingEntryPtr = SelectPacketForDest(m_outgoingEntryPtr_dest);
		if(m_outgoingEntryPtr != NULL && isDataPacketScheduled){
			IEEE802_15_4_Metadata* metadata = m_outgoingEntryPtr->GetMetaData();
			metadata->SetRetryAttempts(metadata->GetRetryAttempts()+1);
		}

		//		Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr_dest);
		//		if(neigh_ptr != NULL){
		//			m_outgoingEntryPtr = NULL;
		//			if(neigh_ptr->send_buffer.GetNumberMessagesInBuffer() > 0 ) {
		//				m_outgoingEntryPtr = neigh_ptr->send_buffer.GetOldestwithoutRemoval();
		//			}
		//			else if(neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0 ){
		//				m_outgoingEntryPtr = neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval();
		//			}
		//			else{
		//				//Commenting out, as sometimes there may not be anything to be sent,
		//				//	in which case Send returns a false.
		//				//ASSERT_SP(0);
		//			}
		//			if(m_outgoingEntryPtr != NULL && isDataPacketScheduled){
		//				IEEE802_15_4_Metadata* metadata = m_outgoingEntryPtr->GetMetaData();
		//				metadata->SetRetryAttempts(metadata->GetRetryAttempts()+1);
		//			}
		//		}

		//Stop execution
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
	OMAC_HAL_PRINTF("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler:ExecuteEvent \n"
			, g_OMAC.m_Clock.ConvertTickstoMicroSecs(g_OMAC.m_Clock.GetCurrentTimeinTicks()), g_OMAC.m_omac_scheduler.GetSlotNumber(), g_OMAC.m_Clock.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, g_OMAC.m_Clock.GetCurrentTimeinTicks())), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, g_OMAC.m_Clock.GetCurrentTimeinTicks())) );
#endif
#endif

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
	CPU_GPIO_SetPinState( DATATX_NEXT_EVENT, TRUE );
#endif

	VirtualTimerReturnMessage rm;
	//When starting a new send, reset attempt to 0
	m_currentSlotRetryAttempt = 0;
	m_RANDOM_BACKOFF = false;
	txhandler_state = DTS_EXECUTE_START;
	DeviceStatus e = DS_Fail;

	Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr_dest);
	if(neigh_ptr == NULL){
		e = DS_Fail;
		txhandler_state = DTS_DEST_NOT_AVAILABLE;
	}
	else if(neigh_ptr->random_back_off_slot != 0){
		--(neigh_ptr->random_back_off_slot);
		e = DS_Fail;
		txhandler_state = DTS_BACKOFF;
	}
	else{
		e = g_OMAC.m_omac_RadioControl.StartRx();
		if(e == DS_Success){
			txhandler_state = DTS_RADIO_START_SUCCESS;
		}
		else{
			txhandler_state = DTS_RADIO_START_FAILED;
		}
	}


	if(e == DS_Success){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATATX_NEXT_EVENT, TRUE );
		CPU_GPIO_SetPinState( DATATX_NEXT_EVENT, FALSE );
#endif
		this->ExecuteEventHelper();
	}
	else{
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATATX_NEXT_EVENT, FALSE );
		CPU_GPIO_SetPinState( DATATX_NEXT_EVENT, TRUE );
		CPU_GPIO_SetPinState( DATATX_NEXT_EVENT, FALSE );
#endif
		//OMAC_HAL_PRINTF("DataTransmissionHandler::ExecuteEvent Radio not in RX state\n");
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 0, TRUE, OMACClockSpecifier );
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
}

void DataTransmissionHandler::SelectRetrySlotNumForNeighborBackOff(){
	Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr_dest);
	if(neigh_ptr != NULL){
		if(neigh_ptr->random_back_off_window_size >= INITIAL_RETRY_BACKOFF_WINDOW_SIZE && neigh_ptr->random_back_off_window_size < MAX_RETRY_BACKOFF_WINDOW_SIZE){
			neigh_ptr->random_back_off_window_size = neigh_ptr->random_back_off_window_size * 2;
		}
		else {
			neigh_ptr->random_back_off_window_size = INITIAL_RETRY_BACKOFF_WINDOW_SIZE;
		}
		if(neigh_ptr->random_back_off_window_size > MAX_RETRY_BACKOFF_WINDOW_SIZE){
			neigh_ptr->random_back_off_window_size = MAX_RETRY_BACKOFF_WINDOW_SIZE;
		}

		UINT16 randVal = g_OMAC.m_omac_scheduler.m_seedGenerator.RandWithMask(&m_backoff_seed, m_backoff_mask);
		m_backoff_seed = randVal;
		neigh_ptr->random_back_off_slot = randVal % neigh_ptr->random_back_off_window_size;
	}
}

void DataTransmissionHandler::SendACKHandler(Message_15_4_t* rcv_msg, UINT8 radioAckStatus){


#if OMAC_DTH_DEBUG_SendACKHandler
	if(DATATX_SendACKHandler_PIN_TOGGLER != DISABLED_PIN){
		CPU_GPIO_SetPinState( DATATX_SendACKHandler_PIN_TOGGLER, !CPU_GPIO_GetPinState(DATATX_SendACKHandler_PIN_TOGGLER) );
		CPU_GPIO_SetPinState( DATATX_SendACKHandler_PIN_TOGGLER, !CPU_GPIO_GetPinState(DATATX_SendACKHandler_PIN_TOGGLER) );
	}
#endif
	txhandler_state = DTS_SEND_FINISHED;
	RadioAddress_t dest = m_outgoingEntryPtr_dest;
	RadioAddress_t myID = g_OMAC.GetMyAddress();
	VirtualTimerReturnMessage rm;

	if(CPU_Radio_GetRadioAckType() == HARDWARE_ACK){
		//resendSuccessful = true;
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RX_DATAACK_PIN, TRUE);
		//CPU_GPIO_SetPinState( HW_ACK_PIN, TRUE );
#endif
		if(radioAckStatus == TRAC_STATUS_SUCCESS || radioAckStatus == TRAC_STATUS_SUCCESS_DATA_PENDING){
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
			CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
#endif

			if(true){ //if(g_OMAC.m_txAckHandler != NULL){
				if(m_outgoingEntryPtr->GetHeader()->payloadType != MFM_OMAC_TIMESYNCREQ){
					SendACKToUpperLayers(rcv_msg, sizeof(Message_15_4_t), NetworkOperations_SendACKed, radioAckStatus);
#if OMAC_DTH_DEBUG_ReceiveDATAACK_PRINTOUT
				hal_printf("DataTransmissionHandler:HARDWARE_ACKSendACK:NetworkOperations_SendACKed dest = %u \r\n", rcv_msg->GetHeader()->dest);
#endif

				}
			}

			//Drop data packets only if send was successful
			DropPacket();
			//set flag to false after packet has been sent and ack received

			m_currentSlotRetryAttempt = 0;
			//m_currentFrameRetryAttempt = 0;
			//dest = rcv_msg->GetHeader()->dest;
			if(dest != myID){
				DeviceStatus ds = g_NeighborTable.RecordLastHeardTime(dest, g_OMAC.m_Clock.GetCurrentTimeinTicks());
				if(ds != DS_Success){
#ifdef OMAC_DEBUG_PRINTF
					OMAC_HAL_PRINTF("DataTransmissionHandler::SendACKHandler RecordLastHeardTime failure; address: %d; line: %d\n", dest, __LINE__);
#endif
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

				if(true){//if(g_OMAC.m_txAckHandler != NULL){
					if(m_outgoingEntryPtr->GetHeader()->payloadType != MFM_OMAC_TIMESYNCREQ){
						SendACKToUpperLayers(rcv_msg, sizeof(Message_15_4_t), NetworkOperations_SendNACKed, radioAckStatus);
#if OMAC_DTH_DEBUG_ReceiveDATAACK_PRINTOUT
				hal_printf("DataTransmissionHandler:HARDWARE_ACKSendACK:NetworkOperations_SendNACKed dest = %u \r\n", rcv_msg->GetHeader()->dest);
#endif
					}
				}

				if(FAST_RECOVERY2){
					g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.UnsuccessfulTransmission(m_outgoingEntryPtr_dest);
				}
#ifdef OMAC_DEBUG_GPIO
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
#endif
			}
			else if(radioAckStatus == TRAC_STATUS_FAIL_TO_SEND){
#ifdef OMAC_DEBUG_GPIO
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, TRUE );
				CPU_GPIO_SetPinState( DATATX_SEND_ACK_HANDLER, FALSE );
#endif
			}

			if(true){//if(g_OMAC.m_txAckHandler != NULL){
				if(m_outgoingEntryPtr->GetHeader()->payloadType != MFM_OMAC_TIMESYNCREQ){
					SendACKToUpperLayers(rcv_msg, sizeof(Message_15_4_t), NetworkOperations_Busy, radioAckStatus);
#if OMAC_DTH_DEBUG_ReceiveDATAACK_PRINTOUT
				hal_printf("DataTransmissionHandler:HARDWARE_ACKSendACK:NetworkOperations_Busy dest = %u \r\n", rcv_msg->GetHeader()->dest);
#endif
				}
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
#ifdef OMAC_DEBUG_PRINTF
			OMAC_HAL_PRINTF("radioAckStatus: %d\n", radioAckStatus);
#endif
			ASSERT_SP(0);
		}
	}
	else if(CPU_Radio_GetRadioAckType() == SOFTWARE_ACK){
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, g_OMAC.ACK_RX_MAX_DURATION_MICRO, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm == TimerSupported) txhandler_state = DTS_WAITING_FOR_ACKS;
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
		if(true){ //if(g_OMAC.m_txAckHandler != NULL){
			if(m_outgoingEntryPtr->GetHeader()->payloadType != MFM_OMAC_TIMESYNCREQ){
				SendACKToUpperLayers(rcv_msg, sizeof(Message_15_4_t), NetworkOperations_SendInitiated, radioAckStatus);
#if OMAC_DTH_DEBUG_ReceiveDATAACK_PRINTOUT
				hal_printf("DataTransmissionHandler:SOFTWARE_ACKSendACK:NetworkOperations_SendInitiated dest = %u \r\n", rcv_msg->GetHeader()->dest);
#endif
			}
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

void DataTransmissionHandler::ReceiveDATAACK(UINT16 sourceaddress){
#if OMAC_DTH_DEBUG_ReceiveDATAACK
	if(DATATX_ReceiveDATAACK_PIN_TOGGLER != DISABLED_PIN){
		CPU_GPIO_SetPinState( DATATX_ReceiveDATAACK_PIN_TOGGLER, !CPU_GPIO_GetPinState(DATATX_ReceiveDATAACK_PIN_TOGGLER) );
		CPU_GPIO_SetPinState( DATATX_ReceiveDATAACK_PIN_TOGGLER, !CPU_GPIO_GetPinState(DATATX_ReceiveDATAACK_PIN_TOGGLER) );
	}
#endif
	//2) Check if DataTransmissionHandler is active
	//1) SOFTWARE_ACKs are used
	//3) If the sourceID is equal to the destination of the original message
	//
	if( true
			&& 	CPU_Radio_GetRadioAckType() == SOFTWARE_ACK
			&&	g_OMAC.m_omac_scheduler.m_state == (I_DATA_SEND_PENDING) && g_OMAC.m_omac_scheduler.m_execution_started
			&& 	sourceaddress == m_outgoingEntryPtr_dest
	){
		txhandler_state = DTS_RECEIVEDDATAACK;

		if(m_outgoingEntryPtr != NULL){ //if(g_OMAC.m_txAckHandler != NULL && m_outgoingEntryPtr != NULL){
			if(m_outgoingEntryPtr->GetHeader()->payloadType != MFM_OMAC_TIMESYNCREQ){
				SendACKToUpperLayers(m_outgoingEntryPtr, sizeof(Message_15_4_t), NetworkOperations_SendACKed, TRAC_STATUS_SUCCESS);
#if OMAC_DTH_DEBUG_ReceiveDATAACK_PRINTOUT
				hal_printf("ReceiveDATAACK:NetworkOperations_SendACKed dest = %u \r\n", m_outgoingEntryPtr->GetHeader()->dest);
#endif
			}
		}


#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RX_DATAACK_PIN, TRUE);
		//CPU_GPIO_SetPinState( HW_ACK_PIN, TRUE );
		CPU_GPIO_SetPinState( DATATX_RECV_SW_ACK, TRUE );
#endif
		VirtualTimerReturnMessage rm;

#if	OMAC_DEBUG_PRINTF_PACKET_ACK_RX_SUCCESS
		if(m_outgoingEntryPtr){
			hal_printf("ACK_RX_SUCCESS dest= %u payloadType= %u, flags = %u, Retry Attempts = %u \n"
					, m_outgoingEntryPtr->GetHeader()->dest
					, m_outgoingEntryPtr->GetHeader()->payloadType
					, m_outgoingEntryPtr->GetHeader()->flags
					, m_outgoingEntryPtr->GetMetaData()->GetRetryAttempts());
		}
		else{
			hal_printf("ACK_RX_SUCCESS NO m_outgoingEntryPtr \n");
		}
#endif

		g_NeighborTable.RecordLastHeardTime(sourceaddress,g_OMAC.m_Clock.GetCurrentTimeinTicks());
		DropPacket(); // The decision for dropping the packet depends on the outcome of the data reception


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
		CPU_GPIO_SetPinState( DATATX_RECV_SW_ACK, FALSE );
#endif
	}
}


/*
 *
 */
void DataTransmissionHandler::PostExecuteEvent(){
#if OMAC_DTH_DEBUG_UNEXPECTED_POST_EX
	if(txhandler_state != DTS_RECEIVEDDATAACK && txhandler_state != DTS_RECEIVEDDATAACK){
		if(m_outgoingEntryPtr){
			hal_printf("ACK RX FAIL dest= %u payloadType= %u, flags = %u, Retry Attempts = %u \n"
					, m_outgoingEntryPtr->GetHeader()->dest
					, m_outgoingEntryPtr->GetHeader()->payloadType
					, m_outgoingEntryPtr->GetHeader()->flags
					, m_outgoingEntryPtr->GetMetaData()->GetRetryAttempts());
		}
		else{
			hal_printf("ACK RX FAIL NO m_outgoingEntryPtr \n");
		}
	}
#endif
	if(txhandler_state == DTS_WAITING_FOR_ACKS){
#if OMAC_DEBUG_PRINTF_PACKET_ACK_RX_FAIL
		if(m_outgoingEntryPtr){
			hal_printf("ACK RX FAIL dest= %u payloadType= %u, flags = %u, Retry Attempts = %u \n"
					, m_outgoingEntryPtr->GetHeader()->dest
					, m_outgoingEntryPtr->GetHeader()->payloadType
					, m_outgoingEntryPtr->GetHeader()->flags
					, m_outgoingEntryPtr->GetMetaData()->GetRetryAttempts());
		}
		else{
			hal_printf("ACK RX FAIL NO m_outgoingEntryPtr \n");
		}

#endif
		SelectRetrySlotNumForNeighborBackOff();
	}
	txhandler_state = DTS_POSTEXECUTION;
	//Scheduler's PostExecution stops the radio
	//g_OMAC.m_omac_RadioControl.Stop();
#ifdef OMAC_DEBUG_GPIO
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
	CPU_GPIO_SetPinState( DATATX_DATA_PIN, TRUE );
	CPU_GPIO_SetPinState( DATATX_DATA_PIN, FALSE );
#endif
	g_OMAC.m_omac_scheduler.PostExecution();
}

void DataTransmissionHandler::FailsafeStop(){
	VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
}

Message_15_4_t* DataTransmissionHandler::SelectPacketForDest(UINT16 m_outgoingEntryPtr_dest_){

	Message_15_4_t* m_outgoingEntryPtr_local = NULL;

	Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr_dest_);
	if(neigh_ptr == NULL){
		return NULL;
	}

	m_outgoingEntryPtr_local = NULL;
	if(neigh_ptr->IsInitializationTimeSamplesNeeded() && neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0 ) {
		m_outgoingEntryPtr_local = neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval();
	}
	else if(neigh_ptr->send_buffer.GetNumberMessagesInBuffer() > 0 ) {
		m_outgoingEntryPtr_local = neigh_ptr->send_buffer.GetOldestwithoutRemoval();
	}
	else if(neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0 ){
		m_outgoingEntryPtr_local = neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval();
	}
	else{
		//Commenting out, as sometimes there may not be anything to be sent,
		//	in which case Send returns a false.
		//ASSERT_SP(0);
	}
	return m_outgoingEntryPtr_local;
}

/*
 *
 */
bool DataTransmissionHandler::Send(){
	//ASSERT_SP(m_outgoingEntryPtr != NULL);

	DeviceStatus rs;

	m_outgoingEntryPtr = SelectPacketForDest(m_outgoingEntryPtr_dest);

	//	Neighbor_t* neigh_ptr = g_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr_dest);
	//	if(neigh_ptr == NULL){
	//		return false;
	//	}
	//
	//	m_outgoingEntryPtr = NULL;
	//	if(neigh_ptr->IsInitializationTimeSamplesNeeded() && neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0 ) {
	//		m_outgoingEntryPtr = neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval();
	//	}
	//	else if(neigh_ptr->send_buffer.GetNumberMessagesInBuffer() > 0 ) {
	//		m_outgoingEntryPtr = neigh_ptr->send_buffer.GetOldestwithoutRemoval();
	//	}
	//	else if(neigh_ptr->tsr_send_buffer.GetNumberMessagesInBuffer() > 0 ){
	//		m_outgoingEntryPtr = neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval();
	//	}
	//	else{
	//		//Commenting out, as sometimes there may not be anything to be sent,
	//		//	in which case Send returns a false.
	//		//ASSERT_SP(0);
	//	}

	//Send only when packet has been scheduled
	if(m_outgoingEntryPtr != NULL && isDataPacketScheduled){
		UINT16 dest = m_outgoingEntryPtr->GetHeader()->dest;
		IEEE802_15_4_Header_t* header = m_outgoingEntryPtr->GetHeader();
		IEEE802_15_4_Metadata* metadata = m_outgoingEntryPtr->GetMetaData();
		metadata->SetRetryAttempts(metadata->GetRetryAttempts()+1);
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATATX_DATA_PIN, TRUE );
		CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
		CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
#endif

#if OMAC_DTH_DEBUG_LATEWAKEUP
		UINT64 y = g_OMAC.m_Clock.GetCurrentTimeinTicks() + OMAC_DTH_DELAY_FROM_SEND_TO_RADIO_DRIVER_SEND_IN_TICKS;
		UINT8 print_OMAC_DTH_DEBUG_LATEWAKEUP_error = 0;
		if(m_scheduledTXTime_in_own_clock_ticks < y  ){
			if(y  > m_scheduledTXTime_in_own_clock_ticks + OMAC_DTH_DEBUG_LATEWAKEUP_ALLOWANCE_IN_TICKS){
				print_OMAC_DTH_DEBUG_LATEWAKEUP_error = 1;
			}
		}
		else {
			if(y + OMAC_DTH_DEBUG_LATEWAKEUP_ALLOWANCE_IN_TICKS < m_scheduledTXTime_in_own_clock_ticks ){
				print_OMAC_DTH_DEBUG_LATEWAKEUP_error = 2;
			}
		}
#endif

		rs = g_OMAC.m_omac_RadioControl.Send(dest, m_outgoingEntryPtr, header->length);


#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATATX_DATA_PIN, FALSE );
#endif


#if OMAC_DTH_DEBUG_LATEWAKEUP_PIN_TOGGLING
		if(print_OMAC_DTH_DEBUG_LATEWAKEUP_error == 2) {
			if(DATATX_TIMING_ERROR_PIN_TOGGLER != DISABLED_PIN){
				CPU_GPIO_SetPinState( DATATX_TIMING_ERROR_PIN_TOGGLER, !CPU_GPIO_GetPinState(DATATX_TIMING_ERROR_PIN_TOGGLER) );
				CPU_GPIO_SetPinState( DATATX_TIMING_ERROR_PIN_TOGGLER, !CPU_GPIO_GetPinState(DATATX_TIMING_ERROR_PIN_TOGGLER) );
			}
		}
#endif

#if OMAC_DTH_DEBUG_LATEWAKEUP

		if(print_OMAC_DTH_DEBUG_LATEWAKEUP_error == 2) {
			hal_printf("\r\n DTH LATE WAKEUP ERROR! Error in MicroSec= %llu \r\n"
					,g_OMAC.m_Clock.ConvertTickstoMicroSecs( m_scheduledTXTime_in_own_clock_ticks - y) );
		}
		else if(print_OMAC_DTH_DEBUG_LATEWAKEUP_error == 1){
			//hal_printf("\r\n OMAC_DTH_DEBUG_LATEWAKEUP EARLY ERROR! scheduledTXTime_ticks = %llu , Cur Ticks = %llu scheduledTXTime_ticks_neigh = %llu Error in MicroSec= %llu\r\n"
			//	,m_scheduledTXTime_in_own_clock_ticks, y, m_scheduledTXTime_in_neigh_clock_ticks, y - m_scheduledTXTime_in_own_clock_ticks );
			hal_printf("\r\n DTH EARLY WAKEUP ERROR! Error in MicroSec= %llu\r\n"
					,g_OMAC.m_Clock.ConvertTickstoMicroSecs( y - m_scheduledTXTime_in_own_clock_ticks) );

		}

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
		if (neighborEntry->MACAddress != dest) {
			DEBUG_OMAC_UNWUS_PRINTF("DataTransmissionHandler::ScheduleDataPacket() incorrect neighbor returned\n");
			return FALSE;
		}
		UINT64 y = g_OMAC.m_Clock.GetCurrentTimeinTicks();
		UINT64 neighborTimeinTicks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, g_OMAC.m_Clock.GetCurrentTimeinTicks());
		if (neighborTimeinTicks == 0){ //Case: No timing info is available for the destination
			//Keep the packet but do not schedule the data packet
			DEBUG_OMAC_UNWUS_PRINTF("DataTransmissionHandler::ScheduleDataPacket() neighbor time is not known!!!\n");
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
		DEBUG_OMAC_UNWUS_PRINTF("Cannot find nbr %u\n", dest);
		return FALSE;
	}
}

