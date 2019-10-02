/*--------- Includes ----------- */

#include "PAK_Bluetooth_Interface.h"
#include <cmsis/m2sxxx.h>
#include "../eNVM/eNVM.h"
#include <TinyHal.h>
#include <MFUpdate_decl.h>
#include <WireProtocol.h>
#include <TinyCLR_Types.h>   //CLR_RECORD_ASSEMBLY
#include <Samraksh\BluetoothMac_Functions.h>
#include <TinyCLR_Endian.h>


// this must match drivers/bluetooth/sf2/btmain.h and drivers\bluetooth\c_code_calling_cpp.h
#define ENCRYPTED_DATA_CHANNEL 0x09
#define UNENCRYPTED_DATA_CHANNEL 0x0C
#define CLOUD_CHANNEL 0x0F

#define SIGNATURE_SIZE 7

uint32_t GetFirstMissingPacket(MFUpdate* updateInfo);  //<! use when estimating this node's missing packets
uint32_t GetFirstMissingPacket(MFUpdate* updateInfo, uint32_t* missingPkts ); //<! use when estimating neighbor's missing packets

Samraksh_Emote_Update g_Samraksh_Emote_Update;
static uint32_t transferSize;
static uint32_t totalPacketNum;
static uint32_t writeLocation;
//MFUpdateHeader g_updateHeader;
//MFUpdate g_updateInfo;

//////////////////////////////
// WP APPLICATION LAYER
/////////////////////////////////////////////////////////////////////////////

//const WP_ApplicationLayer c_Update_app =
//{
   // NULL/*&Samraksh_Emote_Update::App_ProcessHeader*/ ,
   // NULL/*&Samraksh_Emote_Update::App_ProcessPayload*/,
   // NULL/*&Samraksh_Emote_Update::App_Release*/       ,
//};


/*bool Samraksh_Emote_Update::App_ProcessHeader(void* state, WP_Message* msg )
{
    // for wireless, set m_header.m_size to 0 to indicate we've received a complete payload.
    //short-circuits WP_Message.Process() to ReceiveState::CompletePayload to avoid calling phy->ReceiveBytes.
    msg->m_payload = (uint8_t*)g_Samraksh_Emote_Update.ReceiveBuffer + sizeof(WP_Packet);
    msg->m_header.m_size = 0;

    return true;
}*/

/*void Samraksh_Emote_Update::SwapEndian(WP_Message* msg)
{
    msg->m_header.m_crcHeader = ::SwapEndian( msg->m_header.m_crcHeader     );
    msg->m_header.m_crcData   = ::SwapEndian( msg->m_header.m_crcData       );

    msg->m_header.m_cmd       = ::SwapEndian( msg->m_header.m_cmd           );
    msg->m_header.m_seq       = ::SwapEndian( msg->m_header.m_seq           );
    msg->m_header.m_seqReply  = ::SwapEndian( msg->m_header.m_seqReply      );
    msg->m_header.m_flags     = ::SwapEndian( msg->m_header.m_flags         );
    msg->m_header.m_size      = ::SwapEndian( msg->m_header.m_size          );
}*/



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
//void ReplyToCommand( WP_Message* msg, bool fSuccess, bool fCritical, void* ptr, int size )
void ReplyToCommand( uint32_t cmd, bool fSuccess )
{
	int responseDataSize = 12;
	uint8_t responseData[responseDataSize];
	memcpy((void*) responseData, MARKER_PACKET_V1, SIGNATURE_SIZE);
	responseData[7] = (cmd & 0xFF000000) >> 24;
	responseData[8] = (cmd & 0xFF0000) >> 16;
	responseData[9] = (cmd & 0xFF00) >> 8;
	responseData[10] = (cmd & 0xFF);
	responseData[11] = (uint8_t) fSuccess;
	MAC_Send(CLOUD_CHANNEL, NULL, &responseData[0], responseDataSize);
    /*NATIVE_PROFILE_CLR_MESSAGING();
    WP_Message msgReply;
    uint32_t     flags = 0;

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
    }*/


    //msgReply.Initialize( &g_Samraksh_Emote_Update.m_controller );
//#if defined(BIG_ENDIAN)
    //g_Samraksh_Emote_Update.SwapEndian(msg);
//#endif

	
    //msgReply.PrepareReply( msg->m_header, flags, size, (uint8_t*)ptr );  //creates CRC

    //g_Samraksh_Emote_Update.m_controller.SendProtocolMessage( msgReply );  //new: use WP_Controller of message origin.
}


