#include "csmaMAC.h"
#include <Samraksh/HALTimer.h>

//#define DEBUG_MAC 1

csmaMAC gcsmaMacObject;
extern HALTimerManager gHalTimerManagerObject;

UINT8 RadioLockUp;
UINT16 discoveryCounter = 0;

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

	discoveryCounter++;

#ifdef DEBUG_MAC
	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
#endif
	gcsmaMacObject.UpdateNeighborTable();
	gcsmaMacObject.SendToRadio();
	if(discoveryCounter == DISCOVERY_FREQUENCY)
	{
		// Disabling neighbour discovery
		gcsmaMacObject.SendHello();
		discoveryCounter = 0;
	}
#ifdef DEBUG_MAC
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
#endif
}

DeviceStatus csmaMAC::SendHello()
{
	UINT8 helloPayload[5];

	helloPayload[0] = (UINT8) 'H';
	helloPayload[1] = (UINT8) 'E';
	helloPayload[2] = (UINT8) 'L';
	helloPayload[3] = (UINT8) 'L';
	helloPayload[4] = (UINT8) 'O';

	if(gcsmaMacObject.Send(0xffff, MFM_DISCOVERY, (void *) helloPayload, 5) == TRUE)
		return DS_Success;

	return DS_Fail;
}

DeviceStatus csmaMAC::SetConfig(MacConfig *config){
	MyConfig.BufferSize = config->BufferSize;
	MyConfig.CCA = config->BufferSize;
	MyConfig.CCASenseTime = config->CCASenseTime;
	MyConfig.RadioID  = config->RadioID;
	MyConfig.FCF = config->FCF;
	MyConfig.DestPAN = config->DestPAN;
	MyConfig.Network = config->Network;
	MyConfig.NeighbourLivelinessDelay = config->NeighbourLivelinessDelay;

	return DS_Success;
}

