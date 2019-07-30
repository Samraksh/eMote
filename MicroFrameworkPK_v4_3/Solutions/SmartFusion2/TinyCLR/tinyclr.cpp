////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>
#include <cmsis_gcc.h>
#include <Samraksh/os_scheduler.h>
#include <Samraksh/sm.h>

void UserThreadStop(){
	while(1){
	    	debug_printf( "UserThreadStop: User thread finished... Nothing to do. \r\n");
	 }
}


void CLRApplication(){
    CLR_SETTINGS clrSettings;
    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;

    ClrStartup( clrSettings );

#if !defined(BUILD_RTM)
    debug_printf( "Exiting.\r\n" );
#else
    ::CPU_Reset();
#endif
}


void SwitchToUserStack(volatile os_task_t *os_curr_task){
	//ARm implements a full descending stack. That is it points to the last stack frame.
	//so for the very first user frame, it actually points outside of the stack

	//__set_PSP(os_curr_task->sp+64); // Set PSP to the top of task's stack

	__ISB(); // instruction barrier. Make sure everything till now is done
	__set_PSP(os_curr_task->sp+64); // Set PSP to the top of task's stack
	//__set_CONTROL(0x03); // Switch to PSP, unprivilleged mode
	__ASM volatile ("MSR control, %0" : : "r" (3) : "memory");
}

void ApplicationEntryPoint()
{
	//Setup the function that should run as user task
	volatile os_task_t* userTask =SetupUserTask((HandlerFuncPtr)CLRApplication);
	ASSERT(userTask);

	//Everything is setup; switch to user stack
	SwitchToUserStack(userTask);

	//now execute the user task
	userTask->handler();
}


BOOL Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                        OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}

