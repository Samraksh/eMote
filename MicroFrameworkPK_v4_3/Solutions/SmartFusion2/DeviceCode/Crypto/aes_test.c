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
#include <stdio.h>
#include <drivers/mss_sys_services/mss_sys_services.h>
#include <drivers/mss_uart/mss_uart.h>
#include <drivers/mss_gpio/mss_gpio.h>
#include <drivers/CoreGPIO/core_gpio.h>
#include "aes_test.h"

gpio_instance_t g_gpio;

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
    		      MSS_UART_115200_BAUD,
                  MSS_UART_DATA_8_BITS  | \
                  MSS_UART_NO_PARITY    | \
                  MSS_UART_ONE_STOP_BIT);

    /* Display greeting message. */
    display_greeting();
	
    // Select cryptographic operation to perform 
    display_operation_choices();
	
/*	MSS_GPIO_init();
	int i;
	for (i=0; i<31;i++){
		MSS_GPIO_config(0, MSS_GPIO_OUTPUT_MODE);
	}*/
	//uint32_t gpio_pattern;

	/*while (1){
		//MSS_UART_polled_tx_string(gp_my_uart, g_separator);
		for (i=0; i<31;i++){
			MSS_GPIO_set_output(i, 0);
		}
		for (i=0; i<31;i++){
			MSS_GPIO_set_output(i, 1);
		}
		gpio_pattern = MSS_GPIO_get_outputs();
        gpio_pattern ^= 0xFFFFFFFF;
        MSS_GPIO_set_outputs( gpio_pattern );
		
	}*/

	uint32_t io_state;
    
    /**************************************************************************
     * Initialize the CoreGPIO driver with the base address of the CoreGPIO
     * instance to use and the initial state of the outputs. 
     *************************************************************************/    
    GPIO_init( &g_gpio,    COREGPIO_BASE_ADDR, GPIO_APB_32_BITS_BUS );
    
    /**************************************************************************
     * Configure the GPIOs.
     *************************************************************************/     
    GPIO_config( &g_gpio, GPIO_0, GPIO_OUTPUT_MODE );
    //GPIO_config( &g_gpio, GPIO_1, GPIO_OUTPUT_MODE );
    //GPIO_config( &g_gpio, GPIO_2, GPIO_OUTPUT_MODE );
    //GPIO_config( &g_gpio, GPIO_3, GPIO_OUTPUT_MODE );
    //GPIO_config( &g_gpio, GPIO_4, GPIO_INPUT_MODE );
    //GPIO_config( &g_gpio, GPIO_5, GPIO_INPUT_MODE );

    /**************************************************************************
     * Infinite Loop.
     *************************************************************************/        
    while(1)
    {
        /**********************************************************************
         * Read inputs.
         *********************************************************************/
        //io_state = GPIO_get_inputs( &g_gpio );
        
        /**********************************************************************
         * Write state of inputs back to the outputs.
         *********************************************************************/
        //io_state = io_state >> INPUT_TO_OUTPUT_BIT_OFFSET;
        GPIO_set_outputs( &g_gpio, 1 );    
        GPIO_set_outputs( &g_gpio, 0 );    
    }

	/*
    for(;;)
    {
        // Start command line interface if any key is pressed. 
        rx_size = MSS_UART_get_rx(gp_my_uart, rx_buff, sizeof(rx_buff));
        if(rx_size > 0)
        {
            switch(rx_buff[0])
            {
                case '1':
                    // Perform encryption using AES-128 
                    aes128_encryption();
                    display_option();
                    display_operation_choices();
                break;

                case '2':
                    // Perform decryption using AES-128 
                    aes128_decryption();
                    display_option();
                    display_operation_choices();
                break;

                case '3':
                    // Perform encryption using AES-256 
                    aes256_encryption();
                    display_option();
                    display_operation_choices();
                break;

                case '4':
                    // Perform decryption using AES-256 
                    aes256_decryption();
                    display_option();
                    display_operation_choices();
                break;

                default:
                break;
            }
        }
    }
	*/
}

/*==============================================================================
  AES-128 encryption.
 */
