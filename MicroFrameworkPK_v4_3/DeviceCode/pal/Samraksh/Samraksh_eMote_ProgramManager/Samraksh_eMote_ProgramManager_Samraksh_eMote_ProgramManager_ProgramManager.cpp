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


#include "Samraksh_eMote_ProgramManager.h"
#include "Samraksh_eMote_ProgramManager_Samraksh_eMote_ProgramManager_ProgramManager.h"

#include <MFUpdate_decl.h>
#include <PAK_decl.h>

using namespace Samraksh::eMote::ProgramManager;

INT8 ProgramManager::UpdaterStart( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0;
	
	Samraksh_Emote_Update::CreateInstance();
	int len = 10;
	UpdateID_t updateIDs[len];
	MFUpdate_EnumerateUpdates(updateIDs, len);
	if(updateIDs[0] == MFUpdate::badHandle) {
		while(true) {}
	}
	ushort destAddr = Samraksh_Emote_Update::s_destAddr;
	Samraksh_Emote_Update::SendStart(updateIDs[0],destAddr);
	
    return retVal;
}

INT8 ProgramManager::UpdaterStart( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = 0; 
    hal_printf("FIXME UpdaterStart pick updateid!\n");
	//Samraksh_Emote_Update::s_destAddr = param0;
	UpdaterStart(pMngObj, hr);
    return retVal;
}

void ProgramManager::UpdaterSetDestination( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr )
{
	Samraksh_Emote_Update::s_destAddr = param0;
}

