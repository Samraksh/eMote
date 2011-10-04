#include "radio.h"
#include <radio/netmf_radio.h>
#include <led/stm32f10x_led.h>
#include <exti/stm32f10x_exti.h>
#include <rs232/stm32_eval.h>
#include <lcd_basic\stm32f10x_lcd_basic.h>
#include <tinyhal.h>


#define DEBUG_ON

/** ----Radio Layer Initialization Functions----
	Implements the radio initialization function
**/

#ifdef DEBUG_ON
	BOOL status = FALSE;	
#endif

BOOL radio_hal_init()
{	
#ifdef DEBUG_ON	
	debug_printf("Radio Driver\n");	
#endif
	UINT8 irqs = 0;	
	radio_hal_trx_status_t trx_status;

	//Initialize uC components, at this point the assumption is mf initializes
	//the clock, gpio, spi and timers for us
	
	if(mfhal_init() != TRUE)
	{
		return FALSE;
	}
	
	if(radio_init() != TRUE)
	{
		return FALSE;
	}	

#ifdef DEBUG_ON
	STM_EVAL_LEDInit((Led_TypeDef)0); 
	STM_EVAL_LEDInit((Led_TypeDef)1); 
	STM_EVAL_LEDInit((Led_TypeDef)2); 
	STM_EVAL_LEDInit((Led_TypeDef)3); 
	if(!test_radio())
	{
		LED_RED();
	}
#endif

	if(internal_radio_reset(FALSE, FALSE) != TRUE)
	{
		return FALSE;
	}

   configure_exti();   /* Enable transceiver main interrupt. */	
   irqs = radio_reg_read(RG_IRQ_STATUS); //Dummy read	
   //TODO: Set the user defined handler here
   //TODO: Enable the handler

   return TRUE;

}

BOOL radio_init()
{
	
	radio_hal_trx_status_t trx_status;
    UINT8 poll_counter = 0;
	UINT8 state = 0;

	rst(FALSE); //Goes high on true and low on false
	slp_tr(FALSE); //Goes high on true and low on false

	timer_delay(P_ON_TO_CLKM_AVAILABLE); //Wait for 6uS
	
	// apply reset pulse
	rst(FALSE); 
    timer_delay(RST_PULSE_WIDTH_US);
    rst(TRUE); 	

	do
    {
        if (poll_counter == 0xFF)
        {
            return FALSE;
        }
        poll_counter++;
        /* Check if AT86RF230 is connected; omit manufacturer id check */
    } while ((radio_reg_read(RG_VERSION_NUM) != AT86RF230_REV_B) ||
             (radio_reg_read(RG_PART_NUM) != AT86RF230));

	radio_reg_write(RG_TRX_STATE, CMD_TRX_OFF);

	/* verify that trx has reached TRX_OFF */
    
    do
    {
        trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);		
		
        if (poll_counter == 0xFF)
        {
            return FALSE;
        }
        poll_counter++;
    } while (trx_status != TRX_OFF);

    radio_hal_trx_status = TRX_OFF; //Global state variable

    return TRUE;
}

BOOL internal_radio_reset(BOOL default_values, BOOL rand_seed)
{
	if (radio_reset() != TRUE)
    {
		return FALSE;
    }

	//Configures Radio after reset except CSMA seed
	radio_config(); 

	if (rand_seed)
    {
        /*
         * Generate a seed for the random number generator in function rand().
         * This is required (for example) as seed for the CSMA-CA algorithm.
         */
		//TODO: Generate random seed for CSMA
        //generate_rand_seed();
    }

	//TODO: Implement CSMA
	//radio_config_csma(); 

	if (default_values)
    {
        /* Set the default PIB values */
        init_radio_default();
    }
    else
    {
		//TODO: Set it up to user defined values
		init_radio();
        /* nothing to do - the current TAL PIB attribute values are used */
    }

	write_all_default_to_radio();

	//Reset Radio variables //Check for tal_internal.h
	//Set CSMA State        //Check for tal_slotted_csma.h

}

