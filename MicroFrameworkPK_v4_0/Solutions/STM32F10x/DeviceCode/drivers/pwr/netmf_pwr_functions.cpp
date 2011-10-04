#include <cores\arm\include\cpu.h>
#include <netmf_pwr.h>

BOOL CPU_Initialize()
{
    return Power_Driver::Initialize();
}

//--//

//#pragma arm section code


void CPU_ChangePowerLevel(POWER_LEVEL level)
{
	//The CPU continues to run in RUN mode but the clocks are slowed down
    switch(level)
    {
        case POWER_LEVEL__MID_POWER:
            break;

        case POWER_LEVEL__LOW_POWER:
            break;

        case POWER_LEVEL__HIGH_POWER:
        default:
            break;
    }
}

void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents )
{
	//The CPU is made to enter the sleep mode
    switch(level)
    {
        case SLEEP_LEVEL__DEEP_SLEEP:
            Power_Driver::Hibernate();
            break;
        case SLEEP_LEVEL__OFF:
            Power_Driver::Shutdown();
            break;
        case SLEEP_LEVEL__SLEEP:
        default:
            Power_Driver::Sleep();
            break;
    }
}

void CPU_Halt()
{
    Power_Driver::Halt();
}

void CPU_Reset()
{
    Power_Driver::Reset();
}

/*
Indicates whether a soft reboot is supported by the underlying hardware. 
*/
BOOL CPU_IsSoftRebootSupported ()
{
    return TRUE;
}

