#ifndef _OS_SCHEDULER_H_
#define _OS_SCHEDULER_H_
#include <tinyhal_types.h>
#include <stdint.h>


//Based on code at https://github.com/adamheinrich/os.h/tree/blog_2016_07/src
//Task and process means the same

typedef UINT32 os_stack_t;
typedef void (*HandlerFuncPtr)(void);


//The code can be extended quickly to add many user space process, each with their own stack
#define MAX_OS_TASKS 1

typedef enum {
	OS_TASK_STATUS_IDLE = 1,
	OS_TASK_STATUS_ACTIVE
} os_task_status_t;

typedef struct {
	/* The stack pointer (sp) has to be the first element as it is located
	   at the same address as the structure itself (which makes it possible
	   to locate it safely from assembly implementation of PendSV_Handler).
	   The compiler might add padding between other structure elements. */
	volatile UINT32 sp;
	void (*handler)(void);
	volatile os_task_status_t status;
} os_task_t;


struct OS_Task_Table{
	os_task_t tasks[MAX_OS_TASKS];
	volatile UINT32 current_task;
	UINT32 size;
};



void os_init(void);
BOOL os_task_init(HandlerFuncPtr fptr, os_stack_t *p_stack, UINT32 stack_size);
//BOOL os_start(UINT32 systick_ticks);
BOOL StartUserTask(HandlerFuncPtr fptr);




UINT32 GetExecMode();

#endif //_OS_SCHEDULER_H_
