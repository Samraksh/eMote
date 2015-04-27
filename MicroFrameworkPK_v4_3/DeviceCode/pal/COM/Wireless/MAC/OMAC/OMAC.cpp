/*
 * Copyright The Samraksh Company
 *
 * Author: Mukundan.Sridharan, Nived.Sivadas
 *
 * Description :  OMACTypeBora Implementation, v 1.0
 *
 *  Created on: Aug 30, 2012
 */

#include "OMAC.h"
//#include "RadioControl.h"
#include <Samraksh/Radio_decl.h>

#define DEBUG_OMAC 0
#define OMACDEBUGPIN (GPIO_PIN)1

extern Buffer_15_4_t g_send_buffer;
extern Buffer_15_4_t g_receive_buffer;
extern NeighborTable g_NeighborTable;

OMACTypeBora g_OMAC;
RadioControl_t g_omac_RadioControl;
OMACSchedulerBora g_omac_scheduler;

void* OMACReceiveHandler(void* msg, UINT16 size){
#ifdef DEBUG_OMAC
	CPU_GPIO_SetPinState(OMACDEBUGPIN, TRUE);
	CPU_GPIO_SetPinState(OMACDEBUGPIN, FALSE);
#endif

	return (void*) g_OMAC.ReceiveHandler((Message_15_4_t*) msg, size);
}

BOOL OMACRadioInterruptHandler(RadioInterrupt Interrupt, void* Param){
	return TRUE;
}

void OMACSendAckHandler(void *msg, UINT16 Size, NetOpStatus status){
	Message_15_4_t *rcv_msg = (Message_15_4_t *)msg;

		//Demutiplex packets received based on type
		switch(rcv_msg->GetHeader()->GetType()){
			case MFM_DISCOVERY:
				g_omac_scheduler.m_DiscoveryHandler.BeaconAckHandler(rcv_msg,rcv_msg->GetPayloadSize(),status);
				break;
			case MFM_DATA:

				break;
			case MFM_ROUTING:
				break;
			case MFM_NEIGHBORHOOD:
				break;
			case MFM_TIMESYNC:
				break;
			default:
				break;
		};

		//return msg;
}


//DeviceStatus SetConfig(MacConfig *config);
//BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int Size);

DeviceStatus OMACTypeBora::SetConfig(MacConfig *config){
	MyConfig.BufferSize = config->BufferSize;
	MyConfig.CCA = config->BufferSize;
	MyConfig.CCASenseTime = config->CCASenseTime;
	MyConfig.RadioID  = config->RadioID;
	MyConfig.FCF = config->FCF;
	MyConfig.DestPAN = config->DestPAN;
	MyConfig.Network = config->Network;
	MyConfig.NeighborLivenessDelay = config->NeighborLivenessDelay;
	return DS_Success;
}



DeviceStatus OMACTypeBora::Initialize(MacEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioID, MacConfig *config) {
//DeviceStatus OMACTypeBora::Initialize(MacEventHandler* eventHandler, UINT8* macID, UINT8 routingAppID, MacConfig *config) {
	DeviceStatus status;
	//Initialize yourself first (you being the MAC)
	if(this->Initialized){
		hal_printf("OMACTypeBora Error: Already Initialized!! My address: %d\n", CPU_Radio_GetAddress(this->radioName));
	}
	else {
		this->macName = macName;
		this->radioName = radioID;
		SetConfig(config);
		AppCount=0; //number of upperlayers connected to you
		OMACTypeBora::SetMaxPayload((UINT16)(IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t)));

#ifdef DEBUG_OMAC
		hal_printf("Initializing OMACTypeBora: My address: %d\n", CPU_Radio_GetAddress(this->radioName));
		CPU_GPIO_EnableOutputPin(OMACDEBUGPIN, FALSE);
#endif

		Radio_Event_Handler.RadioInterruptMask = (StartOfTransmission|EndOfTransmission|StartOfReception);
		Radio_Event_Handler.SetRadioInterruptHandler(OMACRadioInterruptHandler);
		Radio_Event_Handler.SetRecieveHandler(OMACReceiveHandler);
		Radio_Event_Handler.SetSendAckHandler(OMACSendAckHandler);

		g_send_buffer.Initialize();
		g_receive_buffer.Initialize();

		g_NeighborTable.ClearTable();

		RadioAckPending=FALSE;
		Initialized=TRUE;
		m_recovery = 1;

		if((status = CPU_Radio_Initialize(&Radio_Event_Handler, this->radioName, NumberRadios, macName)) != DS_Success)
			return status;

		g_omac_RadioControl.Initialize();
		//SetAddress(MF_NODE_ID);
		//MyAddress = MF_NODE_ID;
		g_omac_scheduler.Initialize(radioName, macName);
		Initialized=TRUE;
	}

	//Initialize upper layer call backs
	if(routingAppID >= MAX_APPS) {
		return DS_Fail;
	}
	AppHandlers[routingAppID]=eventHandler;
	CurrentActiveApp=routingAppID;
	//*macID=MacId;

	//Initialize radio layer
	return DS_Success;
}

