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


#include "Lib.h"
#include "Lib_SimpleInterop_Library.h"

using namespace SimpleInterop;

bool state = false;

void Library::init( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	CPU_GPIO_EnableOutputPin(8,false);
}

void Library::execute( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	if(state == true)
	{
	CPU_GPIO_SetPinState(8,true);
	state = false;
	}
	else
	{
	CPU_GPIO_SetPinState(8,false);
	state = true;
	}
	
}

