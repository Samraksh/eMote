////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_EmoteDotNow_SELECTOR_H_
#define _PLATFORM_EmoteDotNow_SELECTOR_H_ 1



/////////////////////////////////////////////////////////
//
// processor and features
//
//#define TINYCLR_SOLO


#if defined(PLATFORM_ARM_EmoteDotNow)
#define HAL_SYSTEM_NAME                     "EmoteDotNow"
#define SAM_VERSION_REVISION 15
#define TINYBOOTER_REVISION 2
//#define COMPILE_CUSTOMER_RELEASE 1




//
// processor and features
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// constants
//

#define SYSTEM_CLOCK_HZ                 48000000
#define SYSTEM_CYCLE_CLOCK_HZ           SYSTEM_CLOCK_HZ
#define CLOCK_COMMON_FACTOR             1000000
#define SLOW_CLOCKS_PER_SECOND          48000000
#define SLOW_CLOCKS_TEN_MHZ_GCD         2000000
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

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
#define TOTAL_USART_PORT       2
#define COM1                   ConvertCOM_ComHandle(0)
#define COM2                   ConvertCOM_ComHandle(1)

#define TOTAL_USB_CONTROLLER   1
#define USB1                   ConvertCOM_UsbHandle(0)

#define TOTAL_SOCK_PORT        0

#define TOTAL_DEBUG_PORT       1
#define COM_DEBUG              ConvertCOM_DebugHandle(0)

#define COM_MESSAGING          ConvertCOM_MessagingHandle(0)

#define USART_TX_IRQ_INDEX(x)       ( (x) ? 0 : 0 )     /* TODO set right indexes */
#define USART_DEFAULT_PORT          COM1
#define USART_DEFAULT_BAUDRATE      115200

#define USB_IRQ_INDEX               0  // TODO set right index

#define RF231_SPI_BUS 1					// MUST BE CONSISTENT WITH: netmf_spi.h
#define RF231_LR_SPI_BUS 2

#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE    512  // there is one TX for each usart port
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE    512  // there is one RX for each usart port
#define PLATFORM_DEPENDENT_USB_QUEUE_PACKET_COUNT  2    // there is one queue for each pipe of each endpoint and the size of a single packet is sizeof(USB_PACKET64) == 68 bytes

#define DEBUG_TEXT_PORT    COM1
#define STDIO              COM1
#define DEBUGGER_PORT      COM1
#define MESSAGING_PORT     COM1


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


const UINT8 g_CountOfHardwareTimers = 2;
const UINT8 g_HardwareTimerIDs[g_CountOfHardwareTimers] = {1, 4};
const UINT8 g_VirtualTimerPerHardwareTimer = 16;
const UINT32 g_HardwareTimerFrequency[g_CountOfHardwareTimers] = {8000000, 32768};


/*BK: TImer Mapper does not seem to work. We need manually select timers from range 0-8 anyways
Timers in use :
VIRT_TIMER_TIME 2
HAL_SLOT_TIMER 3
HAL_SLOT_TIMER3 4
HAL_DISCOVERY_TIMER 5
HAL_RECEPTION_TIMER 6

*/
// timers that are run within interrupt context
#define VIRT_TIMER_EVENTS 			0
#define VIRT_TIMER_REALTIME 		1
#define VIRT_TIMER_OMAC_SCHEDULER	7
// timers that are run within continuations (all C# user timers are run outside an interrupt context also)
#define VIRT_TIMER_TIME 			2
#define VIRT_TIMER_MAC_SENDPKT 		3
#define VIRT_TIMER_MAC_BEACON 		4
#define VIRT_TIMER_MAC_FLUSHBUFFER 	5
#define VIRT_TIMER_REALTIME_DEBUGGER 6

#define VIRT_TIMER_OMAC_DISCOVERY	8
#define VIRT_TIMER_OMAC_TIMESYNC 	9
#define VIRT_TIMER_OMAC_RECEIVER 	10
#define VIRT_TIMER_OMAC_TRANSMITTER	11
#define VIRT_TIMER_OMAC_RECEIVER_ACK 	12
//#define HAL_SLOT_TIMER3 13

#define LocalClockMonitor_TIMER1 14
#define NeighborClockMonitor_TIMER1 15

