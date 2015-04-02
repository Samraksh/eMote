#include "csmaMAC.h"
#include <Samraksh/VirtualTimer.h>
#include <Timer/netmf_timers.cpp>


csmaMAC gcsmaMacObject;

volatile UINT32 csmaSendToRadioFailCount = 0;  //!< count DS_Fail from radio during sendToRadio.

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

void SendFirstPacketToRadio(void * arg){

	gcsmaMacObject.SendToRadio();

}

// Send a beacon everytime this fires
void beaconScheduler(void *arg){
	DEBUG_PRINTF_CSMA("bS fire\r\n");
	gcsmaMacObject.UpdateNeighborTable();
	gcsmaMacObject.SendHello();
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
	MyConfig.NeighborLivelinessDelay = config->NeighborLivelinessDelay;

	DEBUG_PRINTF_CSMA("SetConfig: %d %d %d %d %d %d %d %d\r\n",MyConfig.BufferSize,MyConfig.CCA,MyConfig.CCASenseTime,MyConfig.RadioID,MyConfig.FCF,MyConfig.DestPAN,MyConfig.Network,MyConfig.NeighborLivelinessDelay);

	return DS_Success;
}

DeviceStatus csmaMAC::Initialize(MacEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioID, MacConfig *config)
{
	//Initialize yourself first (you being the MAC)
	if(!this->Initialized){
		this->macName = macName;
		this->radioName = radioID;
		SetConfig(config);
		AppCount=0; //number of upperlayers connected to you
		csmaMAC::SetMaxPayload((UINT16)(IEEE802_15_4_FRAME_LENGTH-sizeof(IEEE802_15_4_Header_t)));

		Radio_Event_Handler.SetRadioInterruptHandler(csmaRadioInterruptHandler);
		Radio_Event_Handler.SetRecieveHandler(csmaRecieveHandler);
		Radio_Event_Handler.SetSendAckHandler(csmaSendAckHandler);

		m_send_buffer.Initialize();
		m_receive_buffer.Initialize();
		m_NeighborTable.ClearTable();

		UINT8 numberOfRadios = 1;
		RadioAckPending=FALSE;
		Initialized=TRUE;
		m_recovery = 1;

		CPU_Radio_Initialize(&Radio_Event_Handler, this->radioName, numberOfRadios, macName);

		CPU_Radio_TurnOnRx(this->radioName);

		// This is the one-shot resend timer that will be activated if we need to resend a packet
		/*if(!gHalTimerManagerObject.CreateTimer(1, 0, 30000, TRUE, FALSE, SendFirstPacketToRadio)){
			return DS_Fail;
		}

		// This is the beacon timer that will send a beacon every time it goes off
		if(!gHalTimerManagerObject.CreateTimer(2, 0, 5000000, FALSE, FALSE, beaconScheduler)){*/

		//gHalTimerManagerObject.Initialize();
		//if(!gHalTimerManagerObject.CreateTimer(1, 0, 10000, FALSE, FALSE, SendFirstPacketToRadio)){ //50 milli sec Timer in micro seconds
		if(VirtTimer_SetTimer(VIRT_TIMER_MAC_SENDPKT, 0, 30000, TRUE, TRUE, SendFirstPacketToRadio) != TimerSupported){ //50 milli sec Timer in micro seconds
			SOFT_BREAKPOINT();
			return DS_Fail;
		}

		if(VirtTimer_SetTimer(VIRT_TIMER_MAC_BEACON, 0, 5000000, FALSE, TRUE, beaconScheduler) != TimerSupported){
			SOFT_BREAKPOINT();
			return DS_Fail;
		}
		//gHalTimerManagerObject.StartTimer(2);
		VirtTimer_Start(VIRT_TIMER_MAC_BEACON);

		// This is the buffer flush timer that flushes the send buffer if it contains more than just one packet
		flushTimerRunning = false;
		if(VirtTimer_SetTimer(VIRT_TIMER_MAC_FLUSHBUFFER, 0, 50000, FALSE, TRUE, SendFirstPacketToRadio) != TimerSupported){
			SOFT_BREAKPOINT();
			return DS_Fail;
		}
	}

	// Stop the timer
	//gHalTimerManagerObject.StopTimer(1);
	//VirtTimer_Stop(VIRT_TIMER_MAC_SENDPKT);
	//gHalTimerManagerObject.StopTimer(2);

	//Initialize upperlayer callbacks
	if(routingAppID >=MAX_APPS) {
		SOFT_BREAKPOINT();
		return DS_Fail;
	}
	AppHandlers[routingAppID]=eventHandler;
	CurrentActiveApp=routingAppID;

	return DS_Success;
}

