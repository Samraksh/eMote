/*
 * IBL.cpp
 *
 *  Created on: Apr 25, 2018
 *      Author: MukundanSridharan
 */

//include
#include <Tinyhal.h>
#include <defaults.h>
#include "../../../Application/IBL/ibl_Services.h"

//void BootEntryLoader();

/*void EntryPoint(){
	BootEntry();
}
*/

void ApplicationEntryPoint(){

	debug_printf("Device booting is done...Going to validate the IBL+ Kernel \n\n");


	debug_printf("  \n\n"); debug_printf("  \n\n"); debug_printf("  \n\n");debug_printf("  \n\n");

	debug_printf("===================================================== \n\n");
	debug_printf("               IMMUTABLE BOOT LOADER \n\n");
	debug_printf("===================================================== \n\n");


	debug_printf("Device booting is done...Going to validate the IBL+ Kernel \n\n");

	debug_printf("This will take some time... \n\n");
	debug_printf("Validating ... \n\n");

	///If everything is fine, the kernel will start executing.. will never return

	bool ret=FALSE;
	 ret = SecureOS_Boot(eMoteOS, KERNEL_SIZE, dSig, SIGSIZE, dKey, SIGSIZE);
	//ret= SecureOS_Boot(RoT, SIGSIZE, dPreSig, SIGSIZE, dKey, SIGSIZE);

	//BootIBL();

	if(!ret){
		int i=0;
		while(i<10){
			debug_printf("Validation Failed!!: %d\n", i);  i++;
		}
	}
}







/*
int hal_vsnprintf( char* buffer, size_t len, const char* format, va_list arg )
{
    NATIVE_PROFILE_PAL_CRT();

#if defined(HAL_REDUCESIZE) || defined(PLATFORM_EMULATED_FLOATINGPOINT)

#undef _vsnprintf

    // _vsnprintf do not support floating point, vs vsnprintf supports floating point

    return _vsnprintf( buffer, len, format, arg );

#define _vsnprintf  DoNotUse_*printf []

#else

#undef vsnprintf

    return vsnprintf( buffer, len, format, arg );

#define vsnprintf  DoNotUse_*printf []

#endif

}
*/

/*
void debug_printf( const char* format, ... )
{
    char    buffer[256];
    va_list arg_ptr;

    va_start( arg_ptr, format );

   //int len = hal_vsnprintf( buffer, sizeof(buffer)-1, format, arg_ptr );
   int len = hal_vsnprintf( buffer, sizeof(buffer)-1, format, arg_ptr );

    // flush existing characters
    USART_Flush( DEBUG_TEXT_PORT );

    // write string
    USART_Write( DEBUG_TEXT_PORT, buffer, len );

    // flush new characters
    USART_Flush( DEBUG_TEXT_PORT );

    va_end( arg_ptr );
}
*/

void BootEntryLoader()
{
	debug_printf("In bootentry");

/*
	INT32 timeout = 0;
    COM_HANDLE hComm = HalSystemConfig.DebuggerPorts[0];

    BlockStorageStream stream;
    FLASH_WORD ProgramWordCheck;
    const UINT32 c_NoProgramFound = 0xFFFFFFFF;
    UINT32 Program = c_NoProgramFound;

#if defined(COMPILE_THUMB2)
    // Don't initialize floating-point on small builds.
#else
    setlocale( LC_ALL, "" );
#endif

    CPU_Initialize();

    HAL_Time_Initialize();

    HAL_CONTINUATION::InitializeList();
    HAL_COMPLETION  ::InitializeList();

    Events_Initialize();

    UINT8* BaseAddress;
    UINT32 SizeInBytes;

    HeapLocation( BaseAddress, SizeInBytes );

    // Initialize custom heap with heap block returned from CustomHeapLocation
    SimpleHeap_Initialize( BaseAddress, SizeInBytes );

    // this is the place where interrupts are enabled after boot for the first time after boot
    ENABLE_INTERRUPTS();

    BlockStorageList::Initialize();

    BlockStorage_AddDevices();

    BlockStorageList::InitializeDevices();

    if(!EnterMicroBooter(timeout))
    {
        HAL_UPDATE_CONFIG cfg;

        timeout = 0;

        if(HAL_CONFIG_BLOCK::ApplyConfig(HAL_UPDATE_CONFIG::GetDriverName(), &cfg, sizeof(cfg)))
        {
            MicroBooter_Install(cfg);

            HAL_CONFIG_BLOCK::InvalidateBlockWithName(HAL_UPDATE_CONFIG::GetDriverName(), FALSE);

            CPU_Reset();
        }
    }

    if(stream.Initialize(BlockUsage::CODE))
    {
        do
        {
            while(TRUE)
            {
                FLASH_WORD *pWord = &ProgramWordCheck;
                UINT32 Address = stream.CurrentAddress();

                if(!stream.Read((BYTE**)&pWord, sizeof(FLASH_WORD))) break;

                if(*pWord == MicroBooter_ProgramMarker())
                {
                    Program = (UINT32)Address;
                    break;
                }

                if(!stream.Seek(BlockStorageStream::STREAM_SEEK_NEXT_BLOCK))
                {
                    if(!stream.NextStream())
                    {
                        break;
                    }
                }
            }

            if(Program != c_NoProgramFound) break;
        }
        while(stream.NextStream());
    }

    if(Program == c_NoProgramFound)
    {
        timeout = -1;
    }

#ifdef MICROBOOTER_NO_SREC_PROCESSING
    while(true)
    {
        if(Program != c_NoProgramFound && stream.Device != NULL)
        {
            Program = MicroBooter_PrepareForExecution(Program);

            DISABLE_INTERRUPTS();
            ((void (*)())Program)();
        }

        Events_WaitForEvents(0, 1000);
    }
#else
    while(true)
    {
        if(timeout != 0)
        {
            g_SREC.Initialize();

            DebuggerPort_Initialize(hComm);

            //--//

            while(true)
            {
                char buf[1024];
                INT32 cnt;

                if(0 == Events_WaitForEvents(ExtractEventFromTransport(hComm) | SYSTEM_EVENT_FLAG_SYSTEM_TIMER, timeout))
                {
                    break;
                }

                // wait for chars to build up
                Events_WaitForEvents(0, 8);

                while(0 < (cnt = DebuggerPort_Read(hComm, buf, sizeof(buf))))
                {
                    for(INT32 i=0; i<cnt; i++)
                    {
                        g_SREC.Process(buf[i]);
                    }
                }

            }
        }

        if(Program != c_NoProgramFound && stream.Device != NULL)
        {
            Program = MicroBooter_PrepareForExecution(Program);

            DISABLE_INTERRUPTS();
            ((void (*)())Program)();
        }

        timeout = -1;
    }
#endif
*/
}

void FIQ_SubHandler() {}
void UNDEF_SubHandler() {ASSERT(FALSE);}
void ABORTP_SubHandler(){ASSERT(FALSE);}
void ABORTD_SubHandler(){ASSERT(FALSE);}
