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


#include "InterfaceLibrary.h"
#include "InterfaceLibrary_Samraksh_USBPipe.h"

using namespace Samraksh;

INT32 USBPipe::Write( CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, HRESULT &hr )
{
    INT32 retVal = 0;

    COM_HANDLE ComPortNum = HalSystemConfig.stdio;

   	if(ExtractTransport(ComPortNum) != USB_TRANSPORT)
   		return 0;


    retVal = USB_Write(ConvertCOM_UsbStream( ComPortNum ), (const char *) param0.GetBuffer(), param2);

    return retVal;
}

INT32 USBPipe::Read( CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, HRESULT &hr )
{
    INT32 retVal = 0;

    COM_HANDLE ComPortNum = HalSystemConfig.stdio;

    if(ExtractTransport(ComPortNum) != USB_TRANSPORT)
    	return 0;

    retVal = USB_Read(ConvertCOM_UsbStream( ComPortNum ), (char *) param0.GetBuffer(), param2);

    return retVal;
}

INT8 USBPipe::Flush( HRESULT &hr )
{
    INT8 retVal = 0;

    COM_HANDLE ComPortNum = HalSystemConfig.stdio;

   	if(ExtractTransport(ComPortNum) != USB_TRANSPORT)
   		return 0;


   	retVal = USB_Flush(ConvertCOM_UsbStream( ComPortNum ));


    return retVal;
}

