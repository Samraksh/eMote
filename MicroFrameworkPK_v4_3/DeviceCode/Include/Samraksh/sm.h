#ifndef _SECURE_MONITOR_H_
#define _SECURE_MONITOR_H_

#include <tinyhal_types.h>

/*based on examples at
https://github.com/cvra/arm-cortex-mpu/blob/master/mpu.h
*/

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    GP_RAM=0,
    GP_CODE=1,
    GP_IO=2,
    Kernel_RAM=3,
    Kernel_CODE=4,
    RoT_RAM=5,
    RoT_CODE=6,
	Reserve=7
} MemRegions_t;

void SetupSecureEmoteRegions();
void MemManage_Handler(void);
void SecureMonitor_Initialize();


MemRegions_t SecureMonitor_FindFaultRegion(UINT32 fault_addr);

/*
void CPU_mpu_init(void);

void CPU_mpu_enable(void);

void CPU_mpu_disable(void);

void CPU_mpu_configure_region(UINT8 region, void *addr, UINT8 log2_len,
                          mem_access_permission_t ap, bool executable);

*/


#ifdef __cplusplus
}
#endif

#endif //_SECURE_MONITOR_H_
