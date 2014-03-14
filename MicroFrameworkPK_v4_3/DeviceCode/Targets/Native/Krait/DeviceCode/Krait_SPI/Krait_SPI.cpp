////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Krait_SPI.h>


////Krait_SPI_Driver Krait_SPI_Driver_obj;

#define SOC8200_USE_SPI1
//#define SOC8200_USE_SPI2
//#define SOC8200_USE_SPI3
//#define SOC8200_USE_SPI4

//--//

/***************************************************************************/

#if 0
static UINT32 SPI_CHAN_TO_MOD(UINT32 x) {
	if (x <  4) return OMAP3_SPI1;
	if (x <  6) return OMAP3_SPI2;
	if (x <  8) return OMAP3_SPI3;
	if (x == 8) return OMAP3_SPI4;
	return OMAP3_SPI1;
}
#endif


//AnanthAtSamraksh

#define SPI_Mode_Select      ((UINT16)0xF7FF)
#define CR1_CLEAR_Mask       ((UINT16)0x3040)

/* SPI SSOE mask */
#define CR2_SSOE_Set         ((UINT16)0x0004)
#define CR2_SSOE_Reset       ((UINT16)0xFFFB)

/* SPI SPE mask */
#define CR1_SPE_Set          ((UINT16)0x0040)
#define CR1_SPE_Reset        ((UINT16)0xFFBF)

/**
  * @brief  Initializes the SPIx peripheral according to the specified
  *   parameters in the SPI_InitStruct.
  * @param  SPIx: where x can be 1, 2 or 3 to select the SPI peripheral.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
  *   contains the configuration information for the specified SPI peripheral.
  * @retval None
  */
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
  UINT16 tmpreg = 0;

  /* check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));

  /* Check the SPI parameters */
  assert_param(IS_SPI_DIRECTION_MODE(SPI_InitStruct->SPI_Direction));
  assert_param(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
  assert_param(IS_SPI_DATASIZE(SPI_InitStruct->SPI_DataSize));
  assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
  assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
  assert_param(IS_SPI_NSS(SPI_InitStruct->SPI_NSS));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
  assert_param(IS_SPI_FIRST_BIT(SPI_InitStruct->SPI_FirstBit));
  assert_param(IS_SPI_CRC_POLYNOMIAL(SPI_InitStruct->SPI_CRCPolynomial));

/*---------------------------- SPIx CR1 Configuration ------------------------*/
  /* Get the SPIx CR1 value */
  tmpreg = SPIx->CR1;
  /* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
  tmpreg &= CR1_CLEAR_Mask;
  /* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
     master/salve mode, CPOL and CPHA */
  /* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value */
  /* Set SSM, SSI and MSTR bits according to SPI_Mode and SPI_NSS values */
  /* Set LSBFirst bit according to SPI_FirstBit value */
  /* Set BR bits according to SPI_BaudRatePrescaler value */
  /* Set CPOL bit according to SPI_CPOL value */
  /* Set CPHA bit according to SPI_CPHA value */
  tmpreg |= (UINT16)((UINT32)SPI_InitStruct->SPI_Direction | SPI_InitStruct->SPI_Mode |
                  SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_CPOL |
                  SPI_InitStruct->SPI_CPHA | SPI_InitStruct->SPI_NSS |
                  SPI_InitStruct->SPI_BaudRatePrescaler | SPI_InitStruct->SPI_FirstBit);
  /* Write to SPIx CR1 */
  SPIx->CR1 = tmpreg;

  /* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
  SPIx->I2SCFGR &= SPI_Mode_Select;

/*---------------------------- SPIx CRCPOLY Configuration --------------------*/
  /* Write to SPIx CRCPOLY */
  SPIx->CRCPR = SPI_InitStruct->SPI_CRCPolynomial;
}


/**
  * @brief  Enables or disables the SS output for the selected SPI.
  * @param  SPIx: where x can be 1, 2 or 3 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx SS output.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Enable the selected SPI SS output */
    SPIx->CR2 |= CR2_SSOE_Set;
  }
  else
  {
    /* Disable the selected SPI SS output */
    SPIx->CR2 &= CR2_SSOE_Reset;
  }
}


/**
  * @brief  Enables or disables the specified SPI peripheral.
  * @param  SPIx: where x can be 1, 2 or 3 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Enable the selected SPI peripheral */
    SPIx->CR1 |= CR1_SPE_Set;
  }
  else
  {
    /* Disable the selected SPI peripheral */
    SPIx->CR1 &= CR1_SPE_Reset;
  }
}


