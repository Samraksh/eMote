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
#include "SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFileStream.h"
#include "../DeviceCode/Drivers/FS/FAT/FAT_FS.h"
#include "../DeviceCode/Drivers/FS/FAT/FAT_FS_Utility.h"
#include "../bl/netmf_bl_sdio.h"
#include <tinyhal.h>

using namespace Samraksh::eMote::DotNow;

/*extern FileSystemVolumeList g_FileSystemVolumeList;
extern FAT_LogicDisk g_FAT_LogicDisk;
extern FAT_FS_Driver g_FAT_FS_Driver;
extern FAT_SectorCache g_FAT_SectorCache;
extern FAT_MemoryManager g_FAT_MemoryManager;
extern FAT_FILE g_FAT_FILE;*/

unsigned short stringLength(LPCSTR fileName)
{
	unsigned short i = 0;
	while(fileName[i] != '\0') {
		i++;
	}
	return i;
}

WCHAR* stringToShort(LPCSTR fileName)
{
	unsigned short strLength = stringLength(fileName);
	WCHAR* retVal = (WCHAR*)private_malloc(strLength);
	unsigned short i = 0;
	while(fileName[i] != '\0') {
		retVal[i] = fileName[i];
		//retVal[i] = fileName[i] << 8 | fileName[i+1];
		//i+=2;
		i++;
	}
	retVal[i] = '\0';
	return retVal;
}

WCHAR* getFileFromPath(WCHAR* fileName, int fileLength, UINT32* newFileLen)
{
	int i = 0, static_i = 0;
	int j = 0, static_j = 0;

	while(i < fileLength)
	{
		if(fileName[i] == '\\'){
			i++;
			static_i = i;
			static_j = 0;
			continue;
		}
		else if(fileName[i] == '.' || fileName[i] == '\0') {
			break;
		}

		i++; static_j++;
	}

	WCHAR* file = (WCHAR*)private_malloc(static_j + 3 + 1);
	while(static_i < fileLength)
	{
		file[j] = fileName[static_i];
		j++; static_i++;
	}

	*newFileLen = j;
	file[j] = '\0';
	return file;
}



void NativeFileStream::_ctor( CLR_RT_HeapBlock* pMngObj, LPCSTR fileName, INT32 bufferSize, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::_ctor\n");
	hal_printf("Inside NativeFileStream::fileName: %s\n", fileName);
	hal_printf("Inside NativeFileStream::bufferSize: %d\n", bufferSize);

	//--------//
	/*WCHAR* path = stringToShort(fileName);
	FileSystemVolume* pFSVolume;
	pFSVolume = g_FileSystemVolumeList.FindVolume("ROOT", 4);
	FAT_LogicDisk* fat_LogicDisk = g_FAT_MemoryManager.GetLogicDisk( &(pFSVolume->m_volumeId) );
	if(!fat_LogicDisk)
	{
		fat_LogicDisk = g_FAT_MemoryManager.AllocateLogicDisk( &(pFSVolume->m_volumeId) );
	}
	UINT32 fileNameLen = stringLength(fileName);
	UINT32 newFileLen;
	const WCHAR* file = getFileFromPath(path, fileNameLen, &newFileLen);
	g_FAT_FILE.Create(fat_LogicDisk, 1, file, newFileLen, 0x80);*/

	//--------//

	//Open/Create file
	WCHAR* path = stringToShort(fileName);
	UINT32 fileNameLen = stringLength(fileName);
	UINT32 newFileLen;
	const WCHAR* file = getFileFromPath(path, fileNameLen, &newFileLen);
	UINT32 handle = 0;
	FileSystemVolume* pFSVolume;
	pFSVolume = FileSystemVolumeList::FindVolume("U", 1);
	//FAT_LogicDisk* fat_LogicDisk = FAT_LogicDisk::Initialize(&(pFSVolume->m_volumeId));
	FAT_FS_Driver::Open(&pFSVolume->m_volumeId, (LPCWSTR)file, &handle);
	if(!handle)
	{
		hal_printf("NativeFileStream::_ctor: Cannot open\\create file\r\n");
		FAT_FS_Driver::Close(handle);
	}
	FAT_FS_Driver::Close(handle);


	/*FAT_MemoryManager::Initialize();
	FAT_LogicDisk* fat_LogicDisk = FAT_LogicDisk::Initialize(&(pFSVolume->m_volumeId));
	fat_LogicDisk = FAT_MemoryManager::GetLogicDisk( &(pFSVolume->m_volumeId) );
	if(!fat_LogicDisk)
	{
		fat_LogicDisk = FAT_MemoryManager::AllocateLogicDisk( &(pFSVolume->m_volumeId) );
		//fat_LogicDisk->Open( (LPCWSTR)path, &handle );
	}
	FAT_FS_Driver::Open(&pFSVolume->m_volumeId, (LPCWSTR)file, &handle);
	if(!handle)
	{
		hal_printf("NativeFileStream::_ctor: Cannot open\\create file\r\n");
		FAT_FS_Driver::Close(handle);
	}
	FAT_FS_Driver::Close(handle);*/



	//g_FAT_LogicDisk.CreateDirectory(path);

	//g_FAT_FS_Driver.Initialize();
	/*FileSystemVolume* pFSVolume;
	pFSVolume = g_FileSystemVolumeList.FindVolume("ROOT", 4);
	if (pFSVolume){
		//g_FAT_FS_Driver.InitializeVolume(&(pFSVolume->m_volumeId));
		g_FAT_FS_Driver.CreateDirectory(&(pFSVolume->m_volumeId), path);
	}*/
}

