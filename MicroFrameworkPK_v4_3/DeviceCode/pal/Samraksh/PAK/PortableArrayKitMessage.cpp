/**
 * PortableArrayKitMessage.cpp
 *
 *  Created on: Apr 1, 2013
 *  Updated on: March 31, 2014 - use new MAC APIs.
 *      Author: Michael Andrew McGrath
 */

/**
 * synopsis:
 * The overall architecture of this file is to maintain processing compatibility between
 * Debugger's MFUpdate and new Wireless MFUpdate, so an incoming MFUpdate command
 * is processed only here.  This allows MFUpdate usage over wired+wireless, while
 * allowing migration to "wired MFUpdate without debugger" if debugger is
 * removed for RTM build or security reasons.
 *
 * Therefore, this file is a low-level way to interact with MFUpdate.
 * USB Debugger messages about updates go here and on to MFUpdate.
 * Wireless messages about updates go here and on to MFUpdate.
 * The USB Debugger messages go here as a way to echo them to wireless.
 */

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

//////////////////////////////////////////////////////////////////////////////
// FILE-SCOPE SIGNATURES
//////////////////////////////////////////////////////////////////////////////

void OldReceiveHandler(UINT16 numberOfPackets);
UINT32 GetFirstMissingPacket(MFUpdate* updateInfo);  //<! use when estimating this node's missing packets
UINT32 GetFirstMissingPacket(MFUpdate* updateInfo, UINT32* missingPkts ); //<! use when estimating neighbor's missing packets

void /*CLR_Messaging::*/ReplyToCommand( WP_Message* msg, bool fSuccess, bool fCritical, void* ptr, int size );
void ForwardReplyToCommand( WP_Message* msg, UINT32 flags, void* ptr, int size);

//////////////////////////////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////////////////////////////

#define DEBUG_EMOTE_UPDATE
#if defined(DEBUG_EMOTE_UPDATE) && defined(DEBUG)
//#define ENABLE_PIN(x,y) CPU_GPIO_EnableOutputPin(x,y)
//#define SET_PIN(x,y) CPU_GPIO_SetPinState(x,y)
#define DEBUG_PRINTF_EMOTE_UPDATE(x) hal_printf(x)
#define ASSERT_UPDATE_PROTOCOL(x) { (!(x)) ? SOFT_BREAKPOINT() : __NOP() ; }
#else
//#define ENABLE_PIN(x,y)
//#define SET_PIN(x,y)
#define DEBUG_PRINTF_EMOTE_UPDATE(x)       __NOP()
#define ASSERT_UPDATE_PROTOCOL(x)          __NOP()
#endif

#if !defined(NATIVE_PROFILE_CLR_DEBUGGER)
#define NATIVE_PROFILE_CLR_DEBUGGER
#endif
#ifndef TINYCLR_CLEAR
#define TINYCLR_CLEAR(ref) { memset( &ref, 0, sizeof(ref) ); }
#endif

#define MIN(a, b) (((a) < (b))?(a):(b))

#if defined(DEBUG_EMOTE_UPDATE_SEND)
#define DEBUG_BREAK_SEND_FAIL() SOFT_BREAKPOINT();
#else
#define DEBUG_BREAK_SEND_FAIL() {__ASM volatile ("nop");}
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

//TODO: use packet header identifier to show whether full wire protocol packet or WP packet in wireless transmissions.
// use MSPktV1 instead unless we modify WireProtocol.h
//#define MARKER_SAMRAKSH_V1 "SMRKSH1"

//extern CLR_DBG_Debugger *g_CLR_DBG_Debuggers;  //from Core.cpp, instead this is passed in as void* owner argument.
//extern HALTimerManager gHalTimerManagerObject;

//CLR_UINT32     g_scratchMessaging[ sizeof(Samraksh_Emote_Update) / sizeof(UINT32) + 1 ];

//////////////////////////////////////////////////////////////////////////////
// DEFINE STORAGE
//////////////////////////////////////////////////////////////////////////////

HAL_COMPLETION Samraksh_Emote_Update::s_UpdateCompletion;
void*          Samraksh_Emote_Update::s_UpdateCompletionArg;
WP_Message Samraksh_Emote_Update::s_lastUsbMessage;
void* Samraksh_Emote_Update::s_lastUsbOwner = 0;
BOOL Samraksh_Emote_Update::s_fUseWpPacket = false;
BOOL Samraksh_Emote_Update::s_fRadioOn = false;
BOOL Samraksh_Emote_Update::s_fBaseStationMode = false;
BOOL Samraksh_Emote_Update::s_fPublishUpdateMode = false;
UINT16 Samraksh_Emote_Update::s_destAddr = MAC_BROADCAST_ADDRESS;
UINT32 Samraksh_Emote_Update::s_destMissingPkts[MFUpdate::MAX_MISSING_WORDFIELD_SIZE]; // TODO: static assert 4*MAX_MISSING_WORDFIELD_SIZE must be less than IEEE802_15_4_MAX_PAYLOAD - sizeof(simple_header_t)
UINT8 Samraksh_Emote_Update::s_RadioID = RF231RADIO;
UPDATER_PROGRESS_HANDLER Samraksh_Emote_Update::s_UpdaterProgressHandler = 0;

Samraksh_Emote_Update g_Samraksh_Emote_Update;

//////////////////////////////////////////////////////////////////////////////
// HELPERS
//////////////////////////////////////////////////////////////////////////////
 /**
  * @brief check whether update packet number idx has been received.
  * @detailed convenience function, until Packets are moved to an OO-design.
  * @param idx index of individual packet in the update.
  * @param field array where each bit corresponds to the status of a packet. bit value zero means the packet is safely stored.
  */
inline bool have_packet(int idx, const unsigned* field) {
    return ((field[idx >> 5] & (1ul << (idx % 32))) == 0);
}

