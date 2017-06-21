////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <MFUpdate_decl.h>

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "s_Updates"
#endif


#if defined(WIN32)
#define SOFT_BREAKPOINT() 
#endif

const INT32 g_MaxUpdateCount = MAX_UPDATE_COUNT;
MFUpdate g_Updates[g_MaxUpdateCount];    //!< Keep track of multiple updates, each represented by an MFUpdate instance. Replaces local s_Updates.
static BOOL     s_updatesInitialized = FALSE;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

static void MFUpdate_RebootHandler()
{
    s_updatesInitialized = FALSE;    
}

static int CalculateTotalNumberOfPackets(MFUpdate* pUpdate) {
    MFUpdateHeader* pUpdateHdr = &pUpdate->Header;
    int numberOfPackets = (pUpdateHdr->UpdateSize / pUpdateHdr->PacketSize + (((pUpdateHdr->UpdateSize % pUpdateHdr->PacketSize) > 0)?1:0)); //get number of packets needed.
    ASSERT(numberOfPackets > 0);
    return numberOfPackets;
}

static int CalculateMissingPktsWordfieldSize(MFUpdate* pUpdate) {
    int numberOfPackets = CalculateTotalNumberOfPackets(pUpdate);
    int missingPktsWordfieldSize = (numberOfPackets >> 5) + ((numberOfPackets % 32) > 0 ? 1 : 0); //convert number of packets to number of 32-bit words needed.
    if(pUpdate->m_missingPktsWordfieldSize > pUpdate->MAX_MISSING_WORDFIELD_SIZE) {
        SOFT_BREAKPOINT();
        return 0;
    }
    return missingPktsWordfieldSize;
}

//TODO: init from storage.
void MFUpdate_InitializeFromStorage()
{
    //for each update package
    //  get files from storage provider
    //  for each update file
    //    re-create the struct MFUpdate
    //      re-create header from storage
    //      re-create provider link
    //      re-create extension link
    //      set authenticated to false?
    //      set validated to false?
    //      set INUSE flag to false?

    int itr_Updates = 0;

    for(int itr_updates=0; itr_updates<g_MaxUpdateCount; itr_updates++) {
        ASSERT((int)g_Updates[itr_updates].Header.UpdateID == MFUpdate::badHandle);
    }

    for(int itr_pkg=0; itr_pkg < g_UpdatePackageCount; itr_pkg++)
    {
        int storageIDs[g_MaxUpdateCount];
        int storageCount = g_MaxUpdateCount;

        g_UpdatePackages[itr_pkg].Storage->GetFiles((UINT16)MFUPDATE_UPDATETYPE_ANY, NULL, &storageCount);
        ASSERT(storageCount <= g_MaxUpdateCount);
        if(storageCount > g_MaxUpdateCount) storageCount = g_MaxUpdateCount;
        g_UpdatePackages[itr_pkg].Storage->GetFiles((UINT16)MFUPDATE_UPDATETYPE_ANY, storageIDs, &storageCount);

        for(int itr_storageIDs=0; itr_storageIDs<storageCount; itr_storageIDs++) {
            MFUpdateHeader header;
            BOOL ret = g_UpdatePackages[itr_pkg].Storage->GetHeader(storageIDs[itr_storageIDs], &header);
            if(ret != TRUE) {
                SOFT_BREAKPOINT(); // MASKABLE FAULT. STORAGE PROBLEM.
                // TODO: record storage fault.
                continue;
            }
            MFUpdate* pUpdate = &g_Updates[itr_Updates];
            pUpdate->Providers = &g_UpdatePackages[itr_pkg];
            memcpy( &pUpdate->Header, &header, sizeof(header));
            pUpdate->m_finalPacketIdx = CalculateTotalNumberOfPackets(pUpdate) - 1;
            pUpdate->m_missingPktsWordfieldSize = CalculateMissingPktsWordfieldSize(pUpdate);
            pUpdate->Flags |= MFUPDATE_FLAGS__INUSE;  // INUSE flag set to indicate there is an update here in old API.
            //TODO: RECOVER MISSING PACKET INFORMATION
        }
    }
}


void MFUpdate_EnumerateUpdates(UpdateID_t updateIDs[], int len_updateIDs )
{
    int itr_id = 0;
    for(itr_id=0; itr_id < len_updateIDs; itr_id++)
    {
        updateIDs[itr_id] = MFUpdate::badHandle;
    }
    itr_id = 0;
    for(int itr_updates=0; (itr_updates < g_MaxUpdateCount) && (itr_id < len_updateIDs); itr_updates++)
    {
        if(g_Updates[itr_updates].Header.UpdateID != MFUpdate::badHandle) {
            updateIDs[itr_id++] = g_Updates[itr_updates].Header.UpdateID;
        }
    }
    return;
}


