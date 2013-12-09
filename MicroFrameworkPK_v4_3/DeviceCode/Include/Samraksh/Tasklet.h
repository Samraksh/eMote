#ifndef _TASKLET_H_
#define _TASKLET_H_

/*
 *    Name  :		Tasklet.h
 *
 *    Author :      nived.sivadas
 *
 *    Description : implements delayed processing
 *
 */

#include <tinyhal.h>
#include <Samraksh/Hal_util.h>


//typedef void (*TaskletFuncType)(void* data);

class TaskletType
{

public:
	// Action to be performed at a deferred time
	HAL_CALLBACK_FPN action;

	// Data to be passed to the function
	void *data;
};


class Tasklet
{

	// Create a queue of tasklets
	Hal_Queue_KnownSize<TaskletType*, 16> taskletQueueLowPriority;
	Hal_Queue_KnownSize<TaskletType*, 16> taskletQueueHighPriority;

	LOCK taskletLock;

	TaskletType taskletCache;

	BOOL initialized;


public:
	Tasklet()
	{
		taskletCache.action =  NULL;
		taskletCache.data = NULL;
		initialized = FALSE;
	}

	// Initialize the tasklet
	BOOL initialize();



	BOOL schedule(TaskletType* tasklet);

	// This function schedules a tasklet in the high priority queue
	// This queue should be reserved only for time critical drivers and no one else
	// The idea is to provide a simple mechanism to avoid time critical bottom halves to
	// be behind a slower device
	BOOL schedule_hi(TaskletType* tasklet);

	BOOL enable();

	BOOL disable();

	inline BOOL runhigh();

	BOOL runlow();

	// throw a software interrupt
	BOOL causeirq();

};

// PAL functions
BOOL Tasklet_Initialize();
BOOL Tasklet_Schedule(TaskletType* tasklet);
BOOL Tasklet_Schedule_hi(TaskletType* tasklet);
BOOL Tasklet_Run_Hi();
BOOL Tasklet_Run();

#endif
