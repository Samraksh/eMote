

/*--------- Includes ----------- */

#include "netmf_spi.h"



BOOL CPU_SPI_Initialize ()
{
	
return true;
    
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
	UINT8 read_data = 0;


	return read_data;
}

BOOL CPU_SPI_nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{


	return true;
	
}

BOOL CPU_SPI_nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{


	return true;
	
	
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
	/*
	TODO yet
	*/	
}