/**
 * helps translate and forward a reply packet from wireless interface to usb.
 * perform steps similar to ReplyToCommand(...) but we already have flags so skip first part of ReplyToCommand(...)
 *
 */
/*void ForwardReplyToCommand( WP_Message* msg, uint32_t flags, void* ptr, int size)
{
    WP_Message msgReply;
    msgReply.Initialize( msg->m_parent );
//#if defined(BIG_ENDIAN)
    //SwapEndian( msg, ptr, size, true );
	g_Samraksh_Emote_Update.SwapEndian(msg);
//#endif	

    msgReply.PrepareReply( msg->m_header, flags, size, (uint8_t*)ptr );
    msg->m_parent->SendProtocolMessage( msgReply );
}*/

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

/*const WP_PhysicalLayer c_Update_phy =
{
    &Samraksh_Emote_Update::ReadReceiveBytes,
    &Samraksh_Emote_Update::TransmitMessage,
};*/

/*bool Samraksh_Emote_Update::ReadReceiveBytes( void* state, unsigned char*& ptr, unsigned int & size ) {

	hal_printf("--- read rx bytes\r\n");
	return true;
}*/

/*bool Samraksh_Emote_Update::TransmitMessage( void* state, const WP_Message* msg ) {
	hal_printf("--- tx bytes\r\n");
	int headerSize = sizeof(WP_Packet);
	int responseDataSize = headerSize + msg->m_size;
	uint8_t responseData[responseDataSize];

	memcpy(responseData, &msg->m_header, headerSize);
	memcpy(&responseData[headerSize], msg->m_payload, responseDataSize - headerSize);
	DeviceStatus retVal = MAC_Send(CLOUD_CHANNEL, NULL, &responseData[0], responseDataSize);
	if (retVal == DS_Success)
		return true;
	else
		return false;
}*/

/////////////////////////////////////////////////////////////////////////////
// CALLBACKS
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief parse incoming packet payloads.
 * some support is implemented for constructing large WP_Packets that were split in the PHY.
 * MFUpdateHeader.PacketSize may be larger than MacConfig.BufferSize, so multiplex the packets here.
 * @param *buffer
 * @param size    length of buffer in bytes.
 * FIXME: ASSERT(wireless message size < size(WP_Packet)), or verify payload when .s_fUseWpPacket == false...
 */
