#include "csmaMAC.h"
#include <Samraksh/HALTimer.h>

//#define DEBUG_MAC 1

csmaMAC gcsmaMacObject;
extern HALTimerManager gHalTimerManagerObject;

void* csmaRecieveHandler(void *msg, UINT16 Size)
{
	return (void*) gcsmaMacObject.ReceiveHandler((Message_15_4_t *) msg, Size);
}

void csmaSendAckHandler(void* msg, UINT16 Size, NetOpStatus status)
{
	gcsmaMacObject.SendAckHandler(msg, Size, status);
}

BOOL csmaRadioInterruptHandler(RadioInterrupt Interrupt, void *param)
{
	return gcsmaMacObject.RadioInterruptHandler(Interrupt, param);
}

void csmaMacScheduler(void * arg){
#ifdef DEBUG_MAC
	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
#endif
	gcsmaMacObject.SendToRadio();
#ifdef DEBUG_MAC
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
#endif
}

DeviceStatus csmaMAC::SetConfig(MacConfig *config){
	MyConfig.BufferSize = config->BufferSize;
	MyConfig.CCA = config->BufferSize;
	MyConfig.CCASenseTime = config->CCASenseTime;
	MyConfig.RadioID  = config->RadioID;
	MyConfig.FCF = config->FCF;
	MyConfig.DestPAN = config->DestPAN;
	MyConfig.Network = config->Network;

	return DS_Success;
}

DeviceStatus csmaMAC::Initialize(MacEventHandler* eventHandler, UINT8* macID, UINT8 routingAppID, MacConfig *config)
{
	//Initialize yourself first (you being the MAC)
	if(!this->Initialized){
		MacId = csmaMAC::GetUniqueMacID();
		csmaMAC::SetAddress(MF_NODE_ID);
		SetConfig(config);
		AppCount=0; //number of upperlayers connected to you
		csmaMAC::SetMaxPayload((UINT16)(IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t)-2));

		Radio_Event_Handler.SetRadioInterruptHandler(csmaRadioInterruptHandler);
		Radio_Event_Handler.SetRecieveHandler(csmaRecieveHandler);
		Radio_Event_Handler.SetSendAckHandler(csmaSendAckHandler);

		m_send_buffer.Initialize();
		m_receive_buffer.Initialize();

		//Mukundan: Feb 11, 2013
		//Commenting out neighbor table to test GC bug:
		//m_NeighborTable.InitObject();

		UINT8 numberOfRadios = 1;
		UINT8 radioIds = 1;
		RadioAckPending=FALSE;
		Initialized=TRUE;
		m_recovery = 1;

		CPU_Radio_Initialize(&Radio_Event_Handler, &radioIds, numberOfRadios, MacId);
		gHalTimerManagerObject.Initialize();
		if(!gHalTimerManagerObject.CreateTimer(3, 0, 100000, FALSE, FALSE, csmaMacScheduler)){ //100 milli sec Timer in micro seconds
			return DS_Fail;
		}
	}

	//Initalize upperlayer callbacks
	if(routingAppID >=MAX_APPS) {
		return DS_Fail;
	}
	AppHandlers[routingAppID]=eventHandler;
	CurrentActiveApp=routingAppID;
	*macID=MacId;

	return DS_Success;
}


BOOL csmaMAC::UnInitialize()
{
	CPU_Radio_UnInitialize();

}

UINT8 test = 0;

BOOL csmaMAC::Send(UINT16 dest, UINT8 dataType, void* msg, int Size)
{
	Message_15_4_t msg_carrier;
	if(Size >  csmaMAC::GetMaxPayload()){
		hal_printf("CSMA Send Error: Packet is too big: %d ", Size);
		return FALSE;
	}
	IEEE802_15_4_Header_t *header = msg_carrier.GetHeader();

	header->length = Size + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest =dest;
	header->src = MyAddress;
	header->network = MyConfig.Network;
	header->mac_id = MacId;
	header->type = dataType;

	UINT8* lmsg = (UINT8 *) msg;
	UINT8* payload =  msg_carrier.GetPayload();

	for(UINT8 i = 0 ; i < Size; i++)
		payload[i] = lmsg[i];

	// Check if the circular buffer is full
	if(!m_send_buffer.Store((void *) &msg_carrier, header->GetLength()))
			return FALSE;

	return TRUE;
}

