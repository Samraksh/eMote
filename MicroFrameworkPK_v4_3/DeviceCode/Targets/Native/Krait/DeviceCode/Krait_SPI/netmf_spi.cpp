/*
<License information here>

TODO - 
        Map the whole thing to .NET MF GPIO APIS, right now using lower level APIs directly
		Concurrent SPI access, accessing all peripherals
		Extend it for SPI3, currently its only for SPI1 and SPI2
		Put the pins to reset state when we do an XAction_Stop
*/

/*--------- Includes ----------- */

#include <netmf_spi.h>

/*--------- Macros ----------- */

#undef DEBUG_SPI
#undef FIRST_BIT_LSB 
#undef NSS_SOFTWARE_MODE 

#define MAX_SPI_PORTS 3


/* SPI peripheral Configuration */
#define SPIx             SPI1
#define SPIx_CLK         RCC_APB2Periph_SPI1
#define SPIx_GPIO        GPIOA  
#define SPIx_GPIO_CLK    RCC_APB2Periph_GPIOA 

#define SPIx_NSS	     52 //nss
#define SPIx_PIN_SCK     58 //sck  //purple
#define SPIx_PIN_MISO	 53  //miso //green
#define SPIx_PIN_MOSI    55 //mosi //blue

#define SPIy             SPI2
#define SPIy_CLK         RCC_APB1Periph_SPI2
#define SPIy_GPIO        GPIOB
#define SPIy_GPIO_CLK    RCC_APB2Periph_GPIOB 

#define SPIy_NSS	     52 //nss
#define SPIy_PIN_SCK     58 //sck
#define SPIy_PIN_MISO	 53 //miso
#define SPIy_PIN_MOSI    55 //mosi

/*--------- Global Variables ----------- */
SPI_InitTypeDef SPI_InitStructure;
SPI_TypeDef* SPI_mod;

//indicates the SPI peripheral initialized
int SPI_Initialized[] = {0};

/*--------- Internal fucntion prototypes ----------- */

#define GPT_ENABLE_EN                     1
/*const UINT8 miso_pin = 53;
const UINT8 clk_pin = 58;
const UINT8 mosi_pin = 55;
const UINT8 sleep_pin = 51;
const UINT8 intr_pin = 52;*/

#define clk_pin 58
#define mosi_pin 55
#define miso_pin 53
#define intr_pin 52
#define sleep_pin 51
#define cs_pin 51

#define GPIO_SET_OUT_VAL(gpio, val)	(gpio_set(gpio, (val)<<1))

/*void udelay(unsigned usecs)
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
}*/

static void gpio_set(UINT32 gpio, UINT32 dir)
{
	UINT32 *addr = (UINT32 *)GPIO_IN_OUT_ADDR(gpio);
	writel(dir, addr); // second bit controls GPIO
	return;
}

/*static UINT32 gpio_get(UINT32 gpio)
{
	UINT32 *addr = (UINT32 *)GPIO_IN_OUT_ADDR(gpio);
	return readl(addr);
}*/

static UINT8 sam_fpga_spi(UINT8 data) {
    UINT8 bit = 0x80;
    UINT8 ret = 0;
    const UINT8 d = 10;

    // MISO input
    unsigned int *addr = (unsigned int *)GPIO_IN_OUT_ADDR(miso_pin);

    while (bit) {
        gpio_set(mosi_pin, (data&bit) ? 2 : 0);     // Set output
        udelay(d);
        ret |= ( readl(addr)&1 ); // Latch input
        if(bit>1)
            ret = ret << 1;
        bit = bit >> 1;
        gpio_set(clk_pin, 2); // clock high
        udelay(d);
        gpio_set(clk_pin, 0); // clock low
    }
    //dprintf(INFO, "sent 0x%X got SPI 0x%X\n", data, ret);
    return ret;
}



static void gpio_tlmm_config(UINT32 gpio, UINT8 func,
		      UINT8 dir, UINT8 pull,
		      UINT8 drvstr, UINT32 enable)
{
	UINT32 val = 0;
	val |= pull;
	val |= func << 2;
	val |= drvstr << 6;
	val |= (enable & dir) << 9;
	UINT32 *addr = (UINT32 *)GPIO_CONFIG_ADDR(gpio);
	writel(val, addr);
	return;
}



