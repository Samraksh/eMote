/*
 * Scheduler.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: Bora Karaoglu
 *
 *  Copyright The Samraksh Company
 */

#include <Samraksh/MAC/OMAC/Scheduler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>

extern OMACType g_OMAC;

void PublicPostExecutionTaskHandler1(void * param){
	g_OMAC.m_omac_scheduler.PostPostExecution();
}

void PublicSchedulerTaskHandlerFailsafe(void * param){
	g_OMAC.m_omac_scheduler.FailsafeStop();
}

void PublicSchedulerTaskHandler1(void * param){

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_SCHEDULER);
	////ASSERT_SP(rm == TimerSupported);

	if((g_OMAC.m_omac_scheduler.SchedulerINUse)){
		g_OMAC.m_omac_scheduler.SchedulerINUse = false;
		g_OMAC.m_omac_scheduler.RunEventTask();
	}
}

void OMACScheduler::Initialize(UINT8 _radioID, UINT8 _macID){

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(SCHED_START_STOP_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(SCHED_RX_EXEC_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(SCHED_TX_EXEC_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(SCHED_DISCO_EXEC_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(SCHED_TSREQ_EXEC_PIN, FALSE);
	CPU_GPIO_SetPinState(SCHED_TSREQ_EXEC_PIN, FALSE);
#endif

	radioID = _radioID;
	macID = _macID;
	
#ifdef PROFILING
	minStartDelay = 300; maxStartDelay = 10;
	minStopDelay = 300; maxStopDelay = 10;
#endif

	InputState.ToIdle();

	//Initialize the HAL vitual timer layer

	bool rv = VirtTimer_Initialize();
	//ASSERT_SP(rv);
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_SCHEDULER, 0, SLOT_PERIOD_MILLI * MICSECINMILISEC, FALSE, FALSE, PublicSchedulerTaskHandler1, OMACClockSpecifier);
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_SCHEDULER_FAILSAFE, 0, FAILSAFETIME_MICRO, TRUE, FALSE, PublicSchedulerTaskHandlerFailsafe, OMACClockSpecifier);

	//ASSERT_SP(rm == TimerSupported);

	//Initialize Handlers
	m_DiscoveryHandler.Initialize(radioID, macID);
	m_DataReceptionHandler.Initialize(radioID, macID);
	m_DataTransmissionHandler.Initialize();
	m_TimeSyncHandler.Initialize(radioID, macID);

	m_InitializationTimeinTicks = g_OMAC.m_Clock.GetCurrentTimeinTicks();

	ScheduleNextEvent();
}

void OMACScheduler::UnInitialize(){

}



UINT64 OMACScheduler::GetSlotNumber(){
	UINT64 currentTicks = g_OMAC.m_Clock.GetCurrentTimeinTicks();
	UINT64 slotNumber = currentTicks / SLOT_PERIOD_TICKS;
	return slotNumber;
}

UINT64 OMACScheduler::GetSlotNumberfromTicks(const UINT64 &y){
	return ( y / SLOT_PERIOD_TICKS);
}

UINT32 OMACScheduler::GetSlotNumberfromMicroSec(const UINT64 &y){
	return ( y / MICSECINMILISEC / SLOT_PERIOD_MILLI  );
}

UINT32 OMACScheduler::GetTimeTillTheEndofSlot(){
	UINT64 cur_ticks = g_OMAC.m_Clock.GetCurrentTimeinTicks();
	UINT64 ticks_till_end = SLOT_PERIOD_TICKS - ( (cur_ticks + SLOT_PERIOD_TICKS) % SLOT_PERIOD_TICKS);
	UINT32 ms_till_end = ((UINT32) ticks_till_end) / (TICKS_PER_MILLI / MICSECINMILISEC ) ;
	return ms_till_end;
}

/* ScheduleNextEvent
 *
 */
void OMACScheduler::ScheduleNextEvent(){
	if(g_OMAC.UpdateNeighborTable() > 0 ){//If there are neighbor deleted from the table increase the discovery rate
		m_DiscoveryHandler.TempIncreaseDiscoRate();
	}
	else if(g_NeighborTable.NumberOfNeighbors() == 0){//If there are no neighbors stay in high disco mode.
		m_DiscoveryHandler.TempIncreaseDiscoRate();
	}

	VirtualTimerReturnMessage rm;

	UINT64 rxEventOffset = 0, txEventOffset = 0, beaconEventOffset = 0, timeSyncEventOffset=0, rxEventOffsetAdjust, txEventOffsetAdjust;
	UINT64 curticks_rx, curticks_tx, curticks_beacon, curticks_timesync, curticks;
	nextWakeupTimeInMicSec = MAXSCHEDULERUPDATE;


	timeSyncEventOffset = m_TimeSyncHandler.NextEvent();
	curticks_timesync =  g_OMAC.m_Clock.GetCurrentTimeinTicks();
	if (timeSyncEventOffset < OMAC_SCHEDULER_MIN_REACTION_TIME_IN_MICRO) timeSyncEventOffset = 0xffffffffffffffff;


	rxEventOffset = m_DataReceptionHandler.NextEvent();
	curticks_rx =  g_OMAC.m_Clock.GetCurrentTimeinTicks();
	if (rxEventOffset < OMAC_SCHEDULER_MIN_REACTION_TIME_IN_MICRO) rxEventOffset = 0xffffffffffffffff;
	//rxEventOffset = rxEventOffset-1;

	txEventOffset = m_DataTransmissionHandler.NextEvent();
	curticks_tx =  g_OMAC.m_Clock.GetCurrentTimeinTicks();
	if (txEventOffset < OMAC_SCHEDULER_MIN_REACTION_TIME_IN_MICRO) txEventOffset = 0xffffffffffffffff;
	//txEventOffset = txEventOffset-1;


	beaconEventOffset = m_DiscoveryHandler.NextEvent();
	curticks_beacon = g_OMAC.m_Clock.GetCurrentTimeinTicks();
	if (beaconEventOffset < OMAC_SCHEDULER_MIN_REACTION_TIME_IN_MICRO) beaconEventOffset = 0xffffffffffffffff;
	//beaconEventOffset = beaconEventOffset -1;

	//Readjust: No time intensive tasks after this point
	curticks = g_OMAC.m_Clock.GetCurrentTimeinTicks();
	rxEventOffset = rxEventOffset -  g_OMAC.m_Clock.ConvertTickstoMicroSecs(curticks - curticks_rx);
	txEventOffset = txEventOffset -  g_OMAC.m_Clock.ConvertTickstoMicroSecs(curticks - curticks_tx);

	if(rxEventOffset < MAXSCHEDULERUPDATE && rxEventOffset < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = rxEventOffset;
	}
	if(txEventOffset < MAXSCHEDULERUPDATE && txEventOffset < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = txEventOffset;
	}
	if(beaconEventOffset < MAXSCHEDULERUPDATE && beaconEventOffset + DISCO_SLOT_GUARD * SLOT_PERIOD_MILLI * MICSECINMILISEC < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = beaconEventOffset;
	}
	if(timeSyncEventOffset < MAXSCHEDULERUPDATE && timeSyncEventOffset < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = timeSyncEventOffset;
	}

	if(rxEventOffset == nextWakeupTimeInMicSec) {
		InputState.RequestState(I_DATA_RCV_PENDING);
		//nextWakeupTimeInMicSec = nextWakeupTimeInMicSec - RADIO_TURN_ON_DELAY_MICRO - OMAC_HW_ACK_DELAY; //MMA
		//nextWakeupTimeInMicSec = nextWakeupTimeInMicSec - RADIO_TURN_ON_DELAY_MICRO; //BK: THis calculation is done inside the nextevent in order to prevent a negative value
	}
	else if(txEventOffset == nextWakeupTimeInMicSec) {
		//BK: Transmitter first turns the radio on and hence incurs that delay, then copies the packet on the SPI bus and incurs that delay.
		//TODO: BK: The PROCESSING_DELAY_BEFORE_TX_MICRO should depend on the packet size. We need to experiment and make it better. This will help balance out the guardband and take the bias out of it.
		//nextWakeupTimeInMicSec = nextWakeupTimeInMicSec - PROCESSING_DELAY_BEFORE_TX_MICRO - RADIO_TURN_ON_DELAY_MICRO - OMAC_HW_ACK_DELAY;	//MMA
		//nextWakeupTimeInMicSec = nextWakeupTimeInMicSec + GUARDTIME_MICRO + SWITCHING_DELAY_MICRO - PROCESSING_DELAY_BEFORE_TX_MICRO - RADIO_TURN_ON_DELAY_MICRO;//BK: THis calculation is done inside the nextevent in order to prevent a negative value
		InputState.RequestState(I_DATA_SEND_PENDING);
	}
	else if(beaconEventOffset == nextWakeupTimeInMicSec) {
		//nextWakeupTimeInMicSec = nextWakeupTimeInMicSec - OMAC_HW_ACK_DELAY_MICRO;
		InputState.RequestState(I_DISCO_PENDING);
	}
	else if(timeSyncEventOffset == nextWakeupTimeInMicSec) {
		//nextWakeupTimeInMicSec = nextWakeupTimeInMicSec - OMAC_HW_ACK_DELAY_MICRO;
		CPU_GPIO_SetPinState(SCHED_TSREQ_EXEC_PIN, TRUE);
		CPU_GPIO_SetPinState(SCHED_TSREQ_EXEC_PIN, FALSE);
		InputState.RequestState(I_TIMESYNC_PENDING);
	}
	else{
		InputState.RequestState(I_IDLE);
	}


#ifdef def_Neighbor2beFollowed
#ifdef OMAC_DEBUG_PRINTF
	UINT64 curTicks = g_OMAC.m_Clock.GetCurrentTimeinTicks();
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] OMACScheduler::ScheduleNextEvent() nextWakeupTimeInMicSec= %llu AbsnextWakeupTimeInMicSec= %llu - %lu InputState.GetState() = %d \n"
			, g_OMAC.m_Clock.ConvertTickstoMicroSecs(curTicks), GetSlotNumberfromTicks(curTicks), m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks), GetSlotNumberfromTicks(m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), nextWakeupTimeInMicSec, g_OMAC.m_Clock.ConvertTickstoMicroSecs(curTicks)+nextWakeupTimeInMicSec, GetSlotNumberfromMicroSec(g_OMAC.m_Clock.ConvertTickstoMicroSecs(curTicks)+nextWakeupTimeInMicSec), InputState.GetState() );
	if(curTicks - m_InitializationTimeinTicks > (120 * 8000000)){
		hal_printf("Critial TIme has Passed. Be careful. About to crash!!\n");
	}
