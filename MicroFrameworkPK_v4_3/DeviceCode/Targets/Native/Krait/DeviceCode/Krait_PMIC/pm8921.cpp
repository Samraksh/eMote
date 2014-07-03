/**
 * @file: pm8921.c
 * @author: Michael Andrew McGrath
 * @copyright: Translation: The Samraksh Company, Copyright (c) 2014
 * @date: April 2, 2014
 * @brief: functions in Little Kernel's pm8921 driver.
 * @details: combines design elements from the Little Kernel pm8921.c driver
 * and the Android pm8xxx-* drivers.
 */

/**
 * The design of the LK driver was based around sharing code, not state, so
 * individual clients could pass in a pm8921_dev_t and have it initialized
 * based on the desired SSBI config.  But the driver can technically only
 * be used by one other driver at a time.  Which is not a problem if you've
 * got a device tree or platform initialization that would own the driver's
 * config.  In NetMF, the Krait_POWER driver's initialization is the natural
 * place to initialize the extra PMIC driver, but the config should be owned
 * by this new driver.
 */

// most PMIC control registers already exist in Krait.h
#include "..\Krait.h"

#include "pm8921.h"
#include "pmic_defines.h"

pm8921_dev_t *dev;
pm8921_dev_t pmic;


void pm8921_init(pm8921_dev_t *pmic)
{
	ASSERT(pmic);
	ASSERT(pmic->read);
	ASSERT(pmic->write);

	dev = pmic;

	dev->initialized = 1;
}

int pa1_ssbi2_read_bytes(unsigned char  *buffer, unsigned short length,
                                                unsigned short slave_addr)
{
    unsigned val = 0x0;
    unsigned temp = 0x0000;
    unsigned char *buf = buffer;
    unsigned short len = length;
    unsigned short addr = slave_addr;
    unsigned long timeout = SSBI_TIMEOUT_US;

    while(len)
    {
        val |= ((addr << PA1_SSBI2_REG_ADDR_SHIFT) |
		(PA1_SSBI2_CMD_READ << PA1_SSBI2_CMD_RDWRN_SHIFT));
        writel(val, PA1_SSBI2_CMD);
        while(!((temp = readl(PA1_SSBI2_RD_STATUS)) & (1 << PA1_SSBI2_TRANS_DONE_SHIFT))) {
            if (--timeout == 0) {
	        hal_printf("In device ready function : Timeout");
	        return 1;
	    }
	}
        len--;
        *buf++ = (temp & (PA1_SSBI2_REG_DATA_MASK << PA1_SSBI2_REG_DATA_SHIFT));
    }
    return 0;
}

int pa1_ssbi2_write_bytes(unsigned char  *buffer, unsigned short length,
                                                unsigned short slave_addr)
{
    unsigned val;
    unsigned char *buf = buffer;
    unsigned short len = length;
    unsigned short addr = slave_addr;
    unsigned temp = 0x00;
    unsigned char written_data1 = 0x00;
    unsigned long timeout = SSBI_TIMEOUT_US;
    //unsigned char written_data2 = 0x00;

    while(len)
    {
        temp = 0x00;
        written_data1 = 0x00;
        val = (addr << PA1_SSBI2_REG_ADDR_SHIFT) |
	  (PA1_SSBI2_CMD_WRITE << PA1_SSBI2_CMD_RDWRN_SHIFT) |
 (*buf & 0xFF);
        writel(val, PA1_SSBI2_CMD);
        while(!((temp = readl(PA1_SSBI2_RD_STATUS)) & (1 << PA1_SSBI2_TRANS_DONE_SHIFT))) {
            if (--timeout == 0) {
	        hal_printf("In device write function : Timeout");
	        return 1;
	    }
	}
        len--;
        buf++;
    }
    return 0;
}


int pm8921_masked_write(uint16_t addr,
        uint8_t mask, uint8_t val)
{
    int rc;
    uint8_t reg;

    rc = dev->read(&reg, 1, addr);
    if (rc)
    {
        return rc;
    }

    reg &= ~mask;
    reg |= val & mask;
    rc = dev->write(&reg, 1, addr);

    return rc;
}



/*
 * Configure PMIC for reset and power off.
 * reset = 1: Configure reset.
 * reset = 0: Configure power off.
 */
int pm8921_config_reset_pwr_off(unsigned reset)
{
    int rc;

    /* Enable SMPL(Short Momentary Power Loss) if resetting is desired. */
    rc = pm8921_masked_write(PM8921_SLEEP_CTRL_REG,
            SLEEP_CTRL_SMPL_EN_MASK,
            (reset ? SLEEP_CTRL_SMPL_EN_RESET : SLEEP_CTRL_SMPL_EN_PWR_OFF));
    if (rc)
    {
        goto read_write_err;
    }

    /*
     * Select action to perform (reset or shutdown) when PS_HOLD goes low.
     * Also ensure that KPD, CBL0, and CBL1 pull ups are enabled and that
     * USB charging is enabled.
     */
    rc = pm8921_masked_write(PM8921_PON_CTRL_1_REG,
            PON_CTRL_1_PULL_UP_MASK | PON_CTRL_1_USB_PWR_EN
            | PON_CTRL_1_WD_EN_MASK,
            PON_CTRL_1_PULL_UP_MASK | PON_CTRL_1_USB_PWR_EN
            | (reset ? PON_CTRL_1_WD_EN_RESET : PON_CTRL_1_WD_EN_PWR_OFF));
    if (rc)
    {
        goto read_write_err;
    }

read_write_err:
    return rc;
}



/**
 * @brief: new. Drivers should call this to initialize.
 */
void PMIC_Initialize()
{
    if(dev == NULL || dev->initialized != 1)
    {
        pmic.read = (pm8921_read_func) & pa1_ssbi2_read_bytes;
        pmic.write = (pm8921_write_func) & pa1_ssbi2_write_bytes;
        pm8921_init(&pmic);
    }
}

