/*
 * Neighbors.h : Defines and implements the MAC/Link layer Neighbor Table
 *
 *Created on: Oct 25, 2012
 *Author: Mukundan Sridharan
 *
 */

#ifndef NEIGHBORS_H_
#define NEIGHBORS_H_

#include <Samraksh/Mac_decl.h>
#include <Samraksh\Buffer.h>

#define NeighborChanged 1
#define Received 0

//#include "TinyCLR_Runtime.h"

#define Data_Send_Buffer_15_4_t_SIZE		2
#define TimeSync_Send_Buffer_15_4_t_SIZE 	1

extern UINT8 MacName;
#define MAX_NEIGHBORS 8
#define INVALID_NEIGHBOR_INDEX 255
#define INVALID_MACADDRESS 0

#define NUM_ENFORCED_TSR_PCKTS_BEFORE_DATA_PCKTS 8

//extern void  ManagedCallback(UINT16 arg1, UINT16 arg2);
//#define DEBUG_NEIGHBORTABLE

#if defined(DEBUG_NEIGHBORTABLE)
#define ENABLE_PIN_NB(x,y) CPU_GPIO_EnableOutputPin(x,y)
#define SET_PIN_NB(x,y) CPU_GPIO_SetPinState(x,y)
#define DEBUG_PRINTF_NB(...) hal_printf(__VA_ARGS__)
#else
#define ENABLE_PIN_NB(x,y)
#define SET_PIN_NB(x,y)
#define DEBUG_PRINTF_NB(...) (void)0
#endif

typedef struct {
	UINT8 AvgRSSI;
	UINT8 LinkQuality;
	UINT8 AveDelay;
}Link_t;

enum NeighborStatus {
	Alive = 0,
	Dead = 1,
	//Suspect = 2,
	NbrStatusError = 0xF
};

typedef struct {
	UINT16 MACAddress;
	UINT8 NumTimeSyncMessagesSent;	//Count of timesync packets sent per neighbor
	bool IsInitializationTimeSamplesNeeded(){
		if(NumTimeSyncMessagesSent < NUM_ENFORCED_TSR_PCKTS_BEFORE_DATA_PCKTS) return true;
		else return false;
	}
	void IncrementNumTimeSyncMessagesSent(){
		if( IsInitializationTimeSamplesNeeded() ) {
			++(NumTimeSyncMessagesSent);
			if(NumTimeSyncMessagesSent == NUM_ENFORCED_TSR_PCKTS_BEFORE_DATA_PCKTS){
				++(NumTimeSyncMessagesSent);
			}
		}

	}
	//Send (formerly forward) link details between current and neighbor node
	Link_t SendLink;
	//Receive (formerly reverse) link details between current and neighbor node
	Link_t ReceiveLink;
	NeighborStatus neighborStatus;
	bool IsAvailableForUpperLayers;
	UINT16 CountOfPacketsReceived;
	UINT64 LastHeardTime;
	UINT8 ReceiveDutyCycle; //percentage
	UINT16 FrameLength;

	UINT16  nextSeed; //the seed we have from in the control message last received
	UINT16 mask;
	UINT64  nextwakeupSlot;
	UINT32  seedUpdateIntervalinSlots;

	UINT64  LastTimeSyncRecvTime;			// Lasst time a time sync message is received
	UINT64  LastTimeSyncRequestTime;	// Last time instant a time sync request is sent
	UINT64  LastTimeSyncSendTime;	// Last time instant a time sync is sent(piggbacked)

	UINT8 random_back_off_slot;
	UINT8 random_back_off_window_size;
	//UINT8   numErrors;
	//UINT8   size;
	//BOOL    isInTransition;
	//UINT32  localAvg;
	//INT32   offsetAvg;
	//float   skew;
	//TODO: BK: DELETE THESE NOT USED BUT KEPT FOR THE TIME BEIGN
	UINT16  radioStartDelay;
	UINT16  counterOffset;

	Buffer_15_4<Data_Send_Buffer_15_4_t_SIZE> send_buffer;
	Buffer_15_4<TimeSync_Send_Buffer_15_4_t_SIZE> tsr_send_buffer;
}Neighbor_t;