static void aes128_encryption(void)
{
    uint8_t status = 0u;
    uint8_t aes_mode;
    const uint8_t read_key_msg[] =
    "\r\n Enter the 128 bit key to be used for AES (as hex Bytes, LS Byte first): \r\n";
    const uint8_t read_data_msg[] =
    "\r\n Enter the 16 bytes of input data to encrypt (as hex Bytes, LS Byte first):\r\n";
    const uint8_t read_iv_msg[] =
    "\r\n Enter the 128 bit initialization vector(IV) to be used for AES \r\n (as hex Bytes, LS Byte first): \r\n";

    /* Get the Key from the user to perform Encryption. */
    get_key(KEY_LENGTH_128, g_key_256, ASCII_32_CHARACTERS, read_key_msg);

    /* Get the Initialization Vector value */
    get_key(INIT_VECTOR_VALUE, g_aes_iv, ASCII_32_CHARACTERS, read_iv_msg);

    /* Get the mode */
    aes_mode = get_aes_mode(AES_ENCRYPT);

    /* Clear input and output variables */
    clear_aes_var(AES_ENCRYPT);

    /* Read the 16 bytes of input data from UART terminal. */
    get_input_data(&g_plain_text[0], DATA_LENGTH_128_BITS, read_data_msg);

    g_nb_blocks = 1u;

    /* Perform 128-bit encryption. */
    status = MSS_SYS_128bit_aes(g_key_256,
                                g_aes_iv,
                                g_nb_blocks,
                                aes_mode,
                                g_encrypted_text,
                                g_plain_text);

    /* Display the encrypted data in hex format. */
    if(MSS_SYS_SUCCESS == status)
    {
        MSS_UART_polled_tx_string(gp_my_uart, g_separator);
        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)"\r\n Encrypted data:\r\n");
        display_hex_values(g_encrypted_text, DATA_LENGTH_16_BYTES);
    }
    else
    {
        MSS_UART_polled_tx_string(gp_my_uart, g_separator);
        /* Display the error information */
        display_error_info(status);
    }
}

/*==============================================================================
  AES-128 decryption.
 */
static void aes128_decryption(void)
{
    uint8_t aes_mode = 0u;
    uint8_t status = 0u;
    const uint8_t read_key_msg[] =
    "\r\n Enter the 128 bit key to be used for AES (as hex Bytes, LS Byte first): \r\n";
    const uint8_t read_iv_msg[] =
    "\r\n Enter the 128 bit initialization vector(IV) to be used for AES \r\n (as hex Bytes, LS Byte first): \r\n";
    const uint8_t read_encrypted_data_msg[] =
    "\r\n Enter the 16 bytes of input data to decrypt (as hex Bytes, LS Byte first): \r\n";

    /* Get the Key from the user to perform Decryption. */
    get_key(KEY_LENGTH_128, g_key_256, ASCII_32_CHARACTERS, read_key_msg);

    /* Get the Initialization Vector value. */
    get_key(INIT_VECTOR_VALUE, g_aes_iv, ASCII_32_CHARACTERS, read_iv_msg);

    /* Get the AES Mode. */
    aes_mode = get_aes_mode(AES_DECRYPT);

    /* Clear input and output variables. */
    clear_aes_var(AES_DECRYPT);

    /* Read the encrypted data from UART terminal. */
    get_input_data(&g_dec_input_text[0],
                   DATA_LENGTH_128_BITS,
                   read_encrypted_data_msg);

    /* Identify the number of 128-bit block in the input data. */
    g_nb_blocks = 1;

    /* Perform 128-bit Decryption. */
    status = MSS_SYS_128bit_aes(g_key_256,g_aes_iv,
                                g_nb_blocks,aes_mode,
                                g_decrypted_text,
                                g_dec_input_text);

    /* Display the Decrypted data in Hex Format. */
    if(MSS_SYS_SUCCESS == status)
    {
        MSS_UART_polled_tx_string(gp_my_uart, g_separator);
        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)"\r\n Decrypted data:\r\n");
        display_hex_values(g_decrypted_text, DATA_LENGTH_16_BYTES);
    }
    else
    {
        MSS_UART_polled_tx_string(gp_my_uart, g_separator);
        display_error_info(status);
    }
}

/*==============================================================================
  AES-256 encryption.
 */
