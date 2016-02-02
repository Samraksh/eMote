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
//const uint HARDWARE_ACKS = 0;
//const uint SOFTWARE_ACKS = 1;


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
	g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.PostExecuteEvent();
}

void PublicDataTxExecEventCallback(void* param){
	g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.ExecuteEventHelper();
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
	if(HARDWARE_ACKS){
		SOFTWARE_ACKS = 0;
	}
	else{
		SOFTWARE_ACKS = 1;
	}
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
	if(HARDWARE_ACKS){
		if(FAST_RECOVERY){
			//When a hardware ack is received, stop the 1-shot timer and drop the packet
			VirtualTimerReturnMessage rm = VirtTimer_Stop(VIRT_TIMER_OMAC_FAST_RECOVERY);
			ASSERT_SP(rm == TimerSupported);
			//hal_printf("DataTransmissionHandler::HardwareACKHandler - dropping packet\n");
			//g_send_buffer.DropOldest(1);
		}
		CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
		CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
		//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
		//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
		resendSuccessful = true;
		g_send_buffer.DropOldest(1);
		//CPU_GPIO_SetPinState( HW_ACK_PIN, TRUE );
		currentAttempt = 0;
		//CPU_GPIO_SetPinState( HW_ACK_PIN, FALSE );

		VirtualTimerReturnMessage rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 10, TRUE );
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
}

void DataTransmissionHandler::SendRetry(){
#ifdef OMAC_DEBUG_GPIO
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
#endif

	VirtualTimerReturnMessage rm;
	if(FAST_RECOVERY){
		static UINT8 currentFrameRetryAttempt = 0;
		UINT8 currentFrameRetryMaxAttempt = 2;
		//If an ack is not received, then attempt to send multiple times in current frame itself,
		// as there is a possibility that tx woke up early then rx. In this case, trying to send
		// would allow the packet to reach in current frame itself, rather than next frame.
		/*if(!currentSendRetry){
			//If current retry is a failure, this variable prevents the same retry from happening again.
			currentSendRetry = true;*/
		//"if" condition has to be below "while" because, before 2nd attempt, check if 1st attempt was successful or not (Updated: NOT TRUE)
		if(!resendSuccessful){
			if(currentFrameRetryAttempt < currentFrameRetryMaxAttempt){
				bool rv = false;
				//Do a random back-off here
				//HAL_Time_Sleep_MicroSeconds(100);
				//Check channel for energy
				DeviceStatus DS = CPU_Radio_ClearChannelAssesment(g_OMAC.radioName);
				//DeviceStatus DS = DS_Success;
				if(DS == DS_Success){
					CPU_GPIO_SetPinState( FAST_RECOVERY_SEND, TRUE );
					rv = Send();
					if(rv){
						hal_printf("Fast recovery-send successful\n");
						resendSuccessful = false;
						rm = VirtTimer_Stop(VIRT_TIMER_OMAC_FAST_RECOVERY);
						rm = VirtTimer_Change(VIRT_TIMER_OMAC_FAST_RECOVERY, 0, FAST_RECOVERY_WAIT_PERIOD, TRUE );
						rm = VirtTimer_Start(VIRT_TIMER_OMAC_FAST_RECOVERY);
						ASSERT_SP(rm == TimerSupported);
					}
					CPU_GPIO_SetPinState( FAST_RECOVERY_SEND, FALSE );
				}
				else{
					hal_printf("Fast recovery-channel energy detected\n");
					//Do a random back-off here
					//HAL_Time_Sleep_MicroSeconds(100);
				}
			}
			currentFrameRetryAttempt++;
			if(currentFrameRetryAttempt == currentFrameRetryMaxAttempt && currentFrameRetryMaxAttempt != 1){
				currentFrameRetryAttempt = 0;
				rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
				rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 10, TRUE, OMACClockSpecifier ); //Set up a timer with 0 microsecond delay (that is ideally 0 but would not make a difference)
				rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
				//break;
				goto END;
			}
			if(currentFrameRetryMaxAttempt == 1){
				currentFrameRetryAttempt = 0;
				rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
				rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 10, TRUE, OMACClockSpecifier ); //Set up a timer with 0 microsecond delay (that is ideally 0 but would not make a difference)
				rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
				goto END;
			}
		}
		//}
	}
	//If retry count goes beyond the limit, packet could not reach dest, so drop it
	//else{
		currentAttempt++;
		if(currentAttempt >= maxRetryAttempts){
			//hal_printf("Packet could not reach dest after max attempts. Dropping packet\n");
			currentAttempt = 0;
			g_send_buffer.DropOldest(1);
		}
	//}
END:
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATATX_DATA_PIN, TRUE );
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
#endif

	//Resend same packet if listen period is large enough
	/*if(FAST_RECOVERY_WAIT_PERIOD < LISTEN_PERIOD_FOR_RECEPTION_HANDLER && currentAttempt < maxRetryAttempts){
		//Find out how long into reception slot the sender is.
		//This is based on finding diff between start of transmission and current time at retry.
		//But this logic does not cover the case where the tx starts after the rx, but only when tx is before or
		//	close to beginning of rx slot.
		UINT64 currentEndTicksForRetransmission = HAL_Time_CurrentTicks();
		UINT64 currentAttemptDuration = currentEndTicksForRetransmission - currentStartTicksForRetransmission;
		INT64 currentAttemptDurationTime = HAL_Time_TicksToTime(currentAttemptDuration);
		if(currentAttemptDurationTime < LISTEN_PERIOD_FOR_RECEPTION_HANDLER){
			hal_printf("Retrying %d\n", currentAttempt);
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
	}*/
	//hal_printf("currentAttempt: %d\n", currentAttempt);
	//Attempt to continuously transmit the msg
	/*if(currentAttempt == 1){
		bool rv = Send();
	}*/
}