/**
 * @brief mark that packet number idx is missing.
 * @detailed convenience function, until Packets are moved to an OO-design.
 * @param idx index of individual packet that is stored locally
 * @param field array where each set bit corresponds to the status of a packet. bit value zero means the packet is safely stored.
 * @return for now ignore this.
 */
inline void set_packet_as_received(int idx, unsigned* field) {
    field[idx >> 5] &= ~(1ul << (idx % 32));
}


/////////////////////////////////////////////////////////////////////////////
// Implement WP_PhysicalLayer to provide to WP_Controller.
// Selected functions to match prototypes in CLR/Include/Messaging.h
// messaging interface so the same MFUpdate code may call either the wired
// Debugger messaging interface or the wireless interface.
// Note type WP_Packet is just the header.
/////////////////////////////////////////////////////////////////////////////

const WP_PhysicalLayer c_Update_phy =
{
    &Samraksh_Emote_Update::Wireless_Phy_ReadReceiveBytes,
    &Samraksh_Emote_Update::Wireless_Phy_TransmitMessage,
};

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

/////////////////////////////////////////////////////////////////////////////
//// Commands defined in TinyCLR_Debugging.h
//// Start, AuthCommand, Authenticate, GetMissingPkts, AddPackets, Install
//// Copied here in case we remove debugger but keep Update.
//// Copied here to prevent resolving all of TinyCLR_Debugging.h dependencies.
//// Consider separating commands into other header file included in both places.
/////////////////////////////////////////////////////////////////////////////
struct MFUpdate_Commands
{

struct Debugging_MFUpdate_Start
{
    char       m_provider[64];
    UpdateID_t m_updateId;
    CLR_UINT32 m_updateType;
    CLR_UINT32 m_updateSubType;
    CLR_UINT32 m_updateSize;
    CLR_UINT32 m_updatePacketSize;
    CLR_UINT16 m_versionMajor;
    CLR_UINT16 m_versionMinor;

    struct Reply
    {
        UpdateID_t m_updateHandle;
        CLR_INT32  m_success;
    };
};

struct Debugging_MFUpdate_AuthCommand
{
    CLR_UINT32 m_updateHandle;
    CLR_UINT32 m_authCommand;
    CLR_UINT32 m_authArgsSize;
    CLR_UINT8  m_authArgs[1];

    struct Reply
    {
        CLR_UINT32 m_updateHandle;
        CLR_INT32  m_success;
        CLR_UINT32 m_responseSize;
        CLR_UINT8  m_response[1];
    };
};

struct Debugging_MFUpdate_Authenticate
{
    CLR_UINT32 m_updateHandle;
    CLR_UINT32 m_authenticationLen;
    CLR_UINT8  m_authenticationData[1];

    struct Reply
    {
        CLR_UINT32 m_updateHandle;
        CLR_INT32 m_success;
    };
};

struct Debugging_MFUpdate_GetMissingPkts
{
    CLR_UINT32 m_updateHandle;

    struct Reply
    {
        CLR_UINT32 m_updateHandle;
        CLR_INT32  m_success;
        CLR_INT32  m_missingPktCount;
        CLR_UINT32 m_missingPkts[1];
    };
};

struct Debugging_MFUpdate_AddPacket
{
    CLR_UINT32 m_updateHandle;
    CLR_UINT32 m_packetIndex;
    CLR_UINT32 m_packetValidation;
    CLR_UINT32 m_packetLength;
    CLR_UINT8 m_packetData[1];

    struct Reply
    {
        CLR_UINT32 m_updateHandle;
        CLR_INT32  m_success;
        CLR_UINT32 m_packetIndex;
        CLR_UINT32 m_nextMissingPacketIndex;
    };
};

#define MAX_UPDATE_VALIDATION_SIZE_IN_BYTES 8

struct Debugging_MFUpdate_Install
{
    CLR_UINT32 m_updateHandle;
    CLR_UINT32 m_updateValidationSize; //<! size in bytes of m_updateValidation
    CLR_UINT8  m_updateValidation[1];

    struct Reply
    {
        CLR_UINT32 m_updateHandle;
        CLR_INT32 m_success;
    };
};

};


/////////////////////////////////////////////////////////////////////////////
// WP APPLICATION LAYER
/////////////////////////////////////////////////////////////////////////////

const WP_ApplicationLayer c_Update_app =
{
    NULL/*&Samraksh_Emote_Update::App_ProcessHeader*/ ,
    NULL/*&Samraksh_Emote_Update::App_ProcessPayload*/,
    NULL/*&Samraksh_Emote_Update::App_Release*/       ,
};


bool Samraksh_Emote_Update::App_ProcessHeader(void* state, WP_Message* msg )
{
    // for wireless, set m_header.m_size to 0 to indicate we've received a complete payload.
    //short-circuits WP_Message.Process() to ReceiveState::CompletePayload to avoid calling phy->ReceiveBytes.
    msg->m_payload = (UINT8*)g_Samraksh_Emote_Update.ReceiveBuffer + sizeof(WP_Packet);
    msg->m_header.m_size = 0;

    return true;
}

#define CHECK_HANDLE(h) { if((h) == MFUpdate::badHandle) { SOFT_BREAKPOINT(); break; } }

#define CHECK_PTR(p) { if((p) == NULL) { SOFT_BREAKPOINT(); break; } }

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


