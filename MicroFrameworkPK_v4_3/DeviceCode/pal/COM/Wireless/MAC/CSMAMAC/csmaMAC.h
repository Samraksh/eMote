/*
 * csmaMAC.h
 *
 *  Created on: Sep 27, 2012
 *      Author: Nived
 */

#ifndef CSMAMAC_H_
#define CSMAMAC_H_

/*********************************  Included header files **************************************/

#include <tinyhal.h>
#include <Samraksh\Message.h>
#include <Samraksh\Neighbors.h>
#include <Samraksh\MAC.h>
#include <Samraksh\Buffer.h>
#include <Samraksh\Radio_decl.h>

//typedef  struct Buffer<Message_15_4_t> Buffer_15_4_t;

typedef Buffer_15_4<8> Buffer_15_4_t;

/***********************************************************************************************/

/*********************************  Macro Definitions ******************************************/

#define CSMA_HEADER_SIZE 10
#define CSMA_FOOTER_SIZE 4
#define CSMA_METADATA_SIZE 4
#define CSMA_PAYLOAD_SIZE 20

/***********************************************************************************************/

/********************************* Message format implementations ******************************/
/*struct CSMAConfig{
	UINT16 FCF;
	UINT16 DestPAN;
	UINT8 Network;
};
*/

// Definition for the csma header
class csmaHeader
{
	UINT8 header[CSMA_HEADER_SIZE];

public:
	UINT16 size()
	{
		return CSMA_HEADER_SIZE;
	}
};

// class definition for the csma footer
class csmaFooter
{
	UINT8 footer[CSMA_FOOTER_SIZE];

public:
	UINT16 size()
	{
		return CSMA_FOOTER_SIZE;
	}
};

// class definition for metadata
class csmaMetadata
{
	UINT8 metadata[CSMA_METADATA_SIZE];

public:
	UINT16 size()
	{
		return CSMA_METADATA_SIZE;
	}
};

#if 0
// class definition for payload
class csmaPayload
{
	UINT8 payload[CSMA_PAYLOAD_SIZE];

public:
	UINT16 size()
	{
		return CSMA_PAYLOAD_SIZE;
	}

};
#endif
/*******************************************************************************************************/

/****************************** typedefs ***************************************************************/

typedef Message<csmaHeader, CSMA_PAYLOAD_SIZE ,  csmaFooter, csmaMetadata> MessageCsmaType;

/*******************************************************************************************************/

// core mac class definitions extends the MAC class and implements its virtual functions
// MAC does not have a template because of function pointer issues as they were primarily for the return type of handlers

class csmaMAC: public MAC<Message_15_4_t, MacConfig>
{
	//Buffer variables
	//BufferList<Buffer_15_4_t, Message_15_4_t, 16> SendBuffer;
	//BufferList<Buffer_15_4_t, Message_15_4_t, 16> ReceiveBuffer;
	NeighborTable m_NeighborTable;

	Buffer_15_4_t m_send_buffer;
	Buffer_15_4_t m_receive_buffer;
	UINT8 CurrentActiveApp;
public:

	DeviceStatus Initialize(MacEventHandler* eventHandler, UINT8* macIDs, UINT8 routingAppID, MacConfig *config);
	DeviceStatus SetConfig(MacConfig *config);
	BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int Size);
	Message_15_4_t* ReceiveHandler(Message_15_4_t* msg, int Size);
	BOOL RadioInterruptHandler(RadioInterrupt Interrupt, void* Param);
	void SendAckHandler(void* msg, int Size, NetOpStatus status);
	BOOL UnInitialize(void);
	BOOL HandleBroadcastMessage(Message_15_4_t * msg);
	BOOL HandleUnicastMessage(Message_15_4_t * msg);

};

csmaMAC gcsmaMacObject;


#endif /* CSMAMAC_H_ */
