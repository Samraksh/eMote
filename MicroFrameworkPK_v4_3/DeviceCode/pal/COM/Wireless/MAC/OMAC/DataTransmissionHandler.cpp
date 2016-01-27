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

const uint EXECUTE_WITH_CCA = 1;
const uint FAST_RECOVERY = 1;
//#define HARDWARE_ACKS_ENABLED

static UINT64 currentStartTicksForRetransmission = 0;


void PublicDataTxCallback(void * param){
	g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.PostExecuteEvent();
}

void PublicDataTxExecEventCallback(void* param){
	g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.ExecuteEventHelper();
}

void PublicFastRecoveryCallback(void* param){
	g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.SendRetry();
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
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	//CPU_GPIO_EnableOutputPin(HW_ACK_PIN, TRUE);
	//CPU_GPIO_SetPinState( HW_ACK_PIN, FALSE );
	CPU_GPIO_SetPinState( DATARX_NEXTEVENT, FALSE );
	CPU_GPIO_EnableOutputPin(DATATX_POSTEXEC, TRUE);
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );

	CPU_GPIO_EnableOutputPin(OMAC_RX_DATAACK_PIN, FALSE);
#endif

	isDataPacketScheduled = false;
	currentAttempt = 0;
	maxRetryAttempts = 2;
	//m_TXMsg = (DataMsg_t*)m_TXMsgBuffer.GetPayload() ;

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_TRANSMITTER, 0, MAX_PACKET_TX_DURATION_MICRO, TRUE, FALSE, PublicDataTxCallback, OMACClockSpecifier); //1 sec Timer in micro seconds
	ASSERT_SP(rm == TimerSupported);
	/*rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_TX_EXECEVENT, 0, 0, TRUE, FALSE, PublicDataTxExecEventCallback, OMACClockSpecifier); //1 sec Timer in micro seconds
	ASSERT_SP(rm == TimerSupported);*/
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_FAST_RECOVERY, 0, FAST_RECOVERY_WAIT_PERIOD, TRUE, FALSE, PublicFastRecoveryCallback, OMACClockSpecifier); //1 sec Timer in micro seconds
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
		UINT64 curmicro =  g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks());

		while(nextTXmicro  <= curmicro + OMAC_SCHEDULER_MIN_REACTION_TIME_IN_MICRO) {
			if(!ScheduleDataPacket(1)){
				nextTXTicks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(dest, g_OMAC.m_NeighborTable.GetNeighborPtr(dest)->nextwakeupSlot * SLOT_PERIOD_TICKS);
				nextTXmicro = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(nextTXTicks) + GUARDTIME_MICRO + SWITCHING_DELAY_MICRO - PROCESSING_DELAY_BEFORE_TX_MICRO - RADIO_TURN_ON_DELAY_MICRO;
			}
		}
		UINT64 remMicroSecnextTX = nextTXmicro - curmicro;
		//Wake up the transmitter a little early
		remMicroSecnextTX -= GUARDTIME_MICRO;


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
	if(FAST_RECOVERY){
		//When a hardware ack is received, stop the 1-shot timer and drop the packet
		VirtualTimerReturnMessage rm = VirtTimer_Stop(VIRT_TIMER_OMAC_FAST_RECOVERY);
		ASSERT_SP(rm == TimerSupported);
		//hal_printf("DataTransmissionHandler::HardwareACKHandler - dropping packet\n");
		g_OMAC.m_send_buffer.DropOldest(1);
	}
	//CPU_GPIO_SetPinState( HW_ACK_PIN, TRUE );
	currentAttempt = 0;
	//CPU_GPIO_SetPinState( HW_ACK_PIN, FALSE );

	VirtualTimerReturnMessage rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 100, TRUE );
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
		PostExecuteEvent();
	}
}

