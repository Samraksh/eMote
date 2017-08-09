/*
 * Aes_test.c
 *
 *  Created on: Mar 3, 2017
 *      Author: MukundanSridharan
 *      Copied mostly based on the AES demo on the SmartFusion2 Security Eval Kit
 *
 */

/******************************************************************************
 * (c) Copyright 2012-2014 Microsemi SoC Products Group.  All rights reserved.
 *
 * This example project demonstrates the usage of Cryptographic services
 * supported By SmartFusion2 Device. User Interface support is provided by
 * using the MMUART0. User has to select the specific service and has to provide
 * the required inputs to perform the operation.
 *
 * SVN $Revision: 6794 $
 * SVN $Date: 2014-08-05 22:09:15 +0530 (Tue, 05 Aug 2014) $
 */
#include "../mbedTLS_PKCS/aes_test.h"

#include <stdio.h>
#include <drivers/mss_sys_services/mss_sys_services.h>
#include <drivers/mss_uart/mss_uart.h>

/*==============================================================================
  Messages displayed over the UART.
 */
const uint8_t g_greeting_msg[] =
"\r\n\r\n\
******************************************************************************\r\n\
************* SmartFusion2 Cryptography System Services Example **************\r\n\
******************************************************************************\r\n\
 This example project demonstrates the use of the SmartFusion2 cryptography\r\n\
 System Services. The following system service are demonstrated:\r\n\
  - AES-128 encryption and decryption.\r\n\
  - AES-256 encryption and decryption.\r\n\
Please refer to the README.txt for input and output data format.\r\n";

const uint8_t g_get_aes_mode_msg[] =
"\r\n\
------------------------------------------------------------------------------\r\n\
 Select AES modes:\r\n\
  - ECB: Electronic CodeBook Mode.      Press key 'A'\r\n\
  - CBC: Cipher Block Chaining Mode.    Press key 'B'\r\n\
  - OFB: Output Feedback Mode.          Press key 'C'\r\n\
  - CTR: Counter Mode.                  Press key 'D'\r\n";

const uint8_t g_select_operation_msg[] =
"\r\n\
------------------------------------------------------------------------------\r\n\
 Select the Cryptographic operation to perform:\r\n\
    Press Key '1' to perform AES-128 encryption \r\n\
    Press Key '2' to perform AES-128 decryption \r\n\
    Press Key '3' to perform AES-256 encryption \r\n\
    Press Key '4' to perform AES-256 decryption \r\n\
------------------------------------------------------------------------------\r\n";

const uint8_t *g_errror_message[] =
{
    (const uint8_t*)"\r\n Error - HRESP error occurred during MSS transfer.",
    (const uint8_t*)"\r\n Error - License not available in device.",
    (const uint8_t*)"\r\n Error - Service disabled by factory security.",
    (const uint8_t*)"\r\n Error - Service disabled by user security.",
    (const uint8_t*)"\r\n Error - Unknown."
};

const uint8_t g_separator[] =
"\r\n\
------------------------------------------------------------------------------";

/*==============================================================================
  Private functions.
 */
static void display_greeting(void);
static void display_hex_values(const uint8_t *, uint32_t);
static uint8_t get_aes_mode(uint8_t);
static void display_aes_modes(void);
static uint8_t select_aes_mode(uint8_t aes_operation);
static void clear_aes_var(uint8_t mode);
static void clear_sha256_var(void);
static void display_option(void);
static void display_operation_choices(void);
static void aes128_encryption(void);
static void aes128_decryption(void);
static void aes256_encryption(void);
static void aes256_decryption(void);
static uint8_t convert_ascii_to_hex(uint8_t* arr, uint32_t length);
static uint8_t validate_key_type(uint8_t key_type);
static uint8_t validate_input(uint8_t ascii_input);
static void display_error_info(uint8_t status);
static void clear_variable(uint8_t *p_var, uint16_t size);
static uint8_t get_data_from_uart
(
    uint8_t* src_ptr,
    uint8_t size,
    const uint8_t* msg
);
static uint8_t get_input_data
(
    uint8_t* location,
    uint8_t size,
    const uint8_t* msg
);
static void get_key
(
    uint8_t key_type,
    uint8_t* location,
    uint8_t size,
    const uint8_t* msg
);