#endif
#endif
	//nextWakeupTimeInMicSec = nextWakeupTimeInMicSec - TIMER_EVENT_DELAY_OFFSET; //BK: There seems to be a constant delay in timers. This is to compansate for it.

	if(nextWakeupTimeInMicSec > MAXSCHEDULERUPDATE || nextWakeupTimeInMicSec < OMAC_SCHEDULER_MIN_REACTION_TIME_IN_MICRO ) {
		nextWakeupTimeInMicSec = MAXSCHEDULERUPDATE;
		InputState.RequestState(I_IDLE);
	}

	SchedulerINUse = true;
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_SCHEDULER, 0, nextWakeupTimeInMicSec, FALSE, OMACClockSpecifier); //1 sec Timer in micro seconds
	//ASSERT_SP(rm == TimerSupported);
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_SCHEDULER);
	//ASSERT_SP(rm == TimerSupported);


//
//	BOOL* completionFlag = (BOOL*)false;
//	// we assume only 1 can be active, abort previous just in case
//	OMAC_scheduler_TimerCompletion.Abort();
//	OMAC_scheduler_TimerCompletion.InitializeForISR(PublicSchedulerTaskHandler, completionFlag);
//	//Enqueue a task to listen for messages 100 usec from now (almost immediately)
//	//TODO (Ananth): to check what the right enqueue value should be
//	OMAC_scheduler_TimerCompletion.EnqueueDelta(nextWakeupTimeInMicSec);
}

