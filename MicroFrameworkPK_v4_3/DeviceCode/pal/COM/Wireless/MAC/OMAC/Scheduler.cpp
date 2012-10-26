/*
 * Scheduler.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan
 */

#include "Scheduler.h"
extern RadioControl_t g_omac_RadioControl;

void OMACScheduler::Initialize(){
	//Initialize variables
	startMeasuringDutyCycle=FALSE;
	dutyCycleReset = 0;
	totalRadioUp = 0;


#ifdef PROFILING
	minStartDelay = 300; maxStartDelay = 10;
	minStopDelay = 300; maxStopDelay = 10;
#endif

	ProtoState.ToIdle();
	InputState.ToIdle();

	m_heartBeats = 0, m_shldPrintDuty = 0;
	m_counter = 0;
	m_nonSleepStateCnt = 0;
	m_lastHandler = NULL_HANDLER;
	m_lastPiggybackSlot = 0;
	m_radioDelayAvg = 0;
	radioTiming = 0;
	m_busy = FALSE;


	//Initialize various processes
	this->StartSlotAlarm(SLOT_PERIOD);
	this->StartHeartBeatTimer(1000*TICKS_PER_MILLI);

	//Initialize Handlers
	 m_DiscoveryTimesyncHandler.SetParentSchedulerPtr(this);

}
void OMACScheduler::UnInitialize(){

}