#define VIRT_TIMER_OMAC_SCHEDULER_FAILSAFE 3

#define TEST_0B_TIMER	13


////////////////////////////////////SAMRAKSH's definitions done/////////////////////////////

#include <processor_selector.h>


// For debugging OMAC - START

/*
J11_PIN1 = Vin
public const Cpu.Pin GPIO_J11_PIN3 = 0; (PA0)
public const Cpu.Pin GPIO_J11_PIN4 = 1; (PA1)
public const Cpu.Pin GPIO_J11_PIN5 = 2; (PA2)
public const Cpu.Pin GPIO_J11_PIN6 = 3; (PA3)
public const Cpu.Pin GPIO_J11_PIN7 = 4; (PA4)
public const Cpu.Pin GPIO_J11_PIN8 = 8; (PA8)
public const Cpu.Pin GPIO_J11_PIN9 = 22; (PB6)
public const Cpu.Pin GPIO_J11_PIN10 = 23; (PB7)
public const Cpu.Pin GPIO_J12_PIN1 = 24; (PB8)
public const Cpu.Pin GPIO_J12_PIN2 = 25; (PB9)
public const Cpu.Pin GPIO_J12_PIN3 = 29; (PB13)
public const Cpu.Pin GPIO_J12_PIN4 = 30; (PB14). This is the speaker output.
public const Cpu.Pin GPIO_J12_PIN5 = 31; (PB15)
J12_PIN10 = GND
 */

#define VT_CALLBACK (GPIO_PIN)120					//J11_PIN7

#define NEIGHBORCLOCKMONITORPIN (GPIO_PIN)120 //25 // 120 //31 //2
#define LOCALCLOCKMONITORPIN (GPIO_PIN)120 //24 //120 //22 //25

#define RF231_TX_INTERRUPT (GPIO_PIN)120					//J11_PIN7
#define RF231_RADIO_STATEPIN2 (GPIO_PIN)120					//J11_PIN7
#define RF231_RX (GPIO_PIN)120								//J11_PIN8
#define RF231_TX_TIMESTAMP (GPIO_PIN)120								//J11_PIN9
#define RF231_TX (GPIO_PIN)120								//J11_PIN10
#define RF231_FRAME_BUFF_ACTIVE (GPIO_PIN)120

#define TIMESYNC_GENERATE_MESSAGEPIN (GPIO_PIN)120 //25 //120 //24 //120// // 3 				//J11_pin6
#define TIMESYNC_RECEIVEPIN (GPIO_PIN)120 //24 			//J12_pin5

#define DATARECEPTION_SLOTPIN (GPIO_PIN)120//30 //31 //2				//J12_PIN4

#define DATATX_PIN (GPIO_PIN)120 //29 //120 //120 //2							//J12_PIN3
#define RC_TX_TIMESYNCREQ (GPIO_PIN)31
#define RC_TX_DATA (GPIO_PIN)29

//#define DATATX_TIMESTAMP_PIN	(GPIO_PIN)120 // TIMESYNC_GENERATE_MESSAGEPIN//24 //Same as TIMESYNC_GENERATE_MESSAGEPIN
#define DATATX_DATA_PIN			(GPIO_PIN)120 //25
#define DATARX_TIMESTAMP_PIN	(GPIO_PIN)120 //29


#define SEND_ACK_PIN			(GPIO_PIN)120
#define DATA_RX_INTERRUPT_PIN	(GPIO_PIN)120
#define DATA_TX_ACK_PIN			(GPIO_PIN)120


#define DELAY_BETWEEN_DATATX_SCHED_ACTUAL	(GPIO_PIN)120	//J12_pin5

#define DISCO_SYNCSENDPIN (GPIO_PIN)120 // 24  //120						//J12_PIN1
#define DISCO_SYNCRECEIVEPIN (GPIO_PIN)120 //25 //120					//J12_PIN2

#define OMAC_DEBUG_PIN (GPIO_PIN)120			 			//J11_PIN5

#define OMAC_DATARXPIN (GPIO_PIN)120 //0 //26	//120 //2					//J12_pin5
#define OMAC_RXPIN (GPIO_PIN)22 //120 //23  //120							//J11_pin3 0
#define OMAC_TIMESYNCREQRXPIN 	(GPIO_PIN)120 //23 //30

#define OMAC_TX_DATAACK_PIN (GPIO_PIN)120 //120 //23  //120							//J11_pin3 0
#define OMAC_RX_DATAACK_PIN (GPIO_PIN)120 // 120 //23  //120							//J11_pin3 0


#define RADIOCONTROL_SEND_PIN (GPIO_PIN)120 				//J11_pin4
#define RADIOCONTROL_SENDTS_PIN (GPIO_PIN)120 				//J11_pin5
#define RADIOCONTROL_STATEPIN (GPIO_PIN)30 //120 //23 //120 // 120 //120 				//J11_pin6 //This (GPIO_PIN)3  did not work

#define SCHED_START_STOP_PIN (GPIO_PIN)120 //4

#define SCHED_RX_EXEC_PIN (GPIO_PIN)25 //4
#define SCHED_TX_EXEC_PIN (GPIO_PIN)24 //4
#define SCHED_DISCO_EXEC_PIN (GPIO_PIN)0 //4
#define SCHED_TSREQ_EXEC_PIN (GPIO_PIN)23 //4


/*PIN SETUP FOR TEST LEVEL_0G : TimeSync Test with stability testing
 *
#define VT_CALLBACK (GPIO_PIN)120					//J11_PIN7

#define NEIGHBORCLOCKMONITORPIN (GPIO_PIN)25 // 120 //31 //2
#define LOCALCLOCKMONITORPIN (GPIO_PIN)24 //120 //22 //25

#define RF231_TX_INTERRUPT (GPIO_PIN)120					//J11_PIN7
#define RF231_RADIO_STATEPIN2 (GPIO_PIN)120					//J11_PIN7
#define RF231_RX (GPIO_PIN)120								//J11_PIN8
#define RF231_TX_TIMESTAMP (GPIO_PIN)120								//J11_PIN9
#define RF231_TX (GPIO_PIN)120								//J11_PIN10
#define RF231_FRAME_BUFF_ACTIVE (GPIO_PIN)120

#define TIMESYNC_GENERATE_MESSAGEPIN (GPIO_PIN)120 //25 //120 //24 //120// // 3 				//J11_pin6
#define TIMESYNC_RECEIVEPIN (GPIO_PIN)120 //24 			//J12_pin5

#define DATARECEPTION_SLOTPIN (GPIO_PIN)120//30 //31 //2				//J12_PIN4

#define DATATX_PIN (GPIO_PIN)120 //29 //120 //120 //2							//J12_PIN3
#define RC_TX_TIMESYNCREQ (GPIO_PIN)31
#define RC_TX_DATA (GPIO_PIN)29

//#define DATATX_TIMESTAMP_PIN	(GPIO_PIN)120 // TIMESYNC_GENERATE_MESSAGEPIN//24 //Same as TIMESYNC_GENERATE_MESSAGEPIN
#define DATATX_DATA_PIN			(GPIO_PIN)120 //25
#define DATARX_TIMESTAMP_PIN	(GPIO_PIN)120 //29
#define OMAC_TIMESYNCREQRXPIN			(GPIO_PIN)120 //30
#define SEND_ACK_PIN			(GPIO_PIN)120
#define DATA_RX_INTERRUPT_PIN	(GPIO_PIN)120
#define DATA_TX_ACK_PIN			(GPIO_PIN)120


#define DELAY_BETWEEN_DATATX_SCHED_ACTUAL	(GPIO_PIN)120	//J12_pin5

#define DISCO_SYNCSENDPIN (GPIO_PIN)120 // 24  //120						//J12_PIN1
#define DISCO_SYNCRECEIVEPIN (GPIO_PIN)120 //25 //120					//J12_PIN2

#define OMAC_DEBUG_PIN (GPIO_PIN)120			 			//J11_PIN5
#define OMAC_DATARXPIN (GPIO_PIN)120 //26	//120 //2					//J12_pin5
#define OMAC_RXPIN (GPIO_PIN)22 //120 //23  //120							//J11_pin3 0

#define OMAC_TX_DATAACK_PIN (GPIO_PIN)0 //120 //23  //120							//J11_pin3 0
#define OMAC_RX_DATAACK_PIN (GPIO_PIN)23 // 120 //23  //120							//J11_pin3 0


#define RADIOCONTROL_SEND_PIN (GPIO_PIN)120 				//J11_pin4
#define RADIOCONTROL_SENDTS_PIN (GPIO_PIN)120 				//J11_pin5
#define RADIOCONTROL_STATEPIN (GPIO_PIN)30 //120 //23 //120 // 120 //120 				//J11_pin6 //This (GPIO_PIN)3  did not work

#define SCHED_START_STOP_PIN (GPIO_PIN)120 //4
*/

