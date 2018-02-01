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

/*
// 0 = Cap not rady, 1 = Cap Ready
static int get_radio_charge_status(void) {
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
}

// 0 = Cap not rady, 1 = Cap Ready
static int get_radio_power_status(void) {
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
}
*/

static void rad_power_monitor(GPIO_PIN Pin, BOOL PinState, void* Param) {
	if (CPU_GPIO_GetPinState(39) == TRUE){
		hal_printf("*** rad pwr good ***\r\n");
	} else {
		hal_printf("*** rad pwr bad ***\r\n");
	}
}

static char get_key() {
	int ret;
	char c;
	USART_DiscardBuffer(0, TRUE); // Does this work???
	ret = 0;

	do { // yeah this is silly and will break easily. --NPS
		__WFI(); // Wait for char
		__WFI(); // Wait for idle
		ret = USART_Read(0, &c, 1);
	} while (ret == 0);
	return c;
}

static void my_flash_reader() {
	char keys[16];
	uint8_t data[257];
	int addr;

	hal_printf("Enter Address to Read: ");
	for(int i=0; i<15; i++) {
		keys[i] = '\0';
		char c = get_key();
		hal_printf("%c", c);
		if (c == 13) { break; } // enter key
		keys[i] = c;
	}
	keys[15] = '\0';
	addr = atoi(keys);
	hal_printf("\nReading 256 bytes from 0x%.6x\r\n", addr);

	spiflash_read(data, 256, addr);
	data[256] = '\0';
	hal_printf("%s\r\n", data);
}

static void my_flash_writer() {
	char keys[16];
	uint8_t data[256];
	int addr;
	int size;

	hal_printf("Enter Address to write to: ");
	for(int i=0; i<15; i++) {
		keys[i] = '\0';
		char c = get_key();
		hal_printf("%c", c);
		if (c == 13) { break; } // enter key
		keys[i] = c;
	}
	keys[15] = '\0';
	addr = atoi(keys);
	hal_printf("\nWriting to 0x%.6x\r\n", addr);

	hal_printf("Enter data\r\n");
	for(size=0; size<256; size++) {
		data[size] = '\0';
		char c = get_key();
		hal_printf("%c", c);
		if (c == 13) { break; } // enter key
		data[size] = c;
	}
	hal_printf("\n");
	hal_printf("Doing Write\r\n");
	spiflash_write_page(data, size, addr);
	hal_printf("Write Done\r\n");
}

static void sooper_sekret_menu() __attribute__ ((noreturn));
static void sooper_sekret_menu() {
	char c;
	int ret;

	while(1) {
		hal_printf("\r\n!!! SECRET MENU !!!\r\n");
		hal_printf("E: Erase SPI-Flash Chip (CE)\r\n");
		hal_printf("Q: Reboot\r\n");
		hal_printf("W: Write something to Flash\r\n");
		hal_printf("R: Read from Flash\r\n");
		hal_printf("Make your choice: ");

		c = get_key();

		hal_printf("%c\r\n", c);

		switch(c) {
			case 'E':  spiflash_chip_erase(0, 0); break;
			case 'Q':  hal_printf("REBOOTING...\r\n"); CPU_Reset(); break;
			case 'W':  my_flash_writer(); break;
			case 'R':  my_flash_reader(); break;
			default: hal_printf("Unknown cmd\r\n"); break;
		}
	}

	// Not reachable
	SOFT_BREAKPOINT();
	while(1) { __WFI(); }
}


void ApplicationEntryPoint()
{
#ifdef PLATFORM_EMOTE_AUSTERE // TODO: THIS REALLY SHOULD GO SOMEWHERE ELSE
	CPU_GPIO_EnableInputPin( (GPIO_PIN) 39, FALSE, rad_power_monitor, GPIO_INT_EDGE_BOTH, RESISTOR_PULLUP );
	print_power_supply_status();
#endif

#ifdef PLATFORM_EMOTE_AUSTERE
	char in[3];
	spiflash_init(0,0);
	USART_DiscardBuffer(0, TRUE);
	hal_printf("Press Secret Key For Secret Menu Now...\r\n");
	HAL_Time_Sleep_MicroSeconds(5000000);
	USART_Read(0, in, sizeof(in) );
	if ( hal_strncmp_s(";;;", in, sizeof(in)) == 0 ) {
		sooper_sekret_menu();
	}
#endif // PLATFORM_EMOTE_AUSTERE

	hal_printf("Done!\r\n");
	SOFT_BREAKPOINT();
	while(1) __WFI();
	
    // CLR_SETTINGS clrSettings;

    // memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    // clrSettings.MaxContextSwitches         = 50;
    // clrSettings.WaitForDebugger            = false;
    // clrSettings.EnterDebuggerLoopAfterExit = true;

    // ClrStartup( clrSettings );

// #if !defined(BUILD_RTM)
    // debug_printf( "Exiting.\r\n" );
// #else
    // ::CPU_Reset();
// #endif
	
}


BOOL Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                        OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}

