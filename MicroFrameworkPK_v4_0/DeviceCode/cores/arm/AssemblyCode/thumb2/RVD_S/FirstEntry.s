;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copyright (c) Microsoft Corporation.  All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    EXPORT  EntryPoint


    IF HAL_REDUCESIZE = "1"
        IMPORT BootEntryLoader
    ELSE
        IMPORT  BootEntry
    ENDIF
	
    IMPORT  BootstrapCode
    IMPORT  ARM_Vectors         ; Even if we don't use this symbol, it's required by the linker to properly include the Vector trampolines.


    PRESERVE8

    ;*************************************************************************

PSR_MODE_USER       EQU     0xD0
PSR_MODE_FIQ        EQU     0xD1
PSR_MODE_IRQ        EQU     0xD2
PSR_MODE_SUPERVISOR EQU     0xD3
PSR_MODE_ABORT      EQU     0xD7
PSR_MODE_UNDEF      EQU     0xDB
PSR_MODE_SYSTEM     EQU     0xDF

; main can be use for exception
STACK_MODE_MAIN     EQU     2048
STACK_MODE_PROCESS  EQU     2048

    ;*************************************************************************

    AREA SectionForStackBottom,       DATA
StackBottom       DCD 0
    AREA SectionForStackTop,          DATA
StackTop          DCD 0
    AREA SectionForHeapBegin,         DATA
HeapBegin         DCD 0
    AREA SectionForHeapEnd,           DATA
HeapEnd           DCD 0
    AREA SectionForCustomHeapBegin,   DATA
CustomHeapBegin   DCD 0
    AREA SectionForCustomHeapEnd,     DATA
CustomHeapEnd     DCD 0


    EXPORT StackBottom
    EXPORT StackTop
    EXPORT HeapBegin
    EXPORT HeapEnd
    EXPORT CustomHeapBegin
    EXPORT CustomHeapEnd

  

    IF HAL_REDUCESIZE = "1" :LAND: TargetLocation != "RAM"
    ; -----------------------------------------------
    ; ADD BOOT MARKER HERE IF YOU NEED ONE
    ; -----------------------------------------------
    ENDIF       ;[HAL_REDUCESIZE = "1" :LAND: TargetLocation != "RAM"]

    AREA ||i.EntryPoint||, CODE, READONLY


    ENTRY

EntryPoint

    bl      BootstrapCode

    IF HAL_REDUCESIZE = "1"
        b   BootEntryLoader
    ELSE
        b   BootEntry
    ENDIF


    END
