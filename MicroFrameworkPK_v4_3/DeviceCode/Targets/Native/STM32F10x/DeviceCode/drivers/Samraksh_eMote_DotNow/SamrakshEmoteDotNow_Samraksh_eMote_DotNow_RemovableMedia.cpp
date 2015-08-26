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
#include "SamrakshEmoteDotNow_Samraksh_eMote_DotNow_RemovableMedia.h"
#include "../DeviceCode/Drivers/FS/FAT/FAT_FS.h"
#include <tinyhal.h>
//#include "../DeviceCode/Include/BlockStorage_decl.h"

using namespace Samraksh::eMote::DotNow;

extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE g_FAT32_STREAM_DriverInterface;
extern BlockStorageDevice STM32F10x_BlockStorageDevice_SDCARD;
FileSystemVolume g_STM32F10x_FS;

/*extern FileSystemVolumeList g_FileSystemVolumeList;
extern FAT_LogicDisk g_FAT_LogicDisk;
extern FAT_FS_Driver g_FAT_FS_Driver;*/

//extern struct SD_BL_CONFIGURATION g_SD_BL_Config;

void RemovableMedia::MountRemovableVolumes( HRESULT &hr )
{
	hal_printf("Inside RemovableMedia::MountRemovableVolumes...\n");

	//FS_MountVolume("SD1", 0, 0, g_SD_BL_Config.Device);

	//From MicroFrameworkPK_v4_3\Solutions\SAM9RL64_EK\DeviceCode\config\FS_config_SAM9RL64_EK.cpp (FS_AddVolumes())
	FileSystemVolume* pFSVolume;
	FAT_LogicDisk* pLogicDisk;

	FileSystemVolumeList::AddVolume( &g_STM32F10x_FS, "U", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &STM32F10x_BlockStorageDevice_SDCARD, 0, FALSE );

	pFSVolume = FileSystemVolumeList::FindVolume("U", 1);
	if (pFSVolume)
	{
		FAT_FS_Driver::Initialize();
		//g_FAT_FS_Driver.InitializeVolume(&(pFSVolume->m_volumeId));
		//g_FAT_FS_Driver.Format(&(pFSVolume->m_volumeId), "TEST", FORMAT_PARAMETER_FORCE_FAT32);
		pLogicDisk = FAT_LogicDisk::Initialize(&(pFSVolume->m_volumeId));
		if (pLogicDisk == NULL)
		{
			pFSVolume->Format("", FORMAT_PARAMETER_FORCE_FAT32);
		}
		else
		{
			//pLogicDisk->Open( (LPCWSTR)path, &handle );
			//pLogicDisk->Uninitialize();
		}
	}

	/*//Open/Create file (from MF/MicroFrameworkPK_v4_3/Solutions/SH7264_M3A_HS64/NativeSample/NativeSample.cpp)
	const WCHAR* path = (const WCHAR*)"test1.txt";
	UINT32 handle = 0;
	//WCHAR* path[12] = {'\\', 't', 'e', 's', 't','1', '.', 't', 'x', 't', '\0'};
	//FileSystemVolume* pFSVolume;
	//pFSVolume = FileSystemVolumeList::FindVolume("ROOT", 4);
	FAT_MemoryManager::Initialize();
	FAT_LogicDisk* fat_LogicDisk = FAT_MemoryManager::GetLogicDisk( &(pFSVolume->m_volumeId) );
	if(!fat_LogicDisk)
	{
		fat_LogicDisk = FAT_MemoryManager::AllocateLogicDisk( &(pFSVolume->m_volumeId) );
		fat_LogicDisk->Open( (LPCWSTR)path, &handle );
	}
	FAT_LogicDisk* logicDisk = FAT_MemoryManager::GetLogicDisk( &(pFSVolume->m_volumeId) );
	if(logicDisk)
	{
		logicDisk->Open( (LPCWSTR)path, &handle );
	}
	FAT_FS_Driver::Open(&pFSVolume->m_volumeId, (LPCWSTR)path, &handle);*/
}

