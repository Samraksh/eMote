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


using namespace Samraksh::eMote::DotNow;


UINT32 handle = 0;
WCHAR* path;
const WCHAR* file;
LPCSTR globalFileName;

/*extern FileSystemVolumeList g_FileSystemVolumeList;
extern FAT_LogicDisk g_FAT_LogicDisk;
extern FAT_FS_Driver g_FAT_FS_Driver;
extern FAT_SectorCache g_FAT_SectorCache;
extern FAT_MemoryManager g_FAT_MemoryManager;
extern FAT_FILE g_FAT_FILE;*/

template<class T1>
unsigned short stringLength(T1 fileName)
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

WCHAR* getFileFromPath(const WCHAR* fileName, int fileLength, UINT32* newFileLen)
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

	WCHAR* file = (WCHAR*)private_malloc(static_j + 4 + 1);
	//file[j] = '\\';
	//j++;
	while(static_i < fileLength)
	{
		file[j] = fileName[static_i];
		j++; static_i++;
	}

	*newFileLen = j;
	file[j] = '\0';
	//LPCWSTR newFile = file;
	//return newFile;
	return file;
}



void NativeFileStream::_ctor( CLR_RT_HeapBlock* pMngObj, LPCSTR fileName, INT32 bufferSize, HRESULT &hr )
{
	//hal_printf("Inside NativeFileStream::_ctor\n");
	//hal_printf("Inside NativeFileStream::fileName: %s\n", fileName);
	//hal_printf("Inside NativeFileStream::bufferSize: %d\n", bufferSize);

	//Open/Create file
	FileSystemVolume* pFSVolume;
	UINT32 newFileLen;
	UINT32 fileNameLen;

	globalFileName = fileName;
	fileNameLen = stringLength(fileName);
	path = stringToShort(fileName);
	//file = getFileFromPath(path, fileNameLen, &newFileLen);

	pFSVolume = FileSystemVolumeList::FindVolume("U", 1);

	HRESULT retValCreateDir = FAT_FS_Driver::CreateDirectory(&pFSVolume->m_volumeId, path);

	HRESULT retValOpen = FAT_FS_Driver::Open(&pFSVolume->m_volumeId, path, &handle);
	//FAT_FS_Driver::Open(&pFSVolume->m_volumeId, (LPCWSTR)globalFileName, &handle);
	//FAT_FS_Driver::Open(&pFSVolume->m_volumeId, (LPCWSTR)file, &handle);
	if(!handle)
	{
		hal_printf("NativeFileStream::_ctor: Cannot open\\create file\r\n");
		FAT_FS_Driver::Close(handle);
	}
	FAT_FS_Driver::Close(handle);
}

INT32 NativeFileStream::Read( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, INT32 param3, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::Read\n");
    INT32 retVal = 0; 
    return retVal;
}

INT32 NativeFileStream::Write( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 buffer, INT32 offset, INT32 count, INT32 timeout, HRESULT &hr )
{
	//hal_printf("Inside NativeFileStream::Write\n");

	/*int i = 0;
	while (i < 1)
	{
		HAL_Time_Sleep_MicroSeconds(1000);
		i++;
	}*/

	FileSystemVolume* pFSVolume;

	pFSVolume = FileSystemVolumeList::FindVolume("U", 1);
	HRESULT retValOpen = FAT_FS_Driver::Open(&pFSVolume->m_volumeId, path, &handle);
	//FAT_FS_Driver::Open(&pFSVolume->m_volumeId, (LPCWSTR)globalFileName, &handle);
	//FAT_FS_Driver::Open(&pFSVolume->m_volumeId, (LPCWSTR)file, &handle);
	if(!handle)
	{
		hal_printf("NativeFileStream::Write: Cannot open\\create file\r\n");
	}

	int bytesReadWrite = 0;
	HRESULT retValWrite = FAT_FS_Driver::Write( handle, buffer.GetBuffer(), (int)buffer.GetSize(), &bytesReadWrite );
	//FAT_FS_Driver::Flush(handle);
	if(bytesReadWrite != buffer.GetSize())
	{
		hal_printf("NativeFileStream::Write: - Write File Test: FAILED\r\n");
		FAT_FS_Driver::Close(handle);
		return FALSE;
	}

	FAT_FS_Driver::Close(handle);
	HAL_Time_Sleep_MicroSeconds(1000);
	hal_printf("NativeFileStream::Write: - Write File Test: SUCCESS\r\n");
	return bytesReadWrite;
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

void NativeFileStream::GetStreamProperties( CLR_RT_HeapBlock* pMngObj, INT8 * canRead, INT8 * canWrite, INT8 * canSeek, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::GetStreamProperties\n");
	/*FileSystemVolume* pFSVolume;
	UINT32 attributes;
	pFSVolume = FileSystemVolumeList::FindVolume("U", 1);
	HRESULT retValAttrs = FAT_FS_Driver::GetAttributes(&pFSVolume->m_volumeId, path, &attributes);

	FS_FILEINFO* fileInfo;
	BOOL * found;
	UINT32 newFileLen;
	UINT32 fileNameLen;
	//fileNameLen = stringLength(globalFileName);
	//file = getFileFromPath(path, fileNameLen, &newFileLen);
	//HRESULT retValFileInfo = FAT_FS_Driver::GetFileInfo(&pFSVolume->m_volumeId, path, fileInfo, found);

	//BlockDeviceInfo* bInfo = STM32F10x_blDriver_SDIO::GetDeviceInfo(this->context);

	SD_Error status;
	SD_CardInfo SDCardInfo;
	status = SD_GetCardInfo(&SDCardInfo);*/
}

void NativeFileStream::Close( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::Close\n");
	HAL_Time_Sleep_MicroSeconds(1000);
}

