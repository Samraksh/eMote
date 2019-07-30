/*--------- Includes ----------- */

#include "netmf_spi.h"
#include <cmsis/m2sxxx.h>
#include <drivers/mss_spi/mss_spi.h>

/*--------- Macros ----------- */

/*--------- Global Variables ----------- */
mss_spi_instance_t * const gp_my_spi0 = &g_mss_spi0;
mss_spi_instance_t * const gp_my_spi1 = &g_mss_spi1;

BOOL CPU_SPI_Initialize ()
{
	MSS_SPI_init(gp_my_spi0);
	MSS_SPI_init(gp_my_spi1);

	MSS_SPI_configure_master_mode
	(
		&g_mss_spi0,
		MSS_SPI_SLAVE_0,
		MSS_SPI_MODE0,
		256u,
		MSS_SPI_BLOCK_TRANSFER_FRAME_SIZE
	);
	MSS_SPI_configure_master_mode
	(
		&g_mss_spi1,
		MSS_SPI_SLAVE_0,
		MSS_SPI_MODE0,
		256u,
		MSS_SPI_BLOCK_TRANSFER_FRAME_SIZE
	);
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
