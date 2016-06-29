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
#define MAX_NEIGHBORS 12
#define INVALID_NEIGHBOR_INDEX 255
#define INVALID_MACADDRESS 0

#define NUM_ENFORCED_TSR_PCKTS_BEFORE_DATA_PCKTS 1

//extern void  ManagedCallback(UINT16 arg1, UINT16 arg2);
//#define DEBUG_NEIGHBORTABLE

#if defined(DEBUG_NEIGHBORTABLE)
#define ENABLE_PIN_NB(x,y) CPU_GPIO_EnableOutputPin(x,y)
#define SET_PIN_NB(x,y) CPU_GPIO_SetPinState(x,y)
#define DEBUG_PRINTF_NB(x) hal_printf(x)
#else
#define ENABLE_PIN_NB(x,y)
#define SET_PIN_NB(x,y)
#define DEBUG_PRINTF_NB(x)(void)0
#endif

typedef struct {
	UINT8 AvgRSSI;
	UINT8 LinkQuality;
	UINT8 AveDelay;
}Link_t;

enum NeighborStatus {
	Alive,
	Dead,
	Suspect
};

typedef struct {
	UINT16 MacAddress;
	UINT8 NumTimeSyncMessagesSent;
	Link_t ForwardLink;
	Link_t ReverseLink;
	NeighborStatus Status;
	UINT16 PacketsReceived;
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

class NeighborTable {
public:
	UINT8 NumberValidNeighbor;
	UINT8 PreviousNumberValidNeighbor;
	Neighbor_t Neighbor[MAX_NEIGHBORS];

	// neighbor table util functions
	DeviceStatus GetFreeIdx(UINT8* index);
	DeviceStatus ClearNeighbor(UINT16 MacAddress);
	DeviceStatus ClearNeighborwIndex(UINT8 tableIndex);
	DeviceStatus FindIndexEvenDead(UINT16 MacAddress, UINT8* index);
	DeviceStatus FindIndex(UINT16 MacAddress, UINT8* index);
	void ClearTable();
	UINT8 BringOutYourDead(UINT32 delay);
	Neighbor_t* GetNeighborPtr(UINT16 address);
	UINT8 NumberOfNeighbors();
	UINT8 PreviousNumberOfNeighbors();
	void SetPreviousNumberOfNeighbors(UINT8 previousNeighborCnt);
	DeviceStatus InsertNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16 seed, UINT16 mask, UINT32 nextwakeupSlot, UINT32  seedUpdateIntervalinSlots, UINT8* index);
	DeviceStatus FindOrInsertNeighbor(UINT16 address, UINT8* index);
	DeviceStatus UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink, UINT8* index);
	DeviceStatus UpdateFrameLength(UINT16 address, NeighborStatus status, UINT16 frameLength, UINT8* index);
	DeviceStatus UpdateDutyCycle(UINT16 address, UINT8 dutyCycle, UINT8* index);
	DeviceStatus UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16 seed, UINT16 mask, UINT32 nextwakeupSlot, UINT32  seedUpdateIntervalinSlots, UINT8* index);
	//DeviceStatus UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16  lastSeed, UINT16  dataInterval, UINT16  radioStartDelay, UINT16  counterOffset, UINT8* index);
	DeviceStatus UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, float rssi, float lqi);
	UINT8  UpdateNeighborTable(UINT32 NeighborLivenessDelay, UINT64 currentTime);
	UINT8  UpdateNeighborTable(UINT32 NeighborLivenessDelay);
	DeviceStatus RecordTimeSyncRequestSent(UINT16 address, UINT64 _LastTimeSyncTime);
	DeviceStatus RecordTimeSyncSent(UINT16 address, UINT64 _LastTimeSyncTime);
	DeviceStatus RecordTimeSyncRecv(UINT16 address, UINT64 _lastTimeSyncRecv);
	UINT64 GetLastTimeSyncRecv(UINT16 address);
	Neighbor_t* GetMostObsoleteTimeSyncNeighborPtr();
	//Neighbor_t* GetNeighborWOldestSyncPtr(const UINT64& curticks, const UINT64& request_limit);
	Neighbor_t* GetCritalSyncNeighborWOldestSyncPtr(const UINT64& curticks, const UINT64& request_limit,const UINT64& forcererequest_limit);
	void DegradeLinks();
	UINT16 GetMaxNeighbors();

	DeviceStatus RecordLastHeardTime(UINT16 MacAddress, UINT64 currTime);

};