BOOL radio_reset()
{	

    radio_hal_trx_status_t trx_status;
    UINT8 poll_counter = 0;

    /* trx might sleep, so wake it up */
    slp_tr(FALSE); //PAL_SLP_TR_LOW();
    timer_delay(SLEEP_TO_TRX_OFF_US);

    rst(FALSE); //PAL_RST_LOW();
    timer_delay(RST_PULSE_WIDTH_US);
    rst(TRUE); //PAL_RST_HIGH();

    /* verify that trx has reached TRX_OFF */
    do
    {
        trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);
        poll_counter++;
        if (poll_counter > 250)
        {
            //LED Alert
            return FALSE;
        }

    } while (trx_status != TRX_OFF);

    radio_hal_trx_status = TRX_OFF; //Global state variable

	return TRUE;
}

BOOL radio_deinit()
{
	BOOL return_value = TRUE;
	
	return return_value;
}


/** ----Radio Sleep and Wake up Functions----
	Implements the function for sleep and wake up for the 
	radio
**/


BOOL radio_sleep(sleep_mode_t sleep_mode)
{
	radio_hal_trx_status_t trx_status;
	return TRUE;    
}

void radio_wakeup()
{
	//TODO: Implement functionality
}

/** ----Radio State Machine Configuration Functions----
	Implements the function for setting and getting the state of the
	radio
**/

