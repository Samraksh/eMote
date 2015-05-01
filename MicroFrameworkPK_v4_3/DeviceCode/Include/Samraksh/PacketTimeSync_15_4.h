/*
 * PacketTimeSync.h
 * Copyright: The Samraksh Company
 * Created on: Sep 10, 2012
 * Author: Mukundan Sridharan
 * Implements a packet-level eventTime api
 */

#ifndef PACKET_TIMESYNC_15_4_H_
#define PACKET_TIMESYNC_15_4_H_

#include <Tinyhal.h>
#include <Samraksh/Message.h>

#define TIMESTAMP_OFFSET -4
#define TIMESTAMP_ERROR 0x80000000
//A combined base class for timesync, analogous to TimeSyncMessageC in TinyOS


#define TXRXOFFSET 2420
//Provides Packet level timesync: Similar to TinyOS TEP 133
//http://www.tinyos.net/tinyos-2.1.0/doc/html/tep133.html

class PacketTimeSync_15_4{
  public:
        //EventTime APIs
		// Size here refers to the size of the whole packet including header
        static UINT64 EventTime(Message_15_4_t *msg, UINT8 size){
                UINT8 * rcv_msg =  msg->GetPayload();
                UINT32 * senderEventTime = (UINT32 *)((UINT32)rcv_msg + size - sizeof(IEEE802_15_4_Header_t) + TIMESTAMP_OFFSET);

                UINT64 rcv_ts = msg->GetMetaData()->GetReceiveTimeStamp();

                UINT32 sender_delay = *senderEventTime;
                //BK: I don't understand why sender delay is added but it works better. Check whether it is negated somewehere
                rcv_ts = rcv_ts + (UINT64) sender_delay - (UINT64)TXRXOFFSET * (CPU_TicksPerSecond()/1000000) ;
                return rcv_ts;
        }

        static bool IsValid(Message_15_4_t *msg){
                if(msg->GetMetaData()->GetReceiveTimeStamp()==TIMESTAMP_ERROR)
                        return FALSE;
                else
                        return TRUE;
        }
};

/*
template<class Message_T, UINT8 PayloadOffset_T>
class TimeSyncMessage{

public:
        //Called on receiving a timesync packet


//        DeviceStatus Send(RadioAddress_t address, Message_T * msg, UINT16 size, UINT64 event_time){

//}

        static void * GetPayload (Message_T * msg, UINT16 Size){
                return (msg + PayloadOffset_T);
        }

        //TimeSyncPacket APIs
        UINT64 EventTime(Message_T *msg){

        }
        bool IsValid(Message_T *msg);
};

*/

#endif /* PACKET_TIMESYNC_15_4_H_ */
