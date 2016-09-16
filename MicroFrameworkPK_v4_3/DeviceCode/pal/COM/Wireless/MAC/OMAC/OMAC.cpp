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
const bool NEED_OMAC_CALLBACK_CONTINUATION = false;


#if defined(OMAC_DEBUG_PRINTF) || defined(DEBUG_OMAC_PREPAREMESSAGEBUFFER)
#define DEBUG_OMAC_PMB_PRINTF(...) hal_printf( __VA_ARGS__ )
#else
#define DEBUG_OMAC_PMB_PRINTF(...) (void)0
#endif

OMACType g_OMAC;

UINT8 OMACType::payloadTypeArrayIndex = 0;
HAL_CONTINUATION OMACType::OMAC_callback_continuation;

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
	if(g_OMAC.m_omac_scheduler.m_state == I_DATA_RCV_PENDING && Interrupt==StartOfReception){
		g_OMAC.m_omac_scheduler.m_DataReceptionHandler.HandleRadioInterrupt();
	}
	else if(g_OMAC.m_omac_scheduler.m_state == I_DISCO_PENDING && Interrupt==StartOfReception){
		g_OMAC.m_omac_scheduler.m_DiscoveryHandler.HandleRadioInterrupt();
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

	//Set flag when send is complete
	g_OMAC.isSendDone = true;

	//Demutiplex packets received based on type
	switch(rcv_msg->GetHeader()->payloadType){
		case MFM_OMAC_DISCOVERY:
			g_OMAC.m_omac_scheduler.m_DiscoveryHandler.BeaconAckHandler(rcv_msg,rcv_msg->GetPayloadSize(),status);
			break;
		case MFM_OMAC_ROUTING:
			break;
		case MFM_OMAC_NEIGHBORHOOD:
			break;
		case MFM_TIMESYNC:
			CPU_GPIO_SetPinState(SEND_ACK_PIN, TRUE);
			CPU_GPIO_SetPinState(SEND_ACK_PIN, FALSE);
			break;
		case MFM_OMAC_DATA_ACK:
			if(CPU_Radio_GetRadioAckType() == SOFTWARE_ACK){
				if(g_OMAC.m_omac_scheduler.m_state == I_DATA_RCV_PENDING ){
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
			if(g_OMAC.m_omac_scheduler.m_state == I_DATA_SEND_PENDING){
				g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.SendACKHandler(rcv_msg, radioAckStatus);
			}
			CPU_GPIO_SetPinState(SEND_ACK_PIN, FALSE);
			break;
	};

#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(DATA_TX_ACK_PIN, FALSE);
#endif
}


//DeviceStatus SetConfig(MACConfig *config);
//BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int Size);

/*
 *
 */
DeviceStatus OMACType::SetConfig(MACConfig *config){
	/*MyConfig.FCF = config->FCF;
	MyConfig.DestPAN = config->DestPAN;
	MyConfig.Network = config->Network;*/
	MyConfig.CCA = config->CCA;
	MyConfig.NumberOfRetries = config->NumberOfRetries;
	MyConfig.CCASenseTime = config->CCASenseTime;
	MyConfig.BufferSize = config->BufferSize;
	//MyConfig.RadioType = config->RadioType;
	MyConfig.NeighborLivenessDelay = config->NeighborLivenessDelay;
	return DS_Success;
}

DeviceStatus OMACType::SetOMACParametersBasedOnRadioName(UINT8 radioName){
	/*OMAC_TIME_ERROR = 3*MILLISECINMICSEC;
	DELAY_FROM_OMAC_TX_TO_RADIO_DRIVER_TX = 300;
	DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX = 284;
	DELAY_DUE_TO_CCA_MICRO = 260;
	RETRY_FUDGE_FACTOR = 0.3*MILLISECINMICSEC;
	RANDOM_BACKOFF_COUNT_MAX = 4;
	RANDOM_BACKOFF_COUNT_MIN = 1;*/

	switch(radioName){
		case RF231RADIOLR:
		case RF231RADIO:
		{
			UINT16 TX_BUFFER = 1 * MILLISECINMICSEC;
			UINT16 RX_BUFFER = 1 * MILLISECINMICSEC;
			UINT16 DISCO_BUFFER = 0.5 * MILLISECINMICSEC;
			TX_TIME_PER_BIT_IN_MICROSEC = 3.9;	/*** (1/256kbps) ***/
			RETRY_RANDOM_BACKOFF_DELAY_MICRO = (RANDOM_BACKOFF_COUNT_MAX*DELAY_DUE_TO_CCA_MICRO);
			MAX_PACKET_TX_DURATION_MICRO = (IEEE802_15_4_FRAME_LENGTH*BITS_PER_BYTE*TX_TIME_PER_BIT_IN_MICROSEC) + TX_BUFFER;	//5*MILLISECINMICSEC;
			ACK_RX_MAX_DURATION_MICRO = (sizeof(softwareACKHeader)*BITS_PER_BYTE*TX_TIME_PER_BIT_IN_MICROSEC) + RX_BUFFER;	//20*MILLISECINMICSEC;
			DISCO_PACKET_TX_TIME_MICRO = (sizeof(DiscoveryMsg_t)*BITS_PER_BYTE*TX_TIME_PER_BIT_IN_MICROSEC) + DISCO_BUFFER;	//1*MILLISECINMICSEC;
			DISCO_BEACON_TX_MAX_DURATION_MICRO = 1.2*MILLISECINMICSEC;
			DISCO_SLOT_PERIOD_MICRO = 8*MILLISECINMICSEC;
			HIGH_DISCO_PERIOD_IN_SLOTS = 9000;
			if(CPU_Radio_GetRadioAckType() == SOFTWARE_ACK){
				DELAY_IN_RECEIVING_HW_ACK = 0*MILLISECINMICSEC;
				DELAY_IN_RECEIVING_SW_ACK = 0.6*MILLISECINMICSEC;
				RETRANS_DELAY_DUE_TO_MISSING_HW_ACK = 0*MILLISECINMICSEC;
				RETRANS_DELAY_DUE_TO_MISSING_SW_ACK = 0.2*MILLISECINMICSEC;
				ACK_TX_MAX_DURATION_MICRO = 4*MILLISECINMICSEC;
			}
			else if(CPU_Radio_GetRadioAckType() == HARDWARE_ACK){
				DELAY_IN_RECEIVING_HW_ACK = 0.4*MILLISECINMICSEC;
				DELAY_IN_RECEIVING_SW_ACK = 0*MILLISECINMICSEC;
				RETRANS_DELAY_DUE_TO_MISSING_HW_ACK = 0*MILLISECINMICSEC;
				RETRANS_DELAY_DUE_TO_MISSING_SW_ACK = 0*MILLISECINMICSEC;
				ACK_TX_MAX_DURATION_MICRO = 0*MILLISECINMICSEC;
			}
			else{
				ASSERT_SP(0);
			}
			DELAY_IN_RECEIVING_ACK = DELAY_IN_RECEIVING_HW_ACK+DELAY_IN_RECEIVING_SW_ACK;
			RETRANS_DELAY_DUE_TO_MISSING_ACK = RETRANS_DELAY_DUE_TO_MISSING_HW_ACK+RETRANS_DELAY_DUE_TO_MISSING_SW_ACK;

			if(FAST_RECOVERY)
			LISTEN_PERIOD_FOR_RECEPTION_HANDLER = GUARDTIME_MICRO+GUARDTIME_MICRO+OMAC_TIME_ERROR\
													+DELAY_FROM_OMAC_TX_TO_RADIO_DRIVER_TX+DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX+DELAY_IN_RECEIVING_ACK\
														+RETRY_RANDOM_BACKOFF_DELAY_MICRO+RETRY_FUDGE_FACTOR\
															+RETRANS_DELAY_DUE_TO_MISSING_ACK;
			else if(FAST_RECOVERY2)
				LISTEN_PERIOD_FOR_RECEPTION_HANDLER  =   GUARDTIME_MICRO + GUARDTIME_MICRO + DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX; //This is the duration used in FastRecovery 2.0
			else
				LISTEN_PERIOD_FOR_RECEPTION_HANDLER  =   GUARDTIME_MICRO + GUARDTIME_MICRO + DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX; //This is the duration used in FastRecovery 2.0
			break;
		}
		case SI4468_SPI2:
		{
			UINT16 TX_BUFFER = 2 * MILLISECINMICSEC;
			UINT16 RX_BUFFER = 7 * MILLISECINMICSEC;
			UINT16 DISCO_BUFFER = 1 * MILLISECINMICSEC;
			TX_TIME_PER_BIT_IN_MICROSEC = 25;	/*** (1/40kbps) ***/
			RETRY_RANDOM_BACKOFF_DELAY_MICRO = (RANDOM_BACKOFF_COUNT_MAX*DELAY_DUE_TO_CCA_MICRO);
			MAX_PACKET_TX_DURATION_MICRO = (IEEE802_15_4_FRAME_LENGTH*BITS_PER_BYTE*TX_TIME_PER_BIT_IN_MICROSEC) + TX_BUFFER;	//27.6*MILLISECINMICSEC;
			ACK_RX_MAX_DURATION_MICRO = (sizeof(softwareACKHeader)*BITS_PER_BYTE*TX_TIME_PER_BIT_IN_MICROSEC) + RX_BUFFER;	//8*MILLISECINMICSEC;
			DISCO_PACKET_TX_TIME_MICRO = (sizeof(DiscoveryMsg_t)*BITS_PER_BYTE*TX_TIME_PER_BIT_IN_MICROSEC) + DISCO_BUFFER;	//10*MILLISECINMICSEC;
			DISCO_BEACON_TX_MAX_DURATION_MICRO = 10*MILLISECINMICSEC;
			DISCO_SLOT_PERIOD_MICRO = 8*MILLISECINMICSEC;
			HIGH_DISCO_PERIOD_IN_SLOTS = 9000;
			if(CPU_Radio_GetRadioAckType() == SOFTWARE_ACK){
				DELAY_IN_RECEIVING_HW_ACK = 0*MILLISECINMICSEC;
				DELAY_IN_RECEIVING_SW_ACK = 0*MILLISECINMICSEC;
				RETRANS_DELAY_DUE_TO_MISSING_HW_ACK = 0*MILLISECINMICSEC;
				RETRANS_DELAY_DUE_TO_MISSING_SW_ACK = 0*MILLISECINMICSEC;
				ACK_TX_MAX_DURATION_MICRO = 8*MILLISECINMICSEC;;
			}
			else if(CPU_Radio_GetRadioAckType() == HARDWARE_ACK){
				ASSERT_SP(0);
			}
			else{
				ASSERT_SP(0);
			}
			DELAY_IN_RECEIVING_ACK = 3.6*MILLISECINMICSEC;
			RETRANS_DELAY_DUE_TO_MISSING_ACK = 12.8*MILLISECINMICSEC;

			if(FAST_RECOVERY)
			LISTEN_PERIOD_FOR_RECEPTION_HANDLER = GUARDTIME_MICRO+GUARDTIME_MICRO+OMAC_TIME_ERROR\
													+DELAY_FROM_OMAC_TX_TO_RADIO_DRIVER_TX+DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX+DELAY_IN_RECEIVING_ACK\
														+RETRY_RANDOM_BACKOFF_DELAY_MICRO+RETRY_FUDGE_FACTOR\
															+RETRANS_DELAY_DUE_TO_MISSING_ACK;
			else if(FAST_RECOVERY2)
				LISTEN_PERIOD_FOR_RECEPTION_HANDLER  =   GUARDTIME_MICRO + GUARDTIME_MICRO + DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX + MAX_PACKET_TX_DURATION_MICRO; //This is the duration used in FastRecovery 2.0
			else
				LISTEN_PERIOD_FOR_RECEPTION_HANDLER  =   GUARDTIME_MICRO + GUARDTIME_MICRO + DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX + MAX_PACKET_TX_DURATION_MICRO; //This is the duration used in FastRecovery 2.0

			break;
		}
		default:
		{
			ASSERT_SP(0);
			break;
		}
	}
}

void PushToUpperLayerHelper(void* arg)
{
	g_OMAC.PushPacketsToUpperLayers(arg);
}

void OMACType::PushPacketsToUpperLayers(void* arg){
	/*Message_15_4_t* next_free_buffer;
	while(!g_receive_buffer.IsEmpty()){
		next_free_buffer = g_receive_buffer.GetOldest();
		(*m_rxAckHandler)(next_free_buffer, next_free_buffer->GetHeader()->length - sizeof(IEEE802_15_4_Header_t));
	}*/

	//No need for a while loop, as every time a data msg is received, this callback is enqueued
	//Message_15_4_t* lastFullBuffer = g_receive_buffer.GetLastFullBuffer();
	//(*m_rxAckHandler)(lastFullBuffer, lastFullBuffer->GetHeader()->payloadType);
	Message_15_4_t* lastFullBuffer = NULL;
	payloadTypeArrayIndex--;
	ASSERT_SP(payloadTypeArrayIndex >= 0);
	UINT8 payloadTypeUpperLayer = payloadTypeArray[payloadTypeArrayIndex];
	(*m_rxAckHandler)(lastFullBuffer, payloadTypeUpperLayer);
}

/*
 *
 */
DeviceStatus OMACType::Initialize(MACEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioID, MACConfig* config) {
	//DeviceStatus OMACType::Initialize(MACEventHandler* eventHandler, UINT8* macID, UINT8 routingAppID, MACConfig *config) {
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(OMAC_DATARXPIN, TRUE);
	CPU_GPIO_SetPinState( OMAC_DATARXPIN, FALSE );
	CPU_GPIO_EnableOutputPin(DATARX_TIMESTAMP_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(OMAC_TIMESYNCREQRXPIN, TRUE);
	CPU_GPIO_EnableOutputPin(SEND_ACK_PIN, TRUE);
	CPU_GPIO_EnableOutputPin(OMAC_RXPIN, FALSE);
	CPU_GPIO_EnableOutputPin(DATA_TX_ACK_PIN, FALSE);
	CPU_GPIO_SetPinState(DATA_TX_ACK_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(DATA_RX_INTERRUPT_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(DATATX_POSTEXEC, TRUE);
	CPU_GPIO_SetPinState( DATATX_POSTEXEC, FALSE );
	CPU_GPIO_EnableOutputPin(OMAC_CONTINUATION, TRUE);
	CPU_GPIO_SetPinState( OMAC_CONTINUATION, FALSE );
#endif

	DeviceStatus status;
	//Initialize yourself first (you being the MAC)
	if(this->Initialized){
#ifdef OMAC_DEBUG_PRINTF
		OMAC_HAL_PRINTF("OMACType Error: Already Initialized!! My address: %d\n", g_OMAC.GetMyAddress());
#endif
	}
	else {
		////MAC<Message_15_4_t, MACConfig>::Initialize(eventHandler, macName, routingAppID, radioID, config);
		if(routingAppID >= MAX_APPS) {
			return DS_Fail;
		}
		this->macName = macName;
		this->radioName = radioID;
		this->isSendDone = true;
		SetConfig(config);

		//Initialize upper layer call backs
		MAC<Message_15_4_t, MACConfig>::SetAppIdIndex(routingAppID);
		MAC<Message_15_4_t, MACConfig>::SetAppHandlers(eventHandler);
		MACEventHandler_t* appHandler = MAC<Message_15_4_t, MACConfig>::GetAppHandler(MAC<Message_15_4_t, MACConfig>::GetAppIdIndex());

		AppCount = 0; //number of upperlayers connected to you
		//MaxPayload is already defined in Message.h
		OMACType::SetMaxPayload( (UINT16)( IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t) ) );
		//OMACType::SetMaxPayload( (UINT16)( IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t)-sizeof(IEEE802_15_4_Metadata) ) );
		//OMACType::SetMaxPayload((UINT16)IEEE802_15_4_MAX_PAYLOAD);

		Radio_Event_Handler.RadioInterruptMask = (StartOfTransmission|EndOfTransmission|StartOfReception);
		Radio_Event_Handler.SetRadioInterruptHandler(OMACRadioInterruptHandler);
		Radio_Event_Handler.SetReceiveHandler(OMACReceiveHandler);
		Radio_Event_Handler.SetSendAckHandler(OMACSendAckHandler);

		OMAC_callback_continuation.InitializeCallback(PushToUpperLayerHelper, NULL);

		//g_send_buffer.Initialize();
		g_receive_buffer.Initialize();
		g_NeighborTable.ClearTable();

		g_NeighborTable.SetPreviousNumberOfNeighbors(0);
		if((status = CPU_Radio_Initialize(&Radio_Event_Handler, this->radioName, NumberRadios, macName)) != DS_Success) {
			SOFT_BREAKPOINT();
			return status;
		}

		if((status = CPU_Radio_TurnOnRx(this->radioName)) != DS_Success){
			return status;
		}

		SetOMACParametersBasedOnRadioName(this->radioName);

#ifdef OMAC_DEBUG_PRINTF
		OMAC_HAL_PRINTF("Initializing OMACType: My address: %d\n", g_OMAC.GetMyAddress());
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
	m_omac_scheduler.UnInitialize();
	ret &= (m_omac_RadioControl.Uninitialize() == DS_Success);
	ret &= (CPU_Radio_TurnOffRx(this->radioName) == DS_Success);
	ret &= CPU_Radio_UnInitialize(this->radioName);
	Initialized = FALSE;
	return ret;
}

/*
 *
 */
Message_15_4_t* OMACType::ReceiveHandler(Message_15_4_t* msg, int Size){
	if(!Initialized){
		return msg;
	}
	DeviceStatus ds;
	DiscoveryMsg_t* disco_msg = NULL;
	TimeSyncMsg* tsmg = NULL;
	DataMsg_t* data_msg = NULL;
	TimeSyncRequestMsg* tsreqmg = NULL;

	INT64 evTime;
	UINT64 rx_start_ticks = g_OMAC.m_Clock.GetCurrentTimeinTicks();
	UINT64 senderDelay = MAX_UINT64;
	UINT64 rx_time_stamp;
	UINT16 location_in_packet_payload = 0;

	MsgLinkQualityMetrics_t msgLinkQualityMetrics;
	RadioAddress_t myID = g_OMAC.GetMyAddress();

	if(Size == sizeof(softwareACKHeader)){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RXPIN, TRUE);
#endif
		swAckHeader = (softwareACKHeader*)msg;
		RadioAddress_t sourceID = swAckHeader->src;
		RadioAddress_t destID = swAckHeader->dest;
		UINT8 payloadType = swAckHeader->payloadType;
		if(destID == myID){
			if(CPU_Radio_GetRadioAckType() == SOFTWARE_ACK && payloadType == MFM_OMAC_DATA_ACK){
				g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.ReceiveDATAACK(sourceID);
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RXPIN, FALSE);
#endif
				return msg;
			}
		}
		return msg;
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RXPIN, FALSE);
#endif
	}


	UINT16 maxPayload = OMACType::GetMaxPayload();
	//if( Size > sizeof(IEEE802_15_4_Header_t) && (Size - sizeof(IEEE802_15_4_Header_t)-sizeof(IEEE802_15_4_Footer_t)-sizeof(IEEE802_15_4_Metadata) > maxPayload) ){
	if( Size > sizeof(IEEE802_15_4_Header_t) && (Size - sizeof(IEEE802_15_4_Header_t) > maxPayload) ){
		//OMAC_HAL_PRINTF("CSMA Receive Error: Packet is too big: %d \n", Size+sizeof(IEEE802_15_4_Header_t)+sizeof(IEEE802_15_4_Footer_t)+sizeof(IEEE802_15_4_Metadata));
#ifdef OMAC_DEBUG_PRINTF
		OMAC_HAL_PRINTF("CSMA Receive Error: Packet is too big: %d \n", Size+sizeof(IEEE802_15_4_Header_t));
#endif
		return msg;
	}
	if(Size < sizeof(IEEE802_15_4_Header_t)){
		return msg;
	}

	Size -= sizeof(IEEE802_15_4_Header_t);

	RadioAddress_t sourceID = msg->GetHeader()->src;
	RadioAddress_t destID = msg->GetHeader()->dest;
#ifdef	def_Neighbor2beFollowed
	if(sourceID == Neighbor2beFollowed) {
#endif
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RXPIN, TRUE);
#endif

		//g_OMAC.m_omac_scheduler.m_DataReceptionHandler.m_isreceiving = false;

		//if( destID == myID || destID == RADIO_BROADCAST_ADDRESS){


			//Any message might have timestamping attached to it. Check for it and process
			if(msg->GetHeader()->flags & TIMESTAMPED_FLAG){
				senderDelay = PacketTimeSync_15_4::SenderDelay(msg,Size) + g_OMAC.m_Clock.ConvertMicroSecstoTicks(TIME_RX_TIMESTAMP_OFFSET_MICRO);
				rx_time_stamp = g_OMAC.m_Clock.GetCurrentTimeinTicks() - (HAL_Time_CurrentTicks() - msg->GetMetaData()->GetReceiveTimeStamp());
			}

			//Get the primary packet
			switch(msg->GetHeader()->payloadType){
				case MFM_OMAC_DISCOVERY:
				{
					disco_msg = (DiscoveryMsg_t*) (msg->GetPayload());
					msgLinkQualityMetrics.RSSI = msg->GetMetaData()->GetRssi();
					msgLinkQualityMetrics.LinkQuality = msg->GetMetaData()->GetLqi();
					g_OMAC.m_omac_scheduler.m_DiscoveryHandler.Receive(sourceID, disco_msg, &msgLinkQualityMetrics);
					location_in_packet_payload += sizeof(DiscoveryMsg_t);
					break;
				}
//				case MFM_DATA:
//				{
//#ifdef OMAC_DEBUG_GPIO
//					CPU_GPIO_SetPinState(OMAC_DATARXPIN, TRUE);
//#endif
//					//TODO: Commenting out below code for SI4468 radio. Needs to be re-visited.
//					if(myID == destID && g_OMAC.m_omac_scheduler.m_state == (I_DATA_RCV_PENDING) && g_OMAC.m_omac_scheduler.m_execution_started){
//						g_OMAC.m_omac_scheduler.m_DataReceptionHandler.HandleEndofReception(sourceID);
//					}
//
//					data_msg = (DataMsg_t*) msg->GetPayload();
////							if(data_msg->msg_identifier != 16843009){
////								ASSERT_SP(0);
////							}
//					//location_in_packet_payload += data_msg->size;
//					location_in_packet_payload += data_msg->size + DataMsgOverhead;
////
////					if(false && myID == destID && g_OMAC.m_omac_scheduler.m_state == (I_DATA_RCV_PENDING) && g_OMAC.m_omac_scheduler.m_execution_started) {//Process the data packet only if the destination matches
////						g_OMAC.m_omac_scheduler.m_DataReceptionHandler.HandleEndofReception(sourceID);
////
////						Message_15_4_t* next_free_buffer = g_receive_buffer.GetNextFreeBuffer();
////
////						if(! (next_free_buffer))
////						{
////							g_receive_buffer.DropOldest(1);
////							next_free_buffer = g_receive_buffer.GetNextFreeBuffer();
////						}
////
////						ASSERT_SP(next_free_buffer);
////						memcpy(next_free_buffer->GetPayload(),data_msg->payload,data_msg->size);
////						memcpy(next_free_buffer->GetHeader(),msg->GetHeader(), sizeof(IEEE802_15_4_Header_t));
////						memcpy(next_free_buffer->GetFooter(),msg->GetFooter(), sizeof(IEEE802_15_4_Footer_t));
////						memcpy(next_free_buffer->GetMetaData(),msg->GetMetaData(), sizeof(IEEE802_15_4_Metadata_t));
////						next_free_buffer->GetHeader()->length = data_msg->size + sizeof(IEEE802_15_4_Header_t);
////						if(NEED_OMAC_CALLBACK_CONTINUATION){
////							payloadTypeArray[payloadTypeArrayIndex % payloadTypeArrayMaxValue] = msg->GetHeader()->payloadType;
////							payloadTypeArrayIndex++;
////							OMAC_callback_continuation.Enqueue();
////						}
////						else{
////							//TODO: BK: The following should be revisited. I think this should be handled as the default case.
////							//(*m_rxAckHandler)(next_free_buffer, msg->GetHeader()->payloadType);
////						}
////
////						//Another method of doing the same thing as above
////						/*Message_15_4_t tempMsg;
////						memcpy(tempMsg.GetPayload(), data_msg->payload, data_msg->size);
////						memcpy(tempMsg.GetHeader(), msg->GetHeader(), sizeof(IEEE802_15_4_Header_t));
////						memcpy(tempMsg.GetFooter, msg->GetFooter(), sizeof(IEEE802_15_4_Footer_t));
////						memcpy(tempMsg.GetMetaData, msg->GetMetaData(), sizeof(IEEE802_15_4_Metadata_t));
////						(*next_free_buffer) = &tempMsg;	//put the currently received message into the buffer (thereby its not free anymore)
////						(*m_rxAckHandler)(&tempMsg, data_msg->size);*/
////
////					}
//#ifdef OMAC_DEBUG_GPIO
//						CPU_GPIO_SetPinState(OMAC_DATARXPIN, FALSE);
//#endif
//					break;
//				}
				case MFM_OMAC_ROUTING: //Not processed
				{
					data_msg = (DataMsg_t*) msg->GetPayload();
//					if(data_msg->msg_identifier != 16843009){
//						//ASSERT_SP(0);
//					}
					location_in_packet_payload += data_msg->size + DataMsgOverhead;
#ifdef OMAC_DEBUG_PRINTF
					OMAC_HAL_PRINTF("OMACType::ReceiveHandler MFM_ROUTING\n");
#endif
					break;
				}
				case MFM_OMAC_NEIGHBORHOOD://Not processed
				{
#ifdef OMAC_DEBUG_PRINTF
					OMAC_HAL_PRINTF("OMACType::ReceiveHandler MFM_NEIGHBORHOOD\n");
#endif
					break;
				}
				case MFM_OMAC_TIMESYNCREQ:
				{
					//TODO: Commenting out below code for SI4468 radio. Needs to be re-visited.
					if(g_OMAC.m_omac_scheduler.m_state == (I_DATA_RCV_PENDING)){
						g_OMAC.m_omac_scheduler.m_DataReceptionHandler.HandleEndofReception(sourceID);
					}
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(OMAC_TIMESYNCREQRXPIN, TRUE);
#endif
					//ASSERT_SP(msg->GetHeader()->flags & TIMESTAMPED_FLAG);
#ifdef OMAC_DEBUG_PRINTF
						OMAC_HAL_PRINTF("OMACType::ReceiveHandler MFM_TIMESYNC\n");
#endif
					data_msg = (DataMsg_t*) msg->GetPayload();
					location_in_packet_payload += data_msg->size + DataMsgOverhead;
//					if(data_msg->msg_identifier != 16843009){
//						//ASSERT_SP(0);
//					}
					if(myID == destID) {
						tsreqmg =  (TimeSyncRequestMsg*) (data_msg->payload);
						ds = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ReceiveTSReq(sourceID, tsreqmg);
					}
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(OMAC_TIMESYNCREQRXPIN, FALSE);
#endif
					break;
				}
				case MFM_OMAC_DATA_BEACON_TYPE://Not processed
				{
#ifdef OMAC_DEBUG_PRINTF
					OMAC_HAL_PRINTF("OMACType::ReceiveHandler MFM_OMAC_DATA_BEACON_TYPE\n");
					OMAC_HAL_PRINTF("Got a data beacon packet\n");
#endif
					break;
				}
				case MFM_OMAC_DATA_ACK:
				{
					if(CPU_Radio_GetRadioAckType() == SOFTWARE_ACK){
						g_OMAC.m_omac_scheduler.m_DataTransmissionHandler.ReceiveDATAACK(sourceID);
						location_in_packet_payload += 1;
					}
					break;
				}
				case MFM_DATA:
				default:
				{
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(OMAC_DATARXPIN, TRUE);
#endif

					if(myID == destID && g_OMAC.m_omac_scheduler.m_state == (I_DATA_RCV_PENDING) && g_OMAC.m_omac_scheduler.m_execution_started){
						g_OMAC.m_omac_scheduler.m_DataReceptionHandler.HandleEndofReception(sourceID);
					}

					data_msg = (DataMsg_t*) msg->GetPayload();
					location_in_packet_payload += data_msg->size + DataMsgOverhead;
					//if(data_msg->msg_identifier != 16843009){
						//ASSERT_SP(0);
					//}

//					ASSERT_SP(data_msg->size <= MAX_DATA_PCKT_SIZE);



					static volatile INT32 pt = msg->GetHeader()->payloadType;

					if(myID == destID && g_OMAC.m_omac_scheduler.m_state == (I_DATA_RCV_PENDING) && g_OMAC.m_omac_scheduler.m_execution_started) {//Process the data packet only if the destination matches

						Message_15_4_t* next_free_buffer = g_receive_buffer.GetNextFreeBuffer();
						if(! (next_free_buffer))
						{
							g_receive_buffer.DropOldest(1);
							next_free_buffer = g_receive_buffer.GetNextFreeBuffer();
						}
						ASSERT_SP(next_free_buffer);

						memcpy(next_free_buffer->GetPayload(),data_msg->payload,data_msg->size);
						memcpy(next_free_buffer->GetHeader(),msg->GetHeader(), sizeof(IEEE802_15_4_Header_t));
						memcpy(next_free_buffer->GetFooter(),msg->GetFooter(), sizeof(IEEE802_15_4_Footer_t));
						memcpy(next_free_buffer->GetMetaData(),msg->GetMetaData(), sizeof(IEEE802_15_4_Metadata_t));
						next_free_buffer->GetHeader()->length = data_msg->size + sizeof(IEEE802_15_4_Header_t);

						if(NEED_OMAC_CALLBACK_CONTINUATION){
							payloadTypeArray[payloadTypeArrayIndex % payloadTypeArrayMaxValue] = msg->GetHeader()->payloadType;
							payloadTypeArrayIndex++;
							OMAC_callback_continuation.Enqueue();
						}
						else{
							MACReceiveFuncPtrType multi_m_rxAckHandler = NULL;
							if( IsValidNativeAppIdOffset(msg->GetHeader()->payloadType) )
							{
								multi_m_rxAckHandler = g_OMAC.GetNativeAppHandler(msg->GetHeader()->payloadType)->ReceiveHandler;
							}

							if(multi_m_rxAckHandler == NULL) {
								multi_m_rxAckHandler = m_rxAckHandler;
							}
							if(multi_m_rxAckHandler != NULL) {
								(*multi_m_rxAckHandler)(next_free_buffer, msg->GetHeader()->payloadType);
							}
						}

					}
#ifdef OMAC_DEBUG_GPIO
						CPU_GPIO_SetPinState(OMAC_DATARXPIN, FALSE);
#endif
					break;
				}
			};

			if(msg->GetHeader()->flags & MFM_TIMESYNC_FLAG) {
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(DATARX_TIMESTAMP_PIN, TRUE);
#endif
				//ASSERT_SP(msg->GetHeader()->GetFlags() & TIMESTAMPED_FLAG);
				tsmg = (TimeSyncMsg*) (msg->GetPayload() + location_in_packet_payload);
				ASSERT_SP(senderDelay != MAX_UINT64);
				ds = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.Receive(sourceID, tsmg, senderDelay, rx_time_stamp );
				location_in_packet_payload += sizeof(TimeSyncMsg);
#ifdef OMAC_DEBUG_GPIO
					CPU_GPIO_SetPinState(DATARX_TIMESTAMP_PIN, FALSE);
#endif
			}
			if(msg->GetHeader()->flags &  MFM_DISCOVERY_FLAG) {
				disco_msg = (DiscoveryMsg_t*) (msg->GetPayload() + location_in_packet_payload);
				msgLinkQualityMetrics.RSSI = msg->GetMetaData()->GetRssi();
				msgLinkQualityMetrics.LinkQuality = msg->GetMetaData()->GetLqi();
				g_OMAC.m_omac_scheduler.m_DiscoveryHandler.Receive(sourceID, disco_msg, &msgLinkQualityMetrics);
				location_in_packet_payload += sizeof(DiscoveryMsg_t);
			}

//			if( tsmg != NULL && disco_msg == NULL){
//				rx_start_ticks = g_OMAC.m_Clock.GetCurrentTimeinTicks();
//			}

			ds = g_NeighborTable.RecordLastHeardTime(sourceID,g_OMAC.m_Clock.GetCurrentTimeinTicks());
			if(ds != DS_Success){
#ifdef OMAC_DEBUG_PRINTF
				OMAC_HAL_PRINTF("OMACType::ReceiveHandler RecordLastHeardTime failure; address: %d; line: %d\n", sourceID, __LINE__);
#endif
			}

		//}

#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_RXPIN, FALSE);
#endif
#ifdef	def_Neighbor2beFollowed
	}
#endif
//	UINT64 rx_end_ticks = g_OMAC.m_Clock.GetCurrentTimeinTicks();
//	if(rx_end_ticks - rx_start_ticks > 8*2000000){ //Dummy if conditions to catch interrupted reception
//		return msg;
//	}



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
	if(!Initialized){
		return false;
	}
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
BOOL OMACType::SendTimeStamped(UINT16 address, UINT8 dataType, void* msg, int size, UINT32 eventTime){
	if(!Initialized){
		return false;
	}
	Message_15_4_t* msg_carrier = PrepareMessageBuffer(address, dataType, msg, size);
	if(msg_carrier == (Message_15_4_t*)(NULL)){
		return false;
	}
	IEEE802_15_4_Header_t* header = msg_carrier->GetHeader();
	IEEE802_15_4_Metadata* metadata = msg_carrier->GetMetaData();
	metadata->SetReceiveTimeStamp(eventTime);
	header->flags = (TIMESTAMPED_FLAG);
	return true;
}

/**
 * @return NULL pointer when no buffer available.
 */
Message_15_4_t* OMACType::PrepareMessageBuffer(UINT16 address, UINT8 dataType, void* msg, int size){
	static UINT8 seqNumber = 0;
	UINT8 finalSeqNumber = 0;
	Message_15_4_t* msg_carrier = (Message_15_4_t*)(NULL);

	if(size > OMACType::GetMaxPayload()){
		DEBUG_OMAC_PMB_PRINTF("OMACType Send Error: Packet is too big: %d ", size);
		return (Message_15_4_t*)(NULL);
	}

	Neighbor_t* neighborEntry = g_NeighborTable.GetNeighborPtr(address);
	if(neighborEntry == NULL) {
		DEBUG_OMAC_PMB_PRINTF("OMACType Send Error: Destination does not exist in neighbor table %d \n", address);
		return (Message_15_4_t*)(NULL);
	}
	else if(neighborEntry->neighborStatus == Dead) {
		DEBUG_OMAC_PMB_PRINTF("OMACType Send Error: Destination exists in neighbor table but its status is dead: %d \n", address);
		return (Message_15_4_t*)(NULL);
	}

	if(dataType == MFM_OMAC_TIMESYNCREQ){
		//msg_carrier = neighborEntry->tsr_send_buffer.GetNextFreeBuffer();
		//while(msg_carrier == (Message_15_4_t*)(NULL)){
		while(!neighborEntry->tsr_send_buffer.IsBufferEmpty()){
			neighborEntry->tsr_send_buffer.DropOldest(1);
		}

		msg_carrier = neighborEntry->tsr_send_buffer.GetNextFreeBuffer();

		if(msg_carrier == (Message_15_4_t*)(NULL)){
			DEBUG_OMAC_PMB_PRINTF("ERROR: OMACType::PrepareMessageBuffer no free tsr_send_buffer available. Addr: %d.\n", neighborEntry->MACAddress);
			return (Message_15_4_t*)(NULL);
		}
		else if(neighborEntry->tsr_send_buffer.IsBufferFull()){
			DEBUG_OMAC_PMB_PRINTF("WARN: OMACType::PrepareMessageBuffer neighborEntry->tsr_send_buffer is now full. Addr: %d.\n", neighborEntry->MACAddress);
		}
	}
	else{
		msg_carrier = neighborEntry->send_buffer.GetNextFreeBuffer();

		if(msg_carrier == (Message_15_4_t*)(NULL)){
			DEBUG_OMAC_PMB_PRINTF("ERROR: OMACType::PrepareMessageBuffer no free send_buffer available. Addr: %d.\n", neighborEntry->MACAddress);
			return (Message_15_4_t*)(NULL);
		}
		else if(neighborEntry->send_buffer.IsBufferFull()){
			DEBUG_OMAC_PMB_PRINTF("WARN: OMACType::PrepareMessageBuffer neighborEntry->send_buffer is now full. Addr: %d.\n", neighborEntry->MACAddress);
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

	metadata->ClearData();
	//header->SetLength(size + sizeof(IEEE802_15_4_Header_t) + sizeof(IEEE802_15_4_Footer_t)+sizeof(IEEE802_15_4_Metadata));
	//metadata->SetNetwork(MyConfig.Network);
	metadata->SetNetwork(0);
	header->macName = (macName);
	header->payloadType = (dataType);
	metadata->SetReceiveTimeStamp((UINT32)0);

	DataMsg_t* data_msg = (DataMsg_t*)msg_carrier->GetPayload();
	data_msg->size = size;
	//data_msg->msg_identifier = 16843009; // 0x01010101
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
UINT8 OMACType::UpdateNeighborTable(){
	UINT8 index;

	UINT8 numberOfDeadNeighbors = g_NeighborTable.UpdateNeighborTable(m_Clock.ConvertMicroSecstoTicks(MyConfig.NeighborLivenessDelay * 1000000), m_Clock.GetCurrentTimeinTicks());
	if(numberOfDeadNeighbors > 0)
	{
		for(UINT8 i =0; i < MAX_NBR; ++i){
			if(INVALID_NBR_ID != g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.samples[i].nbrID){
				if( g_NeighborTable.FindIndex( g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.samples[i].nbrID, &index) != DS_Success){ //If it does not exist in the neighbortable, delete from the time entires from globalTime Table
				 	 g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.Clean(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.samples[i].nbrID);
				}
				else if(g_NeighborTable.Neighbor[index].neighborStatus != Alive){ //If it exists in the neighbortable but marked as not alive, delete from the time entires from globalTime Table
					g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.Clean(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.samples[i].nbrID);
				}
			}
		}

		
	}

	if (g_NeighborTable.PreviousNumberOfNeighbors() != g_NeighborTable.NumberOfNeighbors())
	{
		NeighborChangeFuncPtrType appHandler = g_OMAC.GetAppHandler(CurrentActiveApp)->neighborHandler;

		// Check if neighbor change has been registered and the user is interested in this information
		if(appHandler != NULL)
		{
			// Make the neighbor changed callback signaling dead neighbors
			(*appHandler)((INT16) (g_NeighborTable.NumberOfNeighbors()));
		}
		g_NeighborTable.SetPreviousNumberOfNeighbors(g_NeighborTable.NumberOfNeighbors());
	}
	return numberOfDeadNeighbors;
	//g_NeighborTable.DegradeLinks();
}

/*BOOL OMACType::SetAddress(UINT16 address){
	MyAddress = address;
	return TRUE;
}
 */

UINT8 OMACType::GetSendBufferSize(){
	return g_NeighborTable.Neighbor[0].send_buffer.Size();
}

UINT8 OMACType::GetReceiveBufferSize(){
	return g_receive_buffer.Size();
}

UINT16 OMACType::GetSendPending(){
	UINT16 n_msg = 0;
	for(UINT8 i = 0; i < MAX_NEIGHBORS ; ++i){
		if(g_NeighborTable.Neighbor[i].neighborStatus != Dead){
			n_msg = n_msg + g_NeighborTable.Neighbor[i].send_buffer.GetNumberMessagesInBuffer();
			n_msg = n_msg + g_NeighborTable.Neighbor[i].tsr_send_buffer.GetNumberMessagesInBuffer();
		}
	}
	return n_msg;
}

UINT16 OMACType::GetReceivePending(){
	return g_receive_buffer.GetNumberMessagesInBuffer();
}



