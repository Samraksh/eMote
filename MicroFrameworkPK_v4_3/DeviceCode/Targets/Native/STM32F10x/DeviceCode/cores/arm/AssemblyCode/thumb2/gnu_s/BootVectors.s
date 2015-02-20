@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .extern  StackTop
    .extern  EntryPoint
    .extern  HARD_Breakpoint
    .extern  NMI_Handler

    .global  Boot_Vectors

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   .section BootVectors, "xa", %progbits

Boot_Vectors:
    .word SAM_STACK_TOP
    .word EntryPoint
    .word HARD_Breakpoint
    @.word NMI_Handler

    @ remaming vectors
    .zero 460
    .end
