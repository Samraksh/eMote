/*
 * OMACConstants.h
 *
 *  Created on: Sep 4, 2012
 *      Author: Mukundan
 */

#ifndef OMACCONSTANTS_H_
#define OMACCONSTANTS_H_

#include <tinyhal.h>

//types 1-5 are taken up by global types
#define OMAC_DATA_BEACON_TYPE 7

// Define default primes
#ifndef P1
#define P1 97
#endif

#ifndef P2
#define P2 103
#endif

//How often should a node wakeup (in ms). This is roughly the receive duty cycle.
#ifndef WAKEUP_INTERVAL
#define WAKEUP_INTERVAL 2000
#endif
#ifndef DATA_INTERVAL
#define DATA_INTERVAL 43
#endif

#ifndef OPTIMUM_BEACON_PERIOD
#define OPTIMUM_BEACON_PERIOD 1024 * 32
#endif

#define TIMESTAMP_FOOTER_OFFSET -4
#define TIMESTAMP_SIZE 4



#define MAX_UINT16 	(0xFFFF)
#define MAX_UINT32 	(0xFFFFFFFF)
#define MAX_UINT64 	(0xFFFFFFFFFFFFFFFF)
#define MID_UINT64  (0x7FFFFFFFFFFFFFFF)

#define MAX_DATA_PCKT_SIZE 100


/*
 *
 */
typedef enum {
  //after RadioControl.stopDone();
  E_TO_IDLE,
  //after RadioControl.start() is called
  E_TO_START,
  //after RadioControl.stop() is called
  E_TO_STOP,
  //after data beacon is sent
  E_TO_WAITING_DATA,
  //for the first queued packet
  E_TO_PRELOADING_DATA,
  E_TO_DATA_PRELOADED,
  E_TO_SENDING_DATA,
  //for time synchronization
  E_TO_BEACON_1,
  E_TO_BEACON_N,
} OMacEvent_t;

/*
 *
 */
typedef enum {
  //after RadioControl.stopDone();
  S_IDLE,
  //after RadioControl.start() is called
  S_STARTING,
  //after RadioControl.stop() is called
  S_STOPPING,
  //after data beacon is sent
  S_WAITING_DATA,
  //for the first queued packet
  S_PRELOADING_DATA,
  S_DATA_PRELOADED,
  S_SENDING_DATA,
  //for Discovery
  S_BEACON_1,
  S_BEACON_N,
  //Time Sync
  S_TIMESYNC
} OMacState_t;

/*
 *
 */
typedef enum {
  I_IDLE,
  I_DATA_RCV_PENDING, //waiting to receive
  I_DATA_SEND_PENDING, //pending to send
  I_TIMESYNC_PENDING,
  I_DISCO_PENDING,
  I_DWELL_SEND
} OMacInput_t;

/*
 *
 */
typedef enum {
  NULL_HANDLER,
  CONTROL_BEACON_HANDLER,
  DATA_RX_HANDLER,
  DATA_TX_HANDLER,
  TIMESYNC_HANDLER
} HandlerType_t;

/*
 *
 */
typedef enum {
  OMAC_NORMAL_SEND,
  OMAC_SEND_PRELOAD,
  OMAC_FIRST_SEND
} OMacAction_t;

/*
 *
 */
typedef struct DiscoveryMsg
{
	UINT32 msg_identifier;
	//seed to generate the pseduo-random wakeup schedule
	UINT16 nextSeed;
	UINT16 mask;
	//use to compute the offset of neighbor's current slot w.r.t. the start of the next frame
	//UINT64 nextwakeupSlot;
	UINT32 nextwakeupSlot0;
	UINT32 nextwakeupSlot1;
	//the wakeup interval of the neighbor
	UINT32 seedUpdateIntervalinSlots;
	//fields below are just for convenience. not transmitted over the air
	UINT16 nodeID;

	UINT32 localTime0;
	UINT32 localTime1;

	UINT32 lastwakeupSlotUpdateTimeinTicks0;
	UINT32 lastwakeupSlotUpdateTimeinTicks1;


} DiscoveryMsg_t;

#define DataMsgOverhead sizeof(UINT32)+sizeof(UINT8)
typedef struct DataMsg_t
{
	UINT32 msg_identifier;
	UINT8 size;
	UINT8 payload[MAX_DATA_PCKT_SIZE];
} DataMsg_t;
#define DataMsgOverhead sizeof(UINT32)+sizeof(UINT8)

