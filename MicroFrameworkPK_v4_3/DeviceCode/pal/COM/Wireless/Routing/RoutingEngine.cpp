#include <Samraksh\RoutingEngine.h>

extern HALTimerManager gHalTimerManagerObject; //ref: \MicroFrameworkPK_v4_3\DeviceCode\pal\COM\Wireless\MAC\CSMAMAC\csmaMAC.cpp

UINT32 beacon_id = 0;
int send_beacon;
int send_beacon_check;
int send_data;
int send_data_fail;
int rec_data;
int rec_beacon;
int relay_data;
int relay_data_fail;

void  RoutingEngine::SendAckCallback(void *msg, UINT16 size, NetOpStatus status){

}

void RoutingEngine::RecieveCallback(UINT16 numberOfPacketsInBuffer)
{
	
	Message_15_4_t msg;
	Message_15_4_t* msgPointer = &msg;
	for(UINT16 i = 0; i < numberOfPacketsInBuffer; i++)
	{
		/*if(Mac_GetNextWholePacket(&msgPointer) != DS_Success)
		{
			failureToExtract++;
			//hal_printf("RECEIVE ERROR: Failed to Extract this packet");
			continue;
		}*/
		if(msgPointer->GetHeader()->type==CtpMessageType::BEACON){
			rec_beacon++;
			CtpRoutingBeaconMessage beacon;
			memcpy(&beacon, msgPointer->GetPayload(), sizeof(CtpRoutingBeaconMessage));
			hal_printf("\r\nReceived Beacon# %d from Node# %d.\n",beacon.SeqNo,beacon.Sender);
			if(_isRootNode)
				return;
			Receive(&beacon, msgPointer->GetHeader()->src);
		}
		else if(msgPointer->GetHeader()->type==CtpMessageType::DATA){
			rec_data++;
			CtpDataMessage data;
			memcpy(&data, msgPointer->GetPayload(), sizeof(CtpDataMessage));
			hal_printf("\r\nReceived Data# %d from Node# %d.\n",data.SeqNo,data.Sender);
			if(_isRootNode)
				return;
			//Relay the packet as soon as possible
			if(SendData(data)!=DS_Success)
				relay_data_fail++;
			else
				relay_data++;
		}
	}
}

//To be implemented
void RoutingEngine::NeigbhbourChangeCallback(INT16 arg1)
{

}

//MAC Initialize
DeviceStatus RoutingEngine::InitializeMacLayer(UINT8 mac, UINT8 radio)
{
	CPU_SPI_Initialize();
	RoutingEngine::MacID = mac;
	
	MacConfig config;

	// Pass the csma macname id because thats what we are using
	config.CCA = TRUE;
	config.NumberOfRetries = 0;
	config.CCASenseTime = 140;
	config.BufferSize = 8;
	config.RadioID = radio;
	config.NeighbourLivelinessDelay = 300;

	Event_Handler.SetRecieveHandler(&RecieveCallback);
	Event_Handler.SetSendAckHandler(&SendAckCallback);
	Event_Handler.SetNeighbourChangeHandler(&NeigbhbourChangeCallback);

	UINT8 MyAppID=3; //pick a number less than MAX_APPS currently 4

	if(Mac_Initialize(&Event_Handler, RoutingEngine::MacID, MyAppID, config.RadioID, (void*) &config) != DS_Success)
		return DS_Fail;

	//Turned off for the time being --April 2014
	/*if(CPU_Radio_ChangeTxPower(1, 0x0) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeChannel(1, 0xF) != DS_Success)
		return DS_Fail;

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24,FALSE);*/

	return DS_Success;
}

void RoutingEngine::routeRefreshCycle(void* arg){
	send_beacon_check++;
	DeviceStatus ds;
	/*if(msgqueue.datapoint==1){
		ds = SendData(msgqueue);
		if(ds==DS_Success){
			//hal_printf("\r\nSent packet# %d\n",send_data);
			send_data++;
		}
		else
			send_data_fail++;
		msgqueue.datapoint=-1;
	}
	else{*/
		ds = SendBeacon();
		if(ds==DS_Success){
			send_beacon++;
			hal_printf("\r\t\t\t\nSent Beacon# %d.\n",beacon_id-1);
		}
	//}
}

