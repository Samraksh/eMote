// this file contains all code that has been pulled from PortableArrayKitMessage.cpp
// that pertains to wireless MACs

// This code has not been compiled and probably has to be fixed (i.e. Samraksh_emote_update changed to Samraksh_Emote_Update_MAC)

#include <TinyHal.h>
#include <MFUpdate_decl.h>
#include <WireProtocol.h>
#include "..\..\..\Drivers\MFUpdate\Storage\BlockStorageUpdate.h"
//#include "..\..\..\Drivers\MFUpdate\Storage\FS\FSUpdateStorage.h"

#include <TinyCLR_Types.h>   //CLR_RECORD_ASSEMBLY
#include <TinySupport.h>     // SUPPORT_ComputeCRC( payload, payloadSize, 0 );
//#include "../../../CLR/Core/Core.h"
//#include <TinyCLR_Debugging.h> // need TinyCLR_Debugging.h for "struct Debugging_MFUpdate_* packet definitions that should be moved into a shared header file.  including TinyCLR_Debugging.h drags in even more dependencies that are not explicitly included in TinyCLR_Debugging.h!
#include <PAK_decl.h>
//#include <Samraksh/HALTimer.h>
#include <Samraksh/Message.h>
#include <Samraksh/Neighbors.h>


// Mac_decl.h is needed for enum MacNames.  Mac_decl.h should be separated into public API / private definitions.
#include <Samraksh\Mac_decl.h>
#include <Samraksh\MAC.h>
// RF231.h is needed for enum RadioID and enum RadioStateEnum.  enums should be pulled into a public API header file.
#include "..\..\..\Targets\Native\STM32F10x\DeviceCode\drivers\radio\RF231\RF231.h"

extern NeighborTable g_NeighborTable;


WP_Message Samraksh_Emote_Update::s_lastUsbMessage;
void* Samraksh_Emote_Update::s_lastUsbOwner = 0;
BOOL Samraksh_Emote_Update::s_fUseWpPacket = false;
BOOL Samraksh_Emote_Update::s_fRadioOn = false;
BOOL Samraksh_Emote_Update::s_fBaseStationMode = false;
BOOL Samraksh_Emote_Update::s_fPublishUpdateMode = false;
UINT16 Samraksh_Emote_Update::s_destAddr = MAC_BROADCAST_ADDRESS;
UINT32 Samraksh_Emote_Update::s_destMissingPkts[MFUpdate::MAX_MISSING_WORDFIELD_SIZE]; // TODO: static assert 4*MAX_MISSING_WORDFIELD_SIZE must be less than IEEE802_15_4_MAX_PAYLOAD - sizeof(simple_header_t)
UINT8 Samraksh_Emote_Update::s_RadioID = RF231RADIO;


struct Samraksh_Emote_Update_MAC
{
	protected:
    static HAL_COMPLETION s_UpdateCompletion;              //!< used for scheduling installation. only one completion allowed at a time.
    static BOOL           s_UpdateCompletionInitialized;   //!< prevent allocating multiple copies of pending install message on heap.
    static void*          s_UpdateCompletionArg;           //!< save ptr to arg on heap in case completion is aborted.

private:
    bool              m_fInitialized;    //!< singleton pattern.
    bool              m_fMACInitialized;
    bool              m_fMACHandlerInitialized;
    bool              m_fMACInitializedByNativeBeforeInstall;
    WP_Controller     m_controller;      //!< use the WireProtocol engine.
    WP_Message        m_outboundMessage; //!< used for echo and manager

    static WP_Message s_lastUsbMessage;  //!< when basestation, use for constructing correct sequence number in reply to PC.
    static void*      s_lastUsbOwner;    //!< when basestation, use for constructing correct transport comm  in reply to PC.
    UINT8 m_outboundMessagePayload[128];

    static const UINT32 c_Monitor_UpdateInit                       = 0x0000000F; //!< WireProtocol packet type.  Must match duplicate definition in TinyCLR_Debugging.h' CLR_DBG_Commands::
    static const UINT32 c_Monitor_UpdateDeInit                     = 0x00000010; //
    static const UINT32 c_Debugging_MFUpdate_Start                 = 0x00020056; //
    static const UINT32 c_Debugging_MFUpdate_AddPacket             = 0x00020057; //
    static const UINT32 c_Debugging_MFUpdate_Install               = 0x00020058; //
    static const UINT32 c_Debugging_MFUpdate_AuthCommand           = 0x00020059; //
    static const UINT32 c_Debugging_MFUpdate_Authenticate          = 0x00020060; //
    static const UINT32 c_Debugging_MFUpdate_GetMissingPkts        = 0x00020061; //

