/********************* (C) COPYRIGHT 2007 RAISONANCE S.A.S. ****************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
// not yet optimized  NVIC_Configuration & CAN_Configuration
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"
#include    "system_stm32f10x.h"
#include    "stm32f10x_can.h"

/* -------- Used Global variables -----------------------------------------------------*/

/* -------- Create Global variables ---------------------------------------------------*/
void        Configuration_RCC(void);                 // The name says it all
void        Configuration_GPIO(void);                // The name says it all
void        Configuration_NVIC(void);                // The name says it all
void        Configuration_SysTick(void);             // The name says it all
void        Configuration_ADC(void);                 // The name says it all
void        Configuration_SPI(void);                 // The name says it all
void        Configuration_CAN(void);                 // The name says it all

/* -------- Private define ------------------------------------------------------------*/
// CAN BAUD RATE SELECTION
//#define   _125KB
//#define   _250KB
#define  _500KB
//#define   _1000KB

/* -------- Private variables ---------------------------------------------------------*/

/* -------- Prototyp local ------------------------------------------------------------*/

/* -------- Code ----------------------------------------------------------------------*/
void Configuration_SysTick(void)
{
   /* Setup SysTick Timer for 1 msec interrupts  */
   if (SysTick_Config(SystemFrequency / 1000))
   {
      /* Capture error */
      while (1);
   }
}
void Configuration_RCC(void)
{
   // Peripheral Clock enable register
   //-------------------------------------------------------------------------------------------------------------------//
   //                                          .----- FLITFEN FLITF clock enable
   //                                          | .--- SRAMEN SRAM interface clock enable
   //                                          | | .- DMAEN DMA clock enable
   //                                          | | |
   RCC->AHBENR  = 0b00000000000000000000000000010100;
   //-------------------------------------------------------------------------------------------------------------------//
   //                 .------------------------------ DAC Power interface clock enable
   //                 |.----------------------------- PWR  Power interface clock enable
   //                 ||.---------------------------- BKP  Backup interface clock enable
   //                 ||| .-------------------------- CAN clock enable
   //                 ||| | .------------------------ USB clock enable
   //                 ||| | |.----------------------- I2C 2 clock enable
   //                 ||| | ||.---------------------- I2C 1 clock enable
   //                 ||| | |||.--------------------- USART 5 clock enable
   //                 ||| | ||||.-------------------- USART 4 clock enable
   //                 ||| | |||||.------------------- USART 3 clock enable
   //                 ||| | ||||||.------------------ USART 2 clock enable
   //                 ||| | ||||||| .---------------- SPI3EN SPI 3 clock enable
   //                 ||| | ||||||| |.--------------- SPI2EN SPI 2 clock enable
   //                 ||| | ||||||| ||  .------------ WWDGEN Window Watchdog clock enable
   //                 ||| | ||||||| ||  |     .------ TIM4EN Timer 7 clock enable
   //                 ||| | ||||||| ||  |     |.----- TIM4EN Timer 6 clock enable
   //                 ||| | ||||||| ||  |     ||.---- TIM4EN Timer 5 clock enable
   //                 ||| | ||||||| ||  |     |||.--- TIM4EN Timer 4 clock enable
   //                 ||| | ||||||| ||  |     ||||.-- TIM3EN Timer 3 clock enable
   //                 ||| | ||||||| ||  |     |||||.- TIM2EN Timer 2 clock enable
   //                 ||| | ||||||| ||  |     ||||||
   RCC->APB1ENR = 0b00000010000000000100000000000000;
   //-------------------------------------------------------------------------------------------------------------------//
   //                                .--------------- USART1EN USART1 clock enable
   //                                | .------------- SPI1EN SPI 1 clock enable
   //                                | |.------------ TIM1EN TIM1 Timer clock enable
   //                                | ||.----------- ADC2EN ADC 2 interface clock enable
   //                                | |||.---------- ADC1EN ADC 1 interface clock enable
   //                                | ||||  .------- IOPEEN I/O port E clock enable
   //                                | ||||  |.------ IOPDEN I/O port D clock enable
   //                                | ||||  ||.----- IOPCEN I/O port C clock enable
   //                                | ||||  |||.---- IOPBEN I/O port B clock enable
   //                                | ||||  ||||.--- IOPAEN I/O port A clock enable
   //                                | ||||  ||||| .- AFIOEN Alternate Function I/O clock enable
   //                                | ||||  ||||| |
   RCC->APB2ENR = 0b00000000000000000001001001111101;
   //-------------------------------------------------------------------------------------------------------------------//
}
void Configuration_GPIO(void)
{
   //-------------------------------------------------------------------------------------------------------------------//
   // Port configuration register
   // Value   Pin Mode
   // 1 = Output Push-pull  10Mhz  |  9 = Alternate Function Output Push-pull  10Mhz  | 0 = Input Analog input
   // 2 = Output Push-pull   2Mhz  |  A = Alternate Function Output Push-pull   2Mhz  | 4 = Input floating
   // 3 = Output Push-pull  50Mhz  |  B = Alternate Function Output Push-pull  50Mhz  | 8 = Input pull-down Set ODR 0
   // 5 = Output Open-drain 10Mhz  |  D = Alternate Function Output Open-drain 10Mhz  | 8 = Input pull-up   Set ODR 1
   // 6 = Output Open-drain  2Mhz  |  E = Alternate Function Output Open-drain  2Mhz  |
   // 7 = Output Open-drain 50Mhz  |  F = Alternate Function Output Open-drain 50Mhz  |
   //-------------------------------------------------------------------------------------------------------------------//
   // PA 0 = CX_USART_CTS  = to JP2             = B Output AF
   // PA 1 = CX_USART_RTS  = to JP2             = 4 Input
   // PA 2 = CX_USART_TX   = to JP2             = B Output AF
   // PA 3 = CX_USART_RX   = to JP2             = 4 Input
   // PA 4 = CX_USART_CK   = to JP2             = B Output AF
   // PA 5 = SPI_SCK       = to 3 axis Sensor   = B Alternate Function Output Push-pull  50Mhz
   // PA 6 = SPI_MISO      = to 3 axis Sensor   = B Alternate Function Output Push-pull  50Mhz
   // PA 7 = SPI_MOSI      = to 3 axis Sensor   = B Alternate Function Output Push-pull  50Mhz
   //-------------------------------------------------------------------------------------------------------------------//
   // PA 8 = PBUTTON       = to JOYSTICK        = 8 Input pull-down Set ODR 0
   // PA 9 = IRDA TX       = to IRDA            = B Output AF
   // PA10 = IRDA RX       = to IRDA            = 4 Input
   // PA11 = USB-DM        = to User USB        = 4 Input
   // PA12 = USB-DP        = to User USB        = 8 Input
   // PA13 = SWDIO         = Debug              = 8 Input
   // PA14 = SWDCLK        = Debug              = 8 Input
   // PA15 = AUDIO_I2S3_WS = to Audio           = 8 Input
   //-------------------------------------------------------------------------------------------------------------------//
   //             PPPPPPPP                                          PPPPPPPPPPPPPPPP
   //             AAAAAAAA                 PPPPPPPP                 AAAAAAAAAAAAAAAA
   //             111111                   AAAAAAAA                 111111
   //             54321098                 76543210                 5432109876543210
   GPIOA->CRH = 0x888844B8; GPIOA->CRL = 0xBBBB4B4B; GPIOA->ODR = 0b1010000000000000; // Port configuration register High | Low | Port output data register
   //-------------------------------------------------------------------------------------------------------------------//
   // PB 0 = CX_ADC_TIM     = to JP2       = 0 Input Analog input
   // PB 1 = VBAT/2         = to battery   = 0 Input Analog input
   // PB 2 = VCC            = to VCC2V8    = 4 Input floating
   // PB 3 = AUDIO_I2S3_CK  = to Audio     = 4 Input floating
   // PB 4 = NC             =              = 8 Input pull-up   Set ODR 1
   // PB 5 = AUDIO_I2S3_SD  = to Audio     = 4 Input floating
   // PB 6 = CX_I2CSCL      = to JP2       = 4 Input floating
   // PB 7 = CX_I2CSDA      = to JP2       = 4 Input floating
   //-------------------------------------------------------------------------------------------------------------------//
   // PB 8 = BACKLIGHT_/EN  = to Display   = 3 Output Push-pull  50Mhz
   // PB 9 = nc             =              = 8 Input pull-up   Set ODR 1
   // PB10 = AUDIO_SCL      = to Audio     = 4 Input floating
   // PB11 = AUDIO_SDA      = to Audio     = 4 Input floating
   // PB12 = AUDIO_I2S2_WS  = to Audio     = 4 Input floating
   // PB13 = AUDIO_I2S2_SCK = to JP2&Audio = 4 Input floating
   // PB14 = AUDIO_SPI_MISO = to JP2       = 4 Input floating
   // PB15 = AUDIO_I2S2_SD  = to JP2&Audio = 4 Input floating
   //-------------------------------------------------------------------------------------------------------------------//
   //             PPPPPPPP                                          PPPPPPPPPPPPPPPP
   //             BBBBBBBB                 PPPPPPPP                 BBBBBBBBBBBBBBBB
   //             111111                   BBBBBBBB                 111111
   //             54321098                 76543210                 5432109876543210
   GPIOB->CRH = 0xBBB44443; GPIOB->CRL = 0x43444400; GPIOB->ODR = 0b0000001000010000; // Port configuration register High | Low | Port output data register
   //-------------------------------------------------------------------------------------------------------------------//
   // PC 0 = TOUCH_L      = to TOUCHPAD          = 0 Input Analog input
   // PC 1 = TOUCH_R(GND) = to GND               = 0 Input Analog input
   // PC 2 = TOUCH_U      = to TOUCHPAD          = 0 Input Analog input
   // PC 3 = TOUCH_D      = to TOUCHPAD          = 0 Input Analog input
   // PC 4 = CX_ADC1      = to JP2               = 4 Input floating
   // PC 5 = CX_ADC2      = to JP2               = 4 Input floating
   // PC 6 = STATUS1      = to BATTERY UNIT      = 4 Input floating
   // PC 7 = STATUS2      = to BATTERY UNIT      = 4 Input floating
   //-------------------------------------------------------------------------------------------------------------------//
   // PC 8 = SDIO_D0      = to SD CARD CONNECTOR = 4 Input floating
   // PC 9 = SDIO_D1      = to SD CARD CONNECTOR = 4 Input floating
   // PC10 = SDIO_D2      = to SD CARD CONNECTOR = 4 Input floating
   // PC11 = SDIO_D3      = to SD CARD CONNECTOR = 4 Input floating
   // PC12 = SDIO_CK      = to SD CARD CONNECTOR = 4 Input floating
   // PC13 = SHUTDOWN     = to "SHUTDOWN"        = 3 Output Push-pull  50Mhz
   // PC14 = OSC32_IN     = to 32Khz quartz      = 4 Input floating
   // PC15 = OSC32_OUT    = to 32Khz quartz      = 4 Input floating
   //-------------------------------------------------------------------------------------------------------------------//
   //             PPPPPPPP                                          PPPPPPPPPPPPPPPP
   //             CCCCCCCC                 PPPPPPPP                 CCCCCCCCCCCCCCCC
   //             111111                   CCCCCCCC                 111111
   //             54321098                 76543210                 5432109876543210
   GPIOC->CRH = 0x44344444; GPIOC->CRL = 0x44430000; GPIOC->ODR = 0b0000000000000000; // Port configuration register High | Low | Port output data register
   //-------------------------------------------------------------------------------------------------------------------//
   // PD 0 = CAN_RX      = to CAN               = 8 Input pull-up   Set ODR 1
   // PD 1 = CAN_TX      = to CAN               = B Alternate Function Output Push-pull  50Mhz
   // PD 2 = SDIO_CMD    = to SD CARD CONNECTOR = 4 Input floating
   // PD 3 = USB_DISCONN = to User USB DISCONN  = 4 Input floating
   // PD 4 = LCD_/RD     = to LCD DISPLAY       = 3 Output Push-pull  50Mhz
   // PD 5 = LCD_/WR     = to LCD DISPLAY       = 3 Output Push-pull  50Mhz
   // PD 6 = LCD_/RST    = to LCD DISPLAY       = 3 Output Push-pull  50Mhz
   // PD 7 = LCD_/CS     = to LCD DISPLAY       = 3 Output Push-pull  50Mhz
   //-------------------------------------------------------------------------------------------------------------------//
   // PD 8 = IRDA_CS     = to IRDA              = 4 Input floating
   // PD 9 = NC          =                      = 4 Input floating
   // PD10 = NC          =                      = 4 Input floating
   // PD11 = LCD_RS      = to LCD DISPLAY       = 3 Output Push-pull  50Mhz
   // PD12 = NC          =                      = 4 Input floating
   // PD13 = NC          =                      = 4 Input floating
   // PD14 = NC          =                      = 4 Input floating
   // PD15 = NC          =                      = 4 Input floating
   //-------------------------------------------------------------------------------------------------------------------//
   //             PPPPPPPP                                          PPPPPPPPPPPPPPPP
   //             DDDDDDDD                 PPPPPPPP                 DDDDDDDDDDDDDDDD
   //             111111                   DDDDDDDD                 111111
   //             54321098                 76543210                 5432109876543210
   GPIOD->CRH = 0x44443444; GPIOD->CRL = 0x333344B8; GPIOD->ODR = 0b0000000000000001; // Port configuration register High | Low | Port output data register
   //-------------------------------------------------------------------------------------------------------------------//
   // PE 0 = LED0       = to LED           = 3 Output Push-pull  50Mhz
   // PE 1 = LED1       = to LED           = 3 Output Push-pull  50Mhz
   // PE 2 = CS_MEMS    = to 3 axis Sensor = 3 Output Push-pull  50Mhz
   // PE 3 = JOYSTICK_L = to JOYSTICK      = 8 Input pull-down Set ODR 0
   // PE 4 = JOYSTICK_R = to JOYSTICK      = 8 Input pull-down Set ODR 0
   // PE 5 = JOYSTICK_U = to JOYSTICK      = 8 Input pull-down Set ODR 0
   // PE 6 = JOYSTICK_D = to JOYSTICK      = 8 Input pull-down Set ODR 0
   // PE 7 = LCD_D0     = to LCD DISPLAY   = 3 Output Push-pull  50Mhz
   //-------------------------------------------------------------------------------------------------------------------//
   // PE 8 = LCD_D1     = to LCD DISPLAY   = 3 Output Push-pull  50Mhz
   // PE 9 = LCD_D2     = to LCD DISPLAY   = 3 Output Push-pull  50Mhz
   // PE10 = LCD_D3     = to LCD DISPLAY   = 3 Output Push-pull  50Mhz
   // PE11 = LCD_D4     = to LCD DISPLAY   = 3 Output Push-pull  50Mhz
   // PE12 = LCD_D5     = to LCD DISPLAY   = 3 Output Push-pull  50Mhz
   // PE13 = LCD_D6     = to LCD DISPLAY   = 3 Output Push-pull  50Mhz
   // PE14 = LCD_D7     = to LCD DISPLAY   = 3 Output Push-pull  50Mhz
   // PE15 = NC         =                  = 4 Input floating
   //-------------------------------------------------------------------------------------------------------------------//
   //             PPPPPPPP                                          PPPPPPPPPPPPPPPP
   //             EEEEEEEE                 PPPPPPPP                 EEEEEEEEEEEEEEEE
   //             111111                   EEEEEEEE                 111111
   //             54321098                 76543210                 5432109876543210
   GPIOE->CRH = 0x43333333; GPIOE->CRL = 0x38888333; GPIOE->ODR = 0b0000000000000000; // Port configuration register High | Low | Port output data register
   //-------------------------------------------------------------------------------------------------------------------//
   //                  .--------------------------- SWJ_CFG[2:0]     000: Full SWJ (JTAG-DP + SW-DP): Reset State
   //                  |.-------------------------- SWJ_CFG[2:0]     001: Full SWJ (JTAG-DP + SW-DP) but without JNTRST                        100: JTAG-DP Disabled and SW-DP Disabled
   //                  ||.------------------------- SWJ_CFG[2:0]     010: JTAG-DP Disabled and SW-DP Enabled
   //                  |||        .---------------- PD01_REMAP         0: No remapping of PD0 and PD1                                            1: PD0 remapped on OSC_IN, PD1 remapped on OSC_OUT,
   //                  |||        |.--------------- CAN_REMAP[1:0]    00: CANRX mapped to PA11, CANTX mapped to PA12                            10: CANRX mapped to PB8, CANTX mapped to PB9 (not available on 36-pin package)
   //                  |||        ||.-------------- CAN_REMAP[1:0]    01: Not used                                                              11: CANRX mapped to PD0, CANTX mapped to PD1
   //                  |||        |||.------------- TIM4_REMAP         0: No remap (TIM4_CH1/PB6, TIM4_CH2/PB7, TIM4_CH3/PB8, TIM4_CH4/PB9)      1: Full remap (TIM4_CH1/PD12, TIM4_CH2/PD13, TIM4_CH3/PD14, TIM4_CH4/PD15)
   //                  |||        ||||.------------ TIM3_REMAP[1:0]   00: No remap (CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)                         10: Partial remap (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)
   //                  |||        |||||.----------- TIM3_REMAP[1:0]   01: Not used                                                              11: Full remap (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)
   //                  |||        ||||||.---------- TIM2_REMAP[1:0]   00: No remap (CH1/ETR/PA0, CH2/PA1, CH3/PA2, CH4/PA3)                     10: Partial remap (CH1/ETR/PA0, CH2/PA1, CH3/PB10, CH4/PB11)
   //                  |||        |||||||.--------- TIM2_REMAP[1:0]   01: Partial remap (CH1/ETR/PA15, CH2/PB3, CH3/PA2, CH4/PA3)               11: Full remap (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11)
   //                  |||        ||||||||.-------- TIM1_REMAP[1:0]   00: No remap (ETR/PA12,CH1/PA8,CH2/PA9,CH3/PA10,CH4/PA11,BKIN/PB12,CH1N/PB13,CH2N/P14,CH3N/PB15) 10: not used
   //                  |||        |||||||||.------- TIM1_REMAP[1:0]   01: Partial  (ETR/PA12,CH1/PA8,CH2/PA9,CH3/PA10,CH4/PA11,BKIN/PA6, CH1N/PA7, CH2N/PB0,CH3N/PB1)  11: Full remap (ETR/PE7,CH1/PE9,CH2/PE11,CH3/PE13,CH4/PE14,BKIN/PE15,CH1N/PE8,CH2N/PE10,CH3N/PE12)
   //                  |||        ||||||||||.------ USART3_REMAP[1:0] 00: No remap (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14)              10: not used
   //                  |||        |||||||||||.----- USART3_REMAP[1:0] 01: Partial remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14)         11: Full remap (TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12)
   //                  |||        ||||||||||||.---- USART2_REMAP       0: No remap (CTS/PA0, RTS/PA1, TX/PA2, RX/PA3, CK/PA4)                    1: Remap (CTS/PD3, RTS/PD4, TX/PD5, RX/PD6, CK/PD7)
   //                  |||        |||||||||||||.--- USART1_REMAP       0: No remap (TX/PA9, RX/PA10)                                             1: Remap (TX/PB6, RX/PB7)
   //                  |||        ||||||||||||||.-- I2C1_REMAP         0: No remap (SCL/PB6, SDA/PB7)                                            1: Remap (SCL/PB8, SDA/PB9)
   //                  |||        |||||||||||||||.- SPI1_REMAP         0: No remap (NSS/PA4, SCK/PA5, MISO/PA6, MOSI/PA7)                        1: Remap (NSS/PA15, SCK/PB3, MISO/PB4, MOSI/PB5)
   //                  |||        ||||||||||||||||
   AFIO->MAPR = 0b00000000000000000110000000000000;
   //             33222222222111111111119876543210
   //             1098765432109876543210
}
void Configuration_NVIC(void)
{
   NVIC_InitTypeDef  NVIC_InitStructure;
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
   NVIC_InitStructure.NVIC_IRQChannel                   = USB_LP_CAN1_RX0_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x0;
   NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}
