/*
 * ArrayBuffer.h
 *
 *  Created on: Mar 23, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_ARRAYBUFFER_H_
#define MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_ARRAYBUFFER_H_

#include <Samraksh\Message.h>
#include "OMAC_Common_Buffer_I.h"

#define UNUSED_DEST_ID 0

template <UINT16 BUFFERSIZE>
class ArrayBuffer {
public:
	UINT16 last_used_index;
	UINT16 num_elements;

	Message_15_4_t send_packet_buffer[BUFFERSIZE];

	void DeleteElement(UINT16 index){
		if(index < BUFFERSIZE) {
			ClearMsgContents(&(send_packet_buffer[index]));
		}
		--num_elements;
	}

	UINT16 Search(Message_15_4_t & temp_msg){
		//		for(UINT16 i = 0; i < BUFFERSIZE; ++i){
		//			if( &(send_packet_buffer[i]) == &temp_msg){
		//				return i;
		//			}
		//		}

		Message_15_4_t* rm = NULL;
		UINT16 i = 0;
		for(UINT16 j = 0; j < BUFFERSIZE; ++j){
			if(j > last_used_index){
				i = (BUFFERSIZE - j) + last_used_index;
			}
			else{
				i = last_used_index - j;
			}

			if( &(send_packet_buffer[i]) == &temp_msg){
				return i;
			}
		}

		return BUFFERSIZE;
	}

public:
	ArrayBuffer(){
		Initialize();
	}
	void Initialize(){
		last_used_index = 0;
		num_elements = 0;
	}
	bool DeletePacket(Message_15_4_t* msg_carrier){
		UINT16 i = Search(*msg_carrier);
		if( i < BUFFERSIZE) {
			DeleteElement(i);
			return true;
		}
		else return false;
	}
	bool RemovePacketsWithRetryAttemptsGreaterThan(UINT8 r){
		for(UINT16 i = 0; i < BUFFERSIZE; ++i){
			if( send_packet_buffer[i].GetMetaDataConst()->GetRetryAttempts() > r){
				DeleteElement(i);
				return true;
			}
		}
		return false;
	}

	bool IsThereATSRPacketWithDest(const UINT16 address){
		UINT16 i = 0;
		for(UINT16 j = 0; j < BUFFERSIZE; ++j){
			i = last_used_index - j;
			if(j > last_used_index){
				i = (BUFFERSIZE - j) + last_used_index;
			}
			else{
				i = last_used_index - j;
			}

			if( send_packet_buffer[i].GetHeaderConst()->payloadType == MFM_OMAC_TIMESYNCREQ && send_packet_buffer[i].GetHeaderConst()->dest == address){
				return true;
			}
		}

		//		for(UINT16 i = 0; i < BUFFERSIZE; ++i){
		//			if( send_packet_buffer[i].GetHeaderConst()->payloadType == MFM_OMAC_TIMESYNCREQ && send_packet_buffer[i].GetHeaderConst()->dest == address){
		//				return true;
		//			}
		//		}
		return false;
	}
	bool IsThereADataPacketWithDest(const UINT16 address){

		UINT16 i = 0;
		for(UINT16 j = 0; j < BUFFERSIZE; ++j){
			i = last_used_index - j;
			if(j > last_used_index){
				i = (BUFFERSIZE - j) + last_used_index;
			}
			else{
				i = last_used_index - j;
			}

			if( send_packet_buffer[i].GetHeaderConst()->payloadType != MFM_OMAC_TIMESYNCREQ && send_packet_buffer[i].GetHeaderConst()->dest == address){
				return true;
			}
		}

		//		for(UINT16 i = 0; i < BUFFERSIZE; ++i){
		//			if( send_packet_buffer[i].GetHeaderConst()->payloadType != MFM_OMAC_TIMESYNCREQ && send_packet_buffer[i].GetHeaderConst()->dest == address){
		//				return true;
		//			}
		//		}

		return false;
	}
	Message_15_4_t* FindTSRPacketForNeighbor(const UINT16 neigh){
		Message_15_4_t* rm = NULL;
		UINT16 i = 0;
		for(UINT16 j = 0; j < BUFFERSIZE; ++j){
			i = last_used_index - j;
			if(j > last_used_index){
				i = (BUFFERSIZE - j) + last_used_index;
			}
			else{
				i = last_used_index - j;
			}

			if( send_packet_buffer[i].GetHeaderConst()->payloadType == MFM_OMAC_TIMESYNCREQ && send_packet_buffer[i].GetHeaderConst()->dest == neigh){
				if(rm == NULL){
					rm = &(send_packet_buffer[i]);
				}
				else if(rm -> GetMetaDataConst()->GetReceiveTimeStamp() > send_packet_buffer[i].GetMetaDataConst()->GetReceiveTimeStamp()){
					rm = &(send_packet_buffer[i]);
				}
			}
		}
		//		for(UINT16 i = 0; i < BUFFERSIZE; ++i){
		//			if( send_packet_buffer[i].GetHeaderConst()->payloadType == MFM_OMAC_TIMESYNCREQ && send_packet_buffer[i].GetHeaderConst()->dest == neigh){
		//				if(rm == NULL){
		//					rm = &(send_packet_buffer[i]);
		//				}
		//				else if(rm -> GetMetaDataConst()->GetReceiveTimeStamp() > send_packet_buffer[i].GetMetaDataConst()->GetReceiveTimeStamp()){
		//					rm = &(send_packet_buffer[i]);
		//				}
		//			}
		//		}
		return rm;
	}
	Message_15_4_t* FindDataPacketForNeighbor(const UINT16 neigh){
		Message_15_4_t* rm = NULL;
		UINT16 i = 0;
		for(UINT16 j = 0; j < BUFFERSIZE; ++j){
			i = last_used_index - j;
			if(j > last_used_index){
				i = (BUFFERSIZE - j) + last_used_index;
			}
			else{
				i = last_used_index - j;
			}

			if( send_packet_buffer[i].GetHeaderConst()->payloadType != MFM_OMAC_TIMESYNCREQ && send_packet_buffer[i].GetHeaderConst()->dest == neigh){
				if(rm == NULL){
					rm = &(send_packet_buffer[i]);
				}
				else if(rm -> GetMetaDataConst()->GetReceiveTimeStamp() > send_packet_buffer[i].GetMetaDataConst()->GetReceiveTimeStamp()){
					rm = &(send_packet_buffer[i]);
				}
			}
		}
		//		for(UINT16 i = 0; i < BUFFERSIZE; ++i){
		//			if( send_packet_buffer[i].GetHeaderConst()->payloadType != MFM_OMAC_TIMESYNCREQ && send_packet_buffer[i].GetHeaderConst()->dest == neigh){
		//				if(rm == NULL){
		//					rm = &(send_packet_buffer[i]);
		//				}
		//				else if(rm -> GetMetaDataConst()->GetReceiveTimeStamp() > send_packet_buffer[i].GetMetaDataConst()->GetReceiveTimeStamp()){
		//					rm = &(send_packet_buffer[i]);
		//				}
		//			}
		//		}
		return rm;
	}
	bool InsertMessage(Message_15_4_t* msg_carrier){
		if(num_elements < BUFFERSIZE){

			for(UINT16 i = 0; i < BUFFERSIZE; ++i){
				UINT16 index = (i+last_used_index+1)%BUFFERSIZE;
				if( send_packet_buffer[index].GetHeaderConst()->dest == UNUSED_DEST_ID){
					memcpy(&(send_packet_buffer[index]), msg_carrier, sizeof(Message_15_4_t));
					last_used_index = index;
					++num_elements;
					return true;
				}
			}
		}
		return false;
	}

	UINT16 GetNumberofElements() const {
		return num_elements;
	}

	void SetNumberofElements(UINT16 x) {
		num_elements = x;
	}

};




#endif /* MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_ARRAYBUFFER_H_ */
