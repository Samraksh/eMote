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
#include "spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_LCD.h"
#include "..\lcd\LCD.h"

using namespace Samraksh::SPOT::Hardware::EmoteDotNow;

extern EmoteDotNow_LCD_Driver glcdObject;

INT8 LCD::Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
	
	glcdObject.Initialize();
	
    return retVal;
}

INT8 LCD::Write( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, INT32 param2, INT32 param3, HRESULT &hr )
{
    INT8 retVal = 0; 
	
    glcdObject.Write((SymbolType) param0, (SymbolType) param1, (SymbolType) param2, (SymbolType) param3);
	
    return retVal;
}

INT8 LCD::Clear( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 

    glcdObject.Write(DISPLAY_CHAR_NULL, DISPLAY_CHAR_NULL, DISPLAY_CHAR_NULL, DISPLAY_CHAR_NULL);

    return retVal;
}

