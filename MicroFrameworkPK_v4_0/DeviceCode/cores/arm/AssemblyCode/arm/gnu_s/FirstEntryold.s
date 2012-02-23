@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copyright (c) Microsoft Corporation.  All rights reserved.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


    .global  EntryPoint
    .global  PreStackInit_Exit_Pointer
	.extern myEntry

	.extern  NotUsed_Handler
    .extern  PreStackInit

	@#define PLATFORM_ARM_AM3517

	@.ifdef PLATFORM_ARM_AM3517

	.extern ABORTP_SubHandler_Trampoline
	.extern ABORTD_SubHandler_Trampoline
	.extern IRQ_SubHandler_Trampoline
	.extern FIQ_SubHandler_Trampoline
	@.endif

    .ifdef HAL_REDUCESIZE

    .extern BootEntryLoader

    .else

    .extern BootEntry

    .endif

    .extern  BootstrapCode
    .extern  ARM_Vectors         @ Even if we don't use this symbol, it's required by the linker to properly include the Vector trampolines.

@   PRESERVE8

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PSR_MODE_USER       =     0xD0
PSR_MODE_FIQ        =     0xD1
PSR_MODE_IRQ        =     0xD2
PSR_MODE_SUPERVISOR =     0xD3
PSR_MODE_ABORT      =     0xD7
PSR_MODE_UNDEF      =     0xDB
PSR_MODE_SYSTEM     =     0xDF

SRAM_OFFSET0   = 0x40000000
SRAM_OFFSET1   = 0x00200000
@SRAM_OFFSET2   = 0x0000F800
SRAM_OFFSET2   = 0x0000FF00

STACK_TOP = 0x86000000


STACK_MODE_ABORT    =     16
STACK_MODE_UNDEF    =     16
    .ifdef FIQ_SAMPLING_PROFILER
STACK_MODE_FIQ      =     2048
    .else
STACK_MODE_IRQ      =     2048
    .endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .section SectionForStackBottom,       "a", %progbits
StackBottom:
    .word   0
    .section SectionForStackTop,          "a", %progbits
StackTop:
    .word   0
    .section SectionForHeapBegin,         "a", %progbits
HeapBegin:
    .word   0
    .section SectionForHeapEnd,           "a", %progbits
HeapEnd:
    .word   0
    .section SectionForCustomHeapBegin,   "a", %progbits
CustomHeapBegin:
    .word   0
    .section SectionForCustomHeapEnd,     "a", %progbits
CustomHeapEnd:
    .word   0


    .global StackBottom
    .global StackTop
    .global HeapBegin
    .global HeapEnd
    .global CustomHeapBegin
    .global CustomHeapEnd


    .section i.EntryPoint, "xa", %progbits

    .arm


EntryPoint:
	B       PreStackEntry

SetUpExceptionRouting:
	@ Going into system mode
	@msr     cpsr_c, #PSR_MODE_SYSTEM
	@B myEntry
	@.ifdef PLATFORM_ARM_AM3517
	@ Nived.Sivadas - Attempting to replicate uboot start up sequence for the SOC8200
	b reset
	_pad1:			.word 0x12345678 /* now 16*4=64 */
	_pad2:			.word 0x12345678
	ldr	pc, UNDEF_SubHandler_Trampoline
	ldr	pc, _software_interrupt
	ldr	pc, ABORTP_SubHandler_Trampoline
	ldr	pc, ABORTD_SubHandler_Trampoline
	ldr	pc, _not_used
	ldr	pc, IRQ_SubHandler_Trampoline
	ldr	pc, FIQ_SubHandler_Trampoline
	@.endif

	UNDEF_SubHandler_Trampoline:
    .word   UNDEF_SubHandler

	_software_interrupt:
	.word   0xbaadf00d

	ABORTP_SubHandler_Trampoline:
    .word   ABORTP_SubHandler

	ABORTD_SubHandler_Trampoline:
    .word   ABORTD_SubHandler

	_not_used:
	.word   NotUsed_Handler

	IRQ_SubHandler_Trampoline:
    .word  	IRQ_Handler

	FIQ_SubHandler_Trampoline:
    .word   FIQ_Handler

@	_pad:			.word 0x12345678 /* now 16*4=64 */

@.global _end_vect
@_end_vect:

@	.balignl 16,0xdeadbeef


    @.ifdef  HAL_REDUCESIZE
    @.ifndef TARGETLOCATION_RAM
    @ -----------------------------------------------
    @ ADD BOOT MARKER HERE IF YOU NEED ONE
    @ -----------------------------------------------
    @.ifdef  PLATFORM_ARM_LPC22XX
    @ orr     r0, pc,#0x80000000
    @ mov     pc, r0
    @.endif

    @.endif
    @.endif


    @ designed to be a vector area for ARM7
    @ RESET
    @ keep PortBooter signature the same

    @msr     cpsr_c, #PSR_MODE_SYSTEM    @ go into System mode, interrupts off
	@B       PreStackEntry
    @--------------------------------------------------------------------------------
    @ ALLOW pre-stack initilization
    @ Use the relative address of PreStackInit (because memory may need to be remapped)
    @--------------------------------------------------------------------------------


	@.ifdef PLATFORM_ARM_LPC24XX
    @ LPC24XX on chip bootloader requires valid checksum in internal Flash
    @ location 0x14 ( ARM reserved vector ).
    @B       PreStackEntry
    @.space 20
    @.endif

@ Nived.Sivadas - Adding reset code will eventually be moved to PreStackInit.s
@.ifdef PLATFORM_ARM_AM3517

reset:
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr,r0
	adr	r0, SetUpExceptionRouting		@ r0 <- current position of code
	add	r0, r0, #4		@ skip reset vector
	mov	r2, #64			@ r2 <- size to copy
	add	r2, r0, r2		@ r2 <- source end address
	mov	r1, #SRAM_OFFSET0	@ build vect addr
	mov	r3, #SRAM_OFFSET1
	add	r1, r1, r3
	mov	r3, #SRAM_OFFSET2
	add	r1, r1, r3
	add r1, r1 , #192    @ Nived - Goind for broke
next:
	ldmia	r0!, {r3 - r10}		@ copy from source address [r0]
	stmia	r1!, {r3 - r10}		@ copy to   target address [r1]
	cmp	r0, r2			@ until source end address [r2]
	bne	next			@ loop until equal */

	@bl	cpu_init_crit
	@B       PreStackEntry
	ldr     pc, EntryPoint_Restart_Pointer
@.endif


PreStackEntry:
    B       PreStackInit


PreStackInit_Exit_Pointer:

    @ldr     r0, =StackTop               @ new SYS stack pointer for a full decrementing stack

    @msr     cpsr_c, #PSR_MODE_ABORT     @ go into ABORT mode, interrupts off
    @mov     sp, r0                      @ stack top
    @sub     r0, r0, #STACK_MODE_ABORT   @ ( take the size of abort stack off )

    @msr     cpsr_c, #PSR_MODE_UNDEF     @ go into UNDEF mode, interrupts off
    @mov     sp, r0                      @ stack top - abort stack
    @sub     r0, r0, #STACK_MODE_UNDEF   @


    @.ifdef FIQ_SAMPLING_PROFILER
    @msr     cpsr_c, #PSR_MODE_FIQ       @ go into FIQ mode, interrupts off
    @mov     sp, r0                      @ stack top - abort stack - undef stack
    @sub     r0, r0, #STACK_MODE_FIQ
    @.endif

    @msr     cpsr_c, #PSR_MODE_IRQ       @ go into IRQ mode, interrupts off
    @mov     sp, r0                      @ stack top - abort stack - undef stack (- FIQ stack)
    @sub     r0, r0, #STACK_MODE_IRQ

    @msr     cpsr_c, #PSR_MODE_SYSTEM    @ go into System mode, interrupts off
    @mov     sp,r0                       @ stack top - abort stack - undef stack (- FIQ stack) - IRQ stack

	@msr 	cpsr_c, #PSR_MODE_SUPERVISOR
	@msr cpsr_c, #PSR_MODE_USER

	@ trying to solve problem with stack
	B 	SetUpExceptionRouting
        @******************************************************************************************
        @ This ensures that we execute from the real location, regardless of any remapping scheme *
        @******************************************************************************************

    @ldr     pc, EntryPoint_Restart_Pointer
EntryPoint_Restart_Pointer:
    .word   EntryPoint_Restart
EntryPoint_Restart:

        @*********************************************************************

    bl      BootstrapCode

    @ldr     r0, =StackTop               @ new svc stack pointer for a full decrementing stack

    .ifdef FIQ_SAMPLING_PROFILER
    @sub     sp, r0, #STACK_MODE_FIQ     @
    .else
    @sub     sp, r0, #STACK_MODE_IRQ     @
    .endif


    .ifdef HAL_REDUCESIZE

  .ifdef COMPILE_THUMB
        ldr   r0,BootEntryLoaderPointer
        bx      r0
   .else
        ldr   pc,BootEntryLoaderPointer
    .endif

BootEntryLoaderPointer:
        .word   BootEntryLoader


   .else


  .ifdef COMPILE_THUMB
        ldr     r0,BootEntryPointer
        bx      r0
   .else
        ldr     pc,BootEntryPointer
    .endif
BootEntryPointer:
        .word   BootEntry
    .endif

  .ifdef COMPILE_THUMB
    .thumb
  .endif

    .end