DeviceStatus csmaMAC::Initialize(MacEventHandler* eventHandler, UINT8* macID, UINT8 routingAppID, UINT8 radioID, MacConfig *config)
{
	//Initialize yourself first (you being the MAC)
	if(!this->Initialized){
		MacId = csmaMAC::GetUniqueMacID();
		csmaMAC::SetAddress(MF_NODE_ID);
		MyAddress = MF_NODE_ID;
		SetConfig(config);
		AppCount=0; //number of upperlayers connected to you
		csmaMAC::SetMaxPayload((UINT16)(IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t)));

		Radio_Event_Handler.SetRadioInterruptHandler(csmaRadioInterruptHandler);
		Radio_Event_Handler.SetRecieveHandler(csmaRecieveHandler);
		Radio_Event_Handler.SetSendAckHandler(csmaSendAckHandler);

		m_send_buffer.Initialize();
		m_receive_buffer.Initialize();
		m_NeighborTable.ClearTable();

		//m_NeighborTable.InitObject();

		UINT8 numberOfRadios = 1;
		RadioAckPending=FALSE;
		Initialized=TRUE;
		m_recovery = 1;

		CPU_Radio_Initialize(&Radio_Event_Handler, radioID, numberOfRadios, MacId);

		CPU_Radio_TurnOn(radioID);

		gHalTimerManagerObject.Initialize();
		if(!gHalTimerManagerObject.CreateTimer(3, 0, 50000, FALSE, FALSE, csmaMacScheduler)){ //50 milli sec Timer in micro seconds
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
	CPU_Radio_UnInitialize(1);

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
	header->src = MF_NODE_ID;
	header->network = MyConfig.Network;
	header->mac_id = MacId;
	header->type = dataType;

	UINT8* lmsg = (UINT8 *) msg;
	UINT8* payload =  msg_carrier.GetPayload();

	for(UINT8 i = 0 ; i < Size; i++)
		payload[i] = lmsg[i];

	//hal_printf("CSMA Sending: My address is : %d\n",MF_NODE_ID);
	// Check if the circular buffer is full
	if(!m_send_buffer.Store((void *) &msg_carrier, header->GetLength()))
			return FALSE;

	// Try to  send the packet out immediately if possible
	//csmaMacScheduler(NULL);

	return TRUE;
}

// This function calls the updateneighbrtable of the neighbour object and calls the neighbour change
// callback if the neighbours died
void csmaMAC::UpdateNeighborTable(){

	UINT8 numberOfDeadNeighbours = m_NeighborTable.UpdateNeighborTable(MyConfig.NeighbourLivelinessDelay);


	if(numberOfDeadNeighbours > 0)
	{
		NeighbourChangeFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->neighbourHandler;

		// Check if neighbour change has been registered and the user is interested in this information
		if(appHandler != NULL)
		{
			// Make the neighbour changed callback signalling dead neighbours
			(*appHandler)((INT16) ((-1) *numberOfDeadNeighbours));
		}
	}
	//m_NeighborTable.DegradeLinks();
}

// Called by the mac for retrying in case of failed packets 
BOOL csmaMAC::Resend(void* msg, int Size)
{
	// Try and push the packet back into the buffer
	if(!m_send_buffer.Store(msg, Size)) 
		return FALSE;
		
	return TRUE;
}

void csmaMAC::SendToRadio(){
	if(!m_send_buffer.IsEmpty() && !RadioAckPending){

		m_recovery = 1;

		//Try twice with random wait between, if carrier sensing fails return; MAC will try again later
		DeviceStatus ds = CPU_Radio_ClearChannelAssesment2(1, 200);
		if(ds != DS_Success) {
			HAL_Time_Sleep_MicroSeconds((MF_NODE_ID % 500));
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

#if 0
	else if(RadioAckPending)
	{
		RadioAckPending = FALSE;
		hal_printf("Unable to recieve send ack from radio\n");
		RadioLockUp++;
		if(RadioLockUp > 10){
			hal_printf("CSMA: Radio seems to have lock up: %d \n", RadioLockUp);
		}
		CPU_Radio_Reset(1);


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

	}
#endif

}

Message_15_4_t* csmaMAC::ReceiveHandler(Message_15_4_t* msg, int Size)
{
	UINT8 index;
	if(Size- sizeof(IEEE802_15_4_Header_t) >  csmaMAC::GetMaxPayload()){
		hal_printf("CSMA Receive Error: Packet is too big: %d ", Size+sizeof(IEEE802_15_4_Header_t));
		return msg;
	}


	// Get the header packet
	IEEE802_15_4_Header_t *rcv_msg_hdr = msg->GetHeader();
	IEEE802_15_4_Metadata_t * rcv_meta = msg->GetMetaData();

	// If the message type is a discovery then return the same bag you got from the radio layer
	// Don't make a callback here because the neighbour table takes care of informing the application of a changed situation of
	// it neighbours
	if(rcv_msg_hdr->type == MFM_DISCOVERY)
	{
			//Add the sender to NeighborTable
			if(m_NeighborTable.FindIndex(rcv_msg_hdr->src, &index) != DS_Success)
			{
				// Insert into the table if a new node was discovered
				if(m_NeighborTable.InsertNeighbor(rcv_msg_hdr->src, Alive, HAL_Time_CurrentTicks(), 0, 0, 0, 0, &index) == DS_Success)
				{
					NeighbourChangeFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->neighbourHandler;

					// Check if  a neighbour change has been registered
					if(appHandler != NULL)
					{
						// Insert neighbour always inserts one neighbour so the call back argument will alsways be 1
						(*appHandler)(1);
					}
				}
			}
			else
			{
				m_NeighborTable.UpdateNeighbor(rcv_msg_hdr->src, Alive, HAL_Time_CurrentTicks(), rcv_meta->GetRssi(), rcv_meta->GetLqi());
#if 0
				m_NeighborTable.Neighbor[index].ReverseLink.AvgRSSI =  (UINT8)((float)m_NeighborTable.Neighbor[index].ReverseLink.AvgRSSI*0.8 + (float)rcv_meta->GetRssi()*0.2);
				m_NeighborTable.Neighbor[index].ReverseLink.LinkQuality =  (UINT8)((float)m_NeighborTable.Neighbor[index].ReverseLink.LinkQuality*0.8 + (float)rcv_meta->GetLqi()*0.2);
				m_NeighborTable.Neighbor[index].PacketsReceived++;
				m_NeighborTable.Neighbor[index].LastHeardTime = HAL_Time_CurrentTicks();
				m_NeighborTable.Neighbor[index].Status = Alive;
#endif
			}

			return msg;
	}

	// Dont add the packet to the handler if the message happens to be a unicast not intended for me, unless you want to enable promiscous
	if(rcv_msg_hdr->dest != MAC_BROADCAST_ADDRESS && rcv_msg_hdr->dest != MF_NODE_ID)
	{
		//HandlePromiscousMessage(msg);
		return msg;
	}
	// Implement bag exchange if the packet type is data
	Message_15_4_t** next_free_buffer = m_receive_buffer.GetNextFreeBufferPtr();

	if(! (next_free_buffer))
	{
		m_receive_buffer.DropOldest(1);
		next_free_buffer = m_receive_buffer.GetNextFreeBufferPtr();
	}

	//Implement bag exchange, by actually switching the contents.
	Message_15_4_t* temp = *next_free_buffer;	//get the ptr to a msg inside the first free buffer.
	(*next_free_buffer) = msg;	//put the currently received message into the buffer (thereby its not free anymore)
								//finally the temp, which is a ptr to free message will be returned.


	//Call routing/app receive callback
	MacReceiveFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->RecieveHandler;

	// Nived.Sivadas The mac callback design has changed
	(*appHandler)(m_receive_buffer.GetNumberMessagesInBuffer());

#if 0
	//hal_printf("CSMA Receive: SRC address is : %d\n", rcv_msg_hdr->src);
	if(rcv_msg_hdr->dest == MAC_BROADCAST_ADDRESS){

		// Nived.Sivadas - changing interfaces with new dll design
		(*appHandler)(m_receive_buffer.GetNumberMessagesInBuffer());
		//(*appHandler)(msg->GetPayload(), Size- sizeof(IEEE802_15_4_Header_t), rcv_msg_hdr->src,FALSE,rcv_meta->GetRssi(), rcv_meta->GetLqi());
		//HandleBroadcastMessage(msg);
	}else if(rcv_msg_hdr->dest == MF_NODE_ID){
		//HandleUnicastMessage(msg);
		(*appHandler)(m_receive_buffer.GetNumberMessagesInBuffer());
		//(*appHandler)(msg->GetPayload(), Size- sizeof(IEEE802_15_4_Header_t), rcv_msg_hdr->src,TRUE,rcv_meta->GetRssi(), rcv_meta->GetLqi());
	}
	else {
		//HandlePromiscousMessage(msg);
	}
#endif

	return temp;
}


BOOL csmaMAC::RadioInterruptHandler(RadioInterrupt Interrupt, void* Param)
{
	return FALSE;
}


void csmaMAC::SendAckHandler(void* msg, int Size, NetOpStatus status)
{
	switch(status)
	{
		case NO_Success:
			{
				SendAckFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->SendAckHandler;
				(*appHandler)(msg, Size, status);
			}
			break;
		
		case NO_Busy:
			Resend(msg, Size);
			break;
			
		case NO_BadPacket:
			break;
	}
	
#if 0
	//Radio sent it out successfully,
	if(status==NO_Success){
		SendAckFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->SendAckHandler;
		(*appHandler)(msg, Size, status);
	}
	else{	
		//failed
		//retry sending message
		
	}
#endif

	RadioAckPending=FALSE;
	RadioLockUp=0;
}

UINT8 csmaMAC::GetBufferSize(){
	return m_send_buffer.Size();
}

