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

#define NeighbourChanged 1
#define Received 0

#include "TinyCLR_Runtime.h"

extern UINT8 MacName;
#define MAX_NEIGHBORS 12

//extern void  ManagedCallback(UINT16 arg1, UINT16 arg2);
//#define DEBUG_NEIGHBORTABLE

#if defined(DEBUG_NEIGHBORTABLE)
#define ENABLE_PIN(x,y) CPU_GPIO_EnableOutputPin(x,y)
#define SET_PIN(x,y) CPU_GPIO_SetPinState(x,y)
#define DEBUG_PRINTF(x) hal_printf(x)
#else
#define ENABLE_PIN(x,y)
#define SET_PIN(x,y)
#define DEBUG_PRINTF(x)
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
	UINT16  lastSeed; //the seed we have from in the control message last received

	//the starting slot of the frame immediately after last seed's update
	//e.g. if seeds are updated every 8 frames, then the following invariant holds:
	//nextFrameAfterSeedUpdate = 8N + 1, N = 0, 1, 2...
	UINT32  nextFrameAfterSeedUpdate;
	UINT16  dataInterval;
	UINT16  radioStartDelay;
	UINT16  counterOffset;
	//UINT8   numErrors;
	//UINT8   size;
	//BOOL    isInTransition;
	//UINT32  localAvg;
	//INT32   offsetAvg;
	//float   skew;
}Neighbor_t;

class NeighborTable {
public:
	UINT8 NumberValidNeighbor;
	Neighbor_t Neighbor[MAX_NEIGHBORS];

	// neighbor table util functions
	DeviceStatus GetFreeIdx(UINT8* index);
	DeviceStatus ClearNeighbor(UINT16 MacAddress);
	DeviceStatus FindIndex(UINT16 MacAddress, UINT8* index);
	void ClearTable();
	UINT8 BringOutYourDead(UINT32 delay);
	Neighbor_t* GetNeighborPtr(UINT16 address);
	UINT8 NumberOfNeighbors();
	DeviceStatus InsertNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16 seed, UINT16  dataInterval, UINT16  radioStartDelay, UINT16  counterOffset, UINT8* index);
	DeviceStatus UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink, UINT8* index);
	DeviceStatus UpdateFrameLength(UINT16 address, NeighborStatus status, UINT16 frameLength, UINT8* index);
	DeviceStatus UpdateDutyCycle(UINT16 address, UINT8 dutyCycle, UINT8* index);
	//DeviceStatus UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16  lastSeed, UINT16  dataInterval, UINT16  radioStartDelay, UINT16  counterOffset, UINT8* index);
	DeviceStatus UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, float rssi, float lqi);
	UINT8  UpdateNeighborTable(UINT32 NeighbourLivelinessDelay);
	void DegradeLinks();
	UINT16 GetMaxNeighbors();
};

UINT16 NeighborTable::GetMaxNeighbors(void){
	return MAX_NEIGHBORS;
}

UINT8 NeighborTable::UpdateNeighborTable(UINT32 NeighbourLivelinessDelay)
{
	return BringOutYourDead(NeighbourLivelinessDelay);
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

	UINT8 deadNeighbours = 0;

	UINT64 livelinessDelayInTicks = CPU_MillisecondsToTicks(delay * 1000);

	UINT64 currentTime = CPU_Time_CurrentTicks();


	//if (Neighbor[0].Status == Alive)
	//	hal_printf("neighbor 0 last time: %lld\tcurrent time: %lld\tlivelinessDelayinticks: %lld\r\n", Neighbor[0].LastHeardTime,  currentTime, livelinessDelayInTicks);

	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if((Neighbor[i].Status == Alive) && ((currentTime - Neighbor[i].LastHeardTime) > livelinessDelayInTicks) && (Neighbor[i].LastHeardTime != 0))
		{

			DEBUG_PRINTF("[NATIVE] Neighbors.h : Removing Neighbor due to inactivity\n");
			Neighbor[i].Status = Dead;
			deadNeighbours++;
			NumberValidNeighbor--;
		}
	}


	return deadNeighbours;

}

