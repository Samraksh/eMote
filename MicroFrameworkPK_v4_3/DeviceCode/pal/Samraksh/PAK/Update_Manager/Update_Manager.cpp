// functions that interact with the C# app update manager

/////////////////////////////////////////////////////////////////////////////
// WIRELESS UPDATE MANAGER
//////////////////////////////////////////////////////////////////////////////

UINT32 GetFirstMissingPacket(MFUpdate* updateInfo) {
    return GetFirstMissingPacket(updateInfo, updateInfo->m_missingPkts);
}

/**
 * helper.
 * @param updateInfo meta data about the update
 * @param missingPkts bit field of missing packets. 1==missing; 0==received.
 */
UINT32 GetFirstMissingPacket(MFUpdate* updateInfo, UINT32* missingPkts ) {
    UINT32 i=0; ///!< word iterator
    UINT32 j=0; ///!< bit iterator
    ASSERT(updateInfo->m_missingPktsWordfieldSize <= updateInfo->MAX_MISSING_WORDFIELD_SIZE);
    for(; i < updateInfo->m_missingPktsWordfieldSize; i++) {
        if(missingPkts[i] != 0) {
            for(;j < 32; j++) {
                if((missingPkts[i] & (1u << j)) != 0 ) {
                    return (i << 5)+j;
                }
            }
        }
    }
    return (i << 5); //doesn't matter that this is not m_finalPacketIdx+1, satisfies larger than m_finalPacketIdx.
}


void InitializeManager() {
    ASSERT(Samraksh_Emote_Update::s_fBaseStationMode == FALSE); // Manager mode is not necessarily incompatible with Basestation mode, but they were not designed to be used together at-the-moment.
    ASSERT(Samraksh_Emote_Update::s_fRadioOn == TRUE);
    Samraksh_Emote_Update::s_fPublishUpdateMode = TRUE; // may be set as TRUE elsewhere too.
    // TODO: set up timer for handling timeout
    // TODO: start timer
}

void Samraksh_Emote_Update::SendStart(UpdateID_t updateId, UINT16 destAddr)
{
    if(Samraksh_Emote_Update::s_fBaseStationMode == true) { SOFT_BREAKPOINT(); }
    Samraksh_Emote_Update::s_fPublishUpdateMode = TRUE; //TODO: remove this and expose call to InitializeManager then add bool return type to this function and return false if s_fPublishUpdateMode == FALSE in this function.

    MFUpdate* update = MFUpdate::GetUpdate(updateId);
    if( update == NULL) {
        SOFT_BREAKPOINT();
        return;
    }

    WP_Message* msg = &g_Samraksh_Emote_Update.m_outboundMessage;

    MFUpdate_Commands::Debugging_MFUpdate_Start* pay = (MFUpdate_Commands::Debugging_MFUpdate_Start*)&g_Samraksh_Emote_Update.m_outboundMessagePayload;
    memcpy(pay->m_provider, update->Providers->ProviderName, sizeof(pay->m_provider));
    pay->m_updateId         = update->Header.UpdateID;
    pay->m_updateType       = update->Header.UpdateType;
    pay->m_updateSubType    = update->Header.UpdateSubType;
    pay->m_updateSize       = update->Header.UpdateSize;
    pay->m_updatePacketSize = update->Header.PacketSize;
    pay->m_versionMajor     = update->Header.Version.usMajor;
    pay->m_versionMinor     = update->Header.Version.usMinor;

    msg->PrepareRequest(c_Debugging_MFUpdate_Start, 0, sizeof(MFUpdate_Commands::Debugging_MFUpdate_Start), (UINT8*)pay);

    bool tx_ret = g_Samraksh_Emote_Update.Wireless_Phy_TransmitMessage(&g_Samraksh_Emote_Update, msg);
    if(tx_ret != true) {
		DEBUG_BREAK_SEND_FAIL();
    }
    return;
}


