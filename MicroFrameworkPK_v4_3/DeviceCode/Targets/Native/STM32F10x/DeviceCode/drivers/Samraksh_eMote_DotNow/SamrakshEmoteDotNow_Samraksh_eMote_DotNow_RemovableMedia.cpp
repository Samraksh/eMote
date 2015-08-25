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
//#include "../DeviceCode/Include/BlockStorage_decl.h"

using namespace Samraksh::eMote::DotNow;

extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE g_FAT32_STREAM_DriverInterface;
extern BlockStorageDevice STM32F10x_BlockStorageDevice_SDCARD;
FileSystemVolume g_STM32F10x_FS;

extern FileSystemVolumeList g_FileSystemVolumeList;
extern FAT_LogicDisk g_FAT_LogicDisk;

//extern struct SD_BL_CONFIGURATION g_SD_BL_Config;

void RemovableMedia::MountRemovableVolumes( HRESULT &hr )
{
	hal_printf("Inside RemovableMedia::MountRemovableVolumes...\n");

	//FS_MountVolume("SD1", 0, 0, g_SD_BL_Config.Device);

	//From MicroFrameworkPK_v4_3\Solutions\SAM9RL64_EK\DeviceCode\config\FS_config_SAM9RL64_EK.cpp (FS_AddVolumes())
	FileSystemVolume* pFSVolume;
	FAT_LogicDisk* pLogicDisk;

	g_FileSystemVolumeList.AddVolume( &g_STM32F10x_FS, "ROOT", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &STM32F10x_BlockStorageDevice_SDCARD, 0, FALSE );

	pFSVolume = g_FileSystemVolumeList.FindVolume("ROOT", 4);
	if (pFSVolume)
	{
	   //FAT_FS_Driver::Initialize();
	   //FAT_FS_Driver::InitializeVolume(&(pFSVolume->m_volumeId));
	   //FAT_FS_Driver::Format(&(pFSVolume->m_volumeId), "TEST", FORMAT_PARAMETER_FORCE_FAT32);
	   pLogicDisk = g_FAT_LogicDisk.Initialize(&(pFSVolume->m_volumeId));
	   if (pLogicDisk== NULL)
	   {
		   pFSVolume->Format("", FORMAT_PARAMETER_FORCE_FAT32);
	   }
	   else
	   {
			pLogicDisk->Uninitialize();
	   }
	}
}