INT32 NativeFileStream::Read( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, INT32 param3, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::Read\n");
    INT32 retVal = 0; 
    return retVal;
}

INT32 NativeFileStream::Write( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 buffer, INT32 offset, INT32 count, INT32 timeout, HRESULT &hr )
{
	/*hal_printf("Inside NativeFileStream::Write\n");
	int bytesReadWrite = 0;
	UINT32 handle;
	bool result = FAT_FS_Driver::Write( handle, buffer.GetBuffer(), count, &bytesReadWrite );
	if(bytesReadWrite != count)
	{
		hal_printf("NativeFileStream - Write FAILED\r\n");
		FAT_FS_Driver::Close(handle);
		return FALSE;
	}

	FAT_FS_Driver::Close(handle);
	hal_printf("NativeFileStream - Write SUCCESS\r\n");
	return TRUE;*/

	hal_printf("Inside NativeFileStream::Write\n");

	UINT32 handle = 0;
	const WCHAR* file = (const WCHAR*)"\\hello.txt";
	FileSystemVolume* pFSVolume;
	pFSVolume = FileSystemVolumeList::FindVolume("U", 1);
	//FAT_MemoryManager::Initialize();
	////FAT_LogicDisk* fat_LogicDisk = FAT_LogicDisk::Initialize(&(pFSVolume->m_volumeId));
	//fat_LogicDisk = FAT_MemoryManager::AllocateLogicDisk( &(pFSVolume->m_volumeId) );
	//FAT_LogicDisk* fat_LogicDisk = FAT_MemoryManager::GetLogicDisk( &(pFSVolume->m_volumeId) );
	FAT_FS_Driver::Open(&pFSVolume->m_volumeId, (LPCWSTR)file, &handle);
	if(!handle)
	{
		hal_printf("NativeFileStream::Write: Cannot open\\create file\r\n");
	}

	int bytesReadWrite = 0;
	BOOL result = FAT_FS_Driver::Write( handle, buffer.GetBuffer(), 21, &bytesReadWrite );
	FAT_FS_Driver::Flush(handle);
	if(bytesReadWrite != 21)
	{
		hal_printf("NativeFileStream::Write: - Write File Test: FAILED\r\n");
		FAT_FS_Driver::Close(handle);
		return FALSE;
	}

	FAT_FS_Driver::Close(handle);
	hal_printf("NativeFileStream::Write: - Write File Test: SUCCESS\r\n");
	return bytesReadWrite;


	/*SD_Error status;
	SD_CardInfo SDCardInfo;
	status = SD_GetCardInfo(&SDCardInfo);
	status = SD_WriteBlock(buffer.GetBuffer(), 0x0, SDCardInfo.CardBlockSize);

	if(status == SD_OK )
	{
		hal_printf("NativeFileStream - Write SUCCESS\r\n");
		return TRUE;
	}
	else {
		hal_printf("NativeFileStream - Write FAILED\r\n");
		return FALSE;
	}*/
    //INT32 retVal = 0;
    //return retVal;
}

INT64 NativeFileStream::Seek( CLR_RT_HeapBlock* pMngObj, INT64 param0, UINT32 param1, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::Seek\n");
    INT64 retVal = 0; 
    return retVal;
}

void NativeFileStream::Flush( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::Flush\n");
}

INT64 NativeFileStream::GetLength( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::GetLength\n");
    INT64 retVal = 0; 
    return retVal;
}

void NativeFileStream::SetLength( CLR_RT_HeapBlock* pMngObj, INT64 param0, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::SetLength\n");
}

void NativeFileStream::GetStreamProperties( CLR_RT_HeapBlock* pMngObj, INT8 * param0, INT8 * param1, INT8 * param2, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::GetStreamProperties\n");
}

void NativeFileStream::Close( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::Close\n");
}