/**
  * @brief  Checks whether the specified SPI/I2S flag is set or not.
  * @param  SPIx: where x can be
  *   - 1, 2 or 3 in SPI mode
  *   - 2 or 3 in I2S mode
  * @param  SPI_I2S_FLAG: specifies the SPI/I2S flag to check.
  *   This parameter can be one of the following values:
  *     @arg SPI_I2S_FLAG_TXE: Transmit buffer empty flag.
  *     @arg SPI_I2S_FLAG_RXNE: Receive buffer not empty flag.
  *     @arg SPI_I2S_FLAG_BSY: Busy flag.
  *     @arg SPI_I2S_FLAG_OVR: Overrun flag.
  *     @arg SPI_FLAG_MODF: Mode Fault flag.
  *     @arg SPI_FLAG_CRCERR: CRC Error flag.
  *     @arg I2S_FLAG_UDR: Underrun Error flag.
  *     @arg I2S_FLAG_CHSIDE: Channel Side flag.
  * @retval The new state of SPI_I2S_FLAG (SET or RESET).
  */
FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, UINT16 SPI_I2S_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_GET_FLAG(SPI_I2S_FLAG));
  /* Check the status of the specified SPI/I2S flag */
  if ((SPIx->SR & SPI_I2S_FLAG) != (UINT16)RESET)
  {
    /* SPI_I2S_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* SPI_I2S_FLAG is reset */
    bitstatus = RESET;
  }
  /* Return the SPI_I2S_FLAG status */
  return  bitstatus;
}


/**
  * @brief  Transmits a Data through the SPIx/I2Sx peripheral.
  * @param  SPIx: where x can be
  *   - 1, 2 or 3 in SPI mode
  *   - 2 or 3 in I2S mode
  * @param  Data : Data to be transmitted.
  * @retval None
  */
void SPI_I2S_SendData(SPI_TypeDef* SPIx, UINT16 Data)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));

  /* Write in the DR register the data to be sent */
  SPIx->DR = Data;

}


/**
  * @brief  Returns the most recent received data by the SPIx/I2Sx peripheral.
  * @param  SPIx: where x can be
  *   - 1, 2 or 3 in SPI mode
  *   - 2 or 3 in I2S mode
  * @retval The value of the received data.
  */
UINT16 SPI_I2S_ReceiveData(SPI_TypeDef* SPIx)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));

  /* Return the data in the DR register */
  return SPIx->DR;
}

//AnanthAtSamraksh

#ifdef DEBUG_SPI
static void SPI_print_status(const SPI_config &config)
{
	unsigned int value = 0;
	value = readl(QUP_CONFIG(config.gsbi_id));
	hal_printf("QUP_CONFIG is :0x%x\n", value);
	value = readl(QUP_STATE(config.gsbi_id));
	hal_printf("QUP_STATE is :0x%x\n", value);
	value = readl(QUP_IO_MODES(config.gsbi_id));
	hal_printf("QUP_IO_MODES is :0x%x\n", value);
}
#endif


#if 0

void gpio_set(UINT32 gpio, UINT32 dir)
{
	UINT32 *addr = (UINT32 *)GPIO_IN_OUT_ADDR(gpio);
	writel(dir ? 1UL << 1 : 0, addr);
	return;
}

UINT32* gpio_get(UINT32 gpio)
{
	UINT32 *addr = (UINT32 *)GPIO_IN_OUT_ADDR(gpio);
	return readl(addr);
}

void QUP_Reset(const SPI_config &config)
{
	writel(0xFFFFF, QUP_SW_RESET(config.gsbi_id));
}

BOOL set_QUP_state(const SPI_config &config, int value)
{
	UINT32 QUP_CONFIG;

	if(readl(QUP_STATE(config.gsbi_id) | QUP_STATE_STATE_VALID_S))
	{
		QUP_CONFIG = 0;
		if(value == RESET_STATE)	//RESET_STATE
		{
			if(get_QUP_State() == PAUSE_STATE)
			{
				// Need two writes of (10) for transition from PAUSE_STATE to RESET_STATE to complete.
				QUP_CONFIG |= QUP_STATE_STATE_CLEAR_STATE;
				writel(QUP_CONFIG, QUP_STATE(config.gsbi_id));
				writel(QUP_CONFIG, QUP_STATE(config.gsbi_id));
			}
			else
			{
				QUP_CONFIG |= QUP_STATE_STATE_RESET_STATE;
				writel(QUP_CONFIG, QUP_STATE(config.gsbi_id));
			}
			return TRUE;
		}
		else if(value == RUN_STATE)	//Mini-core released from reset and enters RUN_STATE
		{
			QUP_CONFIG |= QUP_STATE_STATE_RUN_STATE;
			writel(QUP_CONFIG, QUP_STATE(config.gsbi_id));
			return TRUE;
		}
		else if(value == CLEAR_STATE)	//STATE is cleared
		{
			QUP_CONFIG |= QUP_STATE_STATE_CLEAR_STATE;
			writel(QUP_CONFIG, QUP_STATE(config.gsbi_id));
			return TRUE;
		}
		else if(value == PAUSE_STATE)	//Mini-core enters PAUSE_STATE
		{
			QUP_CONFIG |= QUP_STATE_STATE_PAUSE_STATE;
			writel(QUP_CONFIG, QUP_STATE(config.gsbi_id));
			return TRUE;
		}
	}
	return FALSE;
}