void Samraksh_Emote_Update::Receive(uint8_t* payload, int size) {
	uint32_t opCode = payload[7]<<24 | payload[8]<<16 | payload[9]<<8 | payload[10];
    //uint32_t command = 0;
    //WP_Packet* msg = (WP_Packet*)buffer;

	 if (opCode == g_Samraksh_Emote_Update.c_Debugging_MFUpdate_Start){
				transferSize = payload[11]<<24 | payload[12]<<16 | payload[13]<<8 | payload[14];
				totalPacketNum = payload[15]<<24 | payload[16]<<16 | payload[17]<<8 | payload[18];
				hal_printf("Update start transferSize: %d sent over %d packets\r\n", transferSize, totalPacketNum);

				ReplyToCommand(g_Samraksh_Emote_Update.c_Debugging_MFUpdate_Start, TRUE);
				
			} else if (opCode == g_Samraksh_Emote_Update.c_Debugging_MFUpdate_AddPacket){
				int currentPacketNum = payload[11];
				size = size - 15; // packet size minus header minus 16 bytes of marker/param
				hal_printf("Update add packet (%d) size: %d\r\n", currentPacketNum, size);
				
				ReplyToCommand(g_Samraksh_Emote_Update.c_Debugging_MFUpdate_AddPacket, true);
				//PrintHex(&payload[15],size); 
				//hal_printf("\r\n");

				/*nvm_status_t status = NVM_unlock(writeLocation, size);
    			if((NVM_SUCCESS == status)||(NVM_WRITE_THRESHOLD_WARNING == status))
    			{
				status = NVM_write(writeLocation, &payload[15], size, NVM_DO_NOT_LOCK_PAGE);
        		if((NVM_SUCCESS == status)||(NVM_WRITE_THRESHOLD_WARNING == status))
        		{
					hal_printf("binary write success\r\n");
        		} else {
					hal_printf("binary envm write error: %d\r\n", status);
				}
        
    			} else {
					hal_printf("binary envm unlock error: %d\r\n", status);
				}

				writeLocation += size;
				hal_printf("Data to write (%d) @ %x: \r\n", size, writeLocation);

				memcpy((void*) responseData, MARKER_PACKET_V1, SIGNATURE_SIZE);
				responseData[7] = 0x00;
				responseData[8] = 0x02;
				responseData[9] = 0x00;
				responseData[10] = 0x57;
				MAC_Send(CLOUD_CHANNEL, NULL, &responseData[0], responseDataSize);*/
			} else if (opCode == g_Samraksh_Emote_Update.c_Debugging_MFUpdate_Install){
				hal_printf("MFUpdate install\r\n");
				ReplyToCommand(g_Samraksh_Emote_Update.c_Debugging_MFUpdate_Install, true);
				/*memcpy((void*) responseData, MARKER_PACKET_V1, SIGNATURE_SIZE);
				responseData[7] = 0x00;
				responseData[8] = 0x02;
				responseData[9] = 0x00;
				responseData[10] = 0x58;
				MAC_Send(CLOUD_CHANNEL, NULL, &responseData[0], responseDataSize);*/
			} else if (opCode == g_Samraksh_Emote_Update.c_Debugging_MFUpdate_GetMissingPkts){
				hal_printf("MFUpdate missing pkts\r\n");
				ReplyToCommand(g_Samraksh_Emote_Update.c_Debugging_MFUpdate_GetMissingPkts, true);
				/*memcpy((void*) responseData, MARKER_PACKET_V1, SIGNATURE_SIZE);
				responseData[7] = 0x00;
				responseData[8] = 0x02;
				responseData[9] = 0x00;
				responseData[10] = 0x61;
				MAC_Send(CLOUD_CHANNEL, NULL, &responseData[0], responseDataSize);*/
			}

    // always check if sender is using WP_Packet format.  WP_Packet header will be in every message.
    // TODO: dynamically turn on/off WP_Packet format?
    /*if(g_Samraksh_Emote_Update.s_fUseWpPacket == true) {  //check that it's okay length to compare the WP header.
        ASSERT(sz_buf >= sizeof(WP_Packet));
        size_t lenCmp = __min( sz_buf, sizeof(msg->m_signature) );
        if(memcmp( (uint8_t*)msg, MARKER_PACKET_V1, sizeof(msg->m_signature) ) == 0) {
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
        uint16_t sz_pad = sizeof ((simple_payload_t *)0)->cmd + sizeof ((simple_payload_t *)0)->flags;
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
    }*/
	//App_ProcessPayload(&g_Samraksh_Emote_Update, p_inboundMessage);
}

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
    //TINYCLR_CLEAR(g_Samraksh_Emote_Update); // clear memory.
	memset(&g_Samraksh_Emote_Update, 0, sizeof(Samraksh_Emote_Update));
    g_Samraksh_Emote_Update.Initialize();
}


void Samraksh_Emote_Update::DeleteInstance() {
    g_Samraksh_Emote_Update.Cleanup();
    //FIXME: Need to check for other users of radio, turn radio off. Or, make a radio monitoring service.
    return;
}


void Samraksh_Emote_Update::Cleanup() {
    if(!g_Samraksh_Emote_Update.m_fInitialized) return;
    /*if(g_Samraksh_Emote_Update.s_UpdateCompletion.IsLinked()) {
        // TODO: save tasks for after reboot, ie if we have multiple initializations.
        g_Samraksh_Emote_Update.s_UpdateCompletion.Abort();  //TODO: completion Abort might not be needed
    }*/
    g_Samraksh_Emote_Update.m_fInitialized = false;
}


/**
 * Singleton initialization.
 * handles initializing everything that is needed to handle receiving an Update command sequence
 */
void Samraksh_Emote_Update::Initialize() {
    bool ret = true;
	hal_printf(" ----- update init 2\r\n");
	
	if(g_Samraksh_Emote_Update.m_fInitialized == true) return;
    //MFUpdate_Initialize(); //safe to call multiple times.

	//g_Samraksh_Emote_Update.m_controller.Initialize( MARKER_PACKET_V1, &c_Update_phy, &c_Update_app, &g_Samraksh_Emote_Update);  //use SAMRAKSHV1 so we can tell whether to cast the WP_Controller's passed state as Samraksh_Emote_Update to check for WP_Packet header usage before sending over wireless.
    //g_Samraksh_Emote_Update.m_outboundMessage.Initialize(NULL);
    g_Samraksh_Emote_Update.m_ReceiveState = 0;
    
	//Samraksh_Emote_Update::s_UpdateCompletion.InitializeForUserMode( UpdateCompletion );

    g_Samraksh_Emote_Update.m_fInitialized = true;

    //ret = InitializeInterface();
    return;
}

