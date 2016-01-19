/*
 *  OMAC.cpp
 *
 *  Created on: Aug 30, 2012
 *  	Authors: Mukundan.Sridharan, Nived.Sivadas
 *  	Description :  OMAC Implementation, v 1.0
 *
 *  Modified on: Oct 30, 2015
 *  	Authors: Bora Karaoglu; Ananth Muralidharan
 *
 *  Copyright The Samraksh Company
 */

#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/Radio_decl.h>

#define DEBUG_OMAC 0

extern Buffer_15_4_t g_send_buffer;
extern Buffer_15_4_t g_receive_buffer;
extern NeighborTable g_NeighborTable;

OMACType g_OMAC;
RadioControl_t g_omac_RadioControl;
OMACScheduler g_omac_scheduler;

MacReceiveFuncPtrType g_rxAckHandler;
SendAckFuncPtrType g_txAckHandler;

/*
 *
 */
void* OMACReceiveHandler(void* msg, UINT16 size){
	return (void*) g_OMAC.ReceiveHandler((Message_15_4_t*) msg, size);
}

/*
 *
 */
BOOL OMACRadioInterruptHandler(RadioInterrupt Interrupt, void* Param){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(DATA_RX_INTERRUPT_PIN, TRUE);
#endif
	if(g_omac_scheduler.InputState.IsState(I_DATA_RCV_PENDING) && Interrupt==StartOfReception){
		g_omac_scheduler.m_DataReceptionHandler.HandleRadioInterrupt();
	}
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(DATA_RX_INTERRUPT_PIN, FALSE);
#endif
	return TRUE;
}

/*
 *
 */
void OMACSendAckHandler(void* msg, UINT16 Size, NetOpStatus status){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(DATA_TX_ACK_PIN, TRUE);
#endif
	Message_15_4_t* rcv_msg = (Message_15_4_t*)msg;

	//Demutiplex packets received based on type
	switch(rcv_msg->GetHeader()->GetType()){
	case MFM_DISCOVERY:
		g_omac_scheduler.m_DiscoveryHandler.BeaconAckHandler(rcv_msg,rcv_msg->GetPayloadSize(),status);
		break;
	case MFM_ROUTING:
		break;
	case MFM_NEIGHBORHOOD:
		break;
	case MFM_TIMESYNC:
		CPU_GPIO_SetPinState(SEND_ACK_PIN, TRUE);
		CPU_GPIO_SetPinState(SEND_ACK_PIN, FALSE);
		break;
	case MFM_DATA_ACK:
#ifdef SOFTWARE_ACKS_ENABLED
		if(g_omac_scheduler.InputState.IsState(I_DATA_RCV_PENDING)){
			g_omac_scheduler.m_DataReceptionHandler.SendACKHandler();
		}
#endif
		break;
	case MFM_DATA:
		CPU_GPIO_SetPinState(SEND_ACK_PIN, TRUE);
		(*g_txAckHandler)(msg, Size, status);
		//break;
	default:
		CPU_GPIO_SetPinState(SEND_ACK_PIN, TRUE);
		if(g_omac_scheduler.InputState.IsState(I_DATA_SEND_PENDING)){
			g_omac_scheduler.m_DataTransmissionHandler.SendACKHandler();
		}
		CPU_GPIO_SetPinState(SEND_ACK_PIN, FALSE);
		break;
	};

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(DATA_TX_ACK_PIN, FALSE);
#endif
}


//DeviceStatus SetConfig(MacConfig *config);
//BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int Size);

/*
 *
 */
DeviceStatus OMACType::SetConfig(MacConfig *config){
	MyConfig.BufferSize = config->BufferSize;
	MyConfig.CCA = config->BufferSize;
	MyConfig.CCASenseTime = config->CCASenseTime;
	MyConfig.RadioID = config->RadioID;
	MyConfig.FCF = config->FCF;
	MyConfig.DestPAN = config->DestPAN;
	MyConfig.Network = config->Network;
	MyConfig.NeighborLivenessDelay = config->NeighborLivenessDelay;
	return DS_Success;
}

/*
 *
 */
