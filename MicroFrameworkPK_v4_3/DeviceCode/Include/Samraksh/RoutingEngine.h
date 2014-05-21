#ifndef _ROUTINGENGINE_H_
#define _ROUTINGENGINE_H_

#include <Samraksh\Constants.h>
#include <Samraksh\Message.h>
#include <Samraksh\Neighbors.h>
#include <Samraksh\Buffer.h>
#include <tinyhal.h>
#include <Samraksh\HALTimer.h>
#include <Samraksh\HAL_util.h>
#include <Samraksh\Mac_decl.h>
#include <Samraksh\MAC.h>
#include <..\..\radio\RF231\RF231.h>
#include <Samraksh\Radio_decl.h>

//Higher alpha is preferring present over past. Varies from 0 to 10 
const UINT8 ALPHA_SNR = 10;
const UINT8 ALPHA_BEACON = 5;
const UINT8 ALPHA_DATA = 5;

//Beta represents weightage of etx in total etx. beta should sum to 10
const UINT8 BETA_SNR = 5;
const UINT8 BETA_BEACON = 5;
const UINT8 BETA_DATA = 0;

//Routing Layer types
typedef UINT16 Ctp_Addr_T;
typedef UINT16 Ctp_Msg_Len_T;

/*struct ErrorFlags{
	enum Values{
		SUCCESS = 0x0,
		FAILURE = 0X1
		//0x2
		//0x4
		//0x8
		//0x16
		//0x32
		//0x64
		//0x128
	};
};*/

typedef UINT8 Ctp_SeqNo_T;

typedef UINT8 Message_T;

typedef UINT16 Etx_T;

class ClientMsgRecvDTO
{
public:
	UINT8* Msg;
	UINT16 Length;
	UINT16 Source;
	UINT16 Destination;
};

class ClientMsgAckDTO
{
public:
	UINT8* Msg;
	UINT16 Length;
};

//TODOI: Implement separate states for tables? Currently it gives a redef error.
struct TableEntryState{
enum Values{
	Invalid = 0x0,
	Valid = 0x1,
	Mature = 0x2,
	Pinned = 0x4
	};
};

struct CtpMessageType{
	enum Values
	{
		BEACON = 0x0,
		DATA = 0X1
	};
};

class CtpDataFrame
{
public:
	UINT8 Flags;
	UINT8 THL;
	Etx_T MultihopEtx;
	Ctp_Addr_T Origin;
	Ctp_SeqNo_T SeqNo;
};

//const Ctp_Msg_Len_T CTP_MAX_DATA_PAYLOAD = MAC_MAX_PAYLOAD - sizeof(CtpDataFrame);

typedef struct DataMessage
{
public:
	//CtpDataFrame Frame;
	//Message_T Payload[IEEE802_15_4_MAX_PAYLOAD];
	//IEEE802_15_4_Message_t Payload;
	UINT32 SeqNo;
	Ctp_Addr_T Sender;
	BOOL decision;
} CtpDataMessage;

class CtpRoutingFrame
{
public:
	UINT8 Flags;
	Ctp_Addr_T Parent;
	Etx_T MultihopEtx;
};

class LEHeader
{
public:
	UINT8 FooterCount;
	Ctp_SeqNo_T SeqNo;
};

class LEFooterEntry
{
public:
	Etx_T LinkEtx;
	Ctp_Addr_T Neighbor;
};
	

const UINT8 LE_FOOTER_COUNT = (
	MAC_MAX_PAYLOAD - sizeof(LEHeader)-sizeof(CtpRoutingFrame))
	/ sizeof(LEFooterEntry);

class CtpRoutingBeaconMessage
{
public:
	UINT32 SeqNo;
	LEHeader Header;
	Ctp_Addr_T Sender;
	CtpRoutingFrame Frame;
	//LEFooterEntry Footer[LE_FOOTER_COUNT];
};

class CandidateTableEntry
{
public:
	UINT8 State;
	Ctp_Addr_T Neighbor;
	UINT8 PacketsReceived;
	//Contains SNR values received so far
	UINT16 SnrValues[LE_SNR_WINDOW_SIZE];
	UINT8 FrameSizes[LE_SNR_WINDOW_SIZE];
};

