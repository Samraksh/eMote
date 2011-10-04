@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copyright (c) Microsoft Corporation.  All rights reserved.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


    .global  EntryPoint
	

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


STACK_MODE_MAIN    	=     2048
STACK_MODE_PROCESS  =     2048 

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
	.type  i.EntryPoint, %function

    @.arm
	
EntryPoint:

    .ifdef  HAL_REDUCESIZE
    .ifndef TARGETLOCATION_RAM
    @ -----------------------------------------------
    @ ADD BOOT MARKER HERE IF YOU NEED ONE
    @ -----------------------------------------------

    .endif
    .endif
	
	
	nop
	nop
    bl      BootstrapCode


    .ifdef HAL_REDUCESIZE      
BootEntryLoaderPointer:
        b   BootEntryLoader
   .else      
BootEntryPointer:
        bl   BootEntry
    .endif

  .ifdef COMPILE_THUMB
    .thumb
  .endif

    .end

