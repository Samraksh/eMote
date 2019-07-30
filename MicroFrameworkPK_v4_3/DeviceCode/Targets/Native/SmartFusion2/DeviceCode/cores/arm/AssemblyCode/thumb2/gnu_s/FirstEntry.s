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
	@ .extern  Prot_Bytes see below
	.extern  SystemInit_ExtMemCtl


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#ifdef SECURE_EMOTE
	.section SectionForUserStackBottom,       "a", %progbits
UserStackBottom:
    .word   0
    .section SectionForUserStackTop,          "a", %progbits
UserStackTop:
    .word   0
#endif
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
    .global UserStackBottom
    .global UserStackTop

    .section i.EntryPoint, "xa", %progbits

    @ signature & mini vector table
    .word   0xDEADBEEF
    .word   EntryPoint

    @ have to reference them otherwise they dont get linked in
    @ .word   Boot_Vectors @ Boot_Vectors is obsolete.
    .word   ARM_Vectors
    @ .word   Prot_Bytes I don't know what this is supposed to do...

    .thumb_func
EntryPoint:
	@Set stack pointer
	@bl SystemInit_ExtMemCtl
#ifdef SECURE_EMOTE
	LDR r0, =UserStackTop   @ new process stack pointer for user space processes in the secure emote
    MSR psp, r0         @ stack top
#endif
    LDR r0, =StackTop   @ new SYS stack pointer for a full decrementing stack. must explicitly set msp because first word of image is the magic word (NOT msp) in Samraksh builds
    MSR msp, r0         @ stack top

	bl VectorRelocate
    bl BootstrapCode
    bl BootEntry
    bx	lr

	.end
