/*******************************************************************************
 * (c) Copyright 2008-2013 Microsemi SoC Products Group.  All rights reserved.
 * 
 * SmartFusion2 microcontroller subsystem GPIO bare metal driver implementation.
 *
 * SVN $Revision: 5394 $
 * SVN $Date: 2013-03-27 20:56:36 +0000 (Wed, 27 Mar 2013) $
 */
#include "netmf_gpio.h"
#include <cmsis/mss_assert.h>
#include <drivers/CoreGPIO/core_gpio.h>
#include <tinyhal.h>

gpio_instance_t g_gpio;

/*-------------------------------------------------------------------------*//**
 * Defines.
 */
#define GPIO_INT_ENABLE_MASK        ((uint32_t)0x00000008uL)
#define OUTPUT_BUFFER_ENABLE_MASK   0x00000004u

const int GPIO_PINS = 112;
#define NB_OF_GPIO  ((uint32_t)32)
typedef void (*GPIO_INTERRUPT_SERVICE_ROUTINE)( GPIO_PIN Pin, BOOL PinState, void* Param );
static GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr[GPIO_PINS];


BOOL CPU_GPIO_Initialize()
{
	//GPIO_init( &g_gpio,    COREGPIO_BASE_ADDR, GPIO_APB_32_BITS_BUS );
	MSS_GPIO_init();
	return TRUE;
}

void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
	//GPIO_config( &g_gpio, GPIO_0, GPIO_OUTPUT_MODE );
	MSS_GPIO_config( (mss_gpio_id_t)Pin , InitialState );
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
	/*uint32_t gpio_outputs;
	//uint32_t mask = 1 << Pin;
    gpio_outputs = GPIO_get_outputs( &g_gpio );
	if  (PinState == TRUE){
		gpio_outputs &= GPIO_0_MASK;
		//gpio_outputs &= mask;
	} else {
		gpio_outputs &= ~GPIO_0_MASK;
		//gpio_outputs &= ~mask;
	}
    GPIO_set_outputs( &g_gpio, gpio_outputs );*/
	MSS_GPIO_set_output( (mss_gpio_id_t)Pin, PinState );
}

INT32  CPU_GPIO_GetPinCount    ()
{
	return 1;
}

BOOL   CPU_GPIO_ReservePin     ( GPIO_PIN Pin, BOOL fReserve )
{
	return TRUE;
}

BOOL   CPU_GPIO_Uninitialize   ()
{
	return TRUE;
}

UINT32 CPU_GPIO_GetDebounce    ()
{
	return 1;
}

BOOL   CPU_GPIO_SetDebounce    ( INT64 debounceTimeMilliseconds )
{
	return TRUE;
}

void   CPU_GPIO_GetPinsMap     ( UINT8* pins, size_t size )
{
}

UINT32 CPU_GPIO_Attributes     ( GPIO_PIN Pin )
{
	return 0;
}

UINT8  CPU_GPIO_GetSupportedResistorModes(GPIO_PIN pin )
{
	return 1;
}

UINT8 CPU_GPIO_GetSupportedInterruptModes( GPIO_PIN pin )
{
    return 1;
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	return TRUE;
}

BOOL   CPU_GPIO_GetPinState    ( GPIO_PIN Pin )
{
	uint32_t gpio_outputs;
	uint32_t mask = 1 << Pin;
    gpio_outputs = GPIO_get_outputs( &g_gpio );
	
	if ( (gpio_outputs & mask) > 0)
		return TRUE;
	else 
		return FALSE;
}

void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
	
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	return FALSE;
}
/*-------------------------------------------------------------------------*//**
 * Lookup table of GPIO configuration registers address indexed on GPIO ID.
 */
