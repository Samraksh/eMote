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
	CPU_mpu_configure_region(GP_RAM, mem_base, mem_size, AP_RW_RW, false);

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

	CPU_mpu_configure_region(Kernel_CODE, mem_base, mem_size, AP_RO_NO, true);

	//Region 6:
	mem_base = (UINT32)EFLASH_ORIGIN;
	//mem_size = (UINT32)Image$$RoT_ER_FLASH$$Length;
	mem_size = (UINT32)ROT_SIZE;
	CPU_mpu_configure_region(RoT_CODE, mem_base, mem_size, AP_RO_NO, true);

	// NULL pointer protection, highest priority.
	//CPU_mpu_configure_region(Reserve, 0, 5, AP_NO_NO, false);

	//Done configuring all regions. Enable and lock mpu
	CPU_mpu_lock();

}


void MPU_Init(){

}

typedef void (*svcall0_t)(void*);
typedef void (*svcall1_t)(void*, void*);



//parameter is the caller's stack frame pointer
void SVCall_HandlerC(UINT32 sp){
	UINT32 svcNumber;
	UINT32 *frame=(UINT32*)sp;
	//register uint32_t * frame;
	register void *arg0, *arg1;
	//asm volatile ("MRS %0, psp\n\t" : "=r" (frame) ); //assumes PSP in use when service_call() invoked
	svcNumber=  ((char *)frame[7])[-2];

	arg0 = (void*)(frame[2]);
	switch(svcNumber){
		case 1:
		{
			arg1 = (void*)(frame[3]);
			register svcall1_t call1 = (svcall1_t)frame[1];
			call1(arg0,arg1);
			break;
		}
		case 0:{}
		default:
			register svcall0_t call0 = (svcall0_t)frame[1];
			call0(arg0);
			break;
	}
}

//Checks execution mode of caller and gets the appropriate stack
//then calls the C handler with the stack pointer
/*void SVCall_Handler(void){
	/*asm volatile(
		"tst lr, #4\t\n" // Check EXC_RETURN[2]
		"ite eq\t\n"
		"mrseq r0, msp\t\n"
		"mrsne r0, psp\t\n"
		"b %[SVCall_HandlerC]\t\n"
		: //no output
		: [SVCall_HandlerC] "i" (SVCall_HandlerC) //input
		: "r0" //clobber
	);
	*/
/*	bool from_psp = EXC_FROM_PSP(__get_LR());
	UINT32 sp = from_psp ? __get_PSP() : __get_MSP();
	SVCall_HandlerC(sp);
}
*/

//void __irq MemManage_Handler(UINT32 lr, UINT32 msp)
void __irq MemManage_Handler()
{
    static char msg[128];
    struct port_extctx ctx;

    UINT32 MMFSR;

    /* Setup default error message. */
    memcpy((void*)msg, (void*)__FUNCTION__, 128);

    // Get context info
    UINT32 lr;
    lr= __get_LR();

    // Determine the origin of the exception.
    bool from_psp = EXC_FROM_PSP(lr);
    UINT32 sp = from_psp ? __get_PSP() : __get_MSP();

    memcpy(&ctx, (void*)sp, sizeof(struct port_extctx));

    /* Get Memory Managment fault adress register */
    MMFSR = SCB->CFSR & SCB_CFSR_MEMFAULTSR_Msk;
    UINT32 faultAddress=SCB->MMFAR;

    /* Data access violation */
    if (MMFSR & (1 << 1)) {
        //debug_printf("MemManage Handler: Data access violation to %08X (pc=%p)", faultAddress, ctx.pc);
    }

    /* Instruction address violation. */
    if (MMFSR & (1 << 0)) {
        //debug_printf("MemManage Handler: Jumped to XN region %08X, %p  (lr_thd=%p)", faultAddress,(void*)SCB->MMFAR, ctx.lr_thd);

        //if calling from user space into kernel space code//
        //Then switch stacks execute code and then return
        if(from_psp && (faultAddress >= ROT_BASE &&  faultAddress <= KERNEL_END)){
        	debug_printf("MemManage Handler: Ok this a call into kernel, lets do it\n");
        }

    }

    //chSysHalt(msg);
}


SM_MemNames SecureMonitor_FindFaultRegion(UINT32 fault_addr){
	MpuRegion_t* mpuRegionPtr = CPU_mpu_findRegion((void*)fault_addr);
	if(mpuRegionPtr==NULL) return INVALID;
	return (SM_MemNames)mpuRegionPtr->regionNo;
}