/**
 * for now, clears all updates out for testing.
 * TODO: restore available updates.
 */
void MFUpdate_Initialize()
{
    if(s_updatesInitialized) return;

    //TODO: ask update provider for available updates, then re-add them to the update list.

    memset(g_Updates, 0, sizeof(g_Updates));

    MFUpdate_InitializeFromStorage();

    HAL_AddSoftRebootHandler(MFUpdate_RebootHandler);

    s_updatesInitialized = TRUE;
}

MFUpdate* MFUpdate::GetUpdate(UpdateID_t handle)
{
    for(int i=0; i < g_MaxUpdateCount; ++i) {
        if(g_Updates[i].Header.UpdateID == handle &&
            0 != (g_Updates[i].Flags & MFUPDATE_FLAGS__INUSE))
        {
            return &g_Updates[i];
        }
    }
    return NULL;
}

static void ReleaseUpdateHandle(UpdateID_t handle)
{

    if(handle == MFUpdate::badHandle) return;
    MFUpdate* p = MFUpdate::GetUpdate(handle);
    if(p == NULL) {
        return;
    }
    memset(&p->Header, 0, sizeof(MFUpdateHeader));
    p->Flags = 0; //clears MFUPDATE_FLAGS__INUSE
}

/**
 * assigns ppUpdate to a storage object
 */
static UpdateID_t GetNewUpdateHandle(MFUpdate** ppUpdate, MFUpdateHeader* pHdr)
{
    UpdateID_t retVal = MFUpdate::badHandle;
    
    for(int i=0; i<ARRAYSIZE(g_Updates); i++)
    {
        // clean up in-use updates that do not have storage.
        // could be previous update was too large, but the new update is small enough to fit, so clear old failed update?
        if(0 != (g_Updates[i].Flags & MFUPDATE_FLAGS__INUSE))
        {
            MFUpdateHeader hdr;
            if(//Allow multiple updates of the same type by commenting this out: (pHdr->UpdateType == s_Updates[i].Header.UpdateType && pHdr->UpdateSubType == s_Updates[i].Header.UpdateSubType) || 
                g_Updates[i].Providers->Storage == NULL ||
               !g_Updates[i].Providers->Storage->GetHeader(g_Updates[i].StorageHandle, &hdr))
            {
                // Clear out update that failed to get storage or header.
                ReleaseUpdateHandle(g_Updates[i].Header.UpdateID);
            }
        }

        if(0 == (g_Updates[i].Flags & MFUPDATE_FLAGS__INUSE))
        {
            // got update handle, not in use
            MFUpdate_Clear(&g_Updates[i]);  // clean up the update handle.
            *ppUpdate = &g_Updates[i];
            (*ppUpdate)->Flags |= MFUPDATE_FLAGS__INUSE;
            retVal = pHdr->UpdateID;
            break;
        }
    }

    return retVal;
}

static const IUpdatePackage* GetProviderByName(LPCSTR szProvider)
{
    for(int i=0; i<g_UpdatePackageCount; i++)
    {
        if(g_UpdatePackages[i].ProviderName != NULL && hal_stricmp(szProvider, g_UpdatePackages[i].ProviderName) == 0)
        {
            return &g_UpdatePackages[i];
        }
    }

    return NULL;
}

