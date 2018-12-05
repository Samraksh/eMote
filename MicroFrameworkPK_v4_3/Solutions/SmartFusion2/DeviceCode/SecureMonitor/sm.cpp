#include <Samraksh/sm.h>

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


///return LR Register value
static UINT32  inline __get_LR(void)
{
  register uint32_t result;

  __ASM volatile ("MOV %0, LR\n" : "=r" (result) );
  return(result);
}

static inline void __set_LR(uint32_t lr)
{
  __ASM volatile ("MOV LR, %0" : : "r" (lr) : "memory");
}


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
	debug_printf("Secure Monitor: BG_ram: Mem base: %X, mem_size: %X, AP_RW_RW\n",mem_base, mem_size);
	CPU_mpu_configure_region(GP_RAM, mem_base, mem_size, AP_RW_RW, MemType_Normal,false);

	//Region 2: Kernel Ram
	mem_base=(UINT32)&Image$$Kernel_ER_RAM_RW$$Base;
	mem_size=(UINT32)&Image$$Kernel_ER_RAM_RW$$Length + (UINT32 )&Image$$Kernel_ER_RAM_RW$$ZI$$Length;

	//mem_base=(void*)Load$$Kernel_ER_RAM_RW$$Base;
	//mem_size=Image$$Kernel_Total_RAM_RW$$Length;
	//CPU_mpu_configure_region(Kernel_RAM, mem_base, mem_size, AP_RW_RO,MemType_Normal, false);

	//Region 5: RoT Ram
	mem_base=(UINT32)&Image$$RoT_ER_RAM_RW$$Base;
	mem_size=(UINT32)&Image$$RoT_ER_RAM_RW$$Length+(UINT32)&Image$$RoT_ER_RAM_RW$$ZI$$Length;
	//CPU_mpu_configure_region(RoT_RAM, mem_base, mem_size, AP_RW_RO,MemType_Normal, false);

	//Flash regions
	//Region 1: Setup entire Flash as this region.
	mem_base = (UINT32)EFLASH_ORIGIN;
	mem_size = EFLASH_SIZE;
	debug_printf("Secure Monitor: BG_code: Mem base: %X, mem_size: %X, AP_RW_RO\n",mem_base, mem_size);
	CPU_mpu_configure_region(GP_CODE, mem_base, mem_size, AP_RO_RO, MemType_Normal, true);
	//mem_base = (UINT32)EFLASH_MIRROR_ORIGIN;
	//CPU_mpu_configure_region(6, mem_base, mem_size, AP_RW_RO, MemType_Normal, true);

	//Region 2: Entire IO is mapped to protect their access. Note: Other than kernel and RoT, nobody else can access IO
	mem_base = (UINT32)IO_ORIGIN;
	mem_size = IO_SIZE;
	debug_printf("Secure Monitor: BG_IO: Mem base: %X, mem_size: %X, AP_RW_RO\n",mem_base, mem_size);
	CPU_mpu_configure_region(GP_IO, mem_base, mem_size, AP_RW_RO, MemType_Device_NonSharable, false);


	//Region 2: Entire IO is mapped to protect their access. Note: Other than kernel and RoT, nobody else can access IO
	mem_base = (UINT32)CPU_PPB_BASE;
	mem_size = CPU_PPB_SIZE;
	//CPU_mpu_configure_region(CPU_PPB, mem_base, mem_size, AP_RW_RW, MemType_StronglyOrdered, false);
	debug_printf("Secure Monitor: CPU_PPB: Mem base: %X, mem_size: %X, AP_RW_RW\n",mem_base, mem_size);
	CPU_mpu_configure_region(CPU_PPB, mem_base, mem_size, AP_RW_RW, MemType_Device_Sharable, false);

	//Deployment
	//mem_base = (UINT32)DEPLOY_BASE;
	//mem_size = DEPLOY_SIZE;
	//CPU_mpu_configure_region(3, mem_base, mem_size, AP_RW_RW, true);


	//Region : Kernel code: Rot + Kernel is just one region now. so permissions can be set as such
	//Plus include the IBL too for convinience
	mem_base = (UINT32)EFLASH_ORIGIN ;
	//mem_size = (UINT32)&Image$$Kernel_ER_FLASH$$Length;
	mem_size = (UINT32) (ROT_BASE+ ROT_SIZE+KERNEL_SIZE);

	debug_printf("Secure Monitor: ker_code: Mem base: %X, mem_size: %X, AP_RO_NO\n",mem_base, mem_size);
	CPU_mpu_configure_region(Kernel_CODE, mem_base, mem_size, AP_RW_NO, MemType_Normal, true);


	/*mem_base = (UINT32)EFLASH_ORIGIN;
	//mem_size = (UINT32)Image$$RoT_ER_FLASH$$Length;
	mem_size = (UINT32)ROT_SIZE + ROT_BASE;
	debug_printf("Secure Monitor: Rot_code: Mem base: %X, mem_size: %X, AP_RO_NO\n",mem_base, mem_size);
	CPU_mpu_configure_region(RoT_CODE, mem_base, mem_size, AP_RW_NO, MemType_Normal, true);
	*/

	// NULL pointer protection, highest priority.
	//CPU_mpu_configure_region(Reserve, 0, 5, AP_NO_NO, false);

	//Done configuring all regions. Enable and lock mpu
	CPU_mpu_lock();

}


