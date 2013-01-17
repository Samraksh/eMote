#ifndef _KRAIT__TOUCHPANEL_H_
#define _KRAIT__TOUCHPANEL_H_

#include "..\Krait_INTC\Krait__INTC.h"
#include "..\Krait_I2C\Krait__I2C.h"
// Macros that connect the gpio lines to the touch panel

#define GPIO_FT5X06_IRQ  11
#define GPIO_FT5X06_RESET 46
#define GPIO_FT5X06_SDA 12
#define GPIO_FT5X06_SCL  13


static struct ft5x0x_ts_platform_data ft5x06_ts_data = {
                GPIO_FT5X06_IRQ,
                GPIO_FT5X06_RESET,
};


static struct i2c_board_info ft5x06_i2c_devices[] = {
		  "ft5x0x_ts",
		  0,
		  0x38,
          &ft5x06_ts_data,
          MSM_GPIO_TO_INT(GPIO_FT5X06_IRQ),

};

static struct i2c_registry msm8960_i2c_devices[] = {
		{
			 I2C_SURF | I2C_FFA | I2C_FLUID,
             MSM_8960_GSBI2_QUP_I2C_BUS_ID,
             ft5x06_i2c_devices,
             ARRAY_SIZE(ft5x06_i2c_devices),

		},
};



#endif
