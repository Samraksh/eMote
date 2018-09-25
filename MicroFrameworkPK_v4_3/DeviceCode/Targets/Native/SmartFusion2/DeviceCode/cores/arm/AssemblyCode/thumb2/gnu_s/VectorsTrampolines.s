@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.syntax unified
	.cpu cortex-m3
	.fpu softvfp
	.thumb

    .extern  StackTop
    .extern  EntryPoint
    .extern  NMI_Handler
    .extern  Fault_Handler_Display
    .extern  MemManage_Handler
    .extern  BusFault_Handler
    .extern  UsageFault_Handler
    .extern  SVC_Handler
    .extern  DebugMon_Handler
    .extern  PendSV_Handler
    .extern  SysTick_Handler
    .extern  RTC_IRQHandler
	.extern  SPI0_IRQHandler
	.extern  SPI1_IRQHandler
	.extern  I2C0_IRQHandler
	.extern  I2C0_SMBAlert_IRQHandler
	.extern  I2C0_SMBus_IRQHandler
	.extern  I2C1_IRQHandler
	.extern  I2C1_SMBAlert_IRQHandler
	.extern  I2C1_SMBus_IRQHandler
	.extern  UART0_IRQHandler
	.extern  UART1_IRQHandler
	.extern  EthernetMAC_IRQHandler
	.extern  DMA_IRQHandler
	.extern  TIM1_CC_IRQHandler
	.extern  TIM2_IRQHandler
	.extern  CAN_IRQHandler
	.extern  EVM0_IRQHandler
	.extern  EVM1_IRQHandler
	.extern  ComBlk_IRQHandler
	.extern  USB_IRQHandler
	.extern  USB_DMA_IRQHandler
	.extern  PLL_Lock_IRQHandler
	.extern  PLL_LockLost_IRQHandler
	.extern  CommSwitchError_IRQHandler
	.extern  CacheError_IRQHandler
	.extern  DDR_IRQHandler
	.extern  HPDMA_Error_IRQHandler
	.extern  MDDR_IOCalib_IRQHandler
	.extern  FAB_PLL_Lock_IRQHandler
	.extern  FAB_PLL_LockLost_IRQHandler
	.extern  FIC64_IRQHandler
	.extern  FabricIrq0_IRQHandler
	.extern  FabricIrq1_IRQHandler
	.extern  FabricIrq2_IRQHandler
	.extern  FabricIrq3_IRQHandler
	.extern  FabricIrq4_IRQHandler
	.extern  FabricIrq5_IRQHandler
	.extern  FabricIrq6_IRQHandler
	.extern  FabricIrq7_IRQHandler
	.extern  FabricIrq8_IRQHandler
	.extern  FabricIrq9_IRQHandler
	.extern  FabricIrq10_IRQHandler
	.extern  FabricIrq11_IRQHandler
	.extern  FabricIrq12_IRQHandler
	.extern  FabricIrq13_IRQHandler
	.extern  FabricIrq14_IRQHandler
	.extern  FabricIrq15_IRQHandler
	.extern  GPIO0_IRQHandler
	.extern  GPIO1_IRQHandler
	.extern  GPIO2_IRQHandler
	.extern  GPIO3_IRQHandler
	.extern  GPIO4_IRQHandler
	.extern  GPIO5_IRQHandler
	.extern  GPIO6_IRQHandler
	.extern  GPIO7_IRQHandler
	.extern  GPIO8_IRQHandler
	.extern  GPIO9_IRQHandler
	.extern  GPIO10_IRQHandler
	.extern  GPIO11_IRQHandler
	.extern  GPIO12_IRQHandler
	.extern  GPIO13_IRQHandler
	.extern  GPIO14_IRQHandler
	.extern  GPIO15_IRQHandler
	.extern  GPIO16_IRQHandler
	.extern  GPIO17_IRQHandler
	.extern  GPIO18_IRQHandler
	.extern  GPIO19_IRQHandler
	.extern  GPIO20_IRQHandler
	.extern  GPIO21_IRQHandler
	.extern  GPIO22_IRQHandler
	.extern  GPIO23_IRQHandler
	.extern  GPIO24_IRQHandler
	.extern  GPIO25_IRQHandler
	.extern  GPIO26_IRQHandler
	.extern  GPIO27_IRQHandler
	.extern  GPIO28_IRQHandler
	.extern  GPIO29_IRQHandler
	.extern  GPIO30_IRQHandler
	.extern  GPIO31_IRQHandler

    .global  ARM_Vectors
    .global  Default_Handler

	.equ  BootRAM,        0xF1E0F85F

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   .section VectorsTrampolines, "xa", %progbits