    Emote_Update_ErrorFlags last_error;


public:
    static const INT32 COMM_BUFFER_SIZE = (IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t));
    //TODO: static_assert(COMM_BUFFER_SIZE == MAC.h's template instantiation size minus the header).

    MACEventHandler PAK_EventHandler;  //!< handle incoming wireless events.
    UINT8 PAK_MacName;                 //!< really enum MacName, aka MacId in other parts of the API.
    UINT8 PAK_routingAppID;
    UINT8 PAK_radioName;
    MACConfig PAK_MacConfig;
    UINT8 PAK_channel;

    UINT8 SendBuffer[Samraksh_Emote_Update::COMM_BUFFER_SIZE];          //!< local buffer for storing outgoing packets.      Regardless of use_wp_packet, internally packets are processed and stored as WP_Messages.
    UINT8 ReceiveBuffer[Samraksh_Emote_Update::COMM_BUFFER_SIZE];       //!< local buffer for manipulating incoming packets. Regardless of use_wp_packet, internally packets are processed and stored as WP_Messages.

    INT32 m_ReceiveState;


    static UINT8  s_RadioID;            //!< MAC API change causes storage of Radio ID outside of MacConfig.
    static BOOL   s_fRadioOn;           //!< indicates whether MAC is initialized.
    static BOOL   s_fUseWpPacket;       //!< determines whether wireless interface will send entire WireProtocol packet.  idea is that short format fits inside single wireless packet payload.
    static BOOL   s_fBaseStationMode;   //!< turn on/off basestation mode.
    static UINT16 s_destAddr;           //!< wireless destination in basestation mode;
    static UINT32 s_destMissingPkts[MFUpdate::MAX_MISSING_WORDFIELD_SIZE];
    static BOOL   s_fPublishUpdateMode; //!< turn on/off update publisher mode
    static UPDATER_PROGRESS_HANDLER s_UpdaterProgressHandler; //!< notify (managed library) something happened.


    static void CreateInstance();
    static void Initialize();
    static bool InitializeMac();
    static bool InitializeMacHandler();
    static bool SetMacHandler();
    static bool InitializeDriversAfterInstall();
    static bool UnInitializeDriversBeforeInstall();
    static void Cleanup();
    static void DeleteInstance();

    static void Receive_IEEE_802_15_4(void* buffer, UINT16 payloadType);
    static void Receive(void* msg, UINT16 size);
    static void SendAck(void* msg, UINT16 size, NetOpStatus status);
    static void NeighborChange(INT16 numberOfNeighbors);

    static bool ReadReceiveBytes( void* state, UINT8*& ptr, UINT32 & size );
    static bool TransmitMessage ( void* state, const WP_Message* msg      );

    static bool App_ProcessHeader ( void* state,  WP_Message* msg );
    static bool App_ProcessPayload( void* state,  WP_Message* msg );
    static bool App_Release       ( void* state,  WP_Message* msg );

    static void SendStart         ( UpdateID_t id, UINT16 destAddr );
    static void SendAuthCommand   ( UpdateID_t id, UINT16 destAddr );
    static void SendAuthenticate  ( UpdateID_t id, UINT16 destAddr );
    static void SendGetMissingPkts( UpdateID_t id, UINT16 destAddr );
    static void SendAddPacket     ( UpdateID_t id, UINT16 destAddr, UINT32 packetIndex );
    static void SendInstall       ( UpdateID_t id, UINT16 destAddr );
    static void HandleTimeout();

    // Pull MFUpdate functions from Debugger.cpp into this library to handle Wired + Wireless
    static bool UpdateInit           ( WP_Message* msg, void* owner );
    static bool UpdateDeInit         ( WP_Message* msg, void* owner );
    static bool Start                ( WP_Message* msg, void* owner );
    static bool AuthCommand          ( WP_Message* msg, void* owner );
    static bool Authenticate         ( WP_Message* msg, void* owner );
    static bool GetMissingPkts       ( WP_Message* msg, void* owner );
    static bool AddPacket            ( WP_Message* msg, void* owner );
    static bool Install              ( WP_Message* msg, void* owner );

    static void SetUpdateProgressCallback(UPDATER_PROGRESS_HANDLER*);

protected:
    static void UpdateCompletion(void *arg);
}

struct NeighborUpdateRecord
{
	static const unsigned int MAX_MISSING_WORDFIELD_SIZE = 25;
	UINT16 destAddr;
    UINT32 m_neighborMissingPkts[MAX_MISSING_WORDFIELD_SIZE];
    UINT32 UpdateID;
};