bool OMACScheduler::RunEventTask(){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( SCHED_START_STOP_PIN, TRUE );
#endif
		VirtualTimerReturnMessage rm;
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_SCHEDULER_FAILSAFE);
	//g_OMAC.UpdateNeighborTable();
	UINT64 curTicks = g_OMAC.m_Clock.GetCurrentTimeinTicks();

#ifdef def_Neighbor2beFollowed
#ifdef OMAC_DEBUG_PRINTF
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] OMACScheduler::RunEventTask() \n"
			, g_OMAC.m_Clock.ConvertTickstoMicroSecs(curTicks), GetSlotNumber(), g_OMAC.m_Clock.ConvertTickstoMicroSecs(m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), GetSlotNumberfromTicks(m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)) );
#endif
#endif

	switch(InputState.GetState()) {
		case I_DATA_SEND_PENDING:
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState(SCHED_TX_EXEC_PIN, TRUE);
#endif
			m_lastHandler = DATA_TX_HANDLER;
			m_DataTransmissionHandler.ExecuteEvent();
			break;
		case I_DATA_RCV_PENDING:
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState(SCHED_RX_EXEC_PIN, TRUE);
#endif
			m_lastHandler = DATA_RX_HANDLER;
			m_DataReceptionHandler.ExecuteEvent();
			break;
		case I_TIMESYNC_PENDING:
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState(SCHED_TSREQ_EXEC_PIN, TRUE);
#endif
			m_lastHandler = TIMESYNC_HANDLER;
			m_TimeSyncHandler.ExecuteEvent();
			break;
		case I_DISCO_PENDING:
#ifdef OMAC_DEBUG_GPIO
			CPU_GPIO_SetPinState(SCHED_DISCO_EXEC_PIN, TRUE);
#endif
			m_DiscoveryHandler.ExecuteEvent();
			m_lastHandler = CONTROL_BEACON_HANDLER;
			break;
		default: //Case for
			PostPostExecution();
	}
}

