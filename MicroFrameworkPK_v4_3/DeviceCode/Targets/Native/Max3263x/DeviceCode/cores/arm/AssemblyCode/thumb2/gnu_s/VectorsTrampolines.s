@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.syntax unified
	.cpu cortex-m4
	.fpu softvfp
	.thumb

    .extern  StackTop
    .extern  EntryPoint
    @.extern  HARD_Breakpoint
    @.extern  HARD_Breakpoint_Handler
    .extern  NMI_Handler
    .extern  Fault_Handler_Display
    .extern  MemManage_Handler
    .extern  BusFault_Handler
    .extern  UsageFault_Handler
    .extern  SVC_Handler
    .extern  DebugMon_Handler
    .extern  PendSV_Handler
    .extern  SysTick_Handler
    .extern  CLKMAN_IRQHandler @ 0
    .extern  PWRMAN_IRQHandler
    .extern  FLC_IRQHandler
    .extern  RTC0_IRQHandler
    .extern  RTC1_IRQHandler
    .extern  RTC2_IRQHandler
    .extern  RTC3_IRQHandler
    .extern  PMU_IRQHandler
    .extern  USB_IRQHandler
    .extern  AES_IRQHandler
    .extern  MAA_IRQHandler @10
    .extern  WDT0_IRQHandler
    .extern  WDT0_P_IRQHandler
    .extern  WDT1_IRQHandler
    .extern  WDT1_P_IRQHandler
    .extern  GPIO_P0_IRQHandler
    .extern  GPIO_P1_IRQHandler
    .extern  GPIO_P2_IRQHandler
    .extern  GPIO_P3_IRQHandler
    .extern  GPIO_P4_IRQHandler
    .extern  GPIO_P5_IRQHandler @20
    .extern  GPIO_P6_IRQHandler
	.extern  TMR0_0_IRQHandler
    .extern  TMR0_1_IRQHandler
    .extern  TMR1_0_IRQHandler
    .extern  TMR1_1_IRQHandler
    .extern  TMR2_0_IRQHandler
    .extern  TMR2_1_IRQHandler
    .extern  TMR3_0_IRQHandler
    .extern  TMR3_1_IRQHandler
    .extern  TMR4_0_IRQHandler @30
    .extern  TMR4_1_IRQHandler
    .extern  TMR5_0_IRQHandler
    .extern  TMR5_1_IRQHandler
    .extern  UART0_IRQHandler
    .extern  UART1_IRQHandler
    .extern  UART2_IRQHandler
    .extern  UART3_IRQHandler
    .extern  PT_IRQHandler
    .extern  I2CM0_IRQHandler
    .extern  I2CM1_IRQHandler @40
    .extern  I2CM2_IRQHandler
    .extern  I2CS_IRQHandler
    .extern  SPIM0_IRQHandler
    .extern  SPIM1_IRQHandler
    .extern  SPIM2_IRQHandler
    .extern  SPIB_IRQHandler
    .extern  OWM_IRQHandler
    .extern  AFE_IRQHandler
    .extern  SPIS_IRQHandler
    .extern  GPIO_P7_IRQHandler
    .extern  GPIO_P8_IRQHandler @51

    .global  ARM_Vectors
    .global  Default_Handler

	.equ  BootRAM,        0xF1E0F85F

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   .section VectorsTrampolines, "xa", %progbits

ARM_Vectors:
  .word  0x2000C000          @ Traditionally the Cortex-M MSP initial value is the first word of an image, but TinyBooter wants the value stored here to match the magic word stored in Tinybooter_ProgramWordCheck().
  .word  EntryPoint          @ reset vector
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  .word  CLKMAN_IRQHandler @ 0
  .word  PWRMAN_IRQHandler
  .word  FLC_IRQHandler
  .word  RTC0_IRQHandler
  .word  RTC1_IRQHandler
  .word  RTC2_IRQHandler
  .word  RTC3_IRQHandler
  .word  PMU_IRQHandler
  .word  USB_IRQHandler
  .word  AES_IRQHandler
  .word  MAA_IRQHandler @10
  .word  WDT0_IRQHandler
  .word  WDT0_P_IRQHandler
  .word  WDT1_IRQHandler
  .word  WDT1_P_IRQHandler
  .word  GPIO_P0_IRQHandler
  .word  GPIO_P1_IRQHandler
  .word  GPIO_P2_IRQHandler
  .word  GPIO_P3_IRQHandler
  .word  GPIO_P4_IRQHandler
  .word  GPIO_P5_IRQHandler @20
  .word  GPIO_P6_IRQHandler
  .word  TMR0_0_IRQHandler
  .word  TMR0_1_IRQHandler
  .word  TMR1_0_IRQHandler
  .word  TMR1_1_IRQHandler
  .word  TMR2_0_IRQHandler
  .word  TMR2_1_IRQHandler
  .word  TMR3_0_IRQHandler
  .word  TMR3_1_IRQHandler
  .word  TMR4_0_IRQHandler @30
  .word  TMR4_1_IRQHandler
  .word  TMR5_0_IRQHandler
  .word  TMR5_1_IRQHandler
  .word  UART0_IRQHandler
  .word  UART1_IRQHandler
  .word  UART2_IRQHandler
  .word  UART3_IRQHandler
  .word  PT_IRQHandler
  .word  I2CM0_IRQHandler
  .word  I2CM1_IRQHandler @40
  .word  I2CM2_IRQHandler
  .word  I2CS_IRQHandler
  .word  SPIM0_IRQHandler
  .word  SPIM1_IRQHandler
  .word  SPIM2_IRQHandler
  .word  SPIB_IRQHandler
  .word  OWM_IRQHandler
  .word  AFE_IRQHandler
  .word  SPIS_IRQHandler
  .word  GPIO_P7_IRQHandler
  .word  GPIO_P8_IRQHandler @51
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  @.word  BootRAM       /* @0x1E0. This is for boot in RAM mode for   STM32F10x High Density devices. */
/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  .weak  NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

