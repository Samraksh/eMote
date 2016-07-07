// PART OF Samraksh_eMote_ProgramManager.cpp!

#include <TinyCLR_Interop.h>
#include <PAK_decl.h>


CLR_RT_HeapBlock_NativeEventDispatcher* g_UpdateManagerContext = NULL;
static UINT64 g_UpdateManagerUserData = 0;
static BOOL g_UpdateManagerInterruptEnabled = FALSE;  //TODO: check this.
static BOOL g_UpdateManager_Driver_Initialized = FALSE;


void NativeToManagedUpdaterProgressHandler(UINT32 updateID, UINT16 destAddr, UINT16 msg) {
	//TODO: filter messages based on user data?
	GLOBAL_LOCK(irq);
	SaveNativeEventToHALQueue( g_UpdateManagerContext, updateID, ((UINT32)destAddr) << 16 | (UINT32)msg );
}

static HRESULT Initialize_UpdateManager_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
    g_UpdateManagerContext  = pContext;
    g_UpdateManagerUserData = userData;
    Samraksh_Emote_Update::s_UpdaterProgressHandler = &NativeToManagedUpdaterProgressHandler;
    g_UpdateManager_Driver_Initialized = TRUE;
    return S_OK;
}

static HRESULT EnableDisable_UpdateManager_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
   g_UpdateManagerInterruptEnabled = fEnable;
   return S_OK;
}

static HRESULT Cleanup_UpdateManager_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
	if (g_UpdateManager_Driver_Initialized == TRUE){
    	g_UpdateManagerContext = NULL;
    	g_UpdateManagerUserData = 0;
    	CleanupNativeEventsFromHALQueue( pContext );
    	g_UpdateManager_Driver_Initialized = FALSE;
	}
    return S_OK;
}


static const CLR_RT_DriverInterruptMethods g_CLR_UpdateManager_DriverMethods =
{
  Initialize_UpdateManager_Driver,
  EnableDisable_UpdateManager_Driver,
  Cleanup_UpdateManager_Driver
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_Emote_UpdateManager_UpdaterProgressCallback =
{
    "UpdaterProgressCallback",
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_CLR_UpdateManager_DriverMethods
};
