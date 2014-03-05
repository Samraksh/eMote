/*  Name : Krait_GPIO.cpp
 *
 *  Author : Mukundan.Sridharan@Samraksh.com
 *
 *  Description :
 *
 */
#include <tinyhal.h>
#include "..\Krait.h"
#include "Krait__GPIO.h"


Krait_GPIO_Driver g_Krait_GPIO_Driver;
GPIO_INTERRUPT_SERVICE_ROUTINE my_isr;

void gpio_irq(void *arg)
{
		//my_isr(GPIO_PIN, BOOL, null);
		my_isr(0, TRUE, NULL);
}

//Taken from platform/msn8960/gpio.c
void gpio_tlmm_config(UINT32 gpio, UINT8 func,
		      UINT8 dir, UINT8 pull,
		      UINT8 drvstr, UINT32 enable)
{
	UINT32 val = 0;
	val |= pull;
	val |= func << 2;
	val |= drvstr << 6;
	val |= enable << 9;
	UINT32 *addr = (UINT32 *)GPIO_CONFIG_ADDR(gpio);
	writel(val, addr);
	return;
}

void gpio_set(UINT32 gpio, UINT32 dir)
{
	UINT32 *addr = (UINT32 *)GPIO_IN_OUT_ADDR(gpio);
	//writel(dir, addr);
	writel(dir ? 1UL << 1 : 0, addr);
	return;
}

/* Configure gpio for uart - based on gsbi id */
void gpio_config_uart_dm(UINT8 id)
{
	switch (id) {

	case GSBI_ID_3:
		/* configure rx gpio */
		gpio_tlmm_config(15, 1, GPIO_INPUT, GPIO_NO_PULL,
				 GPIO_8MA, GPIO_DISABLE);
		/* configure tx gpio */
		gpio_tlmm_config(14, 1, GPIO_OUTPUT, GPIO_NO_PULL,
				 GPIO_8MA, GPIO_DISABLE);
		break;

	case GSBI_ID_5:
		/* configure rx gpio */
		gpio_tlmm_config(23, 1, GPIO_INPUT, GPIO_NO_PULL,
				 GPIO_8MA, GPIO_DISABLE);
		/* configure tx gpio */
		gpio_tlmm_config(22, 1, GPIO_OUTPUT, GPIO_NO_PULL,
				 GPIO_8MA, GPIO_DISABLE);
		break;

	default:
		ASSERT(0);
	}
}

BOOL Krait_GPIO_Driver::GetPinState(GPIO_PIN pin) {
	UINT32 *addr = (unsigned int *)GPIO_IN_OUT_ADDR(pin);
	if (readl(addr)&1)
		return TRUE;
	else
		return FALSE;
}

BOOL Krait_GPIO_Driver::Initialize()
{
}

BOOL Krait_GPIO_Driver::EnableInputPin(GPIO_PIN pin, BOOL glitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE isr, void* isr_param, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState )
{
	gpio_tlmm_config(pin, 1, GPIO_INPUT, resistorState, GPIO_8MA, GPIO_DISABLE);
	
	if(isr) {
		UINT32 *addr = (UINT32 *)GPIO_INTR_CFG(pin);
		UINT32 val = 0x7; // docs page 297, enable summary interrupt for pin.
		writel(val, addr);
		my_isr = isr;
		CPU_INTC_ActivateInterrupt(TLMM_MSM_SUMMARY_IRQ, gpio_irq, 0);
		CPU_INTC_InterruptEnable( TLMM_MSM_SUMMARY_IRQ );
	}
	
	return TRUE;
}

void Krait_GPIO_Driver::EnableOutputPin(GPIO_PIN pin, BOOL initialState)
{
	gpio_tlmm_config(pin, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE);
	if(initialState == TRUE)
		gpio_set(pin,(UINT32)initialState);

}

UINT32 Krait_GPIO_Driver::Attributes( GPIO_PIN Pin )
{

    return GPIO_ATTRIBUTE_OUTPUT;
}


BOOL Krait_GPIO_Driver::ReservePin( GPIO_PIN pin, BOOL fReserve )
{

    return TRUE;
}




void Krait_GPIO_Driver::SetPinState(GPIO_PIN pin, BOOL pinState)
{
	gpio_set(pin,(UINT32)pinState);
}