typedef struct TableEntry
{
	UINT8 State;
	Ctp_Addr_T Neighbor;
	UINT16 NeighborMac;
	Ctp_Addr_T Parent;
	Etx_T MultiHopETX;
} RoutingTableEntry;

/*UINT8 RoutingTableEntry::State;
Ctp_Addr_T RoutingTableEntry::Neighbor;
UINT16 RoutingTableEntry::NeighborMac;
Ctp_Addr_T RoutingTableEntry::Parent;
Etx_T RoutingTableEntry::MultiHopETX;*/

class RoutingEngine
{
private:
	static BOOL SendAckPending;
	static BOOL ResponsePending;
	static UINT8 MacID;
	
	static Ctp_Addr_T _self;
	static int _parentIndex;
	static RoutingTableEntry _routingTable[RE_TABLE_SIZE];
	static bool _isRootNode;
	static CtpRoutingBeaconMessage* _beacon;

	//Route refresh timer
	static HALTimerManager refreshTimer;
	
	//MAC variables
	static MacEventHandler_t Event_Handler;
	static UINT16 lastPacketID;
	static UINT32 failureToExtract;
	static UINT16 missedResponses;
	
	//Link estimator variabes
	//LEHeader* _leHeader;
	//Ctp_SeqNo_T _beaconSequenceNumber;
	//CandidateTableEntry _candidateTable[LE_CANDIDATE_TABLE_SIZE];
	//UINT8 _prevNeighborSentID;
	
	/*
	Returns the best valid neighbor based on total ETX. If no link estimate exists,
	returns best valid neighbor based on multihopEtx only.
	*/
	static int FindBestNeighbor();
	static int FindWorstNeighbor();
	static int FindNeighbor(Ctp_Addr_T neighbor);
	static int FindEmptyNeighbor();
	static void InitNeighborEntry(int index, Ctp_Addr_T neighbor, UINT16 sourceMacAddres);
	static void ClearNeighborEntry(int index);
	static void TryInsertNeighbor(Ctp_Addr_T neighbor, Etx_T multihopEtx, UINT16 sourceMacAddres);
	static void ProcessFrame(CtpRoutingFrame* frame);
	
	//Creates CTP Routing Frame corresponding to current state.
	static DeviceStatus SetFrame(CtpRoutingFrame *frame);

	//Runs parent re-selection procedure.
	static void RefreshParent();
	
	//**Helpers**
	static Etx_T ComputeEtx(UINT16 snr, UINT8 frameSizeInBytes);
	static Etx_T GetTotalEtx(Etx_T snr, Etx_T beacon, Etx_T data);

	//**LE methods**
	//void ProcessReceivedBeacon(CtpRoutingBeaconMessage* msg, Ctp_Addr_T src, UINT16 snr);

	//Returns new message length, updates seqNo and prevSentID internally.
	//DeviceStatus SetLEHeaderAndFooter(CtpRoutingBeaconMessage *msg);
	//DeviceStatus GetIncomingLinkQuality(Ctp_Addr_T neighbor, Etx_T* etx);

	//**Candidate table methods**
	//DeviceStatus InitializeCandidateEntry(UINT8 index, Ctp_Addr_T neighbor);
	//DeviceStatus ClearCandidateEntry(UINT8);
	//int FindCandidateIndex(Ctp_Addr_T candidate);
	//int FindEmptyCandidateIndex();

	//Adds SNR value and if enough values are available, calls PromoteCandidate
	//void AddCandidateSnrFrame(CandidateTableEntry* entry, UINT16 snr, UINT8 frame);

	/*
	Should be called after all SNR values have been obtained. Tries to promote
	candidate to neighbor. Always discards the candidate entry irrespective of 
	promotion status.
	*/
	//DeviceStatus PromoteCandidate(CandidateTableEntry* entry);
	
	//Callback functions
	static void routeRefreshCycle(void* arg);
	static void SendAckCallback(void *msg, UINT16 size, NetOpStatus status);
	static void RecieveCallback(UINT16 numberOfPacketsInBuffer);
	static void NeigbhbourChangeCallback(INT16 arg1);
	
	//Private initialization of mac layer
	static DeviceStatus InitializeMacLayer(UINT8 mac, UINT8 radio);
        
public:
	//static CtpDataMessage msgqueue;
	static DeviceStatus SendBeacon();
	static DeviceStatus SendData(CtpDataMessage msg);
	