static uint32_t volatile * const g_config_reg_lut[NB_OF_GPIO] =
{
    &(GPIO->GPIO_0_CFG),
    &(GPIO->GPIO_1_CFG),
    &(GPIO->GPIO_2_CFG),
    &(GPIO->GPIO_3_CFG),
    &(GPIO->GPIO_4_CFG),
    &(GPIO->GPIO_5_CFG),
    &(GPIO->GPIO_6_CFG),
    &(GPIO->GPIO_7_CFG),
    &(GPIO->GPIO_8_CFG),
    &(GPIO->GPIO_9_CFG),
    &(GPIO->GPIO_10_CFG),
    &(GPIO->GPIO_11_CFG),
    &(GPIO->GPIO_12_CFG),
    &(GPIO->GPIO_13_CFG),
    &(GPIO->GPIO_14_CFG),
    &(GPIO->GPIO_15_CFG),
    &(GPIO->GPIO_16_CFG),
    &(GPIO->GPIO_17_CFG),
    &(GPIO->GPIO_18_CFG),
    &(GPIO->GPIO_19_CFG),
    &(GPIO->GPIO_20_CFG),
    &(GPIO->GPIO_21_CFG),
    &(GPIO->GPIO_22_CFG),
    &(GPIO->GPIO_23_CFG),
    &(GPIO->GPIO_24_CFG),
    &(GPIO->GPIO_25_CFG),
    &(GPIO->GPIO_26_CFG),
    &(GPIO->GPIO_27_CFG),
    &(GPIO->GPIO_28_CFG),
    &(GPIO->GPIO_29_CFG),
    &(GPIO->GPIO_30_CFG),
    &(GPIO->GPIO_31_CFG)
};

/*-------------------------------------------------------------------------*//**
 * Lookup table of Cortex-M3 GPIO interrupt number indexed on GPIO ID.
 */
static const IRQn_Type g_gpio_irqn_lut[NB_OF_GPIO] =
{
    GPIO0_IRQn,
    GPIO1_IRQn,
    GPIO2_IRQn,
    GPIO3_IRQn,
    GPIO4_IRQn,
    GPIO5_IRQn,
    GPIO6_IRQn,
    GPIO7_IRQn,
    GPIO8_IRQn,
    GPIO9_IRQn,
    GPIO10_IRQn,
    GPIO11_IRQn,
    GPIO12_IRQn,
    GPIO13_IRQn,
    GPIO14_IRQn,
    GPIO15_IRQn,
    GPIO16_IRQn,
    GPIO17_IRQn,
    GPIO18_IRQn,
    GPIO19_IRQn,
    GPIO20_IRQn,
    GPIO21_IRQn,
    GPIO22_IRQn,
    GPIO23_IRQn,
    GPIO24_IRQn,
    GPIO25_IRQn,
    GPIO26_IRQn,
    GPIO27_IRQn,
    GPIO28_IRQn,
    GPIO29_IRQn,
    GPIO30_IRQn,
    GPIO31_IRQn
};

/*-------------------------------------------------------------------------*//**
 * MSS_GPIO_init
 * See "mss_gpio.h" for details of how to use this function.
 */
void MSS_GPIO_init( void )
{
    uint32_t inc;
    
    /* reset MSS GPIO hardware */
    SYSREG->SOFT_RST_CR |= SYSREG_GPIO_SOFTRESET_MASK;
    SYSREG->SOFT_RST_CR |= (SYSREG_GPIO_7_0_SOFTRESET_MASK |
                            SYSREG_GPIO_15_8_SOFTRESET_MASK |
                            SYSREG_GPIO_23_16_SOFTRESET_MASK |
                            SYSREG_GPIO_31_24_SOFTRESET_MASK);
                            
    /* Clear any previously pended MSS GPIO interrupt */
    for(inc = 0U; inc < NB_OF_GPIO; ++inc)
    {
        NVIC_DisableIRQ(g_gpio_irqn_lut[inc]);
        NVIC_ClearPendingIRQ(g_gpio_irqn_lut[inc]);
    }
    /* Take MSS GPIO hardware out of reset. */
    SYSREG->SOFT_RST_CR &= ~(SYSREG_GPIO_7_0_SOFTRESET_MASK |
                             SYSREG_GPIO_15_8_SOFTRESET_MASK |
                             SYSREG_GPIO_23_16_SOFTRESET_MASK |
                             SYSREG_GPIO_31_24_SOFTRESET_MASK);
    SYSREG->SOFT_RST_CR &= ~SYSREG_GPIO_SOFTRESET_MASK;
}

/*-------------------------------------------------------------------------*//**
 * MSS_GPIO_config
 * See "mss_gpio.h" for details of how to use this function.
 */
void MSS_GPIO_config
(
    mss_gpio_id_t port_id,
    uint32_t config
)
{
    uint32_t gpio_idx = (uint32_t)port_id;
    
    ASSERT(gpio_idx < NB_OF_GPIO);

    if(gpio_idx < NB_OF_GPIO)
    {
        *(g_config_reg_lut[gpio_idx]) = config;
    }
}

/*-------------------------------------------------------------------------*//**
 * MSS_GPIO_set_output
 * See "mss_gpio.h" for details of how to use this function.
 */