BOOL CPU_SPI_Initialize ()
{
	

	//Set the GPIO pins for the SPI	
	//TODO: Right now we have no way to know if the pins are stolen by any other device
	//before we are restting it. Handle tht in future.

#if defined(DEBUG_SPI)
	hal_printf("SPI Initialize\n");
#endif

	//AnanthAtSamraksh
	/*NATIVE_PROFILE_HAL_PROCESSOR_SPI();
	
	//Initialize Clocks for SPI and GPIO
	RCC_Config();
		
	//Initialize NVIC
	NVIC_Config();
		
	//TODO Make sure the GPIO pins are available and if they are put them in safe state	
	//Loop through all the spi peripherals and put there pin in knwon state and set the value
	//initialized array to 1
	SPI_Initialized[0]= 1; //SPIx
	SPI_Initialized[1]= 1; //SPIy*/
	//AnanthAtSamraksh

	GPIO_SET_OUT_VAL(clk_pin, 0);
	GPIO_SET_OUT_VAL(mosi_pin, 0);
	GPIO_SET_OUT_VAL(cs_pin, 1);

	// Set SDIO pins into SPI comm mode
	gpio_tlmm_config(clk_pin, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE); // SCLK (GPIO1)
	gpio_tlmm_config(mosi_pin, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE); // MOSI (GPIO2)
	gpio_tlmm_config(miso_pin, 0, GPIO_INPUT,  GPIO_NO_PULL, GPIO_2MA, GPIO_DISABLE); // MISO (GPIO3)
	gpio_tlmm_config(intr_pin, 0, GPIO_INPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_DISABLE); // INTERRUPT (GPIO4)
	//gpio_tlmm_config(sleep_pin, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE); // SLEEP (GPIO5)
	gpio_tlmm_config(cs_pin, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE); // CS (GPIO5)


    
}

void CPU_SPI_Uninitialize()
{
	/* Reset the SPI peripherals */
#if defined(DEBUG_SPI)
	hal_printf("SPI Uninitialize\n");
#endif

	//Enable and Disable Reset States for all the SPI peripherals
	
#if 0
	//SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);    
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);

	//SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);

	//SPI3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);
#endif
	
}



void CPU_SPI_Enable(SPI_CONFIGURATION config)
{
	if(config.SPI_mod == SPIBUS1)
	{

		/*RCC_PCLK2Config(RCC_HCLK_Div2);
		RCC_APB2PeriphClockCmd(SPIx_GPIO_CLK | SPIx_CLK, ENABLE);*/

		GPIO_InitTypeDef GPIO_InitStructure;


		/* Configure SPIx pins: SCK, MISO and MOSI */
		GPIO_InitStructure.GPIO_Pin = SPIx_PIN_SCK | SPIx_PIN_MISO | SPIx_PIN_MOSI | SPIx_NSS;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		//GPIO_Init(SPIx_GPIO, &GPIO_InitStructure);


		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = 0;
		SPI_InitStructure.SPI_CPHA = 0;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;

		SPI_Init(SPIx, &SPI_InitStructure);
		//SPI_SSOutputCmd(SPIx, ENABLE);
		//SPI_Cmd(SPIx, ENABLE);
	}
	else if(config.SPI_mod == SPIBUS2)
	{

		/*RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div2);
		RCC_APB2PeriphClockCmd(SPIy_GPIO_CLK, ENABLE);
		RCC_APB1PeriphClockCmd(SPIy_CLK, ENABLE);*/

		GPIO_InitTypeDef GPIO_InitStructure;


		/* Configure SPIy pins: SCK, MOSI */
		GPIO_InitStructure.GPIO_Pin = SPIy_PIN_SCK | SPIy_PIN_MOSI | SPIy_NSS;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		//GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);

		  // SPI MISO pin
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		 //GPIO_Init(GPIOB, &GPIO_InitStructure);

		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = 0;
		SPI_InitStructure.SPI_CPHA = 0;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;

		SPI_Init(SPIy, &SPI_InitStructure);
		//SPI_SSOutputCmd(SPIy, ENABLE);
		//SPI_Cmd(SPIy, ENABLE);

	}
}