UpdateID_t MFUpdate_InitUpdate( LPCSTR szProvider, MFUpdateHeader& update )
{
    MFUpdate* pUpdate;

    if(!s_updatesInitialized) MFUpdate_Initialize();
    
    const IUpdatePackage* providers = GetProviderByName(szProvider);         if(providers == NULL) return MFUpdate::badHandle;
    pUpdate = MFUpdate::GetUpdate(update.UpdateID); //check for existing UpdateID.  assuming UpdateID is a unique hash of update header.
    if(pUpdate) {
        //TODO: reconfigure if request for different packet size. research allowing different packet sizes for same update, how it works with different link qualities.
        pUpdate->Flags |= MFUPDATE_FLAGS__INUSE; //TODO: is this redundant? or is this the case when opening an update after reboot?
        return pUpdate->Header.UpdateID;
    }
    UpdateID_t updateHandle = GetNewUpdateHandle(&pUpdate, &update);
    ASSERT(updateHandle != MFUpdate::badHandle);
    if(updateHandle == MFUpdate::badHandle) return MFUpdate::badHandle;

    memcpy( &pUpdate->Header, &update, sizeof(update) );

    pUpdate->Providers = providers;
    if(&update.PacketSize == 0) {
        SOFT_BREAKPOINT();
        return MFUpdate::badHandle;
    }
    int totalNumberOfPackets = CalculateTotalNumberOfPackets(pUpdate);
    pUpdate->m_finalPacketIdx = totalNumberOfPackets - 1;
    pUpdate->m_missingPktsWordfieldSize = CalculateMissingPktsWordfieldSize(pUpdate);
    if(pUpdate->m_missingPktsWordfieldSize > pUpdate->MAX_MISSING_WORDFIELD_SIZE)
    {
        SOFT_BREAKPOINT();
        return MFUpdate::badHandle; //this could happen for large updates.
    }

    if(providers->Update->InitializeUpdate != NULL && !providers->Update->InitializeUpdate( pUpdate ))
    {
        ReleaseUpdateHandle(updateHandle);
        SOFT_BREAKPOINT();
        return MFUpdate::badHandle;
    }

    //TODO: PAINT STORAGE TO ENABLE MISSING PACKET INFO RECOVERY.

    pUpdate->Flags |= STATE_FLAGS__START;
    return updateHandle;
}

BOOL MFUpdate_AuthCommand( UpdateID_t updateHandle, UINT32 cmd, UINT8* pArgs, INT32 argsLen, UINT8* pResponse, INT32& responseLen )
{
    MFUpdate*                        update = MFUpdate::GetUpdate(updateHandle)      ; if(update == NULL) return FALSE;
    const IUpdateValidationProvider* valid  = update->Providers->Validation; if(valid  == NULL) return FALSE;

    if(valid->AuthCommand == NULL) return FALSE;

    update->Flags &= ~STATE_FLAGS__START;
	update->Flags |= STATE_FLAGS__AUTHCMD;
    return valid->AuthCommand( update, cmd, pArgs, argsLen, pResponse, responseLen );
}

BOOL MFUpdate_Authenticate( UpdateID_t updateHandle, UINT8* pAuthData, INT32 authLen )
{
    BOOL fRet;
    MFUpdate*                        update = MFUpdate::GetUpdate(updateHandle)      ; if(update == NULL) return FALSE;
    const IUpdateValidationProvider* valid  = update->Providers->Validation; if(valid  == NULL) return FALSE;

    if(valid->Authenticate == NULL) return TRUE;

    fRet = valid->Authenticate( update, pAuthData, authLen );

    if(fRet)
    {
        update->Flags |= STATE_FLAGS__AUTHENTICATED;
        update->Flags |= MFUPDATE_FLAGS__AUTHENTICATED;
    }
    else
    {
        update->Flags &= ~MFUPDATE_FLAGS__AUTHENTICATED;
    }

    return fRet;
}


BOOL MFUpdate_GetProperty( UpdateID_t updateHandle, LPCSTR szPropName, UINT8* pPropValue, INT32* pPropValueSize )
{
    MFUpdate*              update   = MFUpdate::GetUpdate(updateHandle)  ; if(update   == NULL) return FALSE;
    const IUpdateProvider* provider = update->Providers->Update; if(provider == NULL) return FALSE;

    if(provider->GetProperty == NULL) return FALSE;

    return provider->GetProperty( update, szPropName, pPropValue, pPropValueSize );
}

BOOL MFUpdate_SetProperty( UpdateID_t updateHandle, LPCSTR szPropName, UINT8* pPropValue, INT32 pPropValueSize )
{
    MFUpdate*              update   = MFUpdate::GetUpdate(updateHandle)  ; if(update   == NULL) return FALSE;
    const IUpdateProvider* provider = update->Providers->Update; if(provider == NULL) return FALSE;

    if(provider->GetProperty == NULL) return FALSE;

    return provider->SetProperty( update, szPropName, pPropValue, pPropValueSize );
}


