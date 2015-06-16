#ifndef _NETMF_POWER_H_
#define _NETMF_POWER_H_

#include <tinyhal.h>
#include <stm32f10x.h>

#define PWR_HSI_SPEED 8000000
#define PWR_HSI_CLOSE_ENOUGH 8092
#define PWR_HSI_DEFAULT_TRIM 16
#define PWR_RTC_TIMEOUT 2000000

#define PWR_HSI_TRIM_MIN_OR_MAX(x) (x == 0 || x == 31)
#define PWR_HSI_TRIM_VALID(x) (x<=31)

volatile int go_back_to_sleep;

enum stm_power_modes {
	POWER_STATE_DEFAULT,
	POWER_STATE_LOW,
	POWER_STATE_HIGH,
};

extern UINT32 pwr_get_hsi(int x);

void Sleep();
void PowerInit();
void High_Power();
void Low_Power();
void Halt();
void Reset();
void Shutdown();
void CalibrateHSI();

#endif // #ifndef _NETMF_POWER_H_