void DataTransmissionHandler::SendRetry(){
	currentAttempt++;

	//Resend same packet if listen period is large enough
	if(FAST_RECOVERY_WAIT_PERIOD < LISTEN_PERIOD_FOR_RECEPTION_HANDLER && currentAttempt < maxRetryAttempts){
		hal_printf("Retrying %d\n", currentAttempt);
		//Find out how long into reception slot the sender is.
		//This is based on finding diff between start of transmission and current time at retry.
		//But this logic does not cover the case where the tx starts after the rx, but only when tx is before or
		//	close to beginning of rx slot.
		UINT64 currentEndTicksForRetransmission = HAL_Time_CurrentTicks();
		UINT64 currentAttemptDuration = currentEndTicksForRetransmission - currentStartTicksForRetransmission;
		INT64 currentAttemptDurationTime = HAL_Time_TicksToTime(currentAttemptDuration);
		if(currentAttemptDurationTime < LISTEN_PERIOD_FOR_RECEPTION_HANDLER){
			bool rv = Send();
			if(rv){
				//If send is successful, start timer for hardware ACK.
				//If hardware ack is received within stipulated period, drop the oldest packet.
				//Else retry
				VirtualTimerReturnMessage rm = VirtTimer_Stop(VIRT_TIMER_OMAC_FAST_RECOVERY);
				rm = VirtTimer_Change(VIRT_TIMER_OMAC_FAST_RECOVERY, 0, FAST_RECOVERY_WAIT_PERIOD, TRUE );
				rm = VirtTimer_Start(VIRT_TIMER_OMAC_FAST_RECOVERY);
				ASSERT_SP(rm == TimerSupported);
			}
		}
	}
	//hal_printf("currentAttempt: %d\n", currentAttempt);
	//Attempt to continuously transmit the msg
	/*if(currentAttempt == 1){
		bool rv = Send();
	}*/
	//If retry count goes beyond the limit, packet could not reach dest, so drop it
	if(currentAttempt >= maxRetryAttempts){
		//hal_printf("Packet could not reach dest after max attempts. Dropping packet\n");
		currentAttempt = 0;
		g_OMAC.m_send_buffer.DropOldest(1);
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
		CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
		CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
#endif
		if(canISend && currentAttempt < maxRetryAttempts){
			bool rv = Send();
			if(rv) {
				if(FAST_RECOVERY){
					//If send is successful, start timer for hardware ACK.
					//If hardware ack is received within stipulated period, drop the oldest packet.
					//Else retry
					rm = VirtTimer_Stop(VIRT_TIMER_OMAC_FAST_RECOVERY);
					rm = VirtTimer_Change(VIRT_TIMER_OMAC_FAST_RECOVERY, 0, FAST_RECOVERY_WAIT_PERIOD, TRUE );
					rm = VirtTimer_Start(VIRT_TIMER_OMAC_FAST_RECOVERY);
					ASSERT_SP(rm == TimerSupported);
				}
				else{
#ifndef SOFTWARE_ACKS_ENABLED
#ifndef HARDWARE_ACKS_ENABLED
					g_OMAC.m_send_buffer.DropOldest(1);
#endif
#endif
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
		CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
		CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
#endif

	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, DATATX_POST_EXEC_DELAY, TRUE );
	//rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, MAX_PACKET_TX_DURATION_MICRO, TRUE );
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
			, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks()), g_OMAC.m_omac_scheduler.GetSlotNumber(), g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks())), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks())) );
#endif
#endif

	VirtualTimerReturnMessage rm;

	DeviceStatus e = DS_Fail;
	e = g_OMAC.m_omac_RadioControl.StartRx();

	if(e == DS_Success){
		currentStartTicksForRetransmission = HAL_Time_CurrentTicks();
		this->ExecuteEventHelper();
		/*rm = VirtTimer_Start(VIRT_TIMER_OMAC_TX_EXECEVENT);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}*/
	}
	else{
		hal_printf("Radio not in RX state\n");
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		//rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, DATATX_POST_EXEC_DELAY, TRUE );
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, MAX_PACKET_TX_DURATION_MICRO, TRUE, OMACClockSpecifier );
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
#ifndef SOFTWARE_ACKS_ENABLED
	#ifndef HARDWARE_ACKS_ENABLED
				g_OMAC.m_send_buffer.DropOldest(1); // The decision for dropping the packet depends on the outcome of the data reception
	#endif
#endif
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

	rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, MAX_PACKET_TX_DURATION_MICRO, TRUE, OMACClockSpecifier );
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
		PostExecuteEvent();
	}
	//PostExecuteEvent();
#endif
}

void DataTransmissionHandler::SendACKHandler(){
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
	VirtualTimerReturnMessage rm;

	//If send is successful, start timer for hardware ACK.
	//If hardware ack is received within stipulated period, drop the oldest packet.
	//Else retry
	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_FAST_RECOVERY);
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_FAST_RECOVERY, 0, FAST_RECOVERY_WAIT_PERIOD, TRUE );
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_FAST_RECOVERY);
	ASSERT_SP(rm == TimerSupported);

	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm == TimerSupported){
#ifdef SOFTWARE_ACKS_ENABLED
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, ACK_RX_MAX_DURATION_MICRO, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
#endif
#ifndef SOFTWARE_ACKS_ENABLED
		if(FAST_RECOVERY){
			rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, DATATX_POST_EXEC_DELAY, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
			//rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, ACK_RX_MAX_DURATION_MICRO, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
		}
		else{
			rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 0, TRUE, OMACClockSpecifier ); //Set up a timer with 0 microsecond delay (that is ideally 0 but would not make a difference)
			rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		}
#endif
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
	//else{		//Could not stop timer just wait for it
	//}
}

void DataTransmissionHandler::ReceiveDATAACK(UINT16 address){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RX_DATAACK_PIN, TRUE);
		//CPU_GPIO_SetPinState( HW_ACK_PIN, TRUE );
#endif
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
#ifdef SOFTWARE_ACKS_ENABLED
	g_OMAC.m_send_buffer.DropOldest(1); // The decision for dropping the packet depends on the outcome of the data reception
#endif
#ifdef HARDWARE_ACKS_ENABLED
	g_OMAC.m_send_buffer.DropOldest(1); // The decision for dropping the packet depends on the outcome of the data reception
#endif
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
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
	g_OMAC.m_omac_RadioControl.Stop();
	g_OMAC.m_omac_scheduler.PostExecution();
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
	if (m_outgoingEntryPtr == NULL && g_OMAC.m_send_buffer.GetNumberMessagesInBuffer() > 0 ) {//If we already have a packet
		m_outgoingEntryPtr = g_OMAC.m_send_buffer.GetOldestwithoutRemoval();
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

