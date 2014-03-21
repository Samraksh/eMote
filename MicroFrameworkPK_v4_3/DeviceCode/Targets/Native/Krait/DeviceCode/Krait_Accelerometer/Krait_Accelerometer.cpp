/*  Name : Krait_Accelerometer.cpp
 *
 *  Author : Christopher Morse
 *
 *  Description :
 *
 */
#include <tinyhal.h>
#include "..\Krait.h"
#include "Krait_Accelerometer.h"

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

// --------------

INT8 ADAPT_Accel_Init()
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
	
	// Do a soft reset
	samraksh_i2c_write_reg(BMA250_ADDR, 0x14, 0xB6);

	// Init I2C module to 400 kHz if not already established
	// This is hard, so we're just going to bit-bang I2C for now...
	//if (adapt_i2c_GSBI12 == NULL)
	//	adapt_i2c_GSBI12 = qup_i2c_init(GSBI_ID_12, 400000, 24000000);

	// Do a test read
	samraksh_i2c_read_reg(BMA250_ADDR, 0x00, 1, &data);

	// TODO check return value, believe it should be 0x03 --NPS

	// Set bandwidth to 7.81 Hz by default.
	samraksh_i2c_write_reg(BMA250_ADDR, 0x10, 0);

	return 0;
}

// Perform softreset.
// Returns all registers to default values
INT8 ADAPT_Accel_Reset()
{
	return samraksh_i2c_write_reg(BMA250_ADDR, 0x14, 0xB6);
}

// TODO
INT8 ADAPT_Accel_SelfTest()
{
    return 0;
}

INT16 ADAPT_Accel_GetX()
{
    INT16 xVal; 
	UINT8 data[2];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	// Read X register
	samraksh_i2c_read_reg(BMA250_ADDR, 0x02, 2, data);
	xVal = (INT16) ((INT16)data[0] | ((INT16)data[1] << 8));
	xVal = xVal >> 6;

    return xVal;
}

INT16 ADAPT_Accel_GetY()
{
    INT16 yVal; 
	UINT8 data[2];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	// Read Y register
	samraksh_i2c_read_reg(BMA250_ADDR, 0x04, 2, data);
	yVal = (INT16) ((INT16)data[0] | ((INT16)data[1] << 8));
	yVal = yVal >> 6;

    return yVal;
}

INT16 ADAPT_Accel_GetZ(  )
{
    INT16 zVal; 
	UINT8 data[2];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	// Read Z register
	samraksh_i2c_read_reg(BMA250_ADDR, 0x06, 2, data);
	zVal = (INT16) ((INT16)data[0] | ((INT16)data[1] << 8));
	zVal = zVal >> 6;

    return zVal;
}

// Returns temperature in degrees C
INT16 ADAPT_Accel_GetTemperature(  )
{
    UINT8 t;

	// Read temperature register
	samraksh_i2c_read_reg(BMA250_ADDR, 0x08, 1, &t);
	
	// Center temperature is 24 degrees C and 1 LSB is 0.5 degree C
	return (INT16)((t>>1) + 24);
}

// If you have the datasheet and want to read/write a particular register not listed...
UINT8 ADAPT_Accel_Raw_Read( UINT8 reg )
{
    UINT8 data; 
	samraksh_i2c_read_reg(BMA250_ADDR, reg, 1, &data);
    return data;
}

// If you have the datasheet and want to read/write a particular register not listed...
INT8 ADAPT_Accel_Raw_Write( UINT8 reg, UINT8 data )
{
	return samraksh_i2c_write_reg(BMA250_ADDR, reg, data);
}

// TODO
INT8 ADAPT_Accel_advconfig( UINT32 config )
{
    return 0;
}

// Currently only sets suspend mode.
// Other options revolve around interrupts.
INT8 ADAPT_Accel_SetPowerMode( UINT32 mode )
{
	UINT8 retVal, data=0;
	if (mode & ACCEL_SUSPEND)
		data |= 0x80;
	
	// All other default values are 0.
	retVal |= samraksh_i2c_write_reg(BMA250_ADDR, 0x11, data);
    
	return retVal;
}

// Accepts bandwidth in Hz, tries to pick the best one.
// If you pick something out of the range, return error.
// Rounds UP
INT8 ADAPT_Accel_SetBandwidth( UINT32 bandwidth )
{
    INT8 retVal = 0;
	UINT8 data=0;
	
	if (bandwidth > 1000)
		return 1; // error, bandwidth too high
	
	if (bandwidth <= 7)
		data = 0;
	else if (bandwidth <= 16)
		data = 0x09;
	else if (bandwidth <= 32)
		data = 0x0A;
	else if (bandwidth <= 64)
		data = 0x0B;
	else if (bandwidth <= 125)
		data = 0x0C;
	else if (bandwidth <= 250)
		data = 0x0D;
	else if (bandwidth <= 500)
		data = 0x0E;
	else if (bandwidth <= 1000)
		data = 0x0F;
		
	retVal |= samraksh_i2c_write_reg(BMA250_ADDR, 0x10, data);
	
    return retVal;
}

// Accepts range in +/- g
// Default is 2
INT8 ADAPT_Accel_SetRange( UINT8 range )
{
    INT8 retVal = 0;
	UINT8 data=0;
	
	if (range > 16)
		return 1; // error, range too high
	
	if (range <= 2)
		data = 0x3;
	else if (range <= 4)
		data = 0x5;
	else if (range <= 8)
		data = 0x8;
	else if (range <= 16)
		data = 0xC;
		
	retVal |= samraksh_i2c_write_reg(BMA250_ADDR, 0xF, data);
	
    return retVal;
}

// Read XYZ in one shot
// data array must be at least size 3.
INT8 ADAPT_Accel_GetAll( INT16 data[] )
{
    INT8 retVal = 0;
	INT16 val; 
	UINT8 d[6];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	retVal = samraksh_i2c_read_reg(BMA250_ADDR, 0x02, 6, d);
	
	// X
	val = (INT16) ((INT16)d[0] | ((INT16)d[1] << 8));
	val = val >> 6;
	data[0] = val;
	
	// Y
	val = (INT16) ((INT16)d[2] | ((INT16)d[3] << 8));
	val = val >> 6;
	data[1] = val;
	
	// Z
	val = (INT16) ((INT16)d[4] | ((INT16)d[5] << 8));
	val = val >> 6;
	data[2] = val;
	
    return retVal;
}
