#include "mxc_config.h"
#include "clkman.h"
#include "ioman.h"
#include "netmf_spi.h"

//--------- Macros -----------

#undef DEBUG_SPI
#undef FIRST_BIT_LSB 
#undef NSS_SOFTWARE_MODE 

/*#define MX25_BAUD           48000000    // 48 MHz maximum, 20 kHz minimum
#define MX25_ADDR           0x0
#define MX25_SPIM_WIDTH     SPIM_WIDTH_4
#define MX25_EXP_ID         0xc22538
*/

bool spiDriverInit=false;


inline BOOL CheckSlaves(GPIO_PIN pin){
	BOOL ret=FALSE;
	for (uint i=0 ; i<SPI_SLAVE_COUNT; i++) {
		if(SPI_SLAVES[i]==pin){
			return TRUE;
		}
	}

	return ret;
}

inline mxc_spim_regs_t* GetSPIReg (SPI_MASTER _mport){
	mxc_spim_regs_t* ret;
	switch(_mport){
		case SPIPort_M0: ret= MXC_SPIM0; break;
		case SPIPort_M1: ret= MXC_SPIM1; break;
		case SPIPort_M2: ret= MXC_SPIM2; break;
		case SPIPort_B: ret= MXC_SPIB; break;
		default:
	}
	return ret;
}

inline uint32_t GetSSel(uint32_t slaveNo, bool high){
	uint32_t ret;
	if(high){
		if(slaveNo==0) ret=SPIM_SSEL0_HIGH;
		if(slaveNo==1) ret=SPIM_SSEL1_HIGH;
		if(slaveNo==2) ret=SPIM_SSEL2_HIGH;
		if(slaveNo==3) ret=SPIM_SSEL3_HIGH;
		if(slaveNo==4) ret=SPIM_SSEL4_HIGH;
	}else {
		if(slaveNo==0) ret=SPIM_SSEL0_LOW;
		if(slaveNo==1) ret=SPIM_SSEL1_LOW;
		if(slaveNo==2) ret=SPIM_SSEL2_LOW;
		if(slaveNo==3) ret=SPIM_SSEL3_LOW;
		if(slaveNo==4) ret=SPIM_SSEL4_LOW;
	}
	return ret;
}

inline


/*--------- Internal fucntion prototypes ----------- */

BOOL CPU_SPI_Initialize ()
{
	

#if defined(DEBUG_SPI)
	hal_printf("SPI Initialize\n");
#endif

	NATIVE_PROFILE_HAL_PROCESSOR_SPI();
	
	spiDriverInit=TRUE;
}

void CPU_SPI_Uninitialize()
{
	/* Reset the SPI peripherals */
#if defined(DEBUG_SPI)
	hal_printf("SPI Uninitialize\n");
#endif
	
	//shutdown all SPI masters
	SPIM_Shutdown(MXC_SPIM0);
	SPIM_Shutdown(MXC_SPIM1);
	SPIM_Shutdown(MXC_SPIM2);
	SPIM_Shutdown(MXC_SPIB);

	spiDriverInit=FALSE;
}

void CPU_SPI_Uninitialize(SPI_CONFIGURATION config, uint8_t  _mport)
{
#if defined(DEBUG_SPI)
			hal_printf("SPI_Uninitialize: \n");
#endif
	mxc_spim_regs_t spi_reg=GetSPIReg(_mport);
	if(spi_reg)SPIM_Shutdown(spi_reg);
}

