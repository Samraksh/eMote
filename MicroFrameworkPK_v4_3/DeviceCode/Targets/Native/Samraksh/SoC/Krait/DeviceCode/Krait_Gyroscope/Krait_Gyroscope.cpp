/*  Name : Krait_Gyroscope.cpp
 *
 *  Author : Christopher Morse
 *
 *  Description :
 *
 */
#include <tinyhal.h>
#include "..\Krait.h"
#include "Krait_Gyroscope.h"

#define MSM_TMR_BASE        0x0200A000
#define MSM_DGT_BASE        (MSM_TMR_BASE + 0x24)
#define DGT_REG(off)        (MSM_DGT_BASE + (off))
#define DGT_ENABLE           DGT_REG(0x0008)
#define DGT_CLK_CTL          DGT_REG(0x0010)
#define DGT_MATCH_VAL        DGT_REG(0x0000)
#define DGT_CLEAR            DGT_REG(0x000C)
#define GPT_ENABLE_CLR_ON_MATCH_EN        2
#define GPT_ENABLE_EN                     1
#define DGT_ENABLE_CLR_ON_MATCH_EN        2
#define DGT_ENABLE_EN                     1

// Globals

static bool i2c_started = 0;

// Statics

static UINT8 gyro_addr = 0x68;

// TODO: Clean up the source so you can not use statics here
static void udelay(unsigned int usecs)
{
	usecs = (usecs * 33 + 1000 - 33) / 1000;

	writel(0, GPT_CLEAR);
	writel(0, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) != 0) ;

	writel(GPT_ENABLE_EN, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) < usecs) ;

	writel(0, GPT_ENABLE);
	writel(0, GPT_CLEAR);
}

static void mdelay(unsigned int msecs)
{
	msecs *= 33;

	writel(0, GPT_CLEAR);
	writel(0, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) != 0) ;

	writel(GPT_ENABLE_EN, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) < msecs) ;

	writel(0, GPT_ENABLE);
	writel(0, GPT_CLEAR);
}

static void gpio_tlmm_config(UINT32 gpio, UINT8 func,
		      UINT8 dir, UINT8 pull,
		      UINT8 drvstr, UINT32 enable)
{
	UINT32 val = 0;
	val |= pull;
	val |= func << 2;
	val |= drvstr << 6;
	val |= enable << 9;
	UINT32 *addr = (UINT32 *)GPIO_CONFIG_ADDR(gpio);
	writel(val, addr);
	return;
}

static void gpio_set(UINT32 gpio, UINT32 dir)
{
	UINT32 *addr = (UINT32 *)GPIO_IN_OUT_ADDR(gpio);
	writel(dir, addr); // second bit controls GPIO
	return;
}

static void clear_SCL(void) {
    gpio_set(45, 0);
    gpio_tlmm_config(45, 0, 1, 0, 4, 1); // TODO: Test if 10MA is needed (the '4' parameter) --NPS
}

static void clear_SDA(void) {
    gpio_set(44, 0);
    gpio_tlmm_config(44, 0, 1, 0, 4, 1);
}

static void arbitration_lost() {
	//TODO: Do something here --NPS
}

static void I2C_delay() {
	udelay(2);
}

static bool read_SCL() {
    gpio_tlmm_config(45, 0, 0, 0, 4, 0);
	unsigned int *addr = (unsigned int *)GPIO_IN_OUT_ADDR(45);
	return readl(addr)&1;
}

static bool read_SDA() {
    gpio_tlmm_config(44, 0, 0, 0, 4, 0);
	unsigned int *addr = (unsigned int *)GPIO_IN_OUT_ADDR(44);
	return readl(addr)&1;
}

static void i2c_start_cond(void) {
  if (i2c_started) { // if started, do a restart cond
    // set SDA to 1
    read_SDA();
    I2C_delay();
    while (read_SCL() == 0) {  // Clock stretching
      // You should add timeout to this loop
    }
    // Repeated start setup time, minimum 4.7us
    I2C_delay();
  }
  if (read_SDA() == 0) {
    arbitration_lost();
  }
  // SCL is high, set SDA from 1 to 0.
  clear_SDA();
  I2C_delay();
  clear_SCL();
  i2c_started = true;
}

static void i2c_stop_cond(void){
  // set SDA to 0
  clear_SDA();
  I2C_delay();
  // Clock stretching
  while (read_SCL() == 0) {
    // add timeout to this loop.
  }
  // Stop bit setup time, minimum 4us
  I2C_delay();
  // SCL is high, set SDA from 0 to 1
  if (read_SDA() == 0) {
    arbitration_lost();
  }
  I2C_delay();
  i2c_started = false;
}

// Write a bit to I2C bus
static void i2c_write_bit(bool bit) {
  if (bit) {
    read_SDA();
  } else {
    clear_SDA();
  }
  I2C_delay();
  while (read_SCL() == 0) { // Clock stretching
    // You should add timeout to this loop
  }
  // SCL is high, now data is valid
  // If SDA is high, check that nobody else is driving SDA
  if (bit && read_SDA() == 0) {
    arbitration_lost();
  }
  I2C_delay();
  clear_SCL();
}

// Read a bit from I2C bus
static bool i2c_read_bit(void) {
  bool bit;
  // Let the slave drive data
  read_SDA();
  I2C_delay();
  while (read_SCL() == 0) { // Clock stretching
    // You should add timeout to this loop
  }
  // SCL is high, now data is valid
  bit = read_SDA();
  I2C_delay();
  clear_SCL();
  return bit;
}

