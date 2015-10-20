/*
 * Scheduler.h
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

//#include <Samraksh/Hal_util.h>
//#include <Samraksh/HALTimer.h>
#include <Samraksh/VirtualTimer.h>
#include "OMACConstants.h"
#include "Handlers.h"
#include "RadioControl.h"
#include "DiscoveryHandler.h"
#include "DataReceptionHandler.h"
#include "DataTransmissionHandler.h"
//#include "GlobalTime.h"
#include "CMaxTimeSync.h"
#include "SeedGenerator.h"

//extern HALTimerManager gHalTimerManagerObject;

/*
 *
 */
typedef class State{
  private:
	UINT8 CurrentState;
  public:
	//Changed state if in Idle
	bool RequestState(UINT8 reqState){
		//GLOBAL_LOCK(irq);

		if(CurrentState == S_IDLE){
			CurrentState=reqState;
			return TRUE;
		}
		else if(CurrentState == S_STARTING && reqState == S_STARTING) {
			return TRUE;
		}
		else return FALSE;
	}

	//Force the state machine
	void ForceState(UINT8 reqState){
		//GLOBAL_LOCK(irq);
		//hal_printf("ForceState: currentState: %d; reqState: %d\n", CurrentState, reqState);
		CurrentState = reqState;
	}

	//Set the current state back to S_IDLE
	void ToIdle(){
		//GLOBAL_LOCK(irq);
		CurrentState = S_IDLE;
	}

	//@return TRUE if the state machine is in S_IDLE
	bool IsIdle(){
		if(CurrentState == S_IDLE) return TRUE;
		else return FALSE;
	}

	//@return TRUE if the state machine is in the given state
	bool IsState(UINT8 compState){
		//GLOBAL_LOCK(irq);
		if(CurrentState == compState) return TRUE;
		else return FALSE;
	}

	//Get the current state
	UINT8 GetState(){return CurrentState;}
}State_t;

/*
 *
 */
class OMACScheduler{
private:

	UINT8 radioID;
	UINT8 macID;
	//static bool startMeasuringDutyCycle;
	//static  UINT32 dutyCycleReset;
	bool startMeasuringDutyCycle;
	UINT32 dutyCycleReset;
	UINT32	sendGT, dutyCycleOffset, syncOffset, totalRadioUp, totalDelay;
#ifdef PROFILING
	UINT32 startDelay, stopDelay;
	UINT32 taskDelay1;
	UINT32 minStartDelay, maxStartDelay;
	UINT32 minStopDelay, maxStopDelay;
#endif

	float m_radioDelayAvg;
	UINT32 radioTiming, m_lastPiggybackSlot;
	UINT8 m_discoveryTicks, m_shldPrintDuty;
	UINT8 m_nonSleepStateCnt;
	HandlerType_t m_lastHandler;
	bool m_busy;	//indicates if radio is busy.
	UINT32 m_slotNo, m_slotNoOffset;

	//Define private methods
	void Sleep();
	bool IsNeighborGoingToReceive();

	void PrintDutyCycle();

public:
	bool timer1INuse, timer2INuse;
	State_t ProtoState;		//stores state of the protocol
	State_t InputState;		//stores data needed by protocol for processing
	DiscoveryHandler m_DiscoveryHandler;
	DataReceptionHandler m_DataReceptionHandler;
	DataTransmissionHandler m_DataTransmissionHandler;
	CMaxTimeSync_t m_TimeSyncHandler;
	SeedGenerator m_seedGenerator;


	UINT32 GetCounterOffset(){
		return m_slotNoOffset;
	}
	UINT16 GetRadioDelay(){
		return (UINT16)m_radioDelayAvg;
	}

	//BK The methods from this point on is in use for sure. The rest can be deleted.
	//TODO: Clean up the code by removing unused variables and methods
	//Define public methods
	void Initialize(UINT8 radioID, UINT8 macID);
	void UnInitialize();
	UINT32 GetSlotNumber();
	UINT32 GetSlotNumberfromTicks(const UINT64 &y);
	UINT32 GetSlotNumberfromMicroSec(const UINT64 &y);
	UINT32 GetTimeTillTheEndofSlot();
	void ScheduleNextEvent();


	//Main Tasks
	bool RunEventTask(); // BK: This is the main event for the slot. Queries all the modules and executes one if needed
	void PostExecution();
	bool EnsureStopRadio();





}OMACScheduler_t;




#endif /* SCHEDULER_H_ */
