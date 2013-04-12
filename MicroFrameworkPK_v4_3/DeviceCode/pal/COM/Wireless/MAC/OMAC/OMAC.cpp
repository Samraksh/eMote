/*
 * Copyright The Samraksh Company
 *
 * Author: Mukundan.Sridharan, Nived.Sivadas
 *
 * Description :  OMAC Implementation, v 1.0
 *
 *  Created on: Aug 30, 2012
 */

#include "OMAC.h"
#include "RadioControl.h"
#include <Samraksh/Radio_decl.h>

OMAC g_OMAC;
RadioControl_t g_omac_RadioControl;

void* OMACReceiveHandler(void* msg, UINT16 size){
	return (void*) g_OMAC.ReceiveHandler((Message_15_4_t*) msg, size);
}
BOOL OMACRadioInterruptHandler(RadioInterrupt Interrupt, void* Param){

}
void OMACSendAckHandler(void *msg, UINT16 Size, NetOpStatus status){
	Message_15_4_t *rcv_msg = (Message_15_4_t *)msg;

		//Demutiplex packets received based on type
		switch(rcv_msg->GetHeader()->GetType()){
			case MFM_DISCOVERY:
				g_OMAC.m_omac_scheduler.m_DiscoveryHandler.BeaconAckHandler(rcv_msg,rcv_msg->GetPayloadSize(),status);
				break;
			case MFM_DATA:

				break;
			case MFM_ROUTING:
				break;
			case MFM_NEIGHBORHOOD:
				break;
			default:
				break;
		};

		//return msg;
}


DeviceStatus SetConfig(MacConfig *config);
BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int Size);

DeviceStatus OMAC::SetConfig(MacConfig *config){
	MyConfig.BufferSize = config->BufferSize;
	MyConfig.CCA = config->BufferSize;
	MyConfig.CCASenseTime = config->CCASenseTime;
	MyConfig.RadioID  = config->RadioID;
	MyConfig.FCF = config->FCF;
	MyConfig.DestPAN = config->DestPAN;
	MyConfig.Network = config->Network;
	return DS_Success;
}

//Initialize
DeviceStatus OMAC::Initialize(MacEventHandler* eventHandler, UINT8* macID, UINT8 routingAppID, MacConfig *config)
{
	//Initialize yourself first (you being the MAC)
	if(!this->Initialized){
		MacId = OMAC::GetUniqueMacID();
		OMAC::SetAddress(MF_NODE_ID);
		SetConfig(config);
		AppCount=0; //number of upperlayers connected to you

		CPU_GPIO_EnableOutputPin((GPIO_PIN) 10, FALSE);

		Radio_Event_Handler.RadioInterruptMask = (StartOfTransmission|EndOfTransmission|StartOfReception);
		Radio_Event_Handler.SetRadioInterruptHandler(OMACRadioInterruptHandler);
		Radio_Event_Handler.SetRecieveHandler(OMACReceiveHandler);
		Radio_Event_Handler.SetSendAckHandler(OMACSendAckHandler);

		m_send_buffer.Initialize();
		m_receive_buffer.Initialize();

		//m_NeighborTable.InitObject();

		Initialized=TRUE;

		CPU_Radio_Initialize(&Radio_Event_Handler, RadioIDs, NumberRadios, MacId);
		g_omac_RadioControl.Initialize(RadioIDs[0]);

		//MAC <Message_15_4_t>::Initialize();
		m_omac_scheduler.Initialize();
	}

	//Initialize upper layer call backs
	if(routingAppID >=MAX_APPS) {
		return DS_Fail;
	}
	AppHandlers[routingAppID]=eventHandler;
	CurrentActiveApp=routingAppID;
	*macID=MacId;

	//Initialize radio layer
	return DS_Success;
}



//UnInitialize
BOOL OMAC::UnInitialize()
{
	return TRUE;
}



Message_15_4_t * OMAC::ReceiveHandler(Message_15_4_t * msg, int Size)
{
	//Message_15_4_t *Next;

	//Demutiplex packets received based on type
	switch(msg->GetHeader()->GetType()){
		case MFM_DISCOVERY:
			this->m_omac_scheduler.m_DiscoveryHandler.Receive(msg, msg->GetPayload(),msg->GetPayloadSize());
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


	return msg;
}


void RadioInterruptHandler(RadioInterrupt Interrupt, void* Param)
{

}


BOOL OMAC::Send(UINT16 address, UINT8 dataType, void* msg, int size)
{
	//(SendBuffer.GetNext())->
	//SendBuffer.Store(msg);


	return true;
}