void Samraksh_Emote_Update::SendAuthCommand(UpdateID_t updateId, UINT16 destAddr)
{
    if(Samraksh_Emote_Update::s_fBaseStationMode == true) { SOFT_BREAKPOINT(); }

    MFUpdate* update = MFUpdate::GetUpdate(updateId);
    if( update == NULL ) {
        SOFT_BREAKPOINT();
        return;
    }

    WP_Message* msg = &g_Samraksh_Emote_Update.m_outboundMessage;

    MFUpdate_Commands::Debugging_MFUpdate_AuthCommand* pay = (MFUpdate_Commands::Debugging_MFUpdate_AuthCommand*)&g_Samraksh_Emote_Update.m_outboundMessagePayload;
    pay->m_updateHandle = update->Header.UpdateID;
    pay->m_authCommand = 0; // TODO: use crypto service provider to authenticate.
    pay->m_authArgsSize = 0;
    pay->m_authArgs[0] = 0;

    msg->PrepareRequest(c_Debugging_MFUpdate_AuthCommand, 0, sizeof(MFUpdate_Commands::Debugging_MFUpdate_AuthCommand), (UINT8*)pay);

    bool tx_ret = g_Samraksh_Emote_Update.Wireless_Phy_TransmitMessage(&g_Samraksh_Emote_Update, msg);
    if(tx_ret != true) {
		DEBUG_BREAK_SEND_FAIL();
    }
    return;
}


void Samraksh_Emote_Update::SendAuthenticate(UpdateID_t updateId, UINT16 destAddr)
{
    if(Samraksh_Emote_Update::s_fBaseStationMode == true) { SOFT_BREAKPOINT(); }

    MFUpdate* update = MFUpdate::GetUpdate(updateId);
    if( update == NULL ) {
        SOFT_BREAKPOINT();
        return;
    }

    WP_Message* msg = &g_Samraksh_Emote_Update.m_outboundMessage;

    MFUpdate_Commands::Debugging_MFUpdate_Authenticate* pay = (MFUpdate_Commands::Debugging_MFUpdate_Authenticate*)&g_Samraksh_Emote_Update.m_outboundMessagePayload;
    pay->m_updateHandle = update->Header.UpdateID;
    pay->m_authenticationLen = 0;
    pay->m_authenticationData[0] = 0;

    msg->PrepareRequest(c_Debugging_MFUpdate_Authenticate, 0, sizeof(MFUpdate_Commands::Debugging_MFUpdate_Authenticate), (UINT8*)pay);

    bool tx_ret = g_Samraksh_Emote_Update.Wireless_Phy_TransmitMessage(&g_Samraksh_Emote_Update, msg);
    if(tx_ret != true) {
		DEBUG_BREAK_SEND_FAIL();
    }
    return;
}


void Samraksh_Emote_Update::SendGetMissingPkts(UpdateID_t updateId, UINT16 destAddr)
{
    if(Samraksh_Emote_Update::s_fBaseStationMode == true) { SOFT_BREAKPOINT(); }

    MFUpdate* update = MFUpdate::GetUpdate(updateId);
    if( update == NULL ) {
        SOFT_BREAKPOINT();
        return;
    }

    WP_Message* msg = &g_Samraksh_Emote_Update.m_outboundMessage;

    MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts* pay = (MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts*)&g_Samraksh_Emote_Update.m_outboundMessagePayload;
    pay->m_updateHandle = update->Header.UpdateID;

    msg->PrepareRequest(c_Debugging_MFUpdate_GetMissingPkts, 0, sizeof(MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts), (UINT8*)pay);

    bool tx_ret = g_Samraksh_Emote_Update.Wireless_Phy_TransmitMessage(&g_Samraksh_Emote_Update, msg);
    if(tx_ret != true) {
		DEBUG_BREAK_SEND_FAIL();
        //TODO: FIXME: schedule continuation. or let timeout handle it?
    }
    return;
}


