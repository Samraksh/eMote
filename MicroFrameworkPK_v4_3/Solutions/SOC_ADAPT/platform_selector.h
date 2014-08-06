#ifndef _PLATFORM_SOC_ADAPT_SELECTOR_H_
#define _PLATFORM_SOC_ADAPT_SELECTOR_H_ 1




/////////////////////////////////////////////////////////
//
// processor and features
//

//#if defined(PLATFORM_ARM_SOC_ADAPT)
#define HAL_SYSTEM_NAME                 "SOC_ADAPT"
#define PLATFORM_ARM_KRAIT

//
// processor and features
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// constants
//

#define SYSTEM_CLOCK_HZ                 6750000
#define SYSTEM_CYCLE_CLOCK_HZ           SYSTEM_CLOCK_HZ
#define CLOCK_COMMON_FACTOR             250000
#define SLOW_CLOCKS_PER_SECOND          6750000
#define SLOW_CLOCKS_TEN_MHZ_GCD         250000
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

#define SRAM1_MEMORY_Base   0x80200000
#define SRAM1_MEMORY_Size   0x00800000 // 8 MB
#define FLASH_MEMORY_Base   0x10000000
#define FLASH_MEMORY_Size   0x01000000

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

#define INSTRUMENTATION_H_GPIO_PIN      0

#define DEBUG_TEXT_PORT    COM1
#define STDIO              COM1
#define DEBUGGER_PORT      COM1
#define MESSAGING_PORT     COM1

#define DISPLAY_MIPI_PANEL_HXMAX 1

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

//#undef TINYCLR_ENABLE_SOURCELEVELDEBUGGING
//
// macros
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// global functions
//

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

#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE    512  // there is one TX for each usart port
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE    512  // there is one RX for each usart port
#define PLATFORM_DEPENDENT_USB_QUEUE_PACKET_COUNT  2    // there is one queue for each pipe of each endpoint and the size of a single packet is sizeof(USB_PACKET64) == 68 bytes
//
// communication facilities

#define SAMRAKSH_RTOS_EXT 1 //Samraksh RT extension

const UINT8 g_CountOfHardwareTimers = 1;
const UINT8 g_HardwareTimerIDs[g_CountOfHardwareTimers] = {17};			//From Krait_Timer.h -- INT_DEBUG_TIMER_EXP
const UINT8 g_VirtualTimerPerHardwareTimer[g_CountOfHardwareTimers] = {8};	//For additional virtual timer support, adjust values here as well as in VirtualTimer.cpp, VirtualTimer.h (VirtualTimer class)
const UINT32 g_HardwareTimerFrequency[g_CountOfHardwareTimers] = {SYSTEM_CLOCK_HZ};

#define VIRT_TIMER_TIME 			0
#define VIRT_TIMER_EVENTS 			1
#define VIRT_TIMER_REALTIME 		2

/////////////////////////////////////////////////////////

#include <processor_selector.h>

//#endif // PLATFORM_ARM_SOC8200

#endif // _PLATFORM_SOC_ADAPT_SELECTOR_H_
