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


#include "SamrakshEmoteDotLaura.h"
#include "SamrakshEmoteDotLaura_Samraksh_eMote_DotLaura_LCD.h"
#include <DeviceCode\MaximLib\DevKitBoard\nhd12832.h>
#include <DeviceCode\MaximLib\DevKitBoard\board.h>

using namespace Samraksh::eMote::DotLaura;

INT8 LCD::Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
	NHD12832_Init();
    return retVal;
}

void LCD::WriteString( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, INT32 param3, HRESULT &hr )
{	
	NHD12832_ShowString((uint8_t*)param0.GetBuffer(), (uint8_t)param2, (uint8_t)param3);
}

void LCD::Clear( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, HRESULT &hr )
{
	NHD12832_Clear((uint8_t)param0, (uint8_t)param1);
}

