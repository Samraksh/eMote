/*
 * Scheduler.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan Sridharan;
 *  Modifications:
 *  	Authors: Bora Karaoglu; Ananth Muralidharan
 */

#include <Samraksh/MAC/OMAC/Scheduler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>

extern RadioControl_t g_omac_RadioControl;
extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;

HAL_COMPLETION OMAC_scheduler_TimerCompletion;

#define RADIO_START_STOP_PIN 120 //4

bool flag = true;

/*
 *
 */
void PublicSlotAlarmHanlder(void * param){
	g_omac_scheduler.SlotAlarmHandler(param);
}

/*
 *
 */
void PublicDataAlarmHandlder(void * param){
	g_omac_scheduler.DataAlarmHandler( param);
}

/*
 *
 */
void OMACScheduler::Initialize(UINT8 _radioID, UINT8 _macID){
	radioID = _radioID;
	macID = _macID;
	//Initialize variables
	startMeasuringDutyCycle=TRUE; //Profiling variable, set to true to start sending/receiving
	dutyCycleReset = 0;
	totalRadioUp = 0;
	
	CPU_GPIO_EnableOutputPin((GPIO_PIN) RADIO_START_STOP_PIN, FALSE);

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
	VirtTimer_SetTimer(HAL_SLOT_TIMER, 0, SLOT_PERIOD * MICSECINMILISEC, FALSE, FALSE, PublicSlotAlarmHanlder);
	VirtTimer_SetTimer(HAL_DATAALARM_TIMER, 0, SLOT_PERIOD * MICSECINMILISEC , TRUE, FALSE, PublicDataAlarmHandlder);

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

/*
 *
 */
void OMACScheduler::UnInitialize(){
	OMAC_scheduler_TimerCompletion.Abort();
	OMAC_scheduler_TimerCompletion.Uninitialize();
}

/*
 *
 */
void RadioTaskCallback(void* arg)
{
	g_omac_scheduler.RadioTask();
}


/**********************************************************************
 * Slot alarm APIs
 *********************************************************************/
/*
 *
 */
void OMACScheduler::StartSlotAlarm(UINT64 Delay){
	//Start the SlotAlarm
	//HALTimer()
	/*if(Delay==0){
		//start alarm in default periodic mode
		//VirtTimer_SetTimer(HAL_SLOT_TIMER, 0, SLOT_PERIOD * 1000, FALSE, FALSE, PublicSlotAlarmHanlder);
		VirtTimer_Change(HAL_SLOT_TIMER, 0, SLOT_PERIOD * 1000, FALSE); //1 sec Timer in micro seconds
		VirtTimer_Start(HAL_SLOT_TIMER);

	}else {
		//Change next slot time with delay
		//VirtTimer_SetTimer(HAL_SLOT_TIMER, 0, (Delay-4)*1000, FALSE, FALSE, PublicSlotAlarmHanlder);
		VirtTimer_Change(HAL_SLOT_TIMER, 0, (Delay-4)*1000, FALSE); //1 sec Timer in micro seconds
		VirtTimer_Start(HAL_SLOT_TIMER);
	}*/

	////VirtTimer_Change(HAL_SLOT_TIMER, 0, SLOT_PERIOD * 1000, FALSE); //1 sec Timer in micro seconds
	VirtTimer_Start(HAL_SLOT_TIMER);
}

/*
 *
 */
void OMACScheduler::SlotAlarmHandler(void* Param){
	////hal_printf("start OMACScheduler::SlotAlarmHandler\n");
#ifdef OMAC_DEBUG
	CPU_GPIO_SetPinState((GPIO_PIN) 1, TRUE);
#endif
	UINT64 localTime, nextWakeup, oldSlotNo;
	localTime = HAL_Time_CurrentTime();
	//increment counter
#ifdef PROFILING
	taskDelay1 = localTime;
#endif

	//Compensation for alarm drift becuase of cpu doing other things:
	//m_slotNoOffset: is the offset it fires the first time
	//Doublecheck logic: the logic is not working now
	/*if (m_slotNo == 0) {
		m_slotNoOffset = localTime - (1 << SLOT_PERIOD_BITS);
		m_slotNo = 1;
	}
	else {
		oldSlotNo = m_slotNo;
		//have to compensate for numerical errors
		m_slotNo = (localTime - m_slotNoOffset) >> SLOT_PERIOD_BITS;
		//INVARIANT: localTime = m_slotNo << SLOT_PERIOD_BITS + m_slotNoOffset
		if (m_slotNo <= oldSlotNo) {
			m_slotNo = oldSlotNo + 1;
		}
	}*/
	//Using a simple increment for the time-being
	m_slotNo++;

	//nextWakeup = ((m_slotNo + 1) << SLOT_PERIOD_BITS) + m_slotNoOffset - localTime;
	//this->StartSlotAlarm(nextWakeup);


	//Mukundan: At this point, instead of posting a task as in TinyOS,
	//just run the task directly
	//We will revisit the whole task architecture later.
	this->RunEventTask();
	////hal_printf("end OMACScheduler::SlotAlarmHandler\n");

#ifdef OMAC_DEBUG
	if(m_slotNo % 1000 == 0) {
		hal_printf("curSlot %lu time %llu\n", m_slotNo, localTime);
	}
	CPU_GPIO_SetPinState((GPIO_PIN) 1, FALSE);
#endif
}

/**********************************************************************
 * Data alarm APIs
 *********************************************************************/
/*
 *
 */
void OMACScheduler::StartDataAlarm(UINT64 Delay){
	//Start the SlotAlarm
	//HALTimer()
	if(Delay == 0){
		void* param;
		this->DataAlarmHandler(param);
	}else {
		VirtTimer_Change(HAL_DATAALARM_TIMER, 0, Delay, FALSE);
		VirtTimer_Start(HAL_DATAALARM_TIMER);
	}
}

/*
 *
 */
void OMACScheduler::DataAlarmHandler(void* Param){
	////hal_printf("start OMACScheduler::DataAlarmHandler\n");
	UINT64 localTime, nextWakeup, oldSlotNo;
	////localTime = HAL_Time_CurrentTime();
	//localTicks = HAL_Time_CurrentTicks();

	//increment counter
#ifdef PROFILING
	taskDelay1 = localTime;
#endif

	/*
	 * Compensation for alarm drift: See Wenjies notes
	 * if (m_slotNo == 0) {
		m_slotNoOffset = localTime - (1 << SLOT_PERIOD_BITS);
		m_slotNo = 1;
	}
	else {
		oldSlotNo = m_slotNo;
		//have to compensate for numerical errors
		m_slotNo = (localTime - m_slotNoOffset) >> SLOT_PERIOD_BITS;
		//INVARIANT: localTime = m_slotNo << SLOT_PERIOD_BITS + m_slotNoOffset
		if (m_slotNo <= oldSlotNo) {
			m_slotNo = oldSlotNo + 1;
		}
	}
	nextWakeup = ((m_slotNo + 1) << SLOT_PERIOD_BITS) + m_slotNoOffset - localTime;
	this->StartSlotAlarm(nextWakeup);
	*/

	//Mukundan: At this point, instead of posting a task as in TinyOS,
	//just run the task directly
	//We will revisit the whole task architecture later.
	////this->RunSlotTask();
	m_DataTransmissionHandler.ExecuteEvent(m_slotNo);

	////hal_printf("end OMACScheduler::DataAlarmHandler\n");

#ifdef OMAC_DEBUG
	//if(m_slotNo % 1000 == 0) {
	//	debug_printf("curSlot %lu time %lu\n", m_slotNo, localTime);
	//}
#endif
}

/*
 *
 */
bool OMACScheduler::IsRunningDataAlarm(){
	return TRUE; //BK, Something should be returned
}

/**********************************************************************
 * Discovery timer APIs
 *********************************************************************/
/*
 *
 */
void OMACScheduler::StartDiscoveryTimer(UINT64 Delay){
	//Start the SlotAlarm
	//HALTimer()
	if(Delay==0){
		//start alarm in default periodic mode
		//HALDriver :: Initialize (, TRUE, 0, 0, m_OMACScheduler.SlotAlarm, NULL)
	}else {
		//Change next slot time with delay
		//HALDriver :: Initialize (, TRUE, 0, 0, m_OMACScheduler.SlotAlarm, NULL)
	}
}

/*
 *
 */
void OMACScheduler::DiscoveryTimerHandler(void* Param){

}

/*
 *
 */
bool OMACScheduler::RunEventTask(){
	////hal_printf("start OMACScheduler::RunEventTask\n");
	UINT64 rxEventOffset = 0, txEventOffset = 0, beaconEventOffset = 0, timeSyncEventOffset=0;
	////UINT64 currentTicks = 0;

#ifdef PROFILING
	taskDelay1 = call SubLocalTime.get() - taskDelay1;
#endif

	g_OMAC.UpdateNeighborTable();
	// the NextSlot() for receiver must be called every slot as there are state update
	// operations in it
	////DISABLE_INTERRUPTS();
	rxEventOffset = m_DataReceptionHandler.NextEvent(m_slotNo);
	hal_printf("\nOMACScheduler::RunEventTask() CurTicks: %llu m_slotNo: %d rxEventOffset: %d \n",HAL_Time_CurrentTicks(), m_slotNo, rxEventOffset);
	////rxEventOffset |= HAL_Time_CurrentTicks();
	////ENABLE_INTERRUPTS();

	//BK:This is disabled since should not happen anyways
	// if(InputState.IsState(I_DATA_SEND_PENDING) || !(ProtoState.IsIdle())) {
		//printf("[S %u I %u\n] incorrect state\n", call State.getState(),
		//Input.GetState());
		//return FALSE;
	//}


	/* notice that the transmission handler returns the ticks (1/32 of a milli sec) for the
	 * next scheduled transmission, whereas the reception handler returns the number of slots
	 * before the next scheduled reception*/
	////DISABLE_INTERRUPTS();
	txEventOffset = m_DataTransmissionHandler.NextEvent(m_slotNo);
	////txEventOffset |= HAL_Time_CurrentTicks();
	////ENABLE_INTERRUPTS();

	////DISABLE_INTERRUPTS();
	beaconEventOffset = m_DiscoveryHandler.NextEvent(m_slotNo);
	////beaconEventOffset |= HAL_Time_CurrentTicks();
	////ENABLE_INTERRUPTS();

	////DISABLE_INTERRUPTS();
	timeSyncEventOffset = m_TimeSyncHandler.NextEvent(m_slotNo);
	////timeSyncEventOffset |= HAL_Time_CurrentTicks();
	////ENABLE_INTERRUPTS();
	//	if (rxSlotOffset < 2 * SLOT_PERIOD_32KHZ
	//		|| beaconSlotOffset < 2 * SLOT_PERIOD_32KHZ || txSlotOffset < 2 * SLOT_PERIOD_32KHZ ) {
	//		call OMacSignal.yield();
	//	} else {
	//		call OMacSignal.resume();
	//	}

	////currentTicks = HAL_Time_CurrentTicks();
	///I am already scheduled to send a message this frame, let me play it safe and not do anything now
	////if( startMeasuringDutyCycle && txEventOffset < (currentTicks | (SLOT_PERIOD_MILLI * 1000)) ) {
	if( startMeasuringDutyCycle && txEventOffset < (SLOT_PERIOD_MILLI * 1000) ) {
		if(InputState.GetState() == I_DATA_SEND_PENDING){
			////hal_printf("OMACScheduler::RunEventTask state is already I_DATA_SEND_PENDING\n");

			/*GLOBAL_LOCK(irq);
			VirtTimer_Stop(HAL_SLOT_TIMER);
			m_DataTransmissionHandler.ExecuteEvent(m_slotNo);
			VirtTimer_Start(HAL_SLOT_TIMER);
			InputState.ToIdle();*/

			/*BOOL* completionFlag = (BOOL*)false;
			// we assume only 1 can be active, abort previous just in case
			OMAC_scheduler_TimerCompletion.Abort();
			OMAC_scheduler_TimerCompletion.InitializeForISR(RadioTaskCallback, completionFlag);
			//Enqueue a task to listen for messages 100 usec from now (almost immediately)
			//TODO (Ananth): to check what the right enqueue value should be
			OMAC_scheduler_TimerCompletion.EnqueueDelta(100);*/

			return TRUE;
		}
		else {
			if(InputState.RequestState(I_DATA_SEND_PENDING) == DS_Success) {
				hal_printf("OMACScheduler::RunEventTask setting state to I_DATA_SEND_PENDING\n");
				//Instead of setting an alarm to go off for DataSend, send data immediately. No other task should interrupt this activity.
				////StartDataAlarm(txEventOffset);

				/*GLOBAL_LOCK(irq);
				VirtTimer_Stop(HAL_SLOT_TIMER);
				m_DataTransmissionHandler.ExecuteEvent(m_slotNo);
				VirtTimer_Start(HAL_SLOT_TIMER);
				InputState.ToIdle();*/

				BOOL* completionFlag = (BOOL*)false;
				// we assume only 1 can be active, abort previous just in case
				OMAC_scheduler_TimerCompletion.Abort();
				OMAC_scheduler_TimerCompletion.InitializeForISR(PublicDataAlarmHandlder, completionFlag);
				//Enqueue a task to listen for messages 100 usec from now (almost immediately)
				//TODO (Ananth): to check what the right enqueue value should be
				OMAC_scheduler_TimerCompletion.EnqueueDelta(txEventOffset+100);

				return TRUE;
			}
		}
	}
	/* we are not going to send any beacons if neighbor is not going to receive*/
	else if(TRUE){ //!isNeighborGoingToReceive()) {
		//Keep polling until the timesync flag is reset
		////hal_printf("status CMaxTimeSync::timeSyncSendFlag %d\n", CMaxTimeSync::timeSyncSendFlag);
		/*if(CMaxTimeSync::timeSyncSendFlag) {
			VirtTimer_Stop(HAL_SLOT_TIMER);
		}
		while(CMaxTimeSync::timeSyncSendFlag){
			hal_printf("sleeping\n");
			HAL_Time_Sleep_MicroSeconds(5000);
		}
		VirtTimer_Start(HAL_SLOT_TIMER);*/

		if(startMeasuringDutyCycle && rxEventOffset == 0) {
			if(InputState.RequestState(I_DATA_RCV_PENDING) == DS_Success) {
				//call OMacSignal.yield();
				//Mukundan:Instead of posting RadioTask just run it directly

				hal_printf("OMACScheduler::RunEventTask I_DATA_RCV_PENDING\n");
				BOOL* completionFlag = (BOOL*)false;
				// we assume only 1 can be active, abort previous just in case
				OMAC_scheduler_TimerCompletion.Abort();
				OMAC_scheduler_TimerCompletion.InitializeForISR(RadioTaskCallback, completionFlag);
				//Enqueue a task to listen for messages 100 usec from now (almost immediately)
				//TODO (Ananth): to check what the right enqueue value should be
				OMAC_scheduler_TimerCompletion.EnqueueDelta(100);

				/*if(!RadioTask()){
					debug_printf("RadioTask failed for sending data\n");
					return FALSE;
				}*/
			}
			else {
				//debug_printf("request to I_DATA_RCV_PENDING failed\n");
			}
		}
		// do not send Time Sync message, if i'm going to receive or send packets in 2 slots
		else if( (timeSyncEventOffset == 0) && (rxEventOffset > 2) && (txEventOffset > 2) )  {

			InputState.ForceState(I_TIMESYNC_PENDING);

			BOOL* completionFlag = (BOOL*)false;
			// we assume only 1 can be active, abort previous just in case
			OMAC_scheduler_TimerCompletion.Abort();
			OMAC_scheduler_TimerCompletion.InitializeForISR(RadioTaskCallback, completionFlag);
			//Enqueue a task to listen for messages 100 usec from now
			//TODO (Ananth): to check what the right enqueue value should be
			OMAC_scheduler_TimerCompletion.EnqueueDelta(100);

			/*if(!RadioTask()) {
				debug_printf("RadioTask failed for timesync\n");
				return FALSE;
			}*/
		}
		// do not send Discovery message, if i'm going to receive or send packets in 2 slots
		else if( (beaconEventOffset == 0) && (rxEventOffset > 2) && (txEventOffset > 2) )  {
			//if (startMeasuringDutyCycle) {
			//	return TRUE;
			//}
			InputState.ToIdle();

			BOOL* completionFlag = (BOOL*)false;
			// we assume only 1 can be active, abort previous just in case
			OMAC_scheduler_TimerCompletion.Abort();
			OMAC_scheduler_TimerCompletion.InitializeForISR(RadioTaskCallback, completionFlag);
			//Enqueue a task to listen for messages 100 usec from now
			//TODO (Ananth): to check what the right enqueue value should be
			OMAC_scheduler_TimerCompletion.EnqueueDelta(100);

			/*if(!RadioTask()) {
				debug_printf("RadioTask failed\n");
				return FALSE;
			}*/
		}

	}
	else {
		debug_printf("neighbor is receiving\n");
	}
	////hal_printf("end OMACScheduler::RunSlotTask\n");
	return TRUE;
}

/*
 * This turns on the radio and sends a message
 */
bool OMACScheduler::RadioTask(){
	////hal_printf("start OMACScheduler::RadioTask\n");
	DeviceStatus e = DS_Fail;
	//radioTiming = call GlobalTime.getLocalTime();
	radioTiming = HAL_Time_CurrentTime();
	//radioTiming = m_timeSync.GlobalTime();

	bool reqStateRet = ProtoState.RequestState(S_STARTING);

	if(reqStateRet) {
		CPU_GPIO_SetPinState( (GPIO_PIN) RADIO_START_STOP_PIN, TRUE );
		//hal_printf("Starting PLL\n");
		e = g_omac_RadioControl.StartRx();
	}
	/*else {
		ProtoState.ForceState(S_STARTING);
		CPU_GPIO_SetPinState( (GPIO_PIN) RADIO_START_STOP_PIN, TRUE );
		e = g_omac_RadioControl.StartRx();
		//hal_printf("OMACScheduler::RadioTask radio start failed. state=%u\r\n", ProtoState.GetState());
		//return FALSE;
	}*/

	if(e == DS_Success) {
		switch(InputState.GetState()) {
			case I_DATA_SEND_PENDING:
				hal_printf("OMACScheduler::RadioTask I_DATA_SEND_PENDING\n");
				////Not needed, as StartDataAlarm does the same thing
				m_DataTransmissionHandler.ExecuteEvent(m_slotNo);
				m_lastHandler = DATA_TX_HANDLER;
				break;
			case I_DATA_RCV_PENDING:
				hal_printf("OMACScheduler::RadioTask I_DATA_RCV_PENDING\n");
				m_DataReceptionHandler.ExecuteEvent(m_slotNo);
				m_lastHandler = DATA_RX_HANDLER;
				break;
			case I_TIMESYNC_PENDING:
				hal_printf("OMACScheduler::RadioTask I_TIMESYNC_PENDING\n");
				m_TimeSyncHandler.ExecuteEvent(m_slotNo);
				m_lastHandler = TIMESYNC_HANDLER;
				break;
			default:
				m_DiscoveryHandler.ExecuteEvent(m_slotNo);
				m_lastHandler = CONTROL_BEACON_HANDLER;
				break;
		}
	}
	else {
		//hal_printf("StartPLL failed\n");
		debug_printf("RadioTask failed\n");
		return false;
	}

	//hal_printf("Starting rx\n");
	//g_omac_RadioControl.StartRx();

	PostExecution();
	////hal_printf("end OMACScheduler::RadioTask\n");
	return TRUE;
}

/*
 *
 */
void OMACScheduler::Stop(){
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
			//CPU_GPIO_SetPinState( (GPIO_PIN) RADIO_START_STOP_PIN, FALSE );
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
	if (ds == DS_Success) Sleep();
}

/**********************************************************************
 * Private functions
 *********************************************************************/
/*
 *
 */
void OMACScheduler::Sleep(){
	ProtoState.ToIdle();
	InputState.ToIdle();
}

/*
 *
 */
bool OMACScheduler::IsNeighborGoingToReceive(){
	/*UINT8 nbrIndex;
	TableItem * nbrEntry;
	for(nbrIndex = 0; nbrIndex < MAX_NBR_SIZE; nbrIndex++) {
		//nbrEntry = call NeighborhoodManager.getNeighborByIdx(nbrIndex);
		if(nbrEntry->nodeID != INVALID_ADDRESS) {
			UINT32 globalTime;
			UINT32 counter;
			UINT32 p = nbrEntry->dataInterval;
			//1st,  compute the current localTime of the neighbor
			//call GlobalTime.getGlobalTime(&globalTime, nbrEntry->nodeID);
			// 2nd, compute neighbor's current counter value. The start of the counter
			//is later than the start of the clock. So, a counter offset should be used
			counter = (globalTime - nbrEntry->counterOffset) >> SLOT_PERIOD_BITS;
			// if counter % p == p - 1, neighbor going to receive in the next slot;
			// if counter % p < DWELL_COUNT/2, the neighbor might be very likely to be
			//  receiving dwell packets
			if(counter % p < DWELL_COUNT / 2 || counter % p == p - 1) {
				return TRUE;
			}
		}
	}*/
	return FALSE;
}

/*
 *
 */
void OMACScheduler::PostExecution(){
	switch(m_lastHandler) {
		case CONTROL_BEACON_HANDLER :
			m_DiscoveryHandler.PostExecuteEvent();
			break;
		case TIMESYNC_HANDLER :
			m_TimeSyncHandler.PostExecuteEvent();
			break;
		case DATA_TX_HANDLER :
			//also notify the DiscoveryHandler in case
			//there is piggyback beacon received
			m_DataTransmissionHandler.PostExecuteEvent();
			m_DiscoveryHandler.PostExecuteEvent();
			break;
		case DATA_RX_HANDLER :
			m_DataReceptionHandler.PostExecuteEvent();
			m_DiscoveryHandler.PostExecuteEvent();
			break;
		default :
			break;
	}
	InputState.ToIdle();
	ProtoState.ToIdle();
#ifdef PROFILING
	debug_printf("delay1=%lu,startDelay=%lu,stopDelay=%lu\n", taskDelay1, startDelay, stopDelay);
	//printf("minStart=%lu,maxStart=%lu,minStop=%lu,maxStop=%lu\n", minStartDelay,
	//		maxStartDelay, minStopDelay, maxStopDelay);
	debug_printf("powerDelay=%lu, oscDelay=%lu, taskDelay=%lu\n", RadioDutyCycle.dbg1(), RadioDutyCycle.dbg2(), RadioDutyCycle.dbg3());
#endif
	//call OMacSignal.resume();
}

/*
 *
 */
void OMACScheduler::PrintDutyCycle(){

}