#if defined(LEGACY_RECEIVE_HANDLER)
UINT8 MacReceiveBuffer[Samraksh_Emote_Update::COMM_BUFFER_SIZE +16]; //!< extra buffer because MAC inserts size into first two bytes instead of passing as a parameter, so passing the ReceiveBuffer to MAC would make ReceiveBuffer incompatible with its use as a regular payload buffer elsewhere.  Also I don't know why the design decision was made that MAC_GetNextPacket writes metadata to the end of the buffer instead of passing them as parameters like the old API.
#endif

/**
 * MULTIPLEX_PACKETS() break out of command handler if operating as base-station pass through.
 */
#define MULTIPLEX_PACKETS() \
    if( g_Samraksh_Emote_Update.s_fBaseStationMode && owner != &g_Samraksh_Emote_Update ) { \
        s_lastUsbMessage = *msg; /* copy WP_Packet to construct the reply header later. FIXME: may need queue + lookup if messages out of sync.*/ \
        s_lastUsbOwner = owner; /* copy owner so reply goes out over same interface. */ \
        /* TODO: create new flag to avoid hijacking the c_NonCritical flag.*/ \
        msg->m_header.m_flags |= WP_Flags::c_NonCritical;/* mark packet so the CLR_Messaging::ProcessPayload line 480 does not cause re-transmit (short-circuit CLR_Messaging::ReplyToCommand()). similarly, this file's ReplyToCommand does not check c_NonCritical to allow sending over USB.*/ \
        bool tx_ret =  g_Samraksh_Emote_Update.Wireless_Phy_TransmitMessage(&g_Samraksh_Emote_Update, msg); \
        return tx_ret; /* wireless pass-through so immediately return without performing update on this device. */ \
    }



/**
 * used to simulate WireProtocol reading out of wireless buffer.
 */
bool Samraksh_Emote_Update::Wireless_Phy_ReadReceiveBytes( void* state, UINT8*& ptr, UINT32 & size ) {


    switch(g_Samraksh_Emote_Update.m_ReceiveState) {
    case 0:/*Idle*/
        break;
    case 1:/*Initialize*/
        break;
    case 2:/*WaitingForHeader*/
        memcpy(ptr, g_Samraksh_Emote_Update.ReceiveBuffer, sizeof(WP_Packet));
        size = sizeof(WP_Packet);
        g_Samraksh_Emote_Update.m_ReceiveState = 3;
        break;
    case 3:/*ReadingHeader*/
        size = 0;
        g_Samraksh_Emote_Update.m_ReceiveState = 5;
        break;
    case 4:/*CompleteHeader*/
        break;
    case 5:/*ReadingPayload*/
        memcpy(ptr, g_Samraksh_Emote_Update.ReceiveBuffer+sizeof(WP_Packet), size);
        size = 0;
        break;
    default:
        g_Samraksh_Emote_Update.m_ReceiveState = 0;
        break;
    }
    return true;
}

/**
 * send a message, using the selected wireless payload format.
 */
