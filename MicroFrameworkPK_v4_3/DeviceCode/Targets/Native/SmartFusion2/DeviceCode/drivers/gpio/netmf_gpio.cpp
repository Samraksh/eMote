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
#include <CMSIS/system_m2sxxx.h>
#include <CMSIS/m2sxxx.h>
#include <tinyhal.h>

gpio_instance_t g_gpio;

/*-------------------------------------------------------------------------*//**
 * Defines.
 */
#define GPIO_INT_ENABLE_MASK        ((uint32_t)0x00000008uL)
#define OUTPUT_BUFFER_ENABLE_MASK   0x00000004u

#define NB_OF_GPIO  ((uint32_t)32)
const int GPIO_PINS = NB_OF_GPIO;
typedef void (*GPIO_INTERRUPT_SERVICE_ROUTINE)( GPIO_PIN Pin, BOOL PinState, void* Param );
static GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr[GPIO_PINS];
static void* gpio_parm[GPIO_PINS];
static uint8_t pin_reservations[GPIO_PINS]; 

static void GPIO0_IRQ_HANDLER(void *args);
static void GPIO1_IRQ_HANDLER(void *args);
static void GPIO2_IRQ_HANDLER(void *args);

static void handle_exti(unsigned int pin)
{
	hal_printf("hexti %d\r\n", pin);
		GPIO_INTERRUPT_SERVICE_ROUTINE my_isr;
		void *parm;

		// The C# GPIO ISR will add this interrupt to g_CLR_HW_Hardware.m_interruptData.m_HalQueue via SaveToHALQueue(), which eventually gets added to m_interruptData.m_applicationQueue via CLR_HW_Hardware::TransferAllInterruptsToApplicationQueue() which eventually gets dispatched via CLR_HW_Hardware::SpawnDispatcher()
		my_isr = gpio_isr[pin];
		parm = gpio_parm[pin];
		if(my_isr != NULL)
		{
		    my_isr(pin, CPU_GPIO_GetPinState(pin), parm);
		}
		// the code below was being triggered when a radio app was being deployed. During the app erase, the radio driver is uninitialized (including the GPIO interrupt line to the radio). The erase routine is
		// global locked and at some point the radio could request an interrupt which is not able to be serviced until after the erase is over. At that point the interupt fires but the GPIO interrupt line
		// has already been disabled and the callback function is now null. 
		// If problems occur from this code being commented out we can instead just clear the radio interrupt that is pending instead.
		//else
		//{
		    //GPIO_DEBUG_PRINT("%s",c_strGpioBadCallback);
		    //ASSERT(my_isr != NULL);
		//}
}

BOOL CPU_GPIO_Initialize()
{
	hal_printf("CPU_GPIO_Initialize\r\n");
	MSS_GPIO_init();
	return TRUE;
}

void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
	MSS_GPIO_config( (mss_gpio_id_t)Pin , MSS_GPIO_OUTPUT_MODE );
	MSS_GPIO_set_output( (mss_gpio_id_t)Pin, InitialState );
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
	MSS_GPIO_set_output( (mss_gpio_id_t)Pin, PinState );
}

UINT32 GPIO_GetIRQNumber(GPIO_PIN Pin)
{
	if(Pin > GPIO_PINS)
	{
		return FALSE;
	}
	UINT32 irq_number;

	switch(Pin)
	{
	case	0:
		irq_number = GPIO0_IRQn;
		break;
	case 	1:
		irq_number = GPIO1_IRQn;
		break;
	case 	2:
		irq_number = GPIO2_IRQn;
		break;
	}

	return irq_number;
}

HAL_CALLBACK_FPN GPIO_GetCallBack(GPIO_PIN Pin)
{
	HAL_CALLBACK_FPN callback;

	switch(Pin)
	{
	case	0:
		callback = GPIO0_IRQ_HANDLER;
		break;
	case 	1:
		callback = GPIO1_IRQ_HANDLER;
		break;
	case 	2:
		callback = GPIO2_IRQ_HANDLER;
		break;
	}

	return callback;
}

INT32 CPU_GPIO_GetPinCount()
{
	return GPIO_PINS;
}

BOOL CPU_GPIO_ReservePin( GPIO_PIN Pin, BOOL fReserve )
{
	if(Pin > GPIO_PINS)
	{
		return FALSE;
	}

	if (pin_reservations[Pin]) {
		if (fReserve) { return FALSE; } // Can't reserve it, busy, fail.
		else { pin_reservations[Pin] = 0; } // Un-reserve a busy pin.
	}
	else {
		if (fReserve) { pin_reservations[Pin] = 1; } // Reserve it.
		// Nothing to do for case where un-reserving a non-busy pin
	}
	return TRUE;
}

BOOL   CPU_GPIO_Uninitialize   ()
{
	return TRUE;
}

UINT32 CPU_GPIO_GetDebounce    ()
{
	return 0;
}

BOOL   CPU_GPIO_SetDebounce    ( INT64 debounceTimeMilliseconds )
{
	return FALSE;
}

