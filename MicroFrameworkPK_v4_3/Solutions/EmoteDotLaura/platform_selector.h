////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_EmoteDotLaura_SELECTOR_H_
#define _PLATFORM_EmoteDotLaura_SELECTOR_H_ 1

//needed by maxim peripheral drivers
#define TARGET MAX3263X
#define TARGET_REV 0x4132


// Properly declare them since they are not ANSI
// Previously declared by accident in compiler setup because we used _GNU_SOURCE (default)
// In future, prefer not to use.
// But better to allow here than touch everything.
typedef unsigned uint;
typedef uint16_t ushort;

#include "testDefines.h"

/////////////////////////////////////////////////////////
//
// processor and features
//

// Samraksh eMote Wireless Reprogrammer Updater Portable Array Kit PAK
// To enable, define SAMRAKSH_UPDATE_EXT, uncomment MFUpdate section of TinyCLR.proj, swap commented addresses in scatterfile_tinyclr_gcc.xml
#define SAMRAKSH_UPDATE_EXT


// #define TINYCLR_SOLO      // change some base addresses when no TinyBooter on device.


#if defined(PLATFORM_ARM_EmoteDotLaura)
#define HAL_SYSTEM_NAME                     "EmoteDotLaura"
#define SAM_VERSION_REVISION 68
#define TINYBOOTER_REVISION 2
//#define COMPILE_CUSTOMER_RELEASE 1




//
// processor and features
//
/////////////////////////////////////////////////////////
#define RUNTIME_MEMORY_PROFILE__small 1 

/////////////////////////////////////////////////////////
//
// constants
//

#define SYSTEM_CLOCK_HZ                 96000000
#define SYSTEM_CYCLE_CLOCK_HZ           SYSTEM_CLOCK_HZ
#define CLOCK_COMMON_FACTOR             1000000
#define SLOW_CLOCKS_PER_SECOND          4096
#define SLOW_CLOCKS_TEN_MHZ_GCD         128
#define SLOW_CLOCKS_MILLISECOND_GCD     8

#define SRAM1_MEMORY_Base   0x20000000
#define SRAM1_MEMORY_Size   0x00018000
#if defined(TINYCLR_SOLO) || defined(SAM_APP_TINYBOOTER)
#define FLASH_MEMORY_Base   0x08000000
#define FLASH_MEMORY_Size   0x00100000
#else
#define FLASH_MEMORY_Base   0x08020000
#define FLASH_MEMORY_Size   0x000D0000
#endif


#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

#define INSTRUMENTATION_H_GPIO_PIN      0


//
// constants
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// macros
//

#define GLOBAL_LOCK(x)             SmartPtr_IRQ x
#define DISABLE_INTERRUPTS()       SmartPtr_IRQ::ForceDisabled()
#define ENABLE_INTERRUPTS()        SmartPtr_IRQ::ForceEnabled()
#define INTERRUPTS_ENABLED_STATE() SmartPtr_IRQ::GetState()
#define GLOBAL_LOCK_SOCKETS(x)     SmartPtr_IRQ x

#if defined(_DEBUG)
#define ASSERT_IRQ_MUST_BE_OFF()   ASSERT(!SmartPtr_IRQ::GetState())
#define ASSERT_IRQ_MUST_BE_ON()    ASSERT( SmartPtr_IRQ::GetState())
#else
#define ASSERT_IRQ_MUST_BE_OFF()
#define ASSERT_IRQ_MUST_BE_ON()
#endif

//
// macros
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// global functions
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// communication facilities
//

// Port definitions
#define TOTAL_USART_PORT       3
#define COM1                   ConvertCOM_ComHandle(0)
#define COM2                   ConvertCOM_ComHandle(1)
#define COM3                   ConvertCOM_ComHandle(2)

#define TOTAL_USB_CONTROLLER   1
#define USB1                   ConvertCOM_UsbHandle(0)

#define TOTAL_SOCK_PORT        0

#define TOTAL_DEBUG_PORT       1
#define COM_DEBUG              ConvertCOM_DebugHandle(1)

#define COM_MESSAGING          ConvertCOM_MessagingHandle(1)

#define USART_TX_IRQ_INDEX(x)       ( (x) ? 0 : 0 )     /* TODO set right indexes */
#define USART_DEFAULT_PORT          COM1
#define USART_DEFAULT_BAUDRATE      115200

#define USB_IRQ_INDEX               0  // TODO set right index