class NeighborTableCommonParameters_One_t{
public:
	UINT16 MACAddress;
	NeighborStatus status;
	UINT64 lastHeardTime;
	Link_t linkQualityMetrics;
	NeighborTableCommonParameters_One_t(){
		MACAddress = 0;
		status = NbrStatusError;
		lastHeardTime = 0;
		linkQualityMetrics.LinkQuality = 0;
		linkQualityMetrics.AvgRSSI = 0;
		linkQualityMetrics.AveDelay = 0;
	}
};

class NeighborTableCommonParameters_Two_t{
public:
	UINT16  nextSeed; //the seed we have from in the control message last received
	UINT16 	mask;
	UINT64  nextwakeupSlot;
	UINT32  seedUpdateIntervalinSlots;
	NeighborTableCommonParameters_Two_t(){
		nextSeed = 0;
		mask = 0;
		nextwakeupSlot = 0;
		seedUpdateIntervalinSlots = 0;
	}
};

class NeighborTable {
public:
	UINT8 NumberValidNeighbor;
	UINT8 PreviousNumberValidNeighbor;
	Neighbor_t Neighbor[MAX_NEIGHBORS];

	// neighbor table util functions
	DeviceStatus GetFreeIdx(UINT8* index);
	DeviceStatus ClearNeighbor(UINT16 MACAddress);
	DeviceStatus ClearNeighborwIndex(UINT8 tableIndex);
	DeviceStatus FindIndexEvenDead(const UINT16 MACAddress, UINT8* index);
	DeviceStatus FindIndex(const UINT16 MACAddress, UINT8* index);
	void ClearTable();
	UINT8 BringOutYourDead(UINT32 delay);
	Neighbor_t* GetNeighborPtr(UINT16 address);
	UINT8 NumberOfNeighbors();
	UINT8 PreviousNumberOfNeighbors();
	void SetPreviousNumberOfNeighbors(UINT8 previousNeighborCnt);
	DeviceStatus InsertNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t, const NeighborTableCommonParameters_Two_t *neighborTableCommonParameters_Two_t);
	DeviceStatus FindOrInsertNeighbor(const UINT16 address, UINT8* index);
	DeviceStatus UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink, UINT8* index);
	DeviceStatus UpdateFrameLength(UINT16 address, NeighborStatus status, UINT16 frameLength, UINT8* index);
	DeviceStatus UpdateDutyCycle(UINT16 address, UINT8 dutyCycle, UINT8* index);
	DeviceStatus UpdateNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t, const NeighborTableCommonParameters_Two_t *neighborTableCommonParameters_Two_t);
	//DeviceStatus UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16  lastSeed, UINT16  dataInterval, UINT16  radioStartDelay, UINT16  counterOffset, UINT8* index);
	DeviceStatus UpdateNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t);
	UINT8  UpdateNeighborTable(UINT64 livelinessDelayInTicks, UINT64 currentTime);
	UINT8  UpdateNeighborTable(UINT32 NeighborLivenessDelay);

	DeviceStatus RecordTimeSyncRequestSent(UINT16 address, UINT64 _LastTimeSyncTime);
	DeviceStatus RecordTimeSyncSent(UINT16 address, UINT64 _LastTimeSyncTime);
	DeviceStatus RecordTimeSyncRecv(UINT16 address, UINT64 _lastTimeSyncRecv);
	UINT64 GetLastTimeSyncRecv(UINT16 address);
	Neighbor_t* GetMostObsoleteTimeSyncNeighborPtr(NeighborStatus ns = Alive);
	//Neighbor_t* GetNeighborWOldestSyncPtr(const UINT64& curticks, const UINT64& request_limit);
	Neighbor_t* GetCritalSyncNeighborWOldestSyncPtr(const UINT64& curticks, const UINT64& request_limit,const UINT64& forcererequest_limit, const UINT64& fast_disco_request_interval);
	void DegradeLinks();
	UINT16 GetMaxNeighbors();

	DeviceStatus RecordLastHeardTime(UINT16 MACAddress, UINT64 currTime);

};

