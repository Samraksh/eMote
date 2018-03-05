#pragma once
#include <stm32f10x.h>
#include <tinyhal.h>

#ifndef PLATFORM_ARM_AUSTERE
#error "ERROR, THIS IS NOT AUSTERE PLATORM"
#endif

#define AUSTERE_BIG_CAP		GPIO_Pin_11
#define AUSTERE_1V8 		GPIO_Pin_6
#define AUSTERE_2V5			GPIO_Pin_13

#define IS_AUSTERE_PWR_PIN(x) ( (x == AUSTERE_BIG_CAP) || (x == AUSTERE_1V8) || (x == AUSTERE_2V5) )

#define AUSTERE_RADIO_OFF	1
#define AUSTERE_RADIO ON	0

#define AUSTERE_PWR_BAD		0

void austere_init(void);
void austere_power_init(void);
void print_power_supply_status(void);

int platform_radio_pwr_ctrl(int on, int radio __attribute__((unused)));
int platform_power_event_sub(HAL_CONTINUATION *cb);