BOOL MFUpdate_Open( UpdateID_t updateHandle )
{
    MFUpdate*              update   = MFUpdate::GetUpdate(updateHandle)  ; if(update   == NULL) return FALSE;
    const IUpdateProvider* provider = update->Providers->Update; if(provider == NULL) return FALSE;
    MFUpdateHeader         header;

    if(!update->IsAuthenticated()) return FALSE;

    if(update->Providers->Storage == NULL) return FALSE;

    update->StorageHandle = update->Providers->Storage->Open(update->Header.UpdateID, update->Header.UpdateType, update->Header.UpdateSubType);

    if(update->StorageHandle == -1) return FALSE;

    update->Providers->Storage->GetHeader( update->StorageHandle, &header );

    // Samraksh wants to support multiple ASSEMBLY updates at a time.
    // Otherwise, see original code for matching versions.
    // Check if version in storage is older than MFUpdate version. //TODO: for removable media, or restart with state saved in config? Should the old update be replaced?
    if(
        (header.Version.usMajor >  update->Header.Version.usMajor) ||
       ((header.Version.usMajor >= update->Header.Version.usMajor) && (header.Version.usMinor >  update->Header.Version.usMinor)) ||
       ((header.Version.usMajor >= update->Header.Version.usMajor) && (header.Version.usMinor >= update->Header.Version.usMinor)  && (header.Version.usBuild >  update->Header.Version.usBuild)) ||
       ((header.Version.usMajor >= update->Header.Version.usMajor) && (header.Version.usMinor >= update->Header.Version.usMinor)  && (header.Version.usBuild >= update->Header.Version.usBuild)  && (header.Version.usRevision > update->Header.Version.usRevision))
      )
    {
        update->Providers->Storage->Delete(header.UpdateID, header.UpdateType, header.UpdateSubType);
        return FALSE;
    }

    memcpy(&update->Header, &header, sizeof(header));

    return TRUE;
}

BOOL MFUpdate_Create( UpdateID_t updateHandle )
{
    MFUpdate*              pUpdate  = MFUpdate::GetUpdate(updateHandle)   ; if(pUpdate  == NULL) return FALSE;
    const IUpdateProvider* provider = pUpdate->Providers->Update; if(provider == NULL) return FALSE;
    UINT32 flags = pUpdate->Flags;

    if(!pUpdate->IsAuthenticated()) return FALSE;
    if(pUpdate->Providers->Storage == NULL) return FALSE;

    if(!provider->InitializeUpdate(pUpdate))
    {
        ReleaseUpdateHandle(updateHandle);
        return FALSE;
    }
    // TODO: check storage availability?
    pUpdate->StorageHandle = pUpdate->Providers->Storage->Create(pUpdate->Header, flags);

    if(pUpdate->StorageHandle == -1)
    {
        // should be unreachable; Authenticate should have opened instead of called create on already existing update
        ASSERT_SP(0);
        ReleaseUpdateHandle(updateHandle);
        return FALSE;
    }
    if(pUpdate->StorageHandle == -2) {
        // too many updates are being stored.
        // ... but this should never happen because update storage count == update ID count.
        ASSERT_SP(0);
        ReleaseUpdateHandle(updateHandle);
        return FALSE;
    }
    if(pUpdate->StorageHandle == -3) {
        // not enough free space
        //TODO: only delete minimum number of updates.
        for(int i=0; i < ARRAYSIZE(g_Updates); i++) {
            UpdateID_t uID = g_Updates[i].Header.UpdateID;
            if( ((uID != MFUpdate::badHandle) &&
                 (uID != pUpdate->Header.UpdateID)) )
            {
                MFUpdate_Delete(uID);
            }
        }
        pUpdate->StorageHandle = pUpdate->Providers->Storage->Create(pUpdate->Header, flags);
    }

    if(pUpdate->StorageHandle < 0) {
        // default case
        ASSERT_SP(0);
        ReleaseUpdateHandle(updateHandle);
        return FALSE;
    }

    return TRUE;
}


BOOL MFUpdate_GetMissingPackets( UpdateID_t updateHandle, UINT32* pPacketBits, INT32* pCount )
{
    if(pCount == NULL) return FALSE;

    MFUpdate* update = MFUpdate::GetUpdate(updateHandle); if(update == NULL) return FALSE;
    INT32 partIdx = 0;
    INT32 pktSize    = update->Header.PacketSize;
    INT32 updateSize = update->Header.UpdateSize;
    INT32 cnt        = pPacketBits == NULL ? (updateSize + 31) >> 5 : *pCount;
    INT32 offset     = 0;

    if(update->Providers->Storage == NULL) return FALSE;
    if(!update->IsAuthenticated()        ) return FALSE;

    *pCount = 0;

    updateSize += offset;

    // Bill
    hal_printf( "O %d P %d U %d\r\n", offset, updateSize, partIdx );

    while(offset < updateSize && partIdx < cnt)
    {
        for(int i=0; i<32 && offset < updateSize; i++)
        {
            if(offset + pktSize > updateSize)
            {
                pktSize = updateSize - offset;
            }
            
            if(update->Providers->Storage->IsErased != NULL && update->Providers->Storage->IsErased(update->StorageHandle, offset, pktSize))
            {
                (*pCount)++;
                
                if(pPacketBits != NULL)
                {
                    pPacketBits[partIdx] |= 1u << i;
                }
            }
            else
            {
                if(pPacketBits != NULL)
                {
                    pPacketBits[partIdx] &= ~(1u <<i);
                }
            }

            offset += pktSize;
        }

        partIdx++;
    }

    return TRUE;
}

