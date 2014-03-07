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

struct Krait_GPIO_Config {
private:
public:
	UINT32 enabled[(NR_MSM_GPIO / 32) + 1];
	GPIO_INTERRUPT_SERVICE_ROUTINE gpio_isr[NR_MSM_GPIO];
	int GPIO_GET_NEXT_ENABLED(int next) {
		int itr = next/32;
		int rem = next%32;
		do {
			if(this->enabled[itr] > 0) {
				do {
					if(enabled[itr] & (1 << (rem))) {
						return rem;
					}
					++rem;
				}while(rem < 32);
			}
			rem = 0;
			++itr;
		}while(itr < /*ARRAY_SIZE*/((NR_MSM_GPIO/32)+1));
		return NR_MSM_GPIO;
	}
	inline void GPIO_SET_ISR(UINT32 gpio, GPIO_INTERRUPT_SERVICE_ROUTINE isr) {
		gpio_isr[gpio] = isr;
	}
	inline void GPIO_SET_ENABLED(UINT32 gpio) {
		enabled[gpio/32] |= (1 << (gpio % 32));
	}
	inline void GPIO_CLR_ENABLED(UINT32 gpio) {
		enabled[gpio/32] &= ~(1 << (gpio % 32));
	}
}s_Krait_GPIO_Config;


void gpio_irq(void *arg)
{
	for(int itr = s_Krait_GPIO_Config.GPIO_GET_NEXT_ENABLED(0); itr < NR_MSM_GPIO; itr = s_Krait_GPIO_Config.GPIO_GET_NEXT_ENABLED(++itr))
	{
		if(s_Krait_GPIO_Config.gpio_isr[itr] != NULL) {
			//my_isr(GPIO_PIN, BOOL, null);
			s_Krait_GPIO_Config.gpio_isr[itr](itr, TRUE, NULL);
			writel(1, GPIO_INTR_STATUS(itr)); //clear interrupt for this GPIO.
		}
	}
}

/**
 * Translate NetMF enum GPIO_RESISTOR in CPU_GPIO_decl.h to MSM GPIO_ states in Krait__GPIO.h. NetMF has no notion of GPIO_KEEPER state, so bump 2 to 3.
 */
#define gpio_map_state(rs) (((rs)==RESISTOR_PULLUP)?GPIO_PULL_UP:(rs))

//Taken from platform/msn8960/gpio.c
void gpio_tlmm_config(UINT32 gpio, UINT8 func,
		      UINT8 dir, UINT8 pull,
		      UINT8 drvstr, UINT32 enable)
{
	UINT32 val = 0;
	val |= pull;
	val |= func << 2;
	val |= drvstr << 6;
	val |= (enable & dir) << 9;
	UINT32 *addr = (UINT32 *)GPIO_CONFIG_ADDR(gpio);
	writel(val, addr);
	return;
}

/**
 * controls the value of GPIO Output when GPIO_CFG[GPIO_OE] is set.
 */
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
	gpio_tlmm_config(pin, 1, GPIO_INPUT, gpio_map_state(resistorState), GPIO_8MA, GPIO_DISABLE);
	
	if(isr) {
		//FIXME: GLOBAL_LOCK or disable IRQs around this.
		UINT32 *addr = (UINT32 *)GPIO_INTR_CFG(pin);
		UINT32 val =  0; // docs 80-N1622-3 page 297, enable summary interrupt for pin.  Skip setting INTR_RAW_STATUS_EN to save power and not use per-interrupt handlers.
		switch(intEdge) {
		        case GPIO_INT_NONE:
		            val = 0;
		            break;
		        case GPIO_INT_EDGE_LOW:
		        case GPIO_INT_LEVEL_LOW:
		            val = 0b001;
		            break;
		        case GPIO_INT_EDGE_HIGH:
		        case GPIO_INT_LEVEL_HIGH:
		            val = 0b011;
		            break;
		        case GPIO_INT_EDGE_BOTH:
		            //FIXME: read level and change polarity in interrupt handler.
		            val = 0b101;
		            break;
		        default:
		            return FALSE;
		}
		val |= 0b1000; //set and leave INTR_RAW_STATUS_EN bit.
		writel(val, addr);
		//FIXME: wait 5us before trying to clear status?
		writel(1, GPIO_INTR_STATUS(pin)); //clear interrupt for this GPIO.
		s_Krait_GPIO_Config.GPIO_SET_ISR(pin, isr);
		CPU_INTC_ActivateInterrupt(TLMM_MSM_SUMMARY_IRQ, gpio_irq, 0);
		CPU_INTC_InterruptEnable( TLMM_MSM_SUMMARY_IRQ );
		//FIXME: unlock or enable IRQs.
	}
	
	return TRUE;
}

void Krait_GPIO_Driver::EnableOutputPin(GPIO_PIN pin, BOOL initialState)
{
	gpio_tlmm_config(pin, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE);
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

void Krait_GPIO_Driver::DisablePin(GPIO_PIN pin, GPIO_RESISTOR resistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction)
{
	s_Krait_GPIO_Config.GPIO_CLR_ENABLED(pin);
    gpio_tlmm_config(pin, AltFunction, GPIO_INPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_DISABLE);
}


