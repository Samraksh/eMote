#include "radio.h"
#include <netmf_radio.h>
#include <led/stm32f10x_led.h>
#include <exti/stm32f10x_exti.h>


//Initialize the radio layer
BOOL radio_hal_init()
{
	

	UINT8 address = 0;
	UINT8 state = 0;
	UINT8 channel = 0;
	UINT8 irqs = 0;	
	UINT8 poll_counter = 0;
	UINT8 counter_trxoff = 0;
	UINT8 counter_pllon = 0;
	UINT8 counter_rxon = 0;
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

	configure_exti();   /* Enable transceiver main interrupt. */	
    
	irqs = radio_reg_read(RG_IRQ_STATUS); //Dummy read
	
	radio_reg_write(RG_IRQ_MASK, TRX_IRQ_PLL_LOCK | TRX_IRQ_PLL_UNLOCK | TRX_IRQ_RX_START |  TRX_IRQ_TRX_END | TRX_IRQ_TRX_UR | TRX_IRQ_BAT_LOW); //Set interrupts
	//radio_reg_write(RG_IRQ_MASK, TRX_IRQ_RX_START |  TRX_IRQ_TRX_END); //Set interrupts

	radio_reg_write(RG_TRX_STATE, CMD_TRX_OFF);
	
	do 
	{
		state = radio_bit_read(SR_TRX_STATUS);
		++counter_trxoff;
	}while (state != TRX_OFF);

	radio_reg_write(RG_TRX_STATE, CMD_PLL_ON);
	
	do 
	{
		state = radio_bit_read(SR_TRX_STATUS);
		++counter_pllon;
	}while (state != PLL_ON);

	radio_reg_read(RG_IRQ_STATUS); //PLL INT read!
	
	state = radio_bit_read(SR_TRX_STATUS);

	radio_bit_write(SR_CHANNEL, 12);
	timer_delay(150);
	radio_reg_read(RG_IRQ_STATUS);	
	channel = radio_bit_read(SR_CHANNEL);	

	radio_reg_read(RG_IRQ_STATUS);	

	radio_reg_write(RG_TRX_STATE, CMD_RX_ON);	

	do 
	{
		state = radio_bit_read(SR_TRX_STATUS);
		++counter_rxon;
	}while (state != RX_ON);

	radio_reg_read(RG_IRQ_STATUS);	
	
	state = radio_bit_read(SR_TRX_STATUS);

	//do
	//{
	//	state = radio_bit_read(SR_TRX_STATUS);
	//}while (state != TRX_OFF);

	//if(radio_bit_read(SR_TRX_STATUS) == TRX_OFF)
	//{
	//	//LED_BLUE_OFF();
	//	LED_RED();
	//}

	//radio_reg_write(RG_TRX_STATE, CMD_PLL_ON);

	///* verify that trx has reached PLL_ON */    
 //   do
 //   {
 //       trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);		
	//	
 //       if (poll_counter == 0xFF)
 //       {
 //           return FALSE;
 //       }
 //       poll_counter++;
 //   } while (trx_status != PLL_ON);

 //   radio_hal_trx_status = PLL_ON; //Global state variable

	//radio_reg_read(RG_IRQ_STATUS);	
	//
	//radio_bit_write(SR_CHANNEL, 12);

	//timer_delay(150);

	//radio_reg_read(RG_IRQ_STATUS);	

	//channel = radio_bit_read(SR_CHANNEL);	

	//poll_counter = 0;

	//radio_reg_write(RG_TRX_STATE, CMD_RX_ON);

 //   do
 //   {
 //       trx_status = (radio_hal_trx_status_t)radio_bit_read(SR_TRX_STATUS);		
	//	
 //       if (poll_counter == 0xFF)
 //       {
 //           return FALSE;
 //       }
 //       poll_counter++;
 //   } while (trx_status != RX_ON);

	//radio_hal_trx_status = RX_ON; //Global state variable

	//while(true)
	//{
	//}

     /*
     * Configure interrupt handling.
     * Install a handler for the transceiver interrupt.
     */
	
	//radio_irq_init(&radio_irq_handler);   //Set up the user defined call back
	   

	//To check if values are set right
	//address = radio_reg_read(RG_SHORT_ADDR_0);

	//To verify interrupts
	//radio_bit_write(SR_CHANNEL, radio_hal_pib_CurrentChannel); 



	return TRUE;

}

