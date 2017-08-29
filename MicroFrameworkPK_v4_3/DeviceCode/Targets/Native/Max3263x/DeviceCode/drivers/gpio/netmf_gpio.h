#ifndef _NETMF_GPIO_H_
#define _NETMF_GPIO_H_ 1

#include <max3263x.h>
#include <gpio.h>

#define EV_KIT_GPIO_LED0 24 // Port 3, pin 0
#define EV_KIT_GPIO_LED1 25 // Port 3, pin 1
#define EV_KIT_GPIO_LED2 26 // Port 3, pin 2
#define EV_KIT_GPIO_LED3 27 // Port 3, pin 3

void GPIO_ConfigurePin( uint16_t port, uint16_t Pin, gpio_func_t function, gpio_pad_t pad);


#endif