/*PIN SETUP FOR TEST LEVEL_0G : TimeSync Test


#define NEIGHBORCLOCKMONITORPIN (GPIO_PIN)25 // 120 //31 //2
#define LOCALCLOCKMONITORPIN (GPIO_PIN)24 //120 //22 //25

#define RF231_TX_INTERRUPT (GPIO_PIN)120					//J11_PIN7
#define RF231_RADIO_STATEPIN2 (GPIO_PIN)120					//J11_PIN7
#define RF231_RX (GPIO_PIN)120								//J11_PIN8
#define RF231_TX_TIMESTAMP (GPIO_PIN)120								//J11_PIN9
#define RF231_TX (GPIO_PIN)120								//J11_PIN10
#define RF231_FRAME_BUFF_ACTIVE (GPIO_PIN)120

#define TIMESYNC_GENERATE_MESSAGEPIN (GPIO_PIN)120 // 3 				//J11_pin6
#define TIMESYNC_RECEIVEPIN (GPIO_PIN)29 			//J12_pin5

#define DATARECEPTION_SLOTPIN (GPIO_PIN)30 //2				//J12_PIN4

#define DATATX_PIN (GPIO_PIN)120 //2							//J12_PIN3

#define DELAY_BETWEEN_DATATX_SCHED_ACTUAL	(GPIO_PIN)120	//J12_pin5

#define DISCO_SYNCSENDPIN (GPIO_PIN)24						//J12_PIN1
#define DISCO_SYNCRECEIVEPIN (GPIO_PIN)25					//J12_PIN2

#define OMAC_DEBUG_PIN (GPIO_PIN)120			 			//J11_PIN5
#define OMAC_DATARXPIN (GPIO_PIN)120 //2					//J12_pin5
#define OMAC_RXPIN (GPIO_PIN)0							//J11_pin3 0

#define RADIOCONTROL_SEND_PIN (GPIO_PIN)120 				//J11_pin4
#define RADIOCONTROL_SENDTS_PIN (GPIO_PIN)120 				//J11_pin5
#define RADIOCONTROL_STATEPIN (GPIO_PIN)23 //120 				//J11_pin6 //This (GPIO_PIN)3  did not work

#define SCHED_START_STOP_PIN (GPIO_PIN)120 //4
 *
 *
 */

/*PIN SETUP FOR TEST LEVEL_0A : TimeSync Test
 *
#define RF231_TX_INTERRUPT (GPIO_PIN)120					//J11_PIN7
#define RF231_RADIO_STATEPIN2 (GPIO_PIN)120					//J11_PIN7
#define RF231_RX (GPIO_PIN)120								//J11_PIN8
#define RF231_TX_TIMESTAMP (GPIO_PIN)120								//J11_PIN9
#define RF231_TX (GPIO_PIN)120								//J11_PIN10
#define RF231_FRAME_BUFF_ACTIVE (GPIO_PIN)120

#define TIMESYNC_GENERATE_MESSAGEPIN (GPIO_PIN)120 // 3 				//J11_pin6
#define TIMESYNC_RECEIVEPIN (GPIO_PIN)23 			//J12_pin5

#define DATARECEPTION_SLOTPIN (GPIO_PIN)30 //2				//J12_PIN4

#define DATATX_PIN (GPIO_PIN)29 //2							//J12_PIN3

#define DELAY_BETWEEN_DATATX_SCHED_ACTUAL	(GPIO_PIN)120	//J12_pin5

#define DISCO_SYNCSENDPIN (GPIO_PIN)24						//J12_PIN1
#define DISCO_SYNCRECEIVEPIN (GPIO_PIN)25					//J12_PIN2

#define OMAC_DEBUG_PIN (GPIO_PIN)120			 			//J11_PIN5
#define OMAC_DATARXPIN (GPIO_PIN)120 //2					//J12_pin5
#define OMAC_RXPIN (GPIO_PIN)0							//J11_pin3 0

#define RADIOCONTROL_SEND_PIN (GPIO_PIN)120 				//J11_pin4
#define RADIOCONTROL_SENDTS_PIN (GPIO_PIN)120 				//J11_pin5
#define RADIOCONTROL_STATEPIN (GPIO_PIN)120 				//J11_pin6 //This (GPIO_PIN)3  did not work

#define SCHED_START_STOP_PIN (GPIO_PIN)120 //4
 *
 *
 */

