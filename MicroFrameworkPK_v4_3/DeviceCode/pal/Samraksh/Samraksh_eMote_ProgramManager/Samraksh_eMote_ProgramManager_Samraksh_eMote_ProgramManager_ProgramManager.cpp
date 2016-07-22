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

UpdateID_t updateIDInUse;

INT8 ProgramManager::UpdaterStart( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0;
	
	Samraksh_Emote_Update::CreateInstance();
	if(updateIDInUse == MFUpdate::badHandle) {
		int len = 10;  // should be >= to number of updates.
		UpdateID_t updateIDs[len];
		MFUpdate_EnumerateUpdates(updateIDs, len);
		for(int itr=len - 1; itr >= 0; --itr) {
			if(updateIDs[itr] != MFUpdate::badHandle) {
				updateIDInUse = updateIDs[itr];
				break;
			}
		}
	}
	if(updateIDInUse != MFUpdate::badHandle) {
		ushort destAddr = Samraksh_Emote_Update::s_destAddr;
		Samraksh_Emote_Update::SendStart(updateIDInUse,destAddr);
	}
    return retVal;
}

INT8 ProgramManager::UpdaterStart( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = 0;
    MFUpdate* updateInfo = MFUpdate::GetUpdate(param0);
    if(updateInfo != NULL) {
        updateIDInUse = updateInfo->Header.UpdateID;
        UpdaterStart(pMngObj, hr);
    }
    return retVal;
}

void ProgramManager::UpdaterSetDestination( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr )
{
	Samraksh_Emote_Update::s_destAddr = param0;
}