DeviceStatus RoutingEngine::InitializeRoutingLayer(CtpRoutingBeaconMessage *beacon, Ctp_Addr_T routingAddress, UINT8 radio)
{
	lastPacketID = 0;
	failureToExtract = 0;
	missedResponses = 0;
	
	//_beaconSequenceNumber = 0;
	//_prevNeighborSentID = 0;
	
	_beacon = beacon;
	_isRootNode = routingAddress==0;
	_self = routingAddress;
	_beacon->Sender = _self;
	_parentIndex = _isRootNode?0:-1;
	
	send_data = 0;
	send_data_fail = 0;
	send_beacon_check = 0;
	send_beacon = 0;
	rec_data = 0;
	rec_beacon = 0;
	relay_data = 0;
	relay_data_fail = 0;

	//Initialize routing and cadidate tables
	/*for (int i = 0; i < LE_CANDIDATE_TABLE_SIZE; i++){
		_candidateTable[i].State = 0;
	}*/
	
	for (int i = 0; i < RE_TABLE_SIZE; i++){
		_routingTable[i].State = TableEntryState::Invalid;
	}
	
	//msgqueue.datapoint = -1;
	
	//Initialize CSMAMAC 
	UINT8 mac = 0;
	if(InitializeMacLayer(mac, radio) != DS_Success)
	{
		hal_printf("\r\nMAC ERROR: Mac initialization failed\n");
		return DS_Fail;
	}
	hal_printf("\r\nSUCCESS: MAC iniliatization succeeded\n");
	
	//CPU_Radio_ChangeTxPower(radio,0x01);
	//CPU_Radio_ChangeChannel(radio,11);
	
	//Initialize local timer
	/*gHalTimerManagerObject.Initialize();
	if(!gHalTimerManagerObject.CreateTimer(4, 0, 4000000, FALSE, FALSE, routeRefreshCycle)){ //4-sec Timer in micro seconds
		hal_printf("\r\nERROR: Beacon Timer initialization failed\n");
		return DS_Fail;
	}
	
	gHalTimerManagerObject.StartTimer(4);*/
	return DS_Success;
}

DeviceStatus RoutingEngine::SendData(CtpDataMessage msg)
{	
	if(_parentIndex==-1)
		return DS_Fail;
	char data[sizeof(CtpDataMessage)];
    memcpy(data, &msg, sizeof(CtpDataMessage));
	
	DeviceStatus ds;
	//If successful, send message to LE.
	ds = Send(CtpMessageType::DATA, _routingTable[_parentIndex].NeighborMac, data, sizeof(CtpDataMessage));
    if(ds==DS_Success)
		send_data++;
	return ds;
}

//Sends beacon irrespective of whether it has a valid routing path.
DeviceStatus RoutingEngine::SendBeacon()
{
	RefreshParent();
	_beacon->SeqNo = beacon_id++;
	//Try setting routing frame
	DeviceStatus error = SetFrame(&_beacon->Frame);
	_beacon->Sender = _self;
	//Try adding link estimator header and footer
	
	//DeviceStatus error2 = SetLEHeaderAndFooter(_beacon);
	
	//Omitted &error2 --DJR
	if (error != DS_Success){
		hal_printf("SEND BEACON ERROR: Set frame failed");
		return DS_Fail;
	}
	char data[sizeof(CtpRoutingBeaconMessage)];
    memcpy(data, _beacon, sizeof(CtpRoutingBeaconMessage));
	
	//If successful, send message to LE.
	return Send(CtpMessageType::BEACON, 0xffff, data, sizeof(CtpRoutingBeaconMessage));

}