//////////////////////////////////////////////////////////////////////////////
// WP MESSAGING
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Reply to command, multiplexing between wired and wireless interfaces based on message origin.
 * @detailed modified ReplyToCommand from Messaging.cpp...
 * With wireless, do not use Messaging.  Inserting this function allows
 * wireless to be call-compatible whether a packet comes from wireless
 * or wired interface via debugger. (ie, Messaging.cpp's ReplyToCommand
 * does not directly use any messaging object state.
 * Use this in case we want to use
 * [ 802.15.4 | MAC | WP_Packet::Header | WP_Message::Payload ] stack.
 */
void /*CLR_Messaging::*/ReplyToCommand( WP_Message* msg, bool fSuccess, bool fCritical, void* ptr, int size )
{
    NATIVE_PROFILE_CLR_MESSAGING();
    WP_Message msgReply;
    UINT32     flags = 0;

    //
    // Make sure we reply only once!
    //
    //if(msg->m_header.m_flags & WP_Flags::c_NonCritical) return;  in this ReplyToCommand, we already set c_NonCritical so CLR_Messaging would skip it.  Another way to do this would be to create a new header flag to indicate we want to multiplex.
    msg->m_header.m_flags |= WP_Flags::c_NonCritical;

    //
    // No caching in the request, no caching in the reply...
    //
    if(msg->m_header.m_flags & WP_Flags::c_NoCaching) flags |= WP_Flags::c_NoCaching;

    if(fSuccess  ) flags |= WP_Flags::c_ACK;
    else           flags |= WP_Flags::c_NACK;
    if(!fCritical) flags |= WP_Flags::c_NonCritical;

    if(fSuccess == false)
    {
        ptr  = NULL;
        size = 0;
    }


    msgReply.Initialize( /*&m_controller*/ msg->m_parent );
#if defined(BIG_ENDIAN)
    SwapEndian( msg, ptr, size, true );
#endif

    msgReply.PrepareReply( msg->m_header, flags, size, (UINT8*)ptr );  //creates CRC

    /*m_controller.*/msg->m_parent->SendProtocolMessage( msgReply );  //new: use WP_Controller of message origin.
}


/**
 * helps translate and forward a reply packet from wireless interface to usb.
 * perform steps similar to ReplyToCommand(...) but we already have flags so skip first part of ReplyToCommand(...)
 *
 */
void ForwardReplyToCommand( WP_Message* msg, UINT32 flags, void* ptr, int size)
{
    WP_Message msgReply;
    msgReply.Initialize( msg->m_parent );
#if defined(BIG_ENDIAN)
    SwapEndian( msg, ptr, size, true );
#endif

    msgReply.PrepareReply( msg->m_header, flags, size, (UINT8*)ptr );
    msg->m_parent->SendProtocolMessage( msgReply );
}


/////////////////////////////////////////////////////////////////////////////
// CALLBACKS
/////////////////////////////////////////////////////////////////////////////

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
 * @brief parse incoming packet payloads.
 * some support is implemented for constructing large WP_Packets that were split in the PHY.
 * MFUpdateHeader.PacketSize may be larger than MacConfig.BufferSize, so multiplex the packets here.
 * @param *buffer
 * @param size    length of buffer in bytes.
 * FIXME: ASSERT(wireless message size < size(WP_Packet)), or verify payload when .s_fUseWpPacket == false...
 */
void Samraksh_Emote_Update::Receive(void *buffer, UINT16 sz_buf) {
    UINT32 command = 0;
    WP_Packet* msg = (WP_Packet*)buffer;
    // hal_printf("Receive");
    // always check if sender is using WP_Packet format.  WP_Packet header will be in every message.
    // TODO: dynamically turn on/off WP_Packet format?
    if(g_Samraksh_Emote_Update.s_fUseWpPacket == true) {  //check that it's okay length to compare the WP header.
        ASSERT(sz_buf >= sizeof(WP_Packet));
        size_t lenCmp = __min( sz_buf, sizeof(msg->m_signature) );
        if(memcmp( (UINT8*)msg, MARKER_PACKET_V1, sizeof(msg->m_signature) ) == 0) {
            command = msg->m_cmd;
            memcpy(g_Samraksh_Emote_Update.ReceiveBuffer, msg, sz_buf );
            //g_Samraksh_Emote_Update.m_controller.m_inboundMessage.m_rxState = WP_Message::ReceiveState::CompleteHeader; //skip ReceiveState::WaitingForHeader to avoid call to m_phy->ReceiveBytes.
            g_Samraksh_Emote_Update.m_controller.m_inboundMessage.PrepareReception();
            g_Samraksh_Emote_Update.m_ReceiveState = 2;
            g_Samraksh_Emote_Update.m_controller.AdvanceState();
            if(!g_Samraksh_Emote_Update.m_controller.AdvanceState()) {

                //starts in ReceiveState::CompleteHeader.
                //calls verify on header.
                //calls m_app->ProcessHeader (implemented here).
                //skips to ReceiveState::CompletePayload.
                //calls verify on payload.
                //calls m_app->ProcessPayload (implemented here).
                return; //something went wrong. msg matches MSDBGv1\0 but CRC failed.
            }
        }
    }
    else {
        // handle short format. create WP_Message from parts of the payload.  Note the payload size was set upon initialization.
        // TODO: create a simple struct to typecast msg and reference instead of using offsets.
        WP_Message* p_inboundMessage = &g_Samraksh_Emote_Update.m_controller.m_inboundMessage;

        p_inboundMessage->m_header.m_cmd = ((simple_payload_t*)buffer)->cmd;
        UINT16 sz_pad = sizeof ((simple_payload_t *)0)->cmd + sizeof ((simple_payload_t *)0)->flags;
        if(sz_buf >= sz_pad) {
            p_inboundMessage->m_header.m_size = sz_buf - sz_pad;
        }
        else {
            p_inboundMessage->m_header.m_size = 0;
        }
        p_inboundMessage->m_header.m_flags = ((simple_payload_t*)buffer)->flags;
        p_inboundMessage->m_parent = &g_Samraksh_Emote_Update.m_controller;
        p_inboundMessage->m_payload = &(((simple_payload_t*)buffer)->payload);

        // process Command message
        App_ProcessPayload(&g_Samraksh_Emote_Update, p_inboundMessage);
    }
}



/**
 * Implement wireless SendAckFuncPtrType.  MAC API changed mid-development, so this is glue.
 * FIXME: cut out the middle man.
 */
void SendAckHandler (void* msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus){
    return g_Samraksh_Emote_Update.SendAck(msg,size,status);
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


// TODO: delete this.
/*
static struct UpdateInfoArray {
private:
    const UINT32 size = 4;
    UpdateInfo root[size];
    int count = 0;
public:
    static UpdateInfo* Get(UINT32 updateHandle){
        for(int i = 0; i < size; i++) {
            if(root[i].m_updateHandle == updateHandle) {
                return &root[i];
            }
        }
        return NULL;
    }
    static UpdateInfo* Create(UINT32 updateHandle) {
        UpdateInfo* ret = NULL;
        if(count < size) {
            ret = Get(NULL);
            if(ret) {
                ret->Clear();
                ret->m_updateHandle = updateHandle;
            }
        }
        return ret;
    }
    static void Delete(UINT32 updateHandle) {
        UpdateInfo* p = Get(updateHandle);
        if(p) {
            p->m_updateHandle = 0;
        }
    }
};

static UpdateInfoArray s_UpdateInfo;
*/

//////////////////////////////////////////////////////////////////////////////
// INITIALIZERS
//////////////////////////////////////////////////////////////////////////////

/**
 * Singleton constructor.
 * Called by Debugger::CreateInstance
 * TODO: probably should delete this.
 */
void Samraksh_Emote_Update::CreateInstance()
{
    if(g_Samraksh_Emote_Update.m_fInitialized == true) return;
    TINYCLR_CLEAR(g_Samraksh_Emote_Update); // clear memory.
    g_Samraksh_Emote_Update.Initialize();
}


void Samraksh_Emote_Update::DeleteInstance() {
    g_Samraksh_Emote_Update.Cleanup();
    //FIXME: Need to check for other users of radio, turn radio off. Or, make a radio monitoring service.
    return;
}


void Samraksh_Emote_Update::Cleanup() {
    if(!g_Samraksh_Emote_Update.m_fInitialized) return;
    //CPU_Radio_Sleep(g_Samraksh_Emote_Update.PAK_MacConfig.RadioID,/*RadioStateEnum::*/STATE_SLEEP); //This is now handled by the new MAC API?? not really.
    if(g_Samraksh_Emote_Update.s_UpdateCompletion.IsLinked()) {
        // TODO: save tasks for after reboot, ie if we have multiple initializations.
        g_Samraksh_Emote_Update.s_UpdateCompletion.Abort();  //TODO: completion Abort might not be needed
    }
    g_Samraksh_Emote_Update.m_fInitialized = false;
}


/**
 * Singleton initialization.
 * handles initializing everything that is needed to handle receiving an Update command sequence
 * does not turn on the radio.  radio started by command UpdateInit sent over USB transport, or may be hard-coded in Debugger.cpp's CreateInstance().
 */
void Samraksh_Emote_Update::Initialize() {
    if(g_Samraksh_Emote_Update.m_fInitialized == true) return;
    MFUpdate_Initialize(); //safe to call multiple times.
    g_Samraksh_Emote_Update.m_controller.Initialize( /*MARKER_SAMRAKSH_V1*/MARKER_PACKET_V1, &c_Update_phy, &c_Update_app, &g_Samraksh_Emote_Update);  //use SAMRAKSHV1 so we can tell whether to cast the WP_Controller's passed state as Samraksh_Emote_Update to check for WP_Packet header usage before sending over wireless.
    g_Samraksh_Emote_Update.m_outboundMessage.Initialize(&g_Samraksh_Emote_Update.m_controller);
    g_Samraksh_Emote_Update.m_ReceiveState = 0;
    g_Samraksh_Emote_Update.PAK_routingAppID = MFM_UPDATE;

    Samraksh_Emote_Update::s_fRadioOn = false;
    Samraksh_Emote_Update::s_fUseWpPacket = false;
    Samraksh_Emote_Update::s_fBaseStationMode = false;
    Samraksh_Emote_Update::s_destAddr = MAC_BROADCAST_ADDRESS;
    g_Samraksh_Emote_Update.last_error = UPDATE_INSTALL_NO_MSG;

    Samraksh_Emote_Update::s_UpdateCompletion.InitializeForUserMode( UpdateCompletion );

    g_Samraksh_Emote_Update.m_fInitialized = true;
    g_Samraksh_Emote_Update.m_fMACInitialized = false;
    return;
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
    g_Samraksh_Emote_Update.PAK_MacConfig.NeighborLivenessDelay = 300;
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

// TODO: bool Samraksh_Emote_Updage::UnInitializeMac() { }


bool Samraksh_Emote_Update::InitializeDriversAfterInstall() {
    bool ret = true;
    ret |= HAL_Time_Initialize();
    if(g_Samraksh_Emote_Update.m_fMACInitializedByNativeBeforeInstall == true) {
        ret |= InitializeMac();
    }
    return ret;
}


bool Samraksh_Emote_Update::UnInitializeDriversBeforeInstall() {
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


//////////////////////////////////////////////////////////////////////////////
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
    if( update->Header.PacketSize > MAX_ADDPACKET_DATA ) {
        SOFT_BREAKPOINT();//TODO: USE MACRO and ALLOW FOR HALPRINTF INSTEAD.
        return;
    }
    if( update->Header.UpdateSize > ( update->Header.PacketSize * MAX_MISSING_WORDFIELD_SIZE * 32 ) ) {//TODO: fix access to MAX...
        SOFT_BREAKPOINT();//TODO: tell user.
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

//
///**
// * skeleton code... move to a different library.
// * 2013 idea to manage neighbors
// * receiver requests updates.
// */
//void Samraksh_Emote_Update::HandleTimeout() {
//    for(int i=0; i < g_UpdateCount; i++) {
//        MFUpdate* g_UpdatesItr = &g_Updates[i];
//        if(0 != (g_UpdatesItr->Flags & MFUPDATE_FLAGS__INUSE)) {
//            UINT32 maskedState = 0;
//            maskedState = g_UpdatesItr->Flags & STATE_MASK;
//            if(maskedState & STATE_FLAGS__INSTALL) {
//            }
//            else if(maskedState & STATE_FLAGS__VALIDATED) {
//                //TODO: broadcast available update.
//            }
//            else if(maskedState & (STATE_FLAGS__ADDPACKET || STATE_FLAGS__AUTHENTICATED)) {
//                UINT32 firstMissingPacket = GetFirstMissingPacket(&g_Updates[i], g_UpdatesItr->m_missingPkts);
//                if(firstMissingPacket > g_UpdatesItr->m_finalPacketIdx) {
//                    //received last packet, wait silently for install command to validate packet.
//                }
//                else {
//                    g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_cmd = c_Debugging_MFUpdate_AddPacket;
//                ((MFUpdate_Commands::Debugging_MFUpdate_AddPacket::Reply*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_updateHandle = g_UpdatesItr->Header.UpdateID;
//                ((MFUpdate_Commands::Debugging_MFUpdate_AddPacket::Reply*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_packetIndex = firstMissingPacket; //or do check for firstMissingPacket > 0; m_packetIdx = firstMissingPacket -1; m_success = true;
//                ((MFUpdate_Commands::Debugging_MFUpdate_AddPacket::Reply*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_success = 0;
//                ReplyToCommand( &g_Samraksh_Emote_Update.m_outboundMessage, true, false, &g_Samraksh_Emote_Update.m_outboundMessagePayload, sizeof(MFUpdate_Commands::Debugging_MFUpdate_Start::Reply) );
//                }
//            }
//            else if(maskedState & STATE_FLAGS__START) {
//                //received start message and nothing else.  send reply to get next state.
//                g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_cmd = c_Debugging_MFUpdate_Start;
//                ((MFUpdate_Commands::Debugging_MFUpdate_Start::Reply*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_updateHandle = g_UpdatesItr->Header.UpdateID;
//                ReplyToCommand( &g_Samraksh_Emote_Update.m_outboundMessage, true, false, &g_Samraksh_Emote_Update.m_outboundMessagePayload, sizeof(MFUpdate_Commands::Debugging_MFUpdate_Start::Reply) );
//            }
//            else {
//                //nothing else happened;
//                //TODO check disk for partial updates to query for
//                break;
//            }
//
//            maskedState =g_UpdatesItr->Flags & NEIGHBOR_MASK;
//            /*if(maskedState & NEIGHBOR_FLAGS__INSTALL) {  }
//            else if(maskedState & NEIGHBOR_FLAGS__VALIDATED) {  }
//            else*/
//            if( (NEIGHBOR_FLAGS__ADDPACKET == (maskedState & NEIGHBOR_FLAGS__ADDPACKET))
//                    || (NEIGHBOR_FLAGS__AUTHENTICATED == (maskedState & NEIGHBOR_FLAGS__AUTHENTICATED))) {
//                //TODO: take XOR of both bitfields, then AND result with local bitfield, feed to GetFirstMissingPacket.
//                UINT32 firstNeighborMissingPacket = GetFirstMissingPacket(&g_Updates[i], g_UpdatesItr->m_neighborMissingPkts);
//                if(firstNeighborMissingPacket > g_UpdatesItr->m_finalPacketIdx) {
//                    //TODO send install command if we've installed or scheduled?
//                }
//                else {
//                    UINT32 firstMissingPacket = GetFirstMissingPacket(&g_Updates[i], g_UpdatesItr->m_missingPkts);
//                    if(firstNeighborMissingPacket < firstMissingPacket) {
//                        g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_cmd = c_Debugging_MFUpdate_AddPacket;
//                        g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_flags = 0;
//                        g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_size = sizeof(MFUpdate_Commands::Debugging_MFUpdate_AddPacket) + g_UpdatesItr->Header.PacketSize;
//                        ((MFUpdate_Commands::Debugging_MFUpdate_AddPacket*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_updateHandle = g_UpdatesItr->Header.UpdateID;
//                        ((MFUpdate_Commands::Debugging_MFUpdate_AddPacket*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_packetIndex = firstNeighborMissingPacket;
//                        ((MFUpdate_Commands::Debugging_MFUpdate_AddPacket*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_packetLength = (firstNeighborMissingPacket == g_UpdatesItr->m_finalPacketIdx) ? g_UpdatesItr->Header.UpdateSize % g_UpdatesItr->Header.PacketSize : g_UpdatesItr->Header.PacketSize ;
//                        if( FALSE == g_UpdatesItr->Providers->Storage->Read( g_UpdatesItr->StorageHandle, firstNeighborMissingPacket * g_UpdatesItr->Header.PacketSize, ((MFUpdate_Commands::Debugging_MFUpdate_AddPacket*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_packetData, g_UpdatesItr->Header.PacketSize)) {
//                            continue;
//                        }
//                        ((MFUpdate_Commands::Debugging_MFUpdate_AddPacket*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_packetValidation = SUPPORT_ComputeCRC( ((MFUpdate_Commands::Debugging_MFUpdate_AddPacket*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_packetData, ((MFUpdate_Commands::Debugging_MFUpdate_AddPacket*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_packetLength, 0 );
//                        g_Samraksh_Emote_Update.m_outboundMessage.m_parent->SendProtocolMessage( g_Samraksh_Emote_Update.m_outboundMessage );
//                    }
//                }
//            }
//            else if(NEIGHBOR_FLAGS__START == (maskedState & NEIGHBOR_FLAGS__START)) {
//                //TODO: generate auth command.
//                g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_cmd = c_Debugging_MFUpdate_Authenticate;
//                g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_flags = 0;
//                g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_size = sizeof(MFUpdate_Commands::Debugging_MFUpdate_Authenticate);
//                ((MFUpdate_Commands::Debugging_MFUpdate_Authenticate*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_updateHandle = 0;
//                ((MFUpdate_Commands::Debugging_MFUpdate_Authenticate*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_authenticationLen = 0;
//                g_Samraksh_Emote_Update.m_outboundMessage.m_parent->SendProtocolMessage( g_Samraksh_Emote_Update.m_outboundMessage );
//            }
//            else {
//                g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_cmd = c_Debugging_MFUpdate_Start;
//                g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_flags = 0;
//                g_Samraksh_Emote_Update.m_outboundMessage.m_header.m_size = sizeof(MFUpdate_Commands::Debugging_MFUpdate_Start);
//                hal_strncpy_s ( ((MFUpdate_Commands::Debugging_MFUpdate_Start*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_provider, 63, g_UpdatesItr->Providers->ProviderName, hal_strlen_s(g_UpdatesItr->Providers->ProviderName) ) ;
//                ((MFUpdate_Commands::Debugging_MFUpdate_Start*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_updateId = g_UpdatesItr->Header.UpdateID;
//                ((MFUpdate_Commands::Debugging_MFUpdate_Start*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_updateType = g_UpdatesItr->Header.UpdateType;
//                ((MFUpdate_Commands::Debugging_MFUpdate_Start*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_updateSubType = g_UpdatesItr->Header.UpdateSubType;
//                ((MFUpdate_Commands::Debugging_MFUpdate_Start*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_updateSize = g_UpdatesItr->Header.UpdateSize;
//                ((MFUpdate_Commands::Debugging_MFUpdate_Start*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_updatePacketSize = g_UpdatesItr->Header.PacketSize;
//                ((MFUpdate_Commands::Debugging_MFUpdate_Start*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_versionMajor = g_UpdatesItr->Header.Version.usMajor;
//                ((MFUpdate_Commands::Debugging_MFUpdate_Start*) g_Samraksh_Emote_Update.m_outboundMessage.m_payload)->m_versionMinor = g_UpdatesItr->Header.Version.usMinor;
//                g_Samraksh_Emote_Update.m_outboundMessage.m_parent->SendProtocolMessage( g_Samraksh_Emote_Update.m_outboundMessage );
//            }
//        }
//
//    }
//}


/////////////////////////////////////////////////////////////////////////////
// Functions previously implemented in Debugger.cpp
// Debugging_MFUpdate_[Start, AuthCommand, Authenticate, GetMissingPkts, AddPackets, Install]
// Support Debugger's MFUpdate Wire Protocol commands.
// Copied here to remove WP_Message and Debugger dependencies, so we can use the same code for any transport interface.
/////////////////////////////////////////////////////////////////////////////


bool Samraksh_Emote_Update::UpdateInit( WP_Message* msg, void* owner )
{
	//TODO: accept argument specifying different modes... like burst mode transfer, USB-tethered repeater
    CreateInstance();
    InitializeMac(); // turn on wireless, ie, if message received over USB.
    //TODO: re-initialize the MAC into burst mode.
    return true;
}


bool Samraksh_Emote_Update::UpdateDeInit( WP_Message* msg, void* owner )
{
    DeleteInstance();
    // TODO: UnInitializeMac();
    if(Samraksh_Emote_Update::s_UpdateCompletion.IsLinked() == TRUE) {
    	Samraksh_Emote_Update::s_UpdateCompletion.Abort();
    	private_free( Samraksh_Emote_Update::s_UpdateCompletionArg );
    }
    return true;
}


bool Samraksh_Emote_Update::Start( WP_Message* msg, void* owner )
{
    NATIVE_PROFILE_CLR_DEBUGGER();
    MULTIPLEX_PACKETS();

    MFUpdate_Commands::Debugging_MFUpdate_Start*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_Start*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_Start::Reply reply, *pReply;
    CLR_INT32 replySize = sizeof(reply);
    MFUpdateHeader header;

    pReply = &reply;

    TINYCLR_CLEAR(header);

    header.Version.usMajor = cmd->m_versionMajor;
    header.Version.usMinor = cmd->m_versionMinor;
    header.UpdateID        = cmd->m_updateId;
    header.UpdateType      = cmd->m_updateType;
    header.UpdateSubType   = cmd->m_updateSubType;
    header.UpdateSize      = cmd->m_updateSize;
    header.PacketSize      = cmd->m_updatePacketSize;

    // TODO: check for available contiguous storage region
    // TODO: if no storage, and version is greater, and erasing would give enough storage, then erase existing updates
    reply.m_updateHandle = MFUpdate_InitUpdate(cmd->m_provider, header);

    if( reply.m_updateHandle == MFUpdate::badHandle || reply.m_updateHandle != cmd->m_updateId)
    {
        reply.m_updateHandle = cmd->m_updateId;
        reply.m_success = false;
    }
    else
    {
        reply.m_success = true;
    }

    /*dbg->m_messaging->*/ReplyToCommand( msg, true, false, pReply, replySize );
    //TODO reset timer.  Don't send StartAck unless it was a unicast message (not doing that yet...) or timer expires (ie, wireless sender didn't send us the next state.)

    return true;
}

bool Samraksh_Emote_Update::AuthCommand( WP_Message* msg, void* owner )
{

    NATIVE_PROFILE_CLR_DEBUGGER();
    MULTIPLEX_PACKETS();

    MFUpdate_Commands::Debugging_MFUpdate_AuthCommand*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_AuthCommand*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_AuthCommand::Reply reply, *pReply;

    INT32 respLen = 0;
    INT32 replySize = sizeof(reply);

    TINYCLR_CLEAR(reply);

    reply.m_updateHandle = cmd->m_updateHandle;

    pReply = &reply;

    if(MFUpdate_AuthCommand(cmd->m_updateHandle, cmd->m_authCommand, cmd->m_authArgs, cmd->m_authArgsSize, NULL, respLen))
    {
        if(respLen > 0)
        {
            int cmdSize = respLen + offsetof(MFUpdate_Commands::Debugging_MFUpdate_AuthCommand::Reply, m_response);

            MFUpdate_Commands::Debugging_MFUpdate_AuthCommand::Reply* pTmp = (MFUpdate_Commands::Debugging_MFUpdate_AuthCommand::Reply*)private_malloc(cmdSize);

            if(pTmp != NULL)
            {
                if(MFUpdate_AuthCommand(cmd->m_updateHandle, cmd->m_authCommand, cmd->m_authArgs, cmd->m_authArgsSize, pTmp->m_response, respLen))
                {
                    pReply                 = pTmp;
                    replySize              = cmdSize;
                    pReply->m_responseSize = respLen;
                    pReply->m_success      = true;
                }
                else
                {
                    private_free(pTmp);
                }
            }
        }
    }

    /*dbg->m_messaging->*/ReplyToCommand( msg, true, false, pReply, replySize );

    if(pReply != &reply)
    {
        private_free(pReply);
    }

    return true;
}

bool Samraksh_Emote_Update::Authenticate( WP_Message* msg, void* owner )
{
    NATIVE_PROFILE_CLR_DEBUGGER();
    MULTIPLEX_PACKETS();

    MFUpdate_Commands::Debugging_MFUpdate_Authenticate*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_Authenticate*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_Authenticate::Reply reply;

    reply.m_updateHandle = cmd->m_updateHandle;
    CLR_INT32 authType = 0;
    CLR_INT32 respLen = sizeof(authType);

    //memset(&s_missingPkts, 0xFF, sizeof(s_missingPkts));

    reply.m_success = 0;

    MFUpdate_AuthCommand(cmd->m_updateHandle, MFUPDATE_VALIDATION_COMMAND__GET_AUTH_TYPE, NULL, 0, (UINT8*)&authType, respLen);

    if(authType == MFUPDATE_AUTHENTICATION_TYPE__SSL)
    {
        reply.m_success = 1;

        // reply early for SSL so that the device will try to upgrad the stream at the same time.
        /*dbg->m_messaging->*/ReplyToCommand( msg, true, false, &reply, sizeof(reply) );

        Events_WaitForEvents(0, 400);
    }

    if(MFUpdate_Authenticate(cmd->m_updateHandle, cmd->m_authenticationData, cmd->m_authenticationLen))
    {
        reply.m_success = MFUpdate_Open(cmd->m_updateHandle);

        if(!reply.m_success)
        {
            reply.m_success = MFUpdate_Create(cmd->m_updateHandle);
        }
    }

    /*dbg->m_messaging->*/ReplyToCommand( msg, true, false, &reply, sizeof(reply) );

    return true;
}


bool Samraksh_Emote_Update::GetMissingPkts( WP_Message* msg, void* owner )
{
    NATIVE_PROFILE_CLR_DEBUGGER();
    MULTIPLEX_PACKETS();

    MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts::Reply reply, *pReply;


    MFUpdate* updateInfo = 0x0;
    CLR_INT32 int32Cnt = 0;
    if(cmd->m_updateHandle != MFUpdate::badHandle) {
        updateInfo = g_Updates->GetUpdate(cmd->m_updateHandle);
        int32Cnt = updateInfo->m_missingPktsWordfieldSize;//ARRAYSIZE(s_missingPkts);  //number of uints needed to represent missing packets.
    }

    CLR_INT32 replySize = sizeof(reply);
    CLR_INT32 sizeBytes = (int32Cnt << 2) + offsetof(MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts::Reply, m_missingPkts);

    TINYCLR_CLEAR(reply);
    pReply = &reply;

    if(cmd->m_updateHandle != MFUpdate::badHandle) {
        if(MFUpdate_GetMissingPackets(cmd->m_updateHandle, updateInfo->m_missingPkts, &int32Cnt))
        {
            pReply = (MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts::Reply*)private_malloc(sizeBytes);

            if(pReply != NULL)
            {
                pReply->m_missingPktCount = int32Cnt;
                pReply->m_success = 1;

                memcpy(pReply->m_missingPkts, updateInfo->m_missingPkts, updateInfo->m_missingPktsWordfieldSize << 2);

                replySize = sizeBytes;
            }
            else
            {
                pReply = &reply;
            }
        }
        pReply->m_updateHandle = cmd->m_updateHandle;
    }

    /*dbg->m_messaging->*/ReplyToCommand( msg, true, false, pReply, replySize );

    if(pReply != &reply)
    {
        private_free(pReply);
    }

    return true;
}


bool Samraksh_Emote_Update::AddPacket(WP_Message* msg, void* owner )
{
    NATIVE_PROFILE_CLR_DEBUGGER();
    MULTIPLEX_PACKETS();

    MFUpdate_Commands::Debugging_MFUpdate_AddPacket*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_AddPacket*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_AddPacket::Reply reply;
    MFUpdate* updateInfo = g_Updates->GetUpdate(cmd->m_updateHandle);

    if(updateInfo != NULL)
    {
        if(cmd->m_packetIndex >> 5 < updateInfo->m_missingPktsWordfieldSize)
        {
            if(!have_packet(cmd->m_packetIndex, updateInfo->m_missingPkts))
            {
                reply.m_success = MFUpdate_AddPacket(cmd->m_updateHandle, cmd->m_packetIndex, &cmd->m_packetData[0], cmd->m_packetLength, (CLR_UINT8*)&cmd->m_packetValidation, sizeof(cmd->m_packetValidation));

                if(reply.m_success)
                {
                    set_packet_as_received(cmd->m_packetIndex, updateInfo->m_missingPkts);
                }
            }
            else
            {
                reply.m_success = TRUE;
            }
        }
        else
        {
            reply.m_success = MFUpdate_AddPacket(cmd->m_updateHandle, cmd->m_packetIndex, &cmd->m_packetData[0], cmd->m_packetLength, (CLR_UINT8*)&cmd->m_packetValidation, sizeof(cmd->m_packetValidation));
        }
    }
    else
    {
        reply.m_success = 0;
    }

    //TODO: figure out why the code originally returned false here. could be related to bug where packet index 0 always fails.
    reply.m_updateHandle = cmd->m_updateHandle;
    reply.m_packetIndex = cmd->m_packetIndex;
    reply.m_nextMissingPacketIndex = 0;
    if(updateInfo != NULL)
    {
        reply.m_nextMissingPacketIndex = GetFirstMissingPacket(updateInfo);
    }

    /*dbg->m_messaging->*/ReplyToCommand( msg, true, false, &reply, sizeof(reply) );

    return (reply.m_success == TRUE);
}

/**
 * remember update info between install command and completion
 */
struct SUpdateInstall {
	//TODO: WP_Message* msg to transmit install status after installation.
	CLR_UINT32 m_updateHandle;
	CLR_UINT32 m_updateValidationSize;
	CLR_UINT8  m_updateValidation[1];  //TODO: make this dynamically allocated?
};

/**
 * replaces any pending completions
 */
bool Samraksh_Emote_Update::Install(WP_Message* msg, void* owner )
{
    NATIVE_PROFILE_CLR_DEBUGGER();
    MULTIPLEX_PACKETS();

    MFUpdate_Commands::Debugging_MFUpdate_Install*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_Install*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_Install::Reply reply;
    reply.m_updateHandle = cmd->m_updateHandle;
    reply.m_success = false;

    struct SUpdateInstall* update_completion_arg;

    bool passed_validation = false;

    if (Samraksh_Emote_Update::s_UpdateCompletion.IsLinked() == TRUE) {
    	// UPDATE_INSTALL_MULTIPLE_QUEUED -- already have an update installation pending!
        // ASSERT_UPDATE_PROTOCOL(0); //detect duplicate calls. do not use if callback is pre-initialized.
    	Samraksh_Emote_Update::s_UpdateCompletion.Abort();
    	private_free(Samraksh_Emote_Update::s_UpdateCompletionArg);
    }

    if (cmd->m_updateValidationSize > MAX_UPDATE_VALIDATION_SIZE_IN_BYTES) {
        g_Samraksh_Emote_Update.last_error = UPDATE_INSTALL_FAIL_VALIDATION;
        ASSERT_UPDATE_PROTOCOL(0);
        //TODO: record reason in reply message!
        goto Install_out;
    }

	if ( ( passed_validation = MFUpdate_Validate(cmd->m_updateHandle, &cmd->m_updateValidation[0], cmd->m_updateValidationSize) ) != TRUE) {
		g_Samraksh_Emote_Update.last_error = UPDATE_INSTALL_FAIL_VALIDATION;
		ASSERT_UPDATE_PROTOCOL(passed_validation);
		//TODO: record reason in reply message!
		goto Install_out;
	}

    update_completion_arg = (struct SUpdateInstall*) private_malloc(sizeof(struct SUpdateInstall) + cmd->m_updateValidationSize);
    if(update_completion_arg == NULL) {
		//TODO: record OOM error
		passed_validation = false;
		goto Install_out;
    }

    //FIXME: should add install time/date, use continuation, so multiple assemblies may be scheduled for installation.
    // but the schedule would need to be saved in non-volatile memory (maybe inside Update metadata in FLASH)
    // and we need persistent datetime keeping, and global datetime synchronization first...
    update_completion_arg->m_updateHandle = cmd->m_updateHandle;
    update_completion_arg->m_updateValidationSize = cmd->m_updateValidationSize;
    memcpy(&update_completion_arg->m_updateValidation[0], &cmd->m_updateValidation[0], cmd->m_updateValidationSize);
    Samraksh_Emote_Update::s_UpdateCompletionArg = update_completion_arg;


	Samraksh_Emote_Update::s_UpdateCompletion.InitializeForUserMode(Samraksh_Emote_Update::UpdateCompletion, (void*)update_completion_arg);
	Samraksh_Emote_Update::s_UpdateCompletion.EnqueueDelta64( 5 * 1000000 ); // schedule install in 5 seconds
	//TODO: record "update queued" in reply message!

Install_out:

    reply.m_success = passed_validation;
    /*dbg->m_messaging->*/ReplyToCommand( msg, true, false, &reply, sizeof(reply) );

    return TRUE;
}


/**
 * requires update has already passed validation.
 */
void Samraksh_Emote_Update::UpdateCompletion(void *arg)
{
    NATIVE_PROFILE_CLR_DEBUGGER();

    MFUpdate_Commands::Debugging_MFUpdate_Install::Reply reply;

	bool drivers_paused = false;
    bool successfully_installed = false;

    struct SUpdateInstall* install_args = (struct SUpdateInstall*)arg;

    drivers_paused = UnInitializeDriversBeforeInstall();
    ASSERT_SP(drivers_paused);

	//Events_WaitForEvents(0, 200);  // allow time for sending reply message?
	if (drivers_paused) {
		successfully_installed = MFUpdate_Install(install_args->m_updateHandle, &install_args->m_updateValidation[0], install_args->m_updateValidationSize);
	}
	else {
		//TODO: consider forcing hard CPU reboot if !drivers_paused, with installation trigger in CONFIG.
	}

	if (!successfully_installed) {
		g_Samraksh_Emote_Update.last_error = UPDATE_INSTALL_FAIL_INSTALL;
        ASSERT_UPDATE_PROTOCOL(successfully_installed);
	}

	private_free(arg);

    bool drivers_initialized = InitializeDriversAfterInstall();
    //FIXME: send install success message.
    reply.m_success = successfully_installed;
    ///*dbg->m_messaging->*/ReplyToCommand( msg, true, false, &reply, sizeof(reply) );

    DEBUG_PRINTF_EMOTE_UPDATE("update install complete.\r\n");

    //Force reboot
	bool port_flushed = DebuggerPort_Flush( HalSystemConfig.DebugTextPort );
    Events_WaitForEvents( 0, 100 );

    CPU_Reset();
}

