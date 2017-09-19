/*
 * EmoteLoraHat.h
 *
 *  Created on: Aug 18, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_EMOTELORAHAT_H_
#define MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_EMOTELORAHAT_H_

// Hardware stuff
#include <tinyhal.h>
#include <spi\netmf_spi.h>


static int LORA_ASSERT(int x, const char *err) {
//	if (!x) {
//		si446x_debug_print(ERR100, "%s", err);
//		radio_error();
//		return 1;
//	}
//	else return 0;
	if(!x) return 1;
	else return 0;
}


namespace LoraHat {

class LoraHardwareConfig{
public:
    const uint16_t SPIReadBufferSize = 256;
    uint8_t SPIReadBufferStorage[256];

    const uint16_t SPIWriteBufferSize = 256;
    uint8_t SPIWriteBufferStorgae[256];

	enum LoraDaugterBoards{
		LORA_GPS_HAT,
	};

protected:
	// Stores the configuration of the spi
	SPI_CONFIGURATION m_spi_config;

	struct InterruptPins_t{
		GPIO_PIN DIO0;
		GPIO_PIN DIO1;
		GPIO_PIN DIO2;
		GPIO_PIN DIO3;
		GPIO_PIN DIO4;
		GPIO_PIN DIO5;
	};
	struct SX1276_pin_setup_t{
		InterruptPins_t SX1276_interupt_pins;
		GPIO_PIN		reset_mf_pin;
	};
	SX1276_pin_setup_t SX1276_pin_setup;

private:
	bool SpiInitialize();
	void init_pins();
	void reset();
public:



	LoraHardwareConfig();

	void Initialize();

};



} /* namespace LoraHat */

#endif /* MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_EMOTELORAHAT_H_ */