void   CPU_GPIO_GetPinsMap     ( UINT8* pins, size_t size )
{
	if(size > GPIO_PINS)
	{
		return;
	}
    UINT8 i;
	for(i = 0; i < size; i++) {
		pins[i] = CPU_GPIO_Attributes(i);	//GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT;
	}
}

UINT32 CPU_GPIO_Attributes     ( GPIO_PIN Pin )
{
	if(Pin > GPIO_PINS)
	{
		return 0;
	}
	return (GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT);
}

UINT8  CPU_GPIO_GetSupportedResistorModes(GPIO_PIN pin )
{
	return ((1<<RESISTOR_DISABLED) | (1<<RESISTOR_PULLUP) | (1 << RESISTOR_PULLDOWN));
}

UINT8 CPU_GPIO_GetSupportedInterruptModes( GPIO_PIN pin )
{
    return ((1<<GPIO_INT_EDGE_LOW) | (1<<GPIO_INT_EDGE_HIGH ) | (1<<GPIO_INT_EDGE_BOTH));
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	hal_printf("CPU_GPIO_EnableInputPin2 %d\r\n", Pin);
	if(Pin > GPIO_PINS)
	{
		return FALSE;
	}

	if(PIN_ISR)
	{
		if(GPIO_INT_EDGE_LOW == IntEdge)
			MSS_GPIO_config((mss_gpio_id_t)Pin, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_NEGATIVE);
		else if(GPIO_INT_EDGE_HIGH == IntEdge)
			MSS_GPIO_config((mss_gpio_id_t)Pin, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
		else if(GPIO_INT_EDGE_BOTH == IntEdge)
			MSS_GPIO_config((mss_gpio_id_t)Pin, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_BOTH);
		else if(GPIO_INT_LEVEL_HIGH == IntEdge)
		   MSS_GPIO_config((mss_gpio_id_t)Pin, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_LEVEL_HIGH);
		else if (GPIO_INT_LEVEL_LOW == IntEdge)
			MSS_GPIO_config((mss_gpio_id_t)Pin, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_LEVEL_LOW);
		else
			return FALSE;

		gpio_isr[Pin] = PIN_ISR;
		gpio_parm[Pin] = ISR_Param;

		MSS_GPIO_enable_irq((mss_gpio_id_t)Pin);
		CPU_INTC_ActivateInterrupt(GPIO_GetIRQNumber(Pin), (HAL_CALLBACK_FPN) GPIO_GetCallBack(Pin), NULL);
	}
	else
	{
		gpio_isr[Pin] = NULL;
		gpio_parm[Pin] = NULL;
	}

	return TRUE;
}

BOOL   CPU_GPIO_GetPinState    ( GPIO_PIN Pin )
{
	uint32_t input = MSS_GPIO_get_inputs();
	uint32_t mask = 1 << Pin;
	
	if ( (input & mask) > 0)
		return TRUE;
	else 
		return FALSE;
}

void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
	if(Pin > GPIO_PINS)
	{
		return;
	}

	MSS_GPIO_config( (mss_gpio_id_t)Pin , MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_LEVEL_HIGH );
	CPU_GPIO_SetPinState(Pin, FALSE);

	if ( gpio_isr[Pin] != NULL ) { // Need to deconfigure its interrupt as well.
		MSS_GPIO_disable_irq((mss_gpio_id_t)Pin);
		// Disable in NVIC
		CPU_INTC_InterruptDisable(GPIO_GetIRQNumber(Pin));
	}

	// Remove any user interrupts
	gpio_isr[Pin] = NULL;
	gpio_parm[Pin] = NULL;	
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE PIN_ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
	hal_printf("CPU_GPIO_EnableInputPin %d\r\n", Pin);
	if(Pin > GPIO_PINS)
	{
		return FALSE;
	}

	// Short circuit for simple inputs.
	if ( PIN_ISR == NULL && IntEdge == GPIO_INT_NONE && ResistorState == RESISTOR_DISABLED && GlitchFilterEnable == false ) {
		MSS_GPIO_config((mss_gpio_id_t)Pin, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
		return TRUE;
	}

	return CPU_GPIO_EnableInputPin2(Pin, GlitchFilterEnable, PIN_ISR, NULL, IntEdge, ResistorState );
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
	hal_printf("MSS_GPIO_config %d 0x%x\r\n", port_id, config);
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
	hal_printf("MSS_GPIO_enable_irq %d\r\n", port_id);
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

void GPIO0_IRQ_HANDLER(void *args)
{
	hal_printf("GPIO0_IRQ_HANDLER\r\n");
	handle_exti(0);
}

void GPIO1_IRQ_HANDLER(void *args)
{
	hal_printf("GPIO1_IRQ_HANDLER\r\n");
	handle_exti(1);
}

void GPIO2_IRQ_HANDLER(void *args)
{
	hal_printf("GPIO2_IRQ_HANDLER\r\n");
	handle_exti(2);
}
