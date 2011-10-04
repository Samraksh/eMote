////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#if !defined(__GNUC__)
#include <rt_fp.h>
#endif

//--//

// we need this to force inclusion from library at link time
#if !defined(PLATFORM_ARM_CONNECTME)
#pragma import(EntryPoint)
#endif

//--//

#undef  TRACE_ALWAYS
#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

//--//

#if !defined(BUILD_RTM) && !defined(PLATFORM_ARM_OS_PORT)

UINT32 Stack_MaxUsed()
{
    // this is the value we check for stack overruns
    const UINT32 StackCheckVal = 0xBAADF00D;

    size_t  size = (size_t)&StackTop - (size_t)&StackBottom;
    UINT32* ptr  = (UINT32*)&StackBottom;

    DEBUG_TRACE1(TRACE_ALWAYS, "Stack Max  = %d\r\n", size);

    while(*ptr == StackCheckVal)
    {
        size -= 4;
        ptr++;
    }

    DEBUG_TRACE1(TRACE_ALWAYS, "Stack Used = %d\r\n", size);

    return size;
}

#endif  // !defined(BUILD_RTM)

//--//
// this is the first C function called after bootstrapping ourselves into ram

// these define the region to zero initialize
extern UINT32 Image$$ER_RAM_RW$$ZI$$Base;
extern UINT32 Image$$ER_RAM_RW$$ZI$$Length;

// here is the execution address/length of code to move from FLASH to RAM
#if defined(PLATFORM_ARM_CONNECTME)
#define IMAGE_RAM_RO_BASE   Image$$RAM_EXEC$$Base
#define IMAGE_RAM_RO_LENGTH Image$$RAM_EXEC$$Length
#else
#define IMAGE_RAM_RO_BASE   Image$$ER_RAM_RO$$Base
#define IMAGE_RAM_RO_LENGTH Image$$ER_RAM_RO$$Length
#endif

extern UINT32 IMAGE_RAM_RO_BASE;
extern UINT32 IMAGE_RAM_RO_LENGTH;

// here is the execution address/length of data to move from FLASH to RAM
extern UINT32 Image$$ER_RAM_RW$$Base;
extern UINT32 Image$$ER_RAM_RW$$Length;

// here is the load address of the RAM code/data
#if defined(PLATFORM_ARM_CONNECTME)
#define LOAD_RAM_RO_BASE Load$$RAM_EXEC$$Base
#else
#define LOAD_RAM_RO_BASE Load$$ER_RAM_RO$$Base
#endif

extern UINT32 LOAD_RAM_RO_BASE;
extern UINT32 Load$$ER_RAM_RW$$Base;

//--//

#if defined(TARGETLOCATION_RAM)

extern UINT32 Load$$ER_RAM$$Base;
extern UINT32 Image$$ER_RAM$$Length;

#elif defined(TARGETLOCATION_FLASH)

extern UINT32 Load$$ER_FLASH$$Base;
extern UINT32 Image$$ER_FLASH$$Length;

#else
    !ERROR
#endif

UINT32 LOAD_IMAGE_Start;
UINT32 LOAD_IMAGE_Length;
UINT32 LOAD_IMAGE_CalcCRC;

//--//

#pragma arm section code = "SectionForBootstrapOperations"

static void __section(SectionForBootstrapOperations) Prepare_Copy( UINT32* src, UINT32* dst, UINT32 len )
{
    if(dst != src)
    {
        while(len)
        {
            *dst++ = *src++;

            len -= 4;
        }
    }
}

static void __section(SectionForBootstrapOperations) Prepare_Zero( UINT32* dst, UINT32 len )
{
    while(len)
    {
        *dst++ = 0;

        len -= 4;
    }
}

void __section(SectionForBootstrapOperations) PrepareImageRegions()
{
    //
    // Copy RAM RO and RW regions into proper location.
    //
    {
        UINT32* src = (UINT32*)&LOAD_RAM_RO_BASE; 
        UINT32* dst = (UINT32*)&IMAGE_RAM_RO_BASE;
        UINT32  len = (UINT32 )&IMAGE_RAM_RO_LENGTH + (UINT32)&Image$$ER_RAM_RW$$Length; 

        Prepare_Copy( src, dst, len );
    }

    //
    // Initialize RAM ZI regions.
    //
    {
        UINT32* dst = (UINT32*)&Image$$ER_RAM_RW$$ZI$$Base;
        UINT32  len = (UINT32 )&Image$$ER_RAM_RW$$ZI$$Length;

        Prepare_Zero( dst, len );
    }
}