void RoutingEngine::RefreshParent()
{
	if (_isRootNode)
	{
		return;
	}

	//Else try finding a better neighbor
	int bestNeighbor = FindBestNeighbor();
	
	
	
	//In case parent was and remains invalid, or is valid and remains unchanged.
	if (bestNeighbor==_parentIndex) {
		Neighbor_t neighborstats;
			/*if(Mac_GetNeighbourStatus(_routingTable[bestNeighbor].NeighborMac,&neighborstats) != DS_Success){
					_parentIndex=-1;
					return;
			}*/
	}
	
	//Parent was invalid, will become valid
	/*if (bestNeighbor >= 0 && _parentIndex < 0)
	{
		_parentIndex = bestNeighbor;
		//ValidParentFound(_routingTable[_parentIndex].Neighbor);
		//_linkEstimator->PinNeighbor(_routingTable[_parentIndex].Neighbor);
		return;
	}*/

	//Parent was valid, will become invalid
	if (_parentIndex >= 0 && bestNeighbor < 0)
	{
		if (_routingTable[_parentIndex].State == TableEntryState::Invalid)
		{
			//_linkEstimator->UnpinNeighbor(_routingTable[_parentIndex].Neighbor);
			_parentIndex = -1;
			//RouteIsInvalid();
		}
	}
	else
	{
		_parentIndex = bestNeighbor;
	}
	
	//Parent was valid, will change.
	/*if (bestNeighbor >= 0 && _parentIndex >= 0)
	{
		_linkEstimator->UnpinNeighbor(_routingTable[_parentIndex].Neighbor);
		_parentIndex = bestNeighbor;
		_linkEstimator->PinNeighbor(_routingTable[_parentIndex].Neighbor);
		//ValidParentFound(_routingTable[_parentIndex].Neighbor);
	}*/
}


int RoutingEngine::FindBestNeighbor()
{
	Ctp_Addr_T parentAddress = MAC_INVALID_ADDRESS;

	//int bestTotalIndex = -1;
	int bestMultihopIndex = -1;
	Etx_T bestTotalEtx = CTP_THEORETICAL_MAX_ETX;
	Etx_T bestMultihopEtx = CTP_THEORETICAL_MAX_ETX;

	for (int i = 0; i < RE_TABLE_SIZE; i++)
	{
		if (_routingTable[i].State == TableEntryState::Invalid)
			continue;
		
		RoutingTableEntry *entry = &_routingTable[i];

		if (entry->MultiHopETX < bestMultihopEtx)
		{
			bestMultihopEtx = _routingTable[i].MultiHopETX;
			bestMultihopIndex = i;
		}
		
		
		/*Etx_T AvgRSSI;
		Neighbor_t neighborstats;
		if(Mac_GetNeighbourStatus(entry->NeighborMac,&neighborstats) != DS_Success)
		{
			hal_printf("MAC WARNING: Neighbor %d no longer exists",entry->Neighbor);
			continue;
		}

		AvgRSSI = neighborstats.ReverseLink.AvgRSSI;

		Etx_T totalEtx = AvgRSSI + entry->MultiHopETX;
		if (totalEtx < bestTotalEtx)
		{
			bestTotalIndex = i;
			bestTotalEtx = totalEtx;
		}*/
	}

	return bestMultihopIndex;
}

int RoutingEngine::FindWorstNeighbor()
{
	Etx_T worstEtx = 0;
	int worstIndex = -1;
	for (int i = 0; i < RE_TABLE_SIZE; i++)
	{
		if (_routingTable[i].State == TableEntryState::Invalid)
			return i;
		if (_routingTable[i].MultiHopETX > worstEtx)
		{
			worstEtx = _routingTable[i].MultiHopETX;
			worstIndex = i;
		}
	}
	return worstIndex;
}


int RoutingEngine::FindNeighbor(Ctp_Addr_T neighbor)
{
	int index = -1;
	for (int i = 0; i < RE_TABLE_SIZE; i++)
	{
		if (_routingTable[i].Neighbor == neighbor && _routingTable[i].State==TableEntryState::Valid)
			index = i;
	}
	return index;
}

int RoutingEngine::FindEmptyNeighbor()
{
	int index = -1;
	for (int i = 0; i < RE_TABLE_SIZE; i++)
	{
		if (_routingTable[i].State == TableEntryState::Invalid)
		{
			index = i;
			break;
		}
	}
	return index;
}

