/*
 * EmoteLoraHat.h
 *
 *  Created on: Aug 18, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_EMOTELORAHAT_H_
#define MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_EMOTELORAHAT_H_

// Hardware stuff
#include <stm32f10x.h>


#define SX1276_CMD_ID_READ_RX_FIFO 0x77
#define SX1276_CMD_ID_WRITE_TX_FIFO 0x66


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
	enum LoraDaugterBoards{
		LORA_GPS_HAT,
	};

	struct SX1276_pin_setup_t{
//        PinName mosi;		//mosi_pin
//		PinName miso;  		//miso_pin
//		PinName sclk;		//sclk_pin
//		PinName nss;
//		PinName reset;
//        PinName dio0;
//		PinName dio1;
//		PinName dio2;
//		PinName dio3;
//		PinName dio4;
//		PinName dio5;

		SPI_TypeDef 	*spi_base;
		GPIO_TypeDef 	*spi_port;
		uint32_t		spi_rcc;
		GPIO_TypeDef 	*nirq_port;
		uint16_t		nirq_pin;
		GPIO_PIN		nirq_mf_pin;
		GPIO_TypeDef 	*gpio0_port;
		GPIO_TypeDef 	*gpio1_port;
		uint16_t		gpio0_pin;
		uint16_t		gpio1_pin;
		GPIO_TypeDef 	*cs_port;
		uint16_t		cs_pin;
		uint16_t		sclk_pin;
		uint16_t		miso_pin;
		uint16_t		mosi_pin;
		GPIO_TypeDef 	*sdn_port;
		uint16_t		sdn_pin;
	};

	struct InterruptPins_t{
		UINT8 DIO0;
		UINT8 DIO1;
		UINT8 DIO2;
		UINT8 DIO3;
		UINT8 DIO4;
		UINT8 DIO5;
	};

private:
	static SX1276_pin_setup_t SX1276_pin_setup;
	static unsigned ctsWentHigh;

	uint8_t radio_spi_go(uint8_t data);
	void radio_spi_sel_no_assert();

public:
	static InterruptPins_t SX1276_interupt_pins;



	LoraHardwareConfig();
	void initSPI2(); //Initialize a SPI structure

	static void spi_write_bytes(unsigned count, const uint8_t *buf);

	static void spi_read_bytes(unsigned count, uint8_t *buf);

	unsigned int radio_comm_PollCTS();

	void radio_spi_sel_assert();

	uint8_t radio_comm_GetResp(uint8_t byteCount, uint8_t* pData);

};






class Emote_Lora_Hat : private LoraHardwareConfig {
private:


public:
	Emote_Lora_Hat();
	virtual ~Emote_Lora_Hat();
//
//	void write_tx_fifo(uint8_t numBytes, uint8_t* pTxData) {
//	  radio_comm_WriteData( SX1276_CMD_ID_WRITE_TX_FIFO, 0, numBytes, pTxData );
//	}
//
//	void read_rx_fifo(uint8_t numBytes, uint8_t* pRxData) {
//	  radio_comm_ReadData( SX1276_CMD_ID_READ_RX_FIFO, 0, numBytes, pRxData );
//	}

	bool SpiInitialize();

	void radio_comm_WriteData(uint8_t cmd, unsigned pollCts, uint8_t byteCount, uint8_t* pData);
	void radio_comm_ReadData(uint8_t cmd, unsigned pollCts, uint8_t byteCount, uint8_t* pData);



};

} /* namespace LoraHat */

#endif /* MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_EMOTELORAHAT_H_ */