static void aes256_encryption(void)
{
    uint8_t aes_mode = 0u;
    uint8_t status = 0u;
    const uint8_t read_key_msg[] =
    "\r\n Enter the 256 bit key to be used for AES (as hex Bytes, LS Byte first): \r\n";
    const uint8_t read_iv_msg[] =
    "\r\n Enter the 128 bit initialization vector(IV) to be used for AES \r\n (as hex Bytes, LS Byte first): \r\n";
    const uint8_t read_data_msg[] =
    "\r\n Enter the 16 bytes of input data to encrypt (as hex Bytes, LS Byte first): \r\n";

    /* Get the Key from the user to perform Encryption. */
    get_key(KEY_LENGTH_256, g_key_256, ASCII_64_CHARACTERS, read_key_msg);

    /* Get the Initialization Vector value. */
    get_key(INIT_VECTOR_VALUE, g_aes_iv, ASCII_32_CHARACTERS, read_iv_msg);

    /* Get the mode. */
    aes_mode = get_aes_mode(AES_ENCRYPT);

    /* Clear input and output variables. */
    clear_aes_var(AES_ENCRYPT);

    /* Read the input data to encrypt from the MMUART0 and store it. */
    get_input_data(&g_plain_text[0], DATA_LENGTH_128_BITS, read_data_msg);

    /* Identify the number of 128-bit block in the input data */
    g_nb_blocks = 1;

    /* Perform 256-bit encryption. */
    status = MSS_SYS_256bit_aes(g_key_256,
                                g_aes_iv,
                                g_nb_blocks,
                                aes_mode,
                                g_encrypted_text,
                                g_plain_text);

    /* Display the encrypted data in Hex Format. */
    if(MSS_SYS_SUCCESS == status)
    {
        MSS_UART_polled_tx_string(gp_my_uart, g_separator);
        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)"\r\n Encrypted data:\r\n");
        display_hex_values(g_encrypted_text, DATA_LENGTH_16_BYTES);
    }
    else
    {
        MSS_UART_polled_tx_string(gp_my_uart, g_separator);
        display_error_info(status);
    }
}

/*==============================================================================
  AES-256 decryption.
 */
static void aes256_decryption(void)
{
    uint8_t aes_mode = 0u;
    uint8_t status;
    const uint8_t read_key_msg[] =
    "\r\n Enter the 256 bit key to be used for AES (as hex Bytes, LS Byte first): \r\n";
    const uint8_t read_iv_msg[] =
    "\r\n Enter the 128 bit initialization vector(IV) to be used for AES \r\n (as hex Bytes, LS Byte first): \r\n";
    const uint8_t read_encrypted_data_msg[] =
    "\r\n Enter the 16 bytes of input data to decrypt (as hex Bytes, LS Byte first): \r\n";

    /* Get the Key from the user to perform Decryption. */
    get_key(KEY_LENGTH_256, g_key_256, ASCII_64_CHARACTERS, read_key_msg);

    /* Get the Initialization Vector value. */
    get_key(INIT_VECTOR_VALUE, g_aes_iv, ASCII_32_CHARACTERS, read_iv_msg);

    /* Get the AES Mode. */
    aes_mode = get_aes_mode(AES_DECRYPT);

    /* Clear input and output variables. */
    clear_aes_var(AES_DECRYPT);

    /* Read the input data to encrypt from the MMUART0 and store it. */
    get_input_data(&g_dec_input_text[0],
                   DATA_LENGTH_128_BITS,
                   read_encrypted_data_msg);

    /* Identify the number of 128-bit block in the input data. */
    g_nb_blocks = 1;

    /* Perform 256-bit Decryption. */
    status = MSS_SYS_256bit_aes(g_key_256,
                                g_aes_iv,
                                g_nb_blocks,
                                aes_mode,
                                g_decrypted_text,
                                g_dec_input_text);

    /* Display the Decrypted data in Hex Format. */
    if(MSS_SYS_SUCCESS == status)
    {
        MSS_UART_polled_tx_string(gp_my_uart, g_separator);
        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)"\r\n Decrypted data:\r\n");
        display_hex_values(g_decrypted_text, DATA_LENGTH_16_BYTES);
    }
    else
    {
        MSS_UART_polled_tx_string(gp_my_uart, g_separator);
        display_error_info(status);
    }
}


/*==============================================================================
  Function to get the AES mode for Encryption/Decryption
 */
static uint8_t get_aes_mode(uint8_t aes_operation)
{
    uint8_t ret_mode = 0u;

    display_aes_modes();
    ret_mode = select_aes_mode(aes_operation);

    return(ret_mode);
}

