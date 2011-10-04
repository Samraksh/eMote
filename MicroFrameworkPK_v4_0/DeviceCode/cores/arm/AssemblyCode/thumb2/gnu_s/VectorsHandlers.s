@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copyright (c) Microsoft Corporation.  All rights reserved.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .global UNDEF_SubHandler
    .global ABORTP_SubHandler
    .global ABORTD_SubHandler

    .extern UNDEF_Handler               @ void UNDEF_Handler  (unsigned int*, unsigned int, unsigned int)
    .extern ABORTP_Handler              @ void ABORTP_Handler (unsigned int*, unsigned int, unsigned int)
    .extern ABORTD_Handler              @ void ABORTD_Handler (unsigned int*, unsigned int, unsigned int)

    .global HARD_Breakpoint
    .extern HARD_Breakpoint_Handler     @ HARD_Breakpoint_Handler(unsigned int*, unsigned int, unsigned int)

    .extern StackBottom
    .extern StackOverflow               @ StackOverflow(unsigned int)



    .ifdef FIQ_SAMPLING_PROFILER
        LEAVE_IRQ_DISABLED = 1
    .endif

    .ifdef FIQ_LATENCY_PROFILER
        LEAVE_IRQ_DISABLED = 1
    .endif

.ifdef LEAVE_IRQ_DISABLED

    @ here we always leave the IRQ disabled, FIQ enabled for profiling

PSR_MODE_USER       =   0x90
PSR_MODE_FIQ        =   0x91
PSR_MODE_IRQ        =   0x92
PSR_MODE_SUPERVISOR =   0x93
PSR_MODE_ABORT      =   0x97
PSR_MODE_UNDEF      =   0x9B
PSR_MODE_SYSTEM     =   0x9F

.else

    @ here we always leave the IRQ disabled, FIQ disabled for safety

PSR_MODE_USER       =   0xD0
PSR_MODE_FIQ        =   0xD1
PSR_MODE_IRQ        =   0xD2
PSR_MODE_SUPERVISOR =   0xD3
PSR_MODE_ABORT      =   0xD7
PSR_MODE_UNDEF      =   0xDB
PSR_MODE_SYSTEM     =   0xDF

.endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ the UNDEF Instruction conditions on the ARM7TDMI are catastrophic, so there is no return, but it is supported in non-RTM builds

    .section    .text.UNDEF_SubHandler, "xa", %progbits

  .ifdef COMPILE_THUMB
	.arm
  .endif

UNDEF_SubHandler:
@ on entry, were are in UNDEF mode, without a usable stack

UNDEF_Handler_Ptr:
    .word   UNDEF_Handler

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ the ABORT conditions on the ARM7TDMI are catastrophic, so there is no return, but it is supported in non-RTM builds

    .section    .text.ABORTP_SubHandler, "xa", %progbits

  .ifdef COMPILE_THUMB
	.arm
  .endif

ABORTP_SubHandler:
    @ on entry, were are in ABORT mode, without a usable stack

ABORTP_Handler_Ptr:
    .word   ABORTP_Handler

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .section    .text.ABORTD_SubHandler, "xa", %progbits

  .ifdef COMPILE_THUMB
	.arm
  .endif
	
ABORTD_SubHandler:
    @ on entry, were are in ABORT mode, without a usable stack

ABORTD_Handler_Ptr:
    .word   ABORTD_Handler

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .section    .text.HARD_Breakpoint, "xa", %progbits

  .ifdef COMPILE_THUMB
	.arm
  .endif
	
HARD_Breakpoint:
    @ on entry, were are being called from C/C++ in system mode

HARD_Breakpoint_Handler_Ptr:
    .word   HARD_Breakpoint_Handler

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

  .ifdef COMPILE_THUMB
	.thumb
  .endif




	
	
	