BOOL set_radio_state(radio_cmd_t trx_cmd)
{
	BOOL return_value = TRUE;
	
	radio_hal_trx_status_t tal_trx_status;
	
	if (tal_trx_status == TRX_SLEEP)
    {
        UINT8 bit_status;

        //PAL_SLP_TR_LOW();
		slp_tr(false);
		
        do
        {
            bit_status = radio_bit_read(SR_TRX_STATUS);
        } while (bit_status != TRX_OFF);

        if ((trx_cmd == CMD_TRX_OFF) || (trx_cmd == CMD_FORCE_TRX_OFF))
        {
            tal_trx_status = TRX_OFF;
            return TRX_OFF;
        }
    }
	
	tal_trx_status = (radio_hal_trx_status_t) radio_bit_read (SR_TRX_STATUS);
	
	switch (trx_cmd)    /* new state */
    {
        case CMD_TRX_SLEEP:
            radio_reg_write(RG_TRX_STATE, CMD_FORCE_TRX_OFF);

            {
                uint16_t rand_value;

                /*
                 * TODO: Init the SEED value of the CSMA backoff algorithm.
                 */
                //rand_value = (uint16_t)rand();
                //pal_trx_reg_write(RG_CSMA_SEED_0, (uint8_t)rand_value);
                //pal_trx_bit_write(SR_CSMA_SEED_1, (uint8_t)(rand_value >> 8));
            }

            //PAL_WAIT_1_US();
            //PAL_SLP_TR_HIGH();
			slp_tr(true);
            timer_delay(TRX_OFF_TO_SLEEP_TIME);
            tal_trx_status = TRX_SLEEP;
            return TRX_SLEEP;   /* transceiver register cannot be read during TRX_SLEEP */
            //break; // do not use break, since it is unreachable

        case CMD_TRX_OFF:
            switch (tal_trx_status)
            {
                case TRX_OFF:
                    break;

                default:
                    radio_reg_write(RG_TRX_STATE, CMD_TRX_OFF);
					//FIXME: 
                    //TODO: PAL_WAIT_1_US();
                    break;
            }
            break;

        case CMD_FORCE_TRX_OFF:
            switch (tal_trx_status)
            {
                case TRX_OFF:
                    break;

                case TX_ARET_ON:
                    radio_reg_write(RG_TRX_STATE, CMD_FORCE_TRX_OFF);
                    //PAL_SLP_TR_HIGH();
					slp_tr(true);
                    //pal_timer_delay(2);
					//FIXME: Check this function
					timer_delay(2);
                    //PAL_SLP_TR_LOW();
					slp_tr(false);
                    break;

                default:
                    radio_reg_write(RG_TRX_STATE, CMD_FORCE_TRX_OFF);
                    //PAL_WAIT_1_US();
                    break;
            }
            break;

        case CMD_PLL_ON:
            switch (tal_trx_status)
            {
                case PLL_ON:
                    break;

                case TRX_OFF:
					//FIXME: Implement
                    //switch_pll_on();
                    break;

                case RX_ON:
                case RX_AACK_ON:
                case TX_ARET_ON:
                    radio_reg_write(RG_TRX_STATE, CMD_PLL_ON);
                    //PAL_WAIT_1_US();
                    break;

                case BUSY_RX:
                case BUSY_TX:
                case BUSY_RX_AACK:
                case BUSY_TX_ARET:
                    /* do nothing if trx is busy */
                    break;

                default:
                    ASSERT("state transition not handled" == 0);
                    break;
            }
            break;

        case CMD_FORCE_PLL_ON:  /* software state */
            switch (tal_trx_status)
            {
                case RX_ON:
                case BUSY_RX:
                case RX_AACK_ON:
                case BUSY_RX_AACK:
					//FIXME: Imeplement
                    //ENTER_TRX_REGION();
                    radio_reg_write(RG_TRX_STATE, CMD_FORCE_TRX_OFF);
                    radio_reg_write(RG_TRX_STATE, CMD_PLL_ON);
                    // clear trx irq in case a frame is received meanwhile
                    radio_reg_read(RG_IRQ_STATUS);
					//FIXME: Imeplement
                    //pal_trx_irq_flag_clr();
					////FIXME: Imeplement
                    //LEAVE_TRX_REGION();					
					//FIXME: Imeplement
                    //PAL_WAIT_1_US();
                    break;

                case BUSY_TX:
					//FIXME: Imeplement
                    //ENTER_TRX_REGION();
                    radio_reg_write(RG_TRX_STATE, CMD_FORCE_TRX_OFF);
                    radio_reg_write(RG_TRX_STATE, CMD_PLL_ON);
					//FIXME: Imeplement
                    //LEAVE_TRX_REGION();
                    //PAL_WAIT_1_US();
                    break;

                case BUSY_TX_ARET:				
					//FIXME: Imeplement	
                    //ENTER_TRX_REGION();
                    radio_reg_write(RG_TRX_STATE, CMD_FORCE_TRX_OFF);
                    //PAL_SLP_TR_LOW();
					slp_tr(false);
                    timer_delay(SLP_TR_TOGGLE_US);
                    //PAL_SLP_TR_HIGH();
					slp_tr(true);
                    radio_reg_write(RG_TRX_STATE, CMD_PLL_ON);
					//FIXME: Imeplement
                    //LEAVE_TRX_REGION();
                    //PAL_WAIT_1_US();
                    break;

                case TRX_OFF:
					//FIXME: Imeplement
                    //switch_pll_on();
                    break;

                case PLL_ON:
                    break;

                default:
                    ASSERT("state transition not handled" == 0);
                    break;
            }
            break;

        case CMD_RX_ON:
            switch (tal_trx_status)
            {
                case RX_ON:
                    break;

                case PLL_ON:
                    radio_reg_write(RG_TRX_STATE, CMD_RX_ON);
					//FIXME: Imeplement
                    //PAL_WAIT_1_US();
                    break;

                case TRX_OFF:
					//FIXME: Imeplement
                    //switch_pll_on();
                    radio_reg_write(RG_TRX_STATE, CMD_RX_ON);
                    //PAL_WAIT_1_US();
                    break;

                case RX_AACK_ON:
                case TX_ARET_ON:
                    radio_reg_write(RG_TRX_STATE, CMD_PLL_ON);
                    //PAL_WAIT_1_US();
                    // check if state change could be applied
                    tal_trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);
                    if (tal_trx_status != PLL_ON)
                    {
                        return tal_trx_status;
                    }
                    radio_reg_write(RG_TRX_STATE, CMD_RX_ON);
                    //PAL_WAIT_1_US();
                    break;

                case BUSY_RX:
                case BUSY_TX:
                case BUSY_RX_AACK:
                case BUSY_TX_ARET:
                    /* do nothing if trx is busy */
                    break;

                default:
                    ASSERT("state transition not handled" == 0);
                    break;
            }
            break;

        case CMD_RX_AACK_ON:
            switch (tal_trx_status)
            {
                case RX_AACK_ON:
                    break;

                case PLL_ON:
                    radio_reg_write(RG_TRX_STATE, CMD_RX_AACK_ON);
                    //PAL_WAIT_1_US();
                    break;

                case TRX_OFF:
                    //switch_pll_on();// state change from TRX_OFF to RX_AACK_ON can be done directly, too
                    radio_reg_write(RG_TRX_STATE, CMD_RX_AACK_ON);
                    //PAL_WAIT_1_US();
                    break;

                case TX_ARET_ON:
                case RX_ON:
                    radio_reg_write(RG_TRX_STATE, CMD_PLL_ON);
                    //PAL_WAIT_1_US();
                    // check if state change could be applied
                    tal_trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);
                    if (tal_trx_status != PLL_ON)
                    {
                        return tal_trx_status;
                    }
                    radio_reg_write(RG_TRX_STATE, CMD_RX_AACK_ON);
                    //PAL_WAIT_1_US();
                    break;

                case BUSY_RX:
                case BUSY_TX:
                case BUSY_RX_AACK:
                case BUSY_TX_ARET:
                    /* do nothing if trx is busy */
                    break;

                default:
                    ASSERT("state transition not handled" == 0);
                    break;
            }
            break;

        case CMD_TX_ARET_ON:
            switch (tal_trx_status)
            {
                case TX_ARET_ON:
                    break;

                case PLL_ON:
                    radio_reg_write(RG_TRX_STATE, CMD_TX_ARET_ON);
                    //PAL_WAIT_1_US();
                    break;

                case RX_ON:
                case RX_AACK_ON:
                    radio_reg_write(RG_TRX_STATE, CMD_PLL_ON);
                    //PAL_WAIT_1_US();
                    // check if state change could be applied
                    tal_trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);
                    if (tal_trx_status != PLL_ON)
                    {
                        return tal_trx_status;
                    }
                    radio_reg_write(RG_TRX_STATE, CMD_TX_ARET_ON);
                    //PAL_WAIT_1_US();
                    break;

                case TRX_OFF:
                    //switch_pll_on();// state change from TRX_OFF to TX_ARET_ON can be done directly, too
                    radio_reg_write(RG_TRX_STATE, CMD_TX_ARET_ON);
                    //PAL_WAIT_1_US();
                    break;

                case BUSY_RX:
                case BUSY_TX:
                case BUSY_RX_AACK:
                case BUSY_TX_ARET:
                case STATE_TRANSITION_IN_PROGRESS:
                    /* do nothing if trx is busy */
                    break;

                default:
                    ASSERT("state transition not handled" == 0);
                    break;
            }
            break;

        default:
            /* CMD_NOP, CMD_TX_START */
            ASSERT("trx command not handled" == 0);
            break;
    }

    do
    {
        tal_trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);
    } while (tal_trx_status == STATE_TRANSITION_IN_PROGRESS);

    return tal_trx_status;
	
	return return_value;    
}

