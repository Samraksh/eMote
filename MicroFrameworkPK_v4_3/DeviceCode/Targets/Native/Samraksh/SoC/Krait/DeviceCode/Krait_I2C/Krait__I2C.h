#ifndef __KRAIT__I2C_H_
#define __KRAIT__I2C_H_

#include "Krait__I2C_TouchPanel.h"

#define I2C_SURF 1
#define I2C_FFA  (1 << 1)
#define I2C_RUMI (1 << 2)
#define I2C_SIM  (1 << 3)
#define I2C_FLUID (1 << 4)
#define I2C_LIQUID (1 << 5)

#define MSM_8960_GSBI2_QUP_I2C_BUS_ID 2


struct i2c_registry {
        u8                     machs;
        int                    bus;
        struct i2c_board_info *info;
        int                    len;
};

struct i2c_board_info {
        char            type[20];
        unsigned short  flags;
        unsigned short  addr;
        void            *platform_data;
        //struct dev_archdata     *archdata;
        //struct device_node *of_node;
        int             irq;
};

struct i2c_msg
{
	UINT16 addr;   /* slave address */
	UINT16 flags;
#define I2C_M_TEN               0x0010  /* this is a ten bit chip address */
#define I2C_M_RD                0x0001  /* read data, from slave to master */
#define I2C_M_NOSTART           0x4000  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_REV_DIR_ADDR      0x2000  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK        0x1000  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NO_RD_ACK         0x0800  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_RECV_LEN          0x0400  /* length will be first received byte */
	UINT16 len;              /* msg length                           */
    UINT8 *buf;              /* pointer to msg data                  */

};

struct Krait_I2C_Driver
{
	static BOOL i2c_master_send();
	static BOOL i2c_master_recv();
	static UINT32 i2c_transfer();

};


#endif
