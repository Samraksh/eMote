#ifndef _NETMF_GPIO_H_
#define _NETMF_GPIO_H_

struct PIN_ISR_DESCRIPTOR
{
		 GPIO_INTERRUPT_SERVICE_ROUTINE m_isr;
		 void*                          m_param;

};


#endif