radio_hal_trx_status_t get_radio_state()
{
	radio_hal_trx_status_t state = P_ON;

	return state;	
}


/** -----------------------------------Radio Configuration Functions--------------------------------- **/

BOOL radio_config()
{
	/* Set pin driver strength */
    radio_reg_write(RG_TRX_CTRL_0, ((CLKM_2mA << 6) |
                      (CLKM_2mA << 4) | CLKM_1MHz)); // fast change

    radio_bit_write(SR_AACK_SET_PD, PD_ACK_BIT_SET_ENABLE); /* 1 == frame pending bit is always set to 1 */
    radio_bit_write(SR_TX_AUTO_CRC_ON, TX_AUTO_CRC_ENABLE);    /* enable auto crc */
	/* enable interrupts */
	radio_reg_write(RG_IRQ_MASK, TRX_IRQ_PLL_LOCK | TRX_IRQ_PLL_UNLOCK | TRX_IRQ_RX_START |  TRX_IRQ_TRX_END | TRX_IRQ_TRX_UR | TRX_IRQ_BAT_LOW);
    //radio_reg_write(RG_IRQ_MASK, TRX_IRQ_TRX_END | TRX_IRQ_PLL_LOCK); /* enable TRX_END interrupt */
	return TRUE;
}

void radio_config_csma()
{
	UINT16 rand_value;

    /*
     * Init the SEED value of the CSMA backoff algorithm.
     */
	//TODO - Generate random seed
    //rand_value = (UINT16)rand(); 
    radio_reg_write(RG_CSMA_SEED_0, (UINT8)rand_value);
    radio_bit_write(SR_CSMA_SEED_1, (UINT8)(rand_value >> 8));

    radio_sleep(SLEEP_MODE_1); //TODO:Implement this function

    radio_wakeup(); //TODO: Implement this function
	//Configure radio csma
}

void generate_rand_seed()
{
	UINT16 seed;
    
    seed = 0; //rand_seed(); from uC

    /* Set the seed for the random number generator. */
    //srand(seed); //TODO - fix me..

	//return seed; //TODO _ fix me.. 
	
}

/** -----------------------------------Interrupt Initialization Functions--------------------------------- **/