void MSS_GPIO_set_output
(
    mss_gpio_id_t port_id,
    uint8_t value
)
{
    uint32_t gpio_setting;
    uint32_t gpio_idx = (uint32_t)port_id;
    
    ASSERT(gpio_idx < NB_OF_GPIO);
    
    if(gpio_idx < NB_OF_GPIO)
    {
        gpio_setting = GPIO->GPIO_OUT;
        gpio_setting &= ~((uint32_t)0x01u << gpio_idx);
        gpio_setting |= ((uint32_t)value & 0x01u) << gpio_idx;
        GPIO->GPIO_OUT = gpio_setting;
    }
}

/*-------------------------------------------------------------------------*//**
 * MSS_GPIO_drive_inout
 * See "mss_gpio.h" for details of how to use this function.
 */
void MSS_GPIO_drive_inout
(
    mss_gpio_id_t port_id,
    mss_gpio_inout_state_t inout_state
)
{
    uint32_t outputs_state;
    uint32_t config;
    uint32_t gpio_idx = (uint32_t)port_id;
    
    ASSERT(gpio_idx < NB_OF_GPIO);
    
    if(gpio_idx < NB_OF_GPIO)
    {
        switch(inout_state)
        {
            case MSS_GPIO_DRIVE_HIGH:
                /* Set output high */
                outputs_state = GPIO->GPIO_OUT;
                outputs_state |= (uint32_t)1 << gpio_idx;
                GPIO->GPIO_OUT = outputs_state;
                /* Enable output buffer */
                config = *(g_config_reg_lut[gpio_idx]);
                config |= OUTPUT_BUFFER_ENABLE_MASK;
                *(g_config_reg_lut[gpio_idx]) = config;
            break;
                
            case MSS_GPIO_DRIVE_LOW:
                /* Set output low */
                outputs_state = GPIO->GPIO_OUT;
                outputs_state &= ~((uint32_t)((uint32_t)1 << gpio_idx));
                GPIO->GPIO_OUT = outputs_state;
                /* Enable output buffer */
                config = *(g_config_reg_lut[gpio_idx]);
                config |= OUTPUT_BUFFER_ENABLE_MASK;
                *(g_config_reg_lut[gpio_idx]) = config;
            break;
                
            case MSS_GPIO_HIGH_Z:
                /* Disable output buffer */
                config = *(g_config_reg_lut[gpio_idx]);
                config &= ~OUTPUT_BUFFER_ENABLE_MASK;
                *(g_config_reg_lut[gpio_idx]) = config;
            break;
                
            default:
                ASSERT(0);
            break;
        }
    }
}

/*-------------------------------------------------------------------------*//**
 * MSS_GPIO_enable_irq
 * See "mss_gpio.h" for details of how to use this function.
 */
void MSS_GPIO_enable_irq
(
    mss_gpio_id_t port_id
)
{
    uint32_t cfg_value;
    uint32_t gpio_idx = (uint32_t)port_id;
    
    ASSERT(gpio_idx < NB_OF_GPIO);
    
    if(gpio_idx < NB_OF_GPIO)
    {
        cfg_value = *(g_config_reg_lut[gpio_idx]);
        *(g_config_reg_lut[gpio_idx]) = (cfg_value | GPIO_INT_ENABLE_MASK);
        NVIC_EnableIRQ(g_gpio_irqn_lut[gpio_idx]);
    }
}

/*-------------------------------------------------------------------------*//**
 * MSS_GPIO_disable_irq
 * See "mss_gpio.h" for details of how to use this function.
 */
void MSS_GPIO_disable_irq
(
    mss_gpio_id_t port_id
)
{
    uint32_t cfg_value;
    uint32_t gpio_idx = (uint32_t)port_id;
    
    ASSERT(gpio_idx < NB_OF_GPIO);

    if(gpio_idx < NB_OF_GPIO)
    {
        cfg_value = *(g_config_reg_lut[gpio_idx]);
        *(g_config_reg_lut[gpio_idx]) = (cfg_value & ~GPIO_INT_ENABLE_MASK);
    }
}

/*-------------------------------------------------------------------------*//**
 * MSS_GPIO_clear_irq
 * See "mss_gpio.h" for details of how to use this function.
 */
void MSS_GPIO_clear_irq
(
    mss_gpio_id_t port_id
)
{
    uint32_t gpio_idx = (uint32_t)port_id;
    
    ASSERT(gpio_idx < NB_OF_GPIO);
    
    if(gpio_idx < NB_OF_GPIO)
    {
        GPIO->GPIO_IRQ = ((uint32_t)1) << gpio_idx;
    }
    __ASM volatile ("dsb");

}