int get_QUP_State(const SPI_config &config)
{
	if(readl(QUP_STATE(config.gsbi_id) | QUP_STATE_STATE_VALID_S))
	{
		if(readl(QUP_STATE(config.gsbi_id) & QUP_STATE_STATE) == 0)
			return RESET_STATE;
		else if(readl(QUP_STATE(config.gsbi_id) & QUP_STATE_STATE) == 1)
			return RUN_STATE;
		else if(readl(QUP_STATE(config.gsbi_id) & QUP_STATE_STATE) == 3)
			return PAUSE_STATE;
	}
	return -1;
}


void Krait_SPI_Driver::spi_write_bit(UINT8 data)
{
	gpio_set(SPI_GPIO_CLK, 0);
	usleep(1);	//sleep for 1us
	gpio_set(SPI_GPIO_MOSI, (data & 0x01) ? 1 : 0);
	gpio_set(SPI_GPIO_CLK, 1);
}

UINT8* Krait_SPI_Driver::spi_read_bit()
{
	UINT8* data;
	data = gpio_get(SPI_GPIO_MISO);
	return data;
}


void Krait_SPI_Driver::spi_write_byte(UINT32 data)
{
	UINT32 constant = 0x80;
	UINT32 byteSize = 8;
	while(byteSize--)
	{
		gpio_set(SPI_GPIO_CLK, 0);
		usleep(1);	//sleep for 1us
		gpio_set(SPI_GPIO_MOSI, ((data & constant) == constant) ? 1 : 0);
		gpio_set(SPI_GPIO_CLK, 1);
		data <<= 1;
	}
	gpio_set(SPI_GPIO_MOSI, 0);
}


UINT32* Krait_SPI_Driver::spi_read_byte()
{
	UINT32* data;
	data = gpio_get(SPI_GPIO_MISO);
	return data;
#if 0
	UINT32 constant = 0x80;
	UINT32 byteSize = 8;
	while(byteSize--)
	{
		gpio_set(SPI_GPIO_CLK, 0);
		usleep(1);	//sleep for 1us
		data = data | gpio_get(SPI_GPIO_MISO);
		gpio_set(SPI_GPIO_CLK, 1);
		data <<= 1;
	}
	gpio_set(SPI_GPIO_MOSI, 0);
#endif
}


/*void gpio_set(UINT32 gpio, UINT32 val)
{
	unsigned int *addr = (unsigned int *)GPIO_IN_OUT_ADDR(gpio);
	writel(val, addr);
}*/


/*
	Returns TRUE if init success
*/
BOOL Krait_SPI_Driver::Initialize(int gsbi_id, int SPI_mod, int IO_mode, int master_slave_mode, int clock_rate)
{
	UINT32 l; // Temp var
	UINT32 i; // Timeout

	hal_printf("Inside SPI_Driver Initialize function \n");

	if(gsbi_id <= 0)
		return Invalid_Device_ID;


	/*config.MSK_IDLE = FALSE;
	config.MSK_SampleEdge = FALSE;
	config.MD_16bits = FALSE;
	config.Clock_RateKHz = 0;*/ // Should default to 32768 divider

	/*
	config.DeviceCS = ((inputConfig.DeviceCS != NULL) ? inputConfig.DeviceCS : (GPIO_PIN) 10);
	config.CS_Active = ((inputConfig.CS_Active != NULL) ? inputConfig.CS_Active : FALSE);
	config.MSK_IDLE = ((inputConfig.MSK_IDLE != NULL) ? inputConfig.MSK_IDLE : FALSE);
	config.MSK_SampleEdge = ((inputConfig.MSK_SampleEdge != NULL) ? inputConfig.MSK_SampleEdge : FALSE);
	config.MD_16bits = ((inputConfig.MD_16bits != NULL) ? inputConfig.MD_16bits : FALSE);
	config.Clock_RateKHz = ((inputConfig.Clock_RateKHz != NULL) ? inputConfig.Clock_RateKHz : 0);  // Should default to 32768 divider
	config.CS_Setup_uSecs = ((inputConfig.CS_Setup_uSecs != NULL) ? inputConfig.CS_Setup_uSecs : 0);
	config.CS_Hold_uSecs = ((inputConfig.CS_Hold_uSecs != NULL) ? inputConfig.CS_Hold_uSecs : 0);
	config.SPI_mod = ((inputConfig.SPI_mod != NULL) ? inputConfig.SPI_mod : 0);
	config.BusyPin = ((inputConfig.BusyPin != NULL) ? inputConfig.BusyPin : (GPIO_PIN)1);
	*/
	config.gsbi_id = gsbi_id;
	config.SPI_mod = SPI_mod;
	config.IO_mode = IO_mode;
	config.master_slave_mode = master_slave_mode;
	config.clock_rate = clock_rate;


	/* Refer 16.4.4.3 from 80-N1622-3 MSM8960   -MSM8960    PRO-MSM8X60A   -MSM8X60A    PRO MOBILE STATION MODEM SOFTWARE INTERFACE.pdf.qck for details */

	/* Configure GSBI for SPI protocol */
	writel(GSBI_PROTOCOL_CODE_SPI, GSBI_CTRL_REG(config.gsbi_id));

	/* Step 1 in 16.4.4.3:
	 * Check if the SPI mini core was generated as part of the GSBI instance in use.
	 * This is done by checking the SPI_GEN field (bit 3) in QUP_STATE register to be equal to 1. */
	unsigned int data = readl(QUP_STATE(config.gsbi_id));
	if(!data & (1 << QUP_STATE_SPI_GEN_S))
		return Initialization_Error;

	return TRUE;
}