bool Samraksh_Emote_Update::Wireless_Phy_TransmitMessage( void* state, const WP_Message* msg ) {
    DeviceStatus ds_ret = DS_Fail;

    UINT8 currentMACID = MAC_GetID();
    if(currentMACID != NONE) {

		// FIXME: have to fake broadcast for OMAC. pick random neighbor each time.
		UINT16 local_destAddr = g_Samraksh_Emote_Update.s_destAddr;

		if( local_destAddr == MAC_BROADCAST_ADDRESS
			&& currentMACID == OMAC )
		{
			UINT16 neighbor_addrs[MAX_NEIGHBORS];
			TINYCLR_CLEAR(neighbor_addrs);
			MAC_GetNeighborList(neighbor_addrs);
			const UINT16 BAD_NEIGHBOR_ADDRESS = 0;
			if(neighbor_addrs[0] != BAD_NEIGHBOR_ADDRESS)
			{
				static UINT32 rand_neighbor_idx;
				do
				{
					rand_neighbor_idx = (rand_neighbor_idx + 1) % MAX_NEIGHBORS;
				}while (neighbor_addrs[rand_neighbor_idx] == BAD_NEIGHBOR_ADDRESS);
				local_destAddr = neighbor_addrs[rand_neighbor_idx];
			}
		}

		//need to combine buffers based on header structure.
		if(g_Samraksh_Emote_Update.s_fUseWpPacket) { //also could get g_Sam... through *state.
			// send full WiredProtocol packet over wireless interface.
			((UINT32*)g_Samraksh_Emote_Update.SendBuffer)[0] = msg->m_header.m_cmd;
			memcpy(g_Samraksh_Emote_Update.SendBuffer, &msg->m_header, sizeof(WP_Packet));
			memcpy(g_Samraksh_Emote_Update.SendBuffer + sizeof(WP_Packet), msg->m_payload,  (msg->m_header.m_size > sizeof(UINT8)*Samraksh_Emote_Update::COMM_BUFFER_SIZE) ? sizeof(UINT8)*Samraksh_Emote_Update::COMM_BUFFER_SIZE - sizeof(WP_Packet) : msg->m_header.m_size);
			ds_ret = MAC_Send(/*g_Samraksh_Emote_Update.PAK_MacName,*/ local_destAddr, g_Samraksh_Emote_Update.PAK_routingAppID, (void*)g_Samraksh_Emote_Update.SendBuffer, sizeof(WP_Packet) + msg->m_header.m_size);

		}
		else {
			// send shorter packet over wireless interface.
			//TODO: split message bigger than IEEE802_15_4_MAX_PAYLOAD into multiple messages. 
			size_t shortHeaderSize = sizeof(UINT32 /*m_cmd*/) + sizeof(UINT32 /*m_flags*/);
			((simple_payload_t*)g_Samraksh_Emote_Update.SendBuffer)->cmd = msg->m_header.m_cmd;
			((simple_payload_t*)g_Samraksh_Emote_Update.SendBuffer)->flags = msg->m_header.m_flags;
			memcpy(&((simple_payload_t*)g_Samraksh_Emote_Update.SendBuffer)->payload, msg->m_payload, (msg->m_header.m_size > sizeof(UINT8)*Samraksh_Emote_Update::COMM_BUFFER_SIZE) ? /*FIXME: just throw error and support splitting big packets later.*/ sizeof(UINT8)*Samraksh_Emote_Update::COMM_BUFFER_SIZE - offsetof(simple_payload_t, payload) : msg->m_header.m_size);
			ds_ret = MAC_Send(/*g_Samraksh_Emote_Update.PAK_MacName,*/ local_destAddr, g_Samraksh_Emote_Update.PAK_routingAppID, (void*)g_Samraksh_Emote_Update.SendBuffer, shortHeaderSize + msg->m_header.m_size);
		}
    }

    return (ds_ret == DS_Success);
}

/**
 * process a payload coming in over wireless... this is similar to the Debugger
 * inspecting the header to decide what to call, but it's re-implemented here
 * because the wireless interface comes directly here.  If we remove the Debugger,
 * this can help process incoming wired interface calls, too.
 */