void radio_irq_init(void (*irq)())
{
	//Send the pointer to the radio_irq_handler to the EXTI	
	irq_handler = &radio_irq_handler;
	radio_reg_read(RG_IRQ_STATUS);	
}

void radio_irq_handler()
{	
	
	radio_irq_reason_t irq_cause;

	irq_cause = (radio_irq_reason_t)radio_reg_read(RG_IRQ_STATUS);

	if(irq_cause & TRX_IRQ_RX_START)
	{		
		//If Tx, check for under-run and call the send_end function
		//handle_sending_frame();	
		//debug_printf("In TRX_START ISR");
#ifdef DEBUG_ON
		CPU_GPIO_EnableOutputPin(2, true);
		//STM_EVAL_LEDToggle((Led_TypeDef)0);		
		//LED_GREEN();
#endif		
		return;
	}	

	//Handle end of Tx or Rx
	if(irq_cause & TRX_IRQ_TRX_END)
	{		
		//If Tx, check for under-run and call the send_end function
		//handle_sending_frame();		   
		CPU_GPIO_EnableOutputPin(2, false);		
		/*if (radio_bit_read(SR_RX_CRC_VALID) == CRC16_NOT_VALID)
        {
            LED_RED();
        }		*/
		STM_EVAL_LEDToggle((Led_TypeDef)1);		
		//STM_EVAL_LEDToggle((Led_TypeDef)0);
		//LED_ORANGE();
		handle_recieved_frame();
		return;
	}	
	
//	else
//	{
//		//Call handle_receive
//#ifdef DEBUG_ON
//		debug_printf("In TRX_END ISR");
//		STM_EVAL_LEDToggle((Led_TypeDef)1);		
//
//		if (radio_bit_read(SR_RX_CRC_VALID) == CRC16_NOT_VALID)
//        {
//            LED_RED();
//        }	
//#endif
//		debug_printf("Calling handle_received_frame");
//		handle_recieved_frame();
//	}
	
	//if (irq_cause != TRX_IRQ_TRX_END)
 //   {
 //       if (irq_cause & TRX_IRQ_PLL_LOCK)
 //       {
	//		ASSERT("unexpected IRQ: TRX_IRQ_PLL_LOCK" == 0);
	//		//Call the user specified call back 
 //       }
 //       if (irq_cause & TRX_IRQ_PLL_UNLOCK)
 //       {
 //           ASSERT("unexpected IRQ: TRX_IRQ_PLL_UNLOCK" == 0);
	//		//Call the user specified call back 
 //       }
 //       if (irq_cause & TRX_IRQ_RX_START)
 //       {
 //           ASSERT("unexpected IRQ: TRX_IRQ_RX_START" == 0);
	//		//Call the user specified call back 
 //       }
 //       if (irq_cause & TRX_IRQ_4)
 //       {
 //           ASSERT("unexpected IRQ: TRX_IRQ_4" == 0);
	//		//Call the user specified call back 
 //       }
 //       if (irq_cause & TRX_IRQ_5)
 //       {
 //           //ASSERT("unexpected IRQ: TRX_IRQ_5" == 0);
	//		//Call the user specified call back 
 //       }
 //       if (irq_cause & TRX_IRQ_TRX_UR)
 //       {
 //           ASSERT("unexpected IRQ: TRX_IRQ_TRX_UR" == 0);
	//		//Call the user specified call back 
 //       }
 //       if (irq_cause & TRX_IRQ_BAT_LOW)
 //       {
 //           ASSERT("unexpected IRQ: TRX_IRQ_BAT_LOW" == 0);
	//		//Call the user specified call back 
 //       }
 //   }	
}


/** -----------------------------------Radio Default Initialization Functions--------------------------------- **/

//Set the radio attributes to user defined values
void init_radio()
{
  //TODO: Implement functionality
}

void init_radio_default()
{
	//Initialize radio to default values
	radio_hal_MaxCSMABackoffs = MAX_CSMA_BACKOFFS_DEFAULT;
    radio_hal_MinBE = MINBE_DEFAULT;
    radio_hal_PANId = PANID_BC_DEFAULT;
    radio_hal_ShortAddress = SHORT_ADDRESS_DEFAULT;
    radio_hal_CurrentChannel = CURRENT_CHANNEL_DEFAULT;
    radio_hal_CurrentPage = CURRENT_PAGE_DEFAULT;
    radio_hal_MaxBE = MAXBE_DEFAULT;
    radio_hal_MaxFrameRetries = MAXFRAMERETRIES_DEFAULT;
	radio_hal_PrivatePanCoordinator = PAN_COORDINATOR_DEFAULT;	
	//TODO - Implement default power function
    //radio_hal_pib_TransmitPower = limit_tx_pwr(TAL_TRANSMIT_POWER_DEFAULT); 	
	//TODO - Implement the Clear channel assesment 
    //radio_hal_pib_CCAMode = TAL_CCA_MODE_DEFAULT;     
}


