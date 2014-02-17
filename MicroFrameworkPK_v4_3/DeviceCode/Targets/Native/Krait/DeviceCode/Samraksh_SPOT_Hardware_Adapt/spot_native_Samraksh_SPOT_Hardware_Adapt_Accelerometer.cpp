//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "spot_native.h"
#include "spot_native_Samraksh_SPOT_Hardware_Adapt_Accelerometer.h"

#include "..\Krait.h"

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

using namespace Samraksh::SPOT::Hardware::Adapt;

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

bool i2c_started = 0;

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
    return 1;
}

static uint8_t samraksh_i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t data) {

	GLOBAL_LOCK(irq);
	i2c_write_byte(true,  false, (addr << 1) | 0);
	i2c_write_byte(false, false, reg);
	i2c_write_byte(false, true,  data);

	return 1;

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

INT8 Accelerometer::ADAPT_Accel_Init( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
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

	// Init I2C module to 400 kHz if not already established
	// This is hard, so we're just going to bit-bang I2C for now...
	//if (adapt_i2c_GSBI12 == NULL)
	//	adapt_i2c_GSBI12 = qup_i2c_init(GSBI_ID_12, 400000, 24000000);

	// Do a test read
	samraksh_i2c_read_reg(BMA250_ADDR, 0x00, 1, &data);

	// TODO check return value, believe it should be 0x03 --NPS

	// Set bandwidth to 7.81 Hz by default.
	samraksh_i2c_write_reg(BMA250_ADDR, 0x10, 0);

	return 1;
}

INT8 Accelerometer::ADAPT_Accel_Reset( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Accelerometer::ADAPT_Accel_SelfTest( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT16 Accelerometer::ADAPT_Accel_GetX( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT16 retVal; 
	UINT8 data[2];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	// Read X register
	samraksh_i2c_read_reg(BMA250_ADDR, 0x02, 2, data);
	retVal = (INT16) ((INT16)data[0] | ((INT16)data[1] << 8));

    return retVal;
}

INT16 Accelerometer::ADAPT_Accel_GetY( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT16 retVal; 
	UINT8 data[2];

	//if (adapt_i2c_GSBI12 == NULL) return 0;

	// Read X register
	samraksh_i2c_read_reg(BMA250_ADDR, 0x04, 2, data);
	retVal = (INT16) ((INT16)data[0] | ((INT16)data[1] << 8));

    return retVal;
}

INT16 Accelerometer::ADAPT_Accel_GetZ( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT16 retVal = 0; 
    return retVal;
}

INT16 Accelerometer::ADAPT_Accel_GetTemperature( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT16 retVal = 0; 
    return retVal;
}

UINT8 Accelerometer::ADAPT_Accel_Raw_Read( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr )
{
    UINT8 retVal = 0; 
    return retVal;
}

INT8 Accelerometer::ADAPT_Accel_Raw_Write( CLR_RT_HeapBlock* pMngObj, UINT8 param0, UINT8 param1, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Accelerometer::ADAPT_Accel_advconfig( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Accelerometer::ADAPT_Accel_SetPowerMode( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Accelerometer::ADAPT_Accel_SetBandwidth( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Accelerometer::ADAPT_Accel_SetBandwidth( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Accelerometer::ADAPT_Accel_GetAll( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT16 param0, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}