bool Samraksh_Emote_Update::App_ProcessPayload(void* state, WP_Message* msg )
{
    bool ret = false;
    if( msg->m_header.m_flags & WP_Flags::c_Reply )
    {
        if( msg->m_header.m_flags & WP_Flags::c_ACK )
        {
            //////////////////////////////////////////////////////////////////
            // UPDATE NEIGHBOR STATE FROM REPLY
            //////////////////////////////////////////////////////////////////
            MFUpdate* updateInfo = NULL;
            switch(msg->m_header.m_cmd)
            {
            case c_Debugging_MFUpdate_Start:
            {
                MFUpdate_Commands::Debugging_MFUpdate_Start::Reply* incomingReply = (MFUpdate_Commands::Debugging_MFUpdate_Start::Reply* )msg->m_payload;
                CHECK_HANDLE(incomingReply->m_updateHandle);
                updateInfo = g_Updates->GetUpdate(incomingReply->m_updateHandle);
                CHECK_PTR(updateInfo);
                updateInfo->Flags |= NEIGHBOR_FLAGS__START;
                Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr, START_ACK, 0);
                if(s_fPublishUpdateMode == true) {
                    if( incomingReply->m_success == 1)
                    {
                        SendAuthCommand(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr);
                    }
                    else
                    {
                        //TODO: create and read reason for failure
                        SOFT_BREAKPOINT();
                    }
                }
            }
                break;
            case c_Debugging_MFUpdate_AddPacket:
            {
                MFUpdate_Commands::Debugging_MFUpdate_AddPacket::Reply* incomingReply = (MFUpdate_Commands::Debugging_MFUpdate_AddPacket::Reply* )msg->m_payload;
                CHECK_HANDLE(incomingReply->m_updateHandle);
                updateInfo = g_Updates->GetUpdate(incomingReply->m_updateHandle);
                CHECK_PTR(updateInfo);

                updateInfo->Flags |= NEIGHBOR_FLAGS__ADDPACKET;

                int idx_word = incomingReply->m_packetIndex / 32;
                int idx_bit = incomingReply->m_packetIndex % 32;
                s_destMissingPkts[idx_word] = s_destMissingPkts[idx_word] | (1u << idx_bit);

                Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr, ADDPACKET_ACK, incomingReply->m_nextMissingPacketIndex);
                // TODO: record next missing packet inside update struct, allow querying from c-sharp.
                if(s_fPublishUpdateMode == true) {
                    if( incomingReply->m_success == 1 )
                    {
                        if( incomingReply->m_nextMissingPacketIndex < updateInfo->m_finalPacketIdx )
                        {
                            SendAddPacket(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr, incomingReply->m_nextMissingPacketIndex);
                        }
                        else
                        {
                            SendGetMissingPkts(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr);
                        }
                    }
                    else {
                        //TODO: handle failure.
                    }
                }
                else {
                    SOFT_BREAKPOINT(); // got a reply about an unknown update.
                }
            }
                break;
            case c_Debugging_MFUpdate_Install:
            {
                MFUpdate_Commands::Debugging_MFUpdate_Install::Reply* incomingReply = (MFUpdate_Commands::Debugging_MFUpdate_Install::Reply* )msg->m_payload;
                CHECK_HANDLE(incomingReply->m_updateHandle);
                updateInfo = g_Updates->GetUpdate(incomingReply->m_updateHandle);
                CHECK_PTR(updateInfo);
                //TODO: handle negative install message (and reason).
                //TODO: if negative, try to send a start message to resend Authenticate and fix the problem.
                updateInfo->Flags |= NEIGHBOR_FLAGS__INSTALL;
                Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr/*dest from received packet*/, INSTALL_ACK, 0);
            }
                break;
            case c_Debugging_MFUpdate_AuthCommand:
            {
                MFUpdate_Commands::Debugging_MFUpdate_AuthCommand::Reply* incomingReply = (MFUpdate_Commands::Debugging_MFUpdate_AuthCommand::Reply* )msg->m_payload;
                CHECK_HANDLE(incomingReply->m_updateHandle);
                updateInfo = g_Updates->GetUpdate(incomingReply->m_updateHandle);
                CHECK_PTR(updateInfo);

                updateInfo->Flags |= NEIGHBOR_FLAGS__AUTHCMD;
                Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr, AUTHCMD_ACK, 0);
                if(s_fPublishUpdateMode == true) {
                    //TODO: implement authentication
                    SendAuthenticate(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr);
                }
            }
                break;
            case c_Debugging_MFUpdate_Authenticate:
            {
                MFUpdate_Commands::Debugging_MFUpdate_Authenticate::Reply* incomingReply = (MFUpdate_Commands::Debugging_MFUpdate_Authenticate::Reply* )msg->m_payload;
                CHECK_HANDLE(incomingReply->m_updateHandle);
                updateInfo = g_Updates->GetUpdate(incomingReply->m_updateHandle);
                CHECK_PTR(updateInfo);

                updateInfo->Flags |= NEIGHBOR_FLAGS__AUTHENTICATED;
                Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr, AUTHENTICATE_ACK, 0);
                if(s_fPublishUpdateMode == true) {
                    SendGetMissingPkts(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr);
                }
            }
                break;
            case c_Debugging_MFUpdate_GetMissingPkts:
            {
                MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts::Reply* incomingReply = (MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts::Reply*) msg->m_payload;
                CHECK_HANDLE(incomingReply->m_updateHandle);
                updateInfo = g_Updates->GetUpdate(incomingReply->m_updateHandle);
                CHECK_PTR(updateInfo);

                updateInfo->Flags |= NEIGHBOR_FLAGS__GETMISSINGPKTS;

                //TODO: API for tracking neighbor, externalize for multiple neighbors.
                //TODO: lookup neighbor info.
                //TODO: warn if we thought neighbor had more packets than it reports.
                for(UINT32 i = 0; i < updateInfo->m_missingPktsWordfieldSize; i++) {
                    s_destMissingPkts[i] = incomingReply->m_missingPkts[i];
                }

                Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr, GETMISSINGPKTS_ACK, 0);
                if(s_fPublishUpdateMode == true) {
                    UINT32 missingNo = GetFirstMissingPacket(updateInfo, incomingReply->m_missingPkts);
                    if( ( incomingReply->m_missingPktCount == 0 ) || ( missingNo > updateInfo->m_finalPacketIdx ) ) {
                        SendInstall(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr);
                    }
                    else {
                        SendAddPacket(updateInfo->Header.UpdateID, Samraksh_Emote_Update::s_destAddr, missingNo);
                    }
                }
            }
                break;
            default:
            {
                SOFT_BREAKPOINT();
                return false;
            }
                break;
            }
        }


        //////////////////////////////////////////////////////////////////////
        // HANDLE USB BASESTATION MODE REPLY FORWARDING
        //////////////////////////////////////////////////////////////////////
        // forward here because incoming message could be multi-part WP message on wireless transport or simple message header on wireless transport.
        if(s_fBaseStationMode == true) {
            if(s_fUseWpPacket == true) {
                ASSERT(s_lastUsbMessage.m_parent != NULL);
                // packet already has all needed header parts computed. entire original message was passed wireless and is the correct reply.
                ret = s_lastUsbMessage.m_parent->SendProtocolMessage(*msg);
            }
            else {
                // packet header is simple and missing info that we have saved from the last incoming packet.
                // TODO: make a queue of received packets, look it up. otherwise try to assert incoming wireless reply matches saved USB packet.
                if(s_lastUsbMessage.m_header.m_cmd == msg->m_header.m_cmd) {
                    ForwardReplyToCommand(&s_lastUsbMessage, msg->m_header.m_flags, (void*)msg->m_payload, msg->m_header.m_size );
                }
                else {
                    g_Samraksh_Emote_Update.last_error = UPDATE_INSTALL_MSG_OUT_OF_SYNC; // may need to add queue for messages.
                    SOFT_BREAKPOINT();
                }
            }
        }
    }
    else {
        //////////////////////////////////////////////////////////////////////
        // OBEY WIRELESS COMMAND
        //////////////////////////////////////////////////////////////////////
        switch(msg->m_header.m_cmd) {
        case c_Monitor_UpdateInit:
            SOFT_BREAKPOINT(); // shouldn't be here.
            break;
        case c_Monitor_UpdateDeInit:
            SOFT_BREAKPOINT(); // shouldn't be here.
            break;
        case c_Debugging_MFUpdate_Start:
            Samraksh_Emote_Update::Start(msg, &g_Samraksh_Emote_Update);
            break;
        case c_Debugging_MFUpdate_AddPacket:
            Samraksh_Emote_Update::AddPacket(msg, &g_Samraksh_Emote_Update);
            break;
        case c_Debugging_MFUpdate_Install:
            Samraksh_Emote_Update::Install(msg, &g_Samraksh_Emote_Update);
            break;
        case c_Debugging_MFUpdate_AuthCommand:
            Samraksh_Emote_Update::AuthCommand(msg, &g_Samraksh_Emote_Update);
            break;
        case c_Debugging_MFUpdate_Authenticate:
            Samraksh_Emote_Update::Authenticate(msg, &g_Samraksh_Emote_Update);
            break;
        case c_Debugging_MFUpdate_GetMissingPkts:
            Samraksh_Emote_Update::GetMissingPkts(msg, &g_Samraksh_Emote_Update);
            break;
        default:
            SOFT_BREAKPOINT();
            return false;
            break;
        }
    }
    return ret;
}

