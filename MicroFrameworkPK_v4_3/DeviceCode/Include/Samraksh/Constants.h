#ifndef _CTP_CONSTANTS_H_
#define _CTP_CONSTANTS_H_
#include <tinyhal.h>
#include <math.h>
#include <Samraksh\Message.h>

const UINT16 MAC_INVALID_ADDRESS = 0xFFFF;
const UINT16 MAC_MAX_PAYLOAD = IEEE802_15_4_MAX_PAYLOAD;

//Maximum path length a node is allowed to traverse
const UINT8 CTP_MAX_PATH_LENGTH = 10;

//Minimum reliability expected from a link in percentage.
const UINT8 CTP_MIN_LINK_RELIABILITY = 80;

const UINT16 CTP_INVALID_ETX = 0xFFFF;
const UINT16 CTP_THEORETICAL_MAX_ETX = 0xFFFF;

//TODOI: Change to 60 after debugging.
const UINT8 RE_BEACON_INTERVAL_IN_SEC = 5;

const UINT8 REL_BUF_SIZE = 3;
const UINT8 RE_TABLE_SIZE = 10;
const UINT8 LE_TABLE_SIZE = 10;
const UINT8 LE_CANDIDATE_TABLE_SIZE = 5;
const UINT8 LE_SNR_WINDOW_SIZE = 4;
const UINT8 LE_BEACON_WINDOW_SIZE = 10;

//Maximum allowed ETX
const UINT8 LE_ETX_THRESHOLD = ceil(1000 / CTP_MIN_LINK_RELIABILITY);		//13

/*
The maximum average time a beacon should take to get sent from an external node
to the local node.
*/
const UINT8 LE_MAX_BEACON_TIME =
	ceil(RE_BEACON_INTERVAL_IN_SEC * LE_ETX_THRESHOLD / 10);

/*
The maximum amount of time a candidate is allowed in the candidate table before
being discarded. Given in terms of beacon timer's timeouts.
Should be using LE_SNR_WINDOW_SIZE - 1, but the following formula is intended to
be less strict.
*/
const UINT8 LE_CT_AGE_BEACON_COUNT = ceil(
	LE_MAX_BEACON_TIME * LE_SNR_WINDOW_SIZE / RE_BEACON_INTERVAL_IN_SEC);	//6
/*
Number of minimum consecutive packet sending attempts from an external node which
must have failed for the local node to consider it dead. The packet count is
expressed in terms of beacons.
*/
const UINT8 LE_DEATH_WINDOW_SIZE = 6;

/*
External neighbor death is counted in terms of consecutive local beacon time 
intervals elapsed. This is reflected in the following value.
*/
const UINT8 LE_DEATH_BEACON_COUNT = ceil(
	LE_MAX_BEACON_TIME * LE_DEATH_WINDOW_SIZE / RE_BEACON_INTERVAL_IN_SEC);	//8

const UINT8 FE_MSG_Q_SIZE = 10;
const UINT8 FE_DUPLICATE_Q_SIZE = 20;
const UINT8 FE_MAX_RETRANSMITS = 3;
const UINT16 FE_TIMER_PERIOD_IN_MSEC = RE_BEACON_INTERVAL_IN_SEC * 500;
const UINT16 FE_TIMER_START_TIME_IN_MSEC = 0;/*(LE_SNR_WINDOW_SIZE + 1) *
	RE_BEACON_INTERVAL_IN_SEC * 1000;*/

#endif//!_CTP_CONSTANTS_H_
