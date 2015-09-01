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


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_AvailableFSInterfaces"
#endif


using namespace Samraksh::eMote::DotNow;

extern FILESYSTEM_DRIVER_INTERFACE g_FAT32_FILE_SYSTEM_DriverInterface;
extern STREAM_DRIVER_INTERFACE g_FAT32_STREAM_DriverInterface;
extern BlockStorageDevice STM32F10x_BlockStorageDevice_SDCARD;
FileSystemVolume g_STM32F10x_FS;


void RemovableMedia::MountRemovableVolumes( HRESULT &hr )
{
	//hal_printf("Inside RemovableMedia::MountRemovableVolumes...\n");

	//From MicroFrameworkPK_v4_3\Solutions\SAM9RL64_EK\DeviceCode\config\FS_config_SAM9RL64_EK.cpp (FS_AddVolumes())
	FileSystemVolume* pFSVolume;
	FAT_LogicDisk* pLogicDisk;

	//From -- MicroFrameworkPK_v4_3\DeviceCode\Targets\Native\sh7264\DeviceCode\BlockStorage\USB\USB_BL_Driver.cpp
	////FileSystemVolumeList::Initialize();
	FileSystemVolumeList::AddVolume( &g_STM32F10x_FS, "U", 0, 0, &g_FAT32_STREAM_DriverInterface, &g_FAT32_FILE_SYSTEM_DriverInterface, &STM32F10x_BlockStorageDevice_SDCARD, 0, FALSE );
	////FileSystemVolumeList::InitializeVolumes();

	pFSVolume = FileSystemVolumeList::FindVolume("U", 1);
	if (pFSVolume)
	{
		FAT_FS_Driver::Initialize();
		FAT_FS_Driver::InitializeVolume(&(pFSVolume->m_volumeId));
		//pFSVolume->Format("U", FORMAT_PARAMETER_FORCE_FAT32);

		/*pLogicDisk = FAT_LogicDisk::Initialize(&(pFSVolume->m_volumeId));
		if (pLogicDisk == NULL)
		{
			pFSVolume->Format("", FORMAT_PARAMETER_FORCE_FAT32);
		}
		else
		{
			pLogicDisk->Uninitialize();
		}*/
	}
}


FILESYSTEM_INTERFACES g_AvailableFSInterfaces[] =
{
    { &g_FAT32_FILE_SYSTEM_DriverInterface, &g_FAT32_STREAM_DriverInterface },
};

const size_t g_InstalledFSCount = ARRAYSIZE(g_AvailableFSInterfaces);

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif


