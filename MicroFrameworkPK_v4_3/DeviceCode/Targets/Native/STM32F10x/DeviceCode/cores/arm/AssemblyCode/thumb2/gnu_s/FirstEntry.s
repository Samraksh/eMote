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

    .extern  VectorRelocate
    .extern  BootstrapCode
    @ .extern  Boot_Vectors         @ Boot_Vectors is obsolete.  Ignore the original warning comment.
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

    @ signature & mini vector table
    .word   0xDEADBEEF
    .word   EntryPoint

    @ have to reference them otherwise they dont get linked in
    @ .word   Boot_Vectors @ Boot_Vectors is obsolete.
    .word   ARM_Vectors
    .word   Prot_Bytes

    .thumb_func
EntryPoint:
	@Set stack pointer
	@bl SystemInit_ExtMemCtl
    LDR r0, =StackTop   @ new SYS stack pointer for a full decrementing stack. must explicitly set msp because first word of image is the magic word (NOT msp) in Samraksh builds
    MSR msp, r0         @ stack top
	bl VectorRelocate
    bl BootstrapCode
    bl BootEntry
    bx	lr

	.end
