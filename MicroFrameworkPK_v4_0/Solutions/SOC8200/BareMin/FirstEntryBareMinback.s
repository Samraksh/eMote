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
_not_used   : .word not_used
_irq : .word irq
_fiq : .word fiq
_pad:			.word 0x12345678 /* now 16*4=64 */
.global _end_vect
/* Not sure of the function of end_vect yet */
_end_vect:

	.balignl 16,0xdeadbeef

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




.global EntryPoint
.extern ApplicationEntryPoint

EntryPoint:
	B ApplicationEntryPoint
