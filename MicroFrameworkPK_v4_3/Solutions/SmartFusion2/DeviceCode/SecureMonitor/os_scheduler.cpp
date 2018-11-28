#include <Samraksh/os_scheduler.h>
#include <string.h>
#include <cmsis_gcc.h>


static struct OS_Task_Table m_task_table;
volatile os_task_t *os_curr_task;
volatile os_task_t *os_next_task;

static os_stack_t* userStack;

static void task_finished(void)
{
	// This function is called when some task handler returns.
	volatile UINT32 i = 0;
	while (1)
		i++;
}

void os_init(void)
{
	memset(&m_task_table, 0, sizeof(m_task_table));
}

BOOL os_task_init(HandlerFuncPtr fptr, os_stack_t *p_stack, UINT32 stack_size)
{
	if (m_task_table.size > MAX_OS_TASKS-1)
		return FALSE;

	// Initialize the task structure and set SP to the top of the stack
	//minus 16 words (64 bytes) to leave space for storing 16 registers:
	os_task_t *p_task = &m_task_table.tasks[m_task_table.size];
	p_task->handler = fptr;
	p_task->sp = ((UINT32)p_stack+stack_size-16);
	p_task->status = OS_TASK_STATUS_IDLE;

	// Save special registers which will be restored on exc. return:
	//   - XPSR: Default value (0x01000000)
	//   - PC: Point to the handler function
	//   - LR: Point to a function to be called when the handler returns
	p_stack[stack_size-1] = 0x01000000;
	p_stack[stack_size-2] = (UINT32)fptr;
	p_stack[stack_size-3] = (UINT32) &task_finished;

#ifdef OS_CONFIG_DEBUG
	UINT32 base = (m_task_table.size+1)*1000;
	p_stack[stack_size-4] = base+12;  // R12
	p_stack[stack_size-5] = base+3;   // R3
	p_stack[stack_size-6] = base+2;   // R2
	p_stack[stack_size-7] = base+1;   // R1
	p_stack[stack_size-8] = base+0;   // R0
	p_stack[stack_size-9] = base+7;   // R7
	p_stack[stack_size-10] = base+6;  // R6
	p_stack[stack_size-11] = base+5;  // R5
	p_stack[stack_size-12] = base+4;  // R4
	p_stack[stack_size-13] = base+11; // R11
	p_stack[stack_size-14] = base+10; // R10
	p_stack[stack_size-15] = base+9;  // R9
	p_stack[stack_size-16] = base+8;  // R8
#endif

	m_task_table.size++;

	return TRUE;
}
/*
BOOL SetupUserStack(volatile os_task_t *os_curr_task){
	//ARm implements a full descending stack. That is it points to the last stack frame.
	//so for the very first user frame, it actually points outside of the stack

	//__set_PSP(os_curr_task->sp+64); // Set PSP to the top of task's stack

	__ISB(); // Exec. ISB after changing CONTORL (recommended)
	__set_PSP(os_curr_task->sp+64); // Set PSP to the top of task's stack
	__set_CONTROL(0x03); // Switch to PSP, unprivilleged mode

}
*/

volatile os_task_t* SetupUserTask(HandlerFuncPtr fptr)
{
	//NVIC_SetPriority(PendSV_IRQn, 0xff); // Lowest possible priority
	//NVIC_SetPriority(SysTick_IRQn, 0x00); // Highest possible priority

	// Start the SysTick timer:
	//UINT32 ret_val = SysTick_Config(systick_ticks);
	//if (ret_val != 0)
		//return FALSE;

	os_init();
	userStack=(os_stack_t*)SAM_USER_STACK_TOP;
	m_task_table.current_task=0;
	os_task_init(fptr,userStack, (SAM_USER_STACK_BOTTOM-SAM_USER_STACK_TOP));

	UINT32 mode = GetExecMode();
	if(mode < 3){
		debug_printf( "ApplicationEntryPoint: In Priviledged Mode %d... Setting up User Stack.\r\n",mode );
		os_curr_task = &m_task_table.tasks[m_task_table.current_task];
		//SetupUserStack(os_curr_task);
	}

	// Start the first task:
	//os_curr_task->handler();
	return os_curr_task;
}

/*
void SysTick_Handler(void)
{
	os_curr_task = &m_task_table.tasks[m_task_table.current_task];
	os_curr_task->status = OS_TASK_STATUS_IDLE;

	// Select next task:
	m_task_table.current_task++;
	if (m_task_table.current_task >= m_task_table.size)
		m_task_table.current_task = 0;

	os_next_task = &m_task_table.tasks[m_task_table.current_task];
	os_next_task->status = OS_TASK_STATUS_ACTIVE;

	// Trigger PendSV which performs the actual context switch:
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}
*/


//returns the exec mode value in mode.
// 1 is priv_interrupt, 2 is priv_thread, 3 is unpriv_thread
UINT32 GetExecMode(){
	UINT32 mode=0;
	if (__get_IPSR())
	{
		mode=1;
	}
	else if( !(__get_CONTROL() & 0x1)){
		mode=2;
	}
	else {
		mode=3;
	}
	return mode;
}