bool Samraksh_Emote_Update::InitializeDriversAfterInstall() {
    bool ret = true;
    ret = InitializeDriversAfterInstall();
    return ret;
}

bool Samraksh_Emote_Update::UnInitializeDriversBeforeInstall() {
    bool ret = true;
    ret = UnInitializeDriversBeforeInstall();
    return ret;
}

/////////////////////////////////////////////////////////////////////////////
// Functions previously implemented in Debugger.cpp
// Debugging_MFUpdate_[Start, AuthCommand, Authenticate, GetMissingPkts, AddPackets, Install]
// Support Debugger's MFUpdate Wire Protocol commands.
// Copied here to remove WP_Message and Debugger dependencies, so we can use the same code for any transport interface.
/////////////////////////////////////////////////////////////////////////////


bool Samraksh_Emote_Update::UpdateInit( WP_Message* msg, void* owner )
{
	hal_printf("---- update init\r\n");
	
	//TODO: accept argument specifying different modes... like burst mode transfer, USB-tethered repeater
    CreateInstance();
    //InitializeMac(); // turn on wireless, ie, if message received over USB.
    //TODO: re-initialize the MAC into burst mode.
    return true;
}


bool Samraksh_Emote_Update::UpdateDeInit( WP_Message* msg, void* owner )
{
    DeleteInstance();
    // TODO: UnInitializeMac();
    /*if(Samraksh_Emote_Update::s_UpdateCompletion.IsLinked() == TRUE) {
    	Samraksh_Emote_Update::s_UpdateCompletion.Abort();
    	private_free( Samraksh_Emote_Update::s_UpdateCompletionArg );
    }*/
    return true;
}


bool Samraksh_Emote_Update::Start( int binarySize, int totalPacketNum)
{
	hal_printf("---- start\r\n");
    /*MFUpdate_Commands::Debugging_MFUpdate_Start*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_Start*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_Start::Reply reply, *pReply;
    int32_t replySize = sizeof(reply);
    

    pReply = &reply;

    //TINYCLR_CLEAR(header);
	memset(&g_updateHeader, 0, sizeof(MFUpdateHeader));

    g_updateHeader.Version.usMajor = cmd->m_versionMajor;
    g_updateHeader.Version.usMinor = cmd->m_versionMinor;
    g_updateHeader.UpdateID        = cmd->m_updateId;
    g_updateHeader.UpdateType      = cmd->m_updateType;
    g_updateHeader.UpdateSubType   = cmd->m_updateSubType;
    g_updateHeader.UpdateSize      = cmd->m_updateSize;
    g_updateHeader.PacketSize      = cmd->m_updatePacketSize;

	reply.m_updateHandle = 1;

	hal_printf("update: %d %d\r\n", g_updateHeader.UpdateSize, g_updateHeader.PacketSize);
	
	// TODO: check for available contiguous storage region
    // TODO: if no storage, and version is greater, and erasing would give enough storage, then erase existing updates
    //reply.m_updateHandle = MFUpdate_InitUpdate(cmd->m_provider, header);*/

    /*if( reply.m_updateHandle == MFUpdate::badHandle || reply.m_updateHandle != cmd->m_updateId)
    {
        reply.m_updateHandle = cmd->m_updateId;
        reply.m_success = false;
    }
    else
    {
        reply.m_success = true;
    }*/

    //ReplyToCommand( msg, true, false, pReply, replySize );
    //TODO reset timer.  Don't send StartAck unless it was a unicast message (not doing that yet...) or timer expires (ie, wireless sender didn't send us the next state.)

    return true;
}