#if defined(LEGACY_RECEIVE_HANDLER)
/**
 * Implement wireless MacReceiveFuncPtrType.  MAC API changed mid-development.
 * This is a callback.  For CSMAMAC, arg1 corresponds to the number of messages in the CSMAMAC's internal buffer.
 */
void OldReceiveHandler(UINT16 numberOfPackets) {

    //TODO: put these on a queue so we're not holding up the ReceiveHandler.  Use continuations. ARGH.
    //Process Packets loop.
    int i = 0;
    for(; i < numberOfPackets; i++) {
        // MAC API is not at all friendly to native code...
        UINT8* p_MacReceiveBuffer = &(g_Samraksh_Emote_Update.MacReceiveBuffer[0]);
        DeviceStatus stat = MAC_GetNextPacket(&p_MacReceiveBuffer);  //MAC_GetNextPacket returns a buffer with first two bytes being the size.  "managedBuffer" is a misnomer.
        if(stat == DS_Success) {
            UINT16 Size = (g_Samraksh_Emote_Update.MacReceiveBuffer[0] & 0xff) | ((g_Samraksh_Emote_Update.MacReceiveBuffer[1] << 8) & 0xff00);
            g_Samraksh_Emote_Update.Receive(&(g_Samraksh_Emote_Update.MacReceiveBuffer[2]), Size);
        }
        else {
            SOFT_BREAKPOINT();
        }
    }
}
#endif

/**
 * New MAC API changed again.
 */