void write_all_default_to_radio()
{
	//Push the values to the radio register
	UINT8* ptr_to_reg;

    /* configure RX_AACK */
    radio_reg_write(RG_PAN_ID_0, (UINT8)radio_hal_PANId);
	radio_reg_write(RG_PAN_ID_1, (UINT8)(radio_hal_PANId >> 8));

    ptr_to_reg = (UINT8 *)&radio_hal_IeeeAddress;
    for (UINT8 i = 0; i < 8; i++)
    {
        radio_reg_write((RG_IEEE_ADDR_0 + i), *ptr_to_reg);
        ptr_to_reg++;
    }

    radio_reg_write(RG_SHORT_ADDR_0, (UINT8)radio_hal_ShortAddress);
    radio_reg_write(RG_SHORT_ADDR_1, (UINT8)(radio_hal_ShortAddress >> 8));

    /* configure TX_ARET; CSMA and CCA */
    {
        UINT8 reg_value;
		
		//FIXME
        //reg_value = convert_phyTransmitPower_to_reg_value(radio_hal_pib_TransmitPower); //TODO fix me.. 
        radio_bit_write(SR_TX_PWR, reg_value);
    }
    radio_bit_write(SR_CCA_MODE, radio_hal_CCAMode);
    radio_bit_write(SR_CHANNEL, radio_hal_CurrentChannel);
    radio_bit_write(SR_MIN_BE, radio_hal_MinBE);

}


/** ----Radio Send Functions----
	Functions to send frames
**/

void handle_sending_frame(bool underrun_occured)
{
	//TODO: Implement
}

void send_frame(UINT8* frame, csma_mode_t csma_mode, bool retry)
{
	//TODO: Imeplement
}

/** ----Radio Receive Functions----
	Functions to send frames
**/

void handle_recieved_frame()
{
	/*TODO: 1) Add time stamping support
			2) Add Beaconing network support
			3) Handle promiscous mode
	*/

	UINT8 ed_value;
	//Actual length of the frame
	UINT8 phy_frame_len;
	//Extended frame length appended by LQI and ED
	UINT8 ext_frame_len;
	//Define a pointer here of the type frame_info
	UINT8* frame_ptr;

	/* Get ED value; needed to normalize LQI. */
    ed_value = radio_reg_read(RG_PHY_ED_LEVEL);

#ifdef DEBUG_ON
	//debug_printf("ED VALUE IS: %d\n", ed_value);
	//debug_printf("-----------------------");
#endif
	
	/* Get frame length from transceiver. */
	radio_frame_read(&phy_frame_len, LENGTH_FIELD_LEN);

#ifdef DEBUG_ON
	//debug_printf("-----------------------");
	//debug_printf("\nFRAME LENGTH IS: %d\n", phy_frame_len);
#endif
	
	/* Check for valid frame length. */
    if (phy_frame_len > 127)
    {
        return;
    }
	
	/*
     * The PHY header is included hence the frame length is incremented.
     * In addition to that, the LQI and ED value are uploaded, too.
     */
    ext_frame_len = phy_frame_len + LENGTH_FIELD_LEN + LQI_LEN + ED_VAL_LEN;
		
	//Read frame buffer
	radio_frame_read(frame_ptr, LENGTH_FIELD_LEN + phy_frame_len + LQI_LEN);
	
	//Check for a valid frame
	if (radio_bit_read(SR_RX_CRC_VALID) == CRC16_NOT_VALID)
    {
            return;
    }	
	
}

void process_recieved_frame()
{
	//TODO: Implement
}

/** ----Radio Read/Write Functions----
	Functions to read and write to and from radio
**/

void radio_reg_write(UINT8 address, UINT8 data)
{	

	//Accquire Global Lock	
	//spi_write_command(address, data);
	spi_write(address, data);
	//Release Global Lock
}