bool Samraksh_Emote_Update::AuthCommand( WP_Message* msg, void* owner )
{
	hal_printf("---- auth cmd packet not implemented\r\n");
   /* NATIVE_PROFILE_CLR_DEBUGGER();
    //MULTIPLEX_PACKETS();

    MFUpdate_Commands::Debugging_MFUpdate_AuthCommand*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_AuthCommand*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_AuthCommand::Reply reply, *pReply;

    int32_t respLen = 0;
    int32_t replySize = sizeof(reply);

    //TINYCLR_CLEAR(reply);
	memset(&reply, 0, sizeof(MFUpdate_Commands::Debugging_MFUpdate_AuthCommand::Reply));

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
*/
//    /*dbg->m_messaging->*/ReplyToCommand( msg, true, false, pReply, replySize );

/*    if(pReply != &reply)
    {
        private_free(pReply);
    }*/

    return true;
}

bool Samraksh_Emote_Update::Authenticate( WP_Message* msg, void* owner )
{
	hal_printf("---- authenticate packet not implemented\r\n");
    /*NATIVE_PROFILE_CLR_DEBUGGER();
    //MULTIPLEX_PACKETS();

    MFUpdate_Commands::Debugging_MFUpdate_Authenticate*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_Authenticate*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_Authenticate::Reply reply;

    reply.m_updateHandle = cmd->m_updateHandle;
    int32_t authType = 0;
    int32_t respLen = sizeof(authType);

    //memset(&s_missingPkts, 0xFF, sizeof(s_missingPkts));

    reply.m_success = 0;

    MFUpdate_AuthCommand(cmd->m_updateHandle, MFUPDATE_VALIDATION_COMMAND__GET_AUTH_TYPE, NULL, 0, (uint8_t*)&authType, respLen);

    if(authType == MFUPDATE_AUTHENTICATION_TYPE__SSL)
    {
        reply.m_success = 1;

        // reply early for SSL so that the device will try to upgrad the stream at the same time.*/
//        /*dbg->m_messaging->*/ReplyToCommand( msg, true, false, &reply, sizeof(reply) );

/*        Events_WaitForEvents(0, 400);
    }

    if(MFUpdate_Authenticate(cmd->m_updateHandle, cmd->m_authenticationData, cmd->m_authenticationLen))
    {
        reply.m_success = MFUpdate_Open(cmd->m_updateHandle);

        if(!reply.m_success)
        {
            reply.m_success = MFUpdate_Create(cmd->m_updateHandle);
        }
    }
*/
 //   /*dbg->m_messaging->*/ReplyToCommand( msg, true, false, &reply, sizeof(reply) );

    return true;
}


bool Samraksh_Emote_Update::GetMissingPkts( WP_Message* msg, void* owner )
{
	hal_printf("---- get missing packet\r\n");
    NATIVE_PROFILE_CLR_DEBUGGER();
    //MULTIPLEX_PACKETS();

    MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts::Reply reply, *pReply;


    MFUpdate* updateInfo = 0x0;
    int int32Cnt = 0;
    if(cmd->m_updateHandle != MFUpdate::badHandle) {
        updateInfo = g_Updates->GetUpdate(cmd->m_updateHandle);
        int32Cnt = updateInfo->m_missingPktsWordfieldSize;//ARRAYSIZE(s_missingPkts);  //number of uints needed to represent missing packets.
    }

    int32_t replySize = sizeof(reply);
    int32_t sizeBytes = (int32Cnt << 2) + offsetof(MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts::Reply, m_missingPkts);

    //TINYCLR_CLEAR(reply);
	memset(&reply, 0, sizeof(MFUpdate_Commands::Debugging_MFUpdate_GetMissingPkts::Reply));
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


	//ReplyToCommand( msg, true, false, pReply, replySize );

    if(pReply != &reply)
    {
        private_free(pReply);
    }

    return true;
}


bool Samraksh_Emote_Update::AddPacket(WP_Message* msg, void* owner )
{
	hal_printf("---- add packet\r\n");
/*    NATIVE_PROFILE_CLR_DEBUGGER();
    //MULTIPLEX_PACKETS();

    MFUpdate_Commands::Debugging_MFUpdate_AddPacket*       cmd = (MFUpdate_Commands::Debugging_MFUpdate_AddPacket*)msg->m_payload;
    MFUpdate_Commands::Debugging_MFUpdate_AddPacket::Reply reply;
    MFUpdate* updateInfo = g_Updates->GetUpdate(cmd->m_updateHandle);

    if(updateInfo != NULL)
    {
        if(cmd->m_packetIndex >> 5 < updateInfo->m_missingPktsWordfieldSize)
        {
            if(!have_packet(cmd->m_packetIndex, updateInfo->m_missingPkts))
            {
                reply.m_success = MFUpdate_AddPacket(cmd->m_updateHandle, cmd->m_packetIndex, &cmd->m_packetData[0], cmd->m_packetLength, (uint8_t*)&cmd->m_packetValidation, sizeof(cmd->m_packetValidation));

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
            reply.m_success = MFUpdate_AddPacket(cmd->m_updateHandle, cmd->m_packetIndex, &cmd->m_packetData[0], cmd->m_packetLength, (uint8_t*)&cmd->m_packetValidation, sizeof(cmd->m_packetValidation));
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

//ReplyToCommand( msg, true, false, &reply, sizeof(reply) );

    return (reply.m_success == TRUE);*/
}