BOOL CPU_SPI_Enable(SPI_CONFIGURATION config, uint8_t _mport)
{
	if(!spiDriverInit) CPU_SPI_Initialize();

	//Initialize the SPIM
	spim_cfg_t cfg;
	cfg.mode =config.SPI_mod;
	cfg.ssel_pol = GetSSel(0,config.CS_Active) ;	//We will always use slave 0 on a master for now.
	cfg.baud = config.Clock_RateKHz * 1000;

	sys_cfg_spim_t sys_cfg;

	// MX25 IO Config                  core I/O, ss0, ss1, ss2, quad, fast I/O

	//set clock to auto
	sys_cfg.clk_scale = CLKMAN_SCALE_AUTO;

	mxc_spim_regs_t *spi_reg;
	//Write the below IOMAN_SPI carefully based on how many slaves are connected to each spi master
	//right now we simply request the slave 0, assuming atleast one slave is connected

	switch (_mport) {
		case SPIPort_M0:
		{
			spi_reg= MXC_SPIM0;
													//io, ss0, ss1, ss2, ss3, ss4, q, f
			sys_cfg.io_cfg = (ioman_cfg_t)IOMAN_SPIM0(1,   1,  	0,  0,   0,    0,  0, 0);
			break;
		}
		case SPIPort_M1:
		{
			spi_reg= MXC_SPIM1;
													//io, ss0, ss1, ss2, q, f
			sys_cfg.io_cfg = (ioman_cfg_t)IOMAN_SPIM1(1,   1,  	0,  0,   0, 0);
			break;
		}
		case SPIPort_M2:
		{
			spi_reg= MXC_SPIM2;
													//io, ss0, ss1, ss2, sr0, sr1, q, f
			sys_cfg.io_cfg = (ioman_cfg_t)IOMAN_SPIM2(1,   1,  	0,  0,   0,    0,  0, 0);
			break;
		}
		case SPIPort_B:
		{
			spi_reg= MXC_SPIB;
													//io,  q, f
			sys_cfg.io_cfg = (ioman_cfg_t)IOMAN_SPIMB(1,   0, 0);
			break;
		}
		default:
#if defined(DEBUG_SPI)
			hal_printf("SPI_Enable: Unknown SPI Port/Master #\n");
#endif
			return FALSE;
	}

	if((error = SPIM_Init(spi_reg, &cfg, &sys_cfg)) != E_NO_ERROR) {
		hal_printf("Error initializing SPIM %d\n", error);
		return FALSE;
	}

	hal_printf("SPIM Initialized\n");
	return TRUE;
}


void CPU_SPI_GetPins (UINT32 _mport, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi)
{
	/* Return the GPIO pin for selected SPI */

#if defined(DEBUG_SPI)
	hal_printf("SPI GetPins\n");
#endif

	switch (_mport) {
		case SPIPort_M0:
			msk = GPIO_Pin_7;
			miso = GPIO_Pin_6;
			mosi = GPIO_Pin_5;
			break;
		case SPIPort_M1:
			msk = GPIO_Pin_15;
			miso = GPIO_Pin_14;
			mosi = GPIO_Pin_13;
			break;
		case SPIPort_M2:
			msk = GPIO_Pin_15;
			miso = GPIO_Pin_14;
			mosi = GPIO_Pin_13;
			break;
		case SPIPort_B:
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

	return (UINT32)SPI_MAX_PORTS;
}


// Nived.Sivadas - Scenarios where you want to just write a single byte and leave the reading part out
BOOL CPU_SPI_WriteByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{
	ASSERT(CheckSlaves(Configuration.DeviceCS));

	return TRUE;

}

UINT8 CPU_SPI_ReadByte(const SPI_CONFIGURATION&  Configuration)
{
	ASSERT(CheckSlaves(Configuration.DeviceCS));

	return TRUE;
}

UINT8 CPU_SPI_WriteReadByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{
	ASSERT(CheckSlaves(Configuration.DeviceCS));

	return TRUE;
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

	ASSERT(CheckSlaves(Configuration.DeviceCS));

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

}

/*
void GPIO_Config(const SPI_CONFIGURATION& Configuration)
{
  GPIO_InitTypeDef GPIO_InitStructure;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

  switch(Configuration.SPI_mod)
  {
	  case 0:
		  // Configure SPIx pins: SCK, MISO and MOSI
		  GPIO_InitStructure.GPIO_Pin = SPIx_PIN_SCK | SPIx_PIN_MISO | SPIx_PIN_MOSI;          		  
          GPIO_Init(SPIx_GPIO, &GPIO_InitStructure);
		  break;

	  case 1:
		  // Configure SPIy pins: SCK, MISO and MOSI
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
  // PCLK2 = HCLK/2
  //RCC_PCLK2Config(RCC_HCLK_Div2);
  
  // Enable SPIx clock and GPIO clock for SPIx and SPIy
  RCC_APB2PeriphClockCmd(SPIx_GPIO_CLK | SPIy_GPIO_CLK | SPIx_CLK, ENABLE);

  // Enable SPIx Periph clock
  RCC_APB1PeriphClockCmd(SPIy_CLK, ENABLE);
}
*/

void NVIC_Config()
{
	/*
	TODO yet
	*/	
}
