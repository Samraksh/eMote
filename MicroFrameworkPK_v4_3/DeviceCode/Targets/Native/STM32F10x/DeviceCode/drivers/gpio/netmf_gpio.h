#ifndef _NETMF_GPIO_H_
#define _NETMF_GPIO_H_ 1

#include <stm32f10x.h>


enum GPIO_DIRECTION
{
    GPIO_DIRECTION_OUT = 0,
    GPIO_DIRECTION_IN = 1
};

#ifdef __cplusplus
void GPIO_ConfigurePin( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode = GPIO_Mode_IN_FLOATING, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);
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