/*==============================================================================
  Global Variables.
 */
static uint8_t g_key_256[64];
static uint8_t g_plain_text[64];
static uint8_t g_encrypted_text[32];
static uint8_t g_decrypted_text[32];
static uint8_t g_dec_input_text[64];
static uint8_t g_hash_input_text[128];
static uint8_t g_hmac_input_text[128];
static uint8_t g_nb_blocks;
static uint8_t g_hmac_code_stored[32];
static uint8_t g_checksum_hash[32];
static uint8_t g_aes_iv[32];

/*==============================================================================
  Macro
 */
#define   VALID                   0U
#define   INVALID                 1U
#define   AES_KEY_REGEN_CNT       10u
#define   KEY_LENGTH_128          32u
#define   KEY_LENGTH_256          64u
#define   KEY_HMAC                6u
#define   INIT_VECTOR_VALUE       8u
#define   AES_DECRYPT             1u
#define   AES_ENCRYPT             2u
#define   ENTER                   13u
#define   DUMMY_DATA              0u
#define   ASCII_32_CHARACTERS     32u
#define   ASCII_64_CHARACTERS     (ASCII_32_CHARACTERS * 2)
#define   ASCII_128_CHARACTERS    (ASCII_32_CHARACTERS * 4)
#define   DATA_LENGTH_128_BITS    ASCII_32_CHARACTERS
#define   DATA_LENGTH_256_BITS    ASCII_64_CHARACTERS
#define   DATA_LENGTH_512_BITS    ASCII_128_CHARACTERS
#define   DATA_LENGTH_16_BYTES    (ASCII_32_CHARACTERS / 2)
#define   DATA_LENGTH_32_BYTES    (ASCII_64_CHARACTERS / 2)
#define   DATA_LENGTH_64_BYTES    (ASCII_128_CHARACTERS / 2)

/*==============================================================================
  UART selection.
  Replace the line below with this one if you want to use UART1 instead of
  UART0:
  mss_uart_instance_t * const gp_my_uart = &g_mss_uart1;
 */
mss_uart_instance_t * const gp_my_uart = &g_mss_uart1;

/*==============================================================================
  Test Main function.
 */
int aesTest()
{
    uint8_t rx_size = 0;
    uint8_t rx_buff[1];

    /* Disable Watchdog */
    SYSREG->WDOG_CR = 0x00000000;

    /* System Services Driver Initialization */
    MSS_SYS_init(MSS_SYS_NO_EVENT_HANDLER);

    /* MMUART Initialization */
    MSS_UART_init(gp_my_uart,
    		      MSS_UART_57600_BAUD,
                  MSS_UART_DATA_8_BITS  | \
                  MSS_UART_NO_PARITY    | \
                  MSS_UART_ONE_STOP_BIT);

    /* Display greeting message. */
    display_greeting();

    /* Select cryptographic operation to perform */
    display_operation_choices();

    for(;;)
    {
        /* Start command line interface if any key is pressed. */
        rx_size = MSS_UART_get_rx(gp_my_uart, rx_buff, sizeof(rx_buff));
        if(rx_size > 0)
        {
            switch(rx_buff[0])
            {
                case '1':
                    /* Perform encryption using AES-128 */
                    aes128_encryption();
                    display_option();
                    display_operation_choices();
                break;

                case '2':
                    /* Perform decryption using AES-128 */
                    aes128_decryption();
                    display_option();
                    display_operation_choices();
                break;

                case '3':
                    /* Perform encryption using AES-256 */
                    aes256_encryption();
                    display_option();
                    display_operation_choices();
                break;

                case '4':
                    /* Perform decryption using AES-256 */
                    aes256_decryption();
                    display_option();
                    display_operation_choices();
                break;

                default:
                break;
            }
        }
    }
}

/*==============================================================================
  AES-128 encryption.
 */
