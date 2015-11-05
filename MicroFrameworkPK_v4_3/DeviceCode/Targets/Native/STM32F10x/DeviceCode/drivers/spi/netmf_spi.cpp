/*
<License information here>

TODO - 
        Map the whole thing to .NET MF GPIO APIS, right now using lower level APIs directly
		Concurrent SPI access, accessing all peripherals
		Extend it for SPI3, currently its only for SPI1 and SPI2
		Put the pins to reset state when we do an XAction_Stop
*/

/*--------- Includes ----------- */

#include "netmf_spi.h"
#include <stm32f10x.h>

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

#define SPIx_NSS	     GPIO_Pin_4 //nss
#define SPIx_PIN_SCK     GPIO_Pin_5 //sck  //purple
#define SPIx_PIN_MISO	 GPIO_Pin_6  //miso //green
#define SPIx_PIN_MOSI    GPIO_Pin_7 //mosi //blue

#define SPIy             SPI2
#define SPIy_CLK         RCC_APB1Periph_SPI2
#define SPIy_GPIO        GPIOB
#define SPIy_GPIO_CLK    RCC_APB2Periph_GPIOB 

#define SPIy_NSS	     GPIO_Pin_12 //nss
#define SPIy_PIN_SCK     GPIO_Pin_13 //sck
#define SPIy_PIN_MISO	 GPIO_Pin_14 //miso
#define SPIy_PIN_MOSI    GPIO_Pin_15 //mosi

/*--------- Global Variables ----------- */
SPI_InitTypeDef SPI_InitStructure;
SPI_TypeDef* SPI_mod;

//indicates the SPI peripheral initialized
int SPI_Initialized[] = {0};

const UINT32 TIMEOUT = 2000; \

#if defined(DEBUG_SAM_SPI)
volatile UINT64 spiSpinWaitCount = 0;
volatile UINT64 spiSpinWaitAvg = 0;
volatile UINT64 spiSpinWaitMax = 0;
volatile UINT64 spiSpinWaitFailureCount = 0;

#define TIMEOUT_WAIT(x) { \
    volatile UINT32 itr_time = 0; \
    while((x) && ++itr_time < TIMEOUT ) {__NOP();}; \
    if(itr_time >= TIMEOUT) { ++spiSpinWaitFailureCount; ASSERT(0); } \
    ++spiSpinWaitCount; \
    if(spiSpinWaitMax < itr_time) { spiSpinWaitMax = itr_time;}; \
    spiSpinWaitAvg = (spiSpinWaitAvg * (spiSpinWaitCount - 1) + itr_time) / spiSpinWaitCount; \
    { \
        static volatile UINT64 localCount = 0; \
        static volatile UINT64 localAvg = 0; \
        static volatile UINT64 localMax = 0; \
        localCount++; \
        if(localMax < itr_time) { localMax = itr_time; }; \
        localAvg = (localAvg * (localCount - 1) + itr_time) / localCount; \
    } \
 }

#else

#define TIMEOUT_WAIT(x) { \
                            volatile int itr_time = 0; \
                            while( (x) && ++itr_time < TIMEOUT ) { __NOP(); } \
                            if( itr_time >= TIMEOUT ) { ASSERT(0); } \
                        }
#endif


/*--------- Internal fucntion prototypes ----------- */

BOOL CPU_SPI_Initialize ()
{
	

	//Set the GPIO pins for the SPI	
	//TODO: Right now we have no way to know if the pins are stolen by any other device
	//before we are restting it. Handle tht in future.

#if defined(DEBUG_SPI)
	hal_printf("SPI Initialize\n");
#endif

	NATIVE_PROFILE_HAL_PROCESSOR_SPI();
	
	//Initialize Clocks for SPI and GPIO
	RCC_Config();
		
	//Initialize NVIC
	NVIC_Config();
		
	//TODO Make sure the GPIO pins are available and if they are put them in safe state	
	//Loop through all the spi peripherals and put there pin in known state and set the value
	//initialized array to 1
	SPI_Initialized[0]= 1; //SPIx
	SPI_Initialized[1]= 1; //SPIy
    
}

void CPU_SPI_Uninitialize()
{
	/* Reset the SPI peripherals */
#if defined(DEBUG_SPI)
	hal_printf("SPI Uninitialize\n");
#endif

	//Enable and Disable Reset States for all the SPI peripherals
	
	//SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);    
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);

	//SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);

	//SPI3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);
	
}


void CPU_SPI_Uninitialize(SPI_CONFIGURATION config)
{
#if defined(DEBUG_SPI)
    hal_printf("SPI Uninitialize #\n");
#endif

    if(config.SPI_mod == SPIBUS1)
    {
        //SPI1
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
    }
    else if(config.SPI_mod == SPIBUS2)
    {
        //SPI2
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
    }
}