void DataTransmissionHandler::ExecuteEventHelper()
{
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
		if(m_outgoingEntryPtr->GetHeader()->dsn != OMAC_DISCO_SEQ_NUMBER){
			DS = CPU_Radio_ClearChannelAssesment(g_OMAC.radioName);
		}
		else{
			DS = DS_Success;
			HAL_Time_Sleep_MicroSeconds(140);
		}

		if(DS != DS_Success){
			hal_printf("transmission detected!\n");
			//i = GUARDTIME_MICRO/140;
			canISend = false;
			break;
		}
		canISend = true;

		if( g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks() - y) > CCA_PERIOD_MICRO){
			break;
		}
	}

	if(canISend && currentAttempt < maxRetryAttempts){
		//Needs some more thought before implementing this fully
		//currentStartTicksForRetransmission = HAL_Time_CurrentTicks();
		resendSuccessful = false;
		//currentSendRetry = false;
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
#endif
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
				if(!SOFTWARE_ACKS && !HARDWARE_ACKS){
					g_send_buffer.DropOldest(1);
				}
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
		//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
		//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
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

	txhandler_state = DTS_EXECUTE_START;
	DeviceStatus e = DS_Fail;
	e = g_OMAC.m_omac_RadioControl.StartRx();

	if(e == DS_Success){
		this->ExecuteEventHelper();
		/*rm = VirtTimer_Start(VIRT_TIMER_OMAC_TX_EXECEVENT);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}*/
	}
	else{
		hal_printf("Radio not in RX state\n");
		txhandler_state = DTS_RADIO_START_FAILED;
	}

	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
	//rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, DATATX_POST_EXEC_DELAY, TRUE );
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, MAX_PACKET_TX_DURATION_MICRO, TRUE, OMACClockSpecifier );
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
		PostExecuteEvent();
	}
}

void DataTransmissionHandler::SendACKHandler(){
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
	txhandler_state = DTS_SEND_FINISHED;
	VirtualTimerReturnMessage rm;

	if(FAST_RECOVERY){
		//If send is successful, start timer for hardware ACK.
		//If hardware ack is received within stipulated period, drop the oldest packet.
		//Else retry
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_FAST_RECOVERY);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_FAST_RECOVERY, 0, FAST_RECOVERY_WAIT_PERIOD, TRUE );
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_FAST_RECOVERY);
		ASSERT_SP(rm == TimerSupported);
	}

	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm == TimerSupported){
		if(SOFTWARE_ACKS){
			if(FAST_RECOVERY){
				//rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, DATATX_POST_EXEC_DELAY, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
				rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, ACK_RX_MAX_DURATION_MICRO, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
				rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
				if(rm == TimerSupported)
					txhandler_state = DTS_WAITING_FOR_POSTEXECUTION;
			}
			else{
				rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 10, TRUE, OMACClockSpecifier ); //Set up a timer with 0 microsecond delay (that is ideally 0 but would not make a difference)
				rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
				if(rm == TimerSupported)
					txhandler_state = DTS_WAITING_FOR_POSTEXECUTION;
			}
		}
		else if(HARDWARE_ACKS){
			if(FAST_RECOVERY){
				//rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, DATATX_POST_EXEC_DELAY, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
				rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, ACK_RX_MAX_DURATION_MICRO, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
				rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
				if(rm == TimerSupported)
					txhandler_state = DTS_WAITING_FOR_POSTEXECUTION;
			}
			else{
				rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 10, TRUE, OMACClockSpecifier ); //Set up a timer with 0 microsecond delay (that is ideally 0 but would not make a difference)
				rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
				if(rm == TimerSupported)
					txhandler_state = DTS_WAITING_FOR_POSTEXECUTION;
			}
		}
		else{
			ASSERT_SP(0);
		}
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, TRUE );
	//CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
	//else{		//Could not stop timer just wait for it
	//}
}

void DataTransmissionHandler::ReceiveDATAACK(UINT16 address){
	txhandler_state = DTS_RECEIVEDDATAACK;
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RX_DATAACK_PIN, TRUE);
		//CPU_GPIO_SetPinState( HW_ACK_PIN, TRUE );
#endif
	VirtualTimerReturnMessage rm;

	if(FAST_RECOVERY){
		//When a software ack is received, stop the 1-shot timer and drop the packet
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_FAST_RECOVERY);
		ASSERT_SP(rm == TimerSupported);
	}

	if(SOFTWARE_ACKS){
		g_send_buffer.DropOldest(1); // The decision for dropping the packet depends on the outcome of the data reception
	}

	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_TRANSMITTER);
	if(rm == TimerSupported){
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_TRANSMITTER, 0, 10, TRUE, OMACClockSpecifier ); //Set up a timer with 1 microsecond delay (that is ideally 0 but would not make a difference)
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_TRANSMITTER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
		else{
			txhandler_state = DTS_WAITING_FOR_POSTEXECUTION;
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