BOOL csmaMAC::SetRadioAddress(UINT16 address){
	CPU_Radio_SetAddress(this->radioName, address);
	return true;
}

UINT16 csmaMAC::GetRadioAddress(){
	UINT16 temp = CPU_Radio_GetAddress(this->radioName);
	return temp;
}

BOOL csmaMAC::UnInitialize()
{
	BOOL ret = TRUE;
	if(this->Initialized) {
		if( VirtTimer_Stop(VIRT_TIMER_MAC_BEACON) != TimerSupported ) {
			SOFT_BREAKPOINT();
			ret = FALSE;
		}
		if( VirtTimer_Stop(VIRT_TIMER_MAC_SENDPKT) != TimerSupported ) {
			SOFT_BREAKPOINT();
			ret = FALSE;
		}
		if( VirtTimer_Stop(VIRT_TIMER_MAC_FLUSHBUFFER) != TimerSupported ) {
			SOFT_BREAKPOINT();
			ret = FALSE;
		}

		ret &= CPU_Radio_UnInitialize(this->radioName);
	}
	return ret;
}

UINT8 test = 0;

BOOL csmaMAC::SendTimeStamped(UINT16 dest, UINT8 dataType, void* msg, int Size, UINT32 eventTime)
{
	Message_15_4_t msg_carrier;
		if(Size >  csmaMAC::GetMaxPayload()){
			hal_printf("CSMA Send Error: Packet is too big: %d \r\n", Size);
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
		header->src = CPU_Radio_GetAddress(this->radioName);
		header->network = MyConfig.Network;
		header->mac_id = this->macName;
		header->type = dataType;
		header->SetFlags(MFM_DATA | MFM_TIMESYNC);

		UINT8* lmsg = (UINT8 *) msg;
		UINT8* payload =  msg_carrier.GetPayload();

		IEEE802_15_4_Metadata_t *metaData =  msg_carrier.GetMetaData();
		metaData->SetReceiveTimeStamp(eventTime);

		for(UINT8 i = 0 ; i < Size; i++)
			payload[i] = lmsg[i];

		DEBUG_PRINTF_CSMA("CSMA Sending: My address is : %d\r\n",CPU_Radio_GetAddress(this->radioName));

		// Check if the circular buffer is full
		if(!m_send_buffer.Store((void *) &msg_carrier, header->GetLength()))
			return FALSE;

		// Try to  send the packet out immediately if possible
		SendFirstPacketToRadio(NULL);

		return TRUE;
}

BOOL csmaMAC::Send(UINT16 dest, UINT8 dataType, void* msg, int Size)
{
	Message_15_4_t msg_carrier;
	if(Size >  csmaMAC::GetMaxPayload()){
		hal_printf("CSMA Send Error: Packet is too big: %d \r\n", Size);
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
	header->src = CPU_Radio_GetAddress(this->radioName);
	header->network = MyConfig.Network;
	header->mac_id = this->macName;
	header->type = dataType;
	header->SetFlags(MFM_DATA);

	UINT8* lmsg = (UINT8 *) msg;
	UINT8* payload =  msg_carrier.GetPayload();

	for(UINT8 i = 0 ; i < Size; i++)
		payload[i] = lmsg[i];

	DEBUG_PRINTF_CSMA("CSMA Sending: dest: %d, src: %d, network: %d, mac_id: %d, type: %d\r\n",dest, CPU_Radio_GetAddress(this->radioName),  MyConfig.Network,this->macName,dataType);

	// Check if the circular buffer is full
	if(!m_send_buffer.Store((void *) &msg_carrier, header->GetLength()))
			return FALSE;

	// Try to  send the packet out immediately if possible
	SendFirstPacketToRadio(NULL);

	return TRUE;
}

// This function calls the updateneighbrtable of the neighbor object and calls the neighbor change
// callback if the neighbors died
void csmaMAC::UpdateNeighborTable(){

	UINT8 numberOfDeadNeighbors = m_NeighborTable.UpdateNeighborTable(MyConfig.NeighborLivelinessDelay);


	if(numberOfDeadNeighbors > 0)
	{
		DEBUG_PRINTF_CSMA("number of dead neighbors: %d\r\n",numberOfDeadNeighbors);
		NeighborChangeFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->neighborHandler;

		// Check if neighbor change has been registered and the user is interested in this information
		if(appHandler != NULL)
		{
			// Make the neighbor changed callback signalling dead neighbors
			//(*appHandler)((INT16) ((-1) *numberOfDeadNeighbors));
			(*appHandler)((INT16) (m_NeighborTable.NumberOfNeighbors()));
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
	// if we have more than one packet in the send buffer we will switch on the timer that will be used to flush the packets out
	DEBUG_PRINTF_CSMA("SndRad<%d> %d\r\n",m_send_buffer.GetNumberMessagesInBuffer(), RadioAckPending);
	if ( (m_send_buffer.GetNumberMessagesInBuffer() > 1) && (flushTimerRunning == false) ){
		DEBUG_PRINTF_CSMA("start FLUSHBUFFER3\r\n");
		//gHalTimerManagerObject.StartTimer(3);
		VirtTimer_Start(VIRT_TIMER_MAC_FLUSHBUFFER);
		flushTimerRunning = true;
	}
	else if ( (m_send_buffer.GetNumberMessagesInBuffer() == 0) && (flushTimerRunning == true) ){
		DEBUG_PRINTF_CSMA("stop FLUSHBUFFER3\r\n");
		//gHalTimerManagerObject.StopTimer(3);
		VirtTimer_Stop(VIRT_TIMER_MAC_FLUSHBUFFER);
		flushTimerRunning = false;
	}


	if(!m_send_buffer.IsEmpty() && !RadioAckPending){

		m_recovery = 1;

		//Try twice with random wait between, if carrier sensing fails return; MAC will try again later
		DeviceStatus ds = CPU_Radio_ClearChannelAssesment2(this->radioName, 200);
		if(ds == DS_Busy) {
			/*HAL_Time_Sleep_MicroSeconds((CPU_Radio_GetAddress() % 200));
			if(CPU_Radio_ClearChannelAssesment2(this->radioName, 200)!=DS_Success)
			{
				gHalTimerManagerObject.StartTimer(1);
				return;
			}*/
			//TODO: AnanthAtSamraksh - check if this is right
			CPU_Timer_Sleep_MicroSeconds((CPU_Radio_GetAddress(this->radioName) % 200));
			//CPU_Time_Sleep_MicroSeconds((CPU_Radio_GetAddress(this->radioName) % 500));
			if(CPU_Radio_ClearChannelAssesment2(this->radioName, 200)!=DS_Success){ 	
				VirtTimer_Start(VIRT_TIMER_MAC_SENDPKT);
				return;
			}
		} else if (ds == DS_Fail) {
			SOFT_BREAKPOINT();
#ifdef DEBUG_CSMAMAC
			ASSERT(0);
			DEBUG_PRINTF_CSMA("Radio might have locked up\r\n");
			//CPU_Radio_Reset(this->radioName);
#endif
			++csmaSendToRadioFailCount;
			VirtTimer_Start(VIRT_TIMER_MAC_SENDPKT);
			return;
		}

		//Message_15_4_t** temp = m_send_buffer.GetOldestPtr();
		//Message_15_4_t* msg = *temp;
		
		Message_15_4_t txMsg;
		Message_15_4_t* txMsgPtr = &txMsg;
		Message_15_4_t** tempPtr = m_send_buffer.GetOldestPtr();
		Message_15_4_t* msgPtr = *tempPtr;
		memset(txMsgPtr, 0, msgPtr->GetMessageSize());
		memcpy(txMsgPtr, msgPtr, msgPtr->GetMessageSize());
		

		UINT8* snd_payload = txMsgPtr->GetPayload();

		//if(temp == NULL || *temp == NULL){test = 0;}

		// Check to see if there are any messages in the buffer
		if(txMsgPtr != NULL){
			DEBUG_PRINTF_CSMA("-------><%d> %d\r\n", (int)snd_payload[0], ((int)(snd_payload[1] << 8) + (int)snd_payload[2]) );
			RadioAckPending=TRUE;
			if(txMsgPtr->GetHeader()->GetFlags() & MFM_TIMESYNC)
			{
				UINT32 snapShot = (UINT32) txMsgPtr->GetMetaData()->GetReceiveTimeStamp();
				txMsgPtr = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(this->radioName, (txMsgPtr), (txMsgPtr->GetHeader())->GetLength(), snapShot);
			}
			else
			{
				txMsgPtr = (Message_15_4_t *) CPU_Radio_Send(this->radioName, (txMsgPtr), (txMsgPtr->GetHeader())->GetLength());
			}
		}
	}
}

Message_15_4_t* csmaMAC::ReceiveHandler(Message_15_4_t* msg, int Size)
{


	UINT8 index;
	if(Size- sizeof(IEEE802_15_4_Header_t) >  csmaMAC::GetMaxPayload()){
		hal_printf("CSMA Receive Error: Packet is too big: %d \r\n", Size+sizeof(IEEE802_15_4_Header_t));

		return msg;
	}


	// Get the header packet
	IEEE802_15_4_Header_t *rcv_msg_hdr = msg->GetHeader();
	IEEE802_15_4_Metadata_t * rcv_meta = msg->GetMetaData();
	//UINT8* rcv_payload = msg->GetPayload();

	//hal_printf("(%d) <%d> %d\r\n",Size, (int)rcv_payload[0],((int)(rcv_payload[1] << 8) + (int)rcv_payload[2]) );

	// If the message type is a discovery then return the same bag you got from the radio layer
	// Don't make a callback here because the neighbor table takes care of informing the application of a changed situation of
	// it neighbors
	if(rcv_msg_hdr->type == MFM_DISCOVERY)
	{
			//Add the sender to NeighborTable
			if(m_NeighborTable.FindIndex(rcv_msg_hdr->src, &index) != DS_Success)
			{
				// Insert into the table if a new node was discovered
				if(m_NeighborTable.InsertNeighbor(rcv_msg_hdr->src, Alive, HAL_Time_CurrentTicks(), 0, 0, 0, 0, &index) == DS_Success)
				{
					NeighborChangeFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->neighborHandler;

					// Check if  a neighbor change has been registered
					if(appHandler != NULL)
					{
						GLOBAL_LOCK(irq);  // CLR_RT_HeapBlock_NativeEventDispatcher::SaveToHALQueue calls ASSERT_IRQ_MUST_BE_OFF()
						// Insert neighbor always inserts one neighbor so the call back argument will alsways be 1
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
	if(rcv_msg_hdr->dest != MAC_BROADCAST_ADDRESS && rcv_msg_hdr->dest != CPU_Radio_GetAddress(this->radioName))
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
	MacReceiveFuncPtrType appHandler = AppHandlers[3]->RecieveHandler;  // TODO: seems wrong. -MichaelAtSamraksh

	// Protect against catastrophic errors like dereferencing a null pointer
	if(appHandler == NULL)
	{
		SOFT_BREAKPOINT();
		hal_printf("[NATIVE] Error from csma mac recieve handler :  Handler not registered\r\n");
		return temp;
	}

	GLOBAL_LOCK(irq); // CLR_RT_HeapBlock_NativeEventDispatcher::SaveToHALQueue requires IRQs off
	(*appHandler)(m_receive_buffer.GetNumberMessagesInBuffer());

#if 0
	//hal_printf("CSMA Receive: SRC address is : %d\n", rcv_msg_hdr->src);
	if(rcv_msg_hdr->dest == MAC_BROADCAST_ADDRESS){

		// Nived.Sivadas - changing interfaces with new dll design
		(*appHandler)(m_receive_buffer.GetNumberMessagesInBuffer());
		//(*appHandler)(msg->GetPayload(), Size- sizeof(IEEE802_15_4_Header_t), rcv_msg_hdr->src,FALSE,rcv_meta->GetRssi(), rcv_meta->GetLqi());
		//HandleBroadcastMessage(msg);
	}else if(rcv_msg_hdr->dest == CPU_Radio_GetAddress(this->radioName)){
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
#ifdef DEBUG_CSMAMAC
	Message_15_4_t* temp = (Message_15_4_t *)msg;
	UINT8* rcv_payload =  temp->GetPayload();
#endif
	switch(status)
	{
		case NO_Success:
			{
				//gHalTimerManagerObject.StopTimer(3);
				DEBUG_PRINTF_CSMA("Success <%d> #%d\r\n", (int)rcv_payload[0],((int)(rcv_payload[1] << 8) + (int)rcv_payload[2]));
				//VirtTimer_Stop(VIRT_TIMER_MAC_FLUSHBUFFER);
				SendAckFuncPtrType appHandler = AppHandlers[CurrentActiveApp]->SendAckHandler;
				(*appHandler)(msg, Size, status);
				// Attempt to send the next packet out since we have no scheduler
				if(!m_send_buffer.IsBufferEmpty())
					SendFirstPacketToRadio(NULL);
			}
			break;
		
		case NO_Busy:
			//TODO: when resend is called, packet should be placed at front of buffer. Right now it is at end of buffer.
			DEBUG_PRINTF_CSMA("Resending <%d> #%d\r\n", (int)rcv_payload[0],((int)(rcv_payload[1] << 8) + (int)rcv_payload[2]));
			Resend(msg, Size);
			//gHalTimerManagerObject.StartTimer(3);
			VirtTimer_Start(VIRT_TIMER_MAC_FLUSHBUFFER);
			flushTimerRunning = true;
			break;
			
		default:
			DEBUG_PRINTF_CSMA("Error #%d\r\n",((int)(rcv_payload[1] << 8) + (int)rcv_payload[2]));
			break;
	}
	

	RadioAckPending=FALSE;
	RadioLockUp=0;
}

UINT8 csmaMAC::GetBufferSize(){
	return m_send_buffer.Size();
}