void MPU_Init(){

}

typedef void (*svcall0_t)(void*);
typedef void (*svcall1_t)(void*, void*);

task_ctx_t *userCallCtx;
UINT32* interruptMSP;

void SwitchBackToUserMode(task_ctx_t ctx){

	/* Save registers R4-R11 (32 bytes) onto current PSP (process stack
		   pointer) and make the PSP point to the last stacked register (R8):
		   - The MRS/MSR instruction is for loading/saving a special registers.
		   - The STMIA inscruction can only save low registers (R0-R7), it is
		     therefore necesary to copy registers R8-R11 into R4-R7 and call
		     STMIA twice. */
	asm(
		"mrs	r0, psp\n"
		"subs	r0, #16\n"
		"stmia	r0!,{r4-r7}\n"
		"mov	r4, r8\n"
		"mov	r5, r9\n"
		"mov	r6, r10\n"
		"mov	r7, r11\n"
		"subs	r0, #32\n"
		"stmia	r0!,{r4-r7}\n"
		"subs	r0, #16"
	);

}



void SwitchToDifferentUserProcess(task_ctx_t *ctx){
	//interrupt handler hardware stacks exeception frame (xPSR, PC, LR, r12 and r3-r0)

	//Manually stack remaining registers r4-r11 on the Process Stack
	/*volatile UINT32 r4r11= (UINT32)ctx->r4_r11;
		asm(
				"mrs	r0, =r4r11\n"
				"subs	r0, #16\n"
				"stmia	r0!,{r4-r7}\n"
				"mov	r4, r8\n"
				"mov	r5, r9\n"
				"mov	r6, r10\n"
				"mov	r7, r11\n"
				"subs	r0, #32\n"
				"stmia	r0!,{r4-r7}\n"
				"subs	r0, #16\n"
		);
	*/

		/* Load registers R4-R11 (32 bytes) from the new PSP and make the PSP
			   point to the end of the exception stack frame. The NVIC hardware
			   will restore remaining registers after returning from exception): */
		asm(
			"ldmia	r0!,{r4-r7}\n"
			"mov	r8, r4\n"
			"mov	r9, r5\n"
			"mov	r10, r6\n"
			"mov	r11, r7\n"
			"ldmia	r0!,{r4-r7}\n"
			//"msr	psp, r0\n"
		);



	//Save current task’s PSP to memory
	//Load next task’s stack pointer and assign it to PSP
	//Manually unstack registers r4-r11
	//Call bx 0xfffffffD which makes the processor switch to Unprivileged Handler Mode, unstack next task’s exception frame and continue on its PC.


	//UINT32 msp = __get_MSP();
	//memcpy((void*)msp, &ctx, sizeof(task_ctx_t));

}


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