/*PIN SETUP FOR TEST LEVEL_0B : Data Tx-Rx pair test
 *
#define RF231_TX_INTERRUPT (GPIO_PIN)120					//J11_PIN7
#define RF231_RADIO_STATEPIN2 (GPIO_PIN)120					//J11_PIN7
#define RF231_RX (GPIO_PIN)120								//J11_PIN8
#define RF231_TX_TIMESTAMP (GPIO_PIN)120								//J11_PIN9
#define RF231_TX (GPIO_PIN)120								//J11_PIN10
#define RF231_FRAME_BUFF_ACTIVE (GPIO_PIN)120

#define TIMESYNC_GENERATE_MESSAGEPIN (GPIO_PIN)120 // 3 				//J11_pin6
#define TIMESYNC_RECEIVEPIN (GPIO_PIN)120 // 23 			//J12_pin5

#define DATARECEPTION_SLOTPIN (GPIO_PIN)30 //2				//J12_PIN4

#define DATATX_PIN (GPIO_PIN)29 //2							//J12_PIN3

#define DELAY_BETWEEN_DATATX_SCHED_ACTUAL	(GPIO_PIN)120	//J12_pin5

#define DISCO_SYNCSENDPIN (GPIO_PIN)24						//J12_PIN1
#define DISCO_SYNCRECEIVEPIN (GPIO_PIN)25					//J12_PIN2

#define OMAC_DEBUG_PIN (GPIO_PIN)120			 			//J11_PIN5
#define OMAC_DATARXPIN (GPIO_PIN)31 //120 //2					//J12_pin5
#define OMAC_RXPIN (GPIO_PIN)0							//J11_pin3 0

#define RADIOCONTROL_SEND_PIN (GPIO_PIN)120 				//J11_pin4
#define RADIOCONTROL_SENDTS_PIN (GPIO_PIN)120 				//J11_pin5
#define RADIOCONTROL_STATEPIN (GPIO_PIN)22 //120 				//J11_pin6 //This (GPIO_PIN)3  did not work

#define SCHED_START_STOP_PIN (GPIO_PIN)120 //4
 *
 *
 */