DeviceStatus NeighborTable::RecordLastHeardTime(UINT16 MACAddress, UINT64 currTime){
	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(MACAddress, &index);

	if ( (retValue==DS_Success) && (MACAddress != 0 || MACAddress != 65535)){
		Neighbor[index].LastHeardTime = currTime;
		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}

UINT16 NeighborTable::GetMaxNeighbors(void){
	return MAX_NEIGHBORS;
}

UINT8 NeighborTable::UpdateNeighborTable(UINT64 livelinessDelayInTicks, UINT64 currentTime)
{
	UINT8 deadNeighbors = 0;

//	UINT64 livelinessDelayInTicks = CPU_MillisecondsToTicks(NeighborLivenessDelay * 1000);
//

	//if (Neighbor[0].Status == Alive)
	//	hal_printf("neighbor 0 last time: %lld\tcurrent time: %lld\tlivelinessDelayinticks: %lld\r\n", Neighbor[0].LastHeardTime,  currentTime, livelinessDelayInTicks);

	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if((Neighbor[i].neighborStatus == Alive) && ((currentTime - Neighbor[i].LastHeardTime) > livelinessDelayInTicks) && (Neighbor[i].LastHeardTime != 0))
		{

			DEBUG_PRINTF_NB("[NATIVE] Neighbors.h : Removing inactive neighbor %hu \n", Neighbor[i].MACAddress);
			Neighbor[i].neighborStatus = Dead;
			++deadNeighbors;
			--NumberValidNeighbor;
		}
	}

	return deadNeighbors;
}

Neighbor_t* NeighborTable::GetMostObsoleteTimeSyncNeighborPtr(NeighborStatus ns){
	Neighbor_t* rn = NULL;
	int tableIndex;
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].neighborStatus == ns){
			if(rn == NULL) {
				rn = &Neighbor[tableIndex];
			}
			else if( Neighbor[tableIndex].LastHeardTime != 0 && rn->neighborStatus <= Neighbor[tableIndex].LastHeardTime){
				rn = &Neighbor[tableIndex];
			}
		}
	}
	return rn;
}


UINT8 NeighborTable::UpdateNeighborTable(UINT32 NeighborLivenessDelay)
{
	return BringOutYourDead(NeighborLivenessDelay);
}

DeviceStatus NeighborTable::FindIndexEvenDead(const UINT16 MACAddress, UINT8* index){
	int tableIndex;

	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if ( (Neighbor[tableIndex].MACAddress == MACAddress)) {
			*index = tableIndex;
			return DS_Success;
		}
	}
	return DS_Fail;
}

DeviceStatus NeighborTable::FindIndex(const UINT16 MACAddress, UINT8* index){
	int tableIndex;
	
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if ( (Neighbor[tableIndex].MACAddress == MACAddress) && (Neighbor[tableIndex].neighborStatus == Alive)) {
			*index = tableIndex;
			return DS_Success;
		}
	}
	return DS_Fail;
}