/*==============================================================================
  Display greeting message when application is started.
 */
static void display_greeting(void)
{
    MSS_UART_polled_tx_string(gp_my_uart, g_greeting_msg);
}

/*==============================================================================
  Display the choice of cryptographic operation to perform.
 */
static void display_operation_choices(void)
{
    MSS_UART_polled_tx_string(gp_my_uart, g_select_operation_msg);
}

/*==============================================================================
  Display content of buffer passed as parameter as hex values
 */
static void display_hex_values
(
    const uint8_t * in_buffer,
    uint32_t byte_length
)
{
    uint8_t display_buffer[128];
    uint32_t inc;

    if(byte_length > 8u)
    {
        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t*)"\r\n");
    }

    for(inc = 0; inc < byte_length; ++inc)
    {
        if((inc > 1u) &&(0u == (inc % 8u)))
        {
            MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t*)"\r\n");
        }
        snprintf((char *)display_buffer,
                 sizeof(display_buffer),
                 "%02x", in_buffer[inc]);
        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" 0x");
        MSS_UART_polled_tx_string(gp_my_uart, display_buffer);
    }
}

/*==============================================================================
  Display Supported AES modes
 */
static void display_aes_modes()
{
    MSS_UART_polled_tx_string(gp_my_uart, g_get_aes_mode_msg);
}

/*==============================================================================
  Display AES Mode to Select for Encryption/Decryption
 */
static uint8_t select_aes_mode(uint8_t aes_operation)
{
    uint8_t aes_mode[1];
    uint8_t rx_size = 0u;
    uint8_t ret_mode = 0u;
    uint8_t complete = 0u;

    while(!complete)
    {
        rx_size = MSS_UART_get_rx(gp_my_uart, aes_mode, sizeof(aes_mode));
        if(rx_size > 0u)
        {
            switch(aes_mode[0])
            {
                /* Selected Mode is ECB */
                case 'A':
                case 'a':
                    /* Check is it for Encryption */
                    if(AES_ENCRYPT == aes_operation)
                    {
                        ret_mode = MSS_SYS_ECB_ENCRYPT;
                        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" Selected Encryption Mode : Electronic CodeBook Mode");
                    }
                    /* Check is it for Decryption */
                    else
                    {
                        ret_mode = MSS_SYS_ECB_DECRYPT;
                        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" Selected Decryption Mode : Electronic CodeBook Mode");
                    }
                    complete = 1u;
                break;

                /* Selected Mode is CBC */
                case 'B':
                case 'b':
                    /* Check is it for Encryption */
                    if(AES_ENCRYPT == aes_operation)
                    {
                        ret_mode = MSS_SYS_CBC_ENCRYPT;
                        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" Selected Encryption Mode : Cipher Block Chaining Mode");
                    }
                    /* Check is it for Decryption */
                    else
                    {
                        ret_mode = MSS_SYS_CBC_DECRYPT;
                        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" Selected Decryption Mode : Cipher Block Chaining Mode");
                    }
                    complete = 1u;
                break;

                /* Selected Mode is OFB */
                case 'C':
                case 'c':
                    /* Check is it for Encryption */
                    if(AES_ENCRYPT == aes_operation)
                    {
                        ret_mode = MSS_SYS_OFB_ENCRYPT;
                        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" Selected Encryption Mode : Output Feedback Mode");
                    }
                    /* Check is it for Decryption */
                    else
                    {
                        ret_mode = MSS_SYS_OFB_DECRYPT;
                        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" Selected Decryption Mode : Output Feedback Mode");
                    }
                    complete = 1u;
                break;

                /* Selected Mode is CTR */
                case 'D':
                case 'd':
                    /* Check is it for Encryption */
                    if(AES_ENCRYPT == aes_operation)
                    {
                        ret_mode = MSS_SYS_CTR_ENCRYPT;
                        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" Selected Encryption Mode : Counter Mode");
                    }
                    /* Check is it for Decryption */
                    else
                    {
                        ret_mode = MSS_SYS_CTR_DECRYPT;
                        MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" Selected Decryption Mode : Counter Mode");
                    }
                    complete = 1u;
                break;

                /* Selected Mode is Invalid */
                default:
                {
                    MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" Selected Mode is Invalid. ");
                    display_aes_modes();
                    complete = 0u;
                }
                break;
            }
        }
    }
    return(ret_mode);
}

