/*
 * SysServices.c
 *
 *  Created on: Mar 7, 2017
 *      Author: Mukundan
 */

#include <tinyhal.h>
#include "SysServices.h"
#include <intc/VectorIndex.h>
#include "mss_sys_services.h"
#include <eNVM/eNVM.h>

#define DEVICE_CERTIFICATE_REQUEST_CMD                  0u
#define SERIAL_NUMBER_REQUEST_CMD                       1u
#define FLASH_FREEZE_REQUEST_CMD                        2u
#define AES128_REQUEST_CMD                              3u
#define USERCODE_REQUEST_CMD                            4u
#define DESIGNVER_REQUEST_CMD                           5u
#define AES256_REQUEST_CMD                              6u
#define KEYTREE_REQUEST_CMD                             9u
#define SHA256_REQUEST_CMD                              10u
#define HMAC_REQUEST_CMD                                12u
#define PPUF_CHALLENGE_RESP_REQUEST_CMD                 14u
#define POINT_MULTIPLICATION_REQUEST_CMD                16u
#define POINT_ADDITION_REQUEST_CMD                      17u  
#define IAP_PROGRAMMING_REQUEST_CMD                     20u
#define ISP_PROGRAMMING_REQUEST_CMD                     21u
#define DIGEST_CHECK_REQUEST_CMD                        23u
#define PUF_ACTIVATION_CODE_REQUEST_CMD                 25u
#define PUF_USER_KEY_CODE_REQUEST_CMD                   26u
#define PUF_FETCH_KEY_REQUEST_CMD                       27u
#define PUF_ECC_PUBLIC_KEY_REQUEST_CMD                  28u
#define PUF_SEED_REQUEST_CMD                            29u
#define SECONDARY_DEVICE_CERTIFICATE_REQUEST_CMD        30u
#define TAMPER_CONTROL_REQUEST_CMD                      31u
#define NRBG_SELF_TEST_REQUEST_CMD                      40u
#define NRBG_INSTANTIATE_REQUEST_CMD                    41u
#define NRBG_GENERATE_REQUEST_CMD                       42u
#define NRBG_RESEED_REQUEST_CMD                         43u
#define NRBG_UNINSTANTIATE_REQUEST_CMD                  44u
#define NRBG_RESET_REQUEST_CMD                          45u
#define FLASHFREEZE_SHUTDOWN_CMD                        224u
#define ZEROIZATION_REQUEST_CMD                         240u
#define POWER_ON_RESET_DIGEST_ERROR_CMD                 241u

/*
 * System Services requests length:
 */
#define FLASH_FREEZE_REQUEST_LENGTH                     2u

/*
 * Service response lengths:
 */
#define STANDARD_SERV_RESP_LENGTH                       6u
#define SERIAL_NUMBER_SERV_RESP_LENGTH                  6u
#define USERCODE_SERV_RESP_LENGTH                       6u
#define DESIGNVER_SERV_RESP_LENGTH                      6u
#define DEVICE_CERT_SERV_RESP_LENGTH                    6u
#define SECONDARY_DEVICE_CERT_SERV_RESP_LENGTH          6u
#define ISP_PROG_SERV_RESP_LENGTH                       2u
#define IAP_PROG_SERV_RESP_LENGTH                       2u
#define NRBG_RESET_SERV_RESP_LENGTH                     2u
#define NRBG_SELF_TEST_SERV_RESP_LENGTH                 2u
#define NRBG_UNINST_SERV_RESP_LENGTH                    3u
#define DRBG_RESET_SERV_RESP_LENGTH                     2u
#define DIGEST_CHECK_SERV_RESP_LENGTH                   2u
#define FLASH_FREEZE_SERV_RESP_LENGTH                   2u
#define PORDIGEST_CHECK_SERV_RESP_LENGTH                2u
#define TAMPER_CONTROL_SERV_RESP_LENGTH                 2u
#define PUF_USER_ACTIVATION_CODE_RESP_LENGTH            2u
#define PUF_GET_NUMBER_OF_KEYS_RESP_LENGTH              6u
#define PUF_ENROLL_KEYS_RESP_LENGTH                     6u
#define PUF_EXPORT_ALL_KEYCODES_RESP_LENGTH             6u
#define PUF_IMPORT_ALL_KEYCODES_RESP_LENGTH             6u
#define FACC_STANDBY_SEL                                0u
#define MSS_25_50MHZ_EN                                 1u
#define MSS_1MHZ_EN                                     1u
#define FACC_STANDBY_SHIFT                              6u
#define MSS_25_50MHZ_EN_SHIFT                           9u
#define MSS_1MHZ_EN_SHIFT                               10u
#define FACC_STANDBY_SEL_MASK                           0x000001C0u
#define MSS_25_50MHZ_EN_MASK                            0x00000200u
#define MSS_1MHZ_EN_MASK                                0x00000400u

/*
 * Non Deterministic Random Bit Generator defines:
 */
#define INVALID_NRBG_HANDLE                 0xFFu

/*
 * RTC_WAKEUP_CR system register bit masks:
 */
#define RTC_WAKEUP_G4C_EN_MASK      0x00000004u
#define RTC_WAKEUP_FAB_EN_MASK      0x00000002u

/*
 * PUF user activation code sub command
 */
#define PUF_CREATE_USER_ACTIVATION_CODE     0u
#define PUF_DELETE_USER_ACTIVATION_CODE     1u

/*
 * Sub Command for PUF service
 */
#define PUF_GET_NUMBER_OF_KC_SUBCOMMAND      0u
#define PUF_CREATE_EXT_KC_SUBCOMMAND         1u
#define PUF_CREATE_INT_KC_SUBCOMMAND         2u
#define PUF_EXPORT_ALL_KC_SUBCOMMAND         3u
#define PUF_IMPORT_ALL_KC_SUBCOMMAND         4u
#define PUF_DELETE_KC_SUBCOMMAND             5u

#define KERNEL_SIZE 349428
#define SIGSIZE 32
#define eNVMAddress 0x60000000
#define RoT_Offset 0x0F000
#define STANDARD_SERV_RESP_LENGTH                       6u
#define HMAC_KEY_LENGTH     32u
#define HMAC_REQUEST_CMD                                12u

#define PORDIGEST_CHECK_SERV_RESP_LENGTH                2u
volatile uint8_t g_request_in_progress = 0u;
static volatile uint16_t g_last_response_length = 0u;
static sys_serv_async_event_handler_t g_event_handler = 0;
static uint8_t g_response[PORDIGEST_CHECK_SERV_RESP_LENGTH] = {0u};
static uint32_t g_initial_mssddr_facc1_cr = 0U;
static uint8_t g_opcode=0u;
static uint16_t service_response_length = 0u;
static uint8_t * service_response = NULL;
static uint8_t service_status = 0u;
static uint8_t service_cmd_opcode = 0u;


#define TAMPER_ATTEMPT_DETECT_OPCODE_RANGE_MIN           0x80u
#define TAMPER_ATTEMPT_DETECT_OPCODE_RANGE_MAX           0x8Fu
#define TAMPER_FAILURE_DETECT_OPCODE_RANGE_MIN           0x90u
#define TAMPER_FAILURE_DETECT_OPCODE_RANGE_MAX           0x9Fu
#define TAMPER_CLOCK_MONITOR_ERROR_OPCODE                0xA0u
#define TAMPER_HARDWARE_MONITOR_ERROR_OPCODE_RANGE_MIN   0xB1u
#define TAMPER_HARDWARE_MONITOR_ERROR_OPCODE_RANGE_MAX   0xB7u

#define FACC_GLMUX_SEL_MASK         0x00001000u
#define DELAY_MORE_THAN_10US        5000U

#define MSS_SYS_NO_EVENT_HANDLER    ((sys_serv_async_event_handler_t)0)

static void request_completion_handler(uint8_t * p_response, uint16_t response_size);
static void signal_request_start(void);
static uint16_t wait_for_request_completion(void);
static uint8_t execute_service
(
    uint8_t cmd_opcode,
    uint8_t * cmd_params_ptr,
    uint8_t * response,
    uint16_t response_length
);

static void asynchronous_event_handler(uint8_t event_opcode);

static void write_ptr_value_into_array
(
    const uint8_t * pointer,
    uint8_t target_array[],
    uint32_t array_index
);

static void write_array_into_ptr_value
(
    uint8_t** pointer,
    uint8_t target_array[],
    uint32_t array_index
);