void Samraksh_Emote_Update::Receive_IEEE_802_15_4(void* buffer, UINT16 payloadType) {
	Message_15_4_t* msg = (Message_15_4_t*)buffer;
	//TODO: basic s_destAddr checking...
	ASSERT_SP(g_Samraksh_Emote_Update.s_destAddr = msg->GetHeader()->src);
	if(g_Samraksh_Emote_Update.s_destAddr != msg->GetHeader()->src) {
		// switching destination; clean neighbor update record.
		memset(g_Samraksh_Emote_Update.s_destMissingPkts, 0xFF, sizeof(UINT32)*MFUpdate::MAX_MISSING_WORDFIELD_SIZE );
	}
	g_Samraksh_Emote_Update.s_destAddr = msg->GetHeader()->src;
	size_t sz_payload = (size_t)msg->GetPayloadSize();
	ASSERT_SP(sz_payload < 256);
	UINT8* aligned_payload = (UINT8*) private_malloc(sz_payload);
	UINT8* unaligned_payload = msg->GetPayload();
	memcpy(aligned_payload, unaligned_payload, sz_payload);
	Receive(aligned_payload, sz_payload);
	private_free(aligned_payload);
}

/**
 * Implement wireless SendAckFuncPtrType.  MAC API changed mid-development, so this is glue.
 * FIXME: cut out the middle man.
 * BK: SendAck is not doing anything either. I am not sure what behaviour is expected. However, I am implemenmting what is expected by the MAC
 */
void SendAckHandler (void* msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus){
	g_Samraksh_Emote_Update.SendAck(msg,size,status);
	Message_15_4_t* packet_ptr = static_cast<Message_15_4_t*>(msg);
	g_NeighborTable.DeletePacket(packet_ptr);
	return;

}


void Samraksh_Emote_Update::SendAck(void *msg, UINT16 size, NetOpStatus status){
    if(status==NetworkOperations_Success) {

    }
    else {

    }
}




/**
 * Implement wireless NeighborChangeFuncPtrType. MAC API changed mid-development, so this is glue.
 * //FIXME: cut out the middle man if possible.
 */
void NeighborChangeHandler (INT16 numberOfNeighbors) {
    return g_Samraksh_Emote_Update.NeighborChange(numberOfNeighbors);
}


void Samraksh_Emote_Update::NeighborChange(INT16 numberOfNeighbors) {

}


// TODO: check if MAC is already initialized.
bool Samraksh_Emote_Update::InitializeMac() {
    bool ret = TRUE;

    if(Samraksh_Emote_Update::s_fRadioOn == TRUE) return TRUE;

    if(g_Samraksh_Emote_Update.m_fMACInitialized == TRUE) return TRUE;

    //TODO: remember whether I turned on the radio so I can turn it off during UnInit.
    //////////////////////////// Radio Initialize.  See Samraksh_SPOT_Net_Samraksh_SPOT_Net_MACBase.cpp's MACBase::InternalReConfigure(...).
    //TODO: Probe for active network and piggy-back on it as an app.
    //TODO: test whether radio is already initialized.
    // For now, configure CSMAMAC and use the C# Interop's App ID because MAC is broken and hard-coded to 3.
    CPU_SPI_Initialize();  //TODO: test whether CPU_SPI_Initialize() needs to implement run-once pattern because a few drivers call it.
    Samraksh_Emote_Update::InitializeMacHandler();
    g_Samraksh_Emote_Update.PAK_MacName = /*MacNames::*/ OMAC; //CSMAMAC;
    // g_Samraksh_Emote_Update.PAK_MacConfig.Network = 138;
    g_Samraksh_Emote_Update.PAK_MacConfig.CCA = true;
    g_Samraksh_Emote_Update.PAK_MacConfig.NumberOfRetries = 0;
    g_Samraksh_Emote_Update.PAK_MacConfig.CCASenseTime = 140;
    g_Samraksh_Emote_Update.PAK_MacConfig.BufferSize = 8;
    // g_Samraksh_Emote_Update.PAK_MacConfig.RadioID = /*RadioID::*/RF231RADIO;  //WHY IS THE RADIOID IN THE CONFIG ALSO PASSED AS A PARAMETER???
    g_Samraksh_Emote_Update.PAK_MacConfig.NeighborLivenessDelay = 620;
    g_Samraksh_Emote_Update.PAK_channel = 0xE;


    //PROTOTYPE: DeviceStatus Mac_Initialize(MacEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioName, void* macConfig);
    if(MAC_Initialize(&(g_Samraksh_Emote_Update.PAK_EventHandler), g_Samraksh_Emote_Update.PAK_MacName, /*g_Samraksh_Emote_Update.PAK_routingAppID*/3, Samraksh_Emote_Update::s_RadioID, (void*) &g_Samraksh_Emote_Update.PAK_MacConfig) != DS_Success)
    {
        SOFT_BREAKPOINT();
        hal_printf("%s: ERROR! Mac_Initialize failed.\n", __func__);
        ret = FALSE;
    }
    if(InitializeMacHandler() != TRUE) {
        SOFT_BREAKPOINT();
        hal_printf("%s: ERROR! InitializeMacHandler failed.\n", __func__);
        ret = FALSE;
    }
    if(CPU_Radio_ChangeTxPower(Samraksh_Emote_Update::s_RadioID, /*WTF, only enumerated in Radio_802_15_4_Base.cs?*/ 0x0/*Power_0Point7dBm:0x5*/ ) != DS_Success)
    {
        SOFT_BREAKPOINT();
        hal_printf("%s: ERROR! CPU_Radio_ChangeTxPower failed.\n", __func__);
        ret = FALSE;
    }
    if(CPU_Radio_ChangeChannel(Samraksh_Emote_Update::s_RadioID, g_Samraksh_Emote_Update.PAK_channel) != DS_Success)
    {
        SOFT_BREAKPOINT();
        hal_printf("%s: ERROR! CPU_Radio_ChangeChannel failed.\n", __func__);
        ret = FALSE;
    }

    Samraksh_Emote_Update::s_fRadioOn = ret;
    g_Samraksh_Emote_Update.m_fMACInitialized = TRUE;
    return ret;
}

