/*
 * 	Name : VirtualTimer.h
 *
 *  Author : nived.sivadas@samraksh.com
 *
 *  Description : Contains a structure that defines the virtual timer and the manager class that is responsible for
 *  			  interfacing the virtual timer with the hardware timer
 *
 */

#ifndef _VIRTUAL_TIMER_H_
#define _VIRTUAL_TIMER_H_

#include <tinyhal.h>

// Defines the number of virtual timers in the system. Increasing this number is fine if you have a large amount of RAM
#define NUM_VIRTUAL_TIMERS 32

// Defines the signature of the timer callback function
typedef void (*TIMER_CALLBACK_FPN)( void* arg );

// This structure defines the structure of the virtual timer
class VirtualTimer
{
	// The Id referenced by the use of the timer
	UINT32 timerId;

	// The expected start time of the timer
	UINT32 startTime;

	// What is the delta after which the timer should fire
	UINT32 dTime;

	// Is the timer oneshot ?? or repetitive ??
	BOOL   isOneShot;

	// Is the timer running, set this to false if you want to invalidate the timer
	BOOL   isRunning;

	// Reserve the timer
	BOOL   _reserved;

	// A pointer to the function that will be called on an interrupt
	TIMER_CALLBACK_FPN callBack;
};

class VirtualTimerManager
{
	// An array of virtual timers, need to use a data structure to optimise
	VirtualTimer m_timer[NUM_VIRTUAL_TIMERS];

	// Maintains mapping of the id reference by the user and the internal id of the timer
	UINT32 m_timer_id_map[NUM_VIRTUAL_TIMERS];

	// Maintains the currentTimer id, this can be used to make sure more than NUM_VIRTUAL_TIMERS are not created
	static int currentTimer;

	// Possible that there are timers that have been stopped and can be reclaimed by the manager to serve other requests
	BOOL reclaimTimer();

public:

	// This function initializes the hardware timer
	BOOL Initialize();

	// This function is used to create a virtual timer
	BOOL createTimer(UINT32 timerId, UINT32 startTime, UINT32 dTime, BOOL isOneShot, BOOL _reserved,TIMER_CALLBACK_FPN callBack);

	// This function is used to stop a timer that has been created
	BOOL stopTimer(UINT32 timerId);

	// De initializes the timer manager
	BOOL DeInitialize();

};

VirtualTimerManager gVirtualTimerManagerObject;


#endif
