/*
 * Buffer.cpp
 *
 *  Created on: Aug 31, 2012
 *      Author: Mukundan Sridharan
 */
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <tinyhal.h>
#include "Message.h"

//#define _DEBUG_BUFFER_

template< class MessageT, UINT8 BufferSizeT>
class CircularBuffer  {

	MessageT *msgPtr[BufferSizeT];
	MessageT msg[BufferSizeT];
	UINT8 nextCleanBuffer, firstFullBuffer;
	UINT8 numElements;

public:

	UINT8 GetFirstFullBufferPosition(){
		return firstFullBuffer;
	}

	MessageT* PeekElementAtPosition(UINT8 cur_pos){
		//Check if valid element
		return msgPtr[cur_pos];
	}

	BOOL DropElementAtPosition(UINT8 cur_pos){
		//Exchange pointers
		if (cur_pos == firstFullBuffer) {
			GetFirstFullBuffer();
			return true;
		}
		else{
			UINT8 j,i = cur_pos;
			j = i;
			bool rtn_var = false;
			i += 1;
			if(i >= BufferSizeT){
				i = 0;
			}
			if(i == nextCleanBuffer){
				rtn_var = true;
			}
			else{
				while (i != nextCleanBuffer){
					rtn_var = true;
					msgPtr[j] = msgPtr[i];
					j = i;
					i += 1;
					if(i >= BufferSizeT){
						i = 0;
					}
				}
			}

			if(rtn_var){
				--numElements;
				if(nextCleanBuffer == 0 ) nextCleanBuffer = BufferSizeT - 1;
				else --nextCleanBuffer;
			}
			return rtn_var;
		}

	}

	BOOL Initialize(){
		int i = 0;
		for (i = 0; i < BufferSizeT; i++){
			msgPtr[i] = &msg[i];
		}
		numElements = 0;
		nextCleanBuffer = 0;
		firstFullBuffer = 0;
		return TRUE;
	}

	MessageT** GetNextFreeBufferPtr()
	{
		if(this->IsFull()){
			return (Message_15_4_t**)(NULL);
		}

		MessageT** rtn= &msgPtr[nextCleanBuffer];
		nextCleanBuffer += 1;
		if(nextCleanBuffer >= BufferSizeT){
			nextCleanBuffer = 0;
		}
		numElements++;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetNextFreeBufferPtr numElements: %u\n", numElements);
#endif
		return rtn;

	}

	MessageT* GetNextFreeBuffer(){
		if(this->IsFull()){
			return (Message_15_4_t*)(NULL);
		}

		MessageT* rtn = msgPtr[nextCleanBuffer];
		nextCleanBuffer += 1;
		if(nextCleanBuffer >= BufferSizeT){
			nextCleanBuffer = 0;
		}
		//nextCleanBuffer= (nextCleanBuffer++) % 16;
		numElements++;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetNextFreeBuffer numElements: %u\n", numElements);
#endif
		return rtn;
	}

	MessageT* GetFirstFullBuffer(){
		MessageT* rtn = msgPtr[firstFullBuffer];
		firstFullBuffer += 1;
		if(firstFullBuffer >= BufferSizeT){
			firstFullBuffer = 0;
		}
		//firstFullBuffer= (firstFullBuffer++) % BufferSizeT;
		numElements--;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetFirstFullBuffer numElements: %u\n", numElements);
#endif
		return rtn;
	}

	MessageT* PeekFirstFullBuffer(){
		MessageT* rtn = msgPtr[firstFullBuffer];
//		firstFullBuffer += 1;
//		if(firstFullBuffer >= BufferSizeT){
//			firstFullBuffer = 0;
//		}
//		//firstFullBuffer= (firstFullBuffer++) % BufferSizeT;
//		numElements--;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetFirstFullBuffer numElements: %u\n", numElements);
#endif
		return rtn;
	}

	MessageT** GetFirstFullBufferPtr(){
		MessageT** rtn = &msgPtr[firstFullBuffer];
		firstFullBuffer += 1;
		if(firstFullBuffer >= BufferSizeT){
			firstFullBuffer = 0;
		}
		//firstFullBuffer= (firstFullBuffer++) % BufferSizeT;
		numElements--;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetFirstFullBufferPtr numElements: %u\n", numElements);
#endif
		return rtn;
	}

	MessageT* GetLastFullBuffer(){
		nextCleanBuffer -= 1;
		UINT8 lastFullBuffer = nextCleanBuffer % BufferSizeT;
		MessageT* rtn = msgPtr[lastFullBuffer];
		nextCleanBuffer = lastFullBuffer;
		numElements--;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetLastFullBuffer numElements: %u\n", numElements);
#endif
		return rtn;
	}

