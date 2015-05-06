/*
 * Scheduler.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan
 */

#include "Scheduler.h"
#include "OMAC.h"

extern RadioControl_t g_omac_RadioControl;
extern OMACTypeBora g_OMAC;
extern OMACSchedulerBora g_omac_scheduler;

void PublicSlotAlarmHanlder(void * param){
	g_omac_scheduler.SlotAlarmHandler(param);
}

void PublicDataAlarmHandlder(void * param){
	g_omac_scheduler.DataAlarmHandler( param);
}

void OMACSchedulerBora::Initialize(UINT8 _radioID, UINT8 _macID){
	radioID = _radioID;
	macID = _macID;
	//Initialize variables
	startMeasuringDutyCycle=TRUE; //Profiling variable, set to true to start sending/receiving
	dutyCycleReset = 0;
	totalRadioUp = 0;


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
	VirtTimer_SetTimer(HAL_DATAALARM_TIMER, 0, SLOT_PERIOD * MICSECINMILISEC , TRUE, FALSE, PublicSlotAlarmHanlder);


	//Initialize Handlers
	 m_DiscoveryHandler.SetParentSchedulerPtr(this);
	 m_DiscoveryHandler.Initialize(radioID, macID);
	 m_DataReceptionHandler.Initialize(radioID, macID);
	 m_DataTransmissionHandler.Initialize();
	 m_TimeSyncHandler.Initialize(radioID, macID);

	//Initialize various processes
	this->StartSlotAlarm((UINT64) SLOT_PERIOD);
	this->StartDiscoveryTimer(1000*(UINT64)TICKS_PER_MILLI);


}
void OMACSchedulerBora::UnInitialize(){

}

bool OMACSchedulerBora::RunSlotTask(){
	UINT32 rxSlotOffset = 0, txSlotOffset = 0, beaconSlotOffset = 0, timeSyncSlotOffset=0;

#ifdef PROFILING
	taskDelay1 = call SubLocalTime.get() - taskDelay1;
#endif

	g_OMAC.UpdateNeighborTable();
	// the NextSlot() for receiver must be called every slot as there are state update
	// operations in it
	rxSlotOffset = m_DataReceptionHandler.NextSlot(m_slotNo);

	//BK:This is disabled since should not happen anyways
	// if(InputState.IsState(I_DATA_SEND_PENDING) || !(ProtoState.IsIdle())) {
		//printf("[S %u I %u\n] incorrect state\n", call State.getState(),
		//Input.GetState());
		//return FALSE;
	//}


	/* notice that the transmission handler returns the ticks (1/32 of a milli sec) for the
	 * next scheduled transmission, whereas the reception handler returns the number of slots
	 * before the next scheduled reception*/
	txSlotOffset = m_DataTransmissionHandler.NextSlot(m_slotNo);
	beaconSlotOffset = m_DiscoveryHandler.NextSlot(m_slotNo);
	timeSyncSlotOffset = m_TimeSyncHandler.NextSlot(m_slotNo);
	//	if (rxSlotOffset < 2 * SLOT_PERIOD_32KHZ
	//		|| beaconSlotOffset < 2 * SLOT_PERIOD_32KHZ || txSlotOffset < 2 * SLOT_PERIOD_32KHZ ) {
	//		call OMacSignal.yield();
	//	} else {
	//		call OMacSignal.resume();
	//	}

	///I am already scheduled to send a message this frame, let me play it safe and not do anything now
	if(startMeasuringDutyCycle && txSlotOffset < SLOT_PERIOD) {
		if(InputState.RequestState(I_DATA_SEND_PENDING) == DS_Success) {
			StartDataAlarm(txSlotOffset);
			return TRUE;
		}
	}
	/* we are not going to send any beacons if neighbor is going to receive*/
	else if(TRUE){//!isNeighborGoingToReceive()) {
		if(startMeasuringDutyCycle && rxSlotOffset == 0) {
			if(InputState.RequestState(I_DATA_RCV_PENDING) == DS_Success) {
				//call OMacSignal.yield();
				//Mukundan:Instead of posting RadioTask just run it directly
				if(!RadioTask()){
					debug_printf("RadioTask failed for sending data\n");
					return FALSE;
				}
			}
			else {
				//debug_printf("request to I_DATA_RCV_PENDING failed\n");
			}
		}
		// do not send Time Sync message, if  i'm going to receive or send packets in 2 slots
		else if((timeSyncSlotOffset == 0) && (rxSlotOffset > 2) && (txSlotOffset > 2))  {

			InputState.ForceState(I_TIMESYNC_PENDING);

			if(!RadioTask()) {
				debug_printf("RadioTask failed for timesync\n");
				return FALSE;
			}
		}
		// do not send Discovery message, if  i'm going to receive or send packets in 2 slots
		else if((beaconSlotOffset == 0) && (rxSlotOffset > 2) && (txSlotOffset > 2))  {
					//if (startMeasuringDutyCycle) {
					//	return TRUE;
					//}
					InputState.ToIdle();

					if(!RadioTask()) {
						debug_printf("RadioTask failed\n");
						return FALSE;
					}
		}

	}
	else {
		debug_printf("neighbor is receiving\n");
	}
	return TRUE;
}