void CPU_SPI_Enable(SPI_CONFIGURATION config)
{

	if(config.SPI_mod == SPIBUS1)
	{

		//RCC_PCLK2Config(RCC_HCLK_Div2);
		RCC_APB2PeriphClockCmd(SPIx_GPIO_CLK | SPIx_CLK, ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;


		/* Configure SPIx pins: SCK, MISO and MOSI */
		GPIO_InitStructure.GPIO_Pin = SPIx_PIN_SCK | SPIx_PIN_MISO | SPIx_PIN_MOSI | SPIx_NSS;
		GPIO_ConfigurePin(SPIx_GPIO, GPIO_InitStructure.GPIO_Pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);


		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = 0;
		SPI_InitStructure.SPI_CPHA = 0;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;

		SPI_Init(SPIx, &SPI_InitStructure);
		SPI_SSOutputCmd(SPIx, ENABLE);
		SPI_Cmd(SPIx, ENABLE);
	}
	else if(config.SPI_mod == SPIBUS2)
	{

		//RCC_PCLK1Config(RCC_HCLK_Div2);
		//RCC_PCLK2Config(RCC_HCLK_Div2);
		RCC_APB2PeriphClockCmd(SPIy_GPIO_CLK, ENABLE);
		RCC_APB1PeriphClockCmd(SPIy_CLK, ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;


		/* Configure SPIy pins: SCK, MOSI */
		GPIO_InitStructure.GPIO_Pin = SPIy_PIN_SCK | SPIy_PIN_MOSI | SPIy_NSS;
		GPIO_ConfigurePin(SPIy_GPIO, GPIO_InitStructure.GPIO_Pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);

		// SPI MISO pin
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		GPIO_ConfigurePin(GPIOB, GPIO_InitStructure.GPIO_Pin, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = 0;
		SPI_InitStructure.SPI_CPHA = 0;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;

		SPI_Init(SPIy, &SPI_InitStructure);
		SPI_SSOutputCmd(SPIy, ENABLE);
		SPI_Cmd(SPIy, ENABLE);

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
			HARD_BREAKPOINT();
			break;
	}


	TIMEOUT_WAIT( SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_TXE) == RESET ); // Spin until TX ready

	SPI_I2S_SendData(SPI_mod, data);

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
			HARD_BREAKPOINT();
			break;
	}
	TIMEOUT_WAIT(SPI_I2S_GetFlagStatus(SPI_mod, (uint16_t )SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI_mod);
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
					HARD_BREAKPOINT();
					break;
		}

	SPI_I2S_SendData(SPI_mod, data);
	TIMEOUT_WAIT(SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI_mod);

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
				HARD_BREAKPOINT();
				break;
	}

	TIMEOUT_WAIT(SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_RXNE) == RESET);
	read_data = SPI_I2S_ReceiveData(SPI_mod);
	//TIMEOUT_WAIT(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_mod, data);

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

	SPI_Init(SPI_mod, &SPI_InitStructure);	
	SPI_SSOutputCmd(SPI_mod, ENABLE);
	SPI_Cmd(SPI_mod, ENABLE); 
	
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

	SPI_I2S_DeInit(SPI_mod);
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
		TIMEOUT_WAIT(SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_TXE) == RESET);
		if (WriteCount != 0)
		{
		    SPI_I2S_SendData(SPI_mod, Write16[0]);
		}
		else
		{			
			SPI_I2S_SendData(SPI_mod, 0);
		}

		//wait till we are ready to recieve		
		TIMEOUT_WAIT(SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_RXNE) == RESET);
		Data16 = SPI_I2S_ReceiveData(SPI_mod);		
		
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
		TIMEOUT_WAIT(SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPI_mod, Write8[0]);
				
		//if(WriteCount != 0)
		//{
		//	//SPI_I2S_SendData(SPI_mod, (UINT16)Write8[0]); //TODO:	Need to check on the cast, just casting as of now
		//	SPI_I2S_SendData(SPI_mod, Write8[0]); //TODO:	Need to check on the cast, just casting as of now
		//}
		//else
		//{						
		//	SPI_I2S_SendData(SPI_mod, (UINT16)0);				
		//}

		// wait for data reception
		TIMEOUT_WAIT(SPI_I2S_GetFlagStatus(SPI_mod, SPI_I2S_FLAG_RXNE) == RESET);
		Data8 = (UINT8) SPI_I2S_ReceiveData(SPI_mod);	

		
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
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  /*
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
  */
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
          GPIO_Init(SPIx_GPIO, &GPIO_InitStructure);
		  break;

	  case 1:
		  /* Configure SPIy pins: SCK, MISO and MOSI */
		  GPIO_InitStructure.GPIO_Pin = SPIy_NSS | SPIy_PIN_SCK | SPIy_PIN_MISO | SPIy_PIN_MOSI;
          GPIO_Init(SPIy_GPIO, &GPIO_InitStructure); 
		  SPI_SSOutputCmd(SPIy, DISABLE);
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
  /* PCLK2 = HCLK/2 */
  //RCC_PCLK2Config(RCC_HCLK_Div2);
  
  /* Enable SPIx clock and GPIO clock for SPIx and SPIy */
  RCC_APB2PeriphClockCmd(SPIx_GPIO_CLK | SPIy_GPIO_CLK | SPIx_CLK, ENABLE);

  /* Enable SPIx Periph clock */
  RCC_APB1PeriphClockCmd(SPIy_CLK, ENABLE);
  
}

void NVIC_Config()
{
	/*
	TODO yet
	*/	
}
