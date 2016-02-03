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
const uint FAST_RECOVERY = 0;


//Allows coordination between retrying and receiving a hw ack
//If a hw ack is received, this variable is set to true, which means that current retry
// was a success
static bool resendSuccessful = false;
//If current retry was not successful, then function SendRetry is called again,
// resulting in an endless loop. This avoids that scenario.
//This is set before sending a packet the first time and is reset during retry.
//volatile bool currentSendRetry = false;

//Needs some more thought before implementing this fully
//static UINT64 currentStartTicksForRetransmission = 0;


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

/*
 *
 */
void DataTransmissionHandler::Initialize(){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(DATATX_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(DATATX_DATA_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(DATARX_NEXTEVENT, TRUE);
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	CPU_GPIO_EnableOutputPin(FAST_RECOVERY_SEND, TRUE);
	CPU_GPIO_SetPinState( FAST_RECOVERY_SEND, FALSE );
	//CPU_GPIO_EnableOutputPin(HW_ACK_PIN, TRUE);
	//CPU_GPIO_SetPinState( HW_ACK_PIN, FALSE );
	CPU_GPIO_SetPinState( DATARX_NEXTEVENT, FALSE );
	CPU_GPIO_EnableOutputPin(DATATX_POSTEXEC, TRUE);
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );

	CPU_GPIO_EnableOutputPin(OMAC_RX_DATAACK_PIN, FALSE);
#endif

	isDataPacketScheduled = false;
	currentAttempt = 0;
	maxRetryAttempts = 100;
	//m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_TRANSMITTER, 0, MAX_PACKET_TX_DURATION_MICRO, TRUE, FALSE, PublicDataTxCallback, OMACClockSpecifier); //1 sec Timer in micro seconds
	ASSERT_SP(rm == TimerSupported);

}


/*
 * This function returns the number of ticks until the transmission time
 */
UINT64 DataTransmissionHandler::NextEvent(){
	//in case the task delay is large and we are already pass
	//tx time, tx immediately
	CPU_GPIO_SetPinState( DATARX_NEXTEVENT, TRUE );

	if(ScheduleDataPacket(0)) {
		UINT16 dest = m_outgoingEntryPtr->GetHeader()->dest;
		UINT64 nextTXTicks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(dest, g_OMAC.m_NeighborTable.GetNeighborPtr(dest)->nextwakeupSlot * SLOT_PERIOD_TICKS);
		UINT64 nextTXmicro = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(nextTXTicks) + GUARDTIME_MICRO + SWITCHING_DELAY_MICRO - PROCESSING_DELAY_BEFORE_TX_MICRO - RADIO_TURN_ON_DELAY_MICRO;
		if(EXECUTE_WITH_CCA){
			nextTXmicro -= CCA_PERIOD_MICRO;
		}
		UINT64 curmicro =  g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks());

		while(nextTXmicro  <= curmicro + OMAC_SCHEDULER_MIN_REACTION_TIME_IN_MICRO) {
			if(ScheduleDataPacket(1)){
				nextTXTicks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(dest, g_OMAC.m_NeighborTable.GetNeighborPtr(dest)->nextwakeupSlot * SLOT_PERIOD_TICKS);
				nextTXmicro = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(nextTXTicks) + GUARDTIME_MICRO + SWITCHING_DELAY_MICRO - PROCESSING_DELAY_BEFORE_TX_MICRO - RADIO_TURN_ON_DELAY_MICRO;
				if(EXECUTE_WITH_CCA){
					nextTXmicro -= CCA_PERIOD_MICRO;
				}
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
		hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataTransmissionHandler::NextEvent() remMicroSecnextTX= %llu AbsnextWakeupTimeInMicSec= %llu - %lu m_neighborNextEventTimeinMicSec = %llu - %lu\n"
				, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(curTicks), g_OMAC.m_omac_scheduler.GetSlotNumber(), g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks))
				, remMicroSecnextTX, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(nextTXTicks), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(nextTXTicks), g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr->GetHeader()->dest)->nextwakeupSlot * SLOT_PERIOD_TICKS), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_OMAC.m_NeighborTable.GetNeighborPtr(m_outgoingEntryPtr->GetHeader()->dest)->nextwakeupSlot * SLOT_PERIOD_TICKS) );