DeviceStatus OMACType::Initialize(MacEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioID, MacConfig* config) {
	//DeviceStatus OMACType::Initialize(MacEventHandler* eventHandler, UINT8* macID, UINT8 routingAppID, MacConfig *config) {
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(OMAC_DATARXPIN, TRUE);
	CPU_GPIO_EnableOutputPin(DATARX_TIMESTAMP_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(DATARX_DATA_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(SEND_ACK_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(OMAC_RXPIN, FALSE);
	CPU_GPIO_EnableOutputPin(DATA_TX_ACK_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(DATA_RX_INTERRUPT_PIN, FALSE);
#endif

	DeviceStatus status;
	//Initialize yourself first (you being the MAC)
	if(this->Initialized){
		hal_printf("OMACType Error: Already Initialized!! My address: %d\n", CPU_Radio_GetAddress(this->radioName));
	}
	else {
		////MAC<Message_15_4_t, MacConfig>::Initialize(eventHandler, macName, routingAppID, radioID, config);
		if(routingAppID >= MAX_APPS) {
			return DS_Fail;
		}
		this->macName = macName;
		this->radioName = radioID;
		SetConfig(config);

		//Initialize upper layer call backs
		MAC<Message_15_4_t, MacConfig>::SetAppIdIndex(routingAppID);
		MAC<Message_15_4_t, MacConfig>::SetAppHandlers(eventHandler);
		MacEventHandler_t* appHandler = MAC<Message_15_4_t, MacConfig>::GetAppHandler(MAC<Message_15_4_t, MacConfig>::GetAppIdIndex());

		AppCount = 0; //number of upperlayers connected to you
		OMACType::SetMaxPayload((UINT16)(IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t)));

		Radio_Event_Handler.RadioInterruptMask = (StartOfTransmission|EndOfTransmission|StartOfReception);
		Radio_Event_Handler.SetRadioInterruptHandler(OMACRadioInterruptHandler);
		Radio_Event_Handler.SetReceiveHandler(OMACReceiveHandler);
		Radio_Event_Handler.SetSendAckHandler(OMACSendAckHandler);

		g_send_buffer.Initialize();
		g_receive_buffer.Initialize();
		g_NeighborTable.ClearTable();

		RadioAckPending = FALSE;
		Initialized = TRUE;
		m_recovery = 1;

		if((status = CPU_Radio_Initialize(&Radio_Event_Handler, this->radioName, NumberRadios, macName)) != DS_Success){
			return status;
		}

#ifdef OMAC_DEBUG_PRINTF
		hal_printf("Initializing OMACType: My address: %d\n", CPU_Radio_GetAddress(this->radioName));
#endif
		SetMyID(CPU_Radio_GetAddress(radioName));

		g_omac_RadioControl.Initialize();
		g_omac_scheduler.Initialize(radioName, macName);
		Initialized = TRUE;
	}

	CurrentActiveApp = routingAppID;

	g_rxAckHandler = g_OMAC.GetAppHandler(g_OMAC.GetAppIdIndex())->GetReceiveHandler();
	g_txAckHandler = g_OMAC.GetAppHandler(g_OMAC.GetAppIdIndex())->GetSendAckHandler();

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(OMAC_DATARXPIN, FALSE);
	CPU_GPIO_SetPinState(OMAC_RXPIN, FALSE);
#endif


#ifdef def_Neighbor2beFollowed
#if defined(TWO_NODES_TX_RX)
	if(g_OMAC.GetAddress() == RXNODEID) {
		Neighbor2beFollowed = TXNODEID;
	}
	else {
		Neighbor2beFollowed = RXNODEID;
	}
#endif
#endif

#if defined(FAN_OUT)
	if(g_OMAC.GetAddress() == RXNODEID1 || g_OMAC.GetAddress() == RXNODEID2) {
#ifdef def_Neighbor2beFollowed
		Neighbor2beFollowed = TXNODEID;
#endif
	}
	else {
#ifdef def_Neighbor2beFollowed2
		Neighbor2beFollowed1 = RXNODEID1;
		Neighbor2beFollowed2 = RXNODEID2;
#endif
	}
#elif defined(FAN_IN)
	if(g_OMAC.GetAddress() == TXNODEID1 || g_OMAC.GetAddress() == TXNODEID2) {
#ifdef def_Neighbor2beFollowed
		Neighbor2beFollowed = RXNODEID;
#endif
	}
	else {
#ifdef def_Neighbor2beFollowed2
		Neighbor2beFollowed1 = TXNODEID1;
		Neighbor2beFollowed2 = TXNODEID2;
#endif
	}
#endif

	//Initialize radio layer
	return DS_Success;
}

/*
 *
 */
BOOL OMACType::UnInitialize(){
	BOOL ret = TRUE;
	Initialized = FALSE;
	ret &= CPU_Radio_UnInitialize(this->radioName);
	return ret;
}

/*
 *
 */
Message_15_4_t* OMACType::ReceiveHandler(Message_15_4_t* msg, int Size)
{
	DeviceStatus ds;
	DiscoveryMsg_t* disco_msg = NULL;
	TimeSyncMsg* tsmg = NULL;
	DataMsg_t* data_msg = NULL;
	TimeSyncRequestMsg* tsreqmg = NULL;

	INT64 evTime;
	UINT64 rx_start_ticks = g_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
	UINT16 location_in_packet_payload = 0;


	UINT16 maxPayload = OMACType::GetMaxPayload();
	if( Size > sizeof(IEEE802_15_4_Header_t) && (Size - sizeof(IEEE802_15_4_Header_t) > maxPayload) ){
		hal_printf("CSMA Receive Error: Packet is too big: %d \n", Size+sizeof(IEEE802_15_4_Header_t));
		return msg;
	}
	if(Size < sizeof(IEEE802_15_4_Header_t)){
		return msg;
	}

	Size -= sizeof(IEEE802_15_4_Header_t);

	RadioAddress_t sourceID = msg->GetHeader()->src;
	RadioAddress_t destID = msg->GetHeader()->dest;
	RadioAddress_t myID = g_OMAC.GetAddress();
#ifdef	def_Neighbor2beFollowed
	if(sourceID == Neighbor2beFollowed) {
#endif
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RXPIN, TRUE);
#endif

		//g_omac_scheduler.m_DataReceptionHandler.m_isreceiving = false;

		if( destID == myID || destID == RADIO_BROADCAST_ADDRESS){

			//Any message might have timestamping attached to it. Check for it and process
			if(msg->GetHeader()->GetFlags() & TIMESTAMPED_FLAG){
				evTime = PacketTimeSync_15_4::EventTime(msg,Size);
			}


			//Get the primary packet
			switch(msg->GetHeader()->GetType()){
			case MFM_DISCOVERY:
				disco_msg = (DiscoveryMsg_t*) (msg->GetPayload());
				g_omac_scheduler.m_DiscoveryHandler.Receive(sourceID, disco_msg);
				location_in_packet_payload += sizeof(DiscoveryMsg_t);
				break;
			case MFM_DATA:
				if(true || myID == destID) {
					if(g_omac_scheduler.InputState.IsState(I_DATA_RCV_PENDING)){
						g_omac_scheduler.m_DataReceptionHandler.HandleEndofReception(sourceID);
					}
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(OMAC_DATARXPIN, TRUE);
					CPU_GPIO_SetPinState(DATARX_TIMESTAMP_PIN, TRUE);
#endif
					data_msg = (DataMsg_t*) msg->GetPayload();
					if(data_msg->msg_identifier != 16843009){
						ASSERT_SP(0);
					}
					//location_in_packet_payload += data_msg->size;
					location_in_packet_payload += data_msg->size + DataMsgOverhead;
					//BK:I don't understand the following stuff. Hence commenting it out
					//BK: Why do we need to store the packet pointer in the g_receive_buffer? It seems like reception is a direct function call
					//AnanthAtSamraksh: As per current design of the Samraksh_eMote_Net dll, g_receive_buffer is the glue between native and managed code.
					//					Received msg is stored in g_receive_buffer and is subsequently accessed by managed code using function GetNextPacket in MAC.cs
					//					GetNextPacket (MAC.cs) passes an internal buffer to MACBase::GetNextPacket and further down to Mac_GetNextPacket (Mac_Functions.cpp)
					//					which is where msgs are read from g_receive_buffer and stuffed into buffer mentioned above. This buffer is then converted into
					//					Message type and finally passed onto the user.
					//					This can be simplified by changing the signature of the user function to receive a msg along with a packet count. But this involves
					//					lot of changes which affects CSMA as well. Hence, going with the approach taken by CSMA in OMAC as well.

					// Implement bag exchange if the packet type is data
					//Message_15_4_t** next_free_buffer = g_receive_buffer.GetNextFreeBufferPtr();

					Message_15_4_t* next_free_buffer = g_receive_buffer.GetNextFreeBuffer();

					if(! (next_free_buffer))
					{
						g_receive_buffer.DropOldest(1);
						next_free_buffer = g_receive_buffer.GetNextFreeBuffer();
					}

					memcpy(next_free_buffer->GetPayload(),data_msg->payload,data_msg->size);
					memcpy(next_free_buffer->GetHeader(),msg->GetHeader(), sizeof(IEEE802_15_4_Header_t));
					memcpy(next_free_buffer->GetFooter(),msg->GetFooter(), sizeof(IEEE802_15_4_Footer_t));
					memcpy(next_free_buffer->GetMetaData(),msg->GetMetaData(), sizeof(IEEE802_15_4_Metadata_t));
					next_free_buffer->GetHeader()->length = data_msg->size + sizeof(IEEE802_15_4_Header_t);
					(*g_rxAckHandler)(next_free_buffer, data_msg->size);


					//Another method of doing the same thing as above
					/*Message_15_4_t tempMsg;
				memcpy(tempMsg.GetPayload(), data_msg->payload, data_msg->size);
				memcpy(tempMsg.GetHeader(), msg->GetHeader(), sizeof(IEEE802_15_4_Header_t));
				memcpy(tempMsg.GetFooter, msg->GetFooter(), sizeof(IEEE802_15_4_Footer_t));
				memcpy(tempMsg.GetMetaData, msg->GetMetaData(), sizeof(IEEE802_15_4_Metadata_t));
				(*next_free_buffer) = &tempMsg;	//put the currently received message into the buffer (thereby its not free anymore)
				(*g_rxAckHandler)(&tempMsg, data_msg->size);*/
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(OMAC_DATARXPIN, FALSE);
					CPU_GPIO_SetPinState(DATARX_TIMESTAMP_PIN, FALSE);
#endif
				}
				break;
			case MFM_ROUTING:
				data_msg = (DataMsg_t*) msg->GetPayload();
				if(data_msg->msg_identifier != 16843009){
					ASSERT_SP(0);
				}
				location_in_packet_payload += data_msg->size + DataMsgOverhead;

				hal_printf("OMACType::ReceiveHandler MFM_ROUTING\n");
				break;
			case MFM_NEIGHBORHOOD:
				hal_printf("OMACType::ReceiveHandler MFM_NEIGHBORHOOD\n");
				break;
			case MFM_TIMESYNCREQ:
				if(true || myID == destID) {
					if(g_omac_scheduler.InputState.IsState(I_DATA_RCV_PENDING)){
						g_omac_scheduler.m_DataReceptionHandler.HandleEndofReception(sourceID);
					}
				}
				CPU_GPIO_SetPinState(DATARX_DATA_PIN, TRUE);
				ASSERT_SP(msg->GetHeader()->GetFlags() & TIMESTAMPED_FLAG);
#ifdef OMAC_DEBUG_PRINTF
				hal_printf("OMACType::ReceiveHandler MFM_TIMESYNC\n");
#endif
				data_msg = (DataMsg_t*) msg->GetPayload();
				if(data_msg->msg_identifier != 16843009){
					ASSERT_SP(0);
				}
				tsreqmg =  (TimeSyncRequestMsg*) (data_msg->payload);
				ds = g_omac_scheduler.m_TimeSyncHandler.ReceiveTSReq(sourceID, tsreqmg);
				if(ds == DS_Success){
					location_in_packet_payload += data_msg->size + DataMsgOverhead;
				}
				else{
					location_in_packet_payload += data_msg->size + DataMsgOverhead;
				}
				CPU_GPIO_SetPinState(DATARX_DATA_PIN, FALSE);
				break;
			case OMAC_DATA_BEACON_TYPE:
				hal_printf("OMACType::ReceiveHandler OMAC_DATA_BEACON_TYPE\n");
				hal_printf("Got a data beacon packet\n");
				break;
#ifdef SOFTWARE_ACKS_ENABLED
			case MFM_DATA_ACK:
				g_omac_scheduler.m_DataTransmissionHandler.ReceiveDATAACK(sourceID);
				location_in_packet_payload += 1;
				break;
#endif
			default:
				break;
			};

			if(msg->GetHeader()->GetFlags() &  MFM_TIMESYNC) {
				ASSERT_SP(msg->GetHeader()->GetFlags() & TIMESTAMPED_FLAG);
				tsmg = (TimeSyncMsg*) (msg->GetPayload() + location_in_packet_payload);
				ds = g_omac_scheduler.m_TimeSyncHandler.Receive(sourceID, tsmg, evTime );
				location_in_packet_payload += sizeof(TimeSyncMsg);
			}
			if(msg->GetHeader()->GetFlags() &  MFM_DISCOVERY) {
				disco_msg = (DiscoveryMsg_t*) (msg->GetPayload() + location_in_packet_payload);
				g_omac_scheduler.m_DiscoveryHandler.Receive(sourceID, disco_msg );
				location_in_packet_payload += sizeof(DiscoveryMsg_t);
			}

			if( tsmg != NULL && disco_msg == NULL){
				rx_start_ticks = g_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
			}
		}

#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RXPIN, FALSE);
#endif
#ifdef	def_Neighbor2beFollowed
	}
#endif
	UINT64 rx_end_ticks = g_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
	if(rx_end_ticks - rx_start_ticks > 8*2000000){ //Dummy if conditions to catch interrupted reception
		return msg;
	}



	return msg;
}

/*
 *
 */
void RadioInterruptHandler(RadioInterrupt Interrupt, void* Param){

}


/*
 * Store packet in the send buffer and return; Scheduler will pick it up later and send it
 */
BOOL OMACType::Send(UINT16 address, UINT8 dataType, void* msg, int size){
	Message_15_4_t* msg_carrier = PrepareMessageBuffer(address, dataType, msg, size);
	if(msg_carrier == (Message_15_4_t*)(NULL)){
		return false;
	}
	IEEE802_15_4_Header_t* header = msg_carrier->GetHeader();
	header->SetFlags(0);
	return true;
}

/*
 * Store packet in the send buffer and return; Scheduler will pick it up later and send it
 */
////BOOL OMACType::SendTimeStamped(RadioAddress_t address, UINT8 dataType, Message_15_4_t* msg, int size, UINT32 eventTime)
BOOL OMACType::SendTimeStamped(UINT16 address, UINT8 dataType, void* msg, int size, UINT32 eventTime)
{
	Message_15_4_t* msg_carrier = PrepareMessageBuffer(address, dataType, msg, size);
	if(msg_carrier == (Message_15_4_t*)(NULL)){
		return false;
	}
	IEEE802_15_4_Header_t* header = msg_carrier->GetHeader();
	msg_carrier->GetMetaData()->SetReceiveTimeStamp(eventTime);
	header->SetFlags(TIMESTAMPED_FLAG);
	return true;
}

Message_15_4_t* OMACType::PrepareMessageBuffer(UINT16 address, UINT8 dataType, void* msg, int size){
	Message_15_4_t* msg_carrier = (Message_15_4_t*)(NULL);
	if(size > OMACType::GetMaxPayload()){
		hal_printf("OMACType Send Error: Packet is too big: %d ", size);
		return msg_carrier;
	}
	msg_carrier = g_send_buffer.GetNextFreeBuffer();
	if(msg_carrier == (Message_15_4_t*)(NULL)){
		hal_printf("OMACType::Send g_send_buffer full.\n");
		return msg_carrier;
	}

	IEEE802_15_4_Header_t* header = msg_carrier->GetHeader();
	//header->length = size + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest = address;
	header->src = CPU_Radio_GetAddress(this->radioName);
	header->network = MyConfig.Network;
	header->mac_id = macName;
	header->type = dataType;

	DataMsg_t* data_msg = (DataMsg_t*)msg_carrier->GetPayload();
	data_msg->size = size;
	data_msg->msg_identifier = 16843009; // 0x01010101
	UINT8* payload = data_msg->payload;
	UINT8* lmsg = (UINT8*) msg;
	for(UINT8 i = 0 ; i < size; i++){
		payload[i] = lmsg[i];
	}
	msg_carrier->GetMetaData()->SetReceiveTimeStamp(0);

	header->length = size + sizeof(UINT32) + sizeof(UINT8) + sizeof(IEEE802_15_4_Header_t);

	return msg_carrier;
}


/*
 *
 */
Message_15_4_t* OMACType::FindFirstSyncedNbrMessage(){
	return NULL;
}

/*
 *
 */
void OMACType::UpdateNeighborTable(){
	g_NeighborTable.UpdateNeighborTable(MyConfig.NeighborLivenessDelay);
	//g_NeighborTable.DegradeLinks();
}

/*BOOL OMACType::SetAddress(UINT16 address){
	MyAddress = address;
	return TRUE;
}
 */

UINT8 OMACType::GetBufferSize(){
	return g_send_buffer.Size();
}

UINT16 OMACType::GetSendPending(){
	return g_send_buffer.GetNumberMessagesInBuffer();
}

UINT16 OMACType::GetReceivePending(){
	return g_send_buffer.GetNumberMessagesInBuffer();
}



