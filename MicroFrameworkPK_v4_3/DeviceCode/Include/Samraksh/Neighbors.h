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
#define MAX_NEIGHBORS 32

extern void  ManagedCallback(UINT16 arg1, UINT16 arg2);;
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
}Neighbor_t;

//class NeighborTable : public CLR_RT_HeapBlock_Node {
class NeighborTable {
public:
	UINT8 NumberValidNeighbor;
	Neighbor_t Neighbor[MAX_NEIGHBORS];

public:
	BOOL Initialize();
	UINT8 FindIndex(UINT16 address);
	UINT8 RemoveSuspects(UINT32 delay);
	UINT8 MarkSuspects(UINT32 delay);
	Neighbor_t* GetNeighborPtr(UINT16 address);
	INT16 NumberOfNeighbors();
	UINT8 InsertNeighbor(UINT16 address, NeighborStatus status, UINT64 currtime);
	INT16 GetNewIndex();
	UINT8 UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink);
	UINT8 UpdateFrameLength(UINT16 address, NeighborStatus status, UINT16 frameLength);
	UINT8 UpdateDutyCycle(UINT16 address, UINT8 dutyCycle);
	UINT8 UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, float rssi, float lqi);
	BOOL  UpdateNeighborTable(UINT32 NeighbourLivelinessDelay);
	void DegradeLinks();
	BOOL  DoesNodeExist(UINT16 address);
};

BOOL NeighborTable::DoesNodeExist(UINT16 address)
{
	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if(Neighbor[i].MacAddress == address)
			return TRUE;
	}

	return FALSE;
}

BOOL NeighborTable::UpdateNeighborTable(UINT32 NeighbourLivelinessDelay)
{
	return RemoveSuspects(NeighbourLivelinessDelay);
}

BOOL NeighborTable::Initialize()
{

	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		DEBUG_PRINTF("Initializing Neighbour table\n");
		Neighbor[i].Status = Dead;
		Neighbor[i].LastHeardTime = 0;
	}
}

INT16 NeighborTable::GetNewIndex()
{
	DEBUG_PRINTF("Get a new index for the neighbour\n");
	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
			if(Neighbor[i].Status == Dead)
				return i;
	}

	return -1;
}

UINT8 NeighborTable::FindIndex(UINT16 address){
	UINT8 i=0;
	for (i=0; i < MAX_NEIGHBORS; i++){
		if(Neighbor[i].MacAddress == address){
			return i;
		}
	}
	return 255;
}

UINT8 NeighborTable::RemoveSuspects(UINT32 delay){

	GLOBAL_LOCK(irq);

	UINT16 deadNeighbours = 0;

	UINT64 livelinesDelayInTicks = CPU_MillisecondsToTicks(delay * 1000);

	UINT64 currentTime = HAL_Time_CurrentTime();

	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	{
		if((Neighbor[i].Status == Alive) && ((currentTime - Neighbor[i].LastHeardTime) > livelinesDelayInTicks) && (Neighbor[i].LastHeardTime != 0))
		{
		
			DEBUG_PRINTF("[NATIVE] Neighbors.h : Removing Neighbor due to inactivity\n");
			Neighbor[i].Status = Dead;
			deadNeighbours++;
			NumberValidNeighbor--;
			
		}
	}

	// Don't make callback if there are no dead neighbours
	if(deadNeighbours > 0)
		ManagedCallback(NeighbourChanged, deadNeighbours);


	return 1;

}

UINT8 NeighborTable::MarkSuspects(UINT32 delay){
	return 0;
}

Neighbor_t* NeighborTable::GetNeighborPtr(UINT16 address){
	UINT8 index = FindIndex(address);
	if(index==255){
		return NULL;
	}else {
		return &Neighbor[index];
	}
}

INT16 NeighborTable::NumberOfNeighbors(){
	return NumberValidNeighbor;
}
UINT8 NeighborTable::InsertNeighbor(UINT16 address, NeighborStatus status, UINT64 currtime){
	UINT8 index = FindIndex(address);
	if (index==255 && (address != 0 || address != 65535) && NumberValidNeighbor < MAX_NEIGHBORS){
		index= GetNewIndex();
		// The neighbour table is full, can not insert this guy now
		if(index == -1)
		{
			return 255;
		}
		NumberValidNeighbor++;
		Neighbor[index].MacAddress = address;
		Neighbor[index].Status = status;
		Neighbor[index].LastHeardTime = currtime;

		ManagedCallback(NeighbourChanged, 1);

		return index;
	}
	else {
		return 255;
	}
}
UINT8 NeighborTable::UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink){
	UINT8 index = FindIndex(address);
	if (index!=255 && (address != 0 || address != 65535)){
			if(forwardLink != NULL){
				Neighbor[index].ForwardLink.AveDelay = forwardLink->AveDelay;
				Neighbor[index].ForwardLink.AvgRSSI = forwardLink->AvgRSSI;
				Neighbor[index].ForwardLink.LinkQuality = forwardLink->LinkQuality;
			}
			if(reverseLink != NULL){
				Neighbor[index].ReverseLink.AveDelay = reverseLink->AveDelay;
				Neighbor[index].ReverseLink.AvgRSSI = reverseLink->AvgRSSI;
				Neighbor[index].ReverseLink.LinkQuality = reverseLink->LinkQuality;
			}
	}
	return index;
}

UINT8 NeighborTable::UpdateFrameLength(UINT16 address, NeighborStatus status, UINT16 frameLength){
	UINT8 index = FindIndex(address);
	if (index!=255 && (address != 0 || address != 65535)){
			Neighbor[index].FrameLength = frameLength;
	}
	return index;
}

UINT8 NeighborTable::UpdateDutyCycle(UINT16 address, UINT8 dutyCycle){
	UINT8 index = FindIndex(address);
	if (index!=255 && (address != 0 || address != 65535)){
			Neighbor[index].ReceiveDutyCycle = dutyCycle;
	}
	return index;
}

UINT8 NeighborTable::UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, float rssi, float lqi){

	BOOL callManaged = FALSE;

	UINT8 index = FindIndex(address);
	if (index!=255 && (address != 0 || address != 65535)){

			// If the node for some reason went dead and came back to life
		    // Let the managed code know of this
			if(Neighbor[index].Status == Dead)
				callManaged = TRUE;

			Neighbor[index].ReverseLink.AvgRSSI =  (UINT8)((float)Neighbor[index].ReverseLink.AvgRSSI*0.8 + (float)rssi*0.2);
			Neighbor[index].ReverseLink.LinkQuality =  (UINT8)((float)Neighbor[index].ReverseLink.LinkQuality*0.8 + (float)lqi*0.2);
			Neighbor[index].PacketsReceived++;
			Neighbor[index].LastHeardTime = currTime;
			Neighbor[index].Status = status;

	}

	if(callManaged)
		ManagedCallback(NeighbourChanged, 1);

	return index;
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