void Krait_SPI_Driver::Uninitialize(const SPI_config &config)
{
	/* Step 2 in 16.4.4.1:
	 * Disable QUP error reporting */
	int Error_Config = 0, QUP_Config = 0;

	Error_Config |= (0 << QUP_ERROR_FLAGS_EN_TIMEOUT);
	Error_Config |= (0 << QUP_ERROR_FLAGS_EN_OUTPUT_OVERRUN);
	Error_Config |= (0 << QUP_ERROR_FLAGS_EN_INPUT_UNDERRUN);
	Error_Config |= (0 << QUP_ERROR_FLAGS_EN_OUTPUT_UNDERRUN);
	Error_Config |= (0 << QUP_ERROR_FLAGS_EN_INPUT_OVERRUN);
	writel(Error_Config, QUP_ERROR_FLAGS_EN(config.gsbi_id));

	/* Step 2 in 16.4.4.3:
	 * Set the MINI_CORE in null mode */
	QUP_Config = 0;
	QUP_Config |= (0x0000 << QUP_CONFIG_MINI_CORE_SPI_S);
	writel(QUP_Config, QUP_CONFIG(config.gsbi_id));

	QUP_Reset(config);

	/*
	//	Disable Clocks
	l = __raw_readl(OMAP3_SPI_FCLOCK); // F CLOCK
	l |= OMAP3_SPI1_FCLOCK_DIS;
	__raw_writel(l, OMAP3_SPI_FCLOCK);
	l = __raw_readl(OMAP3_SPI_ICLOCK); // I CLOCK
	l |= OMAP3_SPI1_ICLOCK_DIS;
	__raw_writel(l, OMAP3_SPI_ICLOCK);

	// Disable and clear SPI interrupts
	__raw_writel(0, 			OMAP3_SPI_IRQENABLE+OMAP3_SPI1_BASE); // Disable
	__raw_writel(0x0001777F, 	OMAP3_SPI_IRQSTATUS+OMAP3_SPI1_BASE); // Clear
	// SPI INTERRUPTS ARE NOW DISABLED
	 */
}


void Krait_SPI_Driver::ISR( void* Param )
{
    ASSERT(0);
}

