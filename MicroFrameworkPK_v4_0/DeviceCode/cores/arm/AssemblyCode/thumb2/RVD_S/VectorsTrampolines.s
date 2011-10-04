;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copyright (c) Microsoft Corporation.  All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;*****************************************************************************

    IMPORT  UNDEF_SubHandler
    IMPORT  ABORTP_SubHandler
    IMPORT  ABORTD_SubHandler 
	IMPORT	EntryPoint
	IMPORT	NMI_Handler
	IMPORT	HardFault_Handler
	IMPORT	MemManage_Handler
	IMPORT	BusFault_Handler
	IMPORT	UsageFault_Handler
	IMPORT	SVC_Handler
	IMPORT	DebugMon_Handler
	IMPORT	PendSV_Handler
	IMPORT	SysTick_Handler
	IMPORT	SUPC_IrqHandler
	IMPORT	RSTC_IrqHandler
	IMPORT	RTC_IrqHandler
	IMPORT	RTT_IrqHandler
	IMPORT	WDT_IrqHandler
	IMPORT	PMC_IrqHandler
	IMPORT	EEFC_IrqHandler
	IMPORT	UART0_IrqHandler
	IMPORT	UART1_IrqHandler
	IMPORT	SMC_IrqHandler
	IMPORT	PIOA_IrqHandler
	IMPORT	PIOB_IrqHandler
	IMPORT	PIOC_IrqHandler
	IMPORT	USART0_IrqHandler
	IMPORT	USART1_IrqHandler
	IMPORT	MCI_IrqHandler
	IMPORT	TWI0_IrqHandler
	IMPORT	TWI1_IrqHandler
	IMPORT	SPI_IrqHandler
	IMPORT	SSC_IrqHandler
	IMPORT	TC0_IrqHandler
	IMPORT	TC1_IrqHandler
	IMPORT	TC2_IrqHandler
	IMPORT	TC3_IrqHandler
	IMPORT	TC4_IrqHandler
	IMPORT	TC5_IrqHandler
	IMPORT	ADC_IrqHandler
	IMPORT	DAC_IrqHandler
	IMPORT	PWM_IrqHandler
	IMPORT	CRCCU_IrqHandler
	IMPORT	ACC_IrqHandler
	IMPORT	USBD_IrqHandler
	IMPORT  IrqHandlerNotUsed

    IF :DEF: FIQ_SAMPLING_PROFILER  
    IMPORT  FIQ_SubHandler
    ENDIF

    IMPORT  IRQ_Handler      ; from stubs.cpp 

    EXPORT  ARM_Vectors

;*****************************************************************************

    AREA |.text|, CODE, READONLY

    ; ARM directive is only valid for ARM/THUMB processor, but not CORTEX 
    IF :DEF:COMPILE_ARM :LOR: :DEF:COMPILE_THUMB
    ARM
    ENDIF
    
ARM_Vectors
STACK_POINTER
	DCD	0x2000BFFC

RESET_VECTOR
    DCD EntryPoint

NMI_VECTOR
	DCD	NMI_Handler
	
HARDFAULT_VECTOR
	DCD	HardFault_Handler
	
MEMMANAGE_VECTOR
	DCD	MemManage_Handler
	
BUSFAULT_VECTOR
	DCD	BusFault_Handler

USAGE_FAULT_VECTOR
	DCD	UsageFault_Handler
	
	DCD	0x00000000
	DCD	0x00000000
	DCD	0x00000000
	DCD	0x00000000
	
SVC_VECTOR
	DCD	SVC_Handler

DEBUG_VECTOR
	DCD	DebugMon_Handler

	DCD	0x00000000
	
PENDSV_VECTOR
	DCD	PendSV_Handler
	
SYSTICK_VECTOR
	DCD	SysTick_Handler
	
IRQ_VECTOR
	DCD	SUPC_IrqHandler
	DCD	RSTC_IrqHandler
	DCD	RTC_IrqHandler
	DCD	RTT_IrqHandler
	DCD	WDT_IrqHandler
	DCD	PMC_IrqHandler
	DCD	EEFC_IrqHandler
	DCD	IrqHandlerNotUsed
	DCD	UART0_IrqHandler
	DCD	UART1_IrqHandler
	DCD	SMC_IrqHandler
	DCD	PIOA_IrqHandler
	DCD	PIOB_IrqHandler
	DCD	PIOC_IrqHandler
	DCD	USART0_IrqHandler
	DCD	USART1_IrqHandler
	DCD	IrqHandlerNotUsed
	DCD	IrqHandlerNotUsed
	DCD	MCI_IrqHandler
	DCD	TWI0_IrqHandler
	DCD	TWI1_IrqHandler
	DCD	SPI_IrqHandler
	DCD	SSC_IrqHandler
	DCD	TC0_IrqHandler
	DCD	TC1_IrqHandler
	DCD	TC2_IrqHandler
	DCD	TC3_IrqHandler
	DCD	TC4_IrqHandler
	DCD	TC5_IrqHandler
	DCD	ADC_IrqHandler
	DCD	DAC_IrqHandler
	DCD	PWM_IrqHandler
	DCD	CRCCU_IrqHandler
	DCD	ACC_IrqHandler
	DCD	USBD_IrqHandler
	DCD	IrqHandlerNotUsed

;*****************************************************************************

    END

