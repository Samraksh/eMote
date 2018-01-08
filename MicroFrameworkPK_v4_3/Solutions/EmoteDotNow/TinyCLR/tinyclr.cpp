////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>
#include <stm32f10x.h>

#include <stdint.h>

//#include "E:\DotNet-MF\MicroFrameworkPK_v4_3\DeviceCode\Targets\Native\STM32F10x\DeviceCode\drivers\radio\SI4468\si4468_driver_test.h"

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

static void power_supply_reset() {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_13; // leave out PC8 due to schematic issues
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  // Configure Inputs
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  // Configure Inputs
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // PB5 is open-drain from LTC3103
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  // Configure Inputs
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  hal_printf("Power Global Reset Complete\r\n");
}

static void power_supply_activate(uint16_t pin) __attribute__ ((unused));
static void power_supply_activate(uint16_t pin) {
  GPIO_InitTypeDef GPIO_InitStructure;
  // if (pin == GPIO_Pin_8) {
	  // hal_printf(BAD\r\n", pin);
	  // return;
  // }
  GPIO_InitStructure.GPIO_Pin = pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  switch(pin) {
	  case GPIO_Pin_6:  hal_printf("1.8v rail enabled...\r\n", pin); break;
	  case GPIO_Pin_8:  hal_printf("3.3v rail enabled...\r\n", pin); break;
	  case GPIO_Pin_11: hal_printf("Rad Charge On enabled...\r\n", pin); break;
	  case GPIO_Pin_13: hal_printf("2.5v radio rail enabled...\r\n", pin); break;
	  default: hal_printf("Pin %d unknown... enabled anyway\r\n", pin); break;
  }
}	

//PC12, PC9, PB5, PC7
static void print_power_supply_status(void) {
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)) {
		hal_printf("Radio Cap Fully Charged OK\r\n");
	}  else {
		hal_printf("Radio Cap Fully Charged FAIL\r\n");
	}
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)) {
		hal_printf("3.3v supply OK\r\n");
	}  else {
		hal_printf("3.3v supply FAIL\r\n");
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)) {
		hal_printf("1.8v supply OK\r\n");
	}  else {
		hal_printf("1.8v supply FAIL\r\n");
	}
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)) {
		hal_printf("2.5v radio rail OK\r\n");
	}  else {
		hal_printf("2.5v radio rail FAIL\r\n");
	}
}

// 0 = Cap not rady, 1 = Cap Ready
static int get_radio_charge_status(void) {
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
}

void ApplicationEntryPoint()
{
	power_supply_reset();

	hal_printf("Turning on Power Supplies...\r\n");

	hal_printf("NOT turning on rfPowOn\r\n");
	//power_supply_activate(GPIO_Pin_8);
	hal_printf("NOT turning on v1.8PowOn\r\n");
	//power_supply_activate(GPIO_Pin_6);
	power_supply_activate(GPIO_Pin_11); // Big Cap

	hal_printf("Waiting for big radio cap...\r\n");
	while( get_radio_charge_status() == 0 );
	hal_printf("Big radio cap charged...\r\n");

	power_supply_activate(GPIO_Pin_13);
	while( get_radio_charge_status() == 0 );

	hal_printf("All power supplies enabled...\r\n\r\n");
	print_power_supply_status();
	
	
	print_cpu_serial();
	//si4468_spi2_init_test();
	//si4468_spi2_init_test();
	//si4468_spi2_init_test();
	//si4468_spi2_init_test();
	//si4468_spi2_init_test();
	
	//si4468_spi2_chain_tx();


	/*while(1) {
		si4468_spi2_tx_test();
		if ( si4468_spi2_rx_test() ) {
	
			for(int i=0; i<500000 + my_rand()*10; i++) ;
		}
	}*/

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
	//while(1);

	
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
	
}


BOOL Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                        OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}