UINT8 radio_reg_read(UINT8 address)
{
	UINT8 register_value = 0;
	//Accquire Global Lock	
	//register_value = spi_read_command(address);
	register_value = spi_read(address);
	return register_value;
	//Release Global Lock		
}

void radio_frame_read(UINT8* data, UINT8 length)
{	
	//TODO: Implement
	//Naive implementation, change to buffered read later with dynamic memory.
	uint8_t data_received[130];	
	//UINT8 data_received[length];
	//UINT8* data_ptr = data_received;
	spi_frame_read(data_received, 130);

	uint8_t count = data_received[20];
	uint8_t* count_ptr = &count;

	LCD_DisplayStringLine(Line1,(uint8_t*)"RADIO_COUNT_SEND_DEMO");
	LCD_DisplayStringLine(Line2,count_ptr);
	for(int i=0; i <length; i++)
	{
	//	//debug_printf("%d ", data_received[i]);		
	//	LCD_DisplayStringLine(Line1,(uint8_t *)"RADIO_COUNTSEND_DEMO");		
	}
	//debug_printf("\n");
}

void radio_frame_write(UINT8* data, UINT8 length)
{	
	//TODO: Implement	
	spi_frame_write(data, length);
}

UINT8 radio_bit_read(UINT8 address, UINT8 mask, UINT8 pos)
{
    UINT8 ret;

    ret = radio_reg_read(address);
    ret &= mask;
    ret >>= pos;

    return ret;
}

void radio_bit_write(UINT8 reg_addr, UINT8 mask, UINT8 pos, UINT8 new_value)
{
    UINT8 current_reg_value;

    current_reg_value = radio_reg_read(reg_addr);
    current_reg_value &= (UINT8)~(UINT16)mask;  // Implicit casting required
    new_value <<= pos;
    new_value &= mask;
    new_value |= current_reg_value;

    radio_reg_write(reg_addr, new_value);
}

void radio_sram_write(UINT8 addr, UINT8 *data, UINT8 length)
{
	//Acquire Global lock
	
    //release global lock    
}

void radio_sram_read(UINT8 addr, UINT8 *data, UINT8 length)
{
    //WAIT_500_NS();
    //Acquire Global lock
	//Call to netmf_radio
	//release global lock    
}

/** -----------------------------------Radio Test Function--------------------------------- **/

BOOL test_radio()
{
	UINT8 address = 0;
	UINT8 state = 0;
	UINT8 channel = 0;	
	UINT8 poll_counter = 0;
	UINT8 counter_trxoff = 0;
	UINT8 counter_pllon = 0;
	UINT8 counter_rxon = 0;
	UINT8 irqs = 0;
	uint8_t count = 0;
	uint8_t* count_ptr = NULL;
	radio_hal_trx_status_t trx_status;

	configure_exti();   /* Enable transceiver main interrupt. */	
    
	irqs = radio_reg_read(RG_IRQ_STATUS); //Dummy read	

	//radio_reg_write(RG_IRQ_MASK, TRX_IRQ_PLL_LOCK | TRX_IRQ_PLL_UNLOCK | TRX_IRQ_RX_START |  TRX_IRQ_TRX_END | TRX_IRQ_TRX_UR | TRX_IRQ_BAT_LOW);
	//Notify me only about start and end
	radio_reg_write(RG_IRQ_MASK, TRX_IRQ_RX_START |  TRX_IRQ_TRX_END);
	
	radio_reg_write(RG_TRX_STATE, CMD_TRX_OFF);

	do
    {
        trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);		
		
        if (poll_counter == 0xFF)
        {
            return FALSE;
        }
        poll_counter++;
    } while (trx_status != TRX_OFF);

    radio_hal_trx_status = TRX_OFF; //Global state variable
		
	radio_reg_write(RG_TRX_STATE, CMD_PLL_ON);

	/* verify that trx has reached PLL_ON */        
	do
    {
        trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);		
		
        if (poll_counter == 0xFF)
        {
            return FALSE;
        }
        poll_counter++;
    } while (trx_status != PLL_ON);

    radio_hal_trx_status = PLL_ON; //Global state variable

	radio_reg_read(RG_IRQ_STATUS);	
	
	radio_bit_write(SR_CHANNEL, 12);

	timer_delay(150);

	radio_reg_read(RG_IRQ_STATUS);	

	channel = radio_bit_read(SR_CHANNEL);	

	//poll_counter = 0;

