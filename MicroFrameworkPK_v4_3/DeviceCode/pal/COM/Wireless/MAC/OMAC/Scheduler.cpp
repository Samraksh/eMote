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

extern RadioControl_t g_omac_RadioControl;
extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;

HAL_COMPLETION OMAC_scheduler_TimerCompletion;



void PublicPostExecutionTaskHandler1(void * param){
	g_omac_scheduler.PostPostExecution();
}

void PublicSchedulerTaskHandler1(void * param){

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_Stop(HAL_SLOT_TIMER);
	ASSERT_SP(rm == TimerSupported);

	if((g_omac_scheduler.SchedulerINUse)){
		g_omac_scheduler.SchedulerINUse = false;
		g_omac_scheduler.RunEventTask();
	}
}

void OMACScheduler::Initialize(UINT8 _radioID, UINT8 _macID){

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(SCHED_START_STOP_PIN, FALSE);
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
	ASSERT_SP(rv);
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(HAL_SLOT_TIMER, 0, SLOT_PERIOD_MILLI * MICSECINMILISEC, FALSE, FALSE, PublicSchedulerTaskHandler1);
	ASSERT_SP(rm == TimerSupported);
//	rm = VirtTimer_SetTimer(HAL_SLOT_TIMER2, 0, SLOT_PERIOD_MILLI * MICSECINMILISEC, TRUE, FALSE, PublicSchedulerTaskHandler2);
//	ASSERT_SP(rm == TimerSupported);
	//rm = VirtTimer_SetTimer(HAL_SLOT_TIMER3, 0, 5 * MICSECINMILISEC, TRUE, FALSE, PublicPostExecutionTaskHandler1);
	//ASSERT_SP(rm == TimerSupported);
	//VirtTimer_SetTimer(HAL_POSTEXECUTE_TIMER, 0, SLOT_PERIOD_MILLI * MICSECINMILISEC, TRUE, FALSE, PublicPostExecuteTaskTCallback);
	OMAC_scheduler_TimerCompletion.Initialize();

	//Initialize Handlers
	m_DiscoveryHandler.Initialize(radioID, macID);
	m_DataReceptionHandler.Initialize(radioID, macID);
	m_DataTransmissionHandler.Initialize();
	m_TimeSyncHandler.Initialize(radioID, macID);

	m_InitializationTimeinTicks = HAL_Time_CurrentTicks();

	ScheduleNextEvent();
}

void OMACScheduler::UnInitialize(){
	OMAC_scheduler_TimerCompletion.Abort();
	OMAC_scheduler_TimerCompletion.Uninitialize();
}

UINT64 OMACScheduler::GetSlotNumber(){
	UINT64 currentTicks = HAL_Time_CurrentTicks();
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
	UINT64 cur_ticks = HAL_Time_CurrentTicks();
	UINT64 ticks_till_end = SLOT_PERIOD_TICKS - ( (cur_ticks + SLOT_PERIOD_TICKS) % SLOT_PERIOD_TICKS);
	UINT32 ms_till_end = ((UINT32) ticks_till_end) / (TICKS_PER_MILLI / MICSECINMILISEC ) ;
	return ms_till_end;
}

/* ScheduleNextEvent
 *
 */
void OMACScheduler::ScheduleNextEvent(){
	g_OMAC.UpdateNeighborTable();
	VirtualTimerReturnMessage rm;

	UINT64 rxEventOffset = 0, txEventOffset = 0, beaconEventOffset = 0, timeSyncEventOffset=0;
	nextWakeupTimeInMicSec = MAXSCHEDULERUPDATE;


	timeSyncEventOffset = m_TimeSyncHandler.NextEvent();
	if (timeSyncEventOffset < 1) timeSyncEventOffset = 0xffffffffffffffff;


	rxEventOffset = m_DataReceptionHandler.NextEvent();
	if (rxEventOffset < 1) rxEventOffset = 0xffffffffffffffff;
	//rxEventOffset = rxEventOffset-1;
	txEventOffset = m_DataTransmissionHandler.NextEvent();
	if (txEventOffset < 1) txEventOffset = 0xffffffffffffffff;
	//txEventOffset = txEventOffset-1;
	beaconEventOffset = m_DiscoveryHandler.NextEvent();
	if (beaconEventOffset < 1) beaconEventOffset = 0xffffffffffffffff;
	//beaconEventOffset = beaconEventOffset -1;


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
	}
	else if(txEventOffset == nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec = nextWakeupTimeInMicSec + GUARDTIME_MICRO + SWITCHING_DELAY_MICRO;
		InputState.RequestState(I_DATA_SEND_PENDING);
	}
	else if(beaconEventOffset == nextWakeupTimeInMicSec) {
		InputState.RequestState(I_DISCO_PENDING);
	}
	else if(timeSyncEventOffset == nextWakeupTimeInMicSec) {
		InputState.RequestState(I_TIMESYNC_PENDING);
	}
	else{
		InputState.RequestState(I_IDLE);
	}


