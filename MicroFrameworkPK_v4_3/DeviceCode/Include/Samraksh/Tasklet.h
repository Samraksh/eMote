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


#if defined(SAM_USE_ATOMIC_LOCK)
#define LOCK_INIT()  taskletLock.init()
#define LOCK_ACQUIRE() while(!taskletLock.acquire_and_save())
#define LOCK_RELEASE() while(!taskletLock.release_and_restore())
#elif defined(SAM_USE_SMART_LOCK)
#define LOCK_INIT()  /*sptr_tasklet = new SmartPtr_IRQ(this);SmartPtr_IRQ::ForceEnabled(this);*/
#define LOCK_ACQUIRE() sptr_tasklet.Acquire()
#define LOCK_RELEASE() sptr_tasklet.Release()
#elif defined(SAM_USE_NO_LOCK)
#define LOCK_INIT()
#define LOCK_ACQUIRE()
#define LOCK_RELEASE()
#endif


class Tasklet
{

	// Create a queue of tasklets
	Hal_Queue_KnownSize<TaskletType*, 16> taskletQueueLowPriority;
	Hal_Queue_KnownSize<TaskletType*, 16> taskletQueueHighPriority;

#if defined(SAM_USE_ATOMIC_LOCK)
	LOCK taskletLock;
#elif defined(SAM_USE_SMART_LOCK)
	SmartPtr_IRQ sptr_tasklet;
#elif defined(SAM_USE_NO_LOCK)
#endif

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