DeviceStatus NeighborTable::RecordLastHeardTime(UINT16 MacAddress, UINT64 currTime){
	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(MacAddress, &index);

	if ( (retValue==DS_Success) && (MacAddress != 0 || MacAddress != 65535)){
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

UINT8 NeighborTable::UpdateNeighborTable(UINT32 NeighborLivenessDelay, UINT64 currentTime)
{
	UINT8 deadNeighbors = 0;

	UINT64 livelinessDelayInTicks = CPU_MillisecondsToTicks(NeighborLivenessDelay * 1000);


	//if (Neighbor[0].Status == Alive)
	//	hal_printf("neighbor 0 last time: %lld\tcurrent time: %lld\tlivelinessDelayinticks: %lld\r\n", Neighbor[0].LastHeardTime,  currentTime, livelinessDelayInTicks);

	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if((Neighbor[i].Status == Alive) && ((currentTime - Neighbor[i].LastHeardTime) > livelinessDelayInTicks) && (Neighbor[i].LastHeardTime != 0))
		{

			DEBUG_PRINTF_NB("[NATIVE] Neighbors.h : Removing Neighbor due to inactivity\n");
			Neighbor[i].Status = Dead;
			++deadNeighbors;
			--NumberValidNeighbor;
		}
	}

	return deadNeighbors;
}

UINT8 NeighborTable::UpdateNeighborTable(UINT32 NeighborLivenessDelay)
{
	return BringOutYourDead(NeighborLivenessDelay);
}

DeviceStatus NeighborTable::FindIndexEvenDead(UINT16 MacAddress, UINT8* index){
	int tableIndex;

	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if ( (Neighbor[tableIndex].MacAddress == MacAddress)) {
			*index = tableIndex;
			return DS_Success;
		}
	}
	return DS_Fail;
}

