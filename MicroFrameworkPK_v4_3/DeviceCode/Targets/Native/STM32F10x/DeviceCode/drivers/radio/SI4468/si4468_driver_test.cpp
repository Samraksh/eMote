
#include <tinyhal.h>
#include <Samraksh\Message.h>
#include <Samraksh\Radio.h>

#include "si4468_driver_test.h"
#include "si446x.h"

#include <unistd.h>

enum {
	packet_size = 64,
};

static void wait() { while (HAL_CONTINUATION::Dequeue_And_Execute() == TRUE); }
static void wait_5_ms(void) { for (volatile int i=0; i<20000; i++) ; } // @ 64 MHz

void si4468_spi2_init_test() {
	hal_printf("SI4468 SPI2 INIT TEST\r\n");
	if ( si446x_hal_init(NULL, 0, 0) == DS_Success ) {
		hal_printf("SI4468 SPI2 INIT TEST PASSED\r\n");
	}
	else hal_printf("SI4468 SPI2 INIT TEST FAILED\r\n");
};

// TX TEST STUFF
static int tx_gate=0; // not volatile since called from Continuation
static void tx_handler() {
	tx_gate = 1;
}

int si4468_spi2_tx_test() {
	const unsigned timeout_max = 1000000;
	const char my_string[] = "Nathan is Great1!Nathan is Great2!Nathan is Great3!Nathan is Great4!Nathan is Great5!Nathan is Great6!\r\n";
	const uint8_t len = sizeof(my_string);
	char test_data[len];
	unsigned timeout=0;
	DeviceStatus ret;
	int retint;

	hal_printf("SI4468 STARTING TX TEST\r\n");

	if (len>=128) { hal_printf("PACKET TOO BIG YA DUMMY, len=%d, ABORTING\r\n", len); while(1); }

	si446x_hal_register_tx_callback(tx_handler);

	//memset(test_data, 0, len);
	//hal_snprintf(test_data, len, "Nathan Is Great!\r\n");

	wait();

	ret = si446x_packet_send(0, (uint8_t *)my_string, len, 0, 1);
	if (ret == DS_Fail) { hal_printf("SI4468 SPI2 SEND TEST FAIL: Tx Call Fail\r\n"); return 0; }

	while(tx_gate == 0 && timeout++ <= timeout_max) // wait for interrupt to update.
		{ wait(); } // Manually wait for Continuation to fire.

	wait();

	if (timeout >= timeout_max) {
		hal_printf("SI4468 SPI2 TX TEST FAILED: Timeout!\r\n");
		retint = 0;
	}
	else {
		hal_printf("SI4468 SPI2 TX TEST PASSED!\r\n");
		retint = 1;
	}

	tx_gate = 0; // reset
	return retint;
}

void si4468_spi2_chain_tx(void) {
	const unsigned timeout_max = 1000000;
	const unsigned sends_done_max = 10;
	const char my_string[] = "Nathan is Great1!Nathan is Great2!Nathan is Great3!Nathan is Great4!Nathan is Great5!Nathan is Great6!\r\n";
	const uint8_t len = sizeof(my_string);
	char test_data[len];
	unsigned timeout=0;
	unsigned sends_done=0;
	DeviceStatus ret;

	hal_printf("SI4468 STARTING TX CHAIN TEST\r\n");
	if (len>=128) { hal_printf("PACKET TOO BIG YA DUMMY, len=%d, ABORTING\r\n", len); while(1); }

	//si446x_hal_register_tx_callback(tx_handler);

	while (timeout < timeout_max && sends_done < sends_done_max) {
		ret = si446x_packet_send(0, (uint8_t *)my_string, len, 0, 0);
		if (ret == DS_Success)
			sends_done++;
		else
			timeout++;
		wait_5_ms();
	}

	if (timeout >= timeout_max) {
		hal_printf("SI4468 SPI2 TX CHAIN TEST FAILED: Timeout!\r\n");
	}
	else {
		hal_printf("SI4468 SPI2 TX CHAIN TEST PASSED!\r\n");
	}

	tx_gate = 0; // reset
	return;
}
// END TX TEST STUFF
// RX TEST STUFF
static int rx_gate = 0;
static void rx_handler(UINT64 ts, unsigned size, const __restrict__ uint8_t *data) {
	const UINT32 *packet_time = (UINT32 *)&data[size-4];

	hal_printf("\tGOT PACKET: size %d\r\n", size);
	hal_printf("\tLocal  TS: %llu\r\n", ts);
	hal_printf("\tRemote TS: %u\r\n", *packet_time);
	hal_printf("\t%s\r\n", (char *)data);
	rx_gate = 1;
}

int si4468_spi2_rx_test() {
	char test_data[packet_size];
	const unsigned timeout_max = 5000000;
	unsigned timeout=0;
	DeviceStatus ret;
	int retint;

	memset(test_data, 0, packet_size);

	si446x_hal_register_rx_callback(rx_handler);

	hal_printf("SI4468 STARTING RX TEST\r\n");

	wait();

	ret = si446x_hal_rx(0);
	if (ret == DS_Fail) { hal_printf("SI4468 SPI2 RX TEST FAIL: Rx Call Fail\r\n"); return 0; }

	while(rx_gate == 0 && timeout++ <= timeout_max) // wait for interrupt to update.
		{ wait(); } // Manually wait for Continuation to fire.

	wait(); // Let a continuation run.

	if (timeout >= timeout_max) {
		hal_printf("SI4468 SPI2 RX TEST FAILED: Timeout!\r\n");
		si446x_hal_sleep(0);
		retint = 0;
	}
	else {
		hal_printf("SI4468 SPI2 RX TEST PASSED!\r\n");
		retint = 1;
	}

	rx_gate = 0; // reset
	return retint;
}
// END RX TEST STUFF

void si4468_spi2_power_change() {
	const UINT32 newpower = 10;
	hal_printf("SI4468 STARTING POWER CHANGE TEST\r\n");

	UINT32 power = si446x_hal_get_power(0);
	hal_printf("SI4468 current power = %d\r\n", power);

	si446x_hal_tx_power(0, newpower);
	power = si446x_hal_get_power(0);

	hal_printf("Power set to %d, readback %d\r\n", newpower, power);

	if (power == newpower)
		hal_printf("Test PASS\r\n");
	else
		hal_printf("Test FAIL\r\n");
}