//The PendSV_Handler is going to switch the stack to kernel stack, execute it and return the results
//presumably we dont have to load and unload the registers because we are just switching stacks. Nothing is in the stacks anyway.
void SwitchToKernelThreadMode(void){
	//userCallCtx should be non zero, otherwise something is messed up.
	ASSERT(userCallCtx);

	//stackSize units are just added to  enable array bases alocation, the stack size is essentially set by
	//boot up process
	UINT32 stackSize=128;
	//get msp and decrement it. Remember its a FULL stack.

	//Get current MSP and store it
	UINT32* msp = (UINT32*)(__get_MSP());
	interruptMSP =msp; //we need to return to this, when we finish executing kernel method.

	//UINT32* msp=userCallCtx->stackframefp;

	//Now get a new stack frame
	//msp-=stackSize; //decrement by 128 bytes, which hopefully covers everthing this method has put on the stack.



	//just change the LR in the current stack to a different function and just return.
	msp[5]=(UINT32)&SwitchBackToUserMode;
	debug_printf("SwitchToKernelThreadMode: MSP is at %p, I want to return to : %p, not to  lr_thd=%p, pc=%p \n",msp, msp[5], userCallCtx->lr_thd, msp[6]);

	// This is the kernel function that caused the fault, and should be unchanged.
	//but double check just to be sure.
	ASSERT(msp[6]==userCallCtx->pc);

	//msp[-1]=  0x01000000; //xPSR
	//msp[-2]=  userCallCtx->pc; //PC/fuction to be called
	//msp[-3]=  (UINT32)&SwitchBackTOUserMode; //Link register

	__ISB(); // instruction barrier. Make sure everything till now is done
	//__set_MSP(((UINT32)msp+64)); // Set PSP to the top of task's stack

	// Load MSP
	/*asm(
		"ldr	r2, =msp\n"
		"ldr	r1, [r2]\n"
		"ldr	r0, [r1]\n""
		);
	 */

	/*There are just 3 modes in M3.
	F1- Handler mode
	F9 - MSP thread
	FD - PSP thread
	*/
	//enable interrupts and return to priviledged thread mode

	//becuase we are in a second stack in the interrupt processing, the original
	//LR has been pushed into the r7 register. change the mode from FD, to F9
	asm(
		"ldr r7, =0xFFFFFFF9\n"
		"cpsie	i\n"
		"bx r7\n"
	);

	//Not doing anything, but just switching the mode. but now we will return in
	__ISB();
	//__set_LR(0xFFFFFFF9);
	/*asm(
		"ldr r0, =0xFFFFFFF9\n"
		"cpsie	i\n"
		"bx	r0\n"
	);*/
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
    task_ctx_t ctx;

    UINT32 MMFSR;

    /* Setup default error message. */
    memcpy((void*)msg, (void*)__FUNCTION__, 128);

    // Get context info
    UINT32 lr;
    lr= __get_LR();

    // Determine the origin of the exception.
    bool from_psp = EXC_FROM_PSP(lr);
    UINT32 sp = from_psp ? __get_PSP() : __get_MSP();
    UINT32* msp =(UINT32*) __get_MSP();
    ctx.stackframefp=sp;

    memcpy(&ctx, (void*)sp, sizeof(task_ctx_t));

    /* Get Memory Managment fault adress register */
    MMFSR = SCB->CFSR & SCB_CFSR_MEMFAULTSR_Msk;
    UINT32 faultAddress=*(UINT32*)(SCB->MMFAR);

    if (MMFSR & (1 << 5)) {
               debug_printf("MemManage Handler: A MemManage fault occurred during FP lazy state preservation, in exec mode: %p at address %p (pc=%p)\n", lr, faultAddress, ctx.pc);
    }

    if (MMFSR & (1 << 4)) {
               debug_printf("MemManage Handler: A derived MemManage fault occurred on exception entry, in exec mode: %p at address %p (pc=%p)\n", lr, faultAddress, ctx.pc);
    }

    if (MMFSR & (1 << 3)) {
           debug_printf("MemManage Handler: A derived MemManage fault occurred on exception return, in exec mode: %p at address %p (pc=%p)\n", lr, faultAddress, ctx.pc);
    }

    // Data access violation
    if (MMFSR & (1 << 1)) {
        debug_printf("MemManage Handler: Data access violation in exec mode: %p at address %p (pc=%p)\n", lr, faultAddress, ctx.pc);
    }

    // Instruction address violation.
    if (MMFSR & (1 << 0)) {
    	if(MMFSR & (1 << 7)){
    		debug_printf("MemManage Handler: instruciton fault, PSP: %d, mmfar is valide. in  exec mode: %p at address %08X, %p,  lr_thd=%p, pc=%p\n",from_psp,lr, faultAddress,(void*)SCB->MMFAR, ctx.lr_thd, ctx.pc);

    	}else {
    		debug_printf("MemManage Handler: instruciton fault, PSP: %d, mmfar is NOT valid. LR: %p,  lr_thd=%p, pc=%p\n",from_psp,lr, ctx.lr_thd, ctx.pc);

			//if calling from user space into kernel space code//
			//Then switch stacks execute code and then return
			if(from_psp && (ctx.pc >= ROT_BASE &&  ctx.pc <= KERNEL_END)){
				debug_printf("MemManage Handler: Ok this a call into kernel, lets Switch the stack and return to %p\n", ctx.lr_thd);
				//Save the contex of the usercall stack to global variable and strigger the PendSV
				userCallCtx=&ctx;
				// Trigger PendSV which performs the actual context switch:
				//SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

				//SwitchToKernelThreadMode();
				msp[5]=(UINT32)&SwitchBackToUserMode;
				debug_printf("SwitchToKernelThreadMode: MSP is at %p, I want to return to : %p, not to  lr_thd=%p, pc=%p\n",msp, msp[5], ctx.lr_thd, msp[6]);
				ASSERT(msp[6]==userCallCtx->pc);
				__set_LR(0xFFFFFFF9);
				__ISB();
			}

    	}
    }

    //chSysHalt(msg);
}


