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

#define NeighborChanged 1
#define Received 0

//#include "TinyCLR_Runtime.h"

extern UINT8 MacName;
#define MAX_NEIGHBORS 12

//extern void  ManagedCallback(UINT16 arg1, UINT16 arg2);
//#define DEBUG_NEIGHBORTABLE

#if defined(DEBUG_NEIGHBORTABLE)
#define ENABLE_PIN_NB(x,y) CPU_GPIO_EnableOutputPin(x,y)
#define SET_PIN_NB(x,y) CPU_GPIO_SetPinState(x,y)
#define DEBUG_PRINTF_NB(x) hal_printf(x)
#else
#define ENABLE_PIN_NB(x,y)
#define SET_PIN_NB(x,y)
#define DEBUG_PRINTF_NB(x)
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

	UINT64  LastTimeSyncTime;			// Lasst time a time sync message is received or a time sync request is sent.
	UINT64  LastTimeSyncRequestTime;	// Last time instant a time sync request is sent
	//UINT8   numErrors;
	//UINT8   size;
	//BOOL    isInTransition;
	//UINT32  localAvg;
	//INT32   offsetAvg;
	//float   skew;
	//TODO: BK: DELETE THESE NOT USED BUT KEPT FOR THE TIME BEIGN
	UINT16  radioStartDelay;
	UINT16  counterOffset;
}Neighbor_t;

class NeighborTable {
public:
	UINT8 NumberValidNeighbor;
	Neighbor_t Neighbor[MAX_NEIGHBORS];

	// neighbor table util functions
	DeviceStatus GetFreeIdx(UINT8* index);
	DeviceStatus ClearNeighbor(UINT16 MacAddress);
	DeviceStatus ClearNeighborwIndex(UINT8 tableIndex);
	DeviceStatus FindIndex(UINT16 MacAddress, UINT8* index);
	void ClearTable();
	UINT8 BringOutYourDead(UINT32 delay);
	Neighbor_t* GetNeighborPtr(UINT16 address);
	UINT8 NumberOfNeighbors();
	DeviceStatus InsertNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16 seed, UINT16 mask, UINT32 nextwakeupSlot, UINT32  seedUpdateIntervalinSlots, UINT8* index);
	DeviceStatus UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink, UINT8* index);
	DeviceStatus UpdateFrameLength(UINT16 address, NeighborStatus status, UINT16 frameLength, UINT8* index);
	DeviceStatus UpdateDutyCycle(UINT16 address, UINT8 dutyCycle, UINT8* index);
	DeviceStatus UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16 seed, UINT16 mask, UINT32 nextwakeupSlot, UINT32  seedUpdateIntervalinSlots, UINT8* index);
	//DeviceStatus UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16  lastSeed, UINT16  dataInterval, UINT16  radioStartDelay, UINT16  counterOffset, UINT8* index);
	DeviceStatus UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, float rssi, float lqi);
	UINT8  UpdateNeighborTable(UINT32 NeighborLivenessDelay);
	DeviceStatus RecordTimeSyncRequestSent(UINT16 address, UINT64 _LastTimeSyncTime);
	DeviceStatus RecordTimeSyncRecv(UINT16 address, UINT64 _lastTimeSyncRecv);
	void RecordTimeSyncBroadcast(UINT64 _LastTimeSyncTime);
	Neighbor_t* GetMostObsoleteTimeSyncNeighborPtr();
	void DegradeLinks();
	UINT16 GetMaxNeighbors();

};

UINT16 NeighborTable::GetMaxNeighbors(void){
	return MAX_NEIGHBORS;
}

UINT8 NeighborTable::UpdateNeighborTable(UINT32 NeighborLivenessDelay)
{
	//return BringOutYourDead(NeighborLivenessDelay);
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

	GLOBAL_LOCK(irq);

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
	Neighbor[tableIndex].MacAddress = 0;

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



	Neighbor[tableIndex].LastTimeSyncTime = 0;
	Neighbor[tableIndex].LastTimeSyncRequestTime = 0;

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
	int tableIndex;

	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].Status == Dead){			
			*index = tableIndex;
			return DS_Success;
		}
	}
	return DS_Fail;
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

DeviceStatus NeighborTable::InsertNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16 seed, UINT16 mask, UINT32 nextwakeupSlot, UINT32  seedUpdateIntervalinSlots, UINT8* index){
    DeviceStatus retValue = GetFreeIdx(index);

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
	Neighbor[*index].MacAddress = address;
	Neighbor[*index].Status = status;
	Neighbor[*index].LastHeardTime = currTime;

	Neighbor[*index].nextSeed = seed;
	Neighbor[*index].nextwakeupSlot = nextwakeupSlot;
	Neighbor[*index].seedUpdateIntervalinSlots = seedUpdateIntervalinSlots;
	return DS_Success;
}

DeviceStatus NeighborTable::UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, float rssi, float lqi){
	 UINT8 index;
	 DeviceStatus retValue = FindIndex(address, &index);

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
	 DeviceStatus retValue = FindIndex(address, &index);

	if ( (retValue==DS_Success) && (address != 0 || address != 65535)){
		Neighbor[index].LastTimeSyncTime = _LastTimeSyncTime;
		Neighbor[index].LastTimeSyncRequestTime = _LastTimeSyncTime;
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
	 DeviceStatus retValue = FindIndex(address, &index);

	if ( (retValue==DS_Success) && (address != 0 || address != 65535)){
		Neighbor[index].LastTimeSyncTime = _LastTimeSyncTime;
		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}

void NeighborTable::RecordTimeSyncBroadcast(UINT64 _LastTimeSyncTime){
	int tableIndex;
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].Status != Dead){
			Neighbor[tableIndex].LastTimeSyncTime = _LastTimeSyncTime;
		}
	}
}

Neighbor_t* NeighborTable::GetMostObsoleteTimeSyncNeighborPtr(){
	Neighbor_t* rn = NULL;
	int tableIndex;
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].Status != Dead){
			if(rn == NULL || Neighbor[tableIndex].LastTimeSyncTime < rn->LastTimeSyncTime  )
				rn = &Neighbor[tableIndex];
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