void Samraksh_Emote_Update::SendAddPacket(UpdateID_t updateId, UINT16 destAddr, UINT32 packetIndex)
{
    if(Samraksh_Emote_Update::s_fBaseStationMode == true) { SOFT_BREAKPOINT(); }

    MFUpdate* pUpdate = MFUpdate::GetUpdate(updateId);
    if( pUpdate == NULL ) {
        SOFT_BREAKPOINT();
        return;
    }

    WP_Message* msg = &g_Samraksh_Emote_Update.m_outboundMessage;

    MFUpdate_Commands::Debugging_MFUpdate_AddPacket* pay = (MFUpdate_Commands::Debugging_MFUpdate_AddPacket*)&g_Samraksh_Emote_Update.m_outboundMessagePayload;
    pay->m_updateHandle = pUpdate->Header.UpdateID;
    pay->m_packetIndex    = packetIndex;

    ASSERT(pUpdate->Providers->Storage != NULL);
    INT32 offset = packetIndex * pUpdate->Header.PacketSize;
    ASSERT(pUpdate->Header.UpdateSize < 64000/*TODO: SIZE OF UPDATE REGION*/);
    ASSERT(offset < pUpdate->Header.UpdateSize);
    INT32 packetDataLen = MIN(((INT32)pUpdate->Header.UpdateSize - offset), pUpdate->Header.PacketSize);
    INT32 readLen = pUpdate->Providers->Storage->Read(pUpdate->StorageHandle, offset, pay->m_packetData, packetDataLen);
    ASSERT(packetDataLen == readLen);
    pay->m_packetLength = readLen;
    pay->m_packetValidation = SUPPORT_ComputeCRC(pay->m_packetData, readLen, 0);

    msg->PrepareRequest(c_Debugging_MFUpdate_AddPacket, 0, (sizeof(MFUpdate_Commands::Debugging_MFUpdate_AddPacket) - 1) + pay->m_packetLength, (UINT8*)pay);

    bool tx_ret = g_Samraksh_Emote_Update.Wireless_Phy_TransmitMessage(&g_Samraksh_Emote_Update, msg);
    if(tx_ret != true) {
        //TODO: tell C-sharp that send failed
		DEBUG_BREAK_SEND_FAIL();
    }
    return;
}


void Samraksh_Emote_Update::SendInstall(UpdateID_t updateId, UINT16 destAddr)
{
    if(Samraksh_Emote_Update::s_fBaseStationMode == true) { SOFT_BREAKPOINT(); }

    MFUpdate* pUpdate = MFUpdate::GetUpdate(updateId);
    if( pUpdate == NULL ) {
        SOFT_BREAKPOINT();
        return;
    }

    WP_Message* msg = &g_Samraksh_Emote_Update.m_outboundMessage;

    MFUpdate_Commands::Debugging_MFUpdate_Install* pay = (MFUpdate_Commands::Debugging_MFUpdate_Install*)&g_Samraksh_Emote_Update.m_outboundMessagePayload;
    pay->m_updateHandle = pUpdate->Header.UpdateID;
    pay->m_updateValidationSize = sizeof(uint);

    ////////////////////////////////////////////////////////////////////
    // TODO: modify IUpdateValidation provider to include creating validation as a callable function
    UINT8 buff[512];
    INT32 len = sizeof(buff);
    INT32 updateSize = pUpdate->Header.UpdateSize;
    INT32 offset = 0;
    UINT32 crc = 0;

    if(pUpdate->Providers->Storage == NULL       ) return;

    while(offset < updateSize)
    {
        if((offset + len) > updateSize)
        {
            len = updateSize - offset;
        }

        pUpdate->Providers->Storage->Read(pUpdate->StorageHandle, offset, buff, len);

        crc = SUPPORT_ComputeCRC(buff, len, crc);

        offset += len;
    }
    /////////////////////////////////////////////////////////////////////

    memcpy(pay->m_updateValidation, &crc, sizeof(crc)); //TODO: consider assigning via shift instead of memcpy
    ASSERT(*((UINT32*)pay->m_updateValidation) == crc);

    msg->PrepareRequest(c_Debugging_MFUpdate_Install, 0, sizeof(MFUpdate_Commands::Debugging_MFUpdate_Install), (UINT8*)pay);

    bool tx_ret = g_Samraksh_Emote_Update.Wireless_Phy_TransmitMessage(&g_Samraksh_Emote_Update, msg);
    if(tx_ret != true) {
		DEBUG_BREAK_SEND_FAIL();
    }
    return;
}
