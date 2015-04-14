#ifndef _NETMF_GPIO_H_
#define _NETMF_GPIO_H_

#include <stm32f10x.h>


enum GPIO_DIRECTION
{
    GPIO_DIRECTION_OUT = 0,
    GPIO_DIRECTION_IN = 1
};

//
#ifdef __cplusplus
void GPIO_ConfigurePin( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode = GPIO_Mode_IN_FLOATING, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);
#endif


#ifdef __cplusplus
extern "C" {
#endif
void GPIO_ConfigurePinC( GPIO_TypeDef* GPIO_PortSource, uint16_t Pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed);
#ifdef __cplusplus
}
#endif

#endif
