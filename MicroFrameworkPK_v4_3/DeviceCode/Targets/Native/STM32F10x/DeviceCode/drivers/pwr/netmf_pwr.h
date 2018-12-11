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

#ifdef POWER_PROFILE_HACK
typedef enum __attribute__ ((packed)) {
	GOING_TO_SLEEP,
	WAKEUP,
	TOO_SHORT,
	WAKELOCK_ON,
	WAKELOCK_OFF,
	WAKELOCK_DENY_START,
	WAKELOCK_DENY_END,
	TIMEWARP,
	SNOOZE,
	BUSY_ISR,
	BUSY_USART,
	MANAGED_ENTER,
	MANAGED_EXIT,
	GC_START,
	GC_END,
	GC_RELOC_START,
	GC_RELOC_END,
	EXE_ST_GO,
	EXE_ST_END,
	EXE_DBG_GO,
	EXE_DBG_END,
	EXE_WA_GO,
	EXE_WA_END,
	E_WFE_S,
	E_WFE_E,
	EVENT_CONT_S,
	EVENT_CONT_E,
	EVENT_WFI_S,
	EVENT_WFI_E,
	EVENT_PROBE_E,
	COMP_VT_QUEUE,
	COMP_WAITIME,
	COMP_RESETSET,
} power_event_enum_t;
extern void power_event_add_now(power_event_enum_t evt, uint16_t data16, int8_t extra);
#endif

enum stm_power_modes {
	POWER_STATE_DEFAULT,
	POWER_STATE_LOW,
	POWER_STATE_MID,
	POWER_STATE_HIGH,
	POWER_STATE_GOING_SLEEP,
};

extern UINT32 pwr_get_hsi(int x);

void Sleep();
void Snooze();
void PowerInit();
void High_Power();
void Mid_Power();
void Low_Power();
void Halt() __attribute__ ((noreturn));
void Reset() __attribute__ ((noreturn));
void Shutdown() __attribute__ ((noreturn));
void CalibrateHSI();

UINT8 Get_CurrentPower();

#endif // #ifndef _NETMF_POWER_H_
