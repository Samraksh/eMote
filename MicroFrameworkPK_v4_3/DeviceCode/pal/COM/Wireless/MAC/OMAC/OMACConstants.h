/*
 * OMACConstants.h
 *
 *  Created on: Sep 4, 2012
 *      Author: Mukundan
 */

#ifndef OMACCONSTANTS_H_
#define OMACCONSTANTS_H_

#include <tinyhal.h>

// Define default primes
#ifndef P1
#define P1 97
#endif

#ifndef P2
#define P2 103
#endif

#ifndef DATA_INTERVAL
#define DATA_INTERVAL 43
#endif

#ifndef OPTIMUM_BEACON_PERIOD
#define OPTIMUM_BEACON_PERIOD 1024 * 32
#endif

#define TIMESTAMP_FOOTER_OFFSET -4

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
  //for time synchronization
  S_BEACON_1,
  S_BEACON_N,
} OMacState_t;

typedef enum {
  I_IDLE,
  I_WAITING_DATA, //waiting to receive
  I_DATA_PENDING, //pending to send
  I_DWELL_SEND
} OMacInput_t;

typedef enum {
  NULL_HANDLER,
  CONTROL_BEACON_HANDLER,
  DATA_RX_HANDLER,
  DATA_TX_HANDLER
} HandlerType_t;

typedef enum {
  OMAC_NORMAL_SEND,
  OMAC_SEND_PRELOAD,
  OMAC_FIRST_SEND
} OMacAction_t;

/* IEEE154 types are from 0 to 3. OMAC extends it to 4 for data beacons.
 * Both data packets and t-sync control beacons are treated as IEEE154_TYPE_DATA */
typedef enum {
  OMAC_DATA_BEACON_TYPE = 4,
} OMacPacketType;

typedef struct TimeSyncMsg
{
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
  UINT32 globalTime;

  /* offset between the start of the counter to global time 0
   * used by neighbors to calculate the sender's next receive slot
   */
  UINT16 counterOffset;
  //the time to startup the radio could be different for different nodes.
  //use this neighbor info along with local info to compute this difference
  UINT16 radioStartDelay;
  /* Beacons requested by Disco that are invalid for ftsp because of
   * insufficient number of samples
   */
  UINT8  flag;
  //seed to generate the pseduo-random wakeup schedule
  UINT16 seed;
  //use to compute the offset of neighbor's current slot w.r.t. the start of the next frame
  UINT32 nextFrame;
  //the wakeup interval of the neighbor
  UINT16 dataInterval;
  /*fields below are just for convenience. not transmitted over the air*/
  UINT32 localTime;
  UINT16 nodeID;
} TimeSyncMsg_t;

typedef struct DataBeacon {
  UINT16 nodeID;
#ifdef J_MULCH
  UINT8 ch;
#endif
} DataBeacon;

typedef struct OMacHeader {
  UINT8 flag;
} OMacHeader;

enum {
  TICKS_PER_MILLI     = 32,

#ifdef SHORT_SLOT
#warning *** USING 8ms SLOT ***
  SLOT_PERIOD_MILLI     = 8,    /*modify this along with SLOT_PERIOD_BITS*/
  SLOT_PERIOD_BITS    = 3 + 5,  /*5 = # of bits of TICKS_PER_MILLI, 4 = # of bits in SLOT_PERIOD_MILLI*/
#else
  SLOT_PERIOD_MILLI     = 16,     /*modify this along with SLOT_PERIOD_BITS*/
  SLOT_PERIOD_BITS    = 4 + 13,  /*13 = # of bits of TICKS_PER_MILLI, assuming its a 10Mhz clock, 4 = # of bits in SLOT_PERIOD_MILLI*/
#endif
  //SLOT_PERIOD   = SLOT_PERIOD_MILLI * TICKS_PER_MILLI,
  SLOT_PERIOD   = SLOT_PERIOD_MILLI,
  DWELL_TIME        = 10,
  /* sender margin compensates for time sync jitter, packet preload delay
   * and radio startup jitter*/
  SENDER_MARGIN     = 3 * TICKS_PER_MILLI,
  TRANSITION_MARGIN   = 3 * TICKS_PER_MILLI,
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

UINT16 CONTROL_P1[] = {47, 37, 43, 37, 53, 29, 31};
UINT16 CONTROL_P2[] = {227, 181, 197, 191, 193, 211, 199};

#endif /* OMACCONSTANTS_H_ */
