//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "spot_native.h"
#include "spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_SDInternal.h"

#include <Samraksh\Hal_util.h>
#include "..\sdio\netmf_sdio.h"

extern "C"
{
	void ManagedSDCallback(DeviceStatus status);

}

void ISR_sdProc( CLR_RT_HeapBlock_NativeEventDispatcher *pContext );

CLR_RT_HeapBlock_NativeEventDispatcher *g_sdContext = NULL;
static UINT64 g_sdUserData = 0;
static BOOL g_sdInterruptEnabled = FALSE;

using namespace Samraksh::SPOT::Hardware::EmoteDotNow;

extern SDIO_Driver g_SDIODriver;

INT32 SDInternal::InternalInitialize( HRESULT &hr )
{
    return g_SDIODriver.Initialize();
}

INT32 SDInternal::InternalWrite( CLR_RT_TypedArray_UINT8 dataArray, UINT16 offset, UINT16 length, UINT32 writeAddressPtr, HRESULT &hr )
{
	if(offset == 0)
	{
		// Erase and write, we need a better file system than this
		if(g_SDIODriver.EraseBlock(writeAddressPtr, 512 + writeAddressPtr) == DS_Success)
		{
			return g_SDIODriver.WriteBlock(dataArray.GetBuffer(), writeAddressPtr, length );
		}
		else
			return DS_Fail;
	}
	else
	{
		return DS_Fail;
	}

	return DS_Success;
}

INT32 SDInternal::InternalRead( CLR_RT_TypedArray_UINT8 dataArray, UINT16 offset, UINT16 length, UINT32 readAddressPtr, HRESULT &hr )
{
	return g_SDIODriver.ReadBlock(dataArray.GetBuffer(), readAddressPtr, length);
}

static HRESULT InitializeSDDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
   g_sdContext  = pContext;
   g_sdUserData = userData;
   return S_OK;
}

static HRESULT EnableDisableSDDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
   g_sdInterruptEnabled = fEnable;
   return S_OK;
}

static HRESULT CleanupSDDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
    g_sdContext = NULL;
    g_sdUserData = 0;
    CleanupNativeEventsFromHALQueue( pContext );
    return S_OK;
}

void ManagedSDCallback(DeviceStatus status)
{
	ISR_sdProc(g_sdContext);
}

void ISR_sdProc( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
    GLOBAL_LOCK(irq);
    SaveNativeEventToHALQueue( pContext, UINT32(g_sdUserData >> 16), UINT32(g_sdUserData & 0xFFFFFFFF) );
}

static const CLR_RT_DriverInterruptMethods g_SDInteropDriverMethods =
{
  InitializeSDDriver,
  EnableDisableSDDriver,
  CleanupSDDriver
};


const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_SD =
{
    "SDCallback",
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_SDInteropDriverMethods
};
