#ifndef __PAK_BT_INTERFACE_H__
#define __PAK_BT_INTERFACE_H__

#include <TinyCLR_Types.h>   //CLR_RECORD_ASSEMBLY
#include <WireProtocol.h>
#include <Samraksh\Mac_decl.h>
#include <Samraksh\Message.h>
#include <MFUpdate_decl.h>


#define CHECK_HANDLE(h) { if((h) == MFUpdate::badHandle) { SOFT_BREAKPOINT(); break; } }

#define CHECK_PTR(p) { if((p) == NULL) { SOFT_BREAKPOINT(); break; } }

/**
 * UpdateMessage in UpdateState.cs
 * must fit inside 4 bits for packing to CLR via in NativeToManagedUpdaterProgressHandler to NativeEventQueue
 */
enum UpdateMessage
{
        NOMSG = 0,
        START = 1, START_ACK = 2,
        AUTHCMD = 3, AUTHCMD_ACK = 4,
        AUTHENTICATE = 5, AUTHENTICATE_ACK = 6,
        GETMISSINGPKTS = 7, GETMISSINGPKTS_ACK = 8,
        ADDPACKET = 9, ADDPACKET_ACK = 10,
        INSTALL = 11, INSTALL_ACK = 12,
};

/// used for g_UpdateManagerContext for Interop callback.
// #include <TinyCLR_Runtime.h>
//extern CLR_RT_HeapBlock_NativeEventDispatcher *g_UpdateManagerContext;
//extern UINT64 g_UpdateManagerUserData;
//extern BOOL g_UpdateManagerInterruptEnabled;

typedef void (*UPDATER_PROGRESS_HANDLER)(uint32_t updateID, uint16_t destAddr, uint8_t u4_cmd, uint16_t u12_data);

//TODO: specify packed attribute. do this better.
/**
 * simple payload contains only necessary pieces of WP_Message
 */
typedef struct {
    uint32_t cmd;
    uint32_t flags;
    uint8_t payload; //todo: make payload the field size since this struct can't be forward-declared anyway.
} simple_payload_t;

enum Emote_Update_ErrorFlags {
    UPDATE_INSTALL_NO_MSG          = 0x1 << 0,
    UPDATE_INSTALL_FAIL_RADIO      = 0x1 << 1,
    UPDATE_INSTALL_FAIL_TIME       = 0x1 << 2,
    UPDATE_INSTALL_FAIL_VALIDATION = 0x1 << 3,
    UPDATE_INSTALL_MSG_OUT_OF_SYNC = 0x1 << 4,
    UPDATE_SEND_BADHANDLE          = 0x1 << 5,
    UPDATE_INSTALL_FAIL_INSTALL    = 0x1 << 6,
};

inline Emote_Update_ErrorFlags operator|(Emote_Update_ErrorFlags a, Emote_Update_ErrorFlags b) {
    return static_cast<Emote_Update_ErrorFlags>(static_cast<int>(a) | static_cast<int>(b));
}

struct Samraksh_Emote_Update
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
    uint8_t m_outboundMessagePayload[128];

    

    Emote_Update_ErrorFlags last_error;


