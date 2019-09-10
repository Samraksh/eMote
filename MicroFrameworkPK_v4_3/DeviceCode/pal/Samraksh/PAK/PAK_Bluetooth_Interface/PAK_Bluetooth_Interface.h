#ifndef PAK_BLUETOOTH_INTERFACE_H_
#define PAK_BLUETOOTH_INTERFACE_H_


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

typedef void (*UPDATER_PROGRESS_HANDLER)(UINT32 updateID, UINT16 destAddr, UINT8 u4_cmd, UINT16 u12_data);

//TODO: specify packed attribute. do this better.
/**
 * simple payload contains only necessary pieces of WP_Message
 */
typedef struct {
    UINT32 cmd;
    UINT32 flags;
    UINT8 payload; //todo: make payload the field size since this struct can't be forward-declared anyway.
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

    //MACEventHandler PAK_EventHandler;  //!< handle incoming wireless events.
    

    UINT8 SendBuffer[Samraksh_Emote_Update::COMM_BUFFER_SIZE];          //!< local buffer for storing outgoing packets.      Regardless of use_wp_packet, internally packets are processed and stored as WP_Messages.
    UINT8 ReceiveBuffer[Samraksh_Emote_Update::COMM_BUFFER_SIZE];       //!< local buffer for manipulating incoming packets. Regardless of use_wp_packet, internally packets are processed and stored as WP_Messages.

    INT32 m_ReceiveState;

	UINT16 s_destAddr;           //!< wireless destination in basestation mode;

	static BOOL   s_fUseWpPacket;       //!< determines whether wireless interface will send entire WireProtocol packet.  idea is that short format fits inside single wireless packet payload.
	static BOOL   s_fPublishUpdateMode; //!< turn on/off update publisher mode
	static UINT32 s_destMissingPkts[MFUpdate::MAX_MISSING_WORDFIELD_SIZE];
   /* static UINT8  s_RadioID;            //!< MAC API change causes storage of Radio ID outside of MacConfig.
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

    static void Receive(void* msg, UINT16 size);
    static void SendAck(void* msg, UINT16 size, NetOpStatus status);

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

///////////////////////////////////////////////


extern Samraksh_Emote_Update    g_Samraksh_Emote_Update;

bool InitializeInterface();	    
bool InitializeDriversAfterInstall();
bool UnInitializeDriversBeforeInstall();

#endif /* PAK_BLUETOOTH_INTERFACE_H_ */