void radio_irq_init(void (*irq_handler)())
{
	//Send the pointer to the radio_irq_handler to the EXTI	
	irq_handler = radio_irq_handler;
}

void radio_irq_handler()
{
	LED_ORANGE();
}

BOOL internal_radio_reset(BOOL default_values, BOOL rand_seed)
{
	if (radio_reset() != TRUE)
    {
		return FALSE;
    }

	radio_config(); //Configure Radio except CSMA seed

	if (rand_seed)
    {
        /*
         * Generate a seed for the random number generator in function rand().
         * This is required (for example) as seed for the CSMA-CA algorithm.
         */
        generate_rand_seed();
    }

	//radio_config_csma(); //TODO: Implement this function

	if (default_values)
    {
        /* Set the default PIB values */
        init_radio_default();
    }
    else
    {
        /* nothing to do - the current TAL PIB attribute values are used */
    }

	write_all_default_to_radio();

	//Reset Radio variables //Check for tal_internal.h
	//Set CSMA State        //Check for tal_slotted_csma.h

}

BOOL radio_reset()
{
	//Check for EEPROM stuff

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
#if (DEBUG > 0)
            //LED Alert
#endif
            return FALSE;
        }

    } while (trx_status != TRX_OFF);

    radio_hal_trx_status = TRX_OFF; //Global state variable

	return TRUE;
}

BOOL radio_config()
{
	/* Set pin driver strength */
    radio_reg_write(RG_TRX_CTRL_0, ((CLKM_2mA << 6) |
                      (CLKM_2mA << 4) | CLKM_1MHz)); // fast change

    radio_bit_write(SR_AACK_SET_PD, PD_ACK_BIT_SET_ENABLE); /* 1 == frame pending bit is always set to 1 */
    radio_bit_write(SR_TX_AUTO_CRC_ON, TX_AUTO_CRC_ENABLE);    /* enable auto crc */
    radio_reg_write(RG_IRQ_MASK, TRX_IRQ_TRX_END | TRX_IRQ_PLL_LOCK); /* enable TRX_END interrupt */
	return TRUE;
}

void generate_rand_seed()
{
	UINT16 seed;
    
    seed = 0; //rand_seed(); from uC

    /* Set the seed for the random number generator. */
    //srand(seed); //TODO - fix me..

	//return seed; //TODO _ fix me.. 
	
}

void radio_config_csma()
{
	UINT16 rand_value;

    /*
     * Init the SEED value of the CSMA backoff algorithm.
     */
    //rand_value = (UINT16)rand(); //TODO - fix this.. 
    radio_reg_write(RG_CSMA_SEED_0, (UINT8)rand_value);
    radio_bit_write(SR_CSMA_SEED_1, (UINT8)(rand_value >> 8));

    radio_sleep(SLEEP_MODE_1); //TODO:Implement this function

    radio_wakeup(); //TODO: Implement this function
	//Configure radio csma
}

BOOL radio_sleep(sleep_mode_t sleep_mode)
{
	radio_hal_trx_status_t trx_status;

	return TRUE;    

}

void radio_wakeup()
{

}

void init_radio_default()
{
	//Initialize radio to default values
	radio_hal_pib_MaxCSMABackoffs = TAL_MAX_CSMA_BACKOFFS_DEFAULT;
    radio_hal_pib_MinBE = TAL_MINBE_DEFAULT;
    radio_hal_pib_PANId = TAL_PANID_BC_DEFAULT;
    radio_hal_pib_ShortAddress = TAL_SHORT_ADDRESS_DEFAULT;
    radio_hal_pib_CurrentChannel = TAL_CURRENT_CHANNEL_DEFAULT;
    radio_hal_pib_CurrentPage = TAL_CURRENT_PAGE_DEFAULT;
    radio_hal_pib_MaxBE = TAL_MAXBE_DEFAULT;
    radio_hal_pib_MaxFrameRetries = TAL_MAXFRAMERETRIES_DEFAULT;
    //radio_hal_pib_TransmitPower = limit_tx_pwr(TAL_TRANSMIT_POWER_DEFAULT); //TODO - fix me
    //radio_hal_pib_CCAMode = TAL_CCA_MODE_DEFAULT; //TODO - fix me.. 
    radio_hal_pib_PrivatePanCoordinator = TAL_PAN_COORDINATOR_DEFAULT;
}