BOOL Krait_SPI_Driver::nWrite16_nRead16( const SPI_config& config, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{
	if(!Xaction_Start( config )) return FALSE;

    {
        SPI_XACTION_16 Transaction;

        Transaction.Read16          = Read16;
        Transaction.ReadCount       = ReadCount;
        Transaction.ReadStartOffset = ReadStartOffset;
        Transaction.Write16         = Write16;
        Transaction.WriteCount      = WriteCount;
        Transaction.SPI_mod         = config.SPI_mod;

        if(!Xaction_nWrite16_nRead16( Transaction )) return FALSE;
    }

    return Xaction_Stop( config );
}

BOOL Krait_SPI_Driver::nWrite8_nRead8( const SPI_config& config, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{

	if(!Xaction_Start( config )) return FALSE;

    {
        SPI_XACTION_8 Transaction;

        Transaction.Read8           = Read8;
        Transaction.ReadCount       = ReadCount;
        Transaction.ReadStartOffset = ReadStartOffset;
        Transaction.Write8          = Write8;
        Transaction.WriteCount      = WriteCount;
        Transaction.SPI_mod        = config.SPI_mod;
        Transaction.BusyPin.Pin     = GPIO_PIN_NONE;

        if(!Xaction_nWrite8_nRead8( Transaction )) return FALSE;
    }

    return Xaction_Stop( config );
}


/*
	Setup SPI channel
	After init, channel in disabled state.
	Returns TRUE.
*/
BOOL Krait_SPI_Driver::Xaction_Start( const SPI_config& config)
{

	UINT32 conf_reg;		// SPI channel configuration register
	UINT32 modulctrl_reg;	// SPI module configuration register
	UINT32 ctrl_reg;		// SPI channel control register
	UINT32 t, n, SPI_Config = 0, QUP_Config = 0;			// temp
	UINT32 chan, SPI_mod = 0;		// channel
	UINT32 CPOL = 0, CPHA = 0;
	UINT32 mask;			// mask to find clock divider
	UINT32 clock;			// clock rate finder
	UINT32 myclock;

	int id = config.gsbi_id;
	int master_slave_mode = config.master_slave_mode;
	int IO_Mode = config.IO_mode;
	int clockRate = config.clock_rate;
	int enableError = 0, Error_Config = 0;


	/* Step 2 in 16.4.4.3:
	 * Set the MINI_CORE field in QUP_CONFIG register for SPI core
	 * The contents are to be changed only when in RESET_STATE (QUP_STATE register) */
	if(get_QUP_State(config) == -1)
	{
		return Invalid_QUP_State;
	}
	else
	{
		QUP_Config = 0;
		/* Step 1 in 16.4.4.1:
		 * Enable the core and app clocks */
		QUP_Config |= QUP_CONFIG_CORE_CLK_ON_EN;
		QUP_Config |= QUP_CONFIG_APP_CLK_ON_EN;

		/* Step 2 in 16.4.4.3:
		 * Set the MINI_CORE in SPI mode */
		QUP_Config |= QUP_CONFIG_MINI_CORE_SPI;

		/* Step 3 in 16.4.4.3:
		 * Set the N field in the QUP_CONFIG register so that transfer size >= 4 bits */
		QUP_Config |= QUP_CONFIG_N;

		/* Step 3 in 16.4.4.4:
		 * Set up full duplex mode */
		QUP_Config |= QUP_CONFIG_NO_INPUT;
		QUP_Config |= QUP_CONFIG_NO_OUTPUT;

		writel(QUP_Config, QUP_CONFIG(id));
	}

	/* Step 2 in 16.4.4.1:
	 * Setup QUP error reporting */
	if(enableError)
	{
		Error_Config |= (1 << QUP_ERROR_FLAGS_EN_TIMEOUT);
		Error_Config |= (1 << QUP_ERROR_FLAGS_EN_OUTPUT_OVERRUN);
		Error_Config |= (1 << QUP_ERROR_FLAGS_EN_INPUT_UNDERRUN);
		Error_Config |= (1 << QUP_ERROR_FLAGS_EN_OUTPUT_UNDERRUN);
		Error_Config |= (1 << QUP_ERROR_FLAGS_EN_INPUT_OVERRUN);
	}
	else
	{
		Error_Config |= (0 << QUP_ERROR_FLAGS_EN_TIMEOUT);
		Error_Config |= (0 << QUP_ERROR_FLAGS_EN_OUTPUT_OVERRUN);
		Error_Config |= (0 << QUP_ERROR_FLAGS_EN_INPUT_UNDERRUN);
		Error_Config |= (0 << QUP_ERROR_FLAGS_EN_OUTPUT_UNDERRUN);
		Error_Config |= (0 << QUP_ERROR_FLAGS_EN_INPUT_OVERRUN);
	}
	writel(Error_Config, QUP_ERROR_FLAGS_EN(id));


	/* Step 4 in 16.4.4.3:
	 * MASTER mode (MOSI, MISO): Program the "SLAVE_OPERATION" field in the SPI_CONFIG register for SPI master configuration.
	 * The contents are to be changed only when in RESET_STATE (QUP_STATE register).
	 *
	 * Step 6 in 16.4.4.3:
	 * Clock-rate: Operation upto 26MHz, do not set the HS_MODE bit in SPI_CONFIG.
	 * For operation between 26 and 52 MHz, set the HS_MODE bit in SPI_CONFIG. */
	if(get_QUP_State(config) == -1)
	{
		return Invalid_QUP_State;
	}
	else
	{
		SPI_Config = 0;
		SPI_Config |= SPI_CONFIG_MASTER_OPER;

		if(clockRate == Non_HS_MODE)
		{
			SPI_Config |= SPI_CONFIG_NON_HS_MODE;
		}
		else
		{
			SPI_Config |= SPI_CONFIG_HS_MODE;
		}
		writel(SPI_Config, SPI_CONFIG(id));
	}

	/* Step 1 in 16.4.4.4:
	 * Check the SIZE fields in QUP_IO_MODES to find out I/O FIFO block and sizes */
	UINT32 Input_Output_Size_temp = readl(QUP_IO_MODES(id));
	UINT32 Input_FIFO_Size = Input_Output_Size_temp & (0x111 << QUP_IO_MODES_INPUT_FIFO_SIZE);
	UINT32 Input_Block_Size = Input_Output_Size_temp & (0x11 << QUP_IO_MODES_INPUT_BLOCK_SIZE);
	UINT32 Output_FIFO_Size = Input_Output_Size_temp & (0x111 << QUP_IO_MODES_OUTPUT_FIFO_SIZE);
	UINT32 Output_Block_Size = Input_Output_Size_temp & (0x11 << QUP_IO_MODES_OUTPUT_BLOCK_SIZE);

	/* Step 4 in 16.4.4.4:
	 * Setup QUP IO mode */
	QUP_Config = 0;
	if(IO_Mode == FIFO_Mode)
	{
		QUP_Config |= (0x00 << QUP_IO_MODES_INPUT_MODE);
		QUP_Config |= (0x00 << QUP_IO_MODES_OUTPUT_MODE);
		/* Step 8 in 16.4.4.4:
		 * Program the QUP_MX_READ_COUNT register for input FIFO and QUP_MX_WRITE_COUNT for output FIFO.
		 * Values written should not exceed the FIFO depth (BLOCK_SIZE * FIFO_SIZE) */
		//// Place holder for above step
	}
	else
	{
		/* Step 9 in 16.4.4.4:
		 * Program the QUP_MX_INPUT_COUNT register for input FIFO and QUP_MX_OUTPUT_COUNT for output FIFO.
		 * Allows total mini-core transfers to be less than an exact multiple of OUTPUT_BLOCK_SIZE */
		//// Place holder for above step

		if(IO_Mode == Block_Mode)
		{
			QUP_Config |= (0x01 << QUP_IO_MODES_INPUT_MODE);
			QUP_Config |= (0x01 << QUP_IO_MODES_OUTPUT_MODE);
		}
		else if(IO_Mode == Data_Mover_Mode)
		{
			QUP_Config |= (0x10 << QUP_IO_MODES_INPUT_MODE);
			QUP_Config |= (0x10 << QUP_IO_MODES_OUTPUT_MODE);
		}
	}
	/* Step 5,6,7 in 16.4.4.4:
	 * Program the PACK_EN field in QUP_IO_MODES to enable/disable packing transfers of size N into 32-bit words.
	 * Program the UNPACK_EN field in QUP_IO_MODES to enable/disable unpacking 32-bit words into N sized transfers.
	 * Program the OUTPUT_BIT_SHIFT_EN field in the QUP_IO_MODES field as needed. */
	QUP_Config |= (1 << QUP_IO_MODES_OUTPUT_BITSHIFT_EN);
	QUP_Config |= (1 << QUP_IO_MODES_PACK_EN);
	QUP_Config |= (1 << QUP_IO_MODES_UNPACK_EN);
	writel(QUP_Config, QUP_IO_MODES(id));


	/* Step 8 in 16.4.4.3:
	 * Changing the contents of the SPI_IO_CONTROL register (clock polarity and phase). They are to be changed only when in RESET_STATE (QUP_STATE register) */
	if(get_QUP_State(config) == -1)
	{
		return Invalid_QUP_State;
	}
	else
	{
		if(master_slave_mode == Master)
		{
			SPI_Config = 0;
			SPI_Config |= SPI_IO_CONTROL_CLK_IDLE_HIGH;
			SPI_Config |= SPI_IO_CONTROL_CLK_ALWAYS_ON;

			SPI_mod = config.SPI_mod;
			if(SPI_mod == CPOL_LOW_CPHA_LOW){
				/* CPOL = 0 ; Base value of clock is low
				 * CPHA = 0 ; Data captured on clock's rising edge and data propagated on falling edge
				 * */
				SPI_Config |= SPI_IO_CONTROL_CS0_N_POLARITY;
				SPI_Config |= SPI_IO_CONTROL_CS0_N_SELECT;
			}else if(SPI_mod == CPOL_LOW_CPHA_HIGH){
				/* CPOL = 0 ; Base value of clock is low
				 * CPHA = 1 ; Data captured on clock's falling edge and data propagated on rising edge
				 * */
				SPI_Config |= SPI_IO_CONTROL_CS1_N_POLARITY;
				SPI_Config |= SPI_IO_CONTROL_CS1_N_SELECT;
			}else if(SPI_mod == CPOL_HIGH_CPHA_LOW){
				/* CPOL = 1 ; Base value of clock is high
				 * CPHA = 0 ; Data captured on clock's falling edge and data propagated on rising edge
				 * */
				SPI_Config |= SPI_IO_CONTROL_CS2_N_POLARITY;
				SPI_Config |= SPI_IO_CONTROL_CS2_N_SELECT;
			}else if(SPI_mod == CPOL_HIGH_CPHA_HIGH){
				/* CPOL = 1 ; Base value of clock is high
				 * CPHA = 1 ; Data captured on clock's rising edge and data propagated on falling edge
				 * */
				SPI_Config |= SPI_IO_CONTROL_CS3_N_POLARITY;
				SPI_Config |= SPI_IO_CONTROL_CS3_N_SELECT;
			}
		}
		writel(SPI_Config, SPI_IO_CONTROL(id));
	}

	/* Step 5 in 16.4.4.3:
	 * If mode is master, program the SPI_DEASSERT_WAIT register */
	if(master_slave_mode == Master)
	{
		SPI_Config = 0;
		SPI_Config |= DEASSERT_WAIT;
		writel(SPI_Config, SPI_DEASSERT_WAIT(id));
	}

	SPI_print_status(config);

	// Enable RUN_STATE
	bool retVal = set_QUP_state(config, RUN_STATE);
	if(retVal)
		return TRUE;
	else
		return FALSE;

}


BOOL Krait_SPI_Driver::Xaction_Stop( const SPI_config& Configuration )
{
	// Enable PAUSE_STATE
	bool retVal = set_QUP_state(config, PAUSE_STATE);
	if(retVal)
		return TRUE;
	else
		return FALSE;
}

/*
	Perform 16-bit write or read
	Returns FALSE on error
*/
BOOL Krait_SPI_Driver::Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
    INT32 i;
    INT32 d;
    UINT16 Data16;
	UINT32 chan, mod;
	UINT32 tempWrite;

	UINT32 write_reg;
	UINT32 poll_reg;
	UINT32 read_reg;
	UINT32 ctrl_reg;

    UINT16* Write16         = Transaction.Write16;
    INT32   WriteCount      = Transaction.WriteCount;
    UINT16* Read16          = Transaction.Read16;
    INT32   ReadCount       = Transaction.ReadCount;
    INT32   ReadStartOffset = Transaction.ReadStartOffset;


    // as master, we must always write something before reading or not
	if(WriteCount <= 0)                     { ASSERT(FALSE); return FALSE; }
	if(Write16 == NULL)                     { ASSERT(FALSE); return FALSE; }
	if((ReadCount > 0) && (Read16 == NULL)) { ASSERT(FALSE); return FALSE; }

	//Keep reading and writing until writeCount is 0
    while(WriteCount--)
	{
    	tempWrite = Write16[0];
    	Write16++;
		//Poll for write ready
    	spi_write_bit(tempWrite);
		//Poll for read done
    	Data16 = spi_read_bit();
    	Read16[0] = Data16;
    	Read16++;
	}

    return TRUE;

#if 0
	mod = SPI_CHAN_TO_MOD(Transaction.SPI_mod);
	chan = Transaction.SPI_mod;

	// Find correct register set.
	if ( mod == OMAP3_SPI1) {
	// no change to chan
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxCTRL);
	}

	if ( mod == OMAP3_SPI2) {
	chan = chan - 4;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxCTRL);
	}

	if ( mod == OMAP3_SPI3) {
	chan = chan - 6;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxCTRL);
	}

	if ( mod == OMAP3_SPI4) {
	chan = 8;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxCTRL);
	}

    // as master, we must always write something before reading or not
    if(WriteCount <= 0)                     { ASSERT(FALSE); return FALSE; }
    if(Write16 == NULL)                     { ASSERT(FALSE); return FALSE; }
    if((ReadCount > 0) && (Read16 == NULL)) { ASSERT(FALSE); return FALSE; }


	tempWrite = __raw_readl(ctrl_reg) | OMAP3_SPI_CHxCTRL_EN;
	__raw_writel(tempWrite, ctrl_reg); // Enable the port

    if(ReadCount)
    {
        if((ReadCount + ReadStartOffset) < WriteCount) { ASSERT(FALSE); return FALSE; }

        i = ReadCount + ReadStartOffset;    // we need to read as many bytes as the buffer is long, plus the offset at which we start
        d = ReadCount                  ;    //
    }
    else
    {
        i = WriteCount;
        d = -1;
    }

    // we will use WriteCount to move in the Write16 array
    // so we do no want to go past the end when we will check for
    // WriteCount to be bigger than zero
    WriteCount -= 1;

    while(i--)
    {
        // Write
		tempWrite = Write16[0];

        while ( (__raw_readl(poll_reg) & OMAP3_SPI_CHxSTAT_TXS_EMPTY) == 0) { ; } 	// Poll for write ready
		__raw_writel(tempWrite, write_reg);

		while ( (__raw_readl(poll_reg) & OMAP3_SPI_CHxSTAT_RXS_FULL) == 0)  { ; } 	// Poll for read done
		Data16 = __raw_readl(read_reg) & 0xFFFF;

        // repeat last write word for all subsequent reads
        if(WriteCount)
        {
            WriteCount--;
            Write16++;
        }

        // only save data once we have reached ReadCount-1 portion of words
        if(i < d)
        {
            Read16[0] = Data16;
            Read16++;
        }
    }

	tempWrite = __raw_readl(ctrl_reg) & (~OMAP3_SPI_CHxCTRL_EN);
	__raw_writel(tempWrite, ctrl_reg); // Disable the port
	return TRUE;
