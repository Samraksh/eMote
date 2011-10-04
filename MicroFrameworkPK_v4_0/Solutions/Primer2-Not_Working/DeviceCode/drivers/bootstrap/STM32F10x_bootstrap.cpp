////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**======================================================
 *		SAMRAKSH
 *=======================================================
 *	NAME	DATE	NOTES
 *	----	----	--------------------
 *	@NK1	07/01	Initial create
 *
 ======================================================**/

#include <tinyhal.h>
#include <gpio/stm32f10x_gpio.h>
#include <rcc/stm32f10x_rcc.h>
#include <misc/misc.h>





//LEDs port and pins
#define LEDS_GPIO       (GPIOE)
#define LEDS_RCC_GPIO   (RCC_APB2Periph_GPIOE)
#define LED0_PIN        (GPIO_Pin_0)
#define LED1_PIN        (GPIO_Pin_1)
#define LEDS_BOTH_PINS  (LED0_PIN|LED1_PIN)

//joystick port and pins
#define JSTK_GPIO       (GPIOE)
#define JSTK_RCC_GPIO   (RCC_APB2Periph_GPIOE)
#define JSTK_LEFT_PIN   (GPIO_Pin_3)
#define JSTK_RIGHT_PIN  (GPIO_Pin_4)
#define JSTK_UP_PIN     (GPIO_Pin_5)
#define JSTK_DOWN_PIN   (GPIO_Pin_6)
#define JSTK_ALL_PINS   (JSTK_LEFT_PIN|JSTK_RIGHT_PIN|JSTK_UP_PIN|JSTK_DOWN_PIN)

//push button port and pin
#define BP_GPIO         (GPIOA)
#define BP_RCC_GPIO     (RCC_APB2Periph_GPIOA)
#define BP_PIN          (GPIO_Pin_8)


//LCD backlight enable port and pin
#define LCDBLEN_GPIO         (GPIOB)
#define LCDBLEN_RCC_GPIO     (RCC_APB2Periph_GPIOB)
#define LCDBLEN_PIN          (GPIO_Pin_8)


/* example defines ------------------------------------------------------------*/

//start, min and max for the delay variable
#define DELAYVAR_MAXVAL         (0x100000)
#define DELAYVAR_STARTVAL       (0x40000)
#define DELAYVAR_MINVAL         (0x1000)

//small and big steps for the delay variable
#define DELAYVAR_BIGCHANGE      (0x4000)
#define DELAYVAR_SMALLCHANGE    (0x1000)

/* example function prototypes -----------------------------------------------*/

void RCC_Configuration(void);
void NVIC_Configuration(void);
void Delay(vu32 nCount);

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

/* example global variables --------------------------------------------------*/

/*these variables could (and should, maybe) be defined as local to the main
function, but declaring them as global and volatile helps for debugging.
*/
GPIO_InitTypeDef GPIO_InitStructure;
volatile int delayvar=DELAYVAR_STARTVAL;
volatile unsigned char toggle=0;
vu16 jstkinput=0;
vu16 bpinput=0;


/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

static void SetSysClockTo72(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /*!< SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /*!< Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /*!< Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /*!< Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /*!< Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    
 
    /*!< HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /*!< PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /*!< PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
    
    /*!< PLLCLK = 12MHz * 6 = 72 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL6);

    /*!< Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /*!< Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /*!< Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /*!< Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /*!< If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */    

    /*!< Go to infinite loop */
    while (1)
    {
    }
  }
}

void LowLevelInit (void)
{
  /*!< RCC system reset(for debug purpose) */
  /*!< Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;
  /*!< Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] and MCO[2:0] bits */
  RCC->CFGR &= (uint32_t)0xF8FF0000;  
  /*!< Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;
  /*!< Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;
  /*!< Reset PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;
  /*!< Disable all interrupts */
  RCC->CIR = 0x00000000;
    
  /*!< Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  /*!< Configure the Flash Latency cycles and enable prefetch buffer */
  SetSysClockTo72();

}
 
/**
 * Calls various bootstrap routines
 **/