DeviceStatus NeighborTable::ClearNeighbor(UINT16 nodeId){
	UINT8 tableIndex;

	if (FindIndex(nodeId, &tableIndex) == DS_Success){
		Neighbor[tableIndex].MacAddress = 0;
		Neighbor[tableIndex].lastSeed = 0;
		Neighbor[tableIndex].nextFrameAfterSeedUpdate = 0;
		Neighbor[tableIndex].dataInterval = 0;
		Neighbor[tableIndex].radioStartDelay = 0;
		Neighbor[tableIndex].counterOffset = 0;

		Neighbor[tableIndex].ForwardLink.AvgRSSI = 0;
		Neighbor[tableIndex].ForwardLink.LinkQuality = 0;
		Neighbor[tableIndex].ForwardLink.AveDelay = 0;
		Neighbor[tableIndex].ReverseLink.AvgRSSI = 0;
		Neighbor[tableIndex].ReverseLink.LinkQuality = 0;
		Neighbor[tableIndex].ReverseLink.AveDelay = 0;
		Neighbor[tableIndex].Status = Dead;
		Neighbor[tableIndex].PacketsReceived = 0;
		Neighbor[tableIndex].LastHeardTime = 0;
		Neighbor[tableIndex].ReceiveDutyCycle = 0; //percentage
		Neighbor[tableIndex].FrameLength = 0;
		
		/*Neighbor[tableIndex].numErrors = 0;
		Neighbor[tableIndex].size = 0;
		Neighbor[tableIndex].isInTransition = 0;
		Neighbor[tableIndex].localAvg = 0;
		Neighbor[tableIndex].offsetAvg = 0;
		Neighbor[tableIndex].skew = 0;*/

		NumberValidNeighbor--;
		return DS_Success;
	} else {
		return DS_Fail;
	}
}

void NeighborTable::ClearTable(){
	int tableIndex;

	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		Neighbor[tableIndex].MacAddress = 0;
		Neighbor[tableIndex].lastSeed = 0;
		Neighbor[tableIndex].nextFrameAfterSeedUpdate = 0;
		Neighbor[tableIndex].dataInterval = 0;
		Neighbor[tableIndex].radioStartDelay = 0;
		Neighbor[tableIndex].counterOffset = 0;

		Neighbor[tableIndex].ForwardLink.AvgRSSI = 0;
		Neighbor[tableIndex].ForwardLink.LinkQuality = 0;
		Neighbor[tableIndex].ForwardLink.AveDelay = 0;
		Neighbor[tableIndex].ReverseLink.AvgRSSI = 0;
		Neighbor[tableIndex].ReverseLink.LinkQuality = 0;
		Neighbor[tableIndex].ReverseLink.AveDelay = 0;
		Neighbor[tableIndex].Status = Dead;
		Neighbor[tableIndex].PacketsReceived = 0;
		Neighbor[tableIndex].LastHeardTime = 0;
		Neighbor[tableIndex].ReceiveDutyCycle = 0; //percentage
		Neighbor[tableIndex].FrameLength = 0;

		/*Neighbor[tableIndex].numErrors = 0;
		Neighbor[tableIndex].size = 0;
		Neighbor[tableIndex].isInTransition = 0;
		Neighbor[tableIndex].localAvg = 0;
		Neighbor[tableIndex].offsetAvg = 0;
		Neighbor[tableIndex].skew = 0;*/
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

DeviceStatus NeighborTable::InsertNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16 seed, UINT16  dataInterval, UINT16  radioStartDelay, UINT16  counterOffset, UINT8* index){
    DeviceStatus retValue = GetFreeIdx(index);

	if ( (retValue==DS_Success) && (address != 0 || address != 65535)){
		NumberValidNeighbor++;
		Neighbor[*index].MacAddress = address;
		Neighbor[*index].Status = status;
		Neighbor[*index].LastHeardTime = currTime;
		Neighbor[*index].dataInterval = dataInterval;
		Neighbor[*index].radioStartDelay = radioStartDelay;
		Neighbor[*index].counterOffset = counterOffset;
		Neighbor[*index].lastSeed = seed;
		//ManagedCallback(NeighbourChanged, NumberValidNeighbor);
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

//DeviceStatus NeighborTable::UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16 seed, UINT16  dataInterval, UINT16  radioStartDelay, UINT16  counterOffset, UINT8* index){
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