#endif

}

/*
	Perform 8-bit write/read.
	Returns FALSE on error
*/
BOOL Krait_SPI_Driver::Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
    INT32 i;
    INT32 d;
    UINT8 Data8;
	UINT32 chan, mod;
	UINT32 tempWrite;

	UINT32 write_reg;
	UINT32 poll_reg;
	UINT32 read_reg;
	UINT32 ctrl_reg;

    UINT8* Write8          = Transaction.Write8;
    INT32  WriteCount      = Transaction.WriteCount;
    UINT8* Read8           = Transaction.Read8;
    INT32  ReadCount       = Transaction.ReadCount;
    INT32  ReadStartOffset = Transaction.ReadStartOffset;

    mod = Transaction.SPI_mod;

    spi_write_byte(*Write8);
    // as master, we must always write something before reading or not
	if(WriteCount <= 0)                     { ASSERT(FALSE); return FALSE; }
	if(Write8 == NULL)                     { ASSERT(FALSE); return FALSE; }
	if((ReadCount > 0) && (Read8 == NULL)) { ASSERT(FALSE); return FALSE; }

	//Keep reading and writing until writeCount is 0
	while(WriteCount--)
	{
		tempWrite = Write8[0];
		Write8++;
		//Poll for write ready
		spi_write_bit(tempWrite);
		//Poll for read done
		Data8 = spi_read_bit();
		Read8[0] = Data8;
		Read8++;
	}

    return TRUE;