DeviceStatus NeighborTable::FindIndex(UINT16 MacAddress, UINT8* index){
	int tableIndex;
	
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if ( (Neighbor[tableIndex].MacAddress == MacAddress) && (Neighbor[tableIndex].Status == Alive)) {
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
		if((Neighbor[i].Status == Alive) && ((currentTime - Neighbor[i].LastHeardTime) > livelinessDelayInTicks) && (Neighbor[i].LastHeardTime != 0))
		{
			DEBUG_PRINTF_NB("[NATIVE] Neighbors.h : Removing Neighbor due to inactivity\n");
			Neighbor[i].Status = Dead;
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
	Neighbor[tableIndex].Status = Dead;
	Neighbor[tableIndex].MacAddress = INVALID_MACADDRESS;
	Neighbor[tableIndex].NumTimeSyncMessagesSent = 0;
	Neighbor[tableIndex].ForwardLink.AvgRSSI = 0;
	Neighbor[tableIndex].ForwardLink.LinkQuality = 0;
	Neighbor[tableIndex].ForwardLink.AveDelay = 0;
	Neighbor[tableIndex].ReverseLink.AvgRSSI = 0;
	Neighbor[tableIndex].ReverseLink.LinkQuality = 0;
	Neighbor[tableIndex].ReverseLink.AveDelay = 0;

	Neighbor[tableIndex].PacketsReceived = 0;
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
		if( Neighbor[tableIndex].MacAddress == INVALID_MACADDRESS){
			*index = tableIndex;
			rv = DS_Success;
			break;
		}
		else if (Neighbor[tableIndex].Status == Dead){
			*index = tableIndex;
		}
	}
	if(Neighbor[*index].MacAddress != INVALID_MACADDRESS){
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
	return NumberValidNeighbor;
}

UINT8 NeighborTable::PreviousNumberOfNeighbors(){
	return PreviousNumberValidNeighbor;
}

void NeighborTable::SetPreviousNumberOfNeighbors(UINT8 previousNeighborCnt){
	PreviousNumberValidNeighbor = previousNeighborCnt;
}

DeviceStatus NeighborTable::FindOrInsertNeighbor(UINT16 address, UINT8* index){
	DeviceStatus retValue = DS_Fail;
	if(address != 0 || address != 65535){
		retValue = FindIndexEvenDead(address, index);
		if(retValue == DS_Fail)  {
			if(address != 0 || address != 65535){
				retValue = GetFreeIdx(index);
				Neighbor[*index].MacAddress = address;
				Neighbor[*index].NumTimeSyncMessagesSent = 0;
			}
			else{
				 retValue = DS_Fail;
			}
		}
		else{
			if(Neighbor[*index].Status != Alive){
				Neighbor[*index].NumTimeSyncMessagesSent = 0;
			}
		}
	}
	return retValue;
}

DeviceStatus NeighborTable::InsertNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16 seed, UINT16 mask, UINT32 nextwakeupSlot, UINT32  seedUpdateIntervalinSlots, UINT8* index){


    DeviceStatus retValue = FindOrInsertNeighbor(address, index);

	if ( (retValue==DS_Success) && (address != 0 || address != 65535)){
		NumberValidNeighbor++;
		UpdateNeighbor(address, status, currTime, seed, mask, nextwakeupSlot, seedUpdateIntervalinSlots, index);
		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}

DeviceStatus NeighborTable::UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink, UINT8* index){
    DeviceStatus retValue = FindIndex(address, index);

	if ((retValue!=DS_Success) && (address != 0 || address != 65535)){
			if(forwardLink != NULL){
				Neighbor[*index].ForwardLink.AveDelay = forwardLink->AveDelay;
				Neighbor[*index].ForwardLink.AvgRSSI = forwardLink->AvgRSSI;
				Neighbor[*index].ForwardLink.LinkQuality = forwardLink->LinkQuality;
			}
			if(reverseLink != NULL){
				Neighbor[*index].ReverseLink.AveDelay = reverseLink->AveDelay;
				Neighbor[*index].ReverseLink.AvgRSSI = reverseLink->AvgRSSI;
				Neighbor[*index].ReverseLink.LinkQuality = reverseLink->LinkQuality;
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

DeviceStatus NeighborTable::UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16 seed, UINT16 mask, UINT32 nextwakeupSlot, UINT32  seedUpdateIntervalinSlots, UINT8* index){
	DeviceStatus retValue = FindOrInsertNeighbor(address, index);


	Neighbor[*index].MacAddress = address;
	Neighbor[*index].Status = status;
	Neighbor[*index].LastHeardTime = currTime;

	Neighbor[*index].nextSeed = seed;
	Neighbor[*index].mask = mask;
	Neighbor[*index].nextwakeupSlot = nextwakeupSlot;
	Neighbor[*index].seedUpdateIntervalinSlots = seedUpdateIntervalinSlots;
	return DS_Success;
}

DeviceStatus NeighborTable::UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, float rssi, float lqi){
	 UINT8 index;
		DeviceStatus retValue = FindOrInsertNeighbor(address, &index);

	 if ((retValue==DS_Success) && (address != 0 || address != 65535)){
			Neighbor[index].ReverseLink.AvgRSSI =  (UINT8)((float)Neighbor[index].ReverseLink.AvgRSSI*0.8 + (float)rssi*0.2);
			Neighbor[index].ReverseLink.LinkQuality =  (UINT8)((float)Neighbor[index].ReverseLink.LinkQuality*0.8 + (float)lqi*0.2);
			Neighbor[index].PacketsReceived++;
			Neighbor[index].LastHeardTime = currTime;
			Neighbor[index].Status = status;
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



Neighbor_t* NeighborTable::GetMostObsoleteTimeSyncNeighborPtr(){
	Neighbor_t* rn = NULL;
	int tableIndex;
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].Status != Dead){
			if(rn == NULL || Neighbor[tableIndex].LastTimeSyncRecvTime < rn->LastTimeSyncRecvTime  )
				rn = &Neighbor[tableIndex];
		}
	}
	return rn;
}

Neighbor_t* NeighborTable::GetCritalSyncNeighborWOldestSyncPtr(const UINT64& curticks, const UINT64& request_limit, const UINT64& forcererequest_limit){
	Neighbor_t* rn = NULL;
	int tableIndex;
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].Status != Dead){
			if(rn == NULL || Neighbor[tableIndex].LastTimeSyncSendTime < rn->LastTimeSyncSendTime || Neighbor[tableIndex].NumTimeSyncMessagesSent < NUM_ENFORCED_TSR_PCKTS_BEFORE_DATA_PCKTS ){ //Consider this neighbor
				if((curticks - Neighbor[tableIndex].LastTimeSyncSendTime > request_limit || curticks - Neighbor[tableIndex].LastTimeSyncRecvTime > forcererequest_limit || Neighbor[tableIndex].NumTimeSyncMessagesSent < NUM_ENFORCED_TSR_PCKTS_BEFORE_DATA_PCKTS )
				&& (Neighbor[tableIndex].LastTimeSyncRequestTime == 0 || curticks - Neighbor[tableIndex].LastTimeSyncRequestTime  > request_limit || curticks - Neighbor[tableIndex].LastTimeSyncRequestTime  > forcererequest_limit )
				){
				rn = &Neighbor[tableIndex];
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
		/*if(Neighbor[i].ReverseLink.Quality >2){
				Neighbor[i].ReverseLink.Quality--;
		}*/
	}
}


#endif /* NEIGHBORS_H_ */