UINT8 NeighborTable::BringOutYourDead(UINT32 delay){

//	GLOBAL_LOCK(irq);

	UINT8 deadNeighbors = 0;

	UINT64 livelinessDelayInTicks = CPU_MillisecondsToTicks(delay * 1000);

	UINT64 currentTime = HAL_Time_CurrentTicks();


	//if (Neighbor[0].Status == Alive)
	//	hal_printf("neighbor 0 last time: %lld\tcurrent time: %lld\tlivelinessDelayinticks: %lld\r\n", Neighbor[0].LastHeardTime,  currentTime, livelinessDelayInTicks);

	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if((Neighbor[i].neighborStatus == Alive) && ((currentTime - Neighbor[i].LastHeardTime) > livelinessDelayInTicks) && (Neighbor[i].LastHeardTime != 0))
		{
			DEBUG_PRINTF_NB("[NATIVE] Neighbors.h : Removing Neighbor %hu due to inactivity\n", Neighbor[i].MACAddress);
			Neighbor[i].neighborStatus = Dead;
			deadNeighbors++;
			NumberValidNeighbor--;
		}
	}

	return deadNeighbors;
}

DeviceStatus NeighborTable::ClearNeighbor(UINT16 nodeId){
	UINT8 tableIndex;
	if (FindIndex(nodeId, &tableIndex) == DS_Success){
		return ClearNeighborwIndex(tableIndex);
	} else {
		return DS_Fail;
	}
}

DeviceStatus NeighborTable::ClearNeighborwIndex(UINT8 tableIndex){
	Neighbor[tableIndex].neighborStatus = Dead;
	Neighbor[tableIndex].MACAddress = INVALID_MACADDRESS;
	Neighbor[tableIndex].NumTimeSyncMessagesSent = 0;
	Neighbor[tableIndex].SendLink.AvgRSSI = 0;
	Neighbor[tableIndex].SendLink.LinkQuality = 0;
	Neighbor[tableIndex].SendLink.AveDelay = 0;
	Neighbor[tableIndex].ReceiveLink.AvgRSSI = 0;
	Neighbor[tableIndex].ReceiveLink.LinkQuality = 0;
	Neighbor[tableIndex].ReceiveLink.AveDelay = 0;

	Neighbor[tableIndex].CountOfPacketsReceived = 0;
	Neighbor[tableIndex].LastHeardTime = 0;
	Neighbor[tableIndex].ReceiveDutyCycle = 0; //percentage
	Neighbor[tableIndex].FrameLength = 0;


	Neighbor[tableIndex].nextSeed = 0;
	Neighbor[tableIndex].mask = 0;
	Neighbor[tableIndex].nextwakeupSlot = 0;
	Neighbor[tableIndex].seedUpdateIntervalinSlots = 0;


	Neighbor[tableIndex].LastTimeSyncRecvTime = 0;
	Neighbor[tableIndex].LastTimeSyncRequestTime = 0;
	Neighbor[tableIndex].LastTimeSyncSendTime = 0;

	Neighbor[tableIndex].random_back_off_slot = 0;
	Neighbor[tableIndex].random_back_off_window_size = 1;

	Neighbor[tableIndex].send_buffer.Initialize();
	Neighbor[tableIndex].tsr_send_buffer.Initialize();


	NumberValidNeighbor--;
	return DS_Success;
}
void NeighborTable::ClearTable(){
	int tableIndex;
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		ClearNeighborwIndex(tableIndex);
	}
	NumberValidNeighbor = 0;
}

// neighbor table util functions
DeviceStatus NeighborTable::GetFreeIdx(UINT8* index){
	DeviceStatus rv = DS_Fail;
	int tableIndex;

	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if( Neighbor[tableIndex].MACAddress == INVALID_MACADDRESS){
			*index = tableIndex;
			rv = DS_Success;
			break;
		}
		else if (Neighbor[tableIndex].neighborStatus == Dead){
			*index = tableIndex;
		}
	}
	if(Neighbor[*index].MACAddress != INVALID_MACADDRESS){
		ClearNeighborwIndex(*index);
	}
	return rv;
}

Neighbor_t* NeighborTable::GetNeighborPtr(UINT16 address){
	UINT8 index;
	
	if (FindIndex(address, &index) != DS_Success){
		return NULL;
	}else {
		return &Neighbor[index];
	}
}