#if 0
	mod = SPI_CHAN_TO_MOD(Transaction.SPI_mod);
	chan = Transaction.SPI_mod;

	// Find correct register set.
	if ( mod == OMAP3_SPI1) {
	// no change to chan
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI1_BASE, OMAP3_SPI_CHxCTRL);
	}

	if ( mod == OMAP3_SPI2) {
	chan = chan - 4;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI2_BASE, OMAP3_SPI_CHxCTRL);
	}

	if ( mod == OMAP3_SPI3) {
	chan = chan - 6;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI3_BASE, OMAP3_SPI_CHxCTRL);
	}

	if ( mod == OMAP3_SPI4) {
	chan = 8;
	write_reg = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_TXx);
	poll_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxSTAT);
	read_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_RXx);
	ctrl_reg  = OMAP3_SPI_CHAN_ADDR(chan, OMAP3_SPI4_BASE, OMAP3_SPI_CHxCTRL);
	}


    // as master, we must always write something before reading or not
    if(WriteCount <= 0)                    { ASSERT(FALSE); return FALSE; }
    if(Write8 == NULL)                     { ASSERT(FALSE); return FALSE; }
    if((ReadCount > 0) && (Read8 == NULL)) { ASSERT(FALSE); return FALSE; }


	tempWrite = __raw_readl(ctrl_reg) | OMAP3_SPI_CHxCTRL_EN;
	__raw_writel(tempWrite, ctrl_reg); // Enable the port

    if(ReadCount)
    {
        if((ReadCount + ReadStartOffset) < WriteCount) { ASSERT(FALSE); return FALSE; }

        i = ReadCount + ReadStartOffset;    // we need to read as many bytes as the buffer is long, plus the offset at which we start
        d = ReadCount                  ;    //
    }
    else
    {
        i = WriteCount;
        d = -1;
    }

    // we will use WriteCount to move in the Write8 array
    // so we do no want to go past the end when we will check for
    // WriteCount to be bigger than zero
    WriteCount -= 1;

    // Do actual writes and reads.
    while(i--)
    {
        // Write
		tempWrite = Write8[0];

        while ( (__raw_readl(poll_reg) & OMAP3_SPI_CHxSTAT_TXS_EMPTY) == 0) { ; } 	// Poll for write ready
		__raw_writel(tempWrite, write_reg);

		while ( (__raw_readl(poll_reg) & OMAP3_SPI_CHxSTAT_RXS_FULL) == 0)  { ; } 	// Poll for read done
		Data8 = __raw_readl(read_reg) & 0xFF;

        // repeat last write word for all subsequent reads
        if(WriteCount)
        {
            WriteCount--;
            Write8++;
        }

        // only save data once we have reached ReadCount-1 portion of words
        if(i < d)
        {
            Read8[0] = Data8;
            Read8++;
        }
    }

	tempWrite = __raw_readl(ctrl_reg) & (~OMAP3_SPI_CHxCTRL_EN);
	__raw_writel(tempWrite, ctrl_reg); // Disable the port

	return TRUE;
#endif
}

#endif
/*
	Not used.
*/
void Krait_SPI_Driver::GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi )
{

}
