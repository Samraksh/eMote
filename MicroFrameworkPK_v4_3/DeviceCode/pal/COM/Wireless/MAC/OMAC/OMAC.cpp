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
	return TRUE;
}

/*
 *
 */
void OMACSendAckHandler(void* msg, UINT16 Size, NetOpStatus status){
	Message_15_4_t* rcv_msg = (Message_15_4_t*)msg;

	//Demutiplex packets received based on type
	switch(rcv_msg->GetHeader()->GetType()){
		case MFM_DISCOVERY:
			g_omac_scheduler.m_DiscoveryHandler.BeaconAckHandler(rcv_msg,rcv_msg->GetPayloadSize(),status);
			break;
		case MFM_DATA:
		{
			(*g_txAckHandler)(msg, Size, status);
			break;
		}
		case MFM_ROUTING:
			break;
		case MFM_NEIGHBORHOOD:
			break;
		case MFM_TIMESYNC:
			break;
		default:
			break;
	};
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


	//DeviceStatus OMACType::Initialize(MacEventHandler* eventHandler, UINT8* macID, UINT8 routingAppID, MacConfig *config) {
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(OMAC_DATARXPIN, TRUE);
	CPU_GPIO_EnableOutputPin(OMAC_RXPIN, TRUE);
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

#ifdef DEBUG_OMAC
		hal_printf("Initializing OMACType: My address: %d\n", CPU_Radio_GetAddress(this->radioName));
#endif

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
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(OMAC_RXPIN, TRUE);
#endif

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

	//Any message might have timestamping attached to it. Check for it and process
	/*if(msg->GetHeader()->flags == TIMESTAMPED_FLAG && msg->GetHeader()->GetType()!=MFM_TIMESYNC){
		UINT8 tmsgSize = sizeof(TimeSyncMsg)+4;
		g_omac_scheduler.m_TimeSyncHandler.Receive(msg,msg->GetPayload()+Size-tmsgSize, tmsgSize);
		Size -= tmsgSize;
	}*/

	// Implement bag exchange if the packet type is data
	Message_15_4_t** next_free_buffer = g_receive_buffer.GetNextFreeBufferPtr();

	if(! (next_free_buffer))
	{
		g_receive_buffer.DropOldest(1);
		next_free_buffer = g_receive_buffer.GetNextFreeBufferPtr();
	}

	//Implement bag exchange, by actually switching the contents.
	Message_15_4_t* temp = *next_free_buffer;	//get the ptr to a msg inside the first free buffer.
	(*next_free_buffer) = msg;	//put the currently received message into the buffer (thereby its not free anymore)
								//finally the temp, which is a ptr to free message will be returned.


	//Demutiplex packets received based on type
	switch(msg->GetHeader()->GetType()){
		case MFM_DISCOVERY:
			g_omac_scheduler.m_DiscoveryHandler.Receive(msg, msg->GetPayload(), Size);
			break;
		case MFM_DATA:
			if(myID == destID) {
				CPU_GPIO_SetPinState(OMAC_DATARXPIN, TRUE);
#ifdef def_Neighbor2beFollowed
				if ( sourceID == Neighbor2beFollowed) {
					//hal_printf("OMACType::ReceiveHandler received a message from  Neighbor2beFollowed %u\n", sourceID);
				}
#endif
				(*g_rxAckHandler)(msg, Size);
				CPU_GPIO_SetPinState(OMAC_DATARXPIN, FALSE);
			}
			break;
		case MFM_ROUTING:
			hal_printf("OMACType::ReceiveHandler MFM_ROUTING\n");
			break;
		case MFM_NEIGHBORHOOD:
			hal_printf("OMACType::ReceiveHandler MFM_NEIGHBORHOOD\n");
			break;
		case MFM_TIMESYNC:
			hal_printf("OMACType::ReceiveHandler MFM_TIMESYNC\n");
			g_omac_scheduler.m_TimeSyncHandler.Receive(msg, msg->GetPayload(), Size);
			break;
		case OMAC_DATA_BEACON_TYPE:
			hal_printf("OMACType::ReceiveHandler OMAC_DATA_BEACON_TYPE\n");
			hal_printf("Got a data beacon packet\n");
			break;
		default:
			/*UINT8 tmsgSize = sizeof(TimeSyncMsg)+4;
			g_omac_scheduler.m_TimeSyncHandler.Receive(msg,msg->GetPayload()+Size-tmsgSize, tmsgSize);
			Size -= tmsgSize;*/
			break;
	};
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState(OMAC_RXPIN, FALSE);
#endif

	return temp;
}

/*
 *
 */
void RadioInterruptHandler(RadioInterrupt Interrupt, void* Param){

}

/*
 * Store packet in the send buffer and return; Scheduler will pick it up later and send it
 */
BOOL OMACType::Send(UINT16 address, UINT8 dataType, void* msg, int size, UINT32 eventTime){
	if(g_send_buffer.IsFull()){
		hal_printf("OMACType::Send g_send_buffer full. Clearing buffer\n");
		g_send_buffer.Erase();
		return FALSE;
	}

	Message_15_4_t* msg_carrier = g_send_buffer.GetNextFreeBuffer();
	if(size > OMACType::GetMaxPayload()){
		hal_printf("OMACType Send Error: Packet is too big: %d ", size);
		return FALSE;
	}
	IEEE802_15_4_Header_t* header = msg_carrier->GetHeader();
	header->length = size + sizeof(IEEE802_15_4_Header_t);
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

	//msg_carrier->GetMetaData()->SetReceiveTimeStamp(0);
	if(eventTime > 0){
		msg_carrier->GetMetaData()->SetReceiveTimeStamp(eventTime);
	}
	else{
		msg_carrier->GetMetaData()->SetReceiveTimeStamp(HAL_Time_CurrentTicks());
	}

	UINT8* lmsg = (UINT8*) msg;
	UINT8* payload = msg_carrier->GetPayload();

	for(UINT8 i = 0 ; i < size; i++){
		payload[i] = lmsg[i];
	}

	return true;
}


#if 0
/*
 * Store packet in the send buffer and return; Scheduler will pick it up later and send it
 */
////BOOL OMACType::SendTimeStamped(RadioAddress_t address, UINT8 dataType, Message_15_4_t* msg, int size, UINT32 eventTime)
BOOL OMACType::SendTimeStamped(UINT16 address, UINT8 dataType, void* msg, int size, UINT32 eventTime)
{
	////hal_printf("start OMACType::SendTimeStamped\n");
	if(g_send_buffer.IsFull()) {
		////hal_printf("OMACType::SendTimeStamped g_send_buffer full\n");
		return FALSE;
	}

	Message_15_4_t* msg_carrier = g_send_buffer.GetNextFreeBuffer();
	//Message_15_4_t* msg_carrier;
	if(size > OMACType::GetMaxPayload()){
		hal_printf("OMACType Send Error: Packet is too big: %d ", size);
		return FALSE;
	}
	IEEE802_15_4_Header_t* header = msg_carrier->GetHeader();
	header->length = size + sizeof(IEEE802_15_4_Header_t);
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

	msg_carrier->GetMetaData()->SetReceiveTimeStamp(eventTime);

	UINT8* lmsg = (UINT8*) msg;
	UINT8* payload = msg_carrier->GetPayload();

	for(UINT8 i = 0; i < size; i++){
		payload[i] = lmsg[i];
	}

	return true;
}
#endif

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