/*
	.thumb_func
  HardFault_Handler:
	add sp,sp, #16
	push {r0-r11}
	mov r0,sp
	mrs r1, IPSR
	b Fault_Handler_Display
*/

  @.weak  HardFault_Handler
  @.thumb_set HardFault_Handler,Default_Handler

  /* For some reason weak aliases are not working here*/


  .weak  MemManage_Handler
  .thumb_set MemManage_Handler,Default_Handler

  .weak  BusFault_Handler
  .thumb_set BusFault_Handler,Default_Handler

  .weak  UsageFault_Handler
  .thumb_set UsageFault_Handler,Default_Handler

/*
  .weak  SVC_Handler
  .thumb_set SVC_Handler,Default_Handler
*/

  .weak  DebugMon_Handler
  .thumb_set DebugMon_Handler,Default_Handler

/*
  .weak  PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak  SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler
*/
  .weak  WWDG_IRQHandler
  .thumb_set WWDG_IRQHandler,Default_Handler
/*
  .weak  PVD_IRQHandler
  .thumb_set PVD_IRQHandler,Default_Handler

  .weak  TAMPER_IRQHandler
  .thumb_set TAMPER_IRQHandler,Default_Handler

  .weak  RTC_IRQHandler
  .thumb_set RTC_IRQHandler,Default_Handler

  .weak  FLASH_IRQHandler
  .thumb_set FLASH_IRQHandler,Default_Handler

  .weak  RCC_IRQHandler
  .thumb_set RCC_IRQHandler,Default_Handler

  .weak  EXTI0_IRQHandler
  .thumb_set EXTI0_IRQHandler,Default_Handler

  .weak  EXTI1_IRQHandler
  .thumb_set EXTI1_IRQHandler,Default_Handler

  .weak  EXTI2_IRQHandler
  .thumb_set EXTI2_IRQHandler,Default_Handler

  .weak  EXTI3_IRQHandler
  .thumb_set EXTI3_IRQHandler,Default_Handler

  .weak  EXTI4_IRQHandler
  .thumb_set EXTI4_IRQHandler,Default_Handler

  .weak  DMA1_Channel1_IRQHandler
  .thumb_set DMA1_Channel1_IRQHandler,Default_Handler

  .weak  DMA1_Channel2_IRQHandler
  .thumb_set DMA1_Channel2_IRQHandler,Default_Handler

  .weak  DMA1_Channel3_IRQHandler
  .thumb_set DMA1_Channel3_IRQHandler,Default_Handler

  .weak  DMA1_Channel4_IRQHandler
  .thumb_set DMA1_Channel4_IRQHandler,Default_Handler

  .weak  DMA1_Channel5_IRQHandler
  .thumb_set DMA1_Channel5_IRQHandler,Default_Handler

  .weak  DMA1_Channel6_IRQHandler
  .thumb_set DMA1_Channel6_IRQHandler,Default_Handler

  .weak  DMA1_Channel7_IRQHandler
  .thumb_set DMA1_Channel7_IRQHandler,Default_Handler

  .weak  ADC1_2_IRQHandler
  .thumb_set ADC1_2_IRQHandler,Default_Handler

  .weak  USB_HP_CAN1_TX_IRQHandler
  .thumb_set USB_HP_CAN1_TX_IRQHandler,Default_Handler

  .weak  USB_LP_CAN1_RX0_IRQHandler
  .thumb_set USB_LP_CAN1_RX0_IRQHandler,Default_Handler

  .weak  CAN1_RX1_IRQHandler
  .thumb_set CAN1_RX1_IRQHandler,Default_Handler

  .weak  CAN1_SCE_IRQHandler
  .thumb_set CAN1_SCE_IRQHandler,Default_Handler

  .weak  EXTI9_5_IRQHandler
  .thumb_set EXTI9_5_IRQHandler,Default_Handler

  .weak  TIM1_BRK_IRQHandler
  .thumb_set TIM1_BRK_IRQHandler,Default_Handler

  .weak  TIM1_UP_IRQHandler
  .thumb_set TIM1_UP_IRQHandler,Default_Handler

  .weak  TIM1_TRG_COM_IRQHandler
  .thumb_set TIM1_TRG_COM_IRQHandler,Default_Handler

  .weak  TIM1_CC_IRQHandler
  .thumb_set TIM1_CC_IRQHandler,Default_Handler

  .weak  TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler,Default_Handler

  .weak  TIM3_IRQHandler
  .thumb_set TIM3_IRQHandler,Default_Handler

  .weak  TIM4_IRQHandler
  .thumb_set TIM4_IRQHandler,Default_Handler

  .weak  I2C1_EV_IRQHandler
  .thumb_set I2C1_EV_IRQHandler,Default_Handler

  .weak  I2C1_ER_IRQHandler
  .thumb_set I2C1_ER_IRQHandler,Default_Handler

  .weak  I2C2_EV_IRQHandler
  .thumb_set I2C2_EV_IRQHandler,Default_Handler

  .weak  I2C2_ER_IRQHandler
  .thumb_set I2C2_ER_IRQHandler,Default_Handler

  .weak  SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Default_Handler

  .weak  SPI2_IRQHandler
  .thumb_set SPI2_IRQHandler,Default_Handler

  .weak  USART1_IRQHandler
  .thumb_set USART1_IRQHandler,Default_Handler

  .weak  USART2_IRQHandler
  .thumb_set USART2_IRQHandler,Default_Handler

  .weak  USART3_IRQHandler
  .thumb_set USART3_IRQHandler,Default_Handler

  .weak  EXTI15_10_IRQHandler
  .thumb_set EXTI15_10_IRQHandler,Default_Handler

  .weak  RTCAlarm_IRQHandler
  .thumb_set RTCAlarm_IRQHandler,Default_Handler

  .weak  USBWakeUp_IRQHandler
  .thumb_set USBWakeUp_IRQHandler,Default_Handler

  .weak  TIM8_BRK_IRQHandler
  .thumb_set TIM8_BRK_IRQHandler,Default_Handler
*/
  .weak  TIM8_UP_IRQHandler
  .thumb_set TIM8_UP_IRQHandler,Default_Handler

  .weak  TIM8_TRG_COM_IRQHandler
  .thumb_set TIM8_TRG_COM_IRQHandler,Default_Handler