#if 0 

	radio_reg_write(RG_TRX_STATE, CMD_RX_AACK_ON);

    do
    {
        trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);		
	
       if (poll_counter == 0xFF)
       {
            return FALSE;
		    //ERROR!
       }
       poll_counter++;
   } while (trx_status != RX_AACK_ON);

   radio_reg_read(RG_IRQ_STATUS);	

   radio_hal_trx_status = RX_AACK_ON; //Global state variable

   radio_reg_write(RG_TRX_STATE, CMD_RX_ON);

    do
    {
        trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);		
	
       if (poll_counter == 0xFF)
       {
            return FALSE;
		    //ERROR!
       }
       poll_counter++;
   } while (trx_status != RX_ON);

   radio_reg_read(RG_IRQ_STATUS);	

   radio_hal_trx_status = RX_ON; //Global state variable


#endif

   //debug_printf("Radio initialized in RX_ON mode\n");

   radio_bit_write(SR_TX_AUTO_CRC_ON, TX_AUTO_CRC_ENABLE);

   UINT8 send_buffer[] = {
						  0,117,65,136,97,34,0,255,255,102,0,63,239,0,102,0,10,0,0,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,
						  0,11,0,12,0,13,0,14,0,15,0,16,0,17,0,18,0,19,0,20,0,21,0,22,0,23,0,24,0,25,0,26,0,27,0,28,0,
	                      29,0,30,0,31,0,32,0,33,0,34,0,35,0,36,0,37,0,38,0,39,0,40,0,41,0,42,0,43,0,44,0,45,0,46,0,47,
						  0,48,0,49, 0, 0
						 };

   LCD_DisplayStringLine(Line1, (uint8_t*)"RADIO_COUNT_SEND_DEMO");

 //  /*UINT8 send_buffer[] = {
	//						0, 5, 1, 2, 3, 4, 5
	//					 };*/


   	radio_frame_write(send_buffer, 119);
 //  //radio_frame_write(send_buffer, 6);

	radio_reg_write(RG_TRX_STATE, CMD_TX_START);

	for(int i = 0; i < 10000; i++);

	//	//do
	//	//{
	//	//   trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);		
	//	//
	//	//   if (poll_counter == 0xFF)
	//	//   {
	//	//	    debug_printf("First packet");
	//	//		return FALSE;
	//	//		//ERROR!
	//	//   }
	//	//   poll_counter++;
	//	//} while (trx_status != BUSY_TX);


	///*UINT8 send_buffer[] = {
	//						0, 117, 65, 136, 97, 34, 0, 255, 255, 102,
	//						0, 63, 239, 0, 102, 0, 10, 0, 0, 0, 1, 0, 
	//						2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0,
	//						9, 0, 10, 0, 11, 0, 12, 0, 13, 0, 14, 0, 15,
	//						0, 16, 0, 17, 0, 18, 0, 19, 0, 20, 0, 21, 0,
	//						22, 0, 23, 0, 24, 0, 25, 0, 26, 0, 27, 0, 28,
	//						0, 29, 0, 30, 0, 31, 0, 32, 0, 33, 0, 34, 0, 
	//						35, 0, 36, 0, 37, 0, 38, 0, 39, 0, 40, 0, 41,
	//						0, 42, 0, 43, 0, 44, 0, 45, 0, 46, 0, 47, 0, 							
	//						48, 0, 49, 112, 102
	//					};*/

      while(true)
	  {
		  
		for(int i = 0; i < 100000; i++);
		
		poll_counter = 0; 

		count = send_buffer[20]++;

		count_ptr = &count;

		LCD_DisplayStringLine(Line2, count_ptr);

	//	radio_frame_write(send_buffer, 119);
	//	//radio_frame_write(send_buffer, 6);

	//	radio_reg_write(RG_TRX_STATE, CMD_TX_START);

	//	for(int i = 0; i < 100000; i++);

	//	//do
	//	//{
	//	//	debug_printf("Second packet");

	//	//	trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);		
	//	//
	//	//    if (poll_counter == 0xFF)
	//	//    {
	//	//		debug_printf("ERROR!");
	//	// 		return FALSE;
	//	// 		//ERROR!
	//	//    }
	//	//    poll_counter++;
	//	//}  while (trx_status != BUSY_TX);
	//	
	  }
   
   while(1)
   {
	     
   }

}	