void CPU_SPI_GetPins (UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi)
{
	/* Return the GPIO pin for selected SPI */

#if defined(DEBUG_SPI)
	hal_printf("SPI GetPins\n");
#endif

	switch (spi_mod)
	{
	case 0:
		msk = GPIO_Pin_7;
		miso = GPIO_Pin_6;
		mosi = GPIO_Pin_5;
		break;

	case 1:
		msk = GPIO_Pin_15;
		miso = GPIO_Pin_14;
		mosi = GPIO_Pin_13;
		break;
	
	default:
		break;

	}
}

UINT32 CPU_SPI_PortsCount()
{
	/* Returns the total no. of SPI peripherals */
#if defined(DEBUG_SPI)
	hal_printf("SPI PortsCount\n");
#endif

	return (UINT32)MAX_SPI_PORTS;
}


// Nived.Sivadas - Scenarios where you want to just write a single byte and leave the reading part out
BOOL CPU_SPI_WriteByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{
	switch(Configuration.SPI_mod)
	{
		case SPIBUS1:
			SPI_mod	= SPIx;
			break;
		case SPIBUS2:
			SPI_mod = SPIy;
			break;
		default:
			// Die Here
			while(1);
			break;
	}

	//SPI_I2S_SendData(SPI_mod, data);
	sam_fpga_spi(data);

	return TRUE;

}

UINT8 CPU_SPI_ReadByte(const SPI_CONFIGURATION&  Configuration)
{

	switch(Configuration.SPI_mod)
	{
		case SPIBUS1:
			SPI_mod = SPIx;
			break;
		case SPIBUS2:
			SPI_mod = SPIy;
			break;
		default:
			// Die Here
			while(1);
			break;
	}

	return sam_fpga_spi(0);

	////while (SPI_I2S_GetFlagStatus(SPI_mod, (UINT16 )SPI_I2S_FLAG_RXNE) == RESET);
	////return SPI_I2S_ReceiveData(SPI_mod);
}

UINT8 CPU_SPI_WriteReadByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{
	switch(Configuration.SPI_mod)
	{
			case SPIBUS1:
				SPI_mod = SPIx;
				break;
			case SPIBUS2:
				SPI_mod = SPIy;
				break;
			default:
				// Die Here
				while(1);
				break;
	}

	sam_fpga_spi(data);
	return sam_fpga_spi(0);
	//return gpio_get(miso_pin);
	////SPI_I2S_SendData(SPI_mod, data);
	////while (SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_RXNE) == RESET);
	////return SPI_I2S_ReceiveData(SPI_mod);

}

UINT8 CPU_SPI_ReadWriteByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{
	UINT8 read_data = 0;

	switch(Configuration.SPI_mod)
	{
			case SPIBUS1:
				SPI_mod = SPIx;
				break;
			case SPIBUS2:
				SPI_mod = SPIy;
				break;
			default:
				// Die Here
				while(1);
				break;
	}

	read_data = sam_fpga_spi(0);
	sam_fpga_spi(data);
	//return gpio_get(miso_pin);
	////while (SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_RXNE) == RESET);
	////read_data = SPI_I2S_ReceiveData(SPI_mod);
	////while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	//AnanthAtSamraksh
	////SPI_I2S_SendData(SPI_mod, data);

	return read_data;
}

BOOL CPU_SPI_nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{

	/* Performs a read/write operation on 16-bit word data.  */
	
#if defined(DEBUG_SPI)
	hal_printf("SPI nWrite16_nRead16\n");
#endif

	if(SPI_Initialized[Configuration.SPI_mod] == 0)
	{
#if defined(DEBUG_SPI)
		hal_printf("SPI peripheral uninitialized\n");
#endif
		return false;
	}

	if(Configuration.SPI_mod > MAX_SPI_PORTS)
	{
#if defined(DEBUG_SPI)
		hal_printf("Ports specified exceeds total number of ports\n");
#endif
		return false;
	}

	if(!CPU_SPI_Xaction_Start(Configuration))
	{
#if defined(DEBUG_SPI)
		hal_printf("Could not start X\n");
#endif
		return false;
	}

	else
	{
		SPI_XACTION_16 Transaction;

		Transaction.Read16 = Read16;
		Transaction.ReadCount = ReadCount;
		Transaction.ReadStartOffset = ReadStartOffset;
		Transaction.Write16 = Write16;
        Transaction.WriteCount = WriteCount;
        Transaction.SPI_mod = Configuration.SPI_mod;
        Transaction.BusyPin = Configuration.BusyPin;

		if(!CPU_SPI_Xaction_nWrite16_nRead16(Transaction))
		{
			return FALSE;
		}		
		
	}	

	return CPU_SPI_Xaction_Stop(Configuration);
	
}

