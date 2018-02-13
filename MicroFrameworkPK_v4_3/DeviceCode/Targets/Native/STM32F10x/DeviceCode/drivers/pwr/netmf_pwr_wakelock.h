#ifndef _NETMF_POWER_WAKELOCKS_H_
#define _NETMF_POWER_WAKELOCKS_H_

#define WAKELOCK_USART 0x00000001

extern void WakeLock(uint32_t lock);
extern void WakeUnlock(uint32_t lock);
extern void WakeUntil(UINT64 until);
extern bool GetWakeLocked(void);
extern void WakeLockInit(void);

#endif // #ifndef _NETMF_POWER_H_
