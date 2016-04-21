////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>

#include <stdint.h>

#include "E:\DotNet-MF\MicroFrameworkPK_v4_3\DeviceCode\Targets\Native\STM32F10x\DeviceCode\drivers\radio\SI4468\si4468_driver_test.h"

////////////////////////////////////////////////////////////////////////////////

static void GetCPUSerial(uint8_t * ptr, unsigned num_of_bytes ){
	unsigned Device_Serial0;unsigned Device_Serial1; unsigned Device_Serial2;
	Device_Serial0 = *(unsigned*)(0x1FFFF7E8);
	Device_Serial1 = *(unsigned*)(0x1FFFF7EC);
	Device_Serial2 = *(unsigned*)(0x1FFFF7F0);
	if(num_of_bytes==12){
	    ptr[0] = (uint8_t)(Device_Serial0 & 0x000000FF);
	    ptr[1] = (uint8_t)((Device_Serial0 & 0x0000FF00) >> 8);
	    ptr[2] = (uint8_t)((Device_Serial0 & 0x00FF0000) >> 16);
	    ptr[3] = (uint8_t)((Device_Serial0 & 0xFF000000) >> 24);

	    ptr[4] = (uint8_t)(Device_Serial1 & 0x000000FF);
	    ptr[5] = (uint8_t)((Device_Serial1 & 0x0000FF00) >> 8);
	    ptr[6] = (uint8_t)((Device_Serial1 & 0x00FF0000) >> 16);
	    ptr[7] = (uint8_t)((Device_Serial1 & 0xFF000000) >> 24);

	    ptr[8] = (uint8_t)(Device_Serial2 & 0x000000FF);
	    ptr[9] = (uint8_t)((Device_Serial2 & 0x0000FF00) >> 8);
	    ptr[10] = (uint8_t)((Device_Serial2 & 0x00FF0000) >> 16);
	    ptr[11] = (uint8_t)((Device_Serial2 & 0xFF000000) >> 24);
	}
}

static UINT16 get_hash(void) {
		//Get cpu serial and hash it to use as node id. THIS IS NOT A DRIVER FUNCTION. MOVE TO MAC LAYER.
		UINT8 cpuserial[12];
		memset(cpuserial, 0, 12);
		GetCPUSerial(cpuserial, 12);
		UINT16 tempNum=0;
		UINT16 * temp = (UINT16 *) cpuserial;
		for (int i=0; i< 6; i++){
			tempNum=tempNum ^ temp[i]; //XOR 72-bit number to generate 16-bit hash
		}
		return tempNum;
}

static void print_cpu_serial(void) {
  uint8_t serial[12];
  GetCPUSerial(serial, 12);
  hal_printf("Serial number: 0x");
  for(int i=0; i<12; i++) {
	  hal_printf("%.2x", serial[i]);
  }
  hal_printf("\r\n");
}


static uint16_t my_rand(void) {
	static uint16_t seed = 0;
	static UINT16 hash = get_hash();

	while (seed == 0)
		seed = (HAL_Time_CurrentTicks() & 0xFFFF) ^ hash;

	unsigned lsb = seed & 1;   /* Get LSB (i.e., the output bit). */
	seed >>= 1;                /* Shift register */
	seed ^= (-lsb) & 0xB400u;  /* If the output bit is 1, apply toggle mask.
								* The value has 1 at bits corresponding
								* to taps, 0 elsewhere. */
    return seed;
}
///////////////////////////////////////////////////////////////////////////////

void ApplicationEntryPoint()
{
	print_cpu_serial();
	si4468_spi2_init_test();
	//si4468_spi2_init_test();
	//si4468_spi2_init_test();
	//si4468_spi2_init_test();
	//si4468_spi2_init_test();
	
	//si4468_spi2_chain_tx();


	while(1) {
		si4468_spi2_tx_test();
		if ( si4468_spi2_rx_test() ) {
			/*We got a packet, so wait abit before TX*/
			for(int i=0; i<500000 + my_rand()*10; i++) ;
		}
	}

	//si4468_spi2_tx_test();
/*
	while(1) {
		si4468_spi2_tx_test();
		for(int i=0; i<10666666; i++) ; // spin, maybe about 1 second
	}
*/
	//while(1) { si4468_spi2_rx_test(); }

	//si4468_spi2_rx_test();
	//si4468_spi2_rx_test(); // do it twice
	//si4468_spi2_power_change();

	hal_printf("All tests done!\r\n");

	//SOFT_Breakpoint();
	//::CPU_Reset();
	while(1);

	/*
    CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;

    ClrStartup( clrSettings );

#if !defined(BUILD_RTM)
    debug_printf( "Exiting.\r\n" );
#else
    ::CPU_Reset();
#endif
	*/
}


BOOL Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                        OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}