static uint8_t execute_service
(
    uint8_t cmd_opcode,
    uint8_t * cmd_params_ptr,
    uint8_t * response,
    uint16_t response_length
)
{
    uint8_t status;
    uint16_t actual_response_length;
    
	hal_printf("ex1\r\n");
    signal_request_start();
    
    MSS_COMBLK_send_cmd_with_ptr(cmd_opcode,                    /* cmd_opcode */
                                 (uint32_t)cmd_params_ptr,      /* cmd_params_ptr */
                                 response,                      /* p_response */
                                 response_length,               /* response_size */
                                 request_completion_handler);   /* completion_handler */
    
	hal_printf("ex2\r\n");
    //actual_response_length = wait_for_request_completion();
	hal_printf("ex3\r\n");
    
    if((response_length == actual_response_length) && (cmd_opcode == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

#define CR_FLUSHOUT_MASK    0x01u
#define CR_FLUSHIN_MASK     0x02u
#define CR_SIZETX_MASK      0x04u
#define CR_ENABLE_MASK      0x10u
#define CR_LOOPBACK_MASK    0x20u

/*------------------------------------------------------------------------------
 * Status and interrupt enable registers bit masks.
 */
#define TXTOKAY_MASK        0x01u
#define RCVOKAY_MASK        0x02u
#define TXOVERFLOW_MASK     0x04u
#define RXUNDERFLOW_MASK    0x08u
/*------------------------------------------------------------------------------
 * DATA8 register bit masks.
 */
#define DATA8_COMMAND_MASK  0x8000u

/*------------------------------------------------------------------------------
 * COMBLK driver states.
 */
#define COMBLK_IDLE             0u
#define COMBLK_TX_CMD           1u
#define COMBLK_TX_DATA          2u
#define COMBLK_WAIT_RESPONSE    3u
#define COMBLK_RX_RESPONSE      4u
#define COMBLK_TX_PAGED_DATA    5u

#define POR_DIGEST_ERROR_OPCODE         0xF1u

/*==============================================================================
 * COMBLK interrupt service routine.
 */
void ComBlk_IRQHandler(void);

/*==============================================================================
 * Local functions.
 */
static void abort_current_cmd(void);
static void send_cmd_opcode(uint8_t opcode);
static uint32_t fill_tx_fifo(const uint8_t * p_cmd, uint32_t cmd_size);
static void handle_tx_okay_irq(void);
static void handle_rx_okay_irq(void);
static void complete_request(uint16_t response_length);
static void process_sys_ctrl_command(uint8_t cmd_opcode);

/*==============================================================================
 * Global variables:
 */
static volatile uint8_t g_comblk_cmd_opcode = 0u;
static const uint8_t * g_comblk_p_cmd = 0u;
static volatile uint16_t g_comblk_cmd_size = 0u;
static const uint8_t * g_comblk_p_data = 0u;
static volatile uint32_t g_comblk_data_size = 0u;
static uint8_t * g_comblk_p_response = 0u;
static uint16_t g_comblk_response_size = 0u;
static volatile uint16_t g_comblk_response_idx = 0u;
static comblk_completion_handler_t g_comblk_completion_handler = 0;
static uint32_t (*g_comblk_page_handler)(uint8_t const ** pp_next_page) = 0;
static uint8_t g_comblk_state = COMBLK_IDLE;
static comblk_async_event_handler_t g_async_event_handler = 0;

/*==============================================================================
 *
 */
void MSS_COMBLK_init
(
    comblk_async_event_handler_t async_event_handler,
    uint8_t* p_response
)
{
	hal_printf("MSS_COMBLK_init\r\n");
    /*
     * Disable and clear previous interrupts.
     */
    NVIC_DisableIRQ(ComBlk_IRQn);
    COMBLK->INT_ENABLE = 0u;
    NVIC_ClearPendingIRQ(ComBlk_IRQn);
    
    g_async_event_handler = async_event_handler;
    
    /*
     * Initialize COMBLK driver state variables:
     */
    g_request_in_progress = 0u;
    g_comblk_cmd_opcode = 0u;
    g_comblk_p_cmd = 0u;
    g_comblk_cmd_size = 0u;
    g_comblk_p_data = 0u;
    g_comblk_data_size = 0u;
    g_comblk_p_response = p_response;
    g_comblk_response_size = 0u;
    g_comblk_response_idx = 0u;
    g_comblk_completion_handler = 0;
    
    g_comblk_state = COMBLK_IDLE;
    
    /*
     * Disable loopback before enabling the MSS COMM_BLK to ensure that any
     * codes waiting in the TX FIFO of the System Controller’s COMM_BLK are
     * not lost.
     */
    COMBLK->CONTROL &= ~CR_LOOPBACK_MASK;
    COMBLK->CONTROL |= CR_ENABLE_MASK;
    
    /*--------------------------------------------------------------------------
     * Enable receive interrupt to receive asynchronous events from the system
     * controller.
     */
    COMBLK->INT_ENABLE &= ~TXTOKAY_MASK;
    COMBLK->INT_ENABLE |= RCVOKAY_MASK;
    NVIC_EnableIRQ(ComBlk_IRQn);
}

/*==============================================================================
 *
 */
void MSS_COMBLK_send_cmd_with_ptr
(
    uint8_t cmd_opcode,
    uint32_t cmd_params_ptr,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
)
{
    uint32_t tx_okay;
    
    /*--------------------------------------------------------------------------
     * Disable and clear previous interrupts.
     */
    NVIC_DisableIRQ(ComBlk_IRQn);
    COMBLK->INT_ENABLE = 0u;
    NVIC_ClearPendingIRQ(ComBlk_IRQn);
    
    /*--------------------------------------------------------------------------
     * Abort current command if any.
     */
    abort_current_cmd();
    
    /*--------------------------------------------------------------------------
     * Initialize COMBLK driver state variables.
     */
    g_request_in_progress = 1u;
    g_comblk_cmd_opcode = cmd_opcode;
    g_comblk_p_cmd = 0u;
    g_comblk_cmd_size = 0u;
    g_comblk_p_data = 0u;
    g_comblk_data_size = 0u;
    g_comblk_p_response = p_response;
    g_comblk_response_size = response_size;
    g_comblk_response_idx = 0u;
    g_comblk_page_handler = 0u;
    g_comblk_completion_handler = completion_handler;
    
    /*--------------------------------------------------------------------------
     * Send command opcode as a single byte write to the Tx FIFO.
     */
	hal_printf("send1\r\n");
    send_cmd_opcode(g_comblk_cmd_opcode);
    
    /*--------------------------------------------------------------------------
     * Send the command parameters pointer to the Tx FIFO as a single 4 bytes
     * write to the Tx FIFO.
     */
    COMBLK->CONTROL |= CR_SIZETX_MASK;
    
	hal_printf("send2\r\n");
    /* Wait for space to become available in Tx FIFO. */
    do {
        tx_okay = COMBLK->STATUS & TXTOKAY_MASK;
    } while(0u == tx_okay);
    
    /* Send command opcode. */
    COMBLK->DATA32 = cmd_params_ptr;
    
    COMBLK->CONTROL &= ~CR_SIZETX_MASK;
    
    g_comblk_state = COMBLK_WAIT_RESPONSE;
    
	hal_printf("send3\r\n");
    /*--------------------------------------------------------------------------
     * Enable interrupt.
     */
    COMBLK->INT_ENABLE |= RCVOKAY_MASK;
    NVIC_EnableIRQ(ComBlk_IRQn);
	hal_printf("send4\r\n");
}

void MSS_COMBLK_send_cmd
(
    const uint8_t * p_cmd,
    uint16_t cmd_size,
    const uint8_t * p_data,
    uint32_t data_size,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
)
{
    uint32_t size_sent;
    
    ASSERT(cmd_size > 0);
    
    /*
     * Disable and clear previous interrupts.
     */
    NVIC_DisableIRQ(ComBlk_IRQn);
    COMBLK->INT_ENABLE = 0u;
    NVIC_ClearPendingIRQ(ComBlk_IRQn);
    
    /*
     * Abort current command if any.
     */
    abort_current_cmd();
    
    /*
     * Initialize COMBLK driver state variables:
     */
    g_request_in_progress = 1u;
    g_comblk_cmd_opcode = p_cmd[0];
    g_comblk_p_cmd = p_cmd;
    g_comblk_cmd_size = cmd_size;
    g_comblk_p_data = p_data;
    g_comblk_data_size = data_size;
    g_comblk_p_response = p_response;
    g_comblk_response_size = response_size;
    g_comblk_response_idx = 0u;
    g_comblk_page_handler = 0u;
    g_comblk_completion_handler = completion_handler;
    
    COMBLK->INT_ENABLE |= RCVOKAY_MASK;

    /*
     * Fill FIFO with command.
     */
    send_cmd_opcode(g_comblk_cmd_opcode);
    size_sent = fill_tx_fifo(&p_cmd[1], cmd_size - 1u);
    ++size_sent;    /* Adjust for opcode byte sent. */
    if(size_sent < cmd_size)
    {
        g_comblk_cmd_size = g_comblk_cmd_size - (uint16_t)size_sent;
        g_comblk_p_cmd = &g_comblk_p_cmd[size_sent];
        
        g_comblk_state = COMBLK_TX_CMD;
    }
    else
    {
        g_comblk_cmd_size = 0u;
        if(g_comblk_data_size > 0u)
        {
            g_comblk_state = COMBLK_TX_DATA;
        }
        else
        {
            g_comblk_state = COMBLK_WAIT_RESPONSE;
        }
    }

    /*
     * Enable interrupt.
     */
    NVIC_EnableIRQ(ComBlk_IRQn);
}


/*==============================================================================
 *
 */
static void request_completion_handler
(
    uint8_t * p_response,
    uint16_t response_size
)
{
	hal_printf("request_completion_handler\r\n");
    g_request_in_progress = 0u;
    g_last_response_length = response_size;
}

/*==============================================================================
 *
 */
static void signal_request_start(void)
{
    /* Wait for current request to complete. */
    while(g_request_in_progress)
    {
        ;
    }
    
    g_request_in_progress = 1u;
    g_last_response_length = 0u;
}

/*==============================================================================
 *
 */
static uint16_t wait_for_request_completion(void)
{
	hal_printf("wait_for_request_completion\r\n");
    while(g_request_in_progress)
    {
        ;
    }
    
    return g_last_response_length;
}

static void write_ptr_value_into_array
(
    const uint8_t * pointer,
    uint8_t target_array[],
    uint32_t array_index
)
{
    target_array[array_index] = (uint8_t)((uint32_t)pointer);
    target_array[array_index + 1] = (uint8_t)((uint32_t)pointer >> 8u);
    target_array[array_index + 2] = (uint8_t)((uint32_t)pointer >> 16u);
    target_array[array_index + 3] = (uint8_t)((uint32_t)pointer >> 24u);
}

/*==============================================================================
  Function to write array address to pointer location.
 */
static void write_array_into_ptr_value
(
    uint8_t** pointer,
    uint8_t target_array[],
    uint32_t array_index
)
{
    uint32_t var;
    
    var = (uint32_t)target_array[array_index + 3];
    var = ((var << 8u) & 0xFFFFFF00) | target_array[array_index + 2];
    var = ((var << 8u) & 0xFFFFFF00) | target_array[array_index + 1];
    var = ((var << 8u) & 0xFFFFFF00) | target_array[array_index];
    
    *pointer = (uint8_t*)var;
}

static void asynchronous_event_handler(uint8_t event_opcode)
{
	hal_printf("asynchronous_event_handler\r\n");
    if (event_opcode == FLASH_FREEZE_SHUTDOWN_OPCODE)
    {
        uint32_t running_on_standby_clock;
        volatile uint32_t timeout;
        const uint32_t apb_divisors_mask = 0x00000EFCU;

        /*
         * Wait for the System Controller to switch the system's clock
         * from the main clock to the  standby clock. This should take place
         * within 10us of receiving the shut-down event.
         */
        timeout = DELAY_MORE_THAN_10US;
        do
        {
            running_on_standby_clock = SYSREG->MSSDDR_FACC1_CR & FACC_GLMUX_SEL_MASK;
            --timeout;
        }
        while ((running_on_standby_clock == 0U) && (timeout != 0U));
        
        /*
         * Set the clock divisors to zero in order to set the AHB
         * to APB bridge's clock ratio between the AHB and APB busses to 1.
         * This is required to ensure correct bus transactions to the APB
         * peripherals while operating from the standby clock.
         */
        SYSREG->MSSDDR_FACC1_CR &= ~apb_divisors_mask;
        
        /* Call the user's event handler. */
        if(g_event_handler != 0)
        {
            g_event_handler(event_opcode, g_response[1]);
        }
    }
    else if (event_opcode == FLASH_FREEZE_EXIT_OPCODE)
    {
        uint32_t running_on_standby_clock;
        volatile uint32_t timeout;
        
        /*
         * Wait for the System Controller to switch the system's clock
         * from the standby clock to the main clock. This should take place
         * within 10us of receiving the shut-down event.
         */
        timeout = DELAY_MORE_THAN_10US;
        do
        {
            running_on_standby_clock = SYSREG->MSSDDR_FACC1_CR & FACC_GLMUX_SEL_MASK;
            --timeout;
        }
        while ((running_on_standby_clock != 0U) && (timeout != 0U));
        
        /* Restore the MSS clock dividers to their normal operations value. */
        SYSREG->MSSDDR_FACC1_CR = g_initial_mssddr_facc1_cr;
        
        if(g_event_handler != 0)
        {
            /* Call the user's event handler. */
            g_event_handler(event_opcode, g_response[1]);
        }
    }
    else
    {
		hal_printf("asynchronous_event_handler2\r\n");
        /*if ((event_opcode == POR_DIGEST_ERROR_OPCODE) || \
            ((event_opcode >= TAMPER_ATTEMPT_DETECT_OPCODE_RANGE_MIN) && \
            (event_opcode <= TAMPER_FAILURE_DETECT_OPCODE_RANGE_MAX)) || \
            (event_opcode == TAMPER_CLOCK_MONITOR_ERROR_OPCODE) || \
            ((event_opcode >= TAMPER_HARDWARE_MONITOR_ERROR_OPCODE_RANGE_MIN) && \
            (event_opcode <= TAMPER_HARDWARE_MONITOR_ERROR_OPCODE_RANGE_MAX)))*/
        {
            /* 
             * Inform to the application that new asynchronous message is received, 
             * only if application call-back function is registered during 
             * initialization. 
             */
            if(g_event_handler != 0)
            {
				hal_printf("asynchronous_event_handler3\r\n");
                /* Call the user's event handler. */
                g_event_handler(event_opcode, g_response[1]);
            }
        }
    }
}

static int testingNum = 0;
uint8_t eNVM_write_buff[5];

uint8_t generate_hmac
(
    const uint8_t * key,
    const uint8_t * p_data_in,
    uint32_t length,
    uint8_t * p_result
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[58];
    uint8_t status;
	
	if(g_request_in_progress){
		hal_printf("g_request_in_progress\r\n");
		return 0;
	}
	testingNum++;
	if (testingNum == 30){
		hal_printf("NVM_write\r\n");
		//NVM_write(0x60000000, eNVM_write_buff, 5, NVM_DO_NOT_LOCK_PAGE);
	}
    
    memcpy(&params[0], key, HMAC_KEY_LENGTH);
    
    params[32] = (uint8_t)((uint32_t)length);
    params[33] = (uint8_t)((uint32_t)length >> 8u);
    params[34] = (uint8_t)((uint32_t)length >> 16u);
    params[35] = (uint8_t)((uint32_t)length >> 24u);

    write_ptr_value_into_array(p_data_in, params, 36u);
    write_ptr_value_into_array(p_result, params, 40u);

    status = execute_service(HMAC_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

void ISR_ComBlk( void* Param ){
	//uint8_t* intParam = (uint8_t*)Param;
	//asynchronous_event_handler(*intParam);
	uint8_t status;
    uint8_t tx_okay;
    uint8_t rcv_okay;
    
    status = (uint8_t)COMBLK->STATUS;
    
    /* Mask off interrupt that are not enabled.*/
    status &= COMBLK->INT_ENABLE;
    
    rcv_okay = status & RCVOKAY_MASK;
    
    if(rcv_okay)
    {
        handle_rx_okay_irq();
    }
        
    tx_okay = status & TXTOKAY_MASK;
    if(tx_okay)
    {
        handle_tx_okay_irq();
    }
}

void MSS_SYS_init(sys_serv_async_event_handler_t event_handler){
	//MSS_SYS_init(MSS_SYS_NO_EVENT_HANDLER);
	hal_printf("MSS_SYS_init\r\n");
	g_event_handler = event_handler;
	g_last_response_length = 0u;
	g_request_in_progress = 0u;

	/*
	 * Set a default good value for g_initial_mssddr_facc1_cr used to control
	 * the clock dividers coming in and out of Flash*Freeze.
	 */
	g_initial_mssddr_facc1_cr = SYSREG->MSSDDR_FACC1_CR;

	//Initialize emote intc driver with the same interrupt handler to avoid conflict.
	CPU_INTC_ActivateInterrupt(VectorIndex::c_IRQ_INDEX_ComBlk, ISR_ComBlk, (void*)&g_opcode);

	/*
	 * Initialize the COMBLK used to communicate with the System Controller.
	 */

	MSS_COMBLK_init(asynchronous_event_handler, g_response);
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_get_serial_number
(
    uint8_t * p_serial_number
)
{
    uint8_t response[SERIAL_NUMBER_SERV_RESP_LENGTH];
    uint8_t status;
    
    status = execute_service(SERIAL_NUMBER_REQUEST_CMD,
                             p_serial_number,
                             response,
                             SERIAL_NUMBER_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_get_user_code
(
    uint8_t * p_user_code
)
{
    uint8_t response[USERCODE_SERV_RESP_LENGTH];
    uint8_t status;
    
    status = execute_service(USERCODE_REQUEST_CMD,
                             p_user_code,
                             response,
                             USERCODE_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_get_design_version
(
    uint8_t * p_design_version
)
{
    uint8_t response[DESIGNVER_SERV_RESP_LENGTH];
    uint8_t status;
    
    status = execute_service(DESIGNVER_REQUEST_CMD,
                             p_design_version,
                             response,
                             DESIGNVER_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_get_device_certificate
(
    uint8_t * p_device_certificate
)
{
    uint8_t response[DEVICE_CERT_SERV_RESP_LENGTH];
    uint8_t status;
    
    status = execute_service(DEVICE_CERTIFICATE_REQUEST_CMD,
                             p_device_certificate,
                             response,
                             DEVICE_CERT_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_get_secondary_device_certificate
(
    uint8_t * p_secondary_device_certificate
)
{
    uint8_t response[SECONDARY_DEVICE_CERT_SERV_RESP_LENGTH];
    uint8_t status;

    /*
     * The get secondary device certificate system service is not available on 
     * M2S050 rev A, rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    status = execute_service(SECONDARY_DEVICE_CERTIFICATE_REQUEST_CMD,
                             p_secondary_device_certificate,
                             response,
                             SECONDARY_DEVICE_CERT_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_flash_freeze(uint8_t options)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t flash_freeze_req[FLASH_FREEZE_REQUEST_LENGTH];
    uint8_t response[FLASH_FREEZE_SERV_RESP_LENGTH];

    /*
     * The Flash Freeze system service is not available on M2S050 rev A and rev B.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    
    /*
     * Keep track of the clocks configuration before entering Flash*Freeze so
     * that it can be restored on Flash*Freeze exit.
     */
    g_initial_mssddr_facc1_cr = SYSREG->MSSDDR_FACC1_CR;
    
    /*
     * Enable RTC wake-up interrupt to System Controller and FPGA fabric.
     */
    SYSREG->RTC_WAKEUP_CR |= (RTC_WAKEUP_G4C_EN_MASK | RTC_WAKEUP_FAB_EN_MASK);

    signal_request_start();

    flash_freeze_req[0] = FLASH_FREEZE_REQUEST_CMD;
    flash_freeze_req[1] = options;

    MSS_COMBLK_send_cmd(flash_freeze_req,               /* p_cmd */
                        FLASH_FREEZE_REQUEST_LENGTH,    /* cmd_size */
                        0,                              /* p_data */
                        0u,                             /* data_size */
                        response,                       /* p_response */
                        FLASH_FREEZE_SERV_RESP_LENGTH,  /* response_size */
                        request_completion_handler);    /* completion_handler */

    actual_response_length = wait_for_request_completion();
    
    if((FLASH_FREEZE_SERV_RESP_LENGTH == actual_response_length) &&
       (FLASH_FREEZE_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
#define AES128_KEY_LENGTH   16u
#define IV_LENGTH           16u

#define AES256_KEY_LENGTH   32u

#define HMAC_KEY_LENGTH     32u

uint8_t MSS_SYS_128bit_aes
(
    const uint8_t * key,
    const uint8_t * iv,
    uint16_t nb_blocks,
    uint8_t mode,
    uint8_t * dest_addr,
    const uint8_t * src_addr)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[44];
    uint8_t status;
    
    memcpy(&params[0], key, AES128_KEY_LENGTH);
    memcpy(&params[16], iv, IV_LENGTH);
    
    params[32] = (uint8_t)nb_blocks;
    params[33] = (uint8_t)(nb_blocks >> 8u);
    params[34] = mode;
    params[35] = 0u;

    write_ptr_value_into_array(dest_addr, params, 36u);
    write_ptr_value_into_array(src_addr, params, 40u);

    status = execute_service(AES128_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_256bit_aes
( 
    const uint8_t * key,
    const uint8_t * iv,
    uint16_t nb_blocks,
    uint8_t mode,
    uint8_t * dest_addr,
    const uint8_t * src_addr
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[60];
    uint8_t status;
    
    memcpy(&params[0], key, AES256_KEY_LENGTH);
    memcpy(&params[32], iv, IV_LENGTH);
    
    params[48] = (uint8_t)nb_blocks;
    params[49] = (uint8_t)(nb_blocks >> 8u);
    params[50] = mode;
    params[51] = 0u;

    write_ptr_value_into_array(dest_addr, params, 52u);
    write_ptr_value_into_array(src_addr, params, 56u);

    status = execute_service(AES256_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_sha256
(
    const uint8_t * p_data_in,
    uint32_t length,
    uint8_t * result
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[12];
    uint8_t status;
    
    params[0] = (uint8_t)((uint32_t)length);
    params[1] = (uint8_t)((uint32_t)length >> 8u);
    params[2] = (uint8_t)((uint32_t)length >> 16u);
    params[3] = (uint8_t)((uint32_t)length >> 24u);
    
    write_ptr_value_into_array(result, params, 4u);
    write_ptr_value_into_array(p_data_in, params, 8u);

    status = execute_service(SHA256_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_hmac
(
    const uint8_t * key,
    const uint8_t * p_data_in,
    uint32_t length,
    uint8_t * p_result
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[58];
    uint8_t status;
    
    memcpy(&params[0], key, HMAC_KEY_LENGTH);
    
    params[32] = (uint8_t)((uint32_t)length);
    params[33] = (uint8_t)((uint32_t)length >> 8u);
    params[34] = (uint8_t)((uint32_t)length >> 16u);
    params[35] = (uint8_t)((uint32_t)length >> 24u);

    write_ptr_value_into_array(p_data_in, params, 36u);
    write_ptr_value_into_array(p_result, params, 40u);

    status = execute_service(HMAC_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
#define KEYTREE_KEY_LENGTH      32
#define KEYTREE_PATH_LENGTH     16

uint8_t MSS_SYS_key_tree
(
    uint8_t* p_key,
    uint8_t op_type,
    const uint8_t* path
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[49];
    uint8_t status;
    
    memcpy(&params[0], p_key, KEYTREE_KEY_LENGTH);
    
    params[32] = op_type;
    
    memcpy(&params[33], path, KEYTREE_PATH_LENGTH);
    
    status = execute_service(KEYTREE_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
    
    if(status == MSS_SYS_SUCCESS)
    {
       memcpy(p_key, &params[0], KEYTREE_KEY_LENGTH);
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_challenge_response
(
    uint8_t* p_key,
    uint8_t op_type,
    const uint8_t* path
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[21];
    uint8_t status;
    
    params[0] = (uint8_t)((uint32_t)p_key);
    params[1] = (uint8_t)((uint32_t)p_key >> 8u);
    params[2] = (uint8_t)((uint32_t)p_key >> 16u);
    params[3] = (uint8_t)((uint32_t)p_key >> 24u);
    
    params[4] = op_type;
    
    memcpy(&params[5], path, KEYTREE_PATH_LENGTH);
    
    status = execute_service(PPUF_CHALLENGE_RESP_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_nrbg_reset(void)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t reset_cmd[4];
    uint8_t response[NRBG_RESET_SERV_RESP_LENGTH];
    
    signal_request_start();
    
    reset_cmd[0] = NRBG_RESET_REQUEST_CMD;
    reset_cmd[1] = 0u;
    reset_cmd[2] = 0u;
    reset_cmd[3] = 0u;

    MSS_COMBLK_send_cmd(reset_cmd,                      /* p_cmd */
                        sizeof(reset_cmd),              /* cmd_size */
                        0,                              /* p_data */
                        0,                              /* data_size */
                        response,                       /* p_response */
                        DRBG_RESET_SERV_RESP_LENGTH,    /* response_size */
                        request_completion_handler);    /* completion_handler */
    
     actual_response_length = wait_for_request_completion();
    
    if((NRBG_RESET_SERV_RESP_LENGTH == actual_response_length) &&
       (NRBG_RESET_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_nrbg_self_test(void)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t self_test;
    uint8_t response[NRBG_SELF_TEST_SERV_RESP_LENGTH];
    
    signal_request_start();
    
    self_test = NRBG_SELF_TEST_REQUEST_CMD;

    MSS_COMBLK_send_cmd(&self_test,                         /* p_cmd */
                        sizeof(self_test),                  /* cmd_size */
                        0,                                  /* p_data */
                        0,                                  /* data_size */
                        response,                           /* p_response */
                        NRBG_SELF_TEST_SERV_RESP_LENGTH,    /* response_size */
                        request_completion_handler);        /* completion_handler */
    
    actual_response_length = wait_for_request_completion();
    
    if((NRBG_SELF_TEST_SERV_RESP_LENGTH == actual_response_length) &&
       (NRBG_SELF_TEST_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_nrbg_instantiate
(
    const uint8_t * personalization_str,
    uint16_t personalization_str_length,
    uint8_t * p_nrbg_handle
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t instantiate_params[7];
    uint8_t status;
    
    write_ptr_value_into_array(personalization_str, instantiate_params, 0u);
    
    instantiate_params[4] = (uint8_t)personalization_str_length;
    instantiate_params[5] = (uint8_t)(personalization_str_length >> 8u);
    instantiate_params[6] = INVALID_NRBG_HANDLE;
    
    status = execute_service(NRBG_INSTANTIATE_REQUEST_CMD,
                             instantiate_params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    if(MSS_SYS_SUCCESS == status)
    {
        *p_nrbg_handle = instantiate_params[6];
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_nrbg_generate
(
    const uint8_t * p_requested_data,
    const uint8_t * p_additional_input,
    uint8_t requested_length,
    uint8_t additional_input_length,
    uint8_t pr_req,
    uint8_t nrbg_handle
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t generate_params[12];
    uint8_t status;

    write_ptr_value_into_array(p_requested_data, generate_params, 0u);
    write_ptr_value_into_array(p_additional_input, generate_params, 4u);

    generate_params[8] = requested_length;
    generate_params[9] = additional_input_length;
    generate_params[10] = pr_req;
    generate_params[11] = nrbg_handle;
    
    status = execute_service(NRBG_GENERATE_REQUEST_CMD,
                             generate_params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_nrbg_reseed
(
    const uint8_t * p_additional_input,
    uint8_t additional_input_length,
    uint8_t nrbg_handle
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[6];
    uint8_t status;

    write_ptr_value_into_array(p_additional_input, params, 0u);

    params[4] = (uint8_t)additional_input_length;
    params[5] = nrbg_handle;
    
    status = execute_service(NRBG_RESEED_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_nrbg_uninstantiate
(
    uint8_t nrbg_handle
)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t uninstantiate_req[2];
    uint8_t response[NRBG_UNINST_SERV_RESP_LENGTH];
    
    signal_request_start();
    
    uninstantiate_req[0] = NRBG_UNINSTANTIATE_REQUEST_CMD;
    uninstantiate_req[1] = nrbg_handle;

    MSS_COMBLK_send_cmd(uninstantiate_req,              /* p_cmd */
                        sizeof(uninstantiate_req),      /* cmd_size */
                        0,                              /* p_data */
                        0,                              /* data_size */
                        response,                       /* p_response */
                        NRBG_UNINST_SERV_RESP_LENGTH,   /* response_size */
                        request_completion_handler);    /* completion_handler */
    
    actual_response_length = wait_for_request_completion();
    
    if((NRBG_UNINST_SERV_RESP_LENGTH == actual_response_length) &&
       (NRBG_UNINSTANTIATE_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
void MSS_SYS_zeroize_device(void)
{
    /*
     * We only need to send one command byte to initiate zeroization.
     * The zeroization process should result in the Cortex-M3 being reset. This
     * means we should never reach the end of this function if zeroization is
     * actually performed.
     */
    uint8_t zeroization_req = ZEROIZATION_REQUEST_CMD;
    
    /*
     * The Zeroization system service is not available on M2S050 rev A, rev B
     * and rev C.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    
    signal_request_start();
    
    MSS_COMBLK_send_cmd(&zeroization_req,               /* p_cmd */
                        sizeof(zeroization_req),        /* cmd_size */
                        0,                              /* p_data */
                        0,                              /* data_size */
                        0,                              /* p_response */
                        0,                              /* response_size */
                        request_completion_handler);    /* completion_handler */
    
    /*
     * Handle case where zeroization is not enabled in the device.
     */
    wait_for_request_completion();
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
#define NVM_FREQRNG_MASK        0x00001FE0U
#define NVM_FREQRNG_MAX         ((uint32_t)0xFFU << 5U)

static uint8_t g_isp_response[ISP_PROG_SERV_RESP_LENGTH];
sys_serv_isp_complete_handler_t g_isp_completion_handler = 0;
comblk_page_handler_t g_isp_page_read_handler = 0;

/*
 * g_initial_envm_cr contains the hardware design's original eNVM configuration
 * set through the ENVM_CR system register. This global variable is used to
 * store the eNVM's configuration while the ISP/IAP is executing with an eNVM
 * configuration ensuring successful eNVM programming on all SamrtFusion2
 * devices. It is then used to restore the eNVM's configuration once ISP/IAP has
 * completed. SAR 57545.
 */
static uint32_t g_initial_envm_cr = 0x00001FF1U;
/*
 * g_initial_mssddr_facc2_cr contains the hardware design's original MSS DDR 
 * Fabric Alignment Clock Controller (FACC) 2 configuration set through the 
 * MSSDDR_FACC2_CR system register. This global variable is used to
 * store the FACC2's configuration while the ISP/IAP is executing on all 
 * SamrtFusion2 devices. It is then used to restore the Fabric alignment clock 
 * configuration once ISP/IAP has completed.
 */
static uint32_t g_initial_mssddr_facc2_cr = 0x00;
static uint8_t g_mode = 0;
static uint8_t wait_for_clock_switch = 1;

static uint32_t isp_page_read_handler
(
    uint8_t const ** pp_next_page
)
{
    uint32_t remaining_length = 0;
    uint32_t running_on_standby_clock;
    volatile uint32_t timeout;
    const uint32_t apb_divisors_mask = 0x00000EFCU;
    
    if((g_mode !=  MSS_SYS_PROG_AUTHENTICATE) & (wait_for_clock_switch == 1))
    {
        timeout = DELAY_MORE_THAN_10US;
        do
        {
            running_on_standby_clock = SYSREG->MSSDDR_FACC1_CR & FACC_GLMUX_SEL_MASK;
            --timeout;
        }
        while ((running_on_standby_clock == 0U) && (timeout != 0U));
        wait_for_clock_switch = 0;
        SYSREG->MSSDDR_FACC1_CR &= ~apb_divisors_mask;
    }
    
    if(g_isp_page_read_handler != 0)
    {
        remaining_length = g_isp_page_read_handler(pp_next_page);
    }
    
    return remaining_length;
}

static void isp_sys_completion_handler
(
    uint8_t * p_response, 
    uint16_t length
)
{
    /*
     * Restore the eNVM's frequency range to the values used before ISP was
     * started.
     */
    SYSREG->ENVM_CR = g_initial_envm_cr;
    
    /*
     * Restore the MSS DDR FACC 2 configuration to the values used before ISP 
     * was started.
     */
    SYSREG->MSSDDR_FACC2_CR = g_initial_mssddr_facc2_cr;
    
    if(g_isp_completion_handler != 0)
    {
        g_isp_completion_handler(p_response[1]);
    }
}

void MSS_SYS_start_isp
(
    uint8_t mode,
    comblk_page_handler_t page_read_handler,
    sys_serv_isp_complete_handler_t isp_completion_handler
)
{
    uint8_t isp_prog_request[2];
        
    if(mode == MSS_SYS_PROG_VERIFY)
    {
        /*
         * Check fabric digest before performing ISP verify
         */
        MSS_SYS_check_digest(MSS_SYS_DIGEST_CHECK_FABRIC);
    }
    /*
     * Keep a copy of the initial eNVM configuration used before ISP was
     * initiated. The eNVM configuration will be restored, as part of the ISP
     * completion handler, when ISP completes.
     */
    g_initial_envm_cr = SYSREG->ENVM_CR;
    
    /* Store the MSS DDR FACC 2 register value so that its can be restored back 
     * when the ISP operation is completed.asynchronous_event_handler. */
    g_initial_mssddr_facc2_cr = SYSREG->MSSDDR_FACC2_CR;

    /*
     * Set the eNVM's frequency range to its maximum. This is required to ensure
     * successful eNVM programming on all devices.
     */
    SYSREG->ENVM_CR = (g_initial_envm_cr & ~NVM_FREQRNG_MASK) | NVM_FREQRNG_MAX;
    
    g_mode = mode;
    
    if(mode != MSS_SYS_PROG_AUTHENTICATE)
    {  
        /* Select output of MUX 0, MUX 1 and MUX 2 during standby */
        SYSREG->MSSDDR_FACC2_CR = SYSREG->MSSDDR_FACC2_CR & ((uint32_t)(FACC_STANDBY_SEL << FACC_STANDBY_SHIFT) & FACC_STANDBY_SEL_MASK);
        
        /* Enable the signal for the 50 MHz RC oscillator */
        SYSREG->MSSDDR_FACC2_CR = SYSREG->MSSDDR_FACC2_CR | ((uint32_t)(MSS_25_50MHZ_EN << MSS_25_50MHZ_EN_SHIFT) & MSS_25_50MHZ_EN_MASK);
        
        /* Enable the signal for the 1 MHz RC oscillator */
        SYSREG->MSSDDR_FACC2_CR = SYSREG->MSSDDR_FACC2_CR | ((uint32_t)(MSS_1MHZ_EN << MSS_1MHZ_EN_SHIFT) & MSS_1MHZ_EN_MASK);
        
        wait_for_clock_switch = 1;
    }
    
    signal_request_start();
    
    isp_prog_request[0] = ISP_PROGRAMMING_REQUEST_CMD;
    isp_prog_request[1] = mode;
    
    g_isp_completion_handler = isp_completion_handler;
    
    g_isp_page_read_handler = page_read_handler;
    
    MSS_COMBLK_send_paged_cmd(isp_prog_request,                 /* p_cmd */
                              sizeof(isp_prog_request),         /* cmd_size */
                              g_isp_response,                   /* p_response */
                              ISP_PROG_SERV_RESP_LENGTH,        /* response_size */
                              isp_page_read_handler,            /* page_handler */
                              isp_sys_completion_handler);      /* completion_handler */
}


/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_initiate_iap
(
    uint8_t mode,
    uint32_t bitstream_spi_addr
)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t iap_prog_req[6];
    uint8_t response[IAP_PROG_SERV_RESP_LENGTH];
    
    if(mode == MSS_SYS_PROG_VERIFY)
    {
        /*
         * Check fabric digest before performing IAP verify
         */
        MSS_SYS_check_digest(MSS_SYS_DIGEST_CHECK_FABRIC);
    }
    
    /*
     * Keep a copy of the initial eNVM configuration used before IAP was
     * initiated. The eNVM configuration will be restored, as part of the IAP
     * completion handler, when IAP completes.
     */
    g_initial_envm_cr = SYSREG->ENVM_CR;

    /* Store the MSS DDR FACC 2 register value so that its can be restored back 
     * when the IAP operation is completed.asynchronous_event_handler. */
    g_initial_mssddr_facc2_cr = SYSREG->MSSDDR_FACC2_CR;
    
    /*
     * Keep track of the clocks configuration before issuing IAP command so
     * that it can be restored on completion of IAP service.
     */
    g_initial_mssddr_facc1_cr = SYSREG->MSSDDR_FACC1_CR;
    
    /*
     * Set the eNVM's frequency range to its maximum. This is required to ensure
     * successful eNVM programming on all devices.
     */
    SYSREG->ENVM_CR = (g_initial_envm_cr & ~NVM_FREQRNG_MASK) | NVM_FREQRNG_MAX;

    /* Select output of MUX 0, MUX 1 and MUX 2 during standby */
    SYSREG->MSSDDR_FACC2_CR = SYSREG->MSSDDR_FACC2_CR & ((uint32_t)(FACC_STANDBY_SEL << FACC_STANDBY_SHIFT) & FACC_STANDBY_SEL_MASK);
    
    /* Enable the signal for the 50 MHz RC oscillator */
    SYSREG->MSSDDR_FACC2_CR = SYSREG->MSSDDR_FACC2_CR | ((uint32_t)(MSS_25_50MHZ_EN << MSS_25_50MHZ_EN_SHIFT) & MSS_25_50MHZ_EN_MASK);
    
    /* Enable the signal for the 1 MHz RC oscillator */
    SYSREG->MSSDDR_FACC2_CR = SYSREG->MSSDDR_FACC2_CR | ((uint32_t)(MSS_1MHZ_EN << MSS_1MHZ_EN_SHIFT) & MSS_1MHZ_EN_MASK);
    
    /*
     * There is no response for Program mode because the Cortex-M3 will
     * get reset on completion of the system service.
     */
    iap_prog_req[0] = IAP_PROGRAMMING_REQUEST_CMD;
    iap_prog_req[1] = mode;

    iap_prog_req[2] = (uint8_t)(bitstream_spi_addr);
    iap_prog_req[3] = (uint8_t)(bitstream_spi_addr >> 8u);
    iap_prog_req[4] = (uint8_t)(bitstream_spi_addr >> 16u);
    iap_prog_req[5] = (uint8_t)(bitstream_spi_addr >> 24u);

    signal_request_start();
    
    MSS_COMBLK_send_cmd(iap_prog_req,                   /* p_cmd */
                        sizeof(iap_prog_req),           /* cmd_size */
                        0,                              /* p_data */
                        0,                              /* data_size */
                        response,                      /* p_response */
                        IAP_PROG_SERV_RESP_LENGTH,      /* response_size */
                        request_completion_handler);    /* completion_handler */
        
    /*
     * Handle case where service is not implemented/enabled in the device.
     */
    actual_response_length = wait_for_request_completion();
    
    if((IAP_PROG_SERV_RESP_LENGTH == actual_response_length) &&
       (IAP_PROGRAMMING_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    /* Restore back to original value. */
    SYSREG->ENVM_CR = g_initial_envm_cr;
    SYSREG->MSSDDR_FACC2_CR = g_initial_mssddr_facc2_cr;
    SYSREG->MSSDDR_FACC1_CR = g_initial_mssddr_facc1_cr;

    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_check_digest
(
    uint8_t options
)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t digest_check_req[2];
    uint8_t response[DIGEST_CHECK_SERV_RESP_LENGTH];
    
    /*
     * The Digest Check system service is not available on M2S050 rev A, rev B and rev C.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    
    /* 
     * Private ENVM factory digest and user digest is available only on G4X 
     * devices
     */
    if((options & 0x30u) != 0x00)
    {
        ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    }
    
    signal_request_start();
    
    digest_check_req[0] = DIGEST_CHECK_REQUEST_CMD;
    digest_check_req[1] = options;

    MSS_COMBLK_send_cmd(digest_check_req,               /* p_cmd */
                        sizeof(digest_check_req),       /* cmd_size */
                        0,                              /* p_data */
                        0u,                             /* data_size */
                        response,                       /* p_response */
                        DIGEST_CHECK_SERV_RESP_LENGTH,  /* response_size */
                        request_completion_handler);    /* completion_handler */
    
    actual_response_length = wait_for_request_completion();
    
    if((DIGEST_CHECK_SERV_RESP_LENGTH == actual_response_length) &&
       (DIGEST_CHECK_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_puf_create_activation_code
(
    void
)
{
    uint8_t response[PUF_USER_ACTIVATION_CODE_RESP_LENGTH];
    uint8_t status;
    uint8_t params;
    uint8_t key_numbers = 0u;
    
    /*
     * The user activation code system service is not available on M2S050 rev A,
     * rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);

    params = PUF_CREATE_USER_ACTIVATION_CODE;
    
    status = execute_service(PUF_ACTIVATION_CODE_REQUEST_CMD,
                             &params,
                             response,
                             PUF_USER_ACTIVATION_CODE_RESP_LENGTH);  
    
    /*
     * System controller is locking eNVM-1 while executing create activation 
     * code service, but system controller is not releasing back after 
     * completing the operation. In order to unlock eNVM-1, call get number of 
     * key service. PUF get number of key service will release the eNVM-1 lock
     * after reading the number of keys enrolled.
     */
    MSS_SYS_puf_get_number_of_keys(&key_numbers);
    
    return status;
}
    
/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_puf_delete_activation_code
(
    void
)
{
    uint8_t response[PUF_USER_ACTIVATION_CODE_RESP_LENGTH];
    uint8_t status;
    uint8_t params;
    uint8_t key_numbers = 0u;
    
    /*
     * The user activation code system service is not available on M2S050 rev A,
     * rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    params = PUF_DELETE_USER_ACTIVATION_CODE;
    
    status = execute_service(PUF_ACTIVATION_CODE_REQUEST_CMD,
                             &params,
                             response,
                             PUF_USER_ACTIVATION_CODE_RESP_LENGTH);
    /*
     * System controller is locking eNVM-1 while executing delete activation 
     * code service, but system controller is not releasing back after 
     * completing the operation. In order to unlock eNVM-1, call get number of 
     * key service. PUF get number of key service will release the eNVM-1 lock
     * after reading the number of keys enrolled.
     */
    MSS_SYS_puf_get_number_of_keys(&key_numbers);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_puf_get_number_of_keys
(
    uint8_t* p_number_of_keys
)
{
    uint8_t response[6u] = { 0x00 };
    uint8_t params[11u] =  { 0x00 };
    uint8_t status;
    
    /*
     * The user key code system service is not available on M2S050 rev A,
     * rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);

    params[0] = PUF_GET_NUMBER_OF_KC_SUBCOMMAND;    
    
    status = execute_service(PUF_USER_KEY_CODE_REQUEST_CMD,
                             params,
                             response,
                             PUF_GET_NUMBER_OF_KEYS_RESP_LENGTH);
    
    *p_number_of_keys = params[9];
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_puf_enroll_key
(
    uint8_t key_number,
    uint16_t key_size,
    uint8_t* p_key_value,
    uint8_t* p_key_location
)
{
    uint8_t response[6u];
    uint8_t params[11u];
    uint8_t status;
    uint8_t key_numbers = 0u;
    
    /*
     * The PUF enroll key system service is not available on M2S050 rev A,
     * rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);

    if(p_key_value == 0)
    {
        params[0] = PUF_CREATE_INT_KC_SUBCOMMAND;
    }
    else
    {
        params[0] = PUF_CREATE_EXT_KC_SUBCOMMAND;
    }
    
    write_ptr_value_into_array(p_key_location, params, 1u);
    write_ptr_value_into_array(p_key_value, params, 5u);
    
    params[9] = key_number;
    params[10] = key_size;
    
    status = execute_service(PUF_USER_KEY_CODE_REQUEST_CMD,
                             params,
                             response,
                             PUF_ENROLL_KEYS_RESP_LENGTH);
    
    /*
     * System controller is locking eNVM-1 while executing create key code 
     * service, but system controller is not releasing back after completing the
     * operation. In order to unlock eNVM-1, call get number of key service. PUF
     * get number of key service will release the eNVM-1 lock after reading the 
     * number of keys enrolled.
     */
    MSS_SYS_puf_get_number_of_keys(&key_numbers);
       
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_puf_delete_key
(
    uint8_t key_number
)
{
    uint8_t response[6u];
    uint8_t params[11u];
    uint8_t status;
    uint8_t key_numbers = 0u;
    
    /*
     * The delete PUF key system service is not available on M2S050 rev A,
     * rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);

    params[0] = PUF_DELETE_KC_SUBCOMMAND;
    params[9] = key_number;
        
    status = execute_service(PUF_USER_KEY_CODE_REQUEST_CMD,
                             params,
                             response,
                             PUF_ENROLL_KEYS_RESP_LENGTH);
       
    /*
     * System controller is locking eNVM-1 while executing delete key code 
     * service, but system controller is not releasing back after completing the
     * operation. In order to unlock eNVM-1, call get number of key service. PUF
     * get number of key service will release the eNVM-1 lock after reading the 
     * number of keys enrolled.
     */
    MSS_SYS_puf_get_number_of_keys(&key_numbers);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_puf_fetch_key
(
    uint8_t key_number,
    uint8_t ** pp_key
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[5];
    uint8_t status;
    
    /*
     * The fetch user key system service is not available on M2S050 rev A,
     * rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    write_ptr_value_into_array(*pp_key, params, 0u);
    
    params[4] = key_number;
    
    status = execute_service(PUF_FETCH_KEY_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
    if(pp_key != NULL)
    {
        write_array_into_ptr_value(&(*pp_key), params, 0);
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_puf_export_keycodes
(
    uint8_t * p_keycodes
)
{
    uint8_t response[PUF_EXPORT_ALL_KEYCODES_RESP_LENGTH];
    uint8_t params[11u];
    uint8_t status;
    
    /*
     * The export all user key system service is not available on M2S050 rev A,
     * rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);

    params[0] = PUF_EXPORT_ALL_KC_SUBCOMMAND;
    
    write_ptr_value_into_array(p_keycodes, params, 1u);
        
    status = execute_service(PUF_USER_KEY_CODE_REQUEST_CMD,
                             params,
                             response,
                             PUF_EXPORT_ALL_KEYCODES_RESP_LENGTH);
       
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_puf_import_keycodes
(
    uint8_t * p_keycodes
)
{
    uint8_t response[PUF_IMPORT_ALL_KEYCODES_RESP_LENGTH];
    uint8_t params[11u];
    uint8_t status;
    
    /*
     * The import all key code system service is not available on M2S050 rev A,
     * rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);

    params[0] = PUF_IMPORT_ALL_KC_SUBCOMMAND;
    
    write_ptr_value_into_array(p_keycodes, params, 1u);
    
    status = execute_service(PUF_USER_KEY_CODE_REQUEST_CMD,
                             params,
                             response,
                             PUF_IMPORT_ALL_KEYCODES_RESP_LENGTH);
        
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_puf_fetch_ecc_public_key
(
    uint8_t* p_puf_public_key
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[4];
    uint8_t status;

    /*
     * The fetch puf ecc public key system service is not available on M2S050
     * rev A, rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    write_ptr_value_into_array(p_puf_public_key, params, 0u);
    
    status = execute_service(PUF_ECC_PUBLIC_KEY_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_puf_get_random_seed
(
    uint8_t* p_puf_seed
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[4];
    uint8_t status;
    
    /*
     * The get puf seed system service is not available on M2S050 rev A, rev B,
     * rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    write_ptr_value_into_array(p_puf_seed, params, 0u);
    
    status = execute_service(PUF_SEED_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_ecc_point_multiplication
(
    uint8_t* p_scalar_d,
    uint8_t* p_point_p,
    uint8_t* p_point_q
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[12];
    uint8_t status;
    
    /*
     * The ECC point multiplication system service is not available on M2S050 
     * rev A, rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    write_ptr_value_into_array(p_scalar_d, params, 0u);
    write_ptr_value_into_array(p_point_p, params, 4u);
    write_ptr_value_into_array(p_point_q, params, 8u);
    
    status = execute_service(POINT_MULTIPLICATION_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_ecc_point_addition
(
    uint8_t* p_point_p,
    uint8_t* p_point_q,
    uint8_t* p_point_r
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[12];
    uint8_t status;
    
    /*
     * The ECC point addition system service is not available on M2S050 
     * rev A, rev B, rev C and rev D.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    write_ptr_value_into_array(p_point_p, params, 0u);
    write_ptr_value_into_array(p_point_q, params, 4u);
    write_ptr_value_into_array(p_point_r, params, 8u);
    
    status = execute_service(POINT_ADDITION_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
void MSS_SYS_ecc_get_base_point
(
    uint8_t* p_point_g
)
{
    const uint8_t base_point_g[] = 
    {
        0xaa, 0x87, 0xca, 0x22, 0xbe, 0x8b, 0x05, 0x37, 0x8e, 0xb1, 0xc7, 0x1e,
        0xf3, 0x20, 0xad, 0x74, 0x6e, 0x1d, 0x3b, 0x62, 0x8b, 0xa7, 0x9b, 0x98,
        0x59, 0xf7, 0x41, 0xe0, 0x82, 0x54, 0x2a, 0x38, 0x55, 0x02, 0xf2, 0x5d,
        0xbf, 0x55, 0x29, 0x6c, 0x3a, 0x54, 0x5e, 0x38, 0x72, 0x76, 0x0a, 0xB7,
        0x36, 0x17, 0xde, 0x4a, 0x96, 0x26, 0x2c, 0x6f, 0x5d, 0x9e, 0x98, 0xbf,
        0x92, 0x92, 0xdc, 0x29, 0xf8, 0xf4, 0x1d, 0xbd, 0x28, 0x9a, 0x14, 0x7c,
        0xe9, 0xda, 0x31, 0x13, 0xb5, 0xf0, 0xb8, 0xc0, 0x0a, 0x60, 0xb1, 0xce,
        0x1d, 0x7e, 0x81, 0x9d, 0x7a, 0x43, 0x1d, 0x7c, 0x90, 0xea, 0x0e, 0x5F
    };
    
    memcpy(p_point_g, &base_point_g[0], sizeof(base_point_g));
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_start_clock_monitor
(
    void
)
{
    uint8_t status;
    uint8_t tamper_control_req[2];
    uint8_t response[TAMPER_CONTROL_SERV_RESP_LENGTH];
    uint16_t actual_response_length;
    
    /*
     * The Start clock monitoring tamper Control service is available only on 
     * G4X device.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    signal_request_start();
    
    tamper_control_req[0] = TAMPER_CONTROL_REQUEST_CMD;
    tamper_control_req[1] = 0x01u;

    MSS_COMBLK_send_cmd(tamper_control_req,                 /* p_cmd */
                        sizeof(tamper_control_req),         /* cmd_size */
                        0,                                  /* p_data */
                        0u,                                 /* data_size */
                        response,                           /* p_response */
                        TAMPER_CONTROL_SERV_RESP_LENGTH,    /* response_size */
                        request_completion_handler);        /* completion_handler */
    
    actual_response_length = wait_for_request_completion();
    
    if((TAMPER_CONTROL_SERV_RESP_LENGTH == actual_response_length) &&
       (TAMPER_CONTROL_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_stop_clock_monitor
(
    void
)
{
    uint8_t status;
    uint8_t tamper_control_req[2];
    uint8_t response[TAMPER_CONTROL_SERV_RESP_LENGTH];
    uint16_t actual_response_length;
    
    /*
     * The Stop clock monitoring tamper Control service is available only on 
     * G4X device.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    signal_request_start();
    
    tamper_control_req[0] = TAMPER_CONTROL_REQUEST_CMD;
    tamper_control_req[1] = 0x02u;

    MSS_COMBLK_send_cmd(tamper_control_req,                 /* p_cmd */
                        sizeof(tamper_control_req),         /* cmd_size */
                        0,                                  /* p_data */
                        0u,                                 /* data_size */
                        response,                           /* p_response */
                        TAMPER_CONTROL_SERV_RESP_LENGTH,    /* response_size */
                        request_completion_handler);        /* completion_handler */
    
    actual_response_length = wait_for_request_completion();
    
    if((TAMPER_CONTROL_SERV_RESP_LENGTH == actual_response_length) &&
       (TAMPER_CONTROL_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_enable_puf_power_down
(
    void
)
{
    uint8_t status;
    uint8_t tamper_control_req[2];
    uint8_t response[TAMPER_CONTROL_SERV_RESP_LENGTH];
    uint16_t actual_response_length;
    
    /*
     * The Enable PUF power down service is available only on G4X device.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    signal_request_start();
    
    tamper_control_req[0] = TAMPER_CONTROL_REQUEST_CMD;
    tamper_control_req[1] = 0x04u;

    MSS_COMBLK_send_cmd(tamper_control_req,                 /* p_cmd */
                        sizeof(tamper_control_req),         /* cmd_size */
                        0,                                  /* p_data */
                        0u,                                 /* data_size */
                        response,                           /* p_response */
                        TAMPER_CONTROL_SERV_RESP_LENGTH,    /* response_size */
                        request_completion_handler);        /* completion_handler */
    
    actual_response_length = wait_for_request_completion();
    
    if((TAMPER_CONTROL_SERV_RESP_LENGTH == actual_response_length) &&
       (TAMPER_CONTROL_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_disable_puf_power_down
(
    void
)
{
    uint8_t status;
    uint8_t tamper_control_req[2];
    uint8_t response[TAMPER_CONTROL_SERV_RESP_LENGTH];
    uint16_t actual_response_length;
    
    /*
     * Disable PUF power down is available only on G4X device.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    signal_request_start();
    
    tamper_control_req[0] = TAMPER_CONTROL_REQUEST_CMD;
    tamper_control_req[1] = 0x08u;

    MSS_COMBLK_send_cmd(tamper_control_req,                 /* p_cmd */
                        sizeof(tamper_control_req),         /* cmd_size */
                        0,                                  /* p_data */
                        0u,                                 /* data_size */
                        response,                           /* p_response */
                        TAMPER_CONTROL_SERV_RESP_LENGTH,    /* response_size */
                        request_completion_handler);        /* completion_handler */
    
    actual_response_length = wait_for_request_completion();
    
    if((TAMPER_CONTROL_SERV_RESP_LENGTH == actual_response_length) &&
       (TAMPER_CONTROL_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_clear_lock_parity
(
    void
)
{
    uint8_t status;
    uint8_t tamper_control_req[2];
    uint8_t response[TAMPER_CONTROL_SERV_RESP_LENGTH];
    uint16_t actual_response_length;
    
    /*
     * The Clear Lock parity is available only on G4X device.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    signal_request_start();
    
    tamper_control_req[0] = TAMPER_CONTROL_REQUEST_CMD;
    tamper_control_req[1] = 0x10u;

    MSS_COMBLK_send_cmd(tamper_control_req,                 /* p_cmd */
                        sizeof(tamper_control_req),         /* cmd_size */
                        0,                                  /* p_data */
                        0u,                                 /* data_size */
                        response,                           /* p_response */
                        TAMPER_CONTROL_SERV_RESP_LENGTH,    /* response_size */
                        request_completion_handler);        /* completion_handler */
    
    actual_response_length = wait_for_request_completion();
    
    if((TAMPER_CONTROL_SERV_RESP_LENGTH == actual_response_length) &&
       (TAMPER_CONTROL_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_clear_mesh_short
(
    void
)
{
    uint8_t status;
    uint8_t tamper_control_req[2];
    uint8_t response[TAMPER_CONTROL_SERV_RESP_LENGTH];
    uint16_t actual_response_length;
    
    /*
     * The Clear mesh short service is available only on G4X device.
     */
    ASSERT(0x0000F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0001F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0002F802u != SYSREG->DEVICE_VERSION);
    ASSERT(0x0003F802u != SYSREG->DEVICE_VERSION);
    
    signal_request_start();
    
    tamper_control_req[0] = TAMPER_CONTROL_REQUEST_CMD;
    tamper_control_req[1] = 0x20u;

    MSS_COMBLK_send_cmd(tamper_control_req,                 /* p_cmd */
                        sizeof(tamper_control_req),         /* cmd_size */
                        0,                                  /* p_data */
                        0u,                                 /* data_size */
                        response,                           /* p_response */
                        TAMPER_CONTROL_SERV_RESP_LENGTH,    /* response_size */
                        request_completion_handler);        /* completion_handler */
    
    actual_response_length = wait_for_request_completion();
    
    if((TAMPER_CONTROL_SERV_RESP_LENGTH == actual_response_length) &&
       (TAMPER_CONTROL_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

static void abort_current_cmd(void)
{
	hal_printf("abort_current_cmd\r\n");
    if(g_request_in_progress)
    {
        uint32_t flush_in_progress;
        
        /*
         * Call completion handler just in case we are in a multi threaded system
         * to avoid a task lockup.
         */
        complete_request(g_comblk_response_idx);
        
        /*
         * Flush the FIFOs
         */
        COMBLK->CONTROL |= CR_FLUSHOUT_MASK;
        do {
            flush_in_progress = COMBLK->CONTROL & CR_FLUSHOUT_MASK;
        } while(flush_in_progress);
    }
}

/*==============================================================================
 *
 */
static void send_cmd_opcode
(
    uint8_t opcode
)
{
    uint32_t tx_okay;
    
    /* Set transmit FIFO to transfer bytes. */
    COMBLK->CONTROL &= ~CR_SIZETX_MASK;
    
    /* Wait for space to become available in Tx FIFO. */
    do {
        tx_okay = COMBLK->STATUS & TXTOKAY_MASK;
    } while(0u == tx_okay);
    
    /* Send command opcode. */
    COMBLK->FRAME_START8 = opcode;
}

/*==============================================================================
 *
 */
static uint32_t fill_tx_fifo
(
    const uint8_t * p_cmd,
    uint32_t cmd_size
)
{
    volatile uint32_t tx_okay;
    uint32_t size_sent;

    /* Set transmit FIFO to transfer bytes. */
    COMBLK->CONTROL &= ~CR_SIZETX_MASK;
    
    size_sent = 0u;
    tx_okay = COMBLK->STATUS & TXTOKAY_MASK;
    while((tx_okay != 0u) && (size_sent < cmd_size))
    {
        COMBLK->DATA8 = p_cmd[size_sent];
        ++size_sent;
        tx_okay = COMBLK->STATUS & TXTOKAY_MASK;
    }
    
    return size_sent;
}

static void complete_request
(
    uint16_t response_length
)
{
	hal_printf("complete_request\r\n");
    if(g_comblk_completion_handler != 0)
    {
        g_comblk_completion_handler(g_comblk_p_response, response_length);
        g_comblk_completion_handler = 0;
        g_request_in_progress = 0u;
    }
}

static void handle_tx_okay_irq(void)
{
    switch(g_comblk_state)
    {
        /*----------------------------------------------------------------------
         * The TX_OKAY interrupt should only be enabled for states COMBLK_TX_CMD
         * and COMBLK_TX_DATA.
         */
        case COMBLK_TX_CMD:
            if(g_comblk_cmd_size > 0u)
            {
                uint32_t size_sent;
                size_sent = fill_tx_fifo(g_comblk_p_cmd, g_comblk_cmd_size);
                if(size_sent < g_comblk_cmd_size)
                {
                    g_comblk_cmd_size = g_comblk_cmd_size - (uint16_t)size_sent;
                    g_comblk_p_cmd = &g_comblk_p_cmd[size_sent];
                }
                else
                {
                    g_comblk_cmd_size = 0u;
                    if(g_comblk_data_size > 0u)
                    {
                        g_comblk_state = COMBLK_TX_DATA;
                    }
                    else
                    {
                        g_comblk_state = COMBLK_WAIT_RESPONSE;
                    }
                }
            }
            else
            {
                /*
                 * This is an invalid situation indicating a bug in the driver
                 * or corrupted memory.
                 */
                ASSERT(0);
                abort_current_cmd();
            }
        break;
            
        case COMBLK_TX_DATA:
            if(g_comblk_data_size > 0u)
            {
                uint32_t size_sent;
                size_sent = fill_tx_fifo(g_comblk_p_data, g_comblk_data_size);
                if(size_sent < g_comblk_data_size)
                {
                    g_comblk_data_size = g_comblk_data_size - size_sent;
                    g_comblk_p_data = &g_comblk_p_data[size_sent];
                }
                else
                {
                    COMBLK->INT_ENABLE &= ~TXTOKAY_MASK;
                    g_comblk_state = COMBLK_WAIT_RESPONSE;
                }
            }
            else
            {
                /*
                 * This is an invalid situation indicating a bug in the driver
                 * or corrupted memory.
                 */
                ASSERT(0);
                abort_current_cmd();
            }
        break;
           
        case COMBLK_TX_PAGED_DATA:
            /*
             * Read a page of data if required.
             */
            if(0u == g_comblk_data_size)
            {
                if(g_comblk_page_handler != 0)
                {
                    g_comblk_data_size = g_comblk_page_handler(&g_comblk_p_data);
                    if(0u == g_comblk_data_size)
                    {
                        COMBLK->INT_ENABLE &= ~TXTOKAY_MASK;
                        g_comblk_state = COMBLK_WAIT_RESPONSE;
                    }
                }
                else
                {
                    ASSERT(0);
                    abort_current_cmd();
                }
            }
            
            /*
             * Transmit the page data or move to COMBLK_WAIT_RESPONSE state if
             * no further page data could be obtained by the call to the page
             * handler above.
             */
            if(0u == g_comblk_data_size)
            {
                COMBLK->INT_ENABLE &= ~TXTOKAY_MASK;
                g_comblk_state = COMBLK_WAIT_RESPONSE;
            }
            else
            {
                uint32_t size_sent;
                size_sent = fill_tx_fifo(g_comblk_p_data, g_comblk_data_size);
                g_comblk_data_size = g_comblk_data_size - size_sent;
                g_comblk_p_data = &g_comblk_p_data[size_sent];
            }
        break;
            
        /*----------------------------------------------------------------------
         * The TX_OKAY interrupt should NOT be enabled for states COMBLK_IDLE,
         * COMBLK_WAIT_RESPONSE and COMBLK_RX_RESPONSE.
         */
        case COMBLK_IDLE:
            /* Fall through */
        case COMBLK_WAIT_RESPONSE:
            /* Fall through */
        case COMBLK_RX_RESPONSE:
            /* Fall through */
        default:
            COMBLK->INT_ENABLE &= ~TXTOKAY_MASK;
            complete_request(0u);
            g_comblk_state = COMBLK_IDLE;
        break;
    }
}

/*==============================================================================
 *
 */
static void handle_rx_okay_irq(void)
{
    uint16_t data16;
    uint16_t is_command;
    uint8_t data8;
    
    data16 = (uint16_t)COMBLK->DATA8;
    is_command = data16 & DATA8_COMMAND_MASK;
    data8 = (uint8_t)data16;
            
    switch(g_comblk_state)
    {
        /*----------------------------------------------------------------------
        * MSS_COMBLK_init() enables the RCV_OKAY interrupt for the COMBLK_IDLE
        * state to receive the asynchronous power-on-reset from the system
        * controller.
        */
        case COMBLK_IDLE:
            if(is_command)
            {
                if(data8 != POR_DIGEST_ERROR_OPCODE)
                {
                    uint8_t rxed_opcode;
                    rxed_opcode = data8;
                    process_sys_ctrl_command(rxed_opcode);
                }
                else
                {  
                    g_comblk_response_idx = 0;
                    g_comblk_p_response[g_comblk_response_idx] = data8;
                    g_comblk_response_idx++;
                    g_comblk_p_response[g_comblk_response_idx] = 0x00u;                
                    g_comblk_state = COMBLK_RX_RESPONSE;
                }
            }
        break;
       
        /*----------------------------------------------------------------------
         * The RCV_OKAY interrupt should only be enabled for states
         * COMBLK_WAIT_RESPONSE and COMBLK_RX_RESPONSE. 
         */
        case COMBLK_WAIT_RESPONSE:
            if(is_command)
            {
                uint8_t rxed_opcode;
                rxed_opcode = data8;
                if(rxed_opcode == g_comblk_cmd_opcode)
                {
                    g_comblk_response_idx = 0u;
                    g_comblk_p_response[g_comblk_response_idx] = rxed_opcode;
                    ++g_comblk_response_idx;
                    g_comblk_state = COMBLK_RX_RESPONSE;
                }
                else
                {
                    process_sys_ctrl_command(rxed_opcode);
                }
            }
        break;
            
        case COMBLK_RX_RESPONSE:
            if(is_command)
            {
                uint8_t rxed_opcode;
                rxed_opcode = data8;
                process_sys_ctrl_command(rxed_opcode);
            }
            else
            {
                if( g_comblk_p_response[g_comblk_response_idx-1] == POR_DIGEST_ERROR_OPCODE)
                {
                    g_comblk_p_response[g_comblk_response_idx] = data8;
                    process_sys_ctrl_command(g_comblk_p_response[g_comblk_response_idx-1]);
                    g_comblk_state = COMBLK_IDLE;
                }
                else
                {
                    if(g_comblk_response_idx < g_comblk_response_size)
                    {
                        uint8_t rxed_data;
                        
                        rxed_data = data8;
                        g_comblk_p_response[g_comblk_response_idx] = rxed_data;
                        ++g_comblk_response_idx;
                    }
                    
                    if(g_comblk_response_idx == g_comblk_response_size)
                    {
                        complete_request(g_comblk_response_idx);
                        g_comblk_state = COMBLK_IDLE;
                    }
                }
            }
        break;
            
        /*----------------------------------------------------------------------
         * The RCV_OKAY interrupt should NOT be enabled for states
         * COMBLK_IDLE, COMBLK_TX_CMD and COMBLK_TX_DATA.
         */
        case COMBLK_TX_PAGED_DATA:
            /* This is needed because when there is an error, we need to terminate loading the data */
            if(!is_command)
            {
                g_comblk_p_response[1] = data8;
                complete_request(2u);
                g_comblk_state = COMBLK_IDLE;
            }
            else
            {
                uint8_t rxed_opcode;
                rxed_opcode = data8;
                process_sys_ctrl_command(rxed_opcode);
            }
        break;
        
        case COMBLK_TX_CMD:
            /* Fall through */
        case COMBLK_TX_DATA:
            /* Fall through */
            if(is_command)
            {
                uint8_t rxed_opcode;
                rxed_opcode = data8;
                process_sys_ctrl_command(rxed_opcode);
            }
        break;
        
        default:
            complete_request(0u);
            g_comblk_state = COMBLK_IDLE;
        break;
    }
}

static void process_sys_ctrl_command(uint8_t cmd_opcode)
{
    if(g_async_event_handler != 0)
    {
        g_async_event_handler(cmd_opcode);
    }
}
