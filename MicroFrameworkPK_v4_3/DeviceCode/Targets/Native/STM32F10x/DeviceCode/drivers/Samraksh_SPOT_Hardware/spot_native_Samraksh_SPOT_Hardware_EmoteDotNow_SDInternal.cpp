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
		if(g_SDIODriver.EraseBlock(writeAddressPtr, 512 + writeAddressPtr) != DS_Success)
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
}

INT32 SDInternal::InternalRead( CLR_RT_TypedArray_UINT8 dataArray, UINT16 offset, UINT16 length, UINT32 readAddressPtr, HRESULT &hr )
{
	return g_SDIODriver.ReadBlock(dataArray.GetBuffer(), readAddressPtr, length);
}