	//Bag should be initialized with RoutingEngine and LinkEstimator before passing
	static DeviceStatus InitializeRoutingLayer(CtpRoutingBeaconMessage *beacon, Ctp_Addr_T routingAddress, UINT8 radio);

	//Root management
	static void SetRoot()
	{
		_isRootNode = true;
	}
	static bool IsRoot()
	{
		return _isRootNode;
	}

	//Is there a valid route?
	static bool HasValidRoute()
	{
		return _parentIndex!=-1;
	}

	static Ctp_Addr_T GetSelfAddress()
	{
		return _self;
	}
	//Events
	//void ValidParentFound(Ctp_Addr_T validParent);
	//void RouteIsInvalid();
	//UINT8 TimerFiredHandler(void* arg);

	/* 
		Triggers immediate refresh of RE, parent is re-selected and a beacon is
		scheduled to be sent.
	*/
	//void TriggerRefresh();

	//  Get current neighbor count
	//UINT8 GetNeighborCount();

	/*
		Returns: Failure if neighbor doesn't exist else the multi-hop etx of 
		the neighbor. 
		Input: *etx is a buffer which needs to be allocated by caller. It should
		be ignored if Failure is returned.
	*/
	static DeviceStatus GetRouteQuality(Ctp_Addr_T neighborAddress, Etx_T *etx);

	//Get 'multihop' Etx of the current parent
	static DeviceStatus GetParentEtx(Etx_T *etx);

	/* 
		Returns: Failure if parent is invalid. 
		Input: *address is a buffer which needs to be allocated by caller. It
		should be ignored if Failure is returned.
	*/
	static DeviceStatus GetParent(Ctp_Addr_T* address);

	/*
		Handler for the event of a neighbor being discovered.
		TO2DO: Subscribe to neighbor discovery service.
	*/
	static void HandleNeighborDiscovered(Ctp_Addr_T neighbor);

	/* 
		Gets called whenever LE receives a beacon. Ignores frame if the sending
		node doesn't have valid route.
	*/
	static CtpRoutingBeaconMessage *Receive(CtpRoutingBeaconMessage *msg, Ctp_Addr_T source);

	/*
	Returns true if routing table has the neighbor. 
	*/
	static bool GetCompareBit(Ctp_Addr_T neighbor);
	
	/*Link estimator functions*/
	//Send message
	static DeviceStatus Send(CtpMessageType::Values type, UINT16 mac_addr, char* payload, UINT16 size);
	
	//Return link quality from self to neighbor
	//DeviceStatus GetForwardLinkQuality(Ctp_Addr_T neighbor, Etx_T *etx);
	//DeviceStatus InsertCandidate(Ctp_Addr_T neighbor);
	//DeviceStatus PinNeighbor(Ctp_Addr_T neighbor);
	//DeviceStatus UnpinNeighbor(Ctp_Addr_T neighbor);
	//Called whenever a data packet (ack or data) is received from a neighbor
	//TODOI
	//DeviceStatus RxData(Ctp_Addr_T neighbor, UINT16 snr, UINT8 totalPacketSize);
	//void NeighborEvicted(Ctp_Addr_T neighbor);
	
	//int FindNeighborIndex(Ctp_Addr_T neighbor);
	//int FindEmptyNeighborIndex();
	//int FindWorstNeighborIndex();
};

BOOL RoutingEngine::SendAckPending;
BOOL RoutingEngine::ResponsePending;
UINT8 RoutingEngine::MacID;
Ctp_Addr_T RoutingEngine::_self;
int RoutingEngine::_parentIndex;
RoutingTableEntry RoutingEngine::_routingTable[RE_TABLE_SIZE];
bool RoutingEngine::_isRootNode;
//CtpDataMessage RoutingEngine::msgqueue;
CtpRoutingBeaconMessage* RoutingEngine::_beacon;
//Route refresh timer
HALTimerManager RoutingEngine::refreshTimer;
//MAC variables
MacEventHandler_t RoutingEngine::Event_Handler;
UINT16 RoutingEngine::lastPacketID;
UINT32 RoutingEngine::failureToExtract;
UINT16 RoutingEngine::missedResponses;

#endif //!_CTP_ROUTING_ENGINE_H_
