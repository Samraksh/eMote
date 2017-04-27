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
#include <cmsis/m2sxxx.h>

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

BOOL CPU_SPI_Initialize ()
{
	return TRUE;
}

void CPU_SPI_Uninitialize()
{

}


void CPU_SPI_Uninitialize(SPI_CONFIGURATION config)
{

}


void CPU_SPI_Enable(SPI_CONFIGURATION config)
{


}




void CPU_SPI_GetPins (UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi)
{

}

UINT32 CPU_SPI_PortsCount()
{

	return 0;
}


// Nived.Sivadas - Scenarios where you want to just write a single byte and leave the reading part out
BOOL CPU_SPI_WriteByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{

	return TRUE;

}

UINT8 CPU_SPI_ReadByte(const SPI_CONFIGURATION&  Configuration)
{

	return 0;
}

UINT8 CPU_SPI_WriteReadByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{

	return 0;

}

UINT8 CPU_SPI_ReadWriteByte(const SPI_CONFIGURATION& Configuration, UINT8 data)
{

	return 0;
}

BOOL CPU_SPI_nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{


	return 0;
	
}

BOOL CPU_SPI_nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{



	return 0;
	
	
}

BOOL CPU_SPI_Xaction_Start( const SPI_CONFIGURATION& Configuration )
{

	return true;
	
}

BOOL CPU_SPI_Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{

	return true;
}

BOOL CPU_SPI_Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{


	return true;
}

BOOL CPU_SPI_Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{

	return true;
}

void SPI_StructInit(const SPI_CONFIGURATION& Configuration)
{

}

void GPIO_Config(const SPI_CONFIGURATION& Configuration)
{


}

void RCC_Config()
{

}

void NVIC_Config()
{

}