BOOL CPU_SPI_nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{

	/* Performs a read/write operation on 8-bit word data.  */
	
#if defined(DEBUG_SPI)
	hal_printf("SPI nWrite8_nRead8\n");
#endif

	if(SPI_Initialized[Configuration.SPI_mod] == 0)
	{
#if defined(DEBUG_SPI)
		hal_printf("SPI peripheral uninitialized\n");
#endif
		return false;
	}

	if(Configuration.SPI_mod > MAX_SPI_PORTS)
	{
#if defined(DEBUG_SPI)
		hal_printf("Ports specified exceeds total number of ports\n");
#endif
		return false;
	}

	if(!CPU_SPI_Xaction_Start(Configuration))
	{
#if defined(DEBUG_SPI)
		hal_printf("Could not start X\n");
#endif
		return false;
	}

	else
	{
		SPI_XACTION_8 Transaction;

		Transaction.Read8 = Read8;
		Transaction.ReadCount = ReadCount;
		Transaction.ReadStartOffset = ReadStartOffset;
		Transaction.Write8 = Write8;
        Transaction.WriteCount = WriteCount;
        Transaction.SPI_mod = Configuration.SPI_mod;
        Transaction.BusyPin = Configuration.BusyPin;

		if(!CPU_SPI_Xaction_nWrite8_nRead8(Transaction))
		{
			return FALSE;
		}		
		
	}	

	return CPU_SPI_Xaction_Stop(Configuration);
	
	
}

BOOL CPU_SPI_Xaction_Start( const SPI_CONFIGURATION& Configuration )
{
	/*

	Sets up and starts a read/write operation using the specified configuration. 


	*/

	//Set pins to correct state	
	GPIO_Config(Configuration);
	
	//Initialize default SPI Struct, this configures SPIx as Master
	SPI_StructInit(Configuration);

	//Initialize the SPI 1
	switch(Configuration.SPI_mod)
	{
	case 0:
		SPI_mod = SPIx;		
		break;
	case 1:
		SPI_mod = SPIy;		
		break;
	default:
#if defined(DEBUG_SPI)
		hal_printf("Should not be here");
#endif
		break;
	}

	//SPI_Init(SPI_mod, &SPI_InitStructure);
	//AnanthAtSamraksh
#if 0
	SPI_SSOutputCmd(SPI_mod, ENABLE);
	SPI_Cmd(SPI_mod, ENABLE);
#endif
	//AnanthAtSamraksh
	
	if(Configuration.DeviceCS != GPIO_PIN_NONE && Configuration.DeviceCS != SPIx_NSS && Configuration.DeviceCS != SPIy_NSS )	// Use GPIO for the function of CS pin to keep it always active during the whole read/write access operation
	{
		CPU_GPIO_EnableOutputPin(Configuration.DeviceCS, Configuration.CS_Active);
    }

	if(Configuration.CS_Setup_uSecs)
	{
		HAL_Time_Sleep_MicroSeconds_InterruptEnabled(Configuration.CS_Setup_uSecs);        
	}

	return true;
	
}

BOOL CPU_SPI_Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{
	/*
	
	Completes a read/write operation using the specified configuration. 
	
	*/

	if(Configuration.CS_Hold_uSecs)
    {
		HAL_Time_Sleep_MicroSeconds_InterruptEnabled(Configuration.CS_Hold_uSecs);
    }

	if(Configuration.DeviceCS != GPIO_PIN_NONE && Configuration.DeviceCS != SPIx_NSS && Configuration.DeviceCS != SPIy_NSS )	// Use GPIO for the function of CS pin to keep it always active during the whole read/write access operation
	{
		CPU_GPIO_SetPinState(Configuration.DeviceCS, !Configuration.CS_Active);	
	}	

	//FIXME Reset the state of pins here

	//AnanthAtSamraksh
	//SPI_I2S_DeInit(SPI_mod);

	return true;
}