UINT8 NeighborTable::NumberOfNeighbors(){
	//return NumberValidNeighbor;
	UINT8 numneigh = 0;
	for (UINT8 tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].neighborStatus == Alive){
			++numneigh;
		}
	}
	return numneigh;
}

UINT8 NeighborTable::PreviousNumberOfNeighbors(){
	return PreviousNumberValidNeighbor;
}

void NeighborTable::SetPreviousNumberOfNeighbors(UINT8 previousNeighborCnt){
	PreviousNumberValidNeighbor = previousNeighborCnt;
}

DeviceStatus NeighborTable::FindOrInsertNeighbor(const UINT16 address, UINT8* index){
	DeviceStatus retValue = DS_Fail;
	if(address != 0 || address != 65535){
		retValue = FindIndexEvenDead(address, index);
		if(retValue == DS_Fail) {
			retValue = GetFreeIdx(index);
			Neighbor[*index].MACAddress = address;
			Neighbor[*index].NumTimeSyncMessagesSent = 0;
			DEBUG_PRINTF_NB("[NATIVE] Neighbors.h : Inserting Neighbor %hu.\n", address);
		}
		else{
			if(Neighbor[*index].neighborStatus != Alive){
				Neighbor[*index].NumTimeSyncMessagesSent = 0;
			}
		}
	}
	return retValue;
}

DeviceStatus NeighborTable::InsertNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t, const NeighborTableCommonParameters_Two_t *neighborTableCommonParameters_Two_t){
	UINT16 address = neighborTableCommonParameters_One_t->MACAddress;
	UINT8 index;
    DeviceStatus retValue = FindOrInsertNeighbor(address, &index);

	if (retValue == DS_Success && (address != 0 || address != 65535)){
		NumberValidNeighbor++;
		Neighbor[index].ReceiveLink.AvgRSSI =  0;
		Neighbor[index].ReceiveLink.LinkQuality =  0;
		Neighbor[index].ReceiveLink.AveDelay =  0;
		Neighbor[index].SendLink.AvgRSSI =  0;
		Neighbor[index].SendLink.LinkQuality =  0;
		Neighbor[index].SendLink.AveDelay =  0;
		retValue = UpdateNeighbor(neighborTableCommonParameters_One_t, neighborTableCommonParameters_Two_t);
	}
	return retValue;
}

DeviceStatus NeighborTable::UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink, UINT8* index){
    DeviceStatus retValue = FindIndex(address, index);

	if ((retValue!=DS_Success) && (address != 0 || address != 65535)){
			if(forwardLink != NULL){
				Neighbor[*index].SendLink.AveDelay = forwardLink->AveDelay;
				Neighbor[*index].SendLink.AvgRSSI = forwardLink->AvgRSSI;
				Neighbor[*index].SendLink.LinkQuality = forwardLink->LinkQuality;
			}
			if(reverseLink != NULL){
				Neighbor[*index].ReceiveLink.AveDelay = reverseLink->AveDelay;
				Neighbor[*index].ReceiveLink.AvgRSSI = reverseLink->AvgRSSI;
				Neighbor[*index].ReceiveLink.LinkQuality = reverseLink->LinkQuality;
			}
	}
	return retValue;
}

DeviceStatus NeighborTable::UpdateFrameLength(UINT16 address, NeighborStatus status, UINT16 frameLength, UINT8* index){
    DeviceStatus retValue = FindIndex(address, index);

	if ((retValue!=DS_Success) && (address != 0 || address != 65535)){
			Neighbor[*index].FrameLength = frameLength;
	}
	return retValue;
}

DeviceStatus NeighborTable::UpdateDutyCycle(UINT16 address, UINT8 dutyCycle, UINT8* index){
    DeviceStatus retValue = FindIndex(address, index);

	if ((retValue!=DS_Success) && (address != 0 || address != 65535)){
			Neighbor[*index].ReceiveDutyCycle = dutyCycle;
	}
	return retValue;
}

