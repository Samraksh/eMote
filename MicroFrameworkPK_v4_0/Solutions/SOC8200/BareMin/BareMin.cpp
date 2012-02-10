/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

//#include <common.h>
//#include <exports.h>
#include "..\..\..\DeviceCode\Targets\Native\TIAM3517\DeviceCode\TIAM3517.h"
//#include <common.h>
//#include <asm/arch/gpio.h>
//#include <asm/io.h>
//#include <asm/errno.h>

#define TIME 50000
#define DUR 500000
#define AA 568>>1
#define GG 638>>1
#define CC 956>>1

typedef int u32;
void playC(void);
void playG(void);
void playA(void);
void omap_set_gpio_dataout(int gpio, int enable);
void omap_free_gpio(int gpio);
int omap_request_gpio(int gpio);
void omap_set_gpio_direction(int gpio, int is_input);


extern "C"
{
extern void printf ( const char* format , ... );
extern int getc(void);
}

void ApplicationEntryPoint();

extern "C"
{
void myEntry()
{
	ApplicationEntryPoint();
}
}

void ApplicationEntryPoint ()
{
	int g = 58;
	int i = 0;

	/* Print the ABI version */
	//app_startup(argv);
	//printf ("Hello Samraksh\n");
	
	omap_free_gpio(g);
	omap_request_gpio(g);
	omap_set_gpio_direction(g, 0);
	omap_set_gpio_dataout(g, 1);

	printf ("Hit any key to exit ... ");

	(void) getc();
	for(int i = 0; i < 1000; i++)
		printf("MicroFramework Porting On Its Way %d!!!!\n",i);
	// while (!tstc()) {
		// udelay(1023);
		// i++;
		// if (i % 2 == 0)
			// omap_set_gpio_dataout(g, 0);
		// else
			// omap_set_gpio_dataout(g, 1);
	// }
	
	/* consume input */
	
	playC();
	playC();
	playG();
	playG();
	playA();
	playA();
	playG();

	omap_free_gpio(g);
	
	printf ("\n\n");
}

void playC(void) {
	int i=0;
	int g=58;
	int d=CC;
	d--;
	int time = DUR/d;
	while (time > 0) {
		//udelay(d);
		if(i%2==0)
			omap_set_gpio_dataout(g, 0);
		else
			omap_set_gpio_dataout(g, 1);
		i++;
		time--;
	}
	omap_set_gpio_dataout(g, 1);
	//udelay(TIME);
}

void playG(void) {
	int i=0;
	int g=58;
	int d=GG;
	d--;
	int time = DUR/d;
	while (time > 0) {
		//udelay(d);
		if(i%2==0)
			omap_set_gpio_dataout(g, 0);
		else
			omap_set_gpio_dataout(g, 1);
		i++;
		time--;
	}
	omap_set_gpio_dataout(g, 1);
	//udelay(TIME);
}

void playA(void) {
	int i=0;
	int g=58;
	int d=AA;
	d--;
	int time = DUR/d;
	while (time > 0) {
		//udelay(d);
		if(i%2==0)
			omap_set_gpio_dataout(g, 0);
		else
			omap_set_gpio_dataout(g, 1);
		i++;
		time--;
	}
	omap_set_gpio_dataout(g, 1);
	//udelay(TIME);
}

static struct gpio_bank gpio_bank_34xx[6] = {
	{ (void *)OMAP34XX_GPIO1_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO2_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO3_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO4_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO5_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO6_BASE, METHOD_GPIO_24XX },
};

static struct gpio_bank *gpio_bank = &gpio_bank_34xx[0];

static inline struct gpio_bank *get_gpio_bank(int gpio)
{
	return &gpio_bank[gpio >> 5];
}

static inline int get_gpio_index(int gpio)
{
	return gpio & 0x1f;
}

static inline int gpio_valid(int gpio)
{
	if (gpio < 0)
		return -1;
	if (gpio < 192)
		return 0;
	return -1;
}

static int check_gpio(int gpio)
{
	if (gpio_valid(gpio) < 0) {
		//printf("ERROR : check_gpio: invalid GPIO %d\n", gpio);
		return -1;
	}
	return 0;
}

static void _set_gpio_direction(struct gpio_bank *bank, int gpio, int is_input)
{
	void *reg = bank->base;
	u32 l;
	
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

static void _set_gpio_dataout(struct gpio_bank *bank, int gpio, int enable)
{
	void *reg = bank->base;
	u32 l = 0;
	

	switch (bank->method) {
	case METHOD_GPIO_24XX:
		if (enable)
			reg += OMAP24XX_GPIO_SETDATAOUT;
		else
			reg += OMAP24XX_GPIO_CLEARDATAOUT;
		l = 1 << gpio;
		break;
	default:
		//printf("omap3-gpio unknown bank method %s %d\n",
		  //     __FILE__, __LINE__);
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

int omap_get_gpio_datain(int gpio)
{
	struct gpio_bank *bank;
	void *reg;

	if (check_gpio(gpio) < 0)
		return -1;
	bank = get_gpio_bank(gpio);
	reg = bank->base;
	switch (bank->method) {
	case METHOD_GPIO_24XX:
		reg += OMAP24XX_GPIO_DATAIN;
		break;
	default:
		return -1;
	}
	return (__raw_readl(reg)
			& (1 << get_gpio_index(gpio))) != 0;
}

static void _reset_gpio(struct gpio_bank *bank, int gpio)
{
	_set_gpio_direction(bank, get_gpio_index(gpio), 1);
}

int omap_request_gpio(int gpio)
{
	if (check_gpio(gpio) < 0)
		return -1;

	return 0;
}

void omap_free_gpio(int gpio)
{
	struct gpio_bank *bank;

	if (check_gpio(gpio) < 0)
		return;
	bank = get_gpio_bank(gpio);

	_reset_gpio(bank, gpio);
}