BOOL OMACTypeBora::UnInitialize()
{
	BOOL ret = TRUE;
	Initialized=FALSE;
	ret &= CPU_Radio_UnInitialize(this->radioName);
	return ret;
}

Message_15_4_t * OMACTypeBora::ReceiveHandler(Message_15_4_t * msg, int Size)
{
	//Message_15_4_t *Next;
#ifdef DEBUG_OMAC
	CPU_GPIO_SetPinState(OMACDEBUGPIN, TRUE);
	CPU_GPIO_SetPinState(OMACDEBUGPIN, FALSE);
#endif

	if(Size- sizeof(IEEE802_15_4_Header_t) >  OMACTypeBora::GetMaxPayload()){
			hal_printf("CSMA Receive Error: Packet is too big: %d ", Size+sizeof(IEEE802_15_4_Header_t));
			return msg;
	}

	Size -= sizeof(IEEE802_15_4_Header_t);

	//Any message might have timestamping attached to it. Check for it and process
	if(msg->GetHeader()->flags == TIMESTAMPED_FLAG && msg->GetHeader()->GetType()!=MFM_TIMESYNC){
		UINT8 tmsgSize = sizeof(TimeSyncMsg)+4;
		g_omac_scheduler.m_TimeSyncHandler.Receive(msg,msg->GetPayload()+Size-tmsgSize, tmsgSize);
		Size -= tmsgSize;
	}

	//Demutiplex packets received based on type
	switch(msg->GetHeader()->GetType()){
		case MFM_DISCOVERY:
			g_omac_scheduler.m_DiscoveryHandler.Receive(msg, msg->GetPayload(),Size);
			break;
		case MFM_DATA:
			hal_printf("Successfully got a data packet");

			break;
		case MFM_ROUTING:
			break;
		case MFM_NEIGHBORHOOD:
			break;
		case MFM_TIMESYNC:
			g_omac_scheduler.m_TimeSyncHandler.Receive(msg,msg->GetPayload(), Size);
			break;
		case OMAC_DATA_BEACON_TYPE:
			hal_printf("Got a data beacon packet\n");

		default:
			break;
	};


	return msg;
}


void RadioInterruptHandler(RadioInterrupt Interrupt, void* Param)
{

}

//Store packet in the send buffer and return; Scheduler will pick it up latter and will send it
BOOL OMACTypeBora::Send(UINT16 address, UINT8 dataType, void* msg, int size)
{
	if(g_send_buffer.IsFull())
		return FALSE;

	Message_15_4_t *msg_carrier = g_send_buffer.GetNextFreeBuffer();
	if(size >  OMACTypeBora::GetMaxPayload()){
		hal_printf("OMACTypeBora Send Error: Packet is too big: %d ", size);
		return FALSE;
	}
	IEEE802_15_4_Header_t *header = msg_carrier->GetHeader();
	header->length = size + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest =address;
	header->src = CPU_Radio_GetAddress(this->radioName);;
	header->network = MyConfig.Network;
	header->mac_id = macName;
	header->type = dataType;

	UINT8* lmsg = (UINT8 *) msg;
	UINT8* payload =  msg_carrier->GetPayload();

	for(UINT8 i = 0 ; i < size; i++)
		payload[i] = lmsg[i];

	return true;
}

Message_15_4_t* OMACTypeBora::FindFirstSyncedNbrMessage(){
	return NULL;
}

void OMACTypeBora::UpdateNeighborTable(){
	g_NeighborTable.UpdateNeighborTable(MyConfig.NeighborLivenessDelay);
	//g_NeighborTable.DegradeLinks();
}

/*BOOL OMACTypeBora::SetAddress(UINT16 address){
	MyAddress = address;
	return TRUE;
}
*/