void csmaMAC::SendToRadio(){
	if(!m_send_buffer.IsEmpty() && !RadioAckPending){

		m_recovery = 1;

		//Try twice with random wait between, if carrier sensing fails return; MAC will try again later
		DeviceStatus ds = CPU_Radio_ClearChannelAssesment2(1, 200);
		if(ds != DS_Success) {
			HAL_Time_Sleep_MicroSeconds((MyAddress % 500));
			if(CPU_Radio_ClearChannelAssesment2(1, 200)!=DS_Success){ 	return;}
		}

		Message_15_4_t** temp = m_send_buffer.GetOldestPtr();
		Message_15_4_t* msg = *temp;

		//if(temp == NULL || *temp == NULL){test = 0;}

		// Check to see if there are any messages in the buffer
		if(*temp != NULL){
			RadioAckPending=TRUE;
			*temp = (Message_15_4_t *) CPU_Radio_Send(1, (msg), (msg->GetHeader())->GetLength());

		}
	}

	else if(RadioAckPending)
	{
		RadioAckPending = FALSE;
		hal_printf("Unable to recieve send ack from radio\n");
		CPU_Radio_Reset(1);

#if 0
		if(m_recovery & LEVEL_0_RECOVER)
		{
			m_recovery = m_recovery << 1;
		}
		else if(m_recovery & LEVEL_1_RECOVER)
		{
			m_recovery = m_recovery << 1;
			RadioAckPending = FALSE;
		}
		else if(m_recovery & LEVEL_2_RECOVER)
		{
			m_recovery = m_recovery << 1;
			RadioAckPending = FALSE;
			CPU_Radio_Reset(1);
			if(CPU_Radio_TurnOn(1) == DS_Fail)
			{
				hal_printf("Radio Reset failed");
			}
		}
		else
		{
			hal_printf("Unable to recieve send acks from radio\n");
			//while(1);
		}
#endif
	}

}

Message_15_4_t* csmaMAC::ReceiveHandler(Message_15_4_t* msg, int Size)
{
	if(Size- sizeof(IEEE802_15_4_Header_t) >  csmaMAC::GetMaxPayload()){
		hal_printf("CSMA Receive Error: Packet is too big: %d ", Size+sizeof(IEEE802_15_4_Header_t));
		return FALSE;
	}
	Message_15_4_t** next_free_buffer = m_receive_buffer.GetNextFreeBufferPtr();

	if(! (next_free_buffer))
	{
		m_receive_buffer.DropOldest(1);
		next_free_buffer = m_receive_buffer.GetNextFreeBufferPtr();
	}

	//if(next_free_buffer == NULL || *next_free_buffer == NULL) {i = 0;}

	//Implement bag exchange, by actually switching the contents.
	Message_15_4_t* temp = *next_free_buffer;	//get the ptr to a msg inside the first free buffer.
	(*next_free_buffer) = msg;	//put the currently received message into the buffer (thereby its not free anymore)
								//finally the temp, which is a ptr to free message will be returned.


	//Handle the message
	IEEE802_15_4_Header_t *rcv_msg_hdr = msg->GetHeader();

	//Mukundan: Feb 11, 2013
	//Commenting out neighbor table to test GC bug:
	//Add the sender to NeighborTable
	/*UINT8 index = m_NeighborTable.FindIndex(rcv_msg_hdr->src);
	if(index==255) {
		m_NeighborTable.InsertNeighbor(rcv_msg_hdr->src, Alive, Time_GetLocalTime());
	}else {
		m_NeighborTable.UpdateNeighbor(rcv_msg_hdr->src, Alive, Time_GetLocalTime());
	}*/

	if(rcv_msg_hdr->dest == MAC_BROADCAST_ADDRESS){
		HandleBroadcastMessage(msg);
	}else if(rcv_msg_hdr->dest == MF_NODE_ID){
		HandleUnicastMessage(msg);
	}
	else {
		HandlePromiscousMessage(msg);
	}

	//Call routing/app receive callback
	MacReceiveFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->RecieveHandler;
	(*appHandler)(msg->GetPayload(), Size- sizeof(IEEE802_15_4_Header_t));

	return temp;
}

BOOL csmaMAC::HandleBroadcastMessage(Message_15_4_t * msg){

}

BOOL csmaMAC::HandleUnicastMessage(Message_15_4_t * msg){

}
BOOL csmaMAC::HandlePromiscousMessage(Message_15_4_t * msg){

}


BOOL csmaMAC::RadioInterruptHandler(RadioInterrupt Interrupt, void* Param)
{
	return FALSE;
}


void csmaMAC::SendAckHandler(void* msg, int Size, NetOpStatus status)
{
	//Radio sent it out successfully,
	if(status==NO_Success){
		SendAckFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->SendAckHandler;
		(*appHandler)(msg, Size, status);
	}
	else{	//failed
		//retry sending message
	}
	RadioAckPending=FALSE;
}

UINT8 csmaMAC::GetBufferSize(){
	return m_send_buffer.Size();
}

