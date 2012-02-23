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
#include "..\TIAM3517_SCM_PinMux\TIAM3517_PinMux.h"


TIAM3517_GPIO_Driver g_TIAM3517_GPIO_Driver;

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
const UINT8 TIAM3517_GPIO_Driver::c_Gpio_Attributes[TIAM3517_GPIO_Driver::c_MaxPins] =
{
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   0   (register 0)
    GPIO_ATTRIBUTE_NONE                         , //   1
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   2
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   3
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   4
    GPIO_ATTRIBUTE_NONE                         , //   5
    GPIO_ATTRIBUTE_NONE                         , //   6
    GPIO_ATTRIBUTE_NONE                         , //   7
    GPIO_ATTRIBUTE_NONE                         , //   8
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   9
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  10
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  11
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  12
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  13
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  14
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  15
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  16
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  17
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  18
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  19
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  20
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  21
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  22
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  23
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  24
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  25
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  26
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  27
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  28
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  29
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  30
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  31
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  32   (register 1)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  33
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  34
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  35
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  36
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  37
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  38
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  39
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  40
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  41
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  42
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  43
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  44
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  45
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  46
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  47
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  48
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  49
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  50
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  51
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  52
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  53
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  54
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  55
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  56
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  57
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  58
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  59
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  60
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  61
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  62
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  63
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  64   (register 2)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  65
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  66
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  67
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  68
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  69
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  70
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  71
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  72
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  73
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  74
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  75
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  76
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  77
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  78
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  79
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  80
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  81
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  82
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  83
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  84
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  85
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  86
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  87
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  88
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  89
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  90
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  91
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  92
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  93
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  94
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  95
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  96   (register 3)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  97
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  98
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  99
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 100
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 101
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 102
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 103
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 104
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 105
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 106
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 107
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 108
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 109
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 110
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 111
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 112
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 113
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 114
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 115
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 116
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 117
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 118
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 119
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT  // 120
};

//struct gpio_bank gpio_bank1;

static const struct gpio_bank gpio_bank_am4317[6] = {
	{ (void *)OMAP34XX_GPIO1_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO2_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO3_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO4_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO5_BASE, METHOD_GPIO_24XX },
	{ (void *)OMAP34XX_GPIO6_BASE, METHOD_GPIO_24XX },
};

//static const struct gpio_bank *gpio_bank_alias = &gpio_bank_34xx[0];

BOOL TIAM3517_GPIO_Driver::Initialize()
{
	TIAM3517_CLOCK_MANAGER_PRCM &CMGRPRCM = TIAM3517::CMGRPRCM();
	UINT32 GPIO_reg = CMGRPRCM.iclken_per;
	for(UINT32 i = 0; i < c_MaxPorts; i++)
	{
		g_TIAM3517_GPIO_Driver.m_PinReservationInfo[i] = 0;
	}
	TIAM3517_PinMux::Initialize();
}

void TIAM3517_GPIO_Driver::EnableOutputPin(GPIO_PIN pin, BOOL initialState)
{
	TIAM3517_PinMux::setMuxGPIO((UINT32)pin);
	omap_set_gpio_direction((int) pin,(int) 0);
	omap_set_gpio_dataout((int) pin, (int) initialState);
}

UINT32 TIAM3517_GPIO_Driver::Attributes( GPIO_PIN Pin )
{
    if(Pin < c_MaxPins)
    {
        return c_Gpio_Attributes[Pin];
    }

    return GPIO_ATTRIBUTE_NONE;
}



static void _set_gpio_dataout(struct gpio_bank *bank, int gpio, int enable)
{
	UINT32 reg = (UINT32)bank->base;
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

BOOL TIAM3517_GPIO_Driver::ReservePin( GPIO_PIN pin, BOOL fReserve )
{
    GLOBAL_LOCK(irq);

    UINT32 bit = pin & 31;
    UINT32& res  = g_TIAM3517_GPIO_Driver.m_PinReservationInfo[pin >> 5];
    UINT32  mask = (fReserve ? 1u : 0u) << bit;

    if((res & (1u << bit)) == mask) return FALSE;

    if(fReserve)
    {
        res |= (1u << bit);
    }
    else
    {
        res &= ~(1u << bit);
    }

    return TRUE;
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
	UINT32 reg = (UINT32)bank->base;
	UINT32 l;

	switch (bank->method) {
	case METHOD_GPIO_24XX:
		reg += OMAP24XX_GPIO_OE;
		break;
	default:
		return;
	}
	l = __raw_readl((void *)reg);
	if (is_input)
		l |= 1 << gpio;
	else
		l &= ~(1 << gpio);
	__raw_writel(l, (void *)reg);
}


void omap_set_gpio_direction(int gpio, int is_input)
{
	struct gpio_bank *bank;

	if (check_gpio(gpio) < 0)
		return;
	bank = get_gpio_bank(gpio);
	int pin=get_gpio_index(gpio);

	_set_gpio_direction(bank, pin, is_input);
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
	UINT32 pin_bank = gpio >> 5;
	struct gpio_bank *bank_base =(struct gpio_bank *) &gpio_bank_am4317[pin_bank];
	return bank_base;
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
	omap_set_gpio_dataout((int) pin, (int) pinState);
}