BOOL CPU_SPI_Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
	/*
	Performs a low-level read/write operation on 16-bit word data
	*/
	UINT16 Data16;
    INT32 i;
    INT32 d;	

	UINT16 *Write16 = Transaction.Write16;
    INT32 WriteCount = Transaction.WriteCount;
    UINT16 *Read16 = Transaction.Read16;
    INT32 ReadCount = Transaction.ReadCount;
    INT32 ReadStartOffset = Transaction.ReadStartOffset;

	// check for boudary error conditions

    if(WriteCount <= 0)
    {
        //ASSERT(FALSE); commenting for now as I am not sure of the handling
        return FALSE;
    }

    if(Write16 == NULL)
    {
        //ASSERT(FALSE);
        return FALSE;
    }

	if((ReadCount > 0) && (Read16 == NULL))
    {
        //ASSERT(FALSE);
        return FALSE;
    }

    if(ReadCount)
    {
        i = ReadCount + ReadStartOffset; // we need to read as many bytes as the buffer is long, plus the offset at which we start
        d = ReadCount; // 
    }
    else
    {
        i = WriteCount;
        d =  - 1;
    }
	
    // we will use WriteCount to move in the Write16 array
    // so we do no want to go past the end when we will check for 
    // WriteCount to be bigger than zero
    
	if(WriteCount != 1)
		WriteCount -= 1; //FIXME Check this..


	while(i--)
    {
		//while (SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_TXE) == RESET);
		if (WriteCount != 0)
		{
			sam_fpga_spi(Write16[0]);
		    //SPI_I2S_SendData(SPI_mod, Write16[0]);
		}
		else
		{			
			sam_fpga_spi(0);
			//SPI_I2S_SendData(SPI_mod, 0);
		}

		//wait till we are ready to recieve		
		//while (SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_RXNE) == RESET);
		//Data16 = SPI_I2S_ReceiveData(SPI_mod);
		
	    // repeat last write word for all subsequent reads
        if(WriteCount)
        {
            WriteCount--;
			Write16++;
            
        }

        // only save data once we have reached ReadCount-1 portion of words
        if(i < d)
        {
			if(Read16 != NULL)
			{
				Read16[0] = Data16;
				Read16++;
			}            
        }
    }

	return true;
}

BOOL CPU_SPI_Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
	/*
	Performs a low-level read/write operation on 8-bit word data
	*/
	INT32 i;
    INT32 d;
    UINT8 Data8;	
	
	UINT8 *Write8 = Transaction.Write8;
    INT32 WriteCount = Transaction.WriteCount;
    UINT8 *Read8 = Transaction.Read8;
    INT32 ReadCount = Transaction.ReadCount;
    INT32 ReadStartOffset = Transaction.ReadStartOffset;	

	if(WriteCount <= 0)
    {		
        //ASSERT(FALSE);
        return FALSE;
    }
    if(Write8 == NULL)
    {		
        //ASSERT(FALSE);
        return FALSE;
    }
    if((ReadCount > 0) && (Read8 == NULL))
    {			
        //ASSERT(FALSE);
        return FALSE;
    }

    if(ReadCount)
    {
        i = ReadCount + ReadStartOffset; // we need to read as many bytes as the buffer is long, plus the offset at which we start
        d = ReadCount; // 
    }
    else
    {
        i = WriteCount;
        d =  - 1;
    }

    // we will use WriteCount to move in the Write8 array
    // so we do no want to go past the end when we will check for 
    // WriteCount to be bigger than zero

	if(WriteCount != 1)
		WriteCount -= 1; //FIXME Check this..


    // Start transmission 
    while(i--)
    {		
		//while (SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_TXE) == RESET);
		//SPI_I2S_SendData(SPI_mod, Write8[0]);
				
		if(WriteCount != 0)
		{
			sam_fpga_spi(Write8[0]);

		//	//SPI_I2S_SendData(SPI_mod, (UINT16)Write8[0]); //TODO:	Need to check on the cast, just casting as of now
		//	SPI_I2S_SendData(SPI_mod, Write8[0]); //TODO:	Need to check on the cast, just casting as of now
		}
		else
		{
			sam_fpga_spi(0);
			//	SPI_I2S_SendData(SPI_mod, (UINT16)0);
		}

		// wait for data reception
		//while (SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_RXNE) == RESET);
		//Data8 = (UINT8) SPI_I2S_ReceiveData(SPI_mod);

		
    	sam_fpga_spi(Write8[0]);
		// repeat last write word for all subsequent reads
        if(WriteCount)
        {
            WriteCount--;
			Write8++;		
        }

        // only save data once we have reached ReadCount-1 portion of words
        if(i < d)
        {
			if(Read8 != NULL)
			{
				Read8[0] = Data8;
				//if(Read8[0])
					//LED_BLUE();
				Read8++;
			}
        }
    }

	return true;
}