DeviceStatus NeighborTable::UpdateNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t, const NeighborTableCommonParameters_Two_t *neighborTableCommonParameters_Two_t){
	UINT16 address = neighborTableCommonParameters_One_t->MACAddress;
	NeighborStatus status = neighborTableCommonParameters_One_t->status;
	UINT64 LastHeardTime = neighborTableCommonParameters_One_t->lastHeardTime;
	UINT8 lqi = neighborTableCommonParameters_One_t->linkQualityMetrics.LinkQuality;
	UINT8 rssi = neighborTableCommonParameters_One_t->linkQualityMetrics.AvgRSSI;
	UINT8 delay = neighborTableCommonParameters_One_t->linkQualityMetrics.AveDelay;

	if(status == NbrStatusError){
		return DS_Fail;
	}
	if(LastHeardTime == 0){
		return DS_Fail;
	}

	UINT16 nextSeed = neighborTableCommonParameters_Two_t->nextSeed;
	UINT16 mask = neighborTableCommonParameters_Two_t->mask;
	UINT64 nextwakeupSlot = neighborTableCommonParameters_Two_t->nextwakeupSlot;
	UINT32 seedUpdateIntervalinSlots = neighborTableCommonParameters_Two_t->seedUpdateIntervalinSlots;

	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(address, &index);
	if (retValue == DS_Success && (address != 0 || address != 65535)){
		Neighbor[index].MACAddress = address;
		Neighbor[index].neighborStatus = status;
		Neighbor[index].LastHeardTime = LastHeardTime;
		Neighbor[index].ReceiveLink.AvgRSSI =  (UINT8)((float)Neighbor[index].ReceiveLink.AvgRSSI*0.8 + (float)rssi*0.2);
		Neighbor[index].ReceiveLink.LinkQuality =  (UINT8)((float)Neighbor[index].ReceiveLink.LinkQuality*0.8 + (float)lqi*0.2);

		Neighbor[index].nextSeed = nextSeed;
		Neighbor[index].mask = mask;
		Neighbor[index].nextwakeupSlot = nextwakeupSlot;
		Neighbor[index].seedUpdateIntervalinSlots = seedUpdateIntervalinSlots;
	}
	return retValue;
}

DeviceStatus NeighborTable::UpdateNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t){
	UINT16 address = neighborTableCommonParameters_One_t->MACAddress;
	NeighborStatus status = neighborTableCommonParameters_One_t->status;
	UINT64 LastHeardTime = neighborTableCommonParameters_One_t->lastHeardTime;
	UINT8 lqi = neighborTableCommonParameters_One_t->linkQualityMetrics.LinkQuality;
	UINT8 rssi = neighborTableCommonParameters_One_t->linkQualityMetrics.AvgRSSI;
	UINT8 delay = neighborTableCommonParameters_One_t->linkQualityMetrics.AveDelay;

	if(status == NbrStatusError){
		return DS_Fail;
	}
	if(LastHeardTime == 0){
		return DS_Fail;
	}

	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(address, &index);
	if (retValue == DS_Success && (address != 0 || address != 65535)){
		Neighbor[index].ReceiveLink.AvgRSSI =  (UINT8)((float)Neighbor[index].ReceiveLink.AvgRSSI*0.8 + (float)rssi*0.2);
		Neighbor[index].ReceiveLink.LinkQuality =  (UINT8)((float)Neighbor[index].ReceiveLink.LinkQuality*0.8 + (float)lqi*0.2);
		Neighbor[index].CountOfPacketsReceived++;
		Neighbor[index].LastHeardTime = LastHeardTime;
		Neighbor[index].neighborStatus = status;
		/*
		Neighbor[index].dataInterval = dataInterval;
		Neighbor[index].radioStartDelay = radioStartDelay;
		Neighbor[index].counterOffset = counterOffset;
		Neighbor[index].lastSeed = seed;
		*/
	}

	return retValue;
}

