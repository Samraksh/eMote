#pragma once
#include <stm32f10x.h>
#include "si446x.h"

const  SI446X_pin_setup_t si446x_pin_config_wln = {
	.spi_base 		= SPI2,
	.spi_port 		= GPIOB,
	.spi_rcc 		= RCC_APB1Periph_SPI2,
	.nirq_port 		= GPIOB,
	.nirq_pin 		= GPIO_Pin_10,
	.nirq_mf_pin 	= (GPIO_PIN) 26,
	.gpio0_port 	= GPIOB,
	.gpio1_port 	= GPIOA,
	.gpio0_pin 		= GPIO_Pin_6,
	.gpio1_pin 		= GPIO_Pin_3,
	.cs_port 		= GPIOB,
	.cs_pin 		= GPIO_Pin_12,
	.sclk_pin 		= GPIO_Pin_13,
	.miso_pin 		= GPIO_Pin_14,
	.mosi_pin 		= GPIO_Pin_15,
	.sdn_port 		= GPIOB,
	.sdn_pin 		= GPIO_Pin_11,
};

const SI446X_pin_setup_t si446x_pin_config_austere = {
	.spi_base 		= SPI2,
	.spi_port 		= GPIOB,
	.spi_rcc 		= RCC_APB1Periph_SPI2,
	.nirq_port 		= GPIOB,
	.nirq_pin 		= GPIO_Pin_10,
	.nirq_mf_pin 	= (GPIO_PIN) 26,
	.gpio0_port 	= NULL, /*GPIOC*/
	.gpio1_port 	= NULL, /*GPIOC*/
	.gpio0_pin 		= GPIO_Pin_0, // ANY PIN FROM FPGA FOR NOW, this is FPGA_GPIO_0
	.gpio1_pin 		= GPIO_Pin_1, // ANY PIN FROM FPGA FOR NOW, this is FPGA_GPIO_1
	.cs_port 		= GPIOB,
	.cs_pin 		= GPIO_Pin_12,
	.sclk_pin 		= GPIO_Pin_13,
	.miso_pin 		= GPIO_Pin_14,
	.mosi_pin 		= GPIO_Pin_15,
	.sdn_port 		= GPIOB,
	.sdn_pin 		= GPIO_Pin_11,
};

/* OLD FUNCTION FOR REFERENCE
static void choose_hardware_config(int hw_id, SI446X_pin_setup_t *config) {
	if (hw_id == RADIO_PLATFORM_HALF_WLN) {	// First test half-integrated board
		config->spi_base 		= SPI2;
		config->spi_port 		= GPIOB;
		config->nirq_port		= GPIOB;
		config->nirq_pin		= GPIO_Pin_10;
		config->nirq_mf_pin		= (GPIO_PIN) 26;
		config->gpio0_port		= GPIOA;
		config->gpio1_port		= GPIOA;
		config->gpio0_pin		= GPIO_Pin_6;
		config->gpio1_pin		= GPIO_Pin_0;
		config->cs_port			= GPIOB;
		config->cs_pin			= GPIO_Pin_12;
		config->sclk_pin		= GPIO_Pin_13;
		config->miso_pin		= GPIO_Pin_14;
		config->mosi_pin		= GPIO_Pin_15;
		config->sdn_port		= GPIOB;
		config->sdn_pin			= GPIO_Pin_11;
		config->spi_rcc			= RCC_APB1Periph_SPI2;
		si446x_debug_print(DEBUG03, "SI446X: Using WWF Hardware Config\r\n");
	}
	else if (hw_id == RADIO_PLATFORM_WLN) { // 2nd iteration fully integrated board
		config->spi_base 		= SPI2;
		config->spi_port 		= GPIOB;
		config->nirq_port		= GPIOB;
		config->nirq_pin		= GPIO_Pin_10;
		config->nirq_mf_pin		= (GPIO_PIN) 26;
		config->gpio0_port		= GPIOB;
		config->gpio1_port		= GPIOA;
		config->gpio0_pin		= GPIO_Pin_6;
		config->gpio1_pin		= GPIO_Pin_3;
		config->cs_port			= GPIOB;
		config->cs_pin			= GPIO_Pin_12;
		config->sclk_pin		= GPIO_Pin_13;
		config->miso_pin		= GPIO_Pin_14;
		config->mosi_pin		= GPIO_Pin_15;
		config->sdn_port		= GPIOB;
		config->sdn_pin			= GPIO_Pin_11;
		config->spi_rcc			= RCC_APB1Periph_SPI2;
		si446x_debug_print(DEBUG03, "SI446X: Using WWF2 Hardware Config\r\n");
		si446x_debug_print(DEBUG02, "SI446X: TEST: Enabling PWM\r\n");

		// TEST CODE
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		set_radio_power_pwm(0);
		// END TEST
	}
	else if (hw_id == RADIO_PLATFORM_AUSTERE) { // Austere v1
		config->spi_base 		= SPI2;
		config->spi_port 		= GPIOB;
		config->nirq_port		= GPIOB;
		config->nirq_pin		= GPIO_Pin_10;
		config->nirq_mf_pin		= (GPIO_PIN) 26;
		config->gpio0_port		= NULL;
		config->gpio1_port		= NULL;
		config->gpio0_pin		= GPIO_Pin_0; // ANY PIN FROM FPGA FOR NOW, this is FPGA_GPIO_0
		config->gpio1_pin		= GPIO_Pin_1; // ANY PIN FROM FPGA FOR NOW, this is FPGA_GPIO_1
		config->cs_port			= GPIOB;
		config->cs_pin			= GPIO_Pin_12;
		config->sclk_pin		= GPIO_Pin_13;
		config->miso_pin		= GPIO_Pin_14;
		config->mosi_pin		= GPIO_Pin_15;
		config->sdn_port		= NULL;
		config->sdn_pin			= GPIO_Pin_11;
		config->spi_rcc			= RCC_APB1Periph_SPI2;
		hal_printf( "SI446X: Using Austere Hardware Config\r\n");
	}
	else { // I am a .NOW
		config->spi_base 		= SPI2;
		config->spi_port 		= GPIOB;
		config->nirq_port		= GPIOA;
		config->nirq_pin		= GPIO_Pin_1;
		config->nirq_mf_pin		= (GPIO_PIN) 1;
		config->gpio0_port		= GPIOA;
		config->gpio1_port		= GPIOA;
		config->gpio0_pin		= GPIO_Pin_3;
		config->gpio1_pin		= GPIO_Pin_8;
		config->cs_port			= GPIOA;
		config->cs_pin			= GPIO_Pin_4;
		config->sclk_pin		= GPIO_Pin_13;
		config->miso_pin		= GPIO_Pin_14;
		config->mosi_pin		= GPIO_Pin_15;
		config->sdn_port		= GPIOA;
		config->sdn_pin			= GPIO_Pin_2;
		config->spi_rcc			= RCC_APB1Periph_SPI2;
		si446x_debug_print(DEBUG03, "SI446X: Using .NOW Hardware Config\r\n");
	}
}
*/