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

void PublicSchedulerTaskHandler(void * param){
	g_omac_scheduler.RunEventTask();
}

void OMACScheduler::Initialize(UINT8 _radioID, UINT8 _macID){
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
	VirtTimer_SetTimer(HAL_SLOT_TIMER, 0, SLOT_PERIOD_MILLI * MICSECINMILISEC, TRUE, FALSE, PublicSchedulerTaskHandler);
	//VirtTimer_SetTimer(HAL_POSTEXECUTE_TIMER, 0, SLOT_PERIOD_MILLI * MICSECINMILISEC, TRUE, FALSE, PublicPostExecuteTaskTCallback);

	//Initialize Handlers
	m_DiscoveryHandler.SetParentSchedulerPtr(this);
	m_DiscoveryHandler.Initialize(radioID, macID);
	m_DataReceptionHandler.Initialize(radioID, macID);
	m_DataTransmissionHandler.Initialize();
	m_TimeSyncHandler.Initialize(radioID, macID);

	//Initialize various processes
	this->StartSlotAlarm((UINT64) SLOT_PERIOD);
	//this->StartDiscoveryTimer(1000*(UINT64)TICKS_PER_MILLI);

	OMAC_scheduler_TimerCompletion.Initialize();
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

	rxEventOffset = m_DataReceptionHandler.NextEvent(m_slotNo);

	txEventOffset = m_DataTransmissionHandler.NextEvent(m_slotNo);

	beaconEventOffset = m_DiscoveryHandler.NextEvent(m_slotNo);

	timeSyncEventOffset = m_TimeSyncHandler.NextEvent(m_slotNo);

	if(rxEventOffset < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = rxEventOffset;
		InputState.RequestState(I_DATA_RCV_PENDING);
	}
	if(txEventOffset < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = txEventOffset;
		InputState.RequestState(I_DATA_SEND_PENDING);
	}
	if(beaconEventOffset < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = beaconEventOffset;
		InputState.RequestState(I_DISCO_PENDING);
	}
	if(timeSyncEventOffset < nextWakeupTimeInMicSec) {
		nextWakeupTimeInMicSec  = timeSyncEventOffset;
		InputState.RequestState(I_TIMESYNC_PENDING);
	}

	BOOL* completionFlag = (BOOL*)false;
	// we assume only 1 can be active, abort previous just in case
	OMAC_scheduler_TimerCompletion.Abort();
	OMAC_scheduler_TimerCompletion.InitializeForISR(RunEventTask, completionFlag);
	//Enqueue a task to listen for messages 100 usec from now (almost immediately)
	//TODO (Ananth): to check what the right enqueue value should be
	OMAC_scheduler_TimerCompletion.EnqueueDelta(nextWakeupTimeInMicSec);
}

bool OMACScheduler::RunEventTask(){
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
	ScheduleNextEvent();
}

bool OMACScheduler::EnsureStopRadio(){
	DeviceStatus  ds = DS_Success;
	bool e = FALSE;
#ifdef FULL_DUTY_CYCLE
#warning "USING FULL_DUTY_CYCLE"
	Sleep();
	PrintDutyCycle();
	PostExecution();
	//printfflush();
	return;
#endif

	switch(ProtoState.GetState()) {
		case S_IDLE :
			InputState.ToIdle();
			return;
		case S_BEACON_1://State will soon change to S_BEACON_N
			return;
		default :
			ProtoState.ForceState(S_STOPPING);
			//InputState.ForceState(S_STOPPING);
			//CPU_GPIO_SetPinState( (GPIO_PIN) SCHED_START_STOP_PIN, FALSE );
			ds = g_omac_RadioControl.Stop();
	}
#ifdef OMAC_DEBUG
	//call SlotScheduler.printState();
	//printf("stop result e=%u\n", e);
#endif
	/* switch(e) {
		case  TRUE: // Ignore.  AMControl.stopDone will provide continuation.
			break;
		default : // Already shut down and we just don't know it.  Sleep.
			Sleep();
	} */
	if (ds == DS_Success) {
		ProtoState.ToIdle();
		InputState.ToIdle();
		return TRUE;
	}
	else {//TODO: BK : This should be implemented in the case of radio not stopping
		return FALSE;
	}
}








