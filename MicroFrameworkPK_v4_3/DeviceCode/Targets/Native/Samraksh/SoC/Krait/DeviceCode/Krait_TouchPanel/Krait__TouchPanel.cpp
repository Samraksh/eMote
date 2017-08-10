/*
 * 	Name 	:  	Krait__TouchPanel.cpp
 *
 *  Author  :   Nived.Sivadas@Samraksh.com
 *
 *  Description : Contains the core logic that drives the touch panel
 */

#include "Krait__TouchPanel.h"


static int ft5x0x_read_reg(UINT8 addr, UINT8 *pdata)
{
        int ret;
        UINT8 buf[2];
        struct i2c_msg msgs[2];

    //
        buf[0] = addr;    //register address

        msgs[0].addr = this_client->addr;
        msgs[0].flags = 0;
        msgs[0].len = 1;
        msgs[0].buf = buf;
        msgs[1].addr = this_client->addr;
        msgs[1].flags = I2C_M_RD;
        msgs[1].len = 1;
        msgs[1].buf = buf;

        ret = Krait_I2C_Driver::i2c_transfer(this_client->adapter, msgs, 2);
        if (ret < 0)
                pr_err("msg %s i2c read error: %d\n", __func__, ret);

        *pdata = buf[0];
        return ret;

}


