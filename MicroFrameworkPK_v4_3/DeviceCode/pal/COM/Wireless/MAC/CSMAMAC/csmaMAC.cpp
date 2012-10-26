#include "csmaMAC.h"


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

		CPU_GPIO_EnableOutputPin((GPIO_PIN) 10, FALSE);

		Radio_Event_Handler.SetRadioInterruptHandler(csmaRadioInterruptHandler);
		Radio_Event_Handler.SetRecieveHandler(csmaRecieveHandler);
		Radio_Event_Handler.SetSendAckHandler(csmaSendAckHandler);

		m_send_buffer.Initialize();
		m_receive_buffer.Initialize();

		m_NeighborTable.InitObject();
		UINT8 numberOfRadios = 1;
		UINT8 radioIds = 1;
		Initialized=TRUE;

		CPU_Radio_Initialize(&Radio_Event_Handler, &radioIds, numberOfRadios, MacId);
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
	//GLOBAL_LOCK(irq);

	DeviceStatus ds;

	while((ds = CPU_Radio_ClearChannelAssesment2(1, 200)) == DS_Fail);

	if(ds != DS_Success)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) 10, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 10, FALSE);
		return FALSE;
	}
	//CPU_GPIO_TogglePinState((GPIO_PIN) 0);
	//CPU_GPIO_SetPinState((GPIO_PIN) 0, TRUE);
	//Message_15_4_t *msg_carrier = SendBuffer.GetNextFreeBuffer();
	Message_15_4_t msg_carrier;

	IEEE802_15_4_Header_t *header = msg_carrier.GetHeader();

	header->length = Size + sizeof(header);
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

	test++;

	if(test == 14)
	{
		test = 0;
	}

	UINT8* lmsg = (UINT8 *) msg;

	UINT8* payload =  msg_carrier.GetPayload();

	//msg_carrier.SetActiveMessageSize(header->size() + Size);

	for(UINT8 i = 0 ; i < Size; i++)
		payload[i] = lmsg[i];

	// Check if the circular buffer is full
	if(!m_send_buffer.CopyPayload((void *) payload, Size))
		return FALSE;

	Message_15_4_t** temp = m_send_buffer.GetOldestPtr();

	if(temp == NULL || *temp == NULL)
	{
		test = 0;
	}

	memcpy(*temp, header, sizeof(header));

	// Check to see if there are any messages in the buffer
	if(*temp == NULL)
		return FALSE;

#if 0
	UINT8 send_buffer[] = {
								111, 65, 136, 97, 34, 0, 255, 255, 102,
								0, 63, 239, 0, 102, 0, 10, 0, 0, 0, 1, 0,
								2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0,
								9, 0, 10, 0, 11, 0, 12, 0, 13, 0, 14, 0, 15,
								0, 16, 0, 17, 0, 18, 0, 19, 0, 20, 0, 21, 0,
								22, 0, 23, 0, 24, 0, 25, 0, 26, 0, 27, 0, 28,
								0, 29, 0, 30, 0, 31, 0, 32, 0, 33, 0, 34, 0,
								35, 0, 36, 0, 37, 0, 38, 0, 39, 0, 40, 0, 41,
								0, 42, 0, 43, 0, 44, 0, 45,112, 102
							};

#endif
	*temp = (Message_15_4_t *) CPU_Radio_Send(1, (*temp), (msg_carrier.GetHeader())->GetLength());

	return TRUE;

}

UINT8 i = 0;

Message_15_4_t* csmaMAC::ReceiveHandler(Message_15_4_t* msg, int Size)
{

	Message_15_4_t** next_free_buffer = m_receive_buffer.GetNextFreeBufferPtr();

	i++;
	if(i > 13)
	{
		i = 0;
	}
	if(! (next_free_buffer))
	{
		m_receive_buffer.DropOldest(1);
		next_free_buffer = m_receive_buffer.GetNextFreeBufferPtr();
	}

	if(next_free_buffer == NULL || *next_free_buffer == NULL)
	{
			i = 0;
	}

	//Implement bag exchange, by actually switching the contents.
	Message_15_4_t* temp = *next_free_buffer;	//get the ptr to a msg inside the first free buffer.
	(*next_free_buffer) = msg;	//put the currently received message into the buffer (thereby its not free anymore)
								//finally the temp, which is a ptr to free message will be returned.


	//Handle the message
	IEEE802_15_4_Header_t *rcv_msg_hdr = msg->GetHeader();

	//Add the sender to NeighborTable
	UINT8 index = m_NeighborTable.FindIndex(rcv_msg_hdr->src);
	if(index==255) {
		m_NeighborTable.InsertNeighbor(rcv_msg_hdr->src, Alive, Time_GetLocalTime());
	}else {
		m_NeighborTable.UpdateNeighbor(rcv_msg_hdr->src, Alive, Time_GetLocalTime());
	}

	if(rcv_msg_hdr->dest == MAC_BROADCAST_ADDRESS){
		HandleBroadcastMessage(msg);
	}else {
		HandleUnicastMessage(msg);
	}




	//Call routing/app receive callback
	MacReceiveFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->RecieveHandler;
	(*appHandler)(msg->GetPayload(), Size );

	return temp;
}

BOOL csmaMAC::HandleBroadcastMessage(Message_15_4_t * msg){

}

BOOL csmaMAC::HandleUnicastMessage(Message_15_4_t * msg){

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
}