void BootstrapCode() 
{
	//LowLevelInit();
//Main_System_Init_Finished_Bol = FALSE;
   
   //SystemInit();
   LowLevelInit();
   
   RCC_ClocksTypeDef RCC_Clocks;
   GPIO_InitTypeDef GPIO_InitStructure;
   
   
   Configuration_RCC();
   Configuration_GPIO();
   Configuration_NVIC();
   
   RCC_GetClocksFreq(&RCC_Clocks);
   RCC_APB2PeriphClockCmd(LCDBLEN_RCC_GPIO, ENABLE);

    // Configure PB.08 as output
    GPIO_InitStructure.GPIO_Pin = LCDBLEN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //GPIO_Init(LCDBLEN_GPIO, &GPIO_InitStructure);
    //GPIO_Write(LCDBLEN_GPIO, ( 0 ) );
    
    
    // Enable LEDs GPIO clock 
    //if(LEDS_RCC_GPIO!=LCDBLEN_RCC_GPIO)
      //  RCC_APB2PeriphClockCmd(LEDS_RCC_GPIO, ENABLE);

    // Configure PE.00 and PE.01 as output push-pull 
    GPIO_InitStructure.GPIO_Pin =  LEDS_BOTH_PINS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LEDS_GPIO, &GPIO_InitStructure);

    // Enable joystick GPIO clock 
//    if( (JSTK_RCC_GPIO!=LCDBLEN_RCC_GPIO) && (JSTK_RCC_GPIO!=LEDS_RCC_GPIO) )
  //      RCC_APB2PeriphClockCmd(JSTK_RCC_GPIO, ENABLE);

    // Configure PE.03 to PE.06 as input 
    GPIO_InitStructure.GPIO_Pin = JSTK_ALL_PINS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(JSTK_GPIO, &GPIO_InitStructure);

    // Enable push button GPIO clock 
//    if( (BP_RCC_GPIO!=LCDBLEN_RCC_GPIO) && (BP_RCC_GPIO!=LEDS_RCC_GPIO) && (BP_RCC_GPIO!=JSTK_RCC_GPIO) )
  //      RCC_APB2PeriphClockCmd(BP_RCC_GPIO, ENABLE);

    // Configure PA.08 as input 
    GPIO_InitStructure.GPIO_Pin = BP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BP_GPIO, &GPIO_InitStructure);

    
    while(1)
    {
        //stop all toggling while push-button is down
        do
            {
            bpinput = ( (GPIO_ReadInputData(BP_GPIO)) & ( BP_PIN ) );
            if(bpinput)
                delayvar=DELAYVAR_STARTVAL;
            }
        while(bpinput);
        
        // change speed of toggling according to joystick direction 
        //jstkinput = ( (GPIO_ReadInputData(JSTK_GPIO)) & ( JSTK_ALL_PINS ) ); 
        jstkinput = ((u16)JSTK_GPIO->IDR) & ( JSTK_ALL_PINS );
        
        switch( jstkinput & ( JSTK_ALL_PINS ) )
            {
            case GPIO_Pin_3:
                //Left button is pressed. slow down toggling a little.
                if ( delayvar <= ( DELAYVAR_MAXVAL - DELAYVAR_SMALLCHANGE ) )
                    delayvar += DELAYVAR_SMALLCHANGE;
                else
                    delayvar = DELAYVAR_MAXVAL;
                    
                break;
                
            case GPIO_Pin_4:
                //Right button is pressed. speed up toggling a little.
                if ( delayvar >= ( DELAYVAR_MINVAL + DELAYVAR_SMALLCHANGE ) )
                    delayvar -= DELAYVAR_SMALLCHANGE;
                else
                    delayvar = DELAYVAR_MINVAL;
                break;
                
            case GPIO_Pin_5:
                //Up button is pressed. slow down toggling quite a lot.
                if ( delayvar <= ( DELAYVAR_MAXVAL - DELAYVAR_BIGCHANGE ) )
                    delayvar += DELAYVAR_BIGCHANGE;
                else
                    delayvar = DELAYVAR_MAXVAL;
                break;
                
            case GPIO_Pin_6:
                //Down button is pressed. speed up toggling quite a lot.
                if ( delayvar >= ( DELAYVAR_MINVAL + DELAYVAR_BIGCHANGE ) )
                    delayvar -= DELAYVAR_BIGCHANGE;
                else
                    delayvar = DELAYVAR_MINVAL;
                break;
                
            default :
                //no buttons are pressed (or several, which is a hardware error)
                //just go on toggling at the same speed
                break;
            }

        // Toggle LEDs 
        GPIO_Write(LEDS_GPIO, ( toggle & LEDS_BOTH_PINS ) );

        // increment Toggle variable 
        toggle++;

        // Insert delay 
        Delay(delayvar);

    }   
}