BOOL MFUpdate_AddPacket( UpdateID_t updateHandle, INT32 packetIndex, UINT8* packetData, INT32 packetLen, UINT8* pValidationData, INT32 validationLen )
{
    MFUpdate* update = MFUpdate::GetUpdate(updateHandle); if(update  == NULL) return FALSE;
    BOOL ret = FALSE;

    if(update->Providers->Storage == NULL) return FALSE;    
    if(!update->IsAuthenticated()        ) return FALSE;

    if( pValidationData != NULL && validationLen > 0 && 
        update->Providers->Validation != NULL && 
        update->Providers->Validation->ValidatePacket != NULL)
    {
        if(!update->Providers->Validation->ValidatePacket(update, packetData, packetLen, pValidationData, validationLen ))
        {
            return FALSE;
        }
    }

    ret = packetLen == update->Providers->Storage->Write( update->StorageHandle, packetIndex * update->Header.PacketSize, packetData, packetLen );

    return (ret > 0);
}

BOOL MFUpdate_Validate( UpdateID_t updateHandle, UINT8* pValidationData, INT32 validationLen )
{
    BOOL fValid = TRUE;
    MFUpdate* update = MFUpdate::GetUpdate(updateHandle);
    if(update == NULL)
    {
        SOFT_BREAKPOINT();
        return FALSE;
    }

    if(!update->IsAuthenticated())
    {
        SOFT_BREAKPOINT();
        return FALSE;
    }

    if(update->IsValidated()) return TRUE;

    if(update->Providers->Validation != NULL && update->Providers->Validation->ValidateUpdate != NULL)
    {
        fValid = update->Providers->Validation->ValidateUpdate( update, pValidationData, validationLen );       

        if(fValid) update->Flags |= MFUPDATE_FLAGS__VALIDATED;
        else       update->Flags &= ~MFUPDATE_FLAGS__VALIDATED;
    }

    if(update->Providers->Storage != NULL && update->Providers->Storage->Close != NULL)
    {
        update->Providers->Storage->Close(update->StorageHandle);
    }

    update->Flags |= MFUPDATE_FLAGS__VALIDATED;

    return fValid;
}

BOOL MFUpdate_Install( UpdateID_t updateHandle, UINT8* pValidationData, INT32 validationLen )
{
    MFUpdate* update = MFUpdate::GetUpdate(updateHandle);  if(update == NULL) return FALSE;

    if(!update->IsAuthenticated()) return FALSE;

    if(!update->IsValidated())
    {
        if(!MFUpdate_Validate( updateHandle, pValidationData, validationLen ))
        {
            return FALSE;
        }
    }

    return update->Providers->Update->InstallUpdate( update, pValidationData, validationLen );
}

BOOL MFUpdate_Delete( UpdateID_t updateHandle )
{
    MFUpdate* update = MFUpdate::GetUpdate(updateHandle); if(update == NULL) return FALSE;
    BOOL fRet;

    if(update->Providers->Storage == NULL) return FALSE;
    if(!update->IsAuthenticated()        ) return FALSE;

    if(update->Providers->Storage->Delete != NULL)
    {
        fRet = update->Providers->Storage->Delete( update->Header.UpdateID, update->Header.UpdateType, update->Header.UpdateSubType );
    }

    ReleaseUpdateHandle( updateHandle );

    return fRet;
}

void MFUpdate_Clear( MFUpdate* update )
{
    if(update == NULL) return;
    memset(&update->Header, 0, sizeof(MFUpdateHeader));
    update->StorageHandle = 0;
    update->Flags = 0;
    update->Providers = 0;
    update->m_finalPacketIdx = 0;
    update->m_missingPktsWordfieldSize = 0;
    memset(update->m_missingPkts, 0xFF, sizeof(UINT32)*MFUpdate::MAX_MISSING_WORDFIELD_SIZE );
}