/*==============================================================================
  Display the Option to continue or exit.
 */
static void display_option(void)
{
    uint8_t rx_size;
    uint8_t rx_buff;

    MSS_UART_polled_tx_string(gp_my_uart, g_separator);
    MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t*)"\r\n Press any key to continue.\r\n");
    do
    {
        rx_size = MSS_UART_get_rx(gp_my_uart, &rx_buff, sizeof(rx_buff));
    } while(0u == rx_size);
}

/*==============================================================================
  Clear AES global variable.
 */
static void clear_aes_var(uint8_t mode)
{
    volatile uint8_t inc;

    for(inc = 0u; inc < 64u; inc++)
    {
        if(mode == AES_ENCRYPT)
        {
            if( inc < 32u)
            {
                g_encrypted_text[inc] = 0u;
            }
            g_plain_text[inc] = 0u;
        }
        else
        {
            if( inc < 32u)
            {
                g_decrypted_text[inc] = 0u;
            }
            g_dec_input_text[inc] = 0u;
        }
    }
}

/*==============================================================================
  Clear SHA-256 global variable.
 */
static void clear_sha256_var(void)
{
    volatile uint32_t inc;

    for(inc = 0u; inc < 128u; inc++)
    {
        g_hash_input_text[inc] = 0u;
        if(inc < 32u)
        {
            g_checksum_hash[inc] = 0u;
        }
    }
}

/*==============================================================================
  Convert ASCII to hex.
 */
static uint8_t convert_ascii_to_hex(uint8_t* arr, uint32_t length)
{
    uint32_t inc = 0u;
    uint8_t nb_digit = 0u;
    uint8_t hex_byte = 0u;
    uint8_t error_flag = 0u;
    uint8_t* src_ptr = arr;
    uint8_t* dst_ptr = arr;

    for(inc = 0; inc < length; inc++)
    {
        if((*src_ptr >= '0') && (*src_ptr <= '9'))
        {
            hex_byte = (hex_byte * 16u) + (*src_ptr - '0');
            *src_ptr = 0u;
            src_ptr++;
            nb_digit++;
        }
        else if((*src_ptr >= 'a') && (*src_ptr <= 'f'))
        {
            hex_byte = (hex_byte * 16u) + (*src_ptr - 'a') + 10u;
            *src_ptr = 0u;
            src_ptr++;
            nb_digit++;
        }
        else if((*src_ptr >= 'A') && (*src_ptr <= 'F'))
        {
            hex_byte = (hex_byte * 16u) + (*src_ptr - 'A') + 10u;
            *src_ptr = 0u;
            src_ptr++;
            nb_digit++;
        }
        else if(*src_ptr != 0x00u)
        {
            MSS_UART_polled_tx_string(gp_my_uart,
                                      (const uint8_t *)"\r\n Invalid data.");
            error_flag = 1u;
            break;
        }

        if(nb_digit >= 2u)
        {
            *dst_ptr = hex_byte;
            nb_digit = 0u;
            hex_byte = 0u;
            dst_ptr++;
        }
    }

    if(nb_digit == 1u)
    {
      *dst_ptr = (hex_byte * 16u);
    }
    return error_flag;
}

/*==============================================================================
  Validate the input ASCII value .
 */
static uint8_t validate_input(uint8_t ascii_input)
{
    uint8_t valid_key = 0u;

    if(((ascii_input >= 'A') && (ascii_input <= 'F')) ||       \
       ((ascii_input >= 'a') && (ascii_input <= 'f')) ||       \
       ((ascii_input >= '0') && (ascii_input <= '9')))
    {
       valid_key = 1u;
    }
    else
    {
       valid_key = 0u;
    }
    return valid_key;
}

/*==============================================================================
  Validate the Key type.
 */
static uint8_t validate_key_type(uint8_t key_type)
{
    uint8_t status = 0u;

    /* Validate the Key type and request the key from user. */
    if((KEY_LENGTH_128 == key_type)     ||                  \
       (KEY_LENGTH_256 == key_type)     ||                  \
       (KEY_HMAC == key_type)           ||                  \
       (INIT_VECTOR_VALUE == key_type))
    {
        status = 0u;
    }
    else
    {
        status = 1u;
    }

    return status;
}

/*==============================================================================
  Display the error message.
 */