#ifdef def_Neighbor2beFollowed
#ifdef OMAC_DEBUG_PRINTF
	UINT64 curTicks = HAL_Time_CurrentTicks();
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] OMACScheduler::ScheduleNextEvent() nextWakeupTimeInMicSec= %llu AbsnextWakeupTimeInMicSec= %llu - %lu InputState.GetState() = %d \n"
			, HAL_Time_TicksToTime(curTicks), GetSlotNumberfromTicks(curTicks), m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks), GetSlotNumberfromTicks(m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), nextWakeupTimeInMicSec, HAL_Time_TicksToTime(curTicks)+nextWakeupTimeInMicSec, GetSlotNumberfromMicroSec(HAL_Time_TicksToTime(curTicks)+nextWakeupTimeInMicSec), InputState.GetState() );
	if(curTicks - m_InitializationTimeinTicks > (120 * 8000000)){
		hal_printf("Critial TIme has Passed. Be careful. About to crash!!\n");
	}
#endif
#endif
	//nextWakeupTimeInMicSec = nextWakeupTimeInMicSec - TIMER_EVENT_DELAY_OFFSET; //BK: There seems to be a constant delay in timers. This is to compansate for it.

	SchedulerINUse = true;
	rm = VirtTimer_Change(HAL_SLOT_TIMER, 0, nextWakeupTimeInMicSec, FALSE); //1 sec Timer in micro seconds
	ASSERT_SP(rm == TimerSupported);
	rm = VirtTimer_Start(HAL_SLOT_TIMER);
	ASSERT_SP(rm == TimerSupported);


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
	//g_OMAC.UpdateNeighborTable();
	UINT64 curTicks = HAL_Time_CurrentTicks();

#ifdef def_Neighbor2beFollowed
#ifdef OMAC_DEBUG_PRINTF
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] OMACScheduler::RunEventTask() \n"
			, HAL_Time_TicksToTime(curTicks), GetSlotNumber(), HAL_Time_TicksToTime(m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), GetSlotNumberfromTicks(m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)) );
#endif
#endif

	switch(InputState.GetState()) {
		case I_DATA_SEND_PENDING:
			m_lastHandler = DATA_TX_HANDLER;
			m_DataTransmissionHandler.ExecuteEvent();
			break;
		case I_DATA_RCV_PENDING:
			m_lastHandler = DATA_RX_HANDLER;
			m_DataReceptionHandler.ExecuteEvent();
			break;
		case I_TIMESYNC_PENDING:
			m_lastHandler = TIMESYNC_HANDLER;
			m_TimeSyncHandler.ExecuteEvent();
			break;
		case I_DISCO_PENDING:
			m_DiscoveryHandler.ExecuteEvent();
			m_lastHandler = CONTROL_BEACON_HANDLER;
			break;
		default: //Case for
			PostPostExecution();
	}
}

void OMACScheduler::PostExecution(){
	//VirtTimer_Start(HAL_SLOT_TIMER3);
	PostPostExecution();
}

void OMACScheduler::PostPostExecution(){
	EnsureStopRadio();
	InputState.ForceState(I_IDLE);
	ScheduleNextEvent();
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( SCHED_START_STOP_PIN, FALSE );
#endif
}

bool OMACScheduler::EnsureStopRadio(){
	DeviceStatus  ds = DS_Success;
	ds = g_omac_RadioControl.Stop();
	if (ds == DS_Success) {
		return TRUE;
	}
	else {//TODO: BK : This should be implemented in the case of radio not stopping
		return FALSE;
	}
}








