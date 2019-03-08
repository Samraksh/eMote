/*******************************************************************************
* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
* Author: Ismail H. Kose <ismail.kose@maximintegrated.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/

// BTstack Core
#include "..\btcore\btstack_debug.h"
#include "..\btcore\btstack.h"
#include "btstack_config.h"
#include "btstack_run_loop_embedded.h"
#include "..\cc256x\btstack_chipset_cc256x.h"

// BTstack HALs
#include "hal_tick.h"

#include "btstack_port.h"

#define CC256X_UART_ID             0
#define UART_RXFIFO_USABLE     (MXC_UART_FIFO_DEPTH-3)

//static uint32_t baud_rate;

// rx state
static int  bytes_to_read = 0;
static uint8_t * rx_buffer_ptr = 0;

// tx state
static int bytes_to_write = 0;
static uint8_t * tx_buffer_ptr = 0;

/*const gpio_cfg_t PAN1326_SLOW_CLK = { PORT_1, PIN_7, GPIO_FUNC_GPIO,
		GPIO_PAD_NORMAL };
const gpio_cfg_t PAN1326_nSHUTD = { PORT_1, PIN_6, GPIO_FUNC_GPIO,
		GPIO_PAD_NORMAL };
const gpio_cfg_t PAN1326_HCIRTS = { PORT_0, PIN_3, GPIO_FUNC_GPIO,
		GPIO_PAD_NORMAL };*/

static void dummy_handler(void) {};
static void (*rx_done_handler)(void) = dummy_handler;
static void (*tx_done_handler)(void) = dummy_handler;

//static int patchingCC256 = false;

void hal_cpu_disable_irqs(void)
{
	//__disable_irq();
	//log_info("*** disable irq not implemented");
}

void hal_cpu_enable_irqs(void)
{
	//__enable_irq();
	//log_info("*** enable irq not implemented");
}
void hal_cpu_enable_irqs_and_sleep(void)
{
	//__enable_irq();
	/* TODO: Add sleep mode */
	//log_info("*** enable irq and sleep mode not implemented");
}

void hal_uart_send_block(const uint8_t *buffer, uint16_t len)
{
	log_info("hal_uart_send_block bytes: %d", len);
	tx_buffer_ptr = (uint8_t *)buffer;
	bytes_to_write = len;
}

void hal_uart_receive_bytes(uint8_t *buffer, uint16_t len)
{
	//log_info("uart asked to rx %d bytes\r\n", len);
	// receive bytes into buffer until <len> bytes have been read
	// these bytes need to be read in run loop and then callbak called
	rx_buffer_ptr = buffer;
	bytes_to_read = len;
}

void hal_btstack_run_loop_execute_once(void)
{
	int rx_avail;
	int num_rx_bytes;
	int tx_avail;
	int rx_bytes;
	int tx_bytes;
	int ret;

    while (bytes_to_read) {
		rx_avail = btUartNumReadAvail();
		if (!rx_avail)
			break;

		if (bytes_to_read > rx_avail)
			num_rx_bytes = rx_avail;
		else
			num_rx_bytes = bytes_to_read;

		rx_bytes = btUartRead(rx_buffer_ptr, num_rx_bytes);
		if (ret < 0)
			break;

		rx_buffer_ptr += rx_bytes;
        	bytes_to_read -= rx_bytes;

		 if (bytes_to_read < 0) {
			bytes_to_read = 0;
		}

         if (bytes_to_read == 0){
             (*rx_done_handler)();
         }
     }

     while (bytes_to_write) {
		tx_avail = btUartNumWriteAvail();
		if (!tx_avail)
			break;

		if (bytes_to_write > tx_avail)
			tx_bytes = tx_avail;
		else
			tx_bytes = bytes_to_write;

		ret = btUartWrite(tx_buffer_ptr, tx_bytes);
		if (ret < 0)
			break;
		bytes_to_write -= tx_bytes;
		tx_buffer_ptr += tx_bytes;
		if (bytes_to_write < 0) {
			bytes_to_write = 0;
		}

        if (bytes_to_write == 0){
             (*tx_done_handler)();
        }
     }

	btstack_run_loop_embedded_execute_once();
}

/*
int hal_uart_set_baud(uint32_t baud){
	baud_rate = baud;
	log_info("BAUD RATE IS = %d \n", baud);
	return baud_rate;
}*/

/*void hal_uart_init(void){
	bytes_to_write = 0;
	bytes_to_read = 0;
	hal_uart_set_baud(115200);
	log_info("hal_uart_dma_init ");
}*/

void hal_uart_set_block_received( void (*block_handler)(void)){
	rx_done_handler = block_handler;
}

void hal_uart_set_block_sent( void (*block_handler)(void)){
	tx_done_handler = block_handler;
}

void hal_uart_dma_set_csr_irq_handler( void (*csr_irq_handler)(void)){
	log_info("***  hal_uart_dma_set_csr_irq_handler not implemented");
}

void hal_uart_dma_set_sleep(uint8_t sleep){
	log_info("***  hal_uart_dma_set_sleep not implemented");
}

void init_slow_clock(void)
{
/*	MXC_PWRSEQ->reg0 &= ~(MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN | MXC_F_PWRSEQ_REG0_PWR_RTCEN_SLP);
	MXC_PWRSEQ->reg4 &= ~MXC_F_PWRSEQ_REG4_PWR_PSEQ_32K_EN;
	MXC_PWRSEQ->reg0 |= MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN | MXC_F_PWRSEQ_REG0_PWR_RTCEN_SLP; // Enable RTC
	hal_delay_us(1);
	MXC_PWRSEQ->reg4 |= MXC_F_PWRSEQ_REG4_PWR_PSEQ_32K_EN; // Enable the RTC out of P1.7*/
}

int bt_comm_init() {
	int error = 0;
	int cnt = 0;

	bytes_to_write = 0;
	bytes_to_read = 0;

	unsigned int delay = 1;
	hal_tick_init();
	hal_delay_us(delay);
	/*if ((error = GPIO_Config(&PAN1326_HCIRTS)) != E_NO_ERROR) {
		printf("Error setting PAN1326_HCIRTS %d\n", error);
	}*/
	//GPIO_OutSet(&PAN1326_HCIRTS);
	//init_slow_clock();


	log_info("%s CC256X init completed. cnt: %d \n", __func__, cnt);
	return 0;
}

static hci_transport_config_uart_t config = {
	    HCI_TRANSPORT_CONFIG_UART,
	    115200,
	    4000000,
	    1, // flow control
	    "sf2",
	};



/******************************************************************************/
int bluetooth_main(void)
{
	bt_comm_init();
	/* BT Stack Initialization */
	btstack_memory_init();
	btstack_run_loop_init(btstack_run_loop_embedded_get_instance());

	// enable packet logger
	//hci_dump_open(NULL, HCI_DUMP_STDOUT);

	/* Init HCI */
	const hci_transport_t * transport = hci_transport_h4_instance(btstack_uart_block_embedded_instance());
	hci_init(transport, &config);
	hci_set_chipset(btstack_chipset_cc256x_instance());

	//btstack_cc256x_patch_step(1);

    // setup Link Key DB using TLV
    //const btstack_link_key_db_t * btstack_link_key_db = btstack_link_key_db_tlv_get_instance(btstack_tlv_impl, &btstack_tlv_flash_bank_context);
    //hci_set_link_key_db(btstack_link_key_db);

    // setup LE Device DB using TLV
    //le_device_db_tlv_configure(btstack_tlv_impl, &btstack_tlv_flash_bank_context);
    
	btstack_main(0, (void *)NULL);
	
	return 0;
}