DeviceStatus RoutingEngine::SetFrame(CtpRoutingFrame *frame)
{
	if (_isRootNode)
	{
		frame->Parent = _self;
		frame->MultihopEtx = 0;
		return DS_Success;
	}

	if (_parentIndex < 0 ||
		_routingTable[_parentIndex].State == TableEntryState::Invalid)
	{
		frame->Parent = MAC_INVALID_ADDRESS;
		frame->MultihopEtx = CTP_THEORETICAL_MAX_ETX;
		return DS_Success;
	}

	RoutingTableEntry *entry = &_routingTable[_parentIndex];
	frame->Parent = entry->Neighbor;
	frame->MultihopEtx = entry->MultiHopETX;
	
	//Etx_T AvgRSSI;
	/*Neighbor_t neighborstats;
	if(Mac_GetNeighbourStatus(entry->NeighborMac,&neighborstats) != DS_Success)
	{
		//Remove parent???
		frame->Parent = MAC_INVALID_ADDRESS;
		frame->MultihopEtx = CTP_THEORETICAL_MAX_ETX;
		//AvgRSSI = LE_ETX_THRESHOLD;	//Assume it to be maximum ETX allowed
	}
	else
	{
		//AvgRSSI = ComputeEtx(neighborstats.ReverseLink.AvgRSSI,sizeof(CtpRoutingBeaconMessage));
		frame->MultihopEtx = entry->MultiHopETX;
	}*/

	return DS_Success;
}

DeviceStatus RoutingEngine::GetRouteQuality(Ctp_Addr_T neighbor, Etx_T *etx)
{
	int index = FindNeighbor(neighbor);
	if (index < 0)
		return DS_Fail;

	(*etx) = _routingTable[index].MultiHopETX;
	return DS_Success;
}

DeviceStatus RoutingEngine::GetParentEtx(Etx_T *etx)
{
	if (_isRootNode)
	{
		*etx = 0;
		return DS_Success;
	}

	if (_parentIndex < 0)
		return DS_Fail;

	(*etx) = _routingTable[_parentIndex].MultiHopETX;
	return DS_Success;
}

DeviceStatus RoutingEngine::GetParent(Ctp_Addr_T* address)
{
	if (_isRootNode)
	{
		*address = _self;
		return DS_Success;
	}

	if (_parentIndex < 0)
		return DS_Fail;

	(*address) = _routingTable[_parentIndex].Neighbor;
	return DS_Success;
}

CtpRoutingBeaconMessage* RoutingEngine::Receive(
	CtpRoutingBeaconMessage *beacon,
	UINT16 sourceMacAddres)
{
	CtpRoutingFrame* frame = &beacon->Frame;
	CtpRoutingBeaconMessage* temp = beacon;
	
	//The node sending the beacon doesn't have a valid route
	if (frame->Parent == MAC_INVALID_ADDRESS ||
		frame->MultihopEtx == CTP_THEORETICAL_MAX_ETX)
		return temp;
	
	Neighbor_t neighborstats;
	/*if(Mac_GetNeighbourStatus(sourceMacAddres,&neighborstats) != DS_Success){
			hal_printf("ERROR: Message from non-existent neighbor.");
			return temp;
	}*/
	
	//Has a valid route
	int index = FindNeighbor(beacon->Sender);
	if (index < 0)
	{
		TryInsertNeighbor(beacon->Sender, frame->MultihopEtx, sourceMacAddres);
		index = FindNeighbor(beacon->Sender);
		if(index < 0)
			return temp;
	}
	
	RoutingTableEntry *entry = &_routingTable[index];
	entry->Neighbor = beacon->Sender;
	entry->NeighborMac = sourceMacAddres;
	entry->Parent = frame->Parent;
	entry->MultiHopETX = frame->MultihopEtx + 1;//TODO:ComputeEtx(neighborstats.ReverseLink.AvgRSSI,sizeof(CtpRoutingBeaconMessage));
	entry->State |= TableEntryState::Valid;

	return temp;
}


//If routing engine is tracking neighbor, set compare bit.
//TO2DO: Verify this is correct.
bool RoutingEngine::GetCompareBit(Ctp_Addr_T neighbor)
{
	int index = FindNeighbor(neighbor);
	if (index >= 0 && _routingTable[index].State == TableEntryState::Valid)
		return true;

	return false;
}

void RoutingEngine::InitNeighborEntry(int index, Ctp_Addr_T neighbor, UINT16 sourceMacAddres)
{
	//ClearNeighborEntry(index);
	/*RoutingTableEntry *entry = &(_routingTable[index]);
	entry->State = TableEntryState::Valid;
	entry->Neighbor = neighbor;
	entry->NeighborMac = sourceMacAddres;*/
	_routingTable[index].State = TableEntryState::Valid;
	_routingTable[index].Neighbor = neighbor;
	_routingTable[index].NeighborMac = sourceMacAddres;
}

