#ifndef _NETMF_POWER_H_
#define _NETMF_POWER_H_

#include <SF2_Main.h>

enum stm_power_modes {
	POWER_STATE_DEFAULT,
	POWER_STATE_LOW,
	POWER_STATE_MID,
	POWER_STATE_HIGH,
};

void Sleep();
void PowerInit();
void High_Power();
void Mid_Power();
void Low_Power();
void Halt() __attribute__ ((noreturn));
void Reset() __attribute__ ((noreturn));
void Shutdown() __attribute__ ((noreturn));

//uint32_t JTAG_Attached();
void PWR_EnterSTANDBYMode(void);

#endif // #ifndef _NETMF_POWER_H_