void OMACScheduler::PostExecution(){
	//VirtTimer_Start(HAL_SLOT_TIMER3);
	//g_OMAC.PushPacketsToUpperLayers();
	PostPostExecution();
}

void OMACScheduler::FailsafeStop(){
	switch(InputState.GetState()) {
		case I_DATA_SEND_PENDING:
			m_DataTransmissionHandler.FailsafeStop();
			PostExecution();
			break;
		case I_DATA_RCV_PENDING:
			m_DataReceptionHandler.FailsafeStop();
			PostExecution();
			break;
		case I_TIMESYNC_PENDING:
			m_TimeSyncHandler.FailsafeStop();
			PostExecution();
			break;
		case I_DISCO_PENDING:
			m_DiscoveryHandler.FailsafeStop();
			PostExecution();
			break;
		default: //Case for
			PostPostExecution();
	}
}

void OMACScheduler::PostPostExecution(){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( SCHED_START_STOP_PIN, FALSE );
		CPU_GPIO_SetPinState( SCHED_START_STOP_PIN, TRUE );
#endif
	VirtualTimerReturnMessage rm = TimerNotSupported;
	//	while (rm !=  TimerSupported){
	//		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_SCHEDULER_FAILSAFE);
	//	}
	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_SCHEDULER_FAILSAFE);

	EnsureStopRadio();
	InputState.ForceState(I_IDLE);
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( SCHED_START_STOP_PIN, FALSE );
		CPU_GPIO_SetPinState( SCHED_START_STOP_PIN, TRUE );
#endif
	ScheduleNextEvent();

#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( SCHED_START_STOP_PIN, FALSE );
		CPU_GPIO_SetPinState(SCHED_RX_EXEC_PIN, FALSE);
		CPU_GPIO_SetPinState(SCHED_TX_EXEC_PIN, FALSE);
		CPU_GPIO_SetPinState(SCHED_DISCO_EXEC_PIN, FALSE);
		CPU_GPIO_SetPinState(SCHED_TSREQ_EXEC_PIN, FALSE);
#endif
}

bool OMACScheduler::EnsureStopRadio(){
	DeviceStatus  ds = DS_Success;
	ds = g_OMAC.m_omac_RadioControl.Stop();
	if (ds == DS_Success) {
		return TRUE;
	}
	else {//TODO: BK : This should be implemented in the case of radio not stopping
		hal_printf("OMACScheduler::EnsureStopRadio Radio did not go to sleep\n");
		return FALSE;
	}
}