#pragma arm section code

//--//

static void InitCRuntime()
{
#if (defined(HAL_REDUCESIZE) || defined(COMPILE_THUMB2))

    // Don't initialize floating-point on small builds.

#else

#if  !defined(__GNUC__)
    _fp_init();
#endif

   setlocale( LC_ALL, "" );

#endif
}

#if !defined(BUILD_RTM)
UINT32 g_Boot_RAMConstants_CRC = 0;
#endif


void HAL_Initialize()
{
	LED_RED();
	
    HAL_CONTINUATION::InitializeList();
    HAL_COMPLETION  ::InitializeList();

    Events_Initialize();

    // this is the place where interrupts are enabled after boot for the first time after boot
    ENABLE_INTERRUPTS();

    // have to initialize the blockstorage first, as the USB device needs to update the configure block

    BlockStorageList::Initialize();

    BlockStorage_AddDevices();

    BlockStorageList::InitializeDevices();

    FileSystemVolumeList::Initialize();

    FS_AddVolumes();

    FileSystemVolumeList::InitializeVolumes();
    
#if !defined(HAL_REDUCESIZE)

    CPU_InitializeCommunication();
#endif

    LCD_Initialize();

    I2C_Initialize();

//    Buttons_Initialize();

    // Initialize the backlight to a default off state
    BackLight_Initialize();

    Piezo_Initialize();

    Battery_Initialize();

    Charger_Initialize();

#if defined(ENABLE_NATIVE_PROFILER)
    Native_Profiler_Init();
#endif
}
#ifndef PLATFORM_ARM_SAM9RL64_ANY
void HAL_UnReserveAllGpios()
{
    for(INT32 i = CPU_GPIO_GetPinCount()-1; i >=0; i--)
    {
        CPU_GPIO_ReservePin((GPIO_PIN)i, false);
    }
}
#else
#define PIN_PER_PORT 32

void HAL_UnReserveAllGpios()
{
    
}
#endif

void HAL_Uninitialize()
{
#if defined(ENABLE_NATIVE_PROFILER)
    Native_Profiler_Stop();
#endif
    LCD_Uninitialize();

    I2C_Uninitialize();

//    Buttons_Uninitialize();

    // Initialize the backlight to a default off state
    BackLight_Uninitialize();

    Piezo_Uninitialize();

    Battery_Uninitialize();
    Charger_Uninitialize();

    SOCKETS_CloseConnections();

#if !defined(HAL_REDUCESIZE)
    CPU_UninitializeCommunication();
#endif

    FileSystemVolumeList::UninitializeVolumes();

    BlockStorageList::UnInitializeDevices();

    USART_CloseAllPorts();

    HAL_UnReserveAllGpios();

    DISABLE_INTERRUPTS();

    Events_Uninitialize();

    HAL_CONTINUATION::Uninitialize();
    HAL_COMPLETION  ::Uninitialize();
}

