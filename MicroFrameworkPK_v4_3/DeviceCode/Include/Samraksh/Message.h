/*
 * Copyright The Samraksh Company
 *
 * Name : 		 message.h
 *
 * Author : 	 Mukundan.Sridharan@samraksh.com, nived.sivadas@samraksh.com
 *
 * Description : defines the interface for message, the theory is that all protocols would be required to
 *				 implement this interface thereby ensuring a common template across multiple higher layers
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <tinyhal.h>
#include <Samraksh\Radio_decl.h>

// template for message header
template<class Header_T,UINT16 PayLoadSize_T, class Footer_T, class Metadata_T>
class Message
{
	Header_T header;
	UINT8 payload[PayLoadSize_T];
	Footer_T footer;
	Metadata_T metadata;

public:
	// Returns a pointer to the message
	Message* GetMessage()
	{
		return this;
	}

	// returns the size of the message packet
	UINT16 	 GetMessageSize()
	{
		return sizeof(Header_T) + sizeof(Footer_T) + sizeof(Metadata_T) + PayLoadSize_T;
	}


	// returns a pointer to the header
	Header_T* GetHeader()
	{
		return &header;
	}

	// Returns the size of the header
	// ensure that the mac header class has a size function
	UINT16 GetHeaderSize()
	{
		return sizeof(Header_T);
	}

	// returns a pointer to the payload
	UINT8* GetPayload()
	{
		return payload;
	}

	// returns the payload size
	UINT16 GetPayloadSize()
	{
		return  PayLoadSize_T;
	}

	// returns a pointer to the footer
	Footer_T* GetFooter()
	{
		return &footer;
	}

	// returns the size of the footer
	UINT16 GetFooterSize()
	{
		return sizeof(Footer_T);
	}

	// returns a pointer to the metadata
	Metadata_T* GetMetaData()
	{
		return &metadata;
	}

	// returns the metadata size
	UINT16 GetMetaDataSize()
	{
		return sizeof(Metadata_T);
	}

};

#define TIMESTAMPED_FLAG (1 << 0)

///Well known message types: MFM stands for MicroFramework Message
#define MFM_DATA (1 << 0)
#define MFM_TIMESYNC (1 << 1)
#define MFM_NEIGHBORHOOD (1 << 2)
#define MFM_ROUTING (1 << 3)
#define MFM_DISCOVERY (1 << 4)


//IEEE802.15.4 Message structure
#define IEEE802_15_4_FRAME_LENGTH 126

//All fields up to 'network' are 802.15.4 specification fields, network is a option field for 6LowPAN compatibility
//mac_id is Samraksh's Radio API to demultiplex radio packets

typedef struct IEEE802_15_4_Header {
  UINT8 length;
  UINT8 dsn;
  UINT16 fcf;
  UINT16 destpan;
  UINT16 dest;
  UINT16 src;
  UINT8 network;  // optionally included with 6LowPAN layer
  UINT8 mac_id;
  UINT8 type;
  UINT8 flags;


  UINT8 GetType(){ return type;}
  UINT8 GetLength(){return length; }
  //bool IsTimestamped() {return timestamped;}
  // Timestamp has been changed to flags
  UINT8 GetFlags() { return flags; }

  void SetFlags(UINT8 flags) { this->flags = flags; }

  void SetLength(UINT8 _length){length = _length; }
} IEEE802_15_4_Header_t;

typedef class IEEE802_15_4_Footer{}IEEE802_15_4_Footer_t;

typedef class IEEE802_15_4_Metadata{
	UINT8 Rssi;
	UINT8 Lqi;
	UINT32 ReceiveTimeStamp0;
	UINT32 ReceiveTimeStamp1;


  public:
	UINT8 GetRssi(){
		return Rssi;
	}
	UINT8 GetLqi(){
		return Lqi;
	}
	UINT64 GetReceiveTimeStamp(){
		UINT64 rtn;
		rtn=((UINT64)ReceiveTimeStamp1)<<32;
		rtn+=ReceiveTimeStamp0;
		return rtn;
	}
	void SetRssi(UINT8 Rssi)
	{
		this->Rssi = Rssi;
	}
	void SetLqi(UINT8 Lqi)
	{
		this->Lqi = Lqi;
	}
	void SetReceiveTimeStamp(INT64 timestamp){
		this->ReceiveTimeStamp0 = (UINT32)timestamp;
		this->ReceiveTimeStamp1= (UINT32)(timestamp>>32);
	}

	void SetRecieveTimeStamp(UINT32 timestamp){
		this->ReceiveTimeStamp0 = timestamp;
	}

}IEEE802_15_4_Metadata_t;
#define IEEE802_15_4_MAX_PAYLOAD (IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t))

typedef Message<IEEE802_15_4_Header_t,IEEE802_15_4_MAX_PAYLOAD,IEEE802_15_4_Footer_t,IEEE802_15_4_Metadata_t> IEEE802_15_4_Message_t;
#define Message_15_4_t IEEE802_15_4_Message_t

#endif /* MESSAGE_H_ */
