#ifndef _NETMF_POWER_H_
#define _NETMF_POWER_H_

#include <tinyhal.h>
#include <stm32f10x.h>

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

#endif // #ifndef _NETMF_POWER_H_