void SPI_StructInit(const SPI_CONFIGURATION& Configuration)
{

  /* Initialize the SPI_Direction member */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //this is not specified by the framework, we need it to initialize
  /* initialize the SPI_Mode member */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //this is not specified by the framework, we need it to initialize
  /* initialize the SPI_DataSize member */
  /* Initialize the SPI_CPOL member */
  SPI_InitStructure.SPI_CPOL = Configuration.MSK_IDLE; //SPI_CPOL_Low;
  /* Initialize the SPI_CPHA member */
  SPI_InitStructure.SPI_CPHA = Configuration.MSK_SampleEdge; //SPI_CPHA_2Edge;
  /* Initialize the SPI_NSS member */
  if(Configuration.MD_16bits == false)
  {
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
  }
  else
  {
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  }
#if defined(NSS_SOFTWARE_MODE)
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
#else 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard; 
#endif
  
  /* Initialize the SPI_BaudRatePrescaler member */
  //FIXME: Right now we are pre-scaling map it to configuration clock
  switch (Configuration.Clock_RateKHz)
  {
  case 2:
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	  break;
  case 4:
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	  break;
  case 8:
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	  break;
  case 16:
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	  break;
  case 32:
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	  break;
  case 64:
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	  break;
  case 128:
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	  break;
  case 256:
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	  break;
  default:
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	  break;
  }   
  /* Initialize the SPI_FirstBit member */
#if defined(FIRST_BIT_LSB)
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
#else
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
#endif
  /* Initialize the SPI_CRCPolynomial member */
  SPI_InitStructure.SPI_CRCPolynomial = 7;	


}

void GPIO_Config(const SPI_CONFIGURATION& Configuration)
{
  GPIO_InitTypeDef GPIO_InitStructure;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

  switch(Configuration.SPI_mod)
  {
	  case 0:
		  /* Configure SPIx pins: SCK, MISO and MOSI */
		  GPIO_InitStructure.GPIO_Pin = SPIx_PIN_SCK | SPIx_PIN_MISO | SPIx_PIN_MOSI;
		  //AnanthAtSamraksh
          //GPIO_Init(SPIx_GPIO, &GPIO_InitStructure);
		  break;

	  case 1:
		  /* Configure SPIy pins: SCK, MISO and MOSI */
		  GPIO_InitStructure.GPIO_Pin = SPIy_NSS | SPIy_PIN_SCK | SPIy_PIN_MISO | SPIy_PIN_MOSI;
		  //AnanthAtSamraksh
          //GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);
		  //SPI_SSOutputCmd(SPIy, DISABLE);
		  break;

	  default:
#if defined(SPI_DEBUG)
		  hal_printf("Should not be here");
#endif
		  break;
  }

}

void RCC_Config()
{
  //AnanthAtSamraksh
#if 0
  /* PCLK2 = HCLK/2 */
  RCC_PCLK2Config(RCC_HCLK_Div2);
  
  /* Enable SPIx clock and GPIO clock for SPIx and SPIy */
  RCC_APB2PeriphClockCmd(SPIx_GPIO_CLK | SPIy_GPIO_CLK | SPIx_CLK, ENABLE);

  /* Enable SPIx Periph clock */
  RCC_APB1PeriphClockCmd(SPIy_CLK, ENABLE);
#endif
  //AnanthAtSamraksh
  
}

void NVIC_Config()
{
	/*
	TODO yet
	*/	
}