#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE    1400  // there is one TX for each usart port
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE    256  // there is one RX for each usart port
#define PLATFORM_DEPENDENT_USB_QUEUE_PACKET_COUNT  2    // there is one queue for each pipe of each endpoint and the size of a single packet is sizeof(USB_PACKET64) == 68 bytes

#define DEBUG_TEXT_PORT    COM1
#define STDIO              COM1
#define DEBUGGER_PORT      COM1
#define MESSAGING_PORT     COM1


//SPI Devices
//first define the chip select for all slave devices and then initialize the SPI_SLAVES array

//Slave 0 gpio config; P5.0->sck, P5.1->mosi, p5.2->miso, p5.3->ssel

#define GPIO_LORA_CHIPSELECT 43 //P5.3
#define GPIO_SLAVE0_CHIPSELECT GPIO_LORA_CHIPSELECT
#define SPI_PORT_SLAVE0 3 //Maps to SPIPort_M2B

//define new slaves  pins when you add them to platform
#define GPIO_SLAVE1_CHIPSELECT 0
#define GPIO_SLAVE2_CHIPSELECT 0
#define GPIO_SLAVE3_CHIPSELECT 0

//RF231 SPI config, we dont support this on maxim yet
//#define RF231_SPI_BUS 1					// MUST BE CONSISTENT WITH: netmf_spi.h
//#define RF231_LR_SPI_BUS 2

#define SPI_SLAVE_COUNT 1

//Setting the upper and lower thresholds for the GC to kick in
//Our total heap is 48 K bytes
#define PLATFORM_DEPENDENT_HEAP_SIZE_THRESHOLD  16 * 1024
#define PLATFORM_DEPENDENT_HEAP_SIZE_THRESHOLD_UPPER 32 * 1024

// communication facilities
/////////////////////////////////////////////////////////

///////////////////////////////////////SAMRAKSH's CLR and Network Stack Definitions/////////////////////
//#define BUILD_RTM

// TODO: centralize Samraksh's many driver debug macros here...
//#define DEBUG_SAM_SPI        // record delay and errors in SPI busy wait
//#define DEBUG_EMOTE_TIME     // record delay when HAL_Time_SetCompare is asked to compare against a time in the past
//#define DEBUG_EMOTE_ADVTIME  // record delay when STM32F10x_AdvancedTimer::SetCompare is asked to compare against a time in the past
//#define DEBUG_VT 1           // break on bad input, multiple initializations, etc.

#if defined(BUILD_RTM) || !defined(DEBUG)
#if defined(DEBUG_EMOTE_TIME) || defined(DEBUG_EMOTE_ADVTIME)
#warning "######################################################################"
#warning " WARNING! FOUND DEBUG MACROS IN NON-DEBUG FLAVOR! YMMV. DO NOT SHIP! >_<"
#warning "######################################################################"
#error "To proceed with using selected debug macros in Release flavor, please temporarily comment-out this #error line."
#endif /* DEBUG_* */
#endif /* !DEBUG */

//#define DISABLE_SLEEP
//#define EMOTE_WAKELOCKS // in power driver
//#define DOTNOW_HSI_CALIB

//Comment below line to below the regular CLR
#define SAMRAKSH_RTOS_EXT 1 //Samraksh RT extension
//#define RT_HARDWARE_TIMER 3

// Rudimentary TinyOS support for TinyBooter
//#define ENABLE_TINYOS_HACK
//#define TINYOS_BOOT_MAGIC 0xb16b00b5 // What, boorish and crass, me?
//#define TINYOS_BOOT_MAGIC_LOC 0x080200EC // Location in flash memory where TinyOS identifier key is located

//#define TINYHAL_BOOTUP_DISPLAY_BUILD_INFO // Seems redundant

//Enable GC extension
//If the CLR has 500ms to sleep, it calls compaction. If it has 5ms to sleep calls mark&Sweep
//#define SAMRAKSH_GC_EXT 1

// Macros configuring the HALTimer system
#define HALTIMER 5
#define HALTIMER_MAX_RESOLUTION 1365
#define HALTIMER_RESOLUTION_USEC HALTIMER_MAX_RESOLUTION
#define NUMBER_ADC_CHANNELS	3

#define INPUT_REBOOT_PIN  22
#define OUTPUT_REBOOT_PIN 23

/* Defines the start address for the data store. To be kept in sync with the actual address in bl_nor_config */
#define DATASTORE_START_ADDRESS 0x64020000
#define DATASTORE_END_ADDRESS 0x64FE0000

