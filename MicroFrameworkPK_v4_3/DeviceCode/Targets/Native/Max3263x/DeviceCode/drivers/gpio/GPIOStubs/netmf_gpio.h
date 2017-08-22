#ifndef _NETMF_GPIO_H_
#define _NETMF_GPIO_H_ 1

//#include <stm32f10x.h>


enum GPIO_DIRECTION
{
    GPIO_DIRECTION_OUT = 0,
    GPIO_DIRECTION_IN = 1
};
typedef struct
{
  uint32_t CRL;
  uint32_t CRH;
  uint32_t IDR;
  uint32_t ODR;
  uint32_t BSRR;
  uint32_t BRR;
  uint32_t LCKR;
} GPIO_TypeDef;

typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;

typedef enum
{ 
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;
#ifdef __cplusplus
void GPIO_ConfigurePin( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode = GPIO_Mode_IN_FLOATING, GPIOSpeed_TypeDef speed = GPIO_Speed_10MHz);
#endif

#if !defined(_TINYHAL_H_)
typedef unsigned int GPIO_PIN; /* FIXME: work-around. GPIO_PIN in tinyhal.h, but this gets put into C Code.*/
#endif

GPIO_PIN GPIO_GetNumber(GPIO_TypeDef* Port, uint16_t Pin);


#ifdef __cplusplus
extern "C" {
#endif
void GPIO_ConfigurePinC( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed);
#ifdef __cplusplus
}
#endif

#endif