/*
  .weak  TIM8_CC_IRQHandler
  .thumb_set TIM8_CC_IRQHandler,Default_Handler

  .weak  ADC3_IRQHandler
  .thumb_set ADC3_IRQHandler,Default_Handler

  .weak  FSMC_IRQHandler
  .thumb_set FSMC_IRQHandler,Default_Handler

  .weak  SDIO_IRQHandler
  .thumb_set SDIO_IRQHandler,Default_Handler

  .weak  TIM5_IRQHandler
  .thumb_set TIM5_IRQHandler,Default_Handler

  .weak  SPI3_IRQHandler
  .thumb_set SPI3_IRQHandler,Default_Handler

  .weak  UART4_IRQHandler
  .thumb_set UART4_IRQHandler,Default_Handler

  .weak  UART5_IRQHandler
  .thumb_set UART5_IRQHandler,Default_Handler

  .weak  TIM6_IRQHandler
  .thumb_set TIM6_IRQHandler,Default_Handler

  .weak  TIM7_IRQHandler
  .thumb_set TIM7_IRQHandler,Default_Handler

  .weak  DMA2_Channel1_IRQHandler
  .thumb_set DMA2_Channel1_IRQHandler,Default_Handler

  .weak  DMA2_Channel2_IRQHandler
  .thumb_set DMA2_Channel2_IRQHandler,Default_Handler

  .weak  DMA2_Channel3_IRQHandler
  .thumb_set DMA2_Channel3_IRQHandler,Default_Handler

  .weak  DMA2_Channel4_5_IRQHandler
  .thumb_set DMA2_Channel4_5_IRQHandler,Default_Handler

  .weak	SystemInit_ExtMemCtl
  .thumb_set SystemInit_ExtMemCtl,SystemInit_ExtMemCtl_Dummy
	*/


    
    