/**
 * allow receiving messages when MAC and radio are initialized by C-sharp app. in that case, native initialization code must call this after Initialize.
 * not automatically part of Initialize() to make it easier to change in the future such that incoming update messages over radio may be ignored.
 */
bool Samraksh_Emote_Update::SetMacHandler() {
	bool ret = true;
	if(g_Samraksh_Emote_Update.m_fInitialized == true) {
		if(g_Samraksh_Emote_Update.m_fMACHandlerInitialized != true) {
			Samraksh_Emote_Update::InitializeMacHandler();
		}
		ret = MAC<Message_15_4_t, MACConfig>::SetNativeAppHandlers((UINT8)MFM_UPDATE, &(g_Samraksh_Emote_Update.PAK_EventHandler));
	}

	return ret;
}

bool Samraksh_Emote_Update::InitializeMacHandler() {
	g_Samraksh_Emote_Update.PAK_EventHandler.SetReceiveHandler(&Receive_IEEE_802_15_4);
	g_Samraksh_Emote_Update.PAK_EventHandler.SetSendAckHandler(&SendAckHandler);
	g_Samraksh_Emote_Update.PAK_EventHandler.SetNeighborChangeHandler(&NeighborChangeHandler);
	g_Samraksh_Emote_Update.m_fMACHandlerInitialized = true;
	return true;
}

bool InitializeInterface(){	    
    Samraksh_Emote_Update::s_fRadioOn = false;
    Samraksh_Emote_Update::s_fUseWpPacket = false;
    Samraksh_Emote_Update::s_fBaseStationMode = false;
    Samraksh_Emote_Update::s_destAddr = MAC_BROADCAST_ADDRESS;
    g_Samraksh_Emote_Update.last_error = UPDATE_INSTALL_NO_MSG;
	g_Samraksh_Emote_Update.PAK_routingAppID = MFM_UPDATE;

    
    g_Samraksh_Emote_Update.m_fMACInitialized = false;
    return true;
}

bool InitializeDriversAfterInstall(){
	bool ret = true;
    ret |= HAL_Time_Initialize();
    if(g_Samraksh_Emote_Update.m_fMACInitializedByNativeBeforeInstall == true) {
        ret |= InitializeMac();
    }
    return ret;
}

bool UnInitializeDriversBeforeInstall(){
	bool ret = true;

    if( g_Samraksh_Emote_Update.m_fMACInitialized == TRUE ) {
        g_Samraksh_Emote_Update.m_fMACInitializedByNativeBeforeInstall = true;
    }

    // always try to uninitialize the MAC even if we did not initialize it.
    if( MAC_UnInitialize( /*g_Samraksh_Emote_Update.PAK_MacName*/ ) != DS_Success ) {
        // (TODO: make the "safe to install" check a separate function so the callback could check, as well as allowing the PC to query what's blocking installs.
        g_Samraksh_Emote_Update.last_error = UPDATE_INSTALL_FAIL_RADIO;
        SOFT_BREAKPOINT();
        ret = FALSE;
    }
    else {
        Samraksh_Emote_Update::s_fRadioOn = FALSE;
        g_Samraksh_Emote_Update.m_fMACInitialized = FALSE;
    }

    if( HAL_Time_Uninitialize() != TRUE ) {
        g_Samraksh_Emote_Update.last_error = UPDATE_INSTALL_FAIL_TIME;
        SOFT_BREAKPOINT();
        ret = FALSE;
    }

    return ret;
}