// Write a byte to I2C bus. Return 0 if ack by the slave.
static bool i2c_write_byte(bool send_start,
                    bool send_stop,
                    unsigned char byte) {
  unsigned bit;
  bool nack;
  if (send_start) {
    i2c_start_cond();
  }
  for (bit = 0; bit < 8; bit++) {
    i2c_write_bit((byte & 0x80) != 0);
    byte <<= 1;
  }
  nack = i2c_read_bit();
  if (send_stop) {
    i2c_stop_cond();
  }
  return nack;
}

// Read a byte from I2C bus
static unsigned char i2c_read_byte(bool nack, bool send_stop) {
  unsigned char byte = 0;
  unsigned bit;
  for (bit = 0; bit < 8; bit++) {
    byte = (byte << 1) | i2c_read_bit();
  }
  i2c_write_bit(nack);
  if (send_stop) {
    i2c_stop_cond();
  }
  return byte;
}

static uint8_t samraksh_i2c_read_reg(uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data) {
	GLOBAL_LOCK(irq);
    i2c_write_byte(true,  false, (addr<<1) | 0);
    i2c_write_byte(false, false, reg);
    i2c_write_byte(true,  false, (addr<<1) | 1);

    for(int i=0; i<size; i++) {
        if(i == size-1)
            data[i] = i2c_read_byte(true, true);
        else
            data[i] = i2c_read_byte(false, false);
    }
    return 0;
}

static uint8_t samraksh_i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t data) {

	GLOBAL_LOCK(irq);
	i2c_write_byte(true,  false, (addr << 1) | 0);
	i2c_write_byte(false, false, reg);
	i2c_write_byte(false, true,  data);

	return 0;

	/* //Code for using the real I2C module from LK (porting it will be a bitch) --NPS
	uint8_t my_data[2];

	my_data[0] = reg;
	my_data[1] = data;

	struct i2c_msg msg_buf[] = {
		{addr, I2C_M_WR, 2, &my_data},
	};

	// Do the transaction bundle
	// device, bundle, num transactions in bundle
	qup_i2c_xfer(dev, msg_buf, 1);

	return 1;
	*/
}

INT8 gyro_init()
{
    UINT8 data;

	// Set pins for GPIO mode for the power controls.
	gpio_tlmm_config(151, 0, 1, 0, 0, 1);
	gpio_tlmm_config(32, 0, 1, 0, 0, 1);
	gpio_set(151, 2); // 1.8v power enable
	gpio_set(32, 2); // 3.3v power enable

	mdelay(10);

    // I2C pins set to function mode
	// Not needed for bit-banging
	//gpio_tlmm_config(44, 1, 1, 0, 0, 0);
	//gpio_tlmm_config(45, 1, 1, 0, 0, 0);

	// Reset both SCL and SDA
	gpio_set(44, 0);
	gpio_set(45, 0);
	{
		GLOBAL_LOCK(irq); // TODO: Don't use global lock here? --NPS
		read_SCL();
		read_SDA();
	}
	mdelay(10);
	return 0;
}

INT8 gyro_reset()
{
	return samraksh_i2c_write_reg(gyro_addr, 0x3D, 0x01); // GYRO_RST
}

INT8 gyro_self_test()
{
	return 0;
}

INT16 gyro_get_X()
{
    INT16 xVal; 
	UINT8 data[2];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	// Read X register
	samraksh_i2c_read_reg(gyro_addr, 0x1D, 2, data);
	xVal = (INT16) ((INT16)data[1] | ((INT16)data[0] << 8));

    return xVal;
}

INT16 gyro_get_Y()
{
    INT16 yVal; 
	UINT8 data[2];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	// Read X register
	samraksh_i2c_read_reg(gyro_addr, 0x1F, 2, data);
	yVal = (INT16) ((INT16)data[1] | ((INT16)data[0] << 8));

    return yVal;
}

INT16 gyro_get_Z()
{
    INT16 zVal; 
	UINT8 data[2];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	// Read X register
	samraksh_i2c_read_reg(gyro_addr, 0x21, 2, data);
	zVal = (INT16) ((INT16)data[1] | ((INT16)data[0] << 8));

    return zVal;
}

INT16 gyro_get_temp()
{
    INT16 temp; 
	UINT8 data[2];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	// Read X register
	samraksh_i2c_read_reg(gyro_addr, 0x1B, 2, data);
	temp = (INT16) ((INT16)data[1] | ((INT16)data[0] << 8));

    return temp;
}

UINT8 gyro_raw_read(UINT8 reg)
{
    UINT8 data; 
	samraksh_i2c_read_reg(gyro_addr, reg, 1, &data);
    return data;
}

INT8 gyro_raw_write( UINT8 reg, UINT8 data )
{
	return samraksh_i2c_write_reg(gyro_addr, reg, data);
}

INT8 gyro_config( UINT32 config )
{
    return 0;
}

INT8 gyro_offsets(INT16 xOffset, INT16 yOffset, INT16 zOffset)
{
	return 0;
}

INT8 gyro_sample_config(UINT32 config)
{
	return 0;
}

INT8 gyro_power( UINT32 mode )
{
	return 0;
}

INT8 gyro_get_XYZ( INT16 data[] )
{
    INT8 retVal = 0;
	INT16 val; 
	UINT8 d[6];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	retVal = samraksh_i2c_read_reg(gyro_addr, 0x1D, 6, d);
	
	// X
	val = (INT16) ((INT16)d[1] | ((INT16)d[0] << 8));
	data[0] = val;
	
	// Y
	val = (INT16) ((INT16)d[3] | ((INT16)d[2] << 8));
	data[1] = val;
	
	// Z
	val = (INT16) ((INT16)d[5] | ((INT16)d[4] << 8));
	data[2] = val;
	
    return retVal;
}