void RoutingEngine::ClearNeighborEntry(int index)
{
	//RoutingTableEntry *entry = &_routingTable[index];
	/*entry->State = TableEntryState::Invalid;
	entry->MultiHopETX = CTP_THEORETICAL_MAX_ETX;
	entry->Neighbor = entry->Parent = MAC_INVALID_ADDRESS;*/
	_routingTable[index].State = TableEntryState::Invalid;
	_routingTable[index].MultiHopETX = CTP_THEORETICAL_MAX_ETX;
	_routingTable[index].Neighbor = _routingTable[index].Parent = MAC_INVALID_ADDRESS;
}

void RoutingEngine::TryInsertNeighbor(Ctp_Addr_T neighbor, Etx_T multihopEtx, UINT16 sourceMacAddres)
{
	int index = FindEmptyNeighbor();

	if (index < 0)
	{
		int worstIndex = index = FindWorstNeighbor();
		Etx_T worstEtx = _routingTable[worstIndex].MultiHopETX;
		if (worstEtx > multihopEtx)
			index = worstIndex;
	}
	
	
	//Either empty neighbor was found, or the etx was better than worst neighbor
	if (index >= 0)
		InitNeighborEntry(index, neighbor, sourceMacAddres);
}

/****Link layer basic functions -- Dhrubo****/
DeviceStatus RoutingEngine::Send(CtpMessageType::Values type, UINT16 mac_addr, char* payload, UINT16 size)
{
	
	//CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	if(Mac_Send(MacID, mac_addr, type, (void*) payload, size) != DS_Success)
	{
		hal_printf("\r\nSEND ERROR: MAC Layer Send failed\n");
		return DS_Fail;
	}
	//DID_STATE_CHANGE(SendAckPending, "Did not recieve send ack from radio");
	//CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
	//DID_STATE_CHANGE(ResponsePending, "Did not recieve response from slave\n");

	//SendAckPending = TRUE;
	//ResponsePending = TRUE;
	//hal_printf("\r\nSEND SUCCESSFUL: Beacon Send succeeded\n");
	return DS_Success;
}

/*void RoutingEngine::ProcessReceivedBeacon(
	CtpRoutingBeaconMessage* msg, 
	Ctp_Addr_T src,
	UINT8 snr)
{
	//Ctp_SeqNo_T seqNo = msg->Header.SeqNo;
	//UINT8 footerCount = msg->Header.FooterCount;
	
	UINT8 neighborStatus[25];
	if(Mac_GetNeighbourStatus(src, neighborStatus) != DS_Success){
		hal_printf("LINK ESTIMATOR ERROR: Received message from non-existing neighbor; discarding");
		return;
	}
	
	\*int candidateIndex = FindCandidateIndex(src);

	if (candidateIndex < 0)
	{
		InsertCandidate(src);
		candidateIndex = FindCandidateIndex(src);
	}
	else
	{
		CandidateTableEntry *entry = &_candidateTable[candidateIndex];
		AddCandidateSnrFrame(entry, snr, sizeof(IEEE802_15_4_Header_t) + sizeof(CtpRoutingBeaconMessage));
		candidateIndex = FindCandidateIndex(src);
	}

	if (candidateIndex < 0 && neighborIndex >= 0)
	{
		NeighborTableEntry* entry = &_neighborTable[neighborIndex];
		AddNeighborBeaconSequence(entry, seqNo);
		AddNeighborSnrFrame(
			entry,
			snr,
			_macDevice->MAC_DATA + sizeof(CtpRoutingBeaconMessage));
		entry->LastPacketAge = 0;
	}
	
	Ctp_Addr_T selfAddress = _macDevice->get_deviceID();
	for (int i = 0; i < msg->Header.FooterCount; i++)
	{
		if (msg->Footer[i].Neighbor == selfAddress)
		{
			UpdateForwardAvgRSSI(src, msg->Footer[i].LinkEtx);
			break;
		}
	}*\
}*/

/*To implement*/
Etx_T RoutingEngine::ComputeEtx(UINT16 snr, UINT8 frameSizeInBytes)
{
	float realSnr = snr / 10;
	float prr = pow(1 - 0.5 * exp(-0.5 * snr), 8 * frameSizeInBytes);
	float etx = 1 / prr;
	UINT8 roundedEtx = etx * 10;
	return roundedEtx;
}
