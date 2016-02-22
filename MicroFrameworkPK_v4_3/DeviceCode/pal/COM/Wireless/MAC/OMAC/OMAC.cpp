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

OMACType g_OMAC;

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
	if(g_OMAC.m_omac_scheduler.InputState.IsState(I_DATA_RCV_PENDING) && Interrupt==StartOfReception){
		g_OMAC.m_omac_scheduler.m_DataReceptionHandler.HandleRadioInterrupt();
	}
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(DATA_RX_INTERRUPT_PIN, FALSE);
#endif
	return TRUE;
}

/*
 *
 */
void OMACSendAckHandler(void* msg, UINT16 Size, NetOpStatus status, UINT8 radioAckStatus){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(DATA_TX_ACK_PIN, TRUE);
#endif
	Message_15_4_t* rcv_msg = (Message_15_4_t*)msg;

	//Demutiplex packets received based on type
	switch(rcv_msg->GetHeader()->type){
		case MFM_DISCOVERY:
			g_OMAC.m_omac_scheduler.m_DiscoveryHandler.BeaconAckHandler(rcv_msg,rcv_msg->GetPayloadSize(),status);
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
			if(SOFTWARE_ACKS){
				if(g_OMAC.m_omac_scheduler.InputState.IsState(I_DATA_RCV_PENDING)){
					g_OMAC.m_omac_scheduler.m_DataReceptionHandler.SendACKHandler();
				}
			}
			break;
		case MFM_DATA:
			CPU_GPIO_SetPinState(SEND_ACK_PIN, TRUE);
			(*g_OMAC.m_txAckHandler)(msg, Size, status, radioAckStatus);
			//break;
		default:
			CPU_GPIO_SetPinState(SEND_ACK_PIN, TRUE);
			if(g_OMAC.m_omac_scheduler.InputState.IsState(I_DATA_SEND_PENDING)){
				g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.SendACKHandler(rcv_msg, radioAckStatus);
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
	CPU_GPIO_EnableOutputPin(OMAC_TIMESYNCREQRXPIN, TRUE);
	CPU_GPIO_EnableOutputPin(SEND_ACK_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(OMAC_RXPIN, FALSE);
	CPU_GPIO_EnableOutputPin(DATA_TX_ACK_PIN, FALSE);
	CPU_GPIO_SetPinState(DATA_TX_ACK_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(DATA_RX_INTERRUPT_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(DATATX_POSTEXEC, TRUE);
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
#endif

	DeviceStatus status;
	//Initialize yourself first (you being the MAC)
	if(this->Initialized){
		hal_printf("OMACType Error: Already Initialized!! My address: %d\n", g_OMAC.GetMyAddress());
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
		//MaxPayload is already defined in Message.h
		OMACType::SetMaxPayload( (UINT16)( IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t) ) );
		//OMACType::SetMaxPayload( (UINT16)( IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t)-sizeof(IEEE802_15_4_Metadata) ) );
		//OMACType::SetMaxPayload((UINT16)IEEE802_15_4_MAX_PAYLOAD);

		Radio_Event_Handler.RadioInterruptMask = (StartOfTransmission|EndOfTransmission|StartOfReception);
		Radio_Event_Handler.SetRadioInterruptHandler(OMACRadioInterruptHandler);
		Radio_Event_Handler.SetReceiveHandler(OMACReceiveHandler);
		Radio_Event_Handler.SetSendAckHandler(OMACSendAckHandler);

		//g_send_buffer.Initialize();
		g_receive_buffer.Initialize();
		m_NeighborTable.ClearTable();

		senderSequenceNumber = receiverSequenceNumber = 0;
		RadioAckPending = FALSE;
		Initialized = TRUE;
		m_recovery = 1;

		if((status = CPU_Radio_Initialize(&Radio_Event_Handler, this->radioName, NumberRadios, macName)) != DS_Success){
			return status;
		}

#ifdef OMAC_DEBUG_PRINTF
		hal_printf("Initializing OMACType: My address: %d\n", g_OMAC.GetMyAddress());
#endif
		SetMyAddress(CPU_Radio_GetAddress(radioName));
		SetMyID(CPU_Radio_GetAddress(radioName));

		m_omac_RadioControl.Initialize();
		m_omac_scheduler.Initialize(radioName, macName);
		Initialized = TRUE;
	}

	CurrentActiveApp = routingAppID;

	m_rxAckHandler = g_OMAC.GetAppHandler(g_OMAC.GetAppIdIndex())->GetReceiveHandler();
	m_txAckHandler = g_OMAC.GetAppHandler(g_OMAC.GetAppIdIndex())->GetSendAckHandler();

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(OMAC_DATARXPIN, FALSE);
	CPU_GPIO_SetPinState(OMAC_RXPIN, FALSE);
#endif


#ifdef def_Neighbor2beFollowed
#if defined(TWO_NODES_TX_RX)
	if(g_OMAC.GetMyAddress() == RXNODEID) {
		Neighbor2beFollowed = TXNODEID;
	}
	else {
		Neighbor2beFollowed = RXNODEID;
	}
#endif
#endif

#if defined(FAN_OUT)
	if(g_OMAC.GetMyAddress() == RXNODEID1 || g_OMAC.GetMyAddress() == RXNODEID2) {
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
	if(g_OMAC.GetMyAddress() == TXNODEID1 || g_OMAC.GetMyAddress() == TXNODEID2) {
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

void OMACType::PushPacketsToUpperLayers(){
	Message_15_4_t* next_free_buffer;
	while(!g_receive_buffer.IsEmpty()){
		next_free_buffer = g_receive_buffer.GetOldest();
		(*m_rxAckHandler)(next_free_buffer, next_free_buffer->GetHeader()->length - sizeof(IEEE802_15_4_Header_t));
	}
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
	UINT64 rx_start_ticks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
	UINT64 senderDelay;
	UINT64 rx_time_stamp;
	UINT16 location_in_packet_payload = 0;


	UINT16 maxPayload = OMACType::GetMaxPayload();
	//if( Size > sizeof(IEEE802_15_4_Header_t) && (Size - sizeof(IEEE802_15_4_Header_t)-sizeof(IEEE802_15_4_Footer_t)-sizeof(IEEE802_15_4_Metadata) > maxPayload) ){
	if( Size > sizeof(IEEE802_15_4_Header_t) && (Size - sizeof(IEEE802_15_4_Header_t) > maxPayload) ){
		//hal_printf("CSMA Receive Error: Packet is too big: %d \n", Size+sizeof(IEEE802_15_4_Header_t)+sizeof(IEEE802_15_4_Footer_t)+sizeof(IEEE802_15_4_Metadata));
		hal_printf("CSMA Receive Error: Packet is too big: %d \n", Size+sizeof(IEEE802_15_4_Header_t));
		return msg;
	}
	if(Size < sizeof(IEEE802_15_4_Header_t)){
		return msg;
	}

	Size -= sizeof(IEEE802_15_4_Header_t);

	RadioAddress_t sourceID = msg->GetHeader()->src;
	RadioAddress_t destID = msg->GetHeader()->dest;
	RadioAddress_t myID = g_OMAC.GetMyAddress();
#ifdef	def_Neighbor2beFollowed
	if(sourceID == Neighbor2beFollowed) {
#endif
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RXPIN, TRUE);
#endif

		//g_OMAC.m_omac_scheduler.m_DataReceptionHandler.m_isreceiving = false;

		if( destID == myID || destID == RADIO_BROADCAST_ADDRESS){
			DeviceStatus ds = g_OMAC.m_NeighborTable.RecordLastHeardTime(sourceID,g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks());
			if(ds != DS_Success){
				hal_printf("OMACType::ReceiveHandler RecordLastHeardTime failure; address: %d; line: %d\n", sourceID, __LINE__);
			}

			//Any message might have timestamping attached to it. Check for it and process
			if(msg->GetHeader()->flags & TIMESTAMPED_FLAG){
				senderDelay = PacketTimeSync_15_4::SenderDelay(msg,Size);
				rx_time_stamp = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks() - (HAL_Time_CurrentTicks() - msg->GetMetaData()->GetReceiveTimeStamp());
			}

			//Get the primary packet
			switch(msg->GetHeader()->type){
			case MFM_DISCOVERY:
				disco_msg = (DiscoveryMsg_t*) (msg->GetPayload());
				g_OMAC.m_omac_scheduler.m_DiscoveryHandler.Receive(sourceID, disco_msg);
				location_in_packet_payload += sizeof(DiscoveryMsg_t);
				break;
			case MFM_DATA:
				if(true || myID == destID) {
					if(g_OMAC.m_omac_scheduler.InputState.IsState(I_DATA_RCV_PENDING)){
						g_OMAC.m_omac_scheduler.m_DataReceptionHandler.HandleEndofReception(sourceID);
					}
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(OMAC_DATARXPIN, TRUE);
#endif
					data_msg = (DataMsg_t*) msg->GetPayload();
					if(data_msg->msg_identifier != 16843009){
						ASSERT_SP(0);
					}
					//location_in_packet_payload += data_msg->size;
					location_in_packet_payload += data_msg->size + DataMsgOverhead;

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
					(*m_rxAckHandler)(next_free_buffer, data_msg->size);


					//Another method of doing the same thing as above
					/*Message_15_4_t tempMsg;
					memcpy(tempMsg.GetPayload(), data_msg->payload, data_msg->size);
					memcpy(tempMsg.GetHeader(), msg->GetHeader(), sizeof(IEEE802_15_4_Header_t));
					memcpy(tempMsg.GetFooter, msg->GetFooter(), sizeof(IEEE802_15_4_Footer_t));
					memcpy(tempMsg.GetMetaData, msg->GetMetaData(), sizeof(IEEE802_15_4_Metadata_t));
					(*next_free_buffer) = &tempMsg;	//put the currently received message into the buffer (thereby its not free anymore)
					(*m_rxAckHandler)(&tempMsg, data_msg->size);*/
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(OMAC_DATARXPIN, FALSE);
#endif
				}
				break;
			case MFM_ROUTING:
				data_msg = (DataMsg_t*) msg->GetPayload();
				if(data_msg->msg_identifier != 16843009){
					//ASSERT_SP(0);
				}
				location_in_packet_payload += data_msg->size + DataMsgOverhead;

				hal_printf("OMACType::ReceiveHandler MFM_ROUTING\n");
				break;
			case MFM_NEIGHBORHOOD:
				hal_printf("OMACType::ReceiveHandler MFM_NEIGHBORHOOD\n");
				break;
			case MFM_TIMESYNCREQ:
				if(true || myID == destID) {
					if(g_OMAC.m_omac_scheduler.InputState.IsState(I_DATA_RCV_PENDING)){
						g_OMAC.m_omac_scheduler.m_DataReceptionHandler.HandleEndofReception(sourceID);
					}
				}
				CPU_GPIO_SetPinState(OMAC_TIMESYNCREQRXPIN, TRUE);
				//ASSERT_SP(msg->GetHeader()->flags & TIMESTAMPED_FLAG);
#ifdef OMAC_DEBUG_PRINTF
				hal_printf("OMACType::ReceiveHandler MFM_TIMESYNC\n");
#endif
				data_msg = (DataMsg_t*) msg->GetPayload();
				if(data_msg->msg_identifier != 16843009){
					//ASSERT_SP(0);
				}
				tsreqmg =  (TimeSyncRequestMsg*) (data_msg->payload);
				ds = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ReceiveTSReq(sourceID, tsreqmg);
				if(ds == DS_Success){
					location_in_packet_payload += data_msg->size + DataMsgOverhead;
				}
				else{
					location_in_packet_payload += data_msg->size + DataMsgOverhead;
				}
				CPU_GPIO_SetPinState(OMAC_TIMESYNCREQRXPIN, FALSE);
				break;
			case OMAC_DATA_BEACON_TYPE:
				hal_printf("OMACType::ReceiveHandler OMAC_DATA_BEACON_TYPE\n");
				hal_printf("Got a data beacon packet\n");
				break;
			case MFM_DATA_ACK:
				if(SOFTWARE_ACKS){
					g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.ReceiveDATAACK(sourceID);
					location_in_packet_payload += 1;
				}
				break;
			default:
				break;
			};

			if(msg->GetHeader()->flags &  MFM_TIMESYNC) {
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(DATARX_TIMESTAMP_PIN, TRUE);
#endif
				//ASSERT_SP(msg->GetHeader()->flags & TIMESTAMPED_FLAG);
				tsmg = (TimeSyncMsg*) (msg->GetPayload() + location_in_packet_payload);
				ds = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.Receive(sourceID, tsmg, senderDelay, rx_time_stamp );
				location_in_packet_payload += sizeof(TimeSyncMsg);
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(DATARX_TIMESTAMP_PIN, FALSE);
#endif
			}
			if(msg->GetHeader()->flags &  MFM_DISCOVERY) {
				disco_msg = (DiscoveryMsg_t*) (msg->GetPayload() + location_in_packet_payload);
				g_OMAC.m_omac_scheduler.m_DiscoveryHandler.Receive(sourceID, disco_msg );
				location_in_packet_payload += sizeof(DiscoveryMsg_t);
			}

			if( tsmg != NULL && disco_msg == NULL){
				rx_start_ticks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
			}
		}

#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RXPIN, FALSE);
#endif
#ifdef	def_Neighbor2beFollowed
	}
#endif
	UINT64 rx_end_ticks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
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
	IEEE802_15_4_Metadata* metadata = msg_carrier->GetMetaData();
	header->flags = (0);
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
	IEEE802_15_4_Metadata* metadata = msg_carrier->GetMetaData();
	header->flags = (TIMESTAMPED_FLAG);
	return true;
}

Message_15_4_t* OMACType::PrepareMessageBuffer(UINT16 address, UINT8 dataType, void* msg, int size){
	static UINT8 seqNumber = 0;
	UINT8 finalSeqNumber = 0;

	Message_15_4_t* msg_carrier = (Message_15_4_t*)(NULL);
	if(size > OMACType::GetMaxPayload()){
		hal_printf("OMACType Send Error: Packet is too big: %d ", size);
		return msg_carrier;
	}

	Neighbor_t* neighborEntry = g_OMAC.m_NeighborTable.GetNeighborPtr(address);
	if(neighborEntry == NULL) {
		hal_printf("OMACType Send Error: Destination does not exist in neighbor table %d ", address);
		return msg_carrier;
	}
	else if(neighborEntry->Status == Dead) {
		hal_printf("OMACType Send Error: Destination exists in neighbor table but its status is dead: %d ", address);
		return msg_carrier;
	}

	if(dataType == MFM_TIMESYNCREQ){
		msg_carrier = neighborEntry->tsr_send_buffer.GetNextFreeBuffer();
		while(msg_carrier == (Message_15_4_t*)(NULL)){
			neighborEntry->tsr_send_buffer.DropOldest(1);
			msg_carrier = neighborEntry->tsr_send_buffer.GetNextFreeBuffer();
		}
	}
	else{
		msg_carrier = neighborEntry->send_buffer.GetNextFreeBuffer();
		if(msg_carrier == (Message_15_4_t*)(NULL)){
			hal_printf("OMACType::Send neighborEntry->send_buffer full.\n");
			return msg_carrier;
		}
	}

	IEEE802_15_4_Header_t* header = msg_carrier->GetHeader();
	/****** Taking the word value of below bits gives FCF_WORD_VALUE *******/
	/*header->fcf->IEEE802_15_4_Header_FCF_BitValue.frameType = FRAME_TYPE_MAC;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.securityEnabled = 0;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.framePending = 0;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.ackRequired = 1;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.intraPAN = 1;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.reserved = 0;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.destAddrMode = 2;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.frameVersion = 1;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.srcAddrMode = 2;*/
	/**************************************************************/
	header->fcf.fcfWordValue = FCF_WORD_VALUE;
	finalSeqNumber = GetMyAddress() ^ 0xAA;
	finalSeqNumber += ((GetMyAddress() >> 8) ^ 0x55);
	finalSeqNumber += seqNumber;
	//Make sure that we differentiate between a DISCO and DATA sequence number
	if(finalSeqNumber == OMAC_DISCO_SEQ_NUMBER){
		finalSeqNumber += 1;
	}
	header->dsn = finalSeqNumber;
	//header->dsn = 97;
	//header->srcpan = SRC_PAN_ID;
	header->destpan = DEST_PAN_ID;
	/*if(GetRadioAddress() == 6846){
		header->dest = 0x0DB1;
	}
	else{
		header->dest = 0x1ABE;
	}*/
	/*//6846(0x1ABE) is the receiver
	if(g_OMAC.GetMyAddress() != 0x1ABE){
		header->dest = 0x1ABE;
	}
	else{
		header->dest = 0x1111;
	}*/
	header->dest = address;
	header->src = GetMyAddress();
	//header->retryAttempt = 0;
	seqNumber++;

	IEEE802_15_4_Metadata* metadata = msg_carrier->GetMetaData();
	//header->SetLength(size + sizeof(IEEE802_15_4_Header_t) + sizeof(IEEE802_15_4_Footer_t)+sizeof(IEEE802_15_4_Metadata));
	metadata->SetNetwork(MyConfig.Network);
	header->mac_id = (macName);
	header->type = (dataType);
	metadata->SetReceiveTimeStamp((UINT32)0);

	DataMsg_t* data_msg = (DataMsg_t*)msg_carrier->GetPayload();
	data_msg->size = size;
	data_msg->msg_identifier = 16843009; // 0x01010101
	UINT8* payload = data_msg->payload;
	UINT8* lmsg = (UINT8*) msg;
	for(UINT8 i = 0 ; i < size; i++){
		payload[i] = lmsg[i];
	}
	//msg_carrier->GetMetaData()->SetReceiveTimeStamp(0);

	//header->SetLength(size + sizeof(UINT32) + sizeof(UINT8) + sizeof(IEEE802_15_4_Header_t) + sizeof(IEEE802_15_4_Footer_t)+sizeof(IEEE802_15_4_Metadata));
	//header->length = (size + sizeof(UINT32) + sizeof(UINT8) + sizeof(IEEE802_15_4_Header_t));
	header->length = (size + DataMsgOverhead + sizeof(IEEE802_15_4_Header_t));

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
	m_NeighborTable.UpdateNeighborTable(MyConfig.NeighborLivenessDelay);
	//m_NeighborTable.DegradeLinks();
}

/*BOOL OMACType::SetAddress(UINT16 address){
	MyAddress = address;
	return TRUE;
}
 */

UINT8 OMACType::GetSendBufferSize(){
	return m_NeighborTable.Neighbor[0].send_buffer.Size();
}

UINT8 OMACType::GetReceiveBufferSize(){
	return g_receive_buffer.Size();
}

UINT16 OMACType::GetSendPending(){
	UINT16 n_msg = 0;
	for(UINT8 i = 0; i < MAX_NEIGHBORS ; ++i){
		if(m_NeighborTable.Neighbor[i].Status != Dead){
			n_msg = n_msg + m_NeighborTable.Neighbor[i].send_buffer.GetNumberMessagesInBuffer();
			n_msg = n_msg + m_NeighborTable.Neighbor[i].tsr_send_buffer.GetNumberMessagesInBuffer();
		}
	}
	return n_msg;
}

UINT16 OMACType::GetReceivePending(){
	return g_receive_buffer.GetNumberMessagesInBuffer();
}