//This turns on the radio and sends a message
bool OMACSchedulerBora::RadioTask(){
	DeviceStatus e = DS_Fail;
	//radioTiming = call GlobalTime.getLocalTime();
	radioTiming = HAL_Time_CurrentTime();
	//radioTiming = m_timeSync.GlobalTime();

	if(ProtoState.RequestState(S_STARTING)) {
		e = g_omac_RadioControl.Start();
	}
	else {
		hal_printf("OMACSchedulerBora::RadioTask radio start failed. state=%u\r\n", ProtoState.GetState());
		return FALSE;
	}

	if(e == DS_Ready) {
		switch(InputState.GetState()) {
			case I_DATA_SEND_PENDING :
				m_DataTransmissionHandler.ExecuteSlot(m_slotNo);
				m_lastHandler = DATA_TX_HANDLER;
				break;
			case I_DATA_RCV_PENDING :
				m_DataReceptionHandler.ExecuteSlot(m_slotNo);
				m_lastHandler = DATA_RX_HANDLER;
				break;
			case I_TIMESYNC_PENDING :
				m_TimeSyncHandler.ExecuteSlot(m_slotNo);
				m_lastHandler = TIMESYNC_HANDLER;
				break;
			default :
				m_DiscoveryHandler.ExecuteSlot(m_slotNo);
				m_lastHandler = CONTROL_BEACON_HANDLER;
				break;
		}
	}

	PostExecution();
	return TRUE;
}




//////Slot Alarm APIs
void OMACSchedulerBora::StartSlotAlarm(UINT64 Delay){
	//Start the SlotAlarm
	//HALTimer()
	if(Delay==0){
		//start alarm in default periodic mode
		//VirtTimer_SetTimer(HAL_SLOT_TIMER, 0, SLOT_PERIOD * 1000, FALSE, FALSE, PublicSlotAlarmHanlder);
		VirtTimer_Change(HAL_SLOT_TIMER, 0, SLOT_PERIOD * 1000, FALSE); //1 sec Timer in micro seconds
		VirtTimer_Start(HAL_SLOT_TIMER);

	}else {
		//Change next slot time with delay
		//VirtTimer_SetTimer(HAL_SLOT_TIMER, 0, (Delay-4)*1000, FALSE, FALSE, PublicSlotAlarmHanlder);
		VirtTimer_Change(HAL_SLOT_TIMER, 0, (Delay-4)*1000, FALSE); //1 sec Timer in micro seconds
		VirtTimer_Start(HAL_SLOT_TIMER);
	}

}

void OMACSchedulerBora::SlotAlarmHandler(void* Param){
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
	//Using a simple increament for the timebeing
	m_slotNo++;

	//nextWakeup = ((m_slotNo + 1) << SLOT_PERIOD_BITS) + m_slotNoOffset - localTime;
	//this->StartSlotAlarm(nextWakeup);


	//Mukundan: At this point, instead of posting a task as in TinyOS,
	//just run the task directly
	//We will revisit the whole task architecture later.
	this->RunSlotTask();

#ifdef OMAC_DEBUG
	if(m_slotNo % 1000 == 0) {
		hal_printf("curSlot %lu time %llu\n", m_slotNo, localTime);
	}
	CPU_GPIO_SetPinState((GPIO_PIN) 1, FALSE);
#endif
}

//////Discovery Timer APIs
void OMACSchedulerBora::StartDiscoveryTimer(UINT64 Delay){
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

void OMACSchedulerBora::DiscoveryTimerHandler(void* Param){

}
//////Data Alarm APIs
void OMACSchedulerBora::StartDataAlarm(UINT64 Delay){
	//Start the SlotAlarm
	//HALTimer()
	if(Delay==0){
		void* param;
		this->DataAlarmHandler(param);
	}else {
		VirtTimer_Change(HAL_DATAALARM_TIMER, 0, Delay*1000, FALSE);
		VirtTimer_Start(HAL_DATAALARM_TIMER);

	}

}

void OMACSchedulerBora::DataAlarmHandler(void* Param){
	UINT64 localTime, nextWakeup, oldSlotNo;
	localTime = HAL_Time_CurrentTime();
	//localTicks = HAL_Time_CurrentTicks();

	//increment counter
#ifdef PROFILING
	taskDelay1 = localTime;
#endif

	/*
	 * Compenstation for alarm drift: See Wenjies notes
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
	this->RunSlotTask();

#ifdef OMAC_DEBUG
	//if(m_slotNo % 1000 == 0) {
	//	debug_printf("curSlot %lu time %lu\n", m_slotNo, localTime);
	//}
#endif
}

bool OMACSchedulerBora::IsRunningDataAlarm(){
	return TRUE; //BK, Something should be returned
}



///////////////////////////////////////Private Functions///////////////////

void OMACSchedulerBora::Sleep(){
	ProtoState.ToIdle();
	InputState.ToIdle();
}

void OMACSchedulerBora::Stop(){
	bool e =FALSE;
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
			e= g_omac_RadioControl.Stop();
	}
#ifdef OMAC_DEBUG
	//call SlotScheduler.printState();
	//printf("stop result e=%u\n", e);
#endif
	switch(e) {
		case  TRUE: // Ignore.  AMControl.stopDone will provide continuation.
			break;
		default : // Already shut down and we just don't know it.  Sleep.
			Sleep();
	}

}

bool OMACSchedulerBora::IsNeighborGoingToReceive(){
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

void OMACSchedulerBora::PostExecution(){
	switch(m_lastHandler) {
		case CONTROL_BEACON_HANDLER :
			m_DiscoveryHandler.PostExecuteSlot();
			break;
		case TIMESYNC_HANDLER :
			m_TimeSyncHandler.PostExecuteSlot();
			break;
		case DATA_TX_HANDLER :
			//also notify the DiscoveryHandler in case
			//there is piggyback beacon received
			m_DataTransmissionHandler.PostExecuteSlot();
			m_DiscoveryHandler.PostExecuteSlot();
			break;
		case DATA_RX_HANDLER :
			m_DataReceptionHandler.PostExecuteSlot();
			m_DiscoveryHandler.PostExecuteSlot();
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

void OMACSchedulerBora::PrintDutyCycle(){

}
