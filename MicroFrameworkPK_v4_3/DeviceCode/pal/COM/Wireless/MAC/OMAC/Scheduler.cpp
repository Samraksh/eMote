/*
 * Scheduler.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: Bora Karaoglu
 */

#include <Samraksh/MAC/OMAC/Scheduler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>

extern RadioControl_t g_omac_RadioControl;
extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;

HAL_COMPLETION OMAC_scheduler_TimerCompletion;

#define MAXSCHEDULERUPDATE 5000000

bool flag = true; //BK: I don't know what this is used for

void PublicSchedulerTaskHandler1(void * param){
	g_omac_scheduler.RunEventTask();
	g_omac_scheduler.timer1INuse=false;
}
void PublicSchedulerTaskHandler2(void * param){
	g_omac_scheduler.RunEventTask();
	g_omac_scheduler.timer2INuse=false;
}
void OMACScheduler::Initialize(UINT8 _radioID, UINT8 _macID){
	timer1INuse = false;
	timer2INuse =false;
	radioID = _radioID;
	macID = _macID;

	
	CPU_GPIO_EnableOutputPin((GPIO_PIN) SCHED_START_STOP_PIN, FALSE);

#ifdef PROFILING
	minStartDelay = 300; maxStartDelay = 10;
	minStopDelay = 300; maxStopDelay = 10;
#endif

	InputState.ToIdle();


	//Initialize the HAL vitual timer layer
	VirtTimer_Initialize();
	VirtTimer_SetTimer(HAL_SLOT_TIMER, 0, SLOT_PERIOD_MILLI * MICSECINMILISEC, TRUE, FALSE, PublicSchedulerTaskHandler1);
	VirtTimer_SetTimer(HAL_SLOT_TIMER2, 0, SLOT_PERIOD_MILLI * MICSECINMILISEC, TRUE, FALSE, PublicSchedulerTaskHandler2);

	//VirtTimer_SetTimer(HAL_POSTEXECUTE_TIMER, 0, SLOT_PERIOD_MILLI * MICSECINMILISEC, TRUE, FALSE, PublicPostExecuteTaskTCallback);
	OMAC_scheduler_TimerCompletion.Initialize();

	//Initialize Handlers
	m_DiscoveryHandler.SetParentSchedulerPtr(this);
	m_DiscoveryHandler.Initialize(radioID, macID);
	m_DataReceptionHandler.Initialize(radioID, macID);
	m_DataTransmissionHandler.Initialize();
	m_TimeSyncHandler.Initialize(radioID, macID);

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

	UINT64 rxEventOffset = 0, txEventOffset = 0, beaconEventOffset = 0, timeSyncEventOffset=0;
	UINT64 nextWakeupTimeInMicSec = MAXSCHEDULERUPDATE;
	rxEventOffset = m_DataReceptionHandler.NextEvent();
	if (rxEventOffset < MINEVENTTIME) rxEventOffset = 0xffffffffffffffff;
	//rxEventOffset = rxEventOffset-1;
	txEventOffset = m_DataTransmissionHandler.NextEvent();
	if (txEventOffset < MINEVENTTIME) txEventOffset = 0xffffffffffffffff;
	//txEventOffset = txEventOffset-1;
	beaconEventOffset = m_DiscoveryHandler.NextEvent();
	if (beaconEventOffset < MINEVENTTIME) beaconEventOffset = 0xffffffffffffffff;
	//beaconEventOffset = beaconEventOffset -1;
	timeSyncEventOffset = m_TimeSyncHandler.NextEvent();
	if (timeSyncEventOffset < MINEVENTTIME) timeSyncEventOffset = 0xffffffffffffffff;
	//timeSyncEventOffset = timeSyncEventOffset-1;

	if(rxEventOffset < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = rxEventOffset;
	}
	if(txEventOffset < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = txEventOffset;
	}
	if(beaconEventOffset < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = beaconEventOffset;
	}
	if(timeSyncEventOffset < nextWakeupTimeInMicSec) {
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


	UINT64 curTicks = HAL_Time_CurrentTicks();
#ifdef def_Neighbor2beFollowed
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] OMACScheduler::ScheduleNextEvent() nextWakeupTimeInMicSec= %llu AbsnextWakeupTimeInMicSec= %llu - %lu InputState.GetState() = %d \n"
			, HAL_Time_TicksToTime(curTicks), GetSlotNumberfromTicks(curTicks), m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks), GetSlotNumberfromTicks(m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), nextWakeupTimeInMicSec, HAL_Time_TicksToTime(curTicks)+nextWakeupTimeInMicSec, GetSlotNumberfromMicroSec(HAL_Time_TicksToTime(curTicks)+nextWakeupTimeInMicSec), InputState.GetState() );

#endif
	nextWakeupTimeInMicSec = nextWakeupTimeInMicSec - TIMER_EVENT_DELAY_OFFSET; //BK: There seems to be a constant delay in timers. This is to compansate for it.
	//hal_printf("OMACScheduler::ScheduleNextEvent nextWakeupTimeInMicSec: %llu\n", nextWakeupTimeInMicSec);

	if(!timer1INuse){
		timer1INuse = true;
		VirtTimer_Change(HAL_SLOT_TIMER, 0, nextWakeupTimeInMicSec, TRUE); //1 sec Timer in micro seconds
		VirtTimer_Start(HAL_SLOT_TIMER);
	}
	else if(!timer2INuse){
		timer2INuse = true;
		VirtTimer_Change(HAL_SLOT_TIMER2, 0, nextWakeupTimeInMicSec, TRUE); //1 sec Timer in micro seconds
		VirtTimer_Start(HAL_SLOT_TIMER2);
	}
	else{
		ASSERT(1==0);
	}
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
	g_OMAC.UpdateNeighborTable();
	UINT64 curTicks = HAL_Time_CurrentTicks();
#ifdef def_Neighbor2beFollowed
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] OMACScheduler::RunEventTask() \n"
			, HAL_Time_TicksToTime(curTicks), GetSlotNumber(), HAL_Time_TicksToTime(m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), GetSlotNumberfromTicks(m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)) );
#endif
	switch(InputState.GetState()) {
		case I_DATA_SEND_PENDING:
			//hal_printf("OMACScheduler::RunEventTask I_DATA_SEND_PENDING\n");
			m_lastHandler = DATA_TX_HANDLER;
			m_DataTransmissionHandler.ExecuteEvent();
			break;
		case I_DATA_RCV_PENDING:
			//hal_printf("OMACScheduler::RunEventTask I_DATA_RCV_PENDING\n");
			m_lastHandler = DATA_RX_HANDLER;
			m_DataReceptionHandler.ExecuteEvent();
			break;
		case I_TIMESYNC_PENDING:
			//hal_printf("OMACScheduler::RunEventTask I_TIMESYNC_PENDING\n");
			m_lastHandler = TIMESYNC_HANDLER;
			m_TimeSyncHandler.ExecuteEvent();
			break;
		case I_DISCO_PENDING:
			m_DiscoveryHandler.ExecuteEvent();
			m_lastHandler = CONTROL_BEACON_HANDLER;
			break;
		default: //Case for
			ScheduleNextEvent();
	}
}

void OMACScheduler::PostExecution(){
	EnsureStopRadio();
	InputState.ForceState(I_IDLE);
	ScheduleNextEvent();
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








