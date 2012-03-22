/*
	Microframework 4.0 HAL driver for basic power functions.
	For now just sleep etc.


	Nathan Stohs
	nathan.stohs@samraksh.com
	The Samraksh Company
	2012-03-20
*/

#include <cores\arm\include\cpu.h>
#include "..\TIAM3517.h"
#include <power_decl.h>


/*
All clocks setup earlier in bootstrap (X-loader)
Nothing here for now. May change later.
*/
BOOL CPU_Initialize() {
	return TRUE;
}

/*
Nothing here at the moment.
Potentially change clocks.
Otherwise hardware is handled dynamically.
*/
void CPU_ChangePowerLevel(POWER_LEVEL level) {
	return;
}

/*
Only sleep for now
*/
void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents )
{
    switch(level)
    {
        case SLEEP_LEVEL__DEEP_SLEEP:
            return;
            break;
        case SLEEP_LEVEL__OFF:
            return;
            break;
        case SLEEP_LEVEL__SLEEP:
        default:
            TIAM3517_POWER_Driver::Sleep();
            break;
    }
}

/*
Not yet supported.
*/
void CPU_Halt(){
    return;
}

/*
Not yet supported.
*/
void CPU_Reset() {
    return;
}

/*
Not yet supported.
*/
BOOL CPU_IsSoftRebootSupported () {
    return FALSE;
}