	void ClearBuffer(){
		while(!IsEmpty()){
			GetFirstFullBuffer();
		}
	}

	BOOL IsFull(){
#ifdef _DEBUG_BUFFER_
		hal_printf("IsFull numElements: %u\n", numElements);
#endif
		if(numElements == BufferSizeT){
			return TRUE;
		}
		else{
			return FALSE;
		}
	}

	BOOL IsEmpty(){
#ifdef _DEBUG_BUFFER_
		hal_printf("IsFull numElements: %u\n", numElements);
#endif
		if(numElements == 0){
			return TRUE;
		}
		else{
			return FALSE;
		}
	}

	UINT8 GetNumberMessagesInBuffer(){
		return numElements;
	}

	UINT8 Size(){
		return BufferSizeT;
	}


};

template <UINT8 BufferSizeT>
class Buffer_15_4: public  CircularBuffer<Message_15_4_t, BufferSizeT>{
public:
	BOOL Initialize(){
		return CircularBuffer<Message_15_4_t, BufferSizeT>::Initialize();
	}

	BOOL CopyPayload(void* payload, UINT8 size){
		Message_15_4_t* dummy;
		if(CircularBuffer<Message_15_4_t, BufferSizeT>::IsFull()){
			return FALSE;
		}
		if(size > dummy->GetPayloadSize()){
			return FALSE;
		}
		Message_15_4_t* msg = this->GetNextFreeBuffer();
		memset(msg->GetPayload(), 0 , size);
		memcpy(msg->GetPayload(), payload , size);
		return TRUE;
	}

	BOOL Store(void* msg, UINT8 size){
		Message_15_4_t* dummy;
		if(CircularBuffer<Message_15_4_t, BufferSizeT>::IsFull()){
			return FALSE;
		}
		if(size > dummy->GetMessageSize()){
			return FALSE;
		}
		Message_15_4_t* buffer = this->GetNextFreeBuffer();
		memset(buffer, 0, sizeof(Message_15_4_t));
		memcpy(buffer, msg, sizeof(Message_15_4_t));
		return TRUE;
	}

	Message_15_4_t* GetOldest(){
		if(this->IsEmpty()){
			return (Message_15_4_t *)(NULL);
		}

		return this->GetFirstFullBuffer();
	}

	Message_15_4_t* GetOldestwithoutRemoval(){
		if(this->IsEmpty()){
			return (Message_15_4_t *)(NULL);
		}

		return this->PeekFirstFullBuffer();
	}

	BOOL IsBufferEmpty(){
		return this->IsEmpty();
	}

	BOOL IsBufferFull(){
		return this->IsFull();
	}

	Message_15_4_t** GetOldestPtr(){
		if(this->IsEmpty()){
			return (Message_15_4_t **)(NULL);
		}

		return this->GetFirstFullBufferPtr();
	}

	Message_15_4_t* GetNewest(){
		if(this->IsEmpty()){
			return (Message_15_4_t *)(NULL);
		}

		return this->GetLastFullBuffer();
	}

	void Erase(){
		this->ClearBuffer();
	}

	void DropOldest(UINT8 numMessages){
		for (int i = 0; i < numMessages; i++){
			GetOldest();
		}
	}

	void DropNewest(UINT8 numMessages){
		for (int i = 0; i < numMessages; i++){
			GetNewest();
		}
	}
};


#endif

//#include "Message.h"

/*
template<class MessageT>
struct Buffer : public HAL_DblLinkedNode<Buffer<MessageT> > {
	MessageT* msg;
};

template<class BufferT, class MessageT, UINT8 ListSizeT>
struct BufferList  {
	HAL_DblLinkedList<BufferT> List;
	BufferT buf[ListSizeT];
	MessageT msg[ListSizeT];

	void Initialize(){
		int i=0;
		//List.Insert(NULL,&buf[1],buf);
		buf[0].Initialize();
		buf[0].msg = &msg[0];
		buf[i].SetNext(&buf[1]);

		for (i=1; i<ListSizeT-1; i++){
			buf[i].msg = &msg[i];
			buf[i].Initialize();
			buf[i].SetNext(&buf[i+1]);
			buf[i].SetPrev(&buf[i-1]);
		}
		buf[ListSizeT-1].SetPrev(&buf[ListSizeT-2]);
		buf[ListSizeT-1].msg = &msg[ListSizeT-1];

		List.Initialize();
	}
};

*/
