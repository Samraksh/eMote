#include "sm.h"

#include <Samraksh/cm_mpu.h>
#include <cmsis/m2sxxx.h>
#include <DeviceCode/stmlib/core_cm3.h>
#include <DeviceCode/stmlib/exc_return.h>
#include <mpu/cmx_ctx.h>
#include "SF2_CM3.h"
//#include <string.h>
#include <tinyhal.h>

void SecureMonitor_Initialize(){
	SetupSecureEmoteRegions();
}
void SetupSecureEmoteRegions(){

	//Set up Memory layout

	//Ram regions
	//Region 0: on RAM, System Stack, Runtime heap, Ram,
	//Set entire ram as this region, other specific regions else can be set on top of this
	void *ram_base = (void *)ERAM_ORIGIN;
	CPU_mpu_configure_region(0, ram_base, ERAM_SIZE_POWER, AP_RW_RW, false);

	//Region 1: Setup entire Flash as this region.



	//Flash regions

	// NULL pointer protection, highest priority.
	CPU_mpu_configure_region(7, NULL, 5, AP_NO_NO, false);

	/* Mark RAM as non executable. Using lowest priority means explicitely
	 * allowing a RAM region to be executable is possible.
	 *
	 * We can use a fixed address range for RAM because it is a standard define
	 * by ARM. */



	  // Disable code execution from RAM. */
	CPU_mpu_configure_region(6, // Region number.
						 (void*)0x20000000, // Base address
						 20, // 1MB (2^20)
						 AP_RW_RW,
						 false); // Non executable.
}


void MPU_Init(){

}


void MemManage_Handler(UINT32 lr, UINT32 msp)
{
    static char msg[128];
    struct port_extctx ctx;

    UINT32 MMFSR;

    /* Setup default error message. */
    memcpy((void*)msg, (void*)__FUNCTION__, 128);

    // Get context info

    // Determine the origin of the exception.
    bool from_psp = EXC_FROM_PSP(lr);
    UINT32 sp = from_psp ? __get_PSP() : msp;
    memcpy(&ctx, (void*)sp, sizeof(struct port_extctx));

    /* Get Memory Managment fault adress register */
    MMFSR = SCB->CFSR & SCB_CFSR_MEMFAULTSR_Msk;

    /* Data access violation */
    if (MMFSR & (1 << 1)) {
        debug_printf(msg, sizeof(msg),
                 "Invalid access to %p (pc=%p)", (void *)SCB->MMFAR, ctx.pc);
    }

    /* Instruction address violation. */
    if (MMFSR & (1 << 0)) {
        debug_printf(msg, sizeof(msg),
                 "Jumped to XN region %p (lr_thd=%p)",
                    (void *)SCB->MMFAR, ctx.lr_thd);
    }

    //chSysHalt(msg);
}
