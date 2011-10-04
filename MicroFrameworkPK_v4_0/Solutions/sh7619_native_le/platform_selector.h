////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_iMXS_SELECTOR_H_
#define _PLATFORM_iMXS_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//
#if defined(PLATFORM_SH7619_NATIVE_LE)
#define HAL_SYSTEM_NAME                    "SH7619_NATIVE_LE"

#define PLATFORM_SH2                        1
#define PLATFORM_SH2_7619                   1

#define USB_ALLOW_CONFIGURATION_OVERRIDE    1

//--//


// processor and features
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// constants
//
#define PLATFORM_SUPPORTS_SOFT_REBOOT   TRUE

#define SYSTEM_CLOCK_HZ                 16000000
#define CLOCK_COMMON_FACTOR             1000000
#define SLOW_CLOCKS_PER_SECOND          16000000
#define SLOW_CLOCKS_TEN_MHZ_GCD         2000000
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

#define SRAM1_MEMORY_Base   0x08000000
#define SRAM1_MEMORY_Size   0x02000000

#define FLASH_MEMORY_Base   0x10000000
#define FLASH_MEMORY_Size   0x01000000

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED


#define DRIVER_PAL_BUTTON_MAPPING                                           \
    { GPIO_PIN_NONE, BUTTON_B0 }, /* Upper Far Right - Backlight (spare n)   RP */ \
    { GPIO_PIN_NONE, BUTTON_B1 }, /* Lower Far Right - Channel               RP */ \
    { GPIO_PIN_NONE, BUTTON_B2 }, /* Upper Center    - Up                    R  */ \
    { GPIO_PIN_NONE, BUTTON_B3 }, /* Far Left        - Spare                    */ \
    { GPIO_PIN_NONE, BUTTON_B4 }, /* Center          - Enter                    */ \
    { GPIO_PIN_NONE, BUTTON_B5 }, /* Lower Center    - Down                   P */

#define SH7619_NATIVE_LE_UNUSED_GPIOS \
    UNUSED_GPIO_PULLUP(A_03), \

   
#define INSTRUMENTATION_H_GPIO_PIN      GPIO_PIN_NONE

#if 1
    #define DEBUG_TEXT_PORT         COM1
    #define STDIO                   COM1
    #define DEBUGGER_PORT           COM1
    #define MESSAGING_PORT          COM1
#else
    #define DEBUG_TEXT_PORT         USB1
    #define STDIO                   USB1
    #define DEBUGGER_PORT           USB1
    #define MESSAGING_PORT          USB1
#endif
//
//
// constants
/////////////////////////////////////////////////////////

#include <processor_selector.h>

#endif // PLATFORM_SH7619_NATIVE_LE
//
// drivers
/////////////////////////////////////////////////////////

#endif // _PLATFORM_SH7619_NATIVE_LE_SELECTOR_H_ 1
