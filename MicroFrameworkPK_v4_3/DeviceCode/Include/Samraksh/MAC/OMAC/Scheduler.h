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
#include "OMACTimeSync.h"
#include "SeedGenerator.h"

//extern HALTimerManager gHalTimerManagerObject;

#define MAXSCHEDULERUPDATE 5000000
#define DISCO_SLOT_GUARD 10

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

	HandlerType_t m_lastHandler;


public:
	bool timer1INuse, timer2INuse, SchedulerINUse;
	State_t InputState;		//stores data needed by protocol for processing
	DiscoveryHandler m_DiscoveryHandler;
	DataReceptionHandler m_DataReceptionHandler;
	DataTransmissionHandler m_DataTransmissionHandler;
	OMACTimeSync m_TimeSyncHandler;
	SeedGenerator m_seedGenerator;



	//BK The methods from this point on is in use for sure. The rest can be deleted.
	//TODO: Clean up the code by removing unused variables and methods
	//Define public methods
	void Initialize(UINT8 radioID, UINT8 macID);
	void UnInitialize();
	UINT64 GetSlotNumber();
	UINT64 GetSlotNumberfromTicks(const UINT64 &y);
	UINT32 GetSlotNumberfromMicroSec(const UINT64 &y);
	UINT32 GetTimeTillTheEndofSlot();
	void ScheduleNextEvent();


	//Main Tasks
	bool RunEventTask(); // BK: This is the main event for the slot. Queries all the modules and executes one if needed
	void PostExecution();
	void PostPostExecution();
	bool EnsureStopRadio();

};




#endif /* SCHEDULER_H_ */
