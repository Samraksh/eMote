/*
	Microframework 4.3 HAL driver for basic power functions on Krait.

	$(SPOCLIENT)\DeviceCode\Targets\Native\Krait\DeviceCode\Krait_POWER\Krait_POWER_functions.cpp

	Michael McGrath
	michael.mcgrath@samraksh.com
	The Samraksh Company
	2013-10-01
*/

//#include <cores\arm\include\cpu.h>
#include <tinyhal.h>
#include "..\Krait.h"
#include "Krait__POWER.h"

#include "..\Krait_PMIC\pmic_decl.h"

// Stub
BOOL CPU_JTAG_Attached(){
	return 0;
}

void HAL_AssertEx() {
    // cause an abort and let the abort handler take over
    //*((char*)0xFFFFFFFF) = 'a';
	// ... except we haven't provided the abort handler.  so just trap. except watchdog?
	while(1);
}

BOOL CPU_Initialize() {
	NATIVE_PROFILE_HAL_PROCESSOR_POWER();

        PMIC_Initialize();
        Krait_POWER_Driver::Init();

        return TRUE;
}


/**
 * Minimal support.
 */
void CPU_Reset() {
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    //FIXME: lock

	// if NetMF Config sector's bootloader flag is set, then set LK Shared IMEM fastboot flag.
	const UINT32        c_Empty     = 0xFFFFFFFF;
	const UINT32        c_Key       = ConfigurationSector::c_BootEntryKey;

	volatile UINT32*    pAddr       = (volatile UINT32 *)&g_ConfigurationSector.BooterFlagArray[0];
	bool                bDone       = false;

	BlockStorageDevice *pBlockDevice = BlockStorageList::s_primaryDevice;
	if ( pBlockDevice != NULL)
	{
		const BlockDeviceInfo* deviceInfo = pBlockDevice->GetDeviceInfo();

		ByteAddress configSectAddress;
		UINT32        iRegion, iRange;

		pBlockDevice->FindForBlockUsage(BlockUsage::CONFIG, configSectAddress, iRegion, iRange );

		for(int i=0; !bDone && i<ConfigurationSector::c_MaxBootEntryFlags; i++ )
		{
			switch(pAddr[i])
			{
				case ConfigurationSector::c_BootEntryKey:
					writel(FASTBOOT_MODE, RESTART_REASON_ADDR);
					bDone = true;
					break;
			}
		}
	}


    Krait_POWER_Driver::Reset();
    //FIXME: unlock
    HAL_AssertEx();
    return;
}

void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents )
{
	NATIVE_PROFILE_HAL_PROCESSOR_POWER();

	switch(level)
	{
		case SLEEP_LEVEL__AWAKE:
			Krait_POWER_Driver::SleepWFI();
			break;
		case SLEEP_LEVEL__SELECTIVE_OFF:

		case SLEEP_LEVEL__SLEEP:
			Krait_POWER_Driver::SleepRetention();
        case SLEEP_LEVEL__DEEP_SLEEP:
        case SLEEP_LEVEL__OFF:
        default:
            Krait_POWER_Driver::SleepWFI();
            break;
    }
}

void CPU_ChangePowerLevel(POWER_LEVEL level) {
	NATIVE_PROFILE_HAL_PROCESSOR_POWER();
	switch(level) {
		case POWER_LEVEL__HIGH_POWER:
			Krait_POWER_Driver::Highpower();
			break;
		case POWER_LEVEL__MID_POWER:
			Krait_POWER_Driver::Highpower();
			break;
		case POWER_LEVEL__LOW_POWER:
			Krait_POWER_Driver::Lowpower();
			break;
		default:
			Krait_POWER_Driver::Highpower();
			break;
	}
}

/*
Not yet supported.
*/
BOOL CPU_IsSoftRebootSupported () {
	NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    return FALSE;
}

/**
 * Unrecoverable error
*/
void CPU_Halt(){
	NATIVE_PROFILE_HAL_PROCESSOR_POWER();
	    while(1);
    return;
}