ARM_Vectors:
  .word  0x2000C000          @ Traditionally the Cortex-M3 MSP initial value is the first word of an image, but TinyBooter wants the value stored here to match the magic word stored in Tinybooter_ProgramWordCheck().
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
  .word  WWDG_IRQHandler
  .word  RTC_IRQHandler
  .word  SPI0_IRQHandler
  .word  SPI1_IRQHandler
  .word  I2C0_IRQHandler
  .word  I2C0_SMBAlert_IRQHandler
  .word  I2C0_SMBus_IRQHandler
  .word  I2C1_IRQHandler
  .word  I2C1_SMBAlert_IRQHandler
  .word  I2C1_SMBus_IRQHandler
  .word  UART0_IRQHandler
  .word  UART1_IRQHandler
  .word  EthernetMAC_IRQHandler
  .word  DMA_IRQHandler
  .word  TIM1_CC_IRQHandler
  .word  TIM2_IRQHandler
  .word  CAN_IRQHandler
  .word  EVM0_IRQHandler
  .word  EVM1_IRQHandler
  .word  ComBlk_IRQHandler
  .word  USB_IRQHandler
  .word  USB_DMA_IRQHandler
  .word  PLL_Lock_IRQHandler
  .word  PLL_LockLost_IRQHandler
  .word  CommSwitchError_IRQHandler
  .word  CacheError_IRQHandler
  .word  DDR_IRQHandler
  .word  HPDMA_Error_IRQHandler
  .word  HPDMA_Error_IRQHandler
  .word  0
  .word  MDDR_IOCalib_IRQHandler
  .word  FAB_PLL_Lock_IRQHandler
  .word  FAB_PLL_LockLost_IRQHandler
  .word  FIC64_IRQHandler
  .word  FabricIrq0_IRQHandler
  .word  FabricIrq1_IRQHandler
  .word  FabricIrq2_IRQHandler
  .word  FabricIrq3_IRQHandler
  .word  FabricIrq4_IRQHandler
  .word  FabricIrq5_IRQHandler
  .word  FabricIrq6_IRQHandler
  .word  FabricIrq7_IRQHandler
  .word  FabricIrq8_IRQHandler
  .word  FabricIrq9_IRQHandler
  .word  FabricIrq10_IRQHandler
  .word  FabricIrq11_IRQHandler
  .word  FabricIrq12_IRQHandler
  .word  FabricIrq13_IRQHandler
  .word  FabricIrq14_IRQHandler
  .word  FabricIrq15_IRQHandler
  .word  GPIO0_IRQHandler
  .word  GPIO1_IRQHandler
  .word  GPIO2_IRQHandler
  .word  GPIO3_IRQHandler
  .word  GPIO4_IRQHandler
  .word  GPIO5_IRQHandler
  .word  GPIO6_IRQHandler
  .word  GPIO7_IRQHandler
  .word  GPIO8_IRQHandler
  .word  GPIO9_IRQHandler
  .word  GPIO10_IRQHandler
  .word  GPIO11_IRQHandler
  .word  GPIO12_IRQHandler
  .word  GPIO13_IRQHandler
  .word  GPIO14_IRQHandler
  .word  GPIO15_IRQHandler
  .word  GPIO16_IRQHandler
  .word  GPIO17_IRQHandler
  .word  GPIO18_IRQHandler
  .word  GPIO19_IRQHandler
  .word  GPIO20_IRQHandler
  .word  GPIO21_IRQHandler
  .word  GPIO22_IRQHandler
  .word  GPIO23_IRQHandler
  .word  GPIO24_IRQHandler
  .word  GPIO25_IRQHandler
  .word  GPIO26_IRQHandler
  .word  GPIO27_IRQHandler
  .word  GPIO28_IRQHandler
  .word  GPIO29_IRQHandler
  .word  GPIO30_IRQHandler
  .word  GPIO31_IRQHandler
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
  .word  BootRAM       /* @0x1E0. This is for boot in RAM mode for
                         STM32F10x High Density devices. */
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

  .weak  UART0_IRQHandler
  .thumb_set UART0_IRQHandler,Default_Handler

  .weak  UART1_IRQHandler
  .thumb_set UART1_IRQHandler,Default_Handler

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


    
    


