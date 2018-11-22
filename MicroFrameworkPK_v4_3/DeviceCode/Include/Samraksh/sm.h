#ifndef _SECURE_MONITOR_H_
#define _SECURE_MONITOR_H_

#include <tinyhal_types.h>
#include <Samraksh/cm_mpu.h>

/*based on examples at
https://github.com/cvra/arm-cortex-mpu/blob/master/mpu.h
*/

#ifdef __cplusplus
extern "C" {
#endif

//higher number, higher previledge
typedef enum {
	INVALID=-1,
    GP_RAM=0,
    GP_CODE=1,
	//GP_CODE_M=2,
	CPU_PPB=2,
	GP_IO=3,
    Kernel_RAM=4,
    Kernel_CODE=5,
    RoT_RAM=6,
    RoT_CODE=7
} SM_MemNames;

void SetupSecureEmoteRegions();
void MemManage_Handler(void);
void SecureMonitor_Initialize();
void SVCall_HandlerC(UINT32 sp);

SM_MemNames SecureMonitor_FindFaultRegion(UINT32 fault_addr);

//UINT32 GetExecMode();
//void ChangeExecMode(bool priv);
void SetupUserStack();
void SwitchToUserMode();

//we need to decrease the optimization so the the compiler
//does not ignore func and args
void __attribute__((optimize("1")))  kernel_call0(void (*func)(void*), void* arg0);
void __attribute__((optimize("1")))  kernel_call1(void (*func)(void*), void* arg0, void* arg1 );

static UINT32 __attribute__(( always_inline )) __get_LR(void);



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
