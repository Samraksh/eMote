

#ifndef TIAM3517_GPIO_H
#define TIAM3517_GPIO_H

void omap_set_gpio_dataout(int gpio, int enable);
void omap_set_gpio_direction(int gpio, int is_input);
int omap_request_gpio(int gpio);
UINT8 omap_get_gpio_input(int gpio);

#endif
