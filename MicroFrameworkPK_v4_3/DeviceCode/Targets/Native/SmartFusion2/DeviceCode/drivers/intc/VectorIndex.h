#ifndef _SF2_VECTORINDEX_H_
#define _SF2_VECTORINDEX_H_

struct VectorIndex {
    /******  Cortex-M3 Processor Exceptions Numbers *********************************************************/
	static const UINT32 c_IRQ_INDEX_NonMaskableInt           = -14;
	static const UINT32 c_IRQ_INDEX_HardFault_IRQn           = -13;
	static const UINT32 c_IRQ_INDEX_MemoryManagementInt    	 = -12;
	static const UINT32 c_IRQ_INDEX_BusFault        		 = -11;
	static const UINT32 c_IRQ_INDEX_UsageFault          	 = -10;
	static const UINT32 c_IRQ_INDEX_DebugMonitor          	 = -4;
	static const UINT32 c_IRQ_INDEX_SysTick          = -1;
	static const UINT32 c_IRQ_INDEX_PendSV          = 82;	//Used to be -2, Modified by Mukundan for Preemptive thread
	static const UINT32 c_IRQ_INDEX_SVCall          = 83;    // Used to -5, Modified by Nived for bottom half thread

	/******  SmartFusion2 specific Interrupt Numbers *********************************************************/
	static const UINT32 c_IRQ_INDEX_WWDG          = 0;
	static const UINT32 c_IRQ_INDEX_RTC          = 1;
    static const UINT32 c_IRQ_INDEX_SPI0       = 2;
    static const UINT32 c_IRQ_INDEX_SPI1     = 3;
    static const UINT32 c_IRQ_INDEX_I2C0     = 4;
    static const UINT32 c_IRQ_INDEX_I2C0_SMBAlert    = 5;
    static const UINT32 c_IRQ_INDEX_I2C0_SMBus   = 6;
    static const UINT32 c_IRQ_INDEX_I2C1      = 7;
	static const UINT32 c_IRQ_INDEX_I2C1_SMBAlert        = 8;
	static const UINT32 c_IRQ_INDEX_I2C1_SMBus        = 9;
	static const UINT32 c_IRQ_INDEX_USART1        = 10;
    static const UINT32 c_IRQ_INDEX_USART2     = 11;
	static const UINT32 c_IRQ_INDEX_EthernetMAC       = 12;
	static const UINT32 c_IRQ_INDEX_DMA       = 13;
	static const UINT32 c_IRQ_INDEX_TIM1_CC       = 14;
	static const UINT32 c_IRQ_INDEX_TIM2      = 15;
	static const UINT32 c_IRQ_INDEX_CAN       = 16;
	static const UINT32 c_IRQ_INDEX_ENVM0       = 17;
	static const UINT32 c_IRQ_INDEX_ENVM1       = 18;
	static const UINT32 c_IRQ_INDEX_ComBlk    = 19;
	static const UINT32 c_IRQ_INDEX_USB       = 20;
	static const UINT32 c_IRQ_INDEX_USB_DMA       = 21;
	static const UINT32 c_IRQ_INDEX_PLL_Lock      = 22;
	static const UINT32 c_IRQ_INDEX_PLL_LockLost      = 23;
	static const UINT32 c_IRQ_INDEX_CommSwitchError     = 24;
	static const UINT32 c_IRQ_INDEX_CacheError     = 25;
	static const UINT32 c_IRQ_INDEX_DDR      = 26;
	static const UINT32 c_IRQ_INDEX_HPDMA_Complete     = 27;
	static const UINT32 c_IRQ_INDEX_HPDMA_Error       = 28;
	static const UINT32 c_IRQ_INDEX_ECC_Error       = 29;
	static const UINT32 c_IRQ_INDEX_MDDR_IOCalib       = 30;
	static const UINT32 c_IRQ_INDEX_FAB_PLL_Lock     = 31;
	static const UINT32 c_IRQ_INDEX_FAB_PLL_LockLost      = 32;
	static const UINT32 c_IRQ_INDEX_FIC64      = 33;
	static const UINT32 c_IRQ_INDEX_FabricIrq0      = 34;
	static const UINT32 c_IRQ_INDEX_FabricIrq1       = 35;
	static const UINT32 c_IRQ_INDEX_FabricIrq2       = 36;
	static const UINT32 c_IRQ_INDEX_FabricIrq3     = 37;
	static const UINT32 c_IRQ_INDEX_FabricIrq4      = 38;
	static const UINT32 c_IRQ_INDEX_FabricIrq5      = 39;
	static const UINT32 c_IRQ_INDEX_FabricIrq6    = 40;
	static const UINT32 c_IRQ_INDEX_FabricIrq7      = 41;
	static const UINT32 c_IRQ_INDEX_FabricIrq8     = 42;
	static const UINT32 c_IRQ_INDEX_FabricIrq9   = 43;
	static const UINT32 c_IRQ_INDEX_FabricIrq10    = 44;
	static const UINT32 c_IRQ_INDEX_FabricIrq11    = 45;
	static const UINT32 c_IRQ_INDEX_FabricIrq12      = 46;
	static const UINT32 c_IRQ_INDEX_FabricIrq13       = 47;
    static const UINT32 c_IRQ_INDEX_FabricIrq14       = 48;
    static const UINT32 c_IRQ_INDEX_FabricIrq15       = 49;
	static const UINT32 c_IRQ_INDEX_GPIO0       = 50;
	static const UINT32 c_IRQ_INDEX_GPIO1       = 51;
	static const UINT32 c_IRQ_INDEX_GPIO2       = 52;
	static const UINT32 c_IRQ_INDEX_GPIO3       = 53;
	static const UINT32 c_IRQ_INDEX_GPIO4       = 54;
	static const UINT32 c_IRQ_INDEX_GPIO5       = 55;
	static const UINT32 c_IRQ_INDEX_GPIO6       = 56;
	static const UINT32 c_IRQ_INDEX_GPIO7       = 57;
	static const UINT32 c_IRQ_INDEX_GPIO8       = 58;
	static const UINT32 c_IRQ_INDEX_GPIO9 		= 59;
	static const UINT32 c_IRQ_INDEX_GPIO10       = 60;
	static const UINT32 c_IRQ_INDEX_GPIO11       = 61;
	static const UINT32 c_IRQ_INDEX_GPIO12       = 62;
	static const UINT32 c_IRQ_INDEX_GPIO13       = 63;
	static const UINT32 c_IRQ_INDEX_GPIO14       = 64;
	static const UINT32 c_IRQ_INDEX_GPIO15       = 65;
	static const UINT32 c_IRQ_INDEX_GPIO16       = 66;
	static const UINT32 c_IRQ_INDEX_GPIO17       = 67;
	static const UINT32 c_IRQ_INDEX_GPIO18       = 68;
	static const UINT32 c_IRQ_INDEX_GPIO19     	 = 69;
	static const UINT32 c_IRQ_INDEX_GPIO20       = 70;
	static const UINT32 c_IRQ_INDEX_GPIO21       = 71;
	static const UINT32 c_IRQ_INDEX_GPIO22       = 72;
	static const UINT32 c_IRQ_INDEX_GPIO23       = 73;
	static const UINT32 c_IRQ_INDEX_GPIO24       = 74;
	static const UINT32 c_IRQ_INDEX_GPIO25       = 75;
	static const UINT32 c_IRQ_INDEX_GPIO26       = 76;
	static const UINT32 c_IRQ_INDEX_GPIO27       = 77;
	static const UINT32 c_IRQ_INDEX_GPIO28       = 78;
	static const UINT32 c_IRQ_INDEX_GPIO29       = 79;
	static const UINT32 c_IRQ_INDEX_GPIO30       = 80;
	static const UINT32 c_IRQ_INDEX_GPIO31       = 81;


};

#endif //_SF2_VECTORINDEX_H_
