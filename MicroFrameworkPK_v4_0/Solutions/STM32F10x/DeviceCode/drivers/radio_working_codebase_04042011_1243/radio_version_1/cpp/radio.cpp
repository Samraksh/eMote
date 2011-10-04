#include "radio.h"
#include <led/stm32f10x_led.h>

bool Radio :: hal_init()
{
	//Check if timers, spi and gpio are initialized
	return 0;
}

bool Radio :: timer_delay(UINT16 delay)
{
	//Set a timer to delay for specified amount of time
	return true;
}

bool Radio :: trx_init()
{
	radio_status_t trx_status;
	UINT8 poll_counter = 0;

	RST_HIGH();
	SLP_TR_LOW();

	timer_delay(P_ON_TO_CLKM_AVAILABLE);

	RST_LOW();
	timer_delay(RST_PULSE_WIDTH_US);
	RST_HIGH();

    do
    {
        if (poll_counter == 0xFF)
        {
            return false;
        }
        poll_counter++;
        
    } while ((register_read(RG_VERSION_NUM) != AT86RF230_REV_B) ||
             (register_read(RG_PART_NUM) != AT86RF230)); //Also can read MANID, skipping but.

	register_write(RG_TRX_STATE, CMD_TRX_OFF);

	poll_counter = 0;
    
	do
    {
        trx_status = (tal_trx_status_t)pal_trx_bit_read(SR_TRX_STATUS);
        if (poll_counter == 0xFF)
        {
            return false;
        }
        poll_counter++;
    } while (trx_status != TRX_OFF);

    trx_status = TRX_OFF;

    return true;
	
}

bool Radio :: internal_reset()
{
	return true;
}

radio_retval_t Radio :: radio_initialize()
{
	LED_RED();

	if(!hal_init())
	{
		return R_FAIL;
	}



	return R_SUCCESS;

}