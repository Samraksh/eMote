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
	//Initialize variables
	startMeasuringDutyCycle=TRUE; //Profiling variable, set to true to start sending/receiving
	dutyCycleReset = 0;
	totalRadioUp = 0;
	
	CPU_GPIO_EnableOutputPin((GPIO_PIN) SCHED_START_STOP_PIN, FALSE);

#ifdef PROFILING
	minStartDelay = 300; maxStartDelay = 10;
	minStopDelay = 300; maxStopDelay = 10;
#endif

	ProtoState.ToIdle();
	InputState.ToIdle();

	m_discoveryTicks = 0, m_shldPrintDuty = 0;
	m_slotNo = 0;
	m_nonSleepStateCnt = 0;
	m_lastHandler = NULL_HANDLER;
	m_lastPiggybackSlot = 0;
	m_radioDelayAvg = 0;
	radioTiming = 0;
	m_busy = FALSE;

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

UINT32 OMACScheduler::GetSlotNumber(){
	return ( HAL_Time_CurrentTicks() / SLOT_PERIOD_TICKS);
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
	UINT64 rxEventOffset = 0, txEventOffset = 0, beaconEventOffset = 0, timeSyncEventOffset=0;
	UINT64 nextWakeupTimeInMicSec = MAXSCHEDULERUPDATE;
	m_slotNo = GetSlotNumber();
	rxEventOffset = m_DataReceptionHandler.NextEvent(GetSlotNumber());
	if (rxEventOffset < MINEVENTTIME) rxEventOffset = 0xffffffffffffffff;
	//rxEventOffset = rxEventOffset-1;
	txEventOffset = m_DataTransmissionHandler.NextEvent(GetSlotNumber());
	if (txEventOffset < MINEVENTTIME) txEventOffset = 0xffffffffffffffff;
	//txEventOffset = txEventOffset-1;
	beaconEventOffset = m_DiscoveryHandler.NextEvent(GetSlotNumber());
	if (beaconEventOffset < MINEVENTTIME) beaconEventOffset = 0xffffffffffffffff;
	//beaconEventOffset = beaconEventOffset -1;
	timeSyncEventOffset = m_TimeSyncHandler.NextEvent(GetSlotNumber());
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
		nextWakeupTimeInMicSec = nextWakeupTimeInMicSec + GUARDTIME_MICRO;
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


	UINT64 curTime = HAL_Time_CurrentTicks() / (TICKS_PER_MILLI/MICSECINMILISEC);
	hal_printf("\n[LT: %llu NT: %llu] OMACScheduler::ScheduleNextEvent() nextWakeupTimeInMicSec= %llu AbsnextWakeupTimeInMicSec= %llu \n",curTime, m_TimeSyncHandler.m_globalTime.Local2NeighborTime(m_TimeSyncHandler.Neighbor2beFollowed, curTime), nextWakeupTimeInMicSec,curTime+nextWakeupTimeInMicSec );

	nextWakeupTimeInMicSec = nextWakeupTimeInMicSec - TIMER_EVENT_DELAY_OFFSET; //BK: There seems to be a constant delay in timers. This is to compansate for it.

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
		assert(1==0);
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
	UINT64 curTime = HAL_Time_CurrentTicks() / (TICKS_PER_MILLI/MICSECINMILISEC);
	hal_printf("\n[LT: %llu NT: %llu] OMACScheduler::RunEventTask() \n",curTime, m_TimeSyncHandler.m_globalTime.Local2NeighborTime(m_TimeSyncHandler.Neighbor2beFollowed, curTime) );

	switch(InputState.GetState()) {
		case I_DATA_SEND_PENDING:
			hal_printf("OMACScheduler::RunEventTask I_DATA_SEND_PENDING\n");
			m_lastHandler = DATA_TX_HANDLER;
			m_DataTransmissionHandler.ExecuteEvent(m_slotNo);
			break;
		case I_DATA_RCV_PENDING:
			hal_printf("OMACScheduler::RunEventTask I_DATA_RCV_PENDING\n");
			m_lastHandler = DATA_RX_HANDLER;
			m_DataReceptionHandler.ExecuteEvent(m_slotNo);
			break;
		case I_TIMESYNC_PENDING:
			hal_printf("OMACScheduler::RunEventTask I_TIMESYNC_PENDING\n");
			m_lastHandler = TIMESYNC_HANDLER;
			m_TimeSyncHandler.ExecuteEvent(m_slotNo);
			break;
		case I_DISCO_PENDING:
			m_DiscoveryHandler.ExecuteEvent(m_slotNo);
			m_lastHandler = CONTROL_BEACON_HANDLER;
			break;
		default: //Case for
			ScheduleNextEvent();
	}
}

void OMACScheduler::PostExecution(){
	EnsureStopRadio();
	InputState.ForceState(I_IDLE);
	ProtoState.ToIdle();
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