extern "C"
{

void BootEntry()
{
#if !defined(BUILD_RTM) && !defined(PLATFORM_ARM_OS_PORT)
    {
        int  marker;
        int* ptr = &marker - 1; // This will point to the current top of the stack.
        int* end = &StackBottom;

        while(ptr >= end)
        {
            *ptr-- = 0xBAADF00D;
        }
    }
#endif

    // these are needed for patch access

#if defined(TARGETLOCATION_RAM)

    LOAD_IMAGE_Start  = (UINT32)&Load$$ER_RAM$$Base;
    LOAD_IMAGE_Length = (UINT32)&Image$$ER_RAM$$Length;

#elif defined(TARGETLOCATION_FLASH)

    LOAD_IMAGE_Start  = (UINT32)&Load$$ER_FLASH$$Base;
    LOAD_IMAGE_Length = (UINT32)&Image$$ER_FLASH$$Length;

#else
    !ERROR
#endif

    InitCRuntime();

    LOAD_IMAGE_Length += (UINT32)&IMAGE_RAM_RO_LENGTH + (UINT32)&Image$$ER_RAM_RW$$Length;

#if !defined(BUILD_RTM)
    UINT32 g_Boot_RAMConstants_CRC = Checksum_RAMConstants();
#endif

    CPU_Initialize();

    Time_Initialize();
    
    HAL_Initialize();
	
	DEBUG_TRACE3(TRACE_ALWAYS, "debug msg: %d, %d, %d\r\n", 1, 1, 1);
	debug_printf("debug msg\r\n");
	hal_printf( "debug msg\r\n");
#if !defined(BUILD_RTM) 
    DEBUG_TRACE4( STREAM_LCD, ".NetMF v%d.%d.%d.%d\r\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION);
    DEBUG_TRACE3(TRACE_ALWAYS, "%s, Build Date:\r\n\t%s %s\r\n", HalName, __DATE__, __TIME__);
#if defined(__GNUC__)
    DEBUG_TRACE1(TRACE_ALWAYS, "GNU Compiler version %d\r\n", __GNUC__);
#else
    DEBUG_TRACE1(TRACE_ALWAYS, "ARM Compiler version %d\r\n", __ARMCC_VERSION);
#endif

    CPU_DisplayProcessorInformation();

    if(CPU_PowerOnReset())
    {
        // clear the screen this way to keep cursor accurate
        LCD_Clear();    // only clear if we are coming up due to power up
        // clear the heap on power up to ensure ram is destroyed

        UINT8* BaseAddress;
        UINT32 SizeInBytes;

        HeapLocation( BaseAddress,    SizeInBytes );
        memset      ( BaseAddress, 0, SizeInBytes );
    }

    lcd_printf("\f");

    lcd_printf("%-15s\r\n", HalName);
    lcd_printf("%-15s\r\n", "Build Date:");
    lcd_printf("  %-13s\r\n", __DATE__);
    lcd_printf("  %-13s\r\n", __TIME__);

#endif  // !defined(BUILD_RTM)

    /***********************************************************************************/

    {
#if defined(FIQ_SAMPLING_PROFILER)
        FIQ_Profiler_Init();
#endif
    }

    // HAL initialization completed.  Interrupts are enabled.  Jump to the Application routine
    ApplicationEntryPoint();

    lcd_printf("\fmain exited!!???.  Halting CPU\r\n");
    debug_printf("main exited!!???.  Halting CPU\r\n");

#if defined(BUILD_RTM)
    CPU_Reset();
#else
    CPU_Halt();
#endif
}

} // extern "C"

//--//

void VALUE_FILTER::Initialize( INT32 values )
{
    ASSERT(values > 0 && values < 65536);

    m_numValues     = 0;
    m_maxValues     = values;
    m_averagedValue = 0;
}

void VALUE_FILTER::Add( INT32 NewValue )
{
    ASSERT(m_maxValues > 0);

    float value = m_averagedValue;
    float add   = NewValue;
    float addFraction;
    float scaleFraction;

    if(m_numValues < m_maxValues)
    {
        m_numValues += 1;

        addFraction   = (float)(m_numValues - 1) / m_numValues;
        scaleFraction = (1.0f - addFraction);

        if(m_numValues == m_maxValues)
        {
            m_addFraction   = addFraction;
            m_scaleFraction = scaleFraction;
        }
    }
    else
    {
        addFraction   = m_addFraction;
        scaleFraction = m_scaleFraction;
    }

    //if(INTERRUPTS_ENABLED_STATE())
    //{
    //    debug_printf( "Add: %08x:%d %f * %f + %f * %f = %f\r\n", (size_t)this, m_numValues, value, addFraction, add, scaleFraction, value * addFraction + add * scaleFraction );
    //}

    m_averagedValue = value * addFraction + add * scaleFraction;
}

void VALUE_FILTER::AdjustBias( INT32 Bias )
{
    m_averagedValue -= (float)Bias;
}

//--//

#if !defined(BUILD_RTM)

void debug_printf( const char* format, ... )
{
    char    buffer[256];
    va_list arg_ptr;

    va_start( arg_ptr, format );

    int len = hal_vsnprintf( buffer, sizeof(buffer)-1, format, arg_ptr );

    // flush existing characters
    DebuggerPort_Flush( HalSystemConfig.DebugTextPort );

    // write string
    DebuggerPort_Write( HalSystemConfig.DebugTextPort, buffer, len );

    // flush new characters
    DebuggerPort_Flush( HalSystemConfig.DebugTextPort );

    va_end( arg_ptr );
}

void lcd_printf( const char* format, ... )
{
    va_list arg_ptr;

    va_start( arg_ptr, format );

    hal_vfprintf( STREAM_LCD, format, arg_ptr );
}

#endif  // !defined(BUILD_RTM)

//--//

volatile INT32 SystemStates[SYSTEM_STATE_TOTAL_STATES];


#if defined(PLATFORM_ARM)

 void SystemState_SetNoLock( SYSTEM_STATE State )
{
    //ASSERT(State < SYSTEM_STATE_TOTAL_STATES);

    ASSERT_IRQ_MUST_BE_OFF();

    SystemStates[State]++;

    //ASSERT(SystemStates[State] > 0);
}


void SystemState_ClearNoLock( SYSTEM_STATE State )
{
    //ASSERT(State < SYSTEM_STATE_TOTAL_STATES);

    ASSERT_IRQ_MUST_BE_OFF();

    SystemStates[State]--;

    //ASSERT(SystemStates[State] >= 0);
}


 BOOL SystemState_QueryNoLock( SYSTEM_STATE State )
{
    //ASSERT(State < SYSTEM_STATE_TOTAL_STATES);

    ASSERT_IRQ_MUST_BE_OFF();

    return (SystemStates[State] > 0) ? TRUE : FALSE;
}

#endif



void SystemState_Set( SYSTEM_STATE State )
{
    GLOBAL_LOCK(irq);

    SystemState_SetNoLock( State );
}


void SystemState_Clear( SYSTEM_STATE State )
{
    GLOBAL_LOCK(irq);

    SystemState_ClearNoLock( State );
}


BOOL SystemState_Query( SYSTEM_STATE State )
{
    GLOBAL_LOCK(irq);

    return SystemState_QueryNoLock( State );
}

//--//

#if !defined(BUILD_RTM)

UINT32 Checksum_RAMConstants()
{
    UINT32* RAMConstants = (UINT32*)&IMAGE_RAM_RO_BASE; 
    UINT32  Length       = (UINT32 )&IMAGE_RAM_RO_LENGTH; 

    UINT32 CRC;

    // start with Vector area CRC
    CRC = SUPPORT_ComputeCRC( NULL, 0x00000020, 0 );

    // add the big block of RAM constants to CRC
    CRC = SUPPORT_ComputeCRC( RAMConstants, Length, CRC );

    return CRC;
}

void Verify_RAMConstants( void* arg )
{
    BOOL BreakpointOnError = (BOOL)arg;

    //debug_printf("RAMC\r\n");

    UINT32 CRC = Checksum_RAMConstants();

    if(CRC != g_Boot_RAMConstants_CRC)
    {
        hal_printf( "RAMC CRC:%08x!=%08x\r\n", CRC, g_Boot_RAMConstants_CRC );

        UINT32* ROMConstants  = (UINT32*)&LOAD_RAM_RO_BASE;
        UINT32* RAMConstants  = (UINT32*)&IMAGE_RAM_RO_BASE;
        UINT32  Length        = (UINT32 )&IMAGE_RAM_RO_LENGTH;
        BOOL    FoundMismatch = FALSE;

        for(int i = 0; i < Length; i += 4)
        {
            if(*RAMConstants != *ROMConstants)
            {
                hal_printf( "RAMC %08x:%08x!=%08x\r\n", (UINT32) RAMConstants, *RAMConstants, *ROMConstants );

                if(!FoundMismatch) lcd_printf( "\fRAMC:%08x\r\n", (UINT32)RAMConstants );  // first one only to LCD
                FoundMismatch = TRUE;
            }

            RAMConstants++;
            ROMConstants++;
        }

        if(!FoundMismatch)
        {
            // the vector area must have been trashed
            lcd_printf("\fRAMC:%08x\r\n", (UINT32) NULL);
            RAMConstants = (UINT32*)NULL;

            for(int i = 0; i < 32; i += 4)
            {
                hal_printf( "RAMC %02x:%08x\r\n", i, *RAMConstants   );
                lcd_printf( "%02x:%08x\r\n"     , i, *RAMConstants++ );
            }
        }

        DebuggerPort_Flush( HalSystemConfig.DebugTextPort );

        if(BreakpointOnError)
        {
            HARD_BREAKPOINT();
        }
    }
}

#endif  // !defined(BUILD_RTM)
