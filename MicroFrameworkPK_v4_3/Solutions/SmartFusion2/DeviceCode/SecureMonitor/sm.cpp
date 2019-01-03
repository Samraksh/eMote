#include <Samraksh/sm.h>

#include <Samraksh/cm_mpu.h>
#include <cmsis/m2sxxx.h>
#include <core_cm3.h>
//#include <DeviceCode/stmlib/exc_return.h>
#include <mpu/cmx_ctx.h>
#include "SF2_CM3.h"
//#include <string.h>
#include <tinyhal.h>
#include <Samraksh/os_scheduler.h>


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


typedef void (*svcall0_t)(void*);
typedef void (*svcall1_t)(void*, void*);

task_ctx_t *userCallCtx; UINT32 userCallPC;
task_ctx_t memFault_ctx;
UINT32 interruptMSP;
UINT32 interruptLR;

extern bool inPendSV_irq;

void Set_InPendSV(bool x){inPendSV_irq=x;}

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
	memset(&memFault_ctx, 0, sizeof(task_ctx_t));
	NVIC_SetPriority(ComBlk_IRQn, 5);
	NVIC_SetPriority(PendSV_IRQn, 15);
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

void KernFuncTest(char *string){

	UINT32 cpuMode=GetExecMode();
	while(1){
		volatile UINT32 x=2+3;
		debug_printf( "KernFuncTest: Nothing to do... in CPU mode: %d. \r\n", cpuMode);
	}
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

void copyResultAndResetCall(UINT32* sp){
	//Lets copy the results back into the user stack

	//first copy r0-r3 registers// args 0-3
	//we push this into the stack, just before this call, so copy from stack.
	UINT32* usp= ((UINT32*)userCallCtx->stackframefp);
	memcpy((void*)usp,sp,16); //copy stuff.

	/*
	//next copy args 4-7 into user stack.
	//while both ptrs are incremented by 8 its for different reasons
	usp =usp +8; //this goes back to end of user stack, before the hardware exception stack push
	sp=sp+4; // this goes back to end of msp when we pushed the args,
				// add 4 words for r0-r3 registers we pushed after that.

	memcpy((void*)usp,sp,16);
	*/
	userCallCtx=0;
	userCallPC=0;
}


void SwitchBackToUserMode(task_ctx_t *ctx){
	/* Save registers R4-R11 (32 bytes) onto current PSP (process stack
		   pointer) and make the PSP point to the last stacked register (R8):
		   - The MRS/MSR instruction is for loading/saving a special registers.
		   - The STMIA inscruction can only save low registers (R0-R7), it is
		     therefore necesary to copy registers R8-R11 into R4-R7 and call
		     STMIA twice. */


	asm("cpsid	i\n"); //disable interrupt
	///first thing store the returns on the stack
	asm("push {r0-r3}");

	copyResultAndResetCall((UINT32*)__get_MSP());

	//get results back from stack, so that stack address are not messed up.
	asm("pop {r0-r3}");

	//go back to interrupt mode and frame and exit.
	__set_MSP(interruptMSP);
	__set_LR(interruptLR);
	asm("cpsie	i\n"); //enable interrupt
	//asm("BX LR");
}



//The PendSV_Handler is going to switch the stack to kernel stack, execute it and return the results
//presumably we dont have to load and unload the registers because we are just switching stacks. Nothing is in the stacks anyway.
void SwitchToKernelThreadMode(void){
	//userCallCtx should be non zero, otherwise something is messed up.
	ASSERT(userCallCtx);

	//stackSize units are just added to  enable array bases alocation, the stack size is essentially set by
	//boot up process
	UINT32 stackGap=128;

	//get msp and decrement it. Remember its a FULL stack.

	//Get current MSP and store it
	//we need to return to this, when we finish executing kernel method.
	interruptMSP= __get_MSP();
	UINT32* msp = (UINT32*)interruptMSP;

	//Now get a new stack frame
	msp=msp - stackGap; //decrement by 128 bytes, which hopefully covers everthing this method has put on the stack.

	//just change the LR in the current stack to a different function and just return.
	//msp[5]=(UINT32)&SwitchBackToUserMode;

	// This is the kernel function that caused the fault, and should be unchanged.
	//but double check just to be sure.
	//ASSERT(msp[6]==userCallCtx->pc);

	// when we get out of nvic, the hardware is going to pop 8 addresses
	//and our task execution will start from where we want the new msp
	msp = msp - 8;

	//copy ro,r1,r3,r12,LR,pc,xpsr of the registers
	//msp[-1]=  0x01000000; //xPSR
	//msp[-2]=  userCallCtx->pc; //PC/fuction to be called
	//msp[-3]=  (UINT32)&SwitchBackToUserMode; //Link register

	msp[7]=  0x01000000; //xPSR
	msp[6]=  ((UINT32)&KernFuncTest) |0x1;
	//msp[6]=  userCallCtx->pc | 0x1; //PC/fuction to be called
	//msp[6]=  (UINT32)userCallPC; //pc
	msp[5]=  ((UINT32)&SwitchBackToUserMode) | 0x1; //Link register
	msp[4]= userCallCtx->r12;
	msp[3]= userCallCtx->r3;
	msp[2]= userCallCtx->r2;
	msp[1]= userCallCtx->r1;
	msp[0]= userCallCtx->r0;


	/*msp[-1]=  0x01000000; //xPSR
	msp[-2]=  userCallCtx->pc; //PC/fuction to be called
	msp[-3]=  (UINT32)&SwitchBackToUserMode; //Link register
	msp[-4]= userCallCtx->r12;
	msp[-5]= userCallCtx->r3;
	msp[-6]= userCallCtx->r2;
	msp[-7]= userCallCtx->r1;
	msp[-8]= userCallCtx->r0;

	msp = msp - 16;
	*/

	//debug_printf("SwitchToKernel: Setting up MSP at %p\n", msp);

	//__ISB(); // instruction barrier. Make sure everything till now is done

	__set_MSP((UINT32)msp);

	/*There are just 3 modes in M3.
	F1- Handler mode
	F9 - MSP thread
	FD - PSP thread
	*/
	//enable interrupts and return to priviledged thread mode


	//We need to copy r4-r11 into the registers from call contex, before jumping
	//UINT32 addr = ((UINT32)userCallCtx) +64;
	/*asm("Mov	r0, %0" : : "r"(&userCallCtx->r4));
	asm(
		"ldmia	r0!,{r4-r7}\n"
		"mov	r8, r4\n"
		"mov	r9, r5\n"
		"mov	r10, r6\n"
		"mov	r11, r7\n"
		//"add	r0, #32\n" //add 32 to address at r0
		"ldmia	r0!,{r4-r7}\n"
		//"msr	psp, r0\n"
	);
    */

	// This section, tries to set up priviledged thread mode
	interruptLR=__get_LR();
	__set_LR(0xFFFFFFF9);
	asm("cpsie	i\n");__ISB();
	//This should pop the interrrupt stack
	//but instead of interrupt stack, it will pop the stack that we just setup.
	asm("BX LR");


	//This continues in interrupt mode
	//volatile UINT32 addr = userCallCtx->pc;
	//asm("MOV pc, %0" : : "r"(userCallCtx->pc));
	//asm("bx	r0\n");
	//asm("BX %0" : : "r"(userCallCtx->pc));

}


void  __irq PendSV_Handler(){
	asm("cpsid	i\n");
	//first copy args 4-11 into stack, args 0-3 are dealt as registers r0-r3
	Set_InPendSV(1);
	asm("Mov	r0, %0" : : "r"(userCallCtx->stackframefp));
	asm(
			"add r0, #48\n" //8 32-bit words were copied into the stack by the hardware exception handler
							//another 4 words for  args4-7, we will copy 8-11 first.
			"ldmia	r0!,{r4-r7}\n"
			"push {r4-r7}\n");

	asm(
			"sub r0, #32\n" //go back to the stack pointer just before exception handler.
							//copy args 4-7 now
			"ldmia	r0!,{r4-r7}\n"
			"push {r4-r7}"
		);


	//We need to copy r4-r11 into the registers from call contex, before jumping
	asm("Mov	r0, %0" : : "r"(&userCallCtx->r8));
	asm(
		"ldmia	r0!,{r4-r7}\n"
		"mov	r8, r4\n"
		"mov	r9, r5\n"
		"mov	r10, r6\n"
		"mov	r11, r7\n"
		"subs	r0, #32\n" //r0 should now point to after r11, subtract 32 to go address of r4
		"ldmia	r0!,{r4-r7}\n"
		//"msr	psp, r0\n"
	);

	asm("Mov	r12, %0" : : "r"(&userCallCtx->r0));
	asm("ldmia	r12!,{r0-r3}\n");
	asm("push	{r3}\n");
	asm("mov r3, %0" :: "r"(userCallPC));
	asm("mov r12,r3");
	asm("pop	{r3}\n");
	//At this point all registers except r12 should be in same state, when the kernel function was called
	//r12 is supposed ot be function scractch,

	//enable interrupt and branch
	asm("cpsie	i\n");__ISB();
	asm("BLX r12");

	asm("cpsid	i\n"); //disable interrupt
	///first thing store the returns on the stack
	asm("push {r0-r3}");

	copyResultAndResetCall((UINT32*)__get_MSP());

	//We wont manipulate the return address here. Thats already been done in MemManage Handler

	//setup link register to be ready to get out
	__set_LR(0xFFFFFFFD);
	//get results back from stack, so that stack address are not messed up.
	asm("pop {r0-r3}"); //for r0-r3

	asm("pop {r0-r3}"); //for arg 4-7
	asm("pop {r0-r3}"); //for arg 8-11

	Set_InPendSV(0);
	asm("cpsie	i\n"); __ISB(); //enable interrupt
	//asm("BX LR");
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
	asm("cpsid	i\n");
    //memcpy(&memFault_ctx, 0, sizeof(task_ctx_t));
    //store r4-r11 into the context
    //regarm_t* addr = &memFault_ctx.r4
    asm("Mov	r0, %0" : : "r"(&memFault_ctx.r4));  //load address in ro
	asm(
		"stmia	r0!,{r4-r7}\n" //store r4-r7 at address
		"mov	r4, r8\n" //move r8-r11 into r4-r7
		"mov	r5, r9\n"
		"mov	r6, r10\n"
		"mov	r7, r11\n"
		//"add	r0, #32\n" //add 32 to address at r0
		"stmia	r0!,{r4-r7}\n" // store r4-r7 again
    );

	__ISB();
	__DSB();
    // Get context info
    UINT32 lr= __get_LR();

    // Determine the origin of the exception.


    UINT32* msp =(UINT32*) __get_MSP();

    bool from_psp = EXC_FROM_PSP(lr);
    UINT32 sp;
    if(from_psp){
    	sp= __get_PSP();
    }
    else{
    	//This fault created a new frame. so the original faulting frame was probably before this
    	sp= (UINT32)msp + 32;
    }

    memFault_ctx.stackframefp=sp;

    static char msg[128];
    UINT32 MMFSR;
    //copy only 32 bytes that get pushed by default by the interrupt handler
    memcpy(&memFault_ctx, (void*)sp, 32);


    /* Setup default error message. */
    memcpy((void*)msg, (void*)__FUNCTION__, 128);

    /* Get Memory Managment fault adress register */
    MMFSR = SCB->CFSR & SCB_CFSR_MEMFAULTSR_Msk;
    UINT32 faultAddress=*(UINT32*)(SCB->MMFAR);

    //clear the interrupt.
    // one does not have to clear if this needs to be manually cleared.
    //but just to be sure
    NVIC_ClearPendingIRQ(MemoryManagement_IRQn); //-12

    if (MMFSR & (1 << 5)) {
               debug_printf("MemManage Handler: A MemManage fault occurred during FP lazy state preservation, in exec mode: %p at address %p (pc=%p)\n", lr, faultAddress, memFault_ctx.pc);
    }

    if (MMFSR & (1 << 4)) {
               debug_printf("MemManage Handler: A derived MemManage fault occurred on exception entry, in exec mode: %p at address %p (pc=%p)\n", lr, faultAddress, memFault_ctx.pc);
    }

    if (MMFSR & (1 << 3)) {
           debug_printf("MemManage Handler: A derived MemManage fault occurred on exception return, in exec mode: %p at address %p (pc=%p)\n", lr, faultAddress, memFault_ctx.pc);
    }

    // Data access violation
    if (MMFSR & (1 << 1)) {
        debug_printf("MemManage Handler: Data access violation. MMFSR %08X, LR : %p at address %p (pc=%p)\n",MMFSR, lr, faultAddress, memFault_ctx.pc);
    }

    // Instruction address violation.
    if (MMFSR & (1 << 0)) {
    	if(MMFSR & (1 << 7)){
    		debug_printf("MemManage Handler: instruciton fault, PSP: %d, mmfar is valide. in  exec mode: %p at address %08X, %p,  lr_thd=%p, pc=%p\n",from_psp,lr, faultAddress,(void*)SCB->MMFAR, memFault_ctx.lr_thd, memFault_ctx.pc);
    		debug_printf("\n\r");
    		debug_printf("\n\r");
    		debug_printf("\n\r");
    	}else {

    		//debug_printf("MemManage Handler: instruciton fault, PSP: %d, mmfar is NOT valid. LR: %p,  lr_thd=%p, pc=%p\n",from_psp,lr, memFault_ctx.lr_thd, memFault_ctx.pc);

			//if calling from user space into kernel space code//
			//Then switch stacks execute code and then return
			if(from_psp ){
				if(memFault_ctx.pc >= ROT_BASE &&  memFault_ctx.pc <= KERNEL_END){

#if KERNEL_LOG==1
					//debug_printf("\nKernel Call: To address %p, return to %p\n", memFault_ctx.pc, memFault_ctx.lr_thd);
#endif
					//Save the contex of the usercall stack to global variable and strigger the PendSV

					if(userCallCtx!=0){
						debug_printf("\nMemManager Handler Error 0: Something wrong, we are already in a Kernel Call\n");
						debug_printf("\nMemManager Handler Error 1: Something wrong, we are already in a Kernel Call\n");
						debug_printf("\nMemManager Handler Error 2: Something wrong, we are already in a Kernel Call\n");
						debug_printf("\nMemManager Handler Error 3: Something wrong, we are already in a Kernel Call\n");
						debug_printf("\nMemManager Handler Error 4: Something wrong, we are already in a Kernel Call\n");


						debug_printf("\nKernel Call: New call to address %p, return to %p\n", memFault_ctx.pc, memFault_ctx.lr_thd);
						debug_printf("\nKernel Call: New call to address %p, return to %p\n", memFault_ctx.pc, memFault_ctx.lr_thd);
						HAL_Assert((LPCSTR)__func__, __LINE__,(LPCSTR)__FILE__);
					}

					userCallCtx=&memFault_ctx;
					userCallPC=userCallCtx->pc | 0x1;

					//lets manipulate the return address before we jump
					//In cortexm, memfault always returns to the same address that caused the fault
					//This is stored as PC, in the stack.
					//We are overwritting this PC location with LR value, which is in the next place.
					UINT32 *userStack=(UINT32 *)sp;
					userStack[6]=userStack[5];


					///Do one of the below, either PendSV or SwitchToKernelThreadMode
					// Trigger PendSV which performs the actual context switch:
					SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
					__enable_irq(); __ISB(); //enable interrrupt

					//or this
					//SwitchToKernelThreadMode();
				}
				else {
						debug_printf("\nMemManage Fault: Call from User space, but outside kernel code region. Potential hack attempt. To address %p, return to %p\n", memFault_ctx.pc, memFault_ctx.lr_thd);
					}
			}
			else {
				debug_printf("MemManage Handler: Not a call from User space. instruciton fault. Weird. PSP: %d, mmfar is NOT valid. LR: %p,  lr_thd=%p, pc=%p\n",from_psp,lr, memFault_ctx.lr_thd, memFault_ctx.pc);
				//debug_printf("\nMemManage Fault:  To address %p, return to %p\n", memFault_ctx.pc, memFault_ctx.lr_thd);
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

