/*  Name : TIAM3517_PinMux.cpp
 *
 *  Author : mukundan.sridharan@Samraksh.com
 *
 *  Description : The IO pin multiplexer configuration function. Wrapper for the .h file
 *
 *  History :
 *  version 1: Date: Feb 20,2012.
 *  	    The Pin defines and address copied from uboot/include/asm-arm/arch-omap3/mux.h
 *  	    AM35XX_CRTL_BASE found from the manual. Look at chapter 6.4.4 in sprugr0b.pdf
 */
#include "TIAM3517_PinMux.h"

struct TIAM3517_GPIO_CNTR_PAD gpio_cntr_pad;

void TIAM3517_PinMux::setMuxUSB(){

	MUX_VAL(CONTROL_PADCONF_HSUSB0_CLK,		(IEN  | PTD | DIS | M0)); //HSUSB0_CLK
	MUX_VAL(CONTROL_PADCONF_HSUSB0_STP,		(IDIS | PTU | EN  | M0)); //HSUSB0_STP
	MUX_VAL(CONTROL_PADCONF_HSUSB0_DIR,		(IEN  | PTD | DIS | M0)); //HSUSB0_DIR
	MUX_VAL(CONTROL_PADCONF_HSUSB0_NXT,		(IEN  | PTD | DIS | M0)); //HSUSB0_NXT
	MUX_VAL(CONTROL_PADCONF_HSUSB0_DATA0,	(IEN  | PTD | DIS | M0)); //HSUSB0_DATA0
	MUX_VAL(CONTROL_PADCONF_HSUSB0_DATA1,	(IEN  | PTD | DIS | M0)); //HSUSB0_DATA1
	MUX_VAL(CONTROL_PADCONF_HSUSB0_DATA2,	(IEN  | PTD | DIS | M0)); //HSUSB0_DATA2
	MUX_VAL(CONTROL_PADCONF_HSUSB0_DATA3,	(IEN  | PTD | DIS | M0)); //HSUSB0_DATA3
	MUX_VAL(CONTROL_PADCONF_HSUSB0_DATA4,	(IEN  | PTD | DIS | M0)); //HSUSB0_DATA4
	MUX_VAL(CONTROL_PADCONF_HSUSB0_DATA5,	(IEN  | PTD | DIS | M0)); //HSUSB0_DATA5
	MUX_VAL(CONTROL_PADCONF_HSUSB0_DATA6,	(IEN  | PTD | DIS | M0)); //HSUSB0_DATA6
	MUX_VAL(CONTROL_PADCONF_HSUSB0_DATA7,	(IEN  | PTD | DIS | M0)); //HSUSB0_DATA7


}


void TIAM3517_PinMux::Initialize(){
	initGPIOCNTR();
	//g_pinMux.Initialized=TRUE;
}

//Only GPIO pins 34-63, 157-162 are confiured
void TIAM3517_PinMux::initGPIOCNTR(){

	//gpio_cntr_pad.pins[33]=CONTROL_PADCONF_GPMC_D8;
	gpio_cntr_pad.pins[34]=CONTROL_PADCONF_GPMC_A1;
	gpio_cntr_pad.pins[35]=CONTROL_PADCONF_GPMC_A2;
	gpio_cntr_pad.pins[36]=CONTROL_PADCONF_GPMC_A3;
	gpio_cntr_pad.pins[37]=CONTROL_PADCONF_GPMC_A4;
	gpio_cntr_pad.pins[38]=CONTROL_PADCONF_GPMC_A5;
	gpio_cntr_pad.pins[39]=CONTROL_PADCONF_GPMC_A6;
	gpio_cntr_pad.pins[40]=CONTROL_PADCONF_GPMC_A7;
	gpio_cntr_pad.pins[41]=CONTROL_PADCONF_GPMC_A8;
	gpio_cntr_pad.pins[42]=CONTROL_PADCONF_GPMC_A9;
	gpio_cntr_pad.pins[43]=CONTROL_PADCONF_GPMC_A10;
	gpio_cntr_pad.pins[44]=CONTROL_PADCONF_GPMC_D8;
	gpio_cntr_pad.pins[45]=CONTROL_PADCONF_GPMC_D9;
	gpio_cntr_pad.pins[46]=CONTROL_PADCONF_GPMC_D10;
	gpio_cntr_pad.pins[47]=CONTROL_PADCONF_GPMC_D11;
	gpio_cntr_pad.pins[48]=CONTROL_PADCONF_GPMC_D12;
	gpio_cntr_pad.pins[49]=CONTROL_PADCONF_GPMC_D13;
	gpio_cntr_pad.pins[50]=CONTROL_PADCONF_GPMC_D14;
	gpio_cntr_pad.pins[51]=CONTROL_PADCONF_GPMC_D15;
	gpio_cntr_pad.pins[52]=CONTROL_PADCONF_GPMC_NCS1;
	gpio_cntr_pad.pins[53]=CONTROL_PADCONF_GPMC_NCS2;
	gpio_cntr_pad.pins[54]=CONTROL_PADCONF_GPMC_NCS3;
	gpio_cntr_pad.pins[55]=CONTROL_PADCONF_GPMC_NCS4;
	gpio_cntr_pad.pins[56]=CONTROL_PADCONF_GPMC_NCS5;
	gpio_cntr_pad.pins[57]=CONTROL_PADCONF_GPMC_NCS6;
	gpio_cntr_pad.pins[58]=CONTROL_PADCONF_GPMC_NCS7;
	gpio_cntr_pad.pins[59]=CONTROL_PADCONF_GPMC_CLK;
	gpio_cntr_pad.pins[60]=CONTROL_PADCONF_GPMC_NBE0_CLE;
	gpio_cntr_pad.pins[61]=CONTROL_PADCONF_GPMC_NBE1;
	gpio_cntr_pad.pins[62]=CONTROL_PADCONF_GPMC_NWP;
	gpio_cntr_pad.pins[63]=CONTROL_PADCONF_GPMC_WAIT1;

	gpio_cntr_pad.pins[157]=CONTROL_PADCONF_MCBSP1_FSR;
	gpio_cntr_pad.pins[158]=CONTROL_PADCONF_MCBSP1_DX;
	gpio_cntr_pad.pins[159]=CONTROL_PADCONF_MCBSP1_DR;
	gpio_cntr_pad.pins[160]=CONTROL_PADCONF_MCBSP_CLKS;
	gpio_cntr_pad.pins[161]=CONTROL_PADCONF_MCBSP1_FSX;
	gpio_cntr_pad.pins[162]=CONTROL_PADCONF_MCBSP1_CLKX;

}


void TIAM3517_PinMux::setMode (UINT32 pin, UINT32 Mode){
	MUX_VAL(pin, Mode);
}

//Only GPIO pins 34-63 are congiured
void TIAM3517_PinMux::setMuxGPIO(UINT32 pin){

	/*if(!g_pinMux.Initialized) {
		Initialize();
	}*/
	UINT32 ADDR = gpio_cntr_pad.pins[pin];

	if((pin >=34 && pin <=63) || (pin >=157 && pin <=162)){
		setMode(ADDR,(IDIS | PTU | EN  | M4));
	}
}
