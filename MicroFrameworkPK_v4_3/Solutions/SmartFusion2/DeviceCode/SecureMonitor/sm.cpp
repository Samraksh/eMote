#include "../../../../DeviceCode/Include/Samraksh/sm.h"

#include <Samraksh/cm_mpu.h>
#include <cmsis/m2sxxx.h>
#include <core_cm3.h>
//#include <DeviceCode/stmlib/exc_return.h>
#include <mpu/cmx_ctx.h>
#include "SF2_CM3.h"
//#include <string.h>
#include <tinyhal.h>


extern UINT32 Load$$RoT_ER_FLASH$$Base;
extern UINT32 Load$$Kernel_ER_FLASH$$Base;
extern UINT32 Load$$RunTime_ER_FLASH$$Base;

extern UINT32 Load$$RoT_ER_FLASH$$Length;
extern UINT32 Load$$Kernel_ER_FLASH$$Length;
extern UINT32 Load$$RunTime_ER_FLASH$$Length;

extern UINT32 Image$$RoT_Total_RAM_RW$$Length;
extern UINT32 Image$$Kernel_Total_RAM_RW$$Length;

// these define the region to zero initialize
extern UINT32 Image$$RoT_ER_RAM_RW$$ZI$$Base;
extern UINT32 Image$$RoT_ER_RAM_RW$$ZI$$Length;
extern UINT32 Image$$Kernel_ER_RAM_RW$$ZI$$Base;
extern UINT32 Image$$Kernel_ER_RAM_RW$$ZI$$Length;
extern UINT32 Image$$RunTime_ER_RAM_RW$$ZI$$Base;
extern UINT32 Image$$RunTime_ER_RAM_RW$$ZI$$Length;

// here is the execution address/length of data to move from FLASH to RAM
extern UINT32 Image$$RoT_ER_RAM_RW$$Base;
extern UINT32 Image$$RoT_ER_RAM_RW$$Length;
extern UINT32 Image$$Kernel_ER_RAM_RW$$Base;
extern UINT32 Image$$Kernel_ER_RAM_RW$$Length;
extern UINT32 Image$$RunTime_ER_RAM_RW$$Base;
extern UINT32 Image$$RunTime_ER_RAM_RW$$Length;

extern UINT32 Image$$RoT_ER_FLASH$$Base;
extern UINT32 Image$$RoT_ER_FLASH$$Length;
extern UINT32 Image$$Kernel_ER_FLASH$$Base;
extern UINT32 Image$$Kernel_ER_FLASH$$Length;

void SecureMonitor_Initialize(){
	CPU_mpu_init();
	SetupSecureEmoteRegions();
}




void SetupSecureEmoteRegions(){

	//Set up Memory layout

	//Ram regions
	//Region 0: on RAM, System Stack, Runtime heap, Ram,
	//Set entire ram as this region, other specific regions else can be set on top of this
	UINT32 mem_base = (UINT32)ERAM_ORIGIN;
	UINT32 mem_size = ERAM_SIZE;
	CPU_mpu_configure_region(GP_RAM, mem_base, mem_size, AP_RW_RO, false);

	//Region 2: Kernel Ram
	mem_base=(UINT32)&Image$$Kernel_ER_RAM_RW$$Base;
	mem_size=(UINT32)&Image$$Kernel_ER_RAM_RW$$Length + (UINT32 )&Image$$Kernel_ER_RAM_RW$$ZI$$Length;

	//mem_base=(void*)Load$$Kernel_ER_RAM_RW$$Base;
	//mem_size=Image$$Kernel_Total_RAM_RW$$Length;
	//CPU_mpu_configure_region(Kernel_RAM, mem_base, mem_size, AP_RW_RO, false);

	//Region 5: RoT Ram
	mem_base=(UINT32)&Image$$RoT_ER_RAM_RW$$Base;
	mem_size=(UINT32)&Image$$RoT_ER_RAM_RW$$Length+(UINT32)&Image$$RoT_ER_RAM_RW$$ZI$$Length;
	//CPU_mpu_configure_region(RoT_RAM, mem_base, mem_size, AP_RW_RO, false);

	//Flash regions
	//Region 1: Setup entire Flash as this region.
	mem_base = (UINT32)EFLASH_ORIGIN;
	mem_size = EFLASH_SIZE;
	CPU_mpu_configure_region(GP_CODE, mem_base, mem_size, AP_RO_RO, true);

	//Region 2: Entire IO is mapped to protect their access. Note: Other than kernel and RoT, nobody else can access IO
	mem_base = (UINT32)IO_ORIGIN;
	mem_size = IO_SIZE;
	CPU_mpu_configure_region(GP_IO, mem_base, mem_size, AP_RW_NO, false);

	//Deployment
	//mem_base = (UINT32)DEPLOY_BASE;
	//mem_size = DEPLOY_SIZE;
	//CPU_mpu_configure_region(3, mem_base, mem_size, AP_RW_RW, true);


	//Region 4: Kernel code
	mem_base = (UINT32)EFLASH_ORIGIN + ROT_SIZE;
	//mem_size = (UINT32)&Image$$Kernel_ER_FLASH$$Length;
	mem_size = (UINT32)KERNEL_SIZE;

	CPU_mpu_configure_region(Kernel_CODE, mem_base, mem_size, AP_RO_RO, true);

	//Region 6:
	mem_base = (UINT32)EFLASH_ORIGIN;
	//mem_size = (UINT32)Image$$RoT_ER_FLASH$$Length;
	mem_size = (UINT32)ROT_SIZE;
	CPU_mpu_configure_region(RoT_CODE, mem_base, mem_size, AP_RO_RO, true);

	// NULL pointer protection, highest priority.
	//CPU_mpu_configure_region(Reserve, 0, 5, AP_NO_NO, false);

	//Done configuring all regions. Enable and lock mpu
	CPU_mpu_lock();

}


void MPU_Init(){

}


void MemManage_HandlerC(UINT32 lr, UINT32 msp)
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
        debug_printf("MemManage Handler: Data access violation to %p (pc=%p)", (void *)SCB->MMFAR, ctx.pc);
    }

    /* Instruction address violation. */
    if (MMFSR & (1 << 0)) {
        debug_printf("MemManage Handler: Jumped to XN region %p (lr_thd=%p)", (void *)SCB->MMFAR, ctx.lr_thd);
    }

    //chSysHalt(msg);
}


SM_MemNames SecureMonitor_FindFaultRegion(UINT32 fault_addr){
	MpuRegion_t* mpuRegionPtr = CPU_mpu_findRegion((void*)fault_addr);
	if(mpuRegionPtr==NULL) return INVALID;
	return (SM_MemNames)mpuRegionPtr->regionNo;
}
