#ifndef __NETMFSPI_H__
#define __NETMFSPI_H__


/*
REVISION LOG
------------

11/03/2010 Kartik Natarajan (KN) - Initial Create

*/

#include <tinyhal.h>
#include "../krait.h"
//#include <rcc/stm32f10x_rcc.h>
#include "../Krait_GPIO/Krait__GPIO.h"
#include <Krait_SPI.h>
#include "../Krait_TIMER/Krait__TIMER.h"
#include "../../../../../Drivers/Samraksh/radio/RF231/RF231.h"


void GPIO_Config(const SPI_CONFIGURATION& Configuration);
void RCC_Config();
void NVIC_Config();
void SPI_StructInit(const SPI_CONFIGURATION& Configuration);

//AnanthAtSamraksh -- these are from stm32f10x_rcc.h -- placed here temporarily

#define RCC_APB2Periph_SPI1              ((UINT32)0x00001000)
#define RCC_APB1Periph_SPI2              ((UINT32)0x00004000)
#define RCC_APB1Periph_SPI3              ((UINT32)0x00008000)
#define RCC_APB2Periph_GPIOA             ((UINT32)0x00000004)
#define RCC_APB2Periph_GPIOB             ((UINT32)0x00000008)
#define RCC_APB2Periph_GPIOC             ((UINT32)0x00000010)

#define SPI_Direction_2Lines_FullDuplex ((UINT16)0x0000)
#define SPI_Direction_2Lines_RxOnly     ((UINT16)0x0400)
#define SPI_Direction_1Line_Rx          ((UINT16)0x8000)
#define SPI_Direction_1Line_Tx          ((UINT16)0xC000)
#define IS_SPI_DIRECTION_MODE(MODE) (((MODE) == SPI_Direction_2Lines_FullDuplex) || \
                                     ((MODE) == SPI_Direction_2Lines_RxOnly) || \
                                     ((MODE) == SPI_Direction_1Line_Rx) || \
                                     ((MODE) == SPI_Direction_1Line_Tx))
/**
  * @}
  */

/** @defgroup SPI_mode
  * @{
  */

#define SPI_Mode_Master                 ((UINT16)0x0104)
#define SPI_Mode_Slave                  ((UINT16)0x0000)
#define IS_SPI_MODE(MODE) (((MODE) == SPI_Mode_Master) || \
                           ((MODE) == SPI_Mode_Slave))


/** @defgroup SPI_data_size
  * @{
  */

#define SPI_DataSize_16b                ((UINT16)0x0800)
#define SPI_DataSize_8b                 ((UINT16)0x0000)
#define IS_SPI_DATASIZE(DATASIZE) (((DATASIZE) == SPI_DataSize_16b) || \
                                   ((DATASIZE) == SPI_DataSize_8b))

/** @defgroup SPI_Slave_Select_management
  * @{
  */

#define SPI_NSS_Soft                    ((UINT16)0x0200)
#define SPI_NSS_Hard                    ((UINT16)0x0000)
#define IS_SPI_NSS(NSS) (((NSS) == SPI_NSS_Soft) || \
                         ((NSS) == SPI_NSS_Hard))


/** @defgroup SPI_BaudRate_Prescaler
  * @{
  */

#define SPI_BaudRatePrescaler_2         ((UINT16)0x0000)
#define SPI_BaudRatePrescaler_4         ((UINT16)0x0008)
#define SPI_BaudRatePrescaler_8         ((UINT16)0x0010)
#define SPI_BaudRatePrescaler_16        ((UINT16)0x0018)
#define SPI_BaudRatePrescaler_32        ((UINT16)0x0020)
#define SPI_BaudRatePrescaler_64        ((UINT16)0x0028)
#define SPI_BaudRatePrescaler_128       ((UINT16)0x0030)
#define SPI_BaudRatePrescaler_256       ((UINT16)0x0038)
#define IS_SPI_BAUDRATE_PRESCALER(PRESCALER) (((PRESCALER) == SPI_BaudRatePrescaler_2) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_4) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_8) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_16) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_32) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_64) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_128) || \
                                              ((PRESCALER) == SPI_BaudRatePrescaler_256))


/** @defgroup SPI_MSB_LSB_transmission
  * @{
  */

#define SPI_FirstBit_MSB                ((UINT16)0x0000)
#define SPI_FirstBit_LSB                ((UINT16)0x0080)
#define IS_SPI_FIRST_BIT(BIT) (((BIT) == SPI_FirstBit_MSB) || \
                               ((BIT) == SPI_FirstBit_LSB))

//AnanthAtSamraksh


#endif