void Configuration_ADC(void)
{
   //{-------------------------------------------------------------------------------------------------------------------//
   //                                         .-----  4 STRT: Regular channel Start flag            1: Regular channel conversion has started
   //                                         |.----  3 JSTRT: Injected channel Start flag          1: Injected group conversion has started
   //                                         ||.---  2 JEOC: Injected channel end of conversion    1: Conversion complete
   //                                         |||.--  1 EOC: End of conversion                      1: Conversion complete
   //                                         ||||.-  0 AWD: Analog watchdog flag                   1: Analog Watchdog event occurred
   //}                                        |||||
   ADC1->SR    = 0b00000000000000000000000000000000;  // ADC status register
   //{-------------------------------------------------------------------------------------------------------------------//
   //                      .------------------------ 23 AWDEN: Analog watchdog enable on regular channels               1: Analog watchdog enabled on regular channels
   //                      |.----------------------- 22 JAWDEN: Analog watchdog enable on injected channels             1: Analog watchdog enabled on injected channels
   //                      ||  .-------------------- 19 DUALMOD[3:0]: Dual mode selection                            0001: Combined regular simultaneous + injected simultaneous mode   0000: Independent mode.                 1000: Slow interleaved mode only
   //                      ||  |.------------------- 18 DUALMOD[3:0]: Dual mode selection                            0010: Combined regular simultaneous + alternate trigger mode       0101: Injected simultaneous mode only   1001: Alternate trigger mode only
   //                      ||  ||.------------------ 17 DUALMOD[3:0]: Dual mode selection                            0011: Combined injected simultaneous + fast interleaved mode       0110: Regular simultaneous mode only
   //                      ||  |||.----------------- 16 DUALMOD[3:0]: Dual mode selection                            0100: Combined injected simultaneous + slow Interleaved mode       0111: Fast interleaved mode only
   //                      ||  ||||.---------------- 15 DISCNUM[2:0]: Discontinuous mode channel count                000: 1 channel
   //                      ||  |||||.--------------- 14 DISCNUM[2:0]: Discontinuous mode channel count                001: 2 channels
   //                      ||  ||||||.-------------- 13 DISCNUM[2:0]: Discontinuous mode channel count                111: 8 channels
   //                      ||  |||||||.------------- 12 JDISCEN: Discontinuous mode on injected channels                1: Discontinuous mode on injected channels enabled
   //                      ||  ||||||||.------------ 11 DISCEN: Discontinuous mode on regular channels                  1: Discontinuous mode on regular channels enabled
   //                      ||  |||||||||.----------- 10 JAUTO: Automatic Injected Group conversion                      1: Automatic injected group conversion enabled
   //                      ||  ||||||||||.----------  9 AWDSGL: Enable the watchdog on a single channel in scan mode    0: Analog watchdog enabled on all channels     1: Analog watchdog enabled on a single channel
   //                      ||  |||||||||||.---------  8 SCAN: Scan mode                                                 1: Scan mode enabled
   //                      ||  ||||||||||||.--------  7 JEOCIE: Interrupt enable for injected channels                  1: JEOC interrupt enabled. An interrupt is generated when the JEOC bit is set.
   //                      ||  |||||||||||||.-------  6 AWDIE: Analog Watchdog interrupt enable                         1: Analog Watchdog interrupt enabled
   //                      ||  ||||||||||||||.------  5 EOCIE: Interrupt enable for EOC                                 1: EOC interrupt enabled
   //                      ||  |||||||||||||||.-----  4 AWDCH[4:0]:                                                 00000: ADC analog input Channel0
   //                      ||  ||||||||||||||||.----  3 AWDCH[4:0]:                                                 00001: ADC analog input Channel1
   //                      ||  |||||||||||||||||.---  2 AWDCH[4:0]:                                                 ....
   //                      ||  ||||||||||||||||||.--  1 AWDCH[4:0]:                                                 10000: ADC analog input Channel16
   //                      ||  |||||||||||||||||||.-  0 AWDCH[4:0]:                                                 10001: ADC analog input Channel17
   //}                     ||  ||||||||||||||||||||
   ADC1->CR1   = 0b00000000000001010000010100000000; // ADC control register 1
   //{-------------------------------------------------------------------------------------------------------------------//
   //                      .------------------------ 23 SMP17[2:0]: Channel x Sample time selection
   //                      |.----------------------- 22 SMP17[2:0]: Channel x Sample time selection
   //                      ||.---------------------- 21 SMP17[2:0]: Channel x Sample time selection
   //                      |||.--------------------- 20 SMP16[2:0]: Channel x Sample time selection
   //                      ||||.-------------------- 19 SMP16[2:0]: Channel x Sample time selection
   //                      |||||.------------------- 18 SMP16[2:0]: Channel x Sample time selection
   //                      ||||||.------------------ 17 SMP15[2:0]: Channel x Sample time selection
   //                      |||||||.----------------- 16 SMP15[2:0]: Channel x Sample time selection
   //                      ||||||||.---------------- 15 SMP15[2:0]: Channel x Sample time selection
   //                      |||||||||.--------------- 14 SMP14[2:0]: Channel x Sample time selection
   //                      ||||||||||.-------------- 13 SMP14[2:0]: Channel x Sample time selection
   //                      |||||||||||.------------- 12 SMP14[2:0]: Channel x Sample time selection
   //                      ||||||||||||.------------ 11 SMP13[2:0]: Channel x Sample time selection
   //                      |||||||||||||.----------- 10 SMP13[2:0]: Channel x Sample time selection
   //                      ||||||||||||||.----------  9 SMP13[2:0]: Channel x Sample time selection
   //                      |||||||||||||||.---------  8 SMP12[2:0]: Channel x Sample time selection
   //                      ||||||||||||||||.--------  7 SMP12[2:0]: Channel x Sample time selection 000: 1.5 cycles
   //                      |||||||||||||||||.-------  6 SMP12[2:0]: Channel x Sample time selection 001: 7.5 cycles
   //                      ||||||||||||||||||.------  5 SMP11[2:0]: Channel x Sample time selection 010: 13.5 cycles
   //                      |||||||||||||||||||.-----  4 SMP11[2:0]: Channel x Sample time selection 011: 28.5 cycles
   //                      ||||||||||||||||||||.----  3 SMP11[2:0]: Channel x Sample time selection 100: 41.5 cycles
   //                      |||||||||||||||||||||.---  2 SMP10[2:0]: Channel x Sample time selection 101: 55.5 cycles
   //                      ||||||||||||||||||||||.--  1 SMP10[2:0]: Channel x Sample time selection 110: 71.5 cycles
   //                      |||||||||||||||||||||||.-  0 SMP10[2:0]: Channel x Sample time selection 111: 239.5 cycles
   //}                     ||||||||||||||||||||||||
   ADC1->SMPR1 = 0b00000111111111111111111111111111; // ADC sample time register 1
   //{-------------------------------------------------------------------------------------------------------------------//
   //                .------------------------------ 29 SMP9[2:0]: Channel x Sample time selection
   //                |.----------------------------- 28 SMP9[2:0]: Channel x Sample time selection
   //                ||.---------------------------- 27 SMP9[2:0]: Channel x Sample time selection
   //                |||.--------------------------- 26 SMP8[2:0]: Channel x Sample time selection
   //                ||||.-------------------------- 25 SMP8[2:0]: Channel x Sample time selection
   //                |||||.------------------------- 24 SMP8[2:0]: Channel x Sample time selection
   //                ||||||.------------------------ 23 SMP7[2:0]: Channel x Sample time selection
   //                |||||||.----------------------- 22 SMP7[2:0]: Channel x Sample time selection
   //                ||||||||.---------------------- 21 SMP7[2:0]: Channel x Sample time selection
   //                |||||||||.--------------------- 20 SMP6[2:0]: Channel x Sample time selection
   //                ||||||||||.-------------------- 19 SMP6[2:0]: Channel x Sample time selection
   //                |||||||||||.------------------- 18 SMP6[2:0]: Channel x Sample time selection
   //                ||||||||||||.------------------ 17 SMP5[2:0]: Channel x Sample time selection
   //                |||||||||||||.----------------- 16 SMP5[2:0]: Channel x Sample time selection
   //                ||||||||||||||.---------------- 15 SMP5[2:0]: Channel x Sample time selection
   //                |||||||||||||||.--------------- 14 SMP4[2:0]: Channel x Sample time selection
   //                ||||||||||||||||.-------------- 13 SMP4[2:0]: Channel x Sample time selection
   //                |||||||||||||||||.------------- 12 SMP4[2:0]: Channel x Sample time selection
   //                ||||||||||||||||||.------------ 11 SMP3[2:0]: Channel x Sample time selection
   //                |||||||||||||||||||.----------- 10 SMP3[2:0]: Channel x Sample time selection
   //                ||||||||||||||||||||.----------  9 SMP3[2:0]: Channel x Sample time selection
   //                |||||||||||||||||||||.---------  8 SMP2[2:0]: Channel x Sample time selection
   //                ||||||||||||||||||||||.--------  7 SMP2[2:0]: Channel x Sample time selection 000: 1.5 cycles
   //                |||||||||||||||||||||||.-------  6 SMP2[2:0]: Channel x Sample time selection 001: 7.5 cycles
   //                ||||||||||||||||||||||||.------  5 SMP1[2:0]: Channel x Sample time selection 010: 13.5 cycles
   //                |||||||||||||||||||||||||.-----  4 SMP1[2:0]: Channel x Sample time selection 011: 28.5 cycles
   //                ||||||||||||||||||||||||||.----  3 SMP1[2:0]: Channel x Sample time selection 100: 41.5 cycles
   //                |||||||||||||||||||||||||||.---  2 SMP0[2:0]: Channel x Sample time selection 101: 55.5 cycles
   //                ||||||||||||||||||||||||||||.--  1 SMP0[2:0]: Channel x Sample time selection 110: 71.5 cycles
   //                |||||||||||||||||||||||||||||.-  0 SMP0[2:0]: Channel x Sample time selection 111: 239.5 cycles
   //}               ||||||||||||||||||||||||||||||
   ADC1->SMPR2 = 0b00111111111111111111111111111111; // ADC sample time register 2
   //{-------------------------------------------------------------------------------------------------------------------//
   //                                  .------------ 11 JOFFSETx[11:0]: Data offset for injected channel x   These bits are written by software
   //                                  |.----------- 10 JOFFSETx[11:0]: Data offset for injected channel x   to define the offset
   //                                  ||.----------  9 JOFFSETx[11:0]: Data offset for injected channel x   to be subtracted from the raw converted data
   //                                  |||.---------  8 JOFFSETx[11:0]: Data offset for injected channel x   when converting injected channels.
   //                                  ||||.--------  7 JOFFSETx[11:0]: Data offset for injected channel x
   //                                  |||||.-------  6 JOFFSETx[11:0]: Data offset for injected channel x   The conversion result can be read from
   //                                  ||||||.------  5 JOFFSETx[11:0]: Data offset for injected channel x   in the ADC_JDRx registers.
   //                                  |||||||.-----  4 JOFFSETx[11:0]: Data offset for injected channel x
   //                                  ||||||||.----  3 JOFFSETx[11:0]: Data offset for injected channel x
   //                                  |||||||||.---  2 JOFFSETx[11:0]: Data offset for injected channel x
   //                                  ||||||||||.--  1 JOFFSETx[11:0]: Data offset for injected channel x
   //                                  |||||||||||.-  0 JOFFSETx[11:0]: Data offset for injected channel x
   //}                                 ||||||||||||
   ADC1->JOFR1 = 0b00000000000000000000000000000000; // ADC injected channel data offset register x
   ADC1->JOFR2 = 0b00000000000000000000000000000000; // ADC injected channel data offset register x
   ADC1->JOFR3 = 0b00000000000000000000000000000000; // ADC injected channel data offset register x
   ADC1->JOFR4 = 0b00000000000000000000000000000000; // ADC injected channel data offset register x
   //{-------------------------------------------------------------------------------------------------------------------//
   //                                  .------------ 11 HT[11:0] Analog watchdog high threshold
   //                                  |.----------- 10 HT[11:0] Analog watchdog high threshold
   //                                  ||.----------  9 HT[11:0] Analog watchdog high threshold
   //                                  |||.---------  8 HT[11:0] Analog watchdog high threshold
   //                                  ||||.--------  7 HT[11:0] Analog watchdog high threshold
   //                                  |||||.-------  6 HT[11:0] Analog watchdog high threshold
   //                                  ||||||.------  5 HT[11:0] Analog watchdog high threshold
   //                                  |||||||.-----  4 HT[11:0] Analog watchdog high threshold
   //                                  ||||||||.----  3 HT[11:0] Analog watchdog high threshold
   //                                  |||||||||.---  2 HT[11:0] Analog watchdog high threshold   These bits are written by software
   //                                  ||||||||||.--  1 HT[11:0] Analog watchdog high threshold   to define the high threshold
   //                                  |||||||||||.-  0 HT[11:0] Analog watchdog high threshold   for the Analog Watchdog.
   //}                                 ||||||||||||
   ADC1->HTR   = 0b00000000000000000000111111111111; // ADC watchdog high threshold register
   //{-------------------------------------------------------------------------------------------------------------------//
   //                                  .------------ 11 LT[11:0] Analog watchdog low threshold
   //                                  |.----------- 10 LT[11:0] Analog watchdog low threshold
   //                                  ||.----------  9 LT[11:0] Analog watchdog low threshold
   //                                  |||.---------  8 LT[11:0] Analog watchdog low threshold
   //                                  ||||.--------  7 LT[11:0] Analog watchdog low threshold
   //                                  |||||.-------  6 LT[11:0] Analog watchdog low threshold
   //                                  ||||||.------  5 LT[11:0] Analog watchdog low threshold
   //                                  |||||||.-----  4 LT[11:0] Analog watchdog low threshold
   //                                  ||||||||.----  3 LT[11:0] Analog watchdog low threshold
   //                                  |||||||||.---  2 LT[11:0] Analog watchdog low threshold   These bits are written by software
   //                                  ||||||||||.--  1 LT[11:0] Analog watchdog low threshold   to define the low threshold
   //                                  |||||||||||.-  0 LT[11:0] Analog watchdog low threshold   for the Analog Watchdog.
   //}                                 ||||||||||||
   ADC1->LTR   = 0b00000000000000000000000000000000; // ADC watchdog low threshold register
   //{-------------------------------------------------------------------------------------------------------------------//
   //                      .------------------------ 23 L[3:0]: Regular channel sequence length  0000: 1 conversion
   //                      |.----------------------- 22 L[3:0]: Regular channel sequence length  0001: 2 conversions
   //                      ||.---------------------- 21 L[3:0]: Regular channel sequence length  .....
   //                      |||.--------------------- 20 L[3:0]: Regular channel sequence length  1111: 16 conversions
   //                      ||||.-------------------- 19 SQ16[4:0]: 16th conversion in regular sequence
   //                      |||||.------------------- 18 SQ16[4:0]: 16th conversion in regular sequence
   //                      ||||||.------------------ 17 SQ16[4:0]: 16th conversion in regular sequence
   //                      |||||||.----------------- 16 SQ16[4:0]: 16th conversion in regular sequence
   //                      ||||||||.---------------- 15 SQ16[4:0]: 16th conversion in regular sequence
   //                      |||||||||.--------------- 14 SQ15[4:0]: 15th conversion in regular sequence
   //                      ||||||||||.-------------- 13 SQ15[4:0]: 15th conversion in regular sequence
   //                      |||||||||||.------------- 12 SQ15[4:0]: 15th conversion in regular sequence
   //                      ||||||||||||.------------ 11 SQ15[4:0]: 15th conversion in regular sequence
   //                      |||||||||||||.----------- 10 SQ15[4:0]: 15th conversion in regular sequence
   //                      ||||||||||||||.----------  9 SQ14[4:0]: 14th conversion in regular sequence
   //                      |||||||||||||||.---------  8 SQ14[4:0]: 14th conversion in regular sequence
   //                      ||||||||||||||||.--------  7 SQ14[4:0]: 14th conversion in regular sequence
   //                      |||||||||||||||||.-------  6 SQ14[4:0]: 14th conversion in regular sequence
   //                      ||||||||||||||||||.------  5 SQ14[4:0]: 14th conversion in regular sequence
   //                      |||||||||||||||||||.-----  4 SQ13[4:0]: 13th conversion in regular sequence
   //                      ||||||||||||||||||||.----  3 SQ13[4:0]: 13th conversion in regular sequence
   //                      |||||||||||||||||||||.---  2 SQ13[4:0]: 13th conversion in regular sequence
   //                      ||||||||||||||||||||||.--  1 SQ13[4:0]: 13th conversion in regular sequence
   //                      |||||||||||||||||||||||.-  0 SQ13[4:0]: 13th conversion in regular sequence
   //}                     ||||||||||||||||||||||||
   ADC1->SQR1  = 0b00000000000000000000000000000000; // ADC regular sequence register 1
   //{-------------------------------------------------------------------------------------------------------------------//
   //                .------------------------------ 29 SQ12[4:0]
   //                |.----------------------------- 28 SQ12[4:0]
   //                ||.---------------------------- 27 SQ12[4:0]
   //                |||.--------------------------- 26 SQ12[4:0]
   //                ||||.-------------------------- 25 SQ12[4:0]: 12th conversion in regular sequence
   //                |||||.------------------------- 24 SQ11[4:0]
   //                ||||||.------------------------ 23 SQ11[4:0]
   //                |||||||.----------------------- 22 SQ11[4:0]
   //                ||||||||.---------------------- 21 SQ11[4:0]
   //                |||||||||.--------------------- 20 SQ11[4:0]: 11th conversion in regular sequence
   //                ||||||||||.-------------------- 19 SQ10[4:0]
   //                |||||||||||.------------------- 18 SQ10[4:0]
   //                ||||||||||||.------------------ 17 SQ10[4:0]
   //                |||||||||||||.----------------- 16 SQ10[4:0]
   //                ||||||||||||||.---------------- 15 SQ10[4:0]: 10th conversion in regular sequence
   //                |||||||||||||||.--------------- 14 SQ9[4:0]
   //                ||||||||||||||||.-------------- 13 SQ9[4:0]
   //                |||||||||||||||||.------------- 12 SQ9[4:0]
   //                ||||||||||||||||||.------------ 11 SQ9[4:0]
   //                |||||||||||||||||||.----------- 10 SQ9[4:0]: 9th conversion in regular sequence
   //                ||||||||||||||||||||.----------  9 SQ8[4:0]
   //                |||||||||||||||||||||.---------  8 SQ8[4:0]
   //                ||||||||||||||||||||||.--------  7 SQ8[4:0]
   //                |||||||||||||||||||||||.-------  6 SQ8[4:0]
   //                ||||||||||||||||||||||||.------  5 SQ8[4:0]: 8th conversion in regular sequence
   //                |||||||||||||||||||||||||.-----  4 SQ7[4:0]
   //                ||||||||||||||||||||||||||.----  3 SQ7[4:0]
   //                |||||||||||||||||||||||||||.---  2 SQ7[4:0]
   //                ||||||||||||||||||||||||||||.--  1 SQ7[4:0]
   //                |||||||||||||||||||||||||||||.-  0 SQ7[4:0]: 7th conversion in regular sequence
   //}               ||||||||||||||||||||||||||||||
   ADC1->SQR2  = 0b00000000000000000000000000000000; // ADC regular sequence register 2
   //{-------------------------------------------------------------------------------------------------------------------//
   //                .------------------------------ 29 SQ6[4:0]
   //                |.----------------------------- 28 SQ6[4:0]
   //                ||.---------------------------- 27 SQ6[4:0]
   //                |||.--------------------------- 26 SQ6[4:0]
   //                ||||.-------------------------- 25 SQ6[4:0]: 6th conversion in regular sequence
   //                |||||.------------------------- 24 SQ5[4:0]
   //                ||||||.------------------------ 23 SQ5[4:0]
   //                |||||||.----------------------- 22 SQ5[4:0]
   //                ||||||||.---------------------- 21 SQ5[4:0]
   //                |||||||||.--------------------- 20 SQ5[4:0]: 5th conversion in regular sequence
   //                ||||||||||.-------------------- 19 SQ4[4:0]
   //                |||||||||||.------------------- 18 SQ4[4:0]
   //                ||||||||||||.------------------ 17 SQ4[4:0]
   //                |||||||||||||.----------------- 16 SQ4[4:0]
   //                ||||||||||||||.---------------- 15 SQ4[4:0]: 4th conversion in regular sequence
   //                |||||||||||||||.--------------- 14 SQ3[4:0]
   //                ||||||||||||||||.-------------- 13 SQ3[4:0]
   //                |||||||||||||||||.------------- 12 SQ3[4:0]
   //                ||||||||||||||||||.------------ 11 SQ3[4:0]
   //                |||||||||||||||||||.----------- 10 SQ3[4:0]: 3th conversion in regular sequence
   //                ||||||||||||||||||||.----------  9 SQ2[4:0]
   //                |||||||||||||||||||||.---------  8 SQ2[4:0]
   //                ||||||||||||||||||||||.--------  7 SQ2[4:0]
   //                |||||||||||||||||||||||.-------  6 SQ2[4:0]
   //                ||||||||||||||||||||||||.------  5 SQ2[4:0]: 2th conversion in regular sequence
   //                |||||||||||||||||||||||||.-----  4 SQ1[4:0]
   //                ||||||||||||||||||||||||||.----  3 SQ1[4:0]
   //                |||||||||||||||||||||||||||.---  2 SQ1[4:0]
   //                ||||||||||||||||||||||||||||.--  1 SQ1[4:0]
   //                |||||||||||||||||||||||||||||.-  0 SQ1[4:0]: 1th conversion in regular sequence
   //}               ||||||||||||||||||||||||||||||
   ADC1->SQR3  = 0b00000000000000000000000000001001; // ADC regular sequence register 3
   //{-------------------------------------------------------------------------------------------------------------------//
   //                        .---------------------- 21 JL[1:0]: Injected Sequence length   00: 1 conversion     10: 3 conversions
   //                        |.--------------------- 20 JL[1:0]: Injected Sequence length   01: 2 conversions    11: 4 conversions
   //                        ||.-------------------- 19 JSQ4[4:0]
   //                        |||.------------------- 18 JSQ4[4:0]
   //                        ||||.------------------ 17 JSQ4[4:0]
   //                        |||||.----------------- 16 JSQ4[4:0]
   //                        ||||||.---------------- 15 JSQ4[4:0]: 4th conversion in injected sequence
   //                        |||||||.--------------- 14 JSQ3[4:0]
   //                        ||||||||.-------------- 13 JSQ3[4:0]
   //                        |||||||||.------------- 12 JSQ3[4:0]
   //                        ||||||||||.------------ 11 JSQ3[4:0]
   //                        |||||||||||.----------- 10 JSQ3[4:0]: 3rd conversion in injected sequence
   //                        ||||||||||||.----------  9 JSQ2[4:0]
   //                        |||||||||||||.---------  8 JSQ2[4:0]
   //                        ||||||||||||||.--------  7 JSQ2[4:0]
   //                        |||||||||||||||.-------  6 JSQ2[4:0]
   //                        ||||||||||||||||.------  5 JSQ2[4:0]: 2nd conversion in injected sequence
   //                        |||||||||||||||||.-----  4 JSQ1[4:0]
   //                        ||||||||||||||||||.----  3 JSQ1[4:0]
   //                        |||||||||||||||||||.---  2 JSQ1[4:0]
   //                        ||||||||||||||||||||.--  1 JSQ1[4:0]
   //                        |||||||||||||||||||||.-  0 JSQ1[4:0]: 1st conversion in injected sequence
   //}                       ||||||||||||||||||||||
   ADC1->JSQR  = 0b00000000001101101011001000001010 ; // ADC injected sequence register
   
   // ADC injected data register  = ADC1->JDR1
   // ADC injected data register  = ADC1->JDR2
   // ADC injected data register  = ADC1->JDR3
   // ADC injected data register  = ADC1->JDR4
   // ADC regular  data register  = ADC1->DR
   
   //{-------------------------------------------------------------------------------------------------------------------//
   //                      .------------------------ 23 TSVREFE: Temperature Sensor and VREFINT Enable                  1: Temperature sensor and VREFINT channel enabled
   //                      |.----------------------- 22 SWSTART: Start Conversion of regular channels                   1: Starts conversion of regular channels
   //                      ||.---------------------- 21 JSWSTART: Start Conversion of injected channels                 1: Starts conversion of injected channels
   //                      |||.--------------------- 20 EXTTRIG: External Trigger Conversion mode for regular CH        1: Conversion on external event enabled
   //                      ||||.-------------------- 19 EXTSEL[2:0]: External Event Select for regular group          000: Timer 1 CC1 event    011: Timer 2 CC2 event   110: EXTI line11
   //                      |||||.------------------- 18 EXTSEL[2:0]: External Event Select for regular group          001: Timer 1 CC2 event    100: Timer 3 TRGO event  111: SWSTART
   //                      ||||||.------------------ 17 EXTSEL[2:0]: External Event Select for regular group          010: Timer 1 CC3 event    101: Timer 4 CC4 event
   //                      ||||||| .---------------- 15 JEXTTRIG: External Trigger Conversion mode for injected CH      1: Conversion on external event enabled
   //                      ||||||| |.--------------- 14 JEXTSEL[2:0]: External event select for injected group        000: Timer 1 TRGO event  011: Timer 2 CC1 event  110: EXTI line15
   //                      ||||||| ||.-------------- 13 JEXTSEL[2:0]: External event select for injected group        001: Timer 1 CC4 event   100: Timer 3 CC4 event  111: JSWSTART
   //                      ||||||| |||.------------- 12 JEXTSEL[2:0]: External event select for injected group        010: Timer 2 TRGO event  101: Timer 4 TRGO event
   //                      ||||||| ||||.------------ 11 ALIGN: Data Alignment                                           0: Right Alignment  1: Left Alignment
   //                      ||||||| |||||  .---------  8 DMA: Direct Memory access mode                                  1: DMA mode enabled
   //                      ||||||| |||||  |    .----  3 RSTCAL: Reset Calibration                                       1: Initialize calibration register
   //                      ||||||| |||||  |    |.---  2 CAL: A/D Calibration                                            1: Enable calibration
   //                      ||||||| |||||  |    ||.--  1 CONT: Continuous Conversion                                     1: Continuous conversion mode
   //                      ||||||| |||||  |    |||.-  0 ADON: A/D Converter ON / OFF                                    1: Enable ADC and to start conversion
   //}                     ||||||| |||||  |    ||||
   ADC1->CR2   = 0b00000000100111100000000000000011; // ADC control register 2
   ADC1->CR2   = 0b00000000111111100000000000000011; // ADC control register 2
}
void Configuration_SPI(void)
{
   //{-------------------------------------------------------------------------------------------------------------------//
   //            .---------------- 15 BIDIMODE: Bidirectional data mode enable      0: 2-line unidirectional data mode selected     1: 1-line bidirectional data mode selected
   //            |.--------------- 14 BIDIOE: Output enable in bidirectional mode   0: Output disabled (receive-only mode)          1: Output enabled (transmit-only mode)
   //            ||.-------------- 13 CRCEN: Hardware CRC calculation enable        0: CRC calculation disabled                     1: CRC calculation Enabled
   //            |||.------------- 12 CRCNEXT: Transmit CRC next                    0: Next transmit value is from Tx buffer        1: Next transmit value is from Tx CRC register
   //            ||||.------------ 11 DFF: Data Frame Format                        0: 8-bit data frame format is selected          1: 16-bit data frame format is selected for transmission/reception
   //            |||||.----------- 10 RXONLY: Receive only                          0: Full duplex (Transmit and receive)           1: Output disabled (Receive only mode)
   //            ||||||.----------  9 SSM: Software slave management                0: Software slave management disabled           1: Software slave management enabled
   //            |||||||.---------  8 SSI: Internal slave select                    This bit has effect only when SSM bit is set. The value of this bit is forced onto the NSS pin and the I/O value of the NSS pin is ignored
   //            ||||||||.--------  7 LSBFIRST: Frame Format                        0: MSB transmitted first                        1: LSB transmitted first
   //            |||||||||.-------  6 SPE: SPI Enable                               0: Peripheral disabled                          1: Peripheral enabled
   //            ||||||||||.------  5 BR[2:0]: Baud Rate Control                  000: fPCLK/2    011: fPCLK/16      110: fPCLK/128
   //            |||||||||||.-----  4 BR[2:0]: Baud Rate Control                  001: fPCLK/4    100: fPCLK/32      111: fPCLK/256
   //            ||||||||||||.----  3 BR[2:0]: Baud Rate Control                  010: fPCLK/8    101: fPCLK/64
   //            |||||||||||||.---  2 MSTR: Master Selection                        0: Slave configuration                          1: Master configuration
   //            ||||||||||||||.--  1 CPOL: Clock Polarity                          0: SCK to 0 when idle                           1: SCK to 1 when idle
   //            |||||||||||||||.-  0 CPHA: Clock Phase                             0: The first clock transition is the first data 1: The second clock transition is the first data capture edge
   //}           ||||||||||||||||
   SPI1->CR1 = 0b0000001101011111; // SPI 1 Control Register 1
   SPI2->CR1 = 0b0000001101111111; // SPI 2 Control Register 1
   
   //{-------------------------------------------------------------------------------------------------------------------//
   //                    .--------  7 TXEIE: Tx buffer empty interrupt enable       0: TXE interrupt masked                         1: TXE  interrupt not masked. This allows a interrupt request to be generated when the TXE  flag is set. Note: To function correctly, the TXEIE and TXDMAEN bits should not be set at the same time.
   //                    |.-------  6 RXNEIE: RX buffer not empty interrupt enable  0: RXNE interrupt masked                        1: RXNE interrupt not masked. This allows a interrupt request to be generated when the RXNE flag is set. Note: To function correctly, the TXEIE and TXDMAEN bits should not be set at the same time.
   //                    ||.------  5 ERRIE: Error interrupt enable                 0: Error interrupt is masked                    1: Error interrupt is enabled
   //                    |||  .---  2 SSOE: SS Output Enable                        0: SS output is disabled in master mode and the cell can work in multimaster configuration   1: SS output is enabled in master mode and when the cell is enabled. The cell cannot work in a multimaster environment.
   //                    |||  |.--  1 TXDMAEN: Tx Buffer DMA Enable                 0: Tx buffer DMA disabled                       1: Tx buffer DMA enabled
   //                    |||  ||.-  0 RXDMAEN: Rx Buffer DMA Enable                 0: Rx buffer DMA disabled                       1: Rx buffer DMA enabled
   //}                   |||  |||
   SPI1->CR2 = 0b0000000000000000; // SPI 1 Control Register 2
   SPI2->CR2 = 0b0000000000000000; // SPI 2 Control Register 2
   //{-------------------------------------------------------------------------------------------------------------------//
   //                      .--------  7 BSY: Busy flag                    0: SPI not busy                                 1: SPI is busy in communication or Tx buffer is not empty This flag is set and reset by hardware.
   //                      |.-------  6 OVR: Overrun flag                 0: No Overrun occurred                          1: Overrun occurred  This flag is set by hardware and reset by a software sequence.
   //                      ||.------  5 MODF: Mode fault                  0: No Mode fault occurred                       1: Mode fault occurred
   //                      |||.-----  4 CRCERR: CRC error flag            0: CRC value received matches the SPI_RXCRCR    1: CRC value received does not match the SPI_RXCRCR value
   //                      ||||  .--  1 TXE: Transmit buffer empty        0: Tx buffer not empty                          1: Tx buffer empty
   //                      ||||  |.-  0 RXNE: Receive buffer not empty    0: Rx buffer empty                              1: Rx buffer not empty
   //}                     ||||  ||
   //SPI1->SR  = 0b0000000000000000; // SPI status register
   //SPI1->DR  = SPI data register (SPI_DR)
   //SPI1->CRCPR   = SPI CRC polynomial register (SPI_CRCPR)
   //SPI1->RXCRCR  = SPI Rx CRC register (SPI_RXCRCR)
   //SPI1->TXCRCR  = SPI Tx CRC register (SPI_TXCRCR)
}
void Configuration_CAN(void)
{
   CAN_InitTypeDef        CAN_InitStructure;
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   /* CAN register init */
   CAN_DeInit(CAN1);
   
   CAN_StructInit(&CAN_InitStructure);
   
   /* CAN cell init */
   CAN_InitStructure.CAN_TTCM = DISABLE;
   CAN_InitStructure.CAN_ABOM = DISABLE;
   CAN_InitStructure.CAN_AWUM = DISABLE;
   CAN_InitStructure.CAN_NART = DISABLE;
   CAN_InitStructure.CAN_RFLM = DISABLE;
   CAN_InitStructure.CAN_TXFP = DISABLE;
   CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
   CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
   #ifdef  _125KB
   CAN_InitStructure.CAN_Prescaler = 16;
   CAN_InitStructure.CAN_BS1       = 8;
   CAN_InitStructure.CAN_BS2       = 7;
   #endif
   #ifdef  _250KB
   CAN_InitStructure.CAN_Prescaler = 8;
   CAN_InitStructure.CAN_BS1       = 8;
   CAN_InitStructure.CAN_BS2       = 7;
   #endif
   #ifdef  _500KB
   CAN_InitStructure.CAN_Prescaler = 4;
   CAN_InitStructure.CAN_BS1       = 8;
   CAN_InitStructure.CAN_BS2       = 7;
   #endif
   #ifdef  _1000KB
   CAN_InitStructure.CAN_Prescaler = 2;
   CAN_InitStructure.CAN_BS1       = 8;
   CAN_InitStructure.CAN_BS2       = 7;
   #endif
   
   CAN_Init(CAN1, &CAN_InitStructure);
   
   /* CAN filter init */
   CAN_FilterInitStructure.CAN_FilterNumber         =      0;
   CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;
   CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;
   CAN_FilterInitStructure.CAN_FilterIdHigh         = 0x0000;
   CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000;
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment =      0;
   CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
   CAN_FilterInit(&CAN_FilterInitStructure);
}