DeviceStatus NeighborTable::RecordTimeSyncRequestSent(UINT16 address, UINT64 _LastTimeSyncTime){
	 UINT8 index;
		DeviceStatus retValue = FindOrInsertNeighbor(address, &index);

	if ( (retValue==DS_Success) && (address != 0 || address != 65535)){
		Neighbor[index].LastTimeSyncRequestTime = _LastTimeSyncTime;
		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}

DeviceStatus NeighborTable::RecordTimeSyncSent(UINT16 address, UINT64 _LastTimeSyncTime){
	 UINT8 index;
		DeviceStatus retValue = FindOrInsertNeighbor(address, &index);


	if ( (retValue==DS_Success) && (address != 0 || address != 65535)){
		Neighbor[index].LastTimeSyncSendTime = _LastTimeSyncTime;
		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}

/*UINT64 NeighborTable::GetLastTimeSyncRequestTime(UINT16 address){ //TODO BK: We should eventually use a class for Neighbor rather than a struct, this would enable protecting variables.
	 UINT8 index;
	 DeviceStatus retValue = FindIndex(address, &index);

	if ( (retValue==DS_Success) && (address != 0 || address != 65535)){
		return(Neighbor[index].LastTimeSyncRequestTime);
	}
	else {
		//Bk: Maybe we should abort or define an uninitialized TimeValue here.
		return 0;
	}
}*/

DeviceStatus NeighborTable::RecordTimeSyncRecv(UINT16 address, UINT64 _LastTimeSyncTime){
	 UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(address, &index);


	if ( (retValue==DS_Success) && (address != 0 || address != 65535)){
		Neighbor[index].LastTimeSyncRecvTime = _LastTimeSyncTime;
		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}

UINT64 NeighborTable::GetLastTimeSyncRecv(UINT16 address){
	 UINT8 index;
	 DeviceStatus retValue = FindIndex(address, &index);

	if ( (retValue==DS_Success) && (address != 0 || address != 65535)){
		return(Neighbor[index].LastTimeSyncRecvTime);
	}
	else {
		return (0);
	}
}





Neighbor_t* NeighborTable::GetCritalSyncNeighborWOldestSyncPtr(const UINT64& curticks, const UINT64& request_limit, const UINT64& forcererequest_limit, const UINT64& fast_disco_request_interval ){
	Neighbor_t* rn = NULL;
	int tableIndex;
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].neighborStatus != Dead){
			//Get neighbor which has to be sent a timesync packet asap
			if(rn == NULL || Neighbor[tableIndex].LastTimeSyncSendTime < rn->LastTimeSyncSendTime || Neighbor[tableIndex].IsInitializationTimeSamplesNeeded() ){ //Consider this neighbor

				if((curticks - Neighbor[tableIndex].LastTimeSyncSendTime > request_limit || curticks - Neighbor[tableIndex].LastTimeSyncRecvTime > forcererequest_limit || Neighbor[tableIndex].IsInitializationTimeSamplesNeeded() )){

					if(Neighbor[tableIndex].LastTimeSyncRequestTime == 0  || curticks - Neighbor[tableIndex].LastTimeSyncRequestTime  > request_limit || curticks - Neighbor[tableIndex].LastTimeSyncRequestTime  > forcererequest_limit ){
						rn = &Neighbor[tableIndex];
					}
					else if(Neighbor[tableIndex].IsInitializationTimeSamplesNeeded() && curticks - Neighbor[tableIndex].LastTimeSyncRequestTime  > fast_disco_request_interval){
						rn = &Neighbor[tableIndex];
					}
				}
			}
		}
	}

	return rn;
}

void NeighborTable::DegradeLinks(){
	UINT8 i=0;
	for (i=0; i < NumberValidNeighbor; i++){
		//Neighbor[index].Status = status;
		/*if(Neighbor[i].ReceiveLink.Quality >2){
				Neighbor[i].ReceiveLink.Quality--;
		}*/
	}
}


#endif /* NEIGHBORS_H_ */