SM_MemNames SecureMonitor_FindFaultRegion(UINT32 fault_addr){
	MpuRegion_t* mpuRegionPtr = CPU_mpu_findRegion((void*)fault_addr);
	if(mpuRegionPtr==NULL) return INVALID;
	return (SM_MemNames)mpuRegionPtr->regionNo;
}


//Misc HAL layer helper functions






void SetupUserStack(){
	__set_PSP(SAM_USER_STACK_TOP);
}

//Only last 2 bits of control register are used in CortexM
void SwitchToPriviledgeMode(){
	//unset bit 0 of control to change to unpriviledge mode
	//unset bit 1 of control to change to psp.
	__set_CONTROL(0x0);
}

void SwitchToUserMode(){
	//set bit 0 of control to change to unpriviledge mode
	//set bit 1 of control to change to psp.
	__set_CONTROL(0x3);
	__ISB();
	//SetupUserStack();

	/*asm(
		"MOVS R0, #0x3 \n"
		"MSR CONTROL, R0 \n" //Switch to non-privileged state
		"ISB \n" //Instruction Synchronization Barrier
	);*/
}

void kernel_call0(void (*func)(void*), void* arg0){
     //by convention func is in r0 and args is in r1
     asm volatile("svc 0");
}

void kernel_call1(void (*func)(void*), void* arg0, void* arg1){
     //by convention func is in r0 and args is in r1
     asm volatile("svc 1");
}