const UINT8 ADVTIMER_32BIT = 1;
const UINT8 DEFAULT_TIMER = ADVTIMER_32BIT;
const UINT8 TIMER_32BIT = ADVTIMER_32BIT;
const UINT8 TIMER1_16BIT = 2;
const UINT8 TIMER2_16BIT = 3;
const UINT8 RTC_32BIT = 4;
const UINT8 LOW_DRIFT_TIMER = RTC_32BIT;

const UINT8 g_CountOfHardwareTimers = 2;
const UINT8 g_HardwareTimerIDs[g_CountOfHardwareTimers] = {DEFAULT_TIMER, LOW_DRIFT_TIMER};
const UINT8 g_VirtualTimerPerHardwareTimer = 16;
//const UINT32 g_HardwareTimerFrequency[g_CountOfHardwareTimers] = {96000000, 32768};
//While the rtc crystal runs at 32768, the prescaler only allows a min tick time of 1/4096 hz
const UINT32 g_HardwareTimerFrequency[g_CountOfHardwareTimers] = {96000000, 4096};


// timers that are run within interrupt context
#define VIRT_TIMER_EVENTS 			0
#define VIRT_TIMER_REALTIME 		1
#define VIRT_TIMER_OMAC_SCHEDULER	2
#define LocalClockMonitor_TIMER1 33
//#define VIRT_TIMER_OMAC_DISCOVERY_POST_EXEC		3
#define NeighborClockMonitor_TIMER1 34
#define VIRT_TIMER_OMAC_RECEIVER_ACK 	5

// The following definition will be used within the code as the decision point in deciding if the timer is to be run within interrupt context or continuation
// Adjust this marker appropriately ( <= marker is interrupt context, > marker is continuation)
#define VIRT_TIMER_INTERRUPT_CONTEXT_MARKER 5

// timers that are run within continuations (all C# user timers are run outside an interrupt context also)
#define VIRT_TIMER_TIME 			10
#define VIRT_TIMER_REALTIME_DEBUGGER 11
#define VIRT_TIMER_ADC_PERIODIC		12
/********CSMAMAC SPECIFIC VIRTUAL TIMERS AND VARIABLES********/
#define VIRT_TIMER_MAC_SENDPKT 		13
#define VIRT_TIMER_MAC_BEACON 		14
#define VIRT_TIMER_MAC_FLUSHBUFFER 	15
/********CSMAMAC SPECIFIC VIRTUAL TIMERS AND VARIABLES********/

/********OMAC SPECIFIC VIRTUAL TIMERS AND VARIABLES********/
#define VIRT_TIMER_OMAC_SCHEDULER_FAILSAFE 16
#define VIRT_TIMER_OMAC_SCHEDULER_RADIO_STOP_RETRY 30
#define VIRT_TIMER_OMAC_DISCOVERY	17
#define VIRT_TIMER_OMAC_TIMESYNC 	18
#define VIRT_TIMER_OMAC_RECEIVER 	19
#define VIRT_TIMER_OMAC_TRANSMITTER	20
//#define VIRT_TIMER_OMAC_TX_EXECEVENT	8
#define VIRT_TIMER_OMAC_FAST_RECOVERY	21
#define VIRT_TIMER_OMAC_POST_EXEC	22
//#define VIRT_TIMER_OMAC_TRANSMITTER_POST_EXEC	23

const uint OMAC_DISCO_SEQ_NUMBER = 27;
//const uint OMAC_HW_ACK_DELAY_MICRO = 100;
//const uint HARDWARE_ACKS = 0;
//const uint SOFTWARE_ACKS = 1;
const uint __SI4468_SOFTWARE_ACK__ = 1;
const uint __SI4468_HARDWARE_ACK__ = 0;		//Hardware ack is always zero for SI4468
const uint __RF231_SOFTWARE_ACK__ = 0;
const uint __RF231_HARDWARE_ACK__ = 1;


///////////////Include GPIO MAP File; All GPIO definition should go into this file

//Include only one of the below
#include "GPIO_MAP_EvalKit.h"
//#include "GPIO_MAP_SamDev.h"
//#include "GPIO_MAP_SamProduct.h"

////////////////////////////////////SAMRAKSH's definitions done/////////////////////////////

#include <processor_selector.h>



#endif // PLATFORM_ARM_EmoteDotLaura

#endif // _PLATFORM_EmoteDotLaura_SELECTOR_H_
