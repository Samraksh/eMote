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

using namespace Samraksh::eMote::DotNow;

extern FileSystemVolumeList g_FileSystemVolumeList;
extern FAT_LogicDisk g_FAT_LogicDisk;
extern FAT_FS_Driver g_FAT_FS_Driver;

unsigned short stringLength(LPCSTR fileName)
{
	unsigned short i = 0;
	while(fileName[i] != '\0') {
		i++;
	}
	return i;
}

LPCWSTR stringToShort(LPCSTR fileName)
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



void NativeFileStream::_ctor( CLR_RT_HeapBlock* pMngObj, LPCSTR fileName, INT32 bufferSize, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::_ctor\n");
	hal_printf("Inside NativeFileStream::fileName: %s\n", fileName);
	hal_printf("Inside NativeFileStream::bufferSize: %d\n", bufferSize);
	LPCWSTR path = stringToShort(fileName);
	//g_FAT_LogicDisk.CreateDirectory(path);

	g_FAT_FS_Driver.Initialize();
	FileSystemVolume* pFSVolume;
	pFSVolume = g_FileSystemVolumeList.FindVolume("ROOT", 4);
	if (pFSVolume){
		//g_FAT_FS_Driver.InitializeVolume(&(pFSVolume->m_volumeId));
		g_FAT_FS_Driver.CreateDirectory(&(pFSVolume->m_volumeId), path);
	}
}

INT32 NativeFileStream::Read( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, INT32 param3, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::Read\n");
    INT32 retVal = 0; 
    return retVal;
}

INT32 NativeFileStream::Write( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 buffer, INT32 offset, INT32 count, INT32 timeout, HRESULT &hr )
{
	hal_printf("Inside NativeFileStream::Write\n");
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
	return TRUE;
    //INT32 retVal = 0;
    //return retVal;
}

INT64 NativeFileStream::Seek( CLR_RT_HeapBlock* pMngObj, INT64 param0, UINT32 param1, HRESULT &hr )
{
    INT64 retVal = 0; 
    return retVal;
}

void NativeFileStream::Flush( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
}

INT64 NativeFileStream::GetLength( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT64 retVal = 0; 
    return retVal;
}

void NativeFileStream::SetLength( CLR_RT_HeapBlock* pMngObj, INT64 param0, HRESULT &hr )
{
}

void NativeFileStream::GetStreamProperties( CLR_RT_HeapBlock* pMngObj, INT8 * param0, INT8 * param1, INT8 * param2, HRESULT &hr )
{
}

void NativeFileStream::Close( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
}

