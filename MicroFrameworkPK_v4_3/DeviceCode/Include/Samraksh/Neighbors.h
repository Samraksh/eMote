/*
 * Neighbors.h : Defines and implements the MAC/Link layer Neighbor Table
 *
 *Created on: Oct 25, 2012
 *Author: Mukundan Sridharan
 *
 */

#ifndef NEIGHBORS_H_
#define NEIGHBORS_H_

#include "TinyCLR_Runtime.h"

#define MAX_NEIGHBORS 256

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
	UINT8 FindIndex(UINT16 address);
	UINT8 RemoveSuspects(UINT32 delay);
	UINT8 MarkSuspects(UINT32 delay);
	Neighbor_t* GetNeighborPtr(UINT16 address);
	UINT8 NumberOfNeighbors();
	UINT8 InsertNeighbor(UINT16 address, NeighborStatus status, UINT64 currtime);
	UINT8 UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink);
	UINT8 UpdateFrameLength(UINT16 address, NeighborStatus status, UINT16 frameLength);
	UINT8 UpdateDutyCycle(UINT16 address, UINT8 dutyCycle);
	UINT8 UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime);
	void DegradeLinks();
};

UINT8 NeighborTable::FindIndex(UINT16 address){
	UINT8 i=0;
	for (i=0; i < NumberValidNeighbor; i++){
		if(Neighbor[i].MacAddress == address){
			return i;
		}
	}
	return 255;
}

UINT8 NeighborTable::RemoveSuspects(UINT32 delay){
	return 0;
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
UINT8 NeighborTable::NumberOfNeighbors(){
	return NumberValidNeighbor;
}
UINT8 NeighborTable::InsertNeighbor(UINT16 address, NeighborStatus status, UINT64 currtime){
	UINT8 index = FindIndex(address);
	if (index==255 && (address != 0 || address != 65535) && NumberValidNeighbor < MAX_NEIGHBORS){
		index=NumberValidNeighbor;
		NumberValidNeighbor++;
		Neighbor[index].MacAddress = address;
		Neighbor[index].Status = status;
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

UINT8 NeighborTable::UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime){
	UINT8 index = FindIndex(address);
	if (index!=255 && (address != 0 || address != 65535)){
			Neighbor[index].Status = status;
			Neighbor[index].LastHeardTime = currTime;
			/*if(Neighbor[index].ReverseLink.Quality < 254){

			}*/
	}
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