bool OMACScheduler::RunSlotTask(){
	UINT16 rxSlotOffset = 0, txSlotOffset = 0, beaconSlotOffset = 0;

#ifdef PROFILING
	taskDelay1 = call SubLocalTime.get() - taskDelay1;
#endif

	// the newSlot() for receiver must be called every slot as there are state update
	// operations in it
	rxSlotOffset = m_DataReceptionHandler.NewSlot(m_counter);

	if(InputState.IsState(I_DATA_PENDING) || !(ProtoState.IsIdle())) {
		//printf("[S %u I %u\n] incorrect state\n", call State.getState(),
		//Input.GetState());
		return FALSE;
	}


	/* notice that the transmission handler returns the ticks (1/32 of a milli sec) for the
	 * next scheduled transmission, whereas the reception handler returns the number of slots
	 * before the next scheduled reception*/
	txSlotOffset = m_DataTransmissionHandler.NewSlot(m_counter);
	beaconSlotOffset = m_DiscoveryTimesyncHandler.NewSlot(m_counter);
	//	if (rxSlotOffset < 2 * SLOT_PERIOD_32KHZ
	//		|| beaconSlotOffset < 2 * SLOT_PERIOD_32KHZ || txSlotOffset < 2 * SLOT_PERIOD_32KHZ ) {
	//		call OMacSignal.yield();
	//	} else {
	//		call OMacSignal.resume();
	//	}

	if(startMeasuringDutyCycle && txSlotOffset < SLOT_PERIOD) {
		if( !IsRunningDataAlarm()) {
			StartDataAlarm(txSlotOffset);
		} else {
			//debug_printf("DataAlarm already running with remaining %lu\n", call DataAlarm.getAlarm() - call DataAlarm.getNow());
		}
	}
	/* we are not going to send any beacons if neighbor is going to receive*/
	else if(TRUE){//!isNeighborGoingToReceive()) {
		if(startMeasuringDutyCycle && rxSlotOffset == 0) {
			if(InputState.RequestState(I_WAITING_DATA) == DS_Success) {
				//call OMacSignal.yield();
				//Mukundan:Instead of posting RadioTask just run it directly
				if(!RadioTask()){
					debug_printf("RadioTask failed\n");
					return FALSE;
				}
			}
			else {
				debug_printf("request to I_WAITING_DATA failed\n");
			}
		}
		/* do not send control beacon if
		 * i) i'm already synced and
		 * ii) i'm going to receive or send packets in 2 slots */
		else if(beaconSlotOffset == 0) {
		// && ((rxSlotOffset > 2
		//	* SLOT_PERIOD_32KHZ && txSlotOffset > 2 * SLOT_PERIOD_32KHZ))) {
			if (startMeasuringDutyCycle) {
				return TRUE;
			}
			InputState.ToIdle();
			//call OMacSignal.yield();
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
bool OMACScheduler::RadioTask(){
	DeviceStatus e = DS_Fail;
	//radioTiming = call GlobalTime.getLocalTime();
	radioTiming = Time_GetLocalTime();

	if(ProtoState.RequestState(S_STARTING)) {
		e = g_omac_RadioControl.Start();
	}
	else {
		debug_printf("radio start failed. state=%u\n", ProtoState.GetState());
		return FALSE;
	}

	if(e == DS_Already) {
		switch(InputState.GetState()) {
			case I_DATA_PENDING :
				m_DataTransmissionHandler.ExecuteSlot(m_counter);
				m_lastHandler = DATA_TX_HANDLER;
				break;
			case I_WAITING_DATA :
				m_DataReceptionHandler.ExecuteSlot(m_counter);
				m_lastHandler = DATA_RX_HANDLER;
				break;
			default :
				m_DiscoveryTimesyncHandler.ExecuteSlot(m_counter);
				m_lastHandler = CONTROL_BEACON_HANDLER;
				break;
		}
	}


	return TRUE;
}




//////Slot Alarm APIs
void OMACScheduler::StartSlotAlarm(UINT64 Delay){
	//Start the SlotAlarm
	//HALTimer()
	if(Delay==0){
		//start alarm in default periodic mode
		//HALTimer :: Initialize (, TRUE, 0, 0, m_OMACScheduler.SlotAlarm, NULL)
	}else {
		//Change next slot time with delay
		//HALTimer :: Initialize (, TRUE, 0, 0, m_OMACScheduler.SlotAlarm, NULL)
	}

}

void OMACScheduler::SlotAlarmHandler(void* Param){
	UINT64 localTime, nextWakeup, oldCounter;
	localTime = Time_GetLocalTime();
	//increment counter
#ifdef PROFILING
	taskDelay1 = localTime;
#endif

	if (m_counter == 0) {
		m_counterOffset = localTime - (1 << SLOT_PERIOD_BITS);
		m_counter = 1;
	}
	else {
		oldCounter = m_counter;
		//have to compensate for numerical errors
		m_counter = (localTime - m_counterOffset) >> SLOT_PERIOD_BITS;
		//INVARIANT: localTime = m_counter << SLOT_PERIOD_BITS + m_counterOffset
		if (m_counter <= oldCounter) {
			m_counter = oldCounter + 1;
		}
	}
	nextWakeup = ((m_counter + 1) << SLOT_PERIOD_BITS) + m_counterOffset - localTime;
	this->StartSlotAlarm(nextWakeup);

	//Mukundan: At this point, instead of posting a task as in TinyOS,
	//just run the task directly
	//We will revisit the whole task architecture later.
	this->RunSlotTask();


	if(m_counter % 1000 == 0) {
		debug_printf("curSlot %lu time %lu\n", m_counter, localTime);
	}
}

//////HeartBeat Timer APIs
void OMACScheduler::StartHeartBeatTimer(UINT64 Delay){
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

void OMACScheduler::HeartBeatTimerHandler(void* Param){

}
//////Data Alarm APIs
void OMACScheduler::StartDataAlarm(UINT64 Delay){
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

void OMACScheduler::DataAlarmHandler(void* Param){
	UINT64 localTime, nextWakeup, oldCounter;
	localTime = Time_GetLocalTime();
	//increment counter
#ifdef PROFILING
	taskDelay1 = localTime;
#endif

	if (m_counter == 0) {
		m_counterOffset = localTime - (1 << SLOT_PERIOD_BITS);
		m_counter = 1;
	}
	else {
		oldCounter = m_counter;
		//have to compensate for numerical errors
		m_counter = (localTime - m_counterOffset) >> SLOT_PERIOD_BITS;
		//INVARIANT: localTime = m_counter << SLOT_PERIOD_BITS + m_counterOffset
		if (m_counter <= oldCounter) {
			m_counter = oldCounter + 1;
		}
	}
	nextWakeup = ((m_counter + 1) << SLOT_PERIOD_BITS) + m_counterOffset - localTime;
	this->StartSlotAlarm(nextWakeup);

	//Mukundan: At this point, instead of posting a task as in TinyOS,
	//just run the task directly
	//We will revisit the whole task architecture later.
	this->RunSlotTask();


	if(m_counter % 1000 == 0) {
		debug_printf("curSlot %lu time %lu\n", m_counter, localTime);
	}
}

bool OMACScheduler::IsRunningDataAlarm(){

}



///////////////////////////////////////Private Functions///////////////////

void OMACScheduler::Sleep(){
	ProtoState.ToIdle();
	InputState.ToIdle();
}

void OMACScheduler::Stop(){
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
		default :
			ProtoState.ForceState(S_STOPPING);
			//e = call RadioControl.stop();
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

void OMACScheduler::PostExecution(){
	switch(m_lastHandler) {
		case CONTROL_BEACON_HANDLER :
			m_DiscoveryTimesyncHandler.PostExecuteSlot();
			break;
		case DATA_TX_HANDLER :
			//also notify the DiscoveryTimesyncHandler in case
			//there is piggyback beacon received
			m_DataTransmissionHandler.PostExecuteSlot();
			m_DiscoveryTimesyncHandler.PostExecuteSlot();
			break;
		case DATA_RX_HANDLER :
			m_DataReceptionHandler.PostExecuteSlot();
			m_DiscoveryTimesyncHandler.PostExecuteSlot();
			break;
		default :
			break;
	}
#ifdef PROFILING
	debug_printf("delay1=%lu,startDelay=%lu,stopDelay=%lu\n", taskDelay1, startDelay, stopDelay);
	//printf("minStart=%lu,maxStart=%lu,minStop=%lu,maxStop=%lu\n", minStartDelay,
	//		maxStartDelay, minStopDelay, maxStopDelay);
	debug_printf("powerDelay=%lu, oscDelay=%lu, taskDelay=%lu\n", RadioDutyCycle.dbg1(), RadioDutyCycle.dbg2(), RadioDutyCycle.dbg3());
#endif
	//call OMacSignal.resume();

}

void OMACScheduler::PrintDutyCycle(){

}