#endif
#endif

		/*UINT64 neighborSlot = g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks()));
		Neighbor_t* neigh_ptr = g_OMAC.m_NeighborTable.GetNeighborPtr(dest);
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
	txhandler_state = DTS_RECEIVEDDATAACK;
	if(HARDWARE_ACKS){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
		CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
		//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
		//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
#endif
		resendSuccessful = true;
		g_send_buffer.DropOldest(1);
		currentAttempt = 0;

		VirtualTimerReturnMessage rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 0, TRUE );
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
}

void DataTransmissionHandler::SendRetry(){ // BK: This function is called to retry in the case of FAST_RECOVERY
#ifdef OMAC_DEBUG_GPIO
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
#endif

	VirtualTimerReturnMessage rm;
	if(FAST_RECOVERY && txhandler_state == DTS_WAITING_FOR_ACKS && m_currentFrameRetryAttempt < CURRENTFRAMERETRYMAXATTEMPT){
		++m_currentFrameRetryAttempt;
		ExecuteEventHelper();
	}
	else{
		PostExecuteEvent();
	}
}

void DataTransmissionHandler::ExecuteEventHelper() { // BK: This function starts sending routine for a packet
	bool canISend = true;
	DeviceStatus DS = DS_Success;
	VirtualTimerReturnMessage rm;

	//The number 500 was chosen arbitrarily. In reality it should be the sum of backoff period + CCA period + guard band.
	//For GUARDTIME_MICRO period check the channel before transmitting
	//140 usec is the time taken for CCA to return a result
	UINT64 y = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
	//for(int i = 0; i < (GUARDTIME_MICRO/140); i++){
	while(EXECUTE_WITH_CCA){
		//Check CCA only for DATA packets
		DS = CPU_Radio_ClearChannelAssesment(g_OMAC.radioName);

		if(DS != DS_Success){
			hal_printf("transmission detected!\n");
			//i = GUARDTIME_MICRO/140;
			canISend = false;
			break;
		}
		canISend = true;

		if(m_currentFrameRetryAttempt == 0){
			if( g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks() - y) > CCA_PERIOD_MICRO){
				break;
			}
		}
		else{
			if( g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks() - y) > CCA_PERIOD_FRAME_RETRY_MICRO){
				break;
			}
		}
	}

	if(canISend){
		resendSuccessful = false;
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
#endif
		bool rv = Send();
		if(rv) {
			if(!SOFTWARE_ACKS && !HARDWARE_ACKS){
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
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, MAX_PACKET_TX_DURATION_MICRO, TRUE, OMACClockSpecifier );
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
			, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks()), g_OMAC.m_omac_scheduler.GetSlotNumber(), g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks())), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks())) );
#endif
#endif

	VirtualTimerReturnMessage rm;

	m_currentFrameRetryAttempt = 0;

	txhandler_state = DTS_EXECUTE_START;
	DeviceStatus e = DS_Fail;
	e = g_OMAC.m_omac_RadioControl.StartRx();

	if(e == DS_Success){
		this->ExecuteEventHelper();
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

void DataTransmissionHandler::SendACKHandler(){
	txhandler_state = DTS_SEND_FINISHED;
	VirtualTimerReturnMessage rm;

	if(SOFTWARE_ACKS || HARDWARE_ACKS){
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
		g_send_buffer.DropOldest(1); // The decision for dropping the packet depends on the outcome of the data reception
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

	//m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

	//Send only when packet has been scheduled
	if(m_outgoingEntryPtr != NULL && isDataPacketScheduled){
		//UINT16 fcf = m_outgoingEntryPtr->GetHeader()->fcf;
		//hal_printf("DataTransmissionHandler::Send - fcf is %d\n", fcf);
		UINT16 dest = m_outgoingEntryPtr->GetHeader()->dest;
		IEEE802_15_4_Header_t* header = m_outgoingEntryPtr->GetHeader();
		IEEE802_15_4_Metadata* metadata = m_outgoingEntryPtr->GetMetaData();
		g_OMAC.senderSequenceNumber = header->dsn;
		CPU_GPIO_SetPinState( DATATX_DATA_PIN, TRUE );
		rs = g_OMAC.m_omac_RadioControl.Send(dest, m_outgoingEntryPtr, header->length);
		CPU_GPIO_SetPinState( DATATX_DATA_PIN, FALSE );

		//set flag to false after packet has been sent
		isDataPacketScheduled = false;
		m_outgoingEntryPtr = NULL;
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
BOOL DataTransmissionHandler::ScheduleDataPacket(UINT8 _skipperiods)
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
		neighborEntry =  g_OMAC.m_NeighborTable.GetNeighborPtr(dest);
		if (neighborEntry != NULL) {
			if (neighborEntry->MacAddress != dest) {
				hal_printf("DataTransmissionHandler::ScheduleDataPacket() incorrect neighbor returned\n");
				//ASSERT_SP(neighborEntry->MacAddress == dest);
				isDataPacketScheduled = false;
				return FALSE;
			}
			UINT64 y = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
			UINT64 neighborTimeinTicks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks());
			if (neighborTimeinTicks == 0){ //Case: No timing info is available for the destination
				//Keep the packet but do not schedule the data packet
				hal_printf("DataTransmissionHandler::ScheduleDataPacket() neighbor time is not known!!!\n");
				isDataPacketScheduled = false;
				return FALSE;
			}
			UINT64 neighborSlot = g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(neighborTimeinTicks);
			if(neighborSlot >= neighborEntry->nextwakeupSlot) {
				g_OMAC.m_omac_scheduler.m_DataReceptionHandler.UpdateSeedandCalculateWakeupSlot(neighborEntry->nextwakeupSlot, neighborEntry->nextSeed, neighborEntry->mask, neighborEntry->seedUpdateIntervalinSlots, neighborSlot );
			}
			while(_skipperiods > 0) {
				--_skipperiods;
				neighborSlot = neighborEntry->nextwakeupSlot;
				g_OMAC.m_omac_scheduler.m_DataReceptionHandler.UpdateSeedandCalculateWakeupSlot(neighborEntry->nextwakeupSlot, neighborEntry->nextSeed, neighborEntry->mask, neighborEntry->seedUpdateIntervalinSlots, neighborSlot );
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