void write_all_default_to_radio()
{
	//Push the values to the radio register
	UINT8* ptr_to_reg;

    /* configure RX_AACK */
    radio_reg_write(RG_PAN_ID_0, (UINT8)radio_hal_pib_PANId);
	radio_reg_write(RG_PAN_ID_1, (UINT8)(radio_hal_pib_PANId >> 8));

    ptr_to_reg = (UINT8 *)&radio_hal_pib_IeeeAddress;
    for (UINT8 i = 0; i < 8; i++)
    {
        radio_reg_write((RG_IEEE_ADDR_0 + i), *ptr_to_reg);
        ptr_to_reg++;
    }

    radio_reg_write(RG_SHORT_ADDR_0, (UINT8)radio_hal_pib_ShortAddress);
    radio_reg_write(RG_SHORT_ADDR_1, (UINT8)(radio_hal_pib_ShortAddress >> 8));

    /* configure TX_ARET; CSMA and CCA */
    {
        UINT8 reg_value;

        //reg_value = convert_phyTransmitPower_to_reg_value(radio_hal_pib_TransmitPower); //TODO fix me.. 
        radio_bit_write(SR_TX_PWR, reg_value);
    }
    radio_bit_write(SR_CCA_MODE, radio_hal_pib_CCAMode);
    radio_bit_write(SR_CHANNEL, radio_hal_pib_CurrentChannel);
    radio_bit_write(SR_MIN_BE, radio_hal_pib_MinBE);

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
	//rst(FALSE); 
    //timer_delay(RST_PULSE_WIDTH_US);
    rst(TRUE); 	

	
	//Normal run works till here - TODO
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

	
	//Crashes here! 

	radio_reg_write(RG_TRX_STATE, CMD_TRX_OFF);

	/* verify that trx has reached TRX_OFF */
    poll_counter = 0;
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

void radio_reg_write(UINT8 address, UINT8 data)
{	

	//Accquire Global Lock
	//Prepare the command byte
	//spi_cs_low();
	//Send the read command byte
	//spi_wait();
	//Send the read data byte to radio data register
	//spi_wait();
	//spi_cs_high();
	//Release Global Lock	
	spi_write(address, data);
}

UINT8 radio_reg_read(UINT8 address)
{
	UINT8 register_value = 0;
	//Accquire Global Lock
	//Prepare the command byte
	//spi_cs_low();
	//Send the Read command byte
	//spi_wait();
	//Do dummy read for initiating SPI read
	//spi_wait();
	//Read the byte received into register_value
	//spi_cs_high();
	//Release Global Lock		
	register_value = spi_read(address);
	return register_value;

}

void radio_frame_read(UINT8* data, UINT8 length)
{
	//Assumption: This function is called within ISR. 
	//spi_cs_low();
	//Send the command byte 
	//spi_wait();
	//call spi read in hal driver
	//spi_wait();
	//*data = data from spi register;
	//spi_cs_high();
}

void radio_frame_write(UINT8* data, UINT8 length)
{
	//Assumption: This function is called within ISR. 
	//spi_cs_low();
	//Send the command byte 
	//spi_wait();
	//call spi write in hal driver
	//spi_wait();
	//*data = data from spi register;
	//spi_cs_high();
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
    //spi_cs_low();
    //Send the command byte
    //spi_wait();
	//Send the address from which the write operation should start
    //spi_wait();
	//Upload the user data to transceiver data register
    //spi_cs_high();
	//release global lock    
}

void radio_sram_read(UINT8 addr, UINT8 *data, UINT8 length)
{
    //WAIT_500_NS();
    //Acquire Global lock
	//spi_cs_low();
    //Send the command byte
    //spi_wait();
    //Send the address from which the read operation should start
    //spi_wait();
	//Upload the received byte in the user provided location 
	//spi_cs_high();
	//release global lock    
}