/**
 * process a payload coming in over wireless... this is similar to the Debugger
 * inspecting the header to decide what to call, but it's re-implemented here
 * because the wireless interface comes directly here.  If we remove the Debugger,
 * this can help process incoming wired interface calls, too.
 */
bool Samraksh_Emote_Update::App_ProcessPayload(void* state, WP_Message* msg )
{
	hal_printf("---- process payload\r\n");
    bool ret = false;

	//if( msg->m_header.m_flags & WP_Flags::c_Reply )
    {
    //    if( msg->m_header.m_flags & WP_Flags::c_ACK )
        {
            //////////////////////////////////////////////////////////////////
            // UPDATE NEIGHBOR STATE FROM REPLY
            //////////////////////////////////////////////////////////////////
            //MFUpdate* updateInfo = NULL;
            switch(msg->m_header.m_cmd)
            {
            case c_Debugging_MFUpdate_Start:
            {
                /*MFUpdate_Commands::Debugging_MFUpdate_Start::Reply* incomingReply = (MFUpdate_Commands::Debugging_MFUpdate_Start::Reply* )msg->m_payload;
                CHECK_HANDLE(incomingReply->m_updateHandle);
                updateInfo = g_Updates->GetUpdate(incomingReply->m_updateHandle);
                CHECK_PTR(updateInfo);
                updateInfo->Flags |= NEIGHBOR_FLAGS__START;
                //Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, START_ACK, 0);
                if(s_fPublishUpdateMode == true) {
                    if( incomingReply->m_success == 1)
                    {
                        SendAuthCommand(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr);
                    }
                    else
                    {
                        //TODO: create and read reason for failure
                        SOFT_BREAKPOINT();
                    }
                }*/
            }
                break;
            case c_Debugging_MFUpdate_AddPacket:
            {
                /*MFUpdate_Commands::Debugging_MFUpdate_AddPacket::Reply* incomingReply = (MFUpdate_Commands::Debugging_MFUpdate_AddPacket::Reply* )msg->m_payload;
                CHECK_HANDLE(incomingReply->m_updateHandle);
                updateInfo = g_Updates->GetUpdate(incomingReply->m_updateHandle);
                CHECK_PTR(updateInfo);

                updateInfo->Flags |= NEIGHBOR_FLAGS__ADDPACKET;

                int idx_word = incomingReply->m_packetIndex / 32;
                int idx_bit = incomingReply->m_packetIndex % 32;
                s_destMissingPkts[idx_word] = s_destMissingPkts[idx_word] | (1u << idx_bit);

                //Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, ADDPACKET_ACK, incomingReply->m_nextMissingPacketIndex);
                // TODO: record next missing packet inside update struct, allow querying from c-sharp.
                if(s_fPublishUpdateMode == true) {
                    if( incomingReply->m_success == 1 )
                    {
                        if( incomingReply->m_nextMissingPacketIndex < updateInfo->m_finalPacketIdx )
                        {
                            SendAddPacket(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, incomingReply->m_nextMissingPacketIndex);
                        }
                        else
                        {
                            SendGetMissingPkts(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr);
                        }
                    }
                    else {
                        //TODO: handle failure.
                    }
                }
                else {
                    SOFT_BREAKPOINT(); // got a reply about an unknown update.
                }*/
            }
                break;
			}
		}
	}
    /*if( msg->m_header.m_flags & WP_Flags::c_Reply )
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
                //Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, START_ACK, 0);
                if(s_fPublishUpdateMode == true) {
                    if( incomingReply->m_success == 1)
                    {
                        SendAuthCommand(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr);
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

                //Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, ADDPACKET_ACK, incomingReply->m_nextMissingPacketIndex);
                // TODO: record next missing packet inside update struct, allow querying from c-sharp.
                if(s_fPublishUpdateMode == true) {
                    if( incomingReply->m_success == 1 )
                    {
                        if( incomingReply->m_nextMissingPacketIndex < updateInfo->m_finalPacketIdx )
                        {
                            SendAddPacket(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, incomingReply->m_nextMissingPacketIndex);
                        }
                        else
                        {
                            SendGetMissingPkts(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr);
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
                //Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, INSTALL_ACK, 0);
            }
                break;
            case c_Debugging_MFUpdate_AuthCommand:
            {
                MFUpdate_Commands::Debugging_MFUpdate_AuthCommand::Reply* incomingReply = (MFUpdate_Commands::Debugging_MFUpdate_AuthCommand::Reply* )msg->m_payload;
                CHECK_HANDLE(incomingReply->m_updateHandle);
                updateInfo = g_Updates->GetUpdate(incomingReply->m_updateHandle);
                CHECK_PTR(updateInfo);

                updateInfo->Flags |= NEIGHBOR_FLAGS__AUTHCMD;
                //Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, AUTHCMD_ACK, 0);
                if(s_fPublishUpdateMode == true) {
                    //TODO: implement authentication
                    SendAuthenticate(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr);
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
                //Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, AUTHENTICATE_ACK, 0);
                if(s_fPublishUpdateMode == true) {
                    SendGetMissingPkts(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr);
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
                for(uint32_t i = 0; i < updateInfo->m_missingPktsWordfieldSize; i++) {
                    s_destMissingPkts[i] = incomingReply->m_missingPkts[i];
                }

                //Samraksh_Emote_Update::s_UpdaterProgressHandler(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, GETMISSINGPKTS_ACK, 0);
                if(s_fPublishUpdateMode == true) {
                    uint32_t missingNo = GetFirstMissingPacket(updateInfo, incomingReply->m_missingPkts);
                    if( ( incomingReply->m_missingPktCount == 0 ) || ( missingNo > updateInfo->m_finalPacketIdx ) ) {
                        SendInstall(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr);
                    }
                    else {
                        SendAddPacket(updateInfo->Header.UpdateID, g_Samraksh_Emote_Update.s_destAddr, missingNo);
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
    }*/
    return ret;
}

