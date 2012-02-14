/* 
 * FirstEntry.s startup code for the arm cortex a8 sitara 
 * 
 * This file is modelled from uboots startup source start.s 
 *
 */

.global _start
_start : b reset
		ldr pc, _undefined_instruction
         ldr pc, _software_interrupt
         ldr pc, _prefetch_abort
         ldr pc, _data_abort
         ldr pc, _not_used
         ldr pc, _irq
         ldr pc, _fiq

_undefined_instruction: .word undefined_instruction
_software_interrupt : .word software_interrupt
_prefetch_abort  : .word prefetch_abort
_data_abort:		.word data_abort
_not_used   : .word not_used
_irq : .word irq
_fiq : .word fiq
_pad:			.word 0x12345678 /* now 16*4=64 */
.global _end_vect
/* Not sure of the function of end_vect yet */
_end_vect:

	.balignl 16,0xdeadbeef


.global IRQ_STACK_START
IRQ_STACK_START:
	.word	0x0badc0de

/* IRQ stack memory (calculated at run-time) */
.globl FIQ_STACK_START
FIQ_STACK_START:
	.word 0x0badc0de

	
_TEXT_BASE:
	.word TEXT_BASE

.global _mfboot_start

_mfboot_start:
	.word _start

.global _bss_start

_bss_start:
	.word __bss_start

.global _bss_end
_bss_end:
	.word _end

	
	.macro get_bad_stack
	ldr	r13, _armboot_start		@ setup our mode stack (enter
						@ in banked mode)
	sub	r13, r13, #(CONFIG_SYS_MALLOC_LEN)	@ move past malloc pool
	sub	r13, r13, #(CONFIG_SYS_GBL_DATA_SIZE + 8) @ move to reserved a couple
						@ spots for abort stack

	str	lr, [r13]			@ save caller lr in position 0
						@ of saved stack
	mrs	lr, spsr			@ get the spsr
	str	lr, [r13, #4]			@ save spsr in position 1 of
						@ saved stack

	mov	r13, #MODE_SVC			@ prepare SVC-Mode
	@ msr	spsr_c, r13
	msr	spsr, r13			@ switch modes, make sure
						@ moves will execute
	mov	lr, pc				@ capture return pc
	movs	pc, lr				@ jump to next instruction &
						@ switch modes.
	.endm
	
	.macro	irq_save_user_regs
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}			@ Calling r0-r12
	add	r8, sp, #S_PC			@ !! R8 NEEDS to be saved !!
						@ a reserved stack spot would
						@ be good.
	stmdb	r8, {sp, lr}^			@ Calling SP, LR
	str	lr, [r8, #0]			@ Save calling PC
	mrs	r6, spsr
	str	r6, [r8, #4]			@ Save CPSR
	str	r0, [r8, #8]			@ Save OLD_R0
	mov	r0, sp
	.endm
	
	.macro	irq_restore_user_regs
	ldmia	sp, {r0 - lr}^			@ Calling r0 - lr
	mov	r0, r0
	ldr	lr, [sp, #S_PC]			@ Get PC
	add	sp, sp, #S_FRAME_SIZE
	subs	pc, lr, #4			@ return & move spsr_svc into
						@ cpsr
	.endm
	
	.macro get_bad_stack_swi
	sub	r13, r13, #4			@ space on current stack for
						@ scratch reg.
	str	r0, [r13]			@ save R0's value.
	ldr	r0, _armboot_start		@ get data regions start
	sub	r0, r0, #(CONFIG_SYS_MALLOC_LEN)	@ move past malloc pool
	sub	r0, r0, #(CONFIG_SYS_GBL_DATA_SIZE + 8)	@ move past gbl and a couple
						@ spots for abort stack
	str	lr, [r0]			@ save caller lr in position 0
						@ of saved stack
	mrs	r0, spsr			@ get the spsr
	str	lr, [r0, #4]			@ save spsr in position 1 of
						@ saved stack
	ldr	r0, [r13]			@ restore r0
	add	r13, r13, #4			@ pop stack entry
	.endm

	.macro	bad_save_user_regs
	sub	sp, sp, #S_FRAME_SIZE		@ carve out a frame on current
						@ user stack
	stmia	sp, {r0 - r12}			@ Save user registers (now in
						@ svc mode) r0-r12

	ldr	r2, _armboot_start
	sub	r2, r2, #(CONFIG_SYS_MALLOC_LEN)
	sub	r2, r2, #(CONFIG_SYS_GBL_DATA_SIZE + 8)	@ set base 2 words into abort
						@ stack
	ldmia	r2, {r2 - r3}			@ get values for "aborted" pc
						@ and cpsr (into parm regs)
	add	r0, sp, #S_FRAME_SIZE		@ grab pointer to old stack

	add	r5, sp, #S_SP
	mov	r1, lr
	stmia	r5, {r0 - r3}			@ save sp_SVC, lr_SVC, pc, cpsr
	mov	r0, sp				@ save current stack into r0
						@ (param register)
	.endm

	.macro get_irq_stack			@ setup IRQ stack
	ldr	sp, IRQ_STACK_START
	.endm

	.macro get_fiq_stack			@ setup FIQ stack
	ldr	sp, FIQ_STACK_START
	.endm
	
undefined_instruction:
	get_bad_stack
	bad_save_user_regs
	bl	do_undefined_instruction

software_interrupt:
	get_bad_stack_swi
	bad_save_user_regs
	bl	do_software_interrupt

	.align	5
prefetch_abort:
	get_bad_stack
	bad_save_user_regs
	bl	do_prefetch_abort

	.align	5
data_abort:
	get_bad_stack
	bad_save_user_regs
	bl	do_data_abort

	.align	5
not_used:
	get_bad_stack
	bad_save_user_regs
	bl	do_not_used


	.align	5
irq:
	get_irq_stack
	irq_save_user_regs
	bl	do_irq
	irq_restore_user_regs

	.align	5
fiq:
	get_fiq_stack
	/* someone ought to write a more effective fiq_save_user_regs */
	irq_save_user_regs
	bl	do_fiq
	irq_restore_user_regs


.global EntryPoint
.extern ApplicationEntryPoint

EntryPoint:
	B ApplicationEntryPoint
