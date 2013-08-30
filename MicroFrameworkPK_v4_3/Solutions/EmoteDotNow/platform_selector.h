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



//
// processor and features
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// constants
//

#define STM32F10X_XL

#define SYSTEM_CLOCK_HZ                 48000000
#define SYSTEM_CYCLE_CLOCK_HZ           SYSTEM_CLOCK_HZ
#define CLOCK_COMMON_FACTOR             1000000
#define SLOW_CLOCKS_PER_SECOND          48000000
#define SLOW_CLOCKS_TEN_MHZ_GCD         2000000
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

#define SRAM1_MEMORY_Base   0x20000000
#define SRAM1_MEMORY_Size   0x00018000
#if defined(TINYCLR_SOLO)
#define FLASH_MEMORY_Base   0x08000000
#else
#define FLASH_MEMORY_Base   0x08020000
#endif
#define FLASH_MEMORY_Size   0x00100000

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
// communicaiton facilities
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

// communicaiton facilities
/////////////////////////////////////////////////////////

///////////////////////////////////////SAMRAKSH's CLR and Network Stack Definitions/////////////////////
//#define BUILD_RTM

//Comment below line to below the regular CLR
#define SAMRAKSH_RTOS_EXT 1 //Samraksh RT extension
#define RT_HARDWARE_TIMER 3

//Enable GC extension
//If the CLR has 500ms to sleep, it calls compaction. If it has 5ms to sleep calls mark&Sweep
//#define SAMRAKSH_GC_EXT 1

//Network Stack definitions
UINT16 MF_NODE_ID;

// Macros configuring the HALTimer system
#define HALTIMER 5
#define HALTIMER_MAX_RESOLUTION 1365
#define HALTIMER_RESOLUTION_USEC HALTIMER_MAX_RESOLUTION
#define NUM_HALTIMER_TIMERS 4
#define NUMBER_ADC_CHANNELS	3

#define INPUT_REBOOT_PIN  4
#define OUTPUT_REBOOT_PIN 8

////////////////////////////////////SAMRAKSH's definitions done/////////////////////////////

#include <processor_selector.h>

#endif // PLATFORM_ARM_EmoteDotNow

#endif // _PLATFORM_EmoteDotNow_SELECTOR_H_