bool InitializeInterface(){
	hal_printf("Init interface\r\n");
	return true;
}

bool InitializeDriversAfterInstall(){
	// verify hardware specific driver is installed
	// TODO:

	return true;
}

bool UnInitializeDriversBeforeInstall(){
	return true;
}

/**
 * replaces any pending completions
 */
bool Samraksh_Emote_Update::Install(WP_Message* msg, void* owner )
{
	hal_printf("---- install\r\n");
    /*NATIVE_PROFILE_CLR_DEBUGGER();
    //MULTIPLEX_PACKETS();

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
        //ASSERT_UPDATE_PROTOCOL(0);
        //TODO: record reason in reply message!
        goto Install_out;
    }

	if ( ( passed_validation = MFUpdate_Validate(cmd->m_updateHandle, &cmd->m_updateValidation[0], cmd->m_updateValidationSize) ) != TRUE) {
		g_Samraksh_Emote_Update.last_error = UPDATE_INSTALL_FAIL_VALIDATION;
		//ASSERT_UPDATE_PROTOCOL(passed_validation);
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
    ReplyToCommand( msg, true, false, &reply, sizeof(reply) );*/

    return TRUE;
}


/**
 * requires update has already passed validation.
 */
void Samraksh_Emote_Update::UpdateCompletion(void *arg)
{
	hal_printf("---- update complete\r\n");
    /*NATIVE_PROFILE_CLR_DEBUGGER();

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
        //ASSERT_UPDATE_PROTOCOL(successfully_installed);
	}

	private_free(arg);

    bool drivers_initialized = InitializeDriversAfterInstall();
    //FIXME: send install success message.
    reply.m_success = successfully_installed;
    ReplyToCommand( msg, true, false, &reply, sizeof(reply) );

    DEBUG_PRINTF_EMOTE_UPDATE("update install complete.\r\n");

    //Force reboot
	bool port_flushed = DebuggerPort_Flush( HalSystemConfig.DebugTextPort );
    Events_WaitForEvents( 0, 100 );

    CPU_Reset();*/
}