/*
 * After TEP 133, the message timestamp contains the difference between
 * event time and the time the message was actually sent out. TimeSyncP
 * sends the local time associated with this globalTime to the
 * TimeStamping mechanism, which then calculates the difference.
 *
 * On the receiving side, the difference is applied to the local
 * timestamp. The receiving timestamp thus represents the time on the
 * receiving clock when the remote globalTime was taken.
 */
struct TimeSyncMsg
{

//  UINT32 globalTime0;
//  UINT32 globalTime1;

  //the time to startup the radio could be different for different nodes.
  //use this neighbor info along with local info to compute this difference
  //UINT16 radioStartDelay;
//  float skew;
  UINT32 timesyncIdentifier;
  UINT32 localTime0;
  UINT32 localTime1;
  bool request_TimeSync;
 // UINT16 nodeID;
  UINT32 seqNo;

};

struct TimeSyncRequestMsg
{
  UINT32 timesyncIdentifier;
  bool request_TimeSync;
};

/*
 *
 */
typedef struct DataBeacon {
  UINT16 nodeID;
} DataBeacon_t;

/*
 *
 */
typedef struct OMacHeader {
  UINT8 flag;
} OMacHeader;

//Overflow provisioned class
template<class Base_T>
class OFProv:Base_T{
public:
	bool isThereOverflow(const Base_T& rhs){
		if(rhs>*this){
			if(rhs - *this <= MID_UINT64) return false;
			else return true;
		}
		else{
			if(*this - rhs <= MID_UINT64) return false;
			else return true;
		}
	}
	bool operator<(const Base_T& rhs){
		if (rhs == *this) return false;
		else if(isThereOverflow(rhs)){
			if (rhs<*this) return true;
			else false;
		}
		else{
			if (rhs<*this) return true;
			else return false;
		}
	}
	bool operator>(const Base_T& rhs){
		if (rhs == *this) return false;
		else if(isThereOverflow(rhs)){
			if (rhs>*this) return true;
			else false;
		}
		else{
			if (rhs>*this) return true;
			else return false;
		}
	};
	bool operator<=(const Base_T& rhs){
		if (*this == rhs) return true;
		return (*this < *rhs);
	}
	bool operator>=(const Base_T& rhs){
		if (*this == rhs) return true;
		return (*this > *rhs);
	}
};

typedef OFProv<UINT64> OMACMicroSeconds;
typedef OFProv<UINT64> OMACTicks;

//GUARDTIME_MICRO should be calculated in conjuction with SLOT_PERIOD_MILLI
// GUARDTIME_MICRO = (SLOT_PERIOD_MILLI - PacketTime)/2 - SWITCHING_DELAY_MICRO
//PacketTime = 125byte * 8 bits/byte / (250*10^3 bits/sec) = 4sec
#define MICSECINMILISEC 1000
#define GUARDTIME_MICRO 6000			//compensate for time-sync errors; accounts for the clock drift
#define SWITCHING_DELAY_MICRO 0		//delay between switching between radio states
#define LISTEN_PERIOD_FOR_RECEPTION_HANDLER 2*GUARDTIME_MICRO
#define PACKET_PERIOD_FOR_RECEPTION_HANDLER 16000
#define ACK_TX_MAX_DURATION_MICRO 4000
#define TIMER_EVENT_DELAY_OFFSET 0
#define MINEVENTTIME 50000				//minimum time (in micro seconds) required by scheduler to switch between modules
#define SEED_UPDATE_INTERVAL_IN_SLOTS 100 //The FRAME SIZE in slots

//#define CCA_REACTION_TIME_MICRO 165 //BK: We need to double check this. This is the reaction time of the CCA module from the beginning of channel activity.
#define CCA_PERIOD_MICRO GUARDTIME_MICRO //BK: We need to double check this. Since 2 nodes will be off by this much. A node should CCA at least this much to make sure there was no other transmitter trying to reach the same destination.

#define MAX_PACKET_TX_DURATION_MICRO 16*MICSECINMILISEC
#define MAX_ACK_DELAY_MICRO 2000

#define HIGH_DISCO_PERIOD_IN_SLOTS 1000

#define FORCE_REQUESTTIMESYNC_INTICKS 100000000					//Translates to 120 secs @8Mhz. Receiver centric time threshold to request for a TImeSync msg.
#define SENDER_CENTRIC_PROACTIVE_TIMESYNC_REQUEST  60000000		//Translates to 10 secs @8Mhz. Sender centric time threshold to send a TImeSync msg.