static void display_error_info(uint8_t status)
{
    switch(status)
    {
        case MSS_SYS_MEM_ACCESS_ERROR:
            MSS_UART_polled_tx_string(gp_my_uart,
                                      (const uint8_t *)g_errror_message[0]);
            break;

        case MSS_SYS_SERVICE_NOT_LICENSED:
            MSS_UART_polled_tx_string(gp_my_uart,
                                      (const uint8_t *)g_errror_message[1]);
            break;

        case MSS_SYS_SERVICE_DISABLED_BY_FACTORY:
            MSS_UART_polled_tx_string(gp_my_uart,
                                      (const uint8_t *)g_errror_message[2]);
            break;

        case MSS_SYS_SERVICE_DISABLED_BY_USER:
            MSS_UART_polled_tx_string(gp_my_uart,
                                      (const uint8_t *)g_errror_message[3]);
            break;

        default:
            MSS_UART_polled_tx_string(gp_my_uart,
                                      (const uint8_t *)g_errror_message[4]);
            break;
    }
}

/*==============================================================================
  Function to get the key from user.
 */
static void get_key
(
    uint8_t key_type,
    uint8_t* location,
    uint8_t size,
    const uint8_t* msg
)
{
    uint8_t status = 0u;
    const uint8_t invalid_ms[] = "\r\n Invalid key type. ";

    /* Validate the key type */
    status = validate_key_type(key_type);

    if(status == VALID)
    {
        /* Read the 16 bytes of input data from UART terminal. */
        get_input_data(location, size, msg);
    }
    else
    {
        MSS_UART_polled_tx_string(gp_my_uart, invalid_ms);
    }
}

/*==============================================================================
  Function to get the input data from user.
 */
static uint8_t get_input_data
(
    uint8_t* location,
    uint8_t size,
    const uint8_t* msg
)
{
    uint8_t count = 0u;

    /* Clear the memory location. */
    clear_variable(location, size);

    /* Read the 16 bytes of input data from UART terminal. */
    count = get_data_from_uart(location, size, msg);

    /* Convert ASCII key to Hex format */
    convert_ascii_to_hex(location, size);

    return count;
}

/*==============================================================================
  Function to clear local variable and array.
 */
static void clear_variable(uint8_t *p_var, uint16_t size)
{
    uint16_t inc;

    for(inc = 0; inc < size; inc++)
    {
        *p_var = 0x00;
        p_var++;
    }
}

/*==============================================================================
  Function to read data from UART terminal and stored it.
 */
static uint8_t get_data_from_uart
(
    uint8_t* src_ptr,
    uint8_t size,
    const uint8_t* msg
)
{
    uint8_t complete = 0u;
    uint8_t rx_buff[1];
    uint8_t rx_size = 0u;
    uint8_t count = 0u;

    MSS_UART_polled_tx_string(gp_my_uart, g_separator);
    MSS_UART_polled_tx_string(gp_my_uart, msg);

    /* Read the key size sent by user and store it. */
    count = 0u;
    while(!complete)
    {
        rx_size = MSS_UART_get_rx (gp_my_uart, rx_buff, sizeof(rx_buff));
        if(rx_size > 0u)
        {
            /* Is it to terminate from the loop */
            if(ENTER == rx_buff[0])
            {
                complete = 1u;
            }
            /* Is entered key valid */
            else if(validate_input(rx_buff[0]) != 1u)
            {
                MSS_UART_polled_tx(gp_my_uart, rx_buff, sizeof(rx_buff));
                MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)"\r\n Invalid input.");
                MSS_UART_polled_tx_string(gp_my_uart, msg);
                complete = 0u;
                count = 0u;
                clear_variable(src_ptr, 4);
            }
            else
            {
                src_ptr[count] = rx_buff[0];

                /* Switching to next line after every 8 bytes */
                if(((count % 16u) == 0x00u) && (count > 0x00u) && (complete != 0x01u))
                {
                    MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)"\n\r");
                }

                if((count % 2u) == 0u)
                {
                    MSS_UART_polled_tx_string(gp_my_uart, (const uint8_t *)" 0x");
                }
                MSS_UART_polled_tx(gp_my_uart, rx_buff, sizeof(rx_buff));
                count++;
                if(size == count)
                {
                   complete = 1u;
                }
            }
        }
    }

    return count;
}



