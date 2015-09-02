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


#include "SamrakshEmoteDotNow.h"
#include "SamrakshEmoteDotNow_Samraksh_eMote_DotNow_SDInternal.h"

#include <Samraksh\Hal_util.h>
#include "../DeviceCode/Drivers/FS/FAT/FAT_FS.h"
#include "..\bl\netmf_bl_sdio.h"

extern "C"
{
	void ManagedSDCallback(DeviceStatus status);

}

void ISR_sdProc( CLR_RT_HeapBlock_NativeEventDispatcher *pContext );

CLR_RT_HeapBlock_NativeEventDispatcher *g_sdContext = NULL;
static UINT64 g_sdUserData = 0;
static BOOL g_sdInterruptEnabled = FALSE;

using namespace Samraksh::eMote::DotNow;

extern STM32F10x_blDriver_SDIO g_STM32F10x_blDriver_SDIO;

INT32 SDInternal::InternalInitialize( HRESULT &hr )
{
    //return g_STM32F10x_blDriver_SDIO.Initialize(ManagedSDCallback);
	return g_STM32F10x_blDriver_SDIO.Initialize();
}

INT32 SDInternal::InternalWrite( CLR_RT_TypedArray_UINT8 dataArray, UINT16 offset, UINT16 length, UINT32 writeAddressPtr, HRESULT &hr )
{

	/*if(offset == 0)
	{
		// Erase and write, we need a better file system than this
		if(g_STM32F10x_blDriver_SDIO.EraseBlock(writeAddressPtr, 512 + writeAddressPtr) == DS_Success)
		{
			return g_STM32F10x_blDriver_SDIO.WriteBlock(dataArray.GetBuffer(), writeAddressPtr, length );
		}
		else
			return DS_Fail;
	}
	else
	{
		return DS_Fail;
	}*/
	return 0;
}

INT32 SDInternal::InternalRead( CLR_RT_TypedArray_UINT8 dataArray, UINT16 offset, UINT16 length, UINT32 readAddressPtr, HRESULT &hr )
{
	return g_STM32F10x_blDriver_SDIO.ReadBlock(dataArray.GetBuffer(), readAddressPtr, length);
}

long long int SDInternal::InternalTotalSize( HRESULT &hr )
{
	FileSystemVolume* pFSVolume;
	pFSVolume = FileSystemVolumeList::FindVolume("U", 1);
	long long int totalSize = 0, totalFreeSpace = 0;
	FAT_FS_Driver::GetSizeInfo(&pFSVolume->m_volumeId, &totalSize, &totalFreeSpace);
	return totalSize;
}

long long int SDInternal::InternalTotalFreeSpace( HRESULT &hr )
{
	FileSystemVolume* pFSVolume;
	pFSVolume = FileSystemVolumeList::FindVolume("U", 1);
	long long int totalSize = 0, totalFreeSpace = 0;
	FAT_FS_Driver::GetSizeInfo(&pFSVolume->m_volumeId, &totalSize, &totalFreeSpace);
	return totalFreeSpace;
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
    SaveNativeEventToHALQueue( pContext, UINT32(g_sdUserData >> 32), UINT32(g_sdUserData & 0xFFFFFFFF) );
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
