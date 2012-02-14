/*  Name : TIAM3517_GPIO.cpp
 *
 *  Author : Nived.Sivadas@Samraksh.com
 *
 *  Description :
 *
 *
 *
 */
#include "..\TIAM3517.h"
#include <tinyhal.h>



static void _set_gpio_dataout(struct gpio_bank *bank, int gpio, int enable);
void omap_set_gpio_dataout(int gpio, int enable);
static void _set_gpio_direction(struct gpio_bank *bank, int gpio, int is_input);
void omap_set_gpio_direction(int gpio, int is_input);
int omap_request_gpio(int gpio);
static inline int get_gpio_index(int gpio);
static inline int gpio_valid(int gpio);
static inline struct gpio_bank *get_gpio_bank(int gpio);
static int check_gpio(int gpio);

// Copied from uboot\cpu\arm_cortexa8\omap3\gpio.c

static struct gpio_bank gpio_bank_34xx[6] = {
	{ (void *)OMAP34XX_GPIO1_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO2_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO3_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO4_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO5_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO6_BASE, METHOD_GPIO_24XX },
};

static struct gpio_bank *gpio_bank = &gpio_bank_34xx[0];



BOOL TIAM3517_GPIO_Driver::Initialize()
{






}

void TIAM3517_GPIO_Driver::EnableOutputPin(GPIO_PIN pin, BOOL initialState)
{


	omap_set_gpio_dataout((int) pin, (int) initialState);


}




static void _set_gpio_dataout(struct gpio_bank *bank, int gpio, int enable)
{
	void *reg = bank->base;
	UINT32 l = 0;
	

	switch (bank->method) {
	case METHOD_GPIO_24XX:
		if (enable)
			reg += OMAP24XX_GPIO_SETDATAOUT;
		else
			reg += OMAP24XX_GPIO_CLEARDATAOUT;
		l = 1 << gpio;
		break;
	default:
		hal_printf("omap3-gpio unknown bank method "
		       );
		return;
	}
	__raw_writel(l, reg);
}


void omap_set_gpio_dataout(int gpio, int enable)
{
	struct gpio_bank *bank;

	if (check_gpio(gpio) < 0)
		return;
	bank = get_gpio_bank(gpio);
	_set_gpio_dataout(bank, get_gpio_index(gpio), enable);
}


static void _set_gpio_direction(struct gpio_bank *bank, int gpio, int is_input)
{
	void *reg = bank->base;
	UINT32 l;
	
	switch (bank->method) {
	case METHOD_GPIO_24XX:
		reg += OMAP24XX_GPIO_OE;
		break;
	default:
		return;
	}
	l = __raw_readl(reg);
	if (is_input)
		l |= 1 << gpio;
	else
		l &= ~(1 << gpio);
	__raw_writel(l, reg);
}


void omap_set_gpio_direction(int gpio, int is_input)
{
	struct gpio_bank *bank;

	if (check_gpio(gpio) < 0)
		return;
	bank = get_gpio_bank(gpio);
	_set_gpio_direction(bank, get_gpio_index(gpio), is_input);
}


int omap_request_gpio(int gpio)
{
	if (check_gpio(gpio) < 0)
		return -1;

	return 0;
}


static inline int get_gpio_index(int gpio)
{
	return gpio & 0x1f;
}


// Copied from uboot\cpu\arm_cortexa8\omap3\gpio.c
static inline int gpio_valid(int gpio)
{
	if (gpio < 0)
		return -1;
	if (gpio < 192)
		return 0;
	return -1;
}



// Copied from uboot\cpu\arm_cortexa8\omap3\gpio.c
static int check_gpio(int gpio)
{
	if (gpio_valid(gpio) < 0) {
		return -1;
	}
	return 0;
}


// Copied from uboot\cpu\arm_cortexa8\omap3\gpio.c
static inline struct gpio_bank *get_gpio_bank(int gpio)
{
	return &gpio_bank[gpio >> 5];
}

static void _reset_gpio(struct gpio_bank *bank, int gpio)
{
	_set_gpio_direction(bank, get_gpio_index(gpio), 1);
}

void omap_free_gpio(int gpio)
{
	struct gpio_bank *bank;

	if (check_gpio(gpio) < 0)
		return;
	bank = get_gpio_bank(gpio);

	_reset_gpio(bank, gpio);
}

void playC(void) {
	int i=0;
	int g=58;
	//int d=CC;
	//d--;
	int time = 40000/1;
	while (time > 0) {
//		udelay(d);
		if(i%2==0)
			omap_set_gpio_dataout(g, 0);
		else
			omap_set_gpio_dataout(g, 1);
		i++;
		time--;
	}
	omap_set_gpio_dataout(g, 1);
//	udelay(TIME);
}

// Contents copied from uboot\cpu\arm_cortexa8\omap3\gpio.c
void TIAM3517_GPIO_Driver::SetPinState(GPIO_PIN pin, BOOL pinState)
{
	int count = 0;
	//omap_free_gpio(58);
	
	//omap_request_gpio(58);
	//omap_set_gpio_direction(58, 0);
	//omap_set_gpio_dataout(58, 1);
    
	/*
	while(count < 10000)
	{
		playC();
		count++;
	}
	*/


}