/*PIN SETUP FOR TEST LEVEL_0B --2 : Data Tx-Rx pair test
 *
#define VT_CALLBACK (GPIO_PIN)120					//J11_PIN7

#define NEIGHBORCLOCKMONITORPIN (GPIO_PIN)120 //25 // 120 //31 //2
#define LOCALCLOCKMONITORPIN (GPIO_PIN)120 //24 //120 //22 //25

#define RF231_TX_INTERRUPT (GPIO_PIN)120					//J11_PIN7
#define RF231_RADIO_STATEPIN2 (GPIO_PIN)120					//J11_PIN7
#define RF231_RX (GPIO_PIN)120								//J11_PIN8
#define RF231_TX_TIMESTAMP (GPIO_PIN)120								//J11_PIN9
#define RF231_TX (GPIO_PIN)120								//J11_PIN10
#define RF231_FRAME_BUFF_ACTIVE (GPIO_PIN)120

#define TIMESYNC_GENERATE_MESSAGE (GPIO_PIN)120 //24 //120// // 3 				//J11_pin6
#define TIMESYNC_RECEIVEPIN (GPIO_PIN)120 			//J12_pin5

#define DATARECEPTION_SLOTPIN (GPIO_PIN)30 //31 //2				//J12_PIN4

#define DATATX_PIN (GPIO_PIN)29 //120 //120 //2							//J12_PIN3
#define RC_TX_TIMESYNCREQ (GPIO_PIN)24
#define RC_TX_DATA (GPIO_PIN)25

//#define DATATX_TIMESTAMP_PIN	(GPIO_PIN)120 // TIMESYNC_GENERATE_MESSAGEPIN//24 //Same as TIMESYNC_GENERATE_MESSAGEPIN
#define DATATX_DATA_PIN			(GPIO_PIN)120 //25
#define DATARX_TIMESTAMP_PIN	(GPIO_PIN)120 //29
#define OMAC_TIMESYNCREQRXPIN			(GPIO_PIN)120 //30
#define SEND_ACK_PIN			(GPIO_PIN)120
#define DATA_RX_INTERRUPT_PIN	(GPIO_PIN)0
#define DATA_TX_ACK_PIN			(GPIO_PIN)31


#define DELAY_BETWEEN_DATATX_SCHED_ACTUAL	(GPIO_PIN)120	//J12_pin5

#define DISCO_SYNCSENDPIN (GPIO_PIN)120 // 24  //120						//J12_PIN1
#define DISCO_SYNCRECEIVEPIN (GPIO_PIN)120 //25 //120					//J12_PIN2

#define OMAC_DEBUG_PIN (GPIO_PIN)120			 			//J11_PIN5
#define OMAC_DATARXPIN (GPIO_PIN)120 //26	//120 //2					//J12_pin5
#define OMAC_RXPIN (GPIO_PIN)22 //120 //23  //120							//J11_pin3 0

#define RADIOCONTROL_SEND_PIN (GPIO_PIN)120 				//J11_pin4
#define RADIOCONTROL_SENDTS_PIN (GPIO_PIN)120 				//J11_pin5
#define RADIOCONTROL_STATEPIN (GPIO_PIN)23 //120 // 120 //120 				//J11_pin6 //This (GPIO_PIN)3  did not work

#define SCHED_START_STOP_PIN (GPIO_PIN)120 //4
 *
 *
 */


/*PIN SETUP FOR TEST LEVEL_1A : TwoNodes PING PONG
 *
#define RF231_TX_INTERRUPT (GPIO_PIN)120					//J11_PIN7
#define RF231_RADIO_STATEPIN2 (GPIO_PIN)23					//J11_PIN7
#define RF231_RX (GPIO_PIN)120								//J11_PIN8
#define RF231_TX_TIMESTAMP (GPIO_PIN)120								//J11_PIN9
#define RF231_TX (GPIO_PIN)120								//J11_PIN10
#define RF231_FRAME_BUFF_ACTIVE (GPIO_PIN)120

#define TIMESYNC_GENERATE_MESSAGEPIN (GPIO_PIN)120 // 3 				//J11_pin6
#define TIMESYNC_RECEIVEPIN (GPIO_PIN)120 // 23 			//J12_pin5

#define DATARECEPTION_SLOTPIN (GPIO_PIN)30 //2				//J12_PIN4

#define DATATX_PIN (GPIO_PIN)29 //2							//J12_PIN3

#define DELAY_BETWEEN_DATATX_SCHED_ACTUAL	(GPIO_PIN)120	//J12_pin5

#define DISCO_SYNCSENDPIN (GPIO_PIN)24						//J12_PIN1
#define DISCO_SYNCRECEIVEPIN (GPIO_PIN)25					//J12_PIN2

#define OMAC_DEBUG_PIN (GPIO_PIN)120			 			//J11_PIN5
#define OMAC_DATARXPIN (GPIO_PIN)31 //2					//J12_pin5
#define OMAC_RXPIN (GPIO_PIN)0							//J11_pin3 0

#define RADIOCONTROL_SEND_PIN (GPIO_PIN)120 				//J11_pin4
#define RADIOCONTROL_SENDTS_PIN (GPIO_PIN)120 				//J11_pin5
#define RADIOCONTROL_STATEPIN (GPIO_PIN)22 				//J11_pin6 //This (GPIO_PIN)3  did not work

#define SCHED_START_STOP_PIN (GPIO_PIN)120 //4
 *
 *
 */

// For debugging OMAC - END

#endif // PLATFORM_ARM_EmoteDotNow

#endif // _PLATFORM_EmoteDotNow_SELECTOR_H_
