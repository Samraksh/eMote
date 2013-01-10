@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copyright (c) Microsoft Corporation.  All rights reserved.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .cpu cortex-m3
    .code 16

    .global  EntryPoint

    .ifdef HAL_REDUCESIZE
    .extern BootEntryLoader
    .else
    .extern BootEntry
    .endif

    .extern  BootstrapCode
    .extern  Boot_Vectors         @ Even if we don't use this symbol, it's required by the linker to properly include the Vector trampolines.
	.extern  ARM_Vectors
	.extern  Prot_Bytes
	.extern  SystemInit_ExtMemCtl


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .section SectionForStackBottom,       "a", %progbits
StackBottom:
    .word   0
    .section SectionForStackTop,          "a", %progbits
StackTop:
    .word   0
    .section SectionForHeapBegin,         "a", %progbits
HeapBegin:
	.word   0x2000C000 @for stmxl of 96KB ram
    @.word   0x20009000
    .section SectionForHeapEnd,           "a", %progbits
HeapEnd:
	.word   0x20018000 @for stmxl of 96KB ram
    @.word   0x20010000 @for stm of 64KB ram
    .section SectionForCustomHeapBegin,   "a", %progbits
CustomHeapBegin:
	.word   0x2000C000 @for stmxl of 96KB ram
    @.word   0x20009000
    .section SectionForCustomHeapEnd,     "a", %progbits
CustomHeapEnd:
	.word   0x20018000 @for stmxl of 96KB ram
    @.word   0x20010000 @for stm of 64KB ram

    .global StackBottom
    .global StackTop
    .global HeapBegin
    .global HeapEnd
    .global CustomHeapBegin
    .global CustomHeapEnd

	.equ  InitStackTop,  0x2000C000     @for stmxl of 96KB ram
	@.equ  InitStackTop,  0x20009000

    .section i.EntryPoint, "xa", %progbits

    @ signature & mini vector table
    .word   0xDEADBEEF
    .word   EntryPoint

    @ have to reference them otherwise they dont get linked in
    .word   Boot_Vectors
    .word   ARM_Vectors
    .word   Prot_Bytes

    .thumb_func
EntryPoint:
	@Set stack pointer
	@bl SystemInit_ExtMemCtl
	LDR r0, =InitStackTop
  	MSR msp, r0
	bl VectorRelocate
    bl BootstrapCode
    bl BootEntry
    bx	lr

	.end
