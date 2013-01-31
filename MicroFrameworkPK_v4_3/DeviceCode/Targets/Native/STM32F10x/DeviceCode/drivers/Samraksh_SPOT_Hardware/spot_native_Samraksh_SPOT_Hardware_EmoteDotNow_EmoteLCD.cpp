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
#include "spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_EmoteLCD.h"
#include <DeviceCode\LCD_PCF85162_HAL\LCD_PCF85162.h>

using namespace Samraksh::SPOT::Hardware::EmoteDotNow;

extern LCD_PCF85162_Driver g_LCD_PCF85162_Driver;

INT8 EmoteLCD::Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 

    g_LCD_PCF85162_Driver.Initialize();
    
    return retVal;
}

INT8 EmoteLCD::Uninitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
    
    g_LCD_PCF85162_Driver.Uninitialize();

    return retVal;
}

INT8 EmoteLCD::Write( CLR_RT_HeapBlock* pMngObj, INT32 data4, INT32 data3, INT32 data2, INT32 data1, HRESULT &hr )
{
    INT8 retVal = 0; 
	
    g_LCD_PCF85162_Driver.Write(data4, data3, data2, data1);
	
    return retVal;
}

INT8 EmoteLCD::SetDP( CLR_RT_HeapBlock* pMngObj, INT8 dp4, INT8 dp3, INT8 dp2, INT8 dp1, HRESULT &hr )
{
    INT8 retVal = 0; 
	
	g_LCD_PCF85162_Driver.SetDP(dp4, dp3, dp2, dp1);

    return retVal;
}

INT8 EmoteLCD::WriteN( CLR_RT_HeapBlock* pMngObj, INT32 column, INT32 data, HRESULT &hr )
{
    INT8 retVal = 0; 

    g_LCD_PCF85162_Driver.WriteN(column, data);

    return retVal;
}

INT8 EmoteLCD::WriteRawBytes( CLR_RT_HeapBlock* pMngObj, INT32 data4, INT32 data3, INT32 data2, INT32 data1, HRESULT &hr )
{
    INT8 retVal = 0; 

    g_LCD_PCF85162_Driver.WriteRawBytes(data4, data3, data2, data1);

    return retVal;
}

INT8 EmoteLCD::Blink( CLR_RT_HeapBlock* pMngObj, INT32 blinkType, HRESULT &hr )
{
    INT8 retVal = 0; 

    g_LCD_PCF85162_Driver.Blink(blinkType);

    return retVal;
}

INT8 EmoteLCD::Clear( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 

    g_LCD_PCF85162_Driver.Write(DISPLAY_CHAR_NULL, DISPLAY_CHAR_NULL, DISPLAY_CHAR_NULL, DISPLAY_CHAR_NULL);

    return retVal;
}