public:
	static const uint32_t c_Monitor_UpdateInit                       = 0x0000000F; //!< WireProtocol packet type.  Must match duplicate definition in TinyCLR_Debugging.h' CLR_DBG_Commands::
    static const uint32_t c_Monitor_UpdateDeInit                     = 0x00000010; //
    static const uint32_t c_Debugging_MFUpdate_Start                 = 0x00020056; //
    static const uint32_t c_Debugging_MFUpdate_AddPacket             = 0x00020057; //
    static const uint32_t c_Debugging_MFUpdate_Install               = 0x00020058; //
    static const uint32_t c_Debugging_MFUpdate_AuthCommand           = 0x00020059; //
    static const uint32_t c_Debugging_MFUpdate_Authenticate          = 0x00020060; //
    static const uint32_t c_Debugging_MFUpdate_GetMissingPkts        = 0x00020061; //
    static const int32_t COMM_BUFFER_SIZE = (IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t));
    //TODO: static_assert(COMM_BUFFER_SIZE == MAC.h's template instantiation size minus the header).

    //MACEventHandler PAK_EventHandler;  //!< handle incoming wireless events.
    

    uint8_t SendBuffer[Samraksh_Emote_Update::COMM_BUFFER_SIZE];          //!< local buffer for storing outgoing packets.      Regardless of use_wp_packet, internally packets are processed and stored as WP_Messages.
    uint8_t ReceiveBuffer[Samraksh_Emote_Update::COMM_BUFFER_SIZE];       //!< local buffer for manipulating incoming packets. Regardless of use_wp_packet, internally packets are processed and stored as WP_Messages.

    int32_t m_ReceiveState;

	uint16_t s_destAddr;           //!< wireless destination in basestation mode;

	static BOOL   s_fUseWpPacket;       //!< determines whether wireless interface will send entire WireProtocol packet.  idea is that short format fits inside single wireless packet payload.
	static BOOL   s_fPublishUpdateMode; //!< turn on/off update publisher mode
	static uint32_t s_destMissingPkts[MFUpdate::MAX_MISSING_WORDFIELD_SIZE];
   /* static uint8_t  s_RadioID;            //!< MAC API change causes storage of Radio ID outside of MacConfig.
    static BOOL   s_fRadioOn;           //!< indicates whether MAC is initialized.
    
    static BOOL   s_fBaseStationMode;   //!< turn on/off basestation mode.
    
    
    
    static UPDATER_PROGRESS_HANDLER s_UpdaterProgressHandler; //!< notify (managed library) something happened.*/
	

    static void CreateInstance();
    static void Initialize();
    //static bool InitializeMac();
    //static bool InitializeMacHandler();
    //static bool SetMacHandler();
    static bool InitializeDriversAfterInstall();
    static bool UnInitializeDriversBeforeInstall();
    static void Cleanup();
    static void DeleteInstance();

    static void Receive(void* msg, uint16_t size);
    static void SendAck(void* msg, uint16_t size, NetOpStatus status);

    static bool ReadReceiveBytes( void* state, unsigned char*& ptr, unsigned int & size );
    static bool TransmitMessage ( void* state, const WP_Message* msg      );

    static bool App_ProcessHeader ( void* state,  WP_Message* msg );
    static bool App_ProcessPayload( void* state,  WP_Message* msg );
    static bool App_Release       ( void* state,  WP_Message* msg );

    static void SendStart         ( UpdateID_t id, uint16_t destAddr );
    static void SendAuthCommand   ( UpdateID_t id, uint16_t destAddr );
    static void SendAuthenticate  ( UpdateID_t id, uint16_t destAddr );
    static void SendGetMissingPkts( UpdateID_t id, uint16_t destAddr );
    static void SendAddPacket     ( UpdateID_t id, uint16_t destAddr, uint32_t packetIndex );
    static void SendInstall       ( UpdateID_t id, uint16_t destAddr );
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

    //static void SetUpdateProgressCallback(UPDATER_PROGRESS_HANDLER*);

protected:
    static void UpdateCompletion(void *arg);
};

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
    uint32_t m_updateType;
    uint32_t m_updateSubType;
    uint32_t m_updateSize;
    uint32_t m_updatePacketSize;
    uint16_t m_versionMajor;
    uint16_t m_versionMinor;

    struct Reply
    {
        UpdateID_t m_updateHandle;
        int32_t  m_success;
    };
};

struct Debugging_MFUpdate_AuthCommand
{
    uint32_t m_updateHandle;
    uint32_t m_authCommand;
    uint32_t m_authArgsSize;
    uint8_t  m_authArgs[1];

    struct Reply
    {
        uint32_t m_updateHandle;
        int32_t  m_success;
        uint32_t m_responseSize;
        uint8_t  m_response[1];
    };
};

struct Debugging_MFUpdate_Authenticate
{
    uint32_t m_updateHandle;
    uint32_t m_authenticationLen;
    uint8_t  m_authenticationData[1];

    struct Reply
    {
        uint32_t m_updateHandle;
        int32_t m_success;
    };
};

struct Debugging_MFUpdate_GetMissingPkts
{
    uint32_t m_updateHandle;

    struct Reply
    {
        uint32_t m_updateHandle;
        int32_t  m_success;
        int32_t  m_missingPktCount;
        uint32_t m_missingPkts[1];
    };
};

struct Debugging_MFUpdate_AddPacket
{
    uint32_t m_updateHandle;
    uint32_t m_packetIndex;
    uint32_t m_packetValidation;
    uint32_t m_packetLength;
    uint8_t m_packetData[1];

    struct Reply
    {
        uint32_t m_updateHandle;
        int32_t  m_success;
        uint32_t m_packetIndex;
        uint32_t m_nextMissingPacketIndex;
    };
};

#define MAX_UPDATE_VALIDATION_SIZE_IN_BYTES 8

struct Debugging_MFUpdate_Install
{
    uint32_t m_updateHandle;
    uint32_t m_updateValidationSize; //<! size in bytes of m_updateValidation
    uint8_t  m_updateValidation[1];

    struct Reply
    {
        uint32_t m_updateHandle;
        int32_t m_success;
    };
};

};

///////////////////////////////////////////////


extern Samraksh_Emote_Update    g_Samraksh_Emote_Update;

bool InitializeInterface();	    
bool InitializeDriversAfterInstall();
bool UnInitializeDriversBeforeInstall();

#endif /* __PAK_BT_INTERFACE_H__ */
