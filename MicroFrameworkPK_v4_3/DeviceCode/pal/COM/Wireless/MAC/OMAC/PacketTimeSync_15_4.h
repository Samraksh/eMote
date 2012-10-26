/*
 * TimeSync.h
 *
 *  Created on: Sep 10, 2012
 *      Author: Mukundan
 */

#ifndef PACKET_TIMESYNC_15_4_H_
#define PACKET_TIMESYNC_15_4_H_

#include <Tinyhal.h>
#include <Samraksh/Message.h>

#define TIMESTAMP_OFFSET (sizeof(IEEE802_15_4_Message_t)- sizeof(IEEE802_15_4_Metadata_t) - 4)
#define TIMESTAMP_ERROR 0x80000000
//A combined base class for timesync, analogous to TimeSyncMessageC in TinyOS

//Provides Packet level timesync: Similar to TinyOS TEP 133
//http://www.tinyos.net/tinyos-2.1.0/doc/html/tep133.html

class PacketTimeSync_15_4 : public Message_15_4_t{

public:
	//Receive API
	//void Receive(Message_T * msg, );
	//DeviceStatus Send(RadioAddress_t address, Message_15_4_t * msg, UINT16 size, UINT32 event_time);
	static void * GetPayload (Message_15_4_t * msg, UINT16 Size){
		return msg->GetPayload();
	}
	//TimeSyncPacket APIs
	UINT32 EventTime(Message_15_4_t *msg){
		UINT32 * senderEventTime = (UINT32 *)((UINT32)msg +  TIMESTAMP_OFFSET);
		return (*senderEventTime)+ (msg->GetMetaData()->GetReceiveTimeStamp());
	}
	bool IsValid(Message_15_4_t *msg){
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


//	DeviceStatus Send(RadioAddress_t address, Message_T * msg, UINT16 size, UINT64 event_time){

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