#define WAKEUPPERIODINTICKS 8000000
enum {
  TICKS_PER_MILLI     = 8000,

#ifdef SHORT_SLOT
#warning *** USING 8ms SLOT ***
  SLOT_PERIOD_MILLI     = 8,    /*modify this along with SLOT_PERIOD_BITS*/
  SLOT_PERIOD_BITS    = 3 + 13,  /*13 = # of bits of TICKS_PER_MILLI, 4 = # of bits in SLOT_PERIOD_MILLI*/
#else
  SLOT_PERIOD_MILLI     = 16,     /*modify this along with SLOT_PERIOD_BITS*/
  SLOT_PERIOD_BITS    = 4 + 13,  /*13 = # of bits of TICKS_PER_MILLI, assuming its a 10Mhz clock, 4 = # of bits in SLOT_PERIOD_MILLI*/
#endif
  SLOT_PERIOD_TICKS   = SLOT_PERIOD_MILLI * TICKS_PER_MILLI,
  SLOT_PERIOD   = SLOT_PERIOD_MILLI,
  DWELL_TIME        = 10,
  /* sender margin compensates for time sync jitter, packet preload delay
   * and radio startup jitter*/
  SENDER_MARGIN_IN_MILLI = 3,
  SENDER_MARGIN     = SENDER_MARGIN_IN_MILLI * TICKS_PER_MILLI,
  TRANSITION_MARGIN   = SENDER_MARGIN_IN_MILLI * TICKS_PER_MILLI,
  MINIMUM_BACKOFF_WINDOW  = 64, /*in the unit of 1/32 ms*/
  RADIO_BACKOFF_WINDOW  = 64, /*in the unit of 1/32 ms*/
  RANDOM_SCHEDULE_WINDOW  = 0x0,
  /* receiver's wait time before the first packet arrives. It will not affect efficiency much
   * if the percentage of failed rendezvous is small because receivers go back to sleep upon
   * receiving the packet tagged as the last packet in the queue*/
  WAIT_TIME_AFTER_DATA_BEACON = (RADIO_BACKOFF_WINDOW + MINIMUM_BACKOFF_WINDOW) / TICKS_PER_MILLI + 12,
  WAIT_TIME_AFTER_PRELOAD = SLOT_PERIOD_MILLI,
  //8 minutes is the optimum time window for linear regression
  //the maximum number of times we tx a packet when previous
  //attempts fail due to congestion
  MAX_RETRY_CNT     = 10,
  // number of entries per neighbor
  MAX_ENTRIES             = 8,
  MAX_POOL_SIZE     = 32,
  MAX_CTRL_MSG_POOL_SIZE  = 8,
  //the number of consecutive messages sent during dwell time
  //DWELL_COUNT       = DEFAULT_DWELL_COUNT,
  DWELL_COUNT       = 3,
  DATA_ALARM_MAX_DURATION = 5,
  DELAY_AVG_FACTOR    = 9,
  //copy  from RIMAC implementation, should acknolwedge them
  //the number of cca detections needed to declare collision
  OMAC_COLLISION_CCA_THRESHOLD = 0,
  MAX_NON_SLEEP_STATE   = 10,
  MAX_NBR_SIZE      = 8,
  AM_DATA_BEACON      = 0x2E,
  AM_TIMESYNCMSG      = 0x3E,
  TIMESYNCMSG_LEN     = sizeof(TimeSyncMsg) - sizeof(UINT32) - sizeof(UINT16),
  INVALID_TIMESTAMP   = 0xFFFFFFFF,
  INVALID_ADDRESS     = 0xFFFF,
  INVALID_INDEX       = 0xFF,
  // time to declare itself the root if no msg was received (in sync periods)
  ROOT_TIMEOUT            = 5,
  // after becoming the root ignore other roots messages (in send period)
  IGNORE_ROOT_MSG         = 4,
  // of entries to become synchronized
  ENTRY_VALID_LIMIT       = 8,
  // if time sync error is bigger than this clear the table
  ENTRY_THROWOUT_LIMIT    = 200,
  // to detect whether my clock or my neighbor's clock has wrapped
  FLAG_TIMESTAMP_VALID  = (UINT8)1 << 3,
  FLAG_REQUEST_BEACON   = (UINT8)1 << 4,
  FLAG_DWELL_TIME     = (UINT8)1 << 6,
  OMAC_HEADER_LEN     = sizeof(OMacHeader),
  SEND_PIGGYBACK_BEACON = 1
    //TODO: needs random mechanism for DATA_INTERVAL
};

UINT16  CONTROL_BEACON_INTERVAL_SLOT = 7500;

UINT32 ArbiterP_Timing;

/*
 * Prime numbers used in determining DISCO period of a node
 */
UINT16 CONTROL_P1[] = {47, 37, 43, 37, 53, 29, 31};
UINT16 CONTROL_P2[] = {227, 181, 197, 191, 193, 211, 199};

#endif /* OMACCONSTANTS_H_ */
