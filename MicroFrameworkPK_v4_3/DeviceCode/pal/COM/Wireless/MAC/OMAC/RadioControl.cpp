/*
 * RadioWrapper.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan
 */


#include <Samraksh/Radio_decl.h>
#include <Samraksh/Message.h>
#include "RadioControl.h"

DeviceStatus RadioControl::Initialize(UINT8 radioID){
	RadioID = radioID;
	return DS_Success;
}

DeviceStatus RadioControl::Send(RadioAddress_t address, Message_15_4_t * msg, UINT16 size){
	CPU_Radio_Send(RadioID, msg, sizeof(Message_15_4_t));
	return DS_Success;
}

DeviceStatus RadioControl::Send_TimeStamped(RadioAddress_t address, Message_15_4_t * msg, UINT16 size, UINT32 eventTime){
	CPU_Radio_Send_TimeStamped(RadioID, msg, sizeof(Message_15_4_t), eventTime);
	return DS_Success;
}


//DeviceStatus RadioControl::Receive(Message_15_4_t * msg, UINT16 size){
//		return DS_Success;
//}

DeviceStatus RadioControl::Stop(){
	return DS_Success;
}

DeviceStatus RadioControl::Start(){
	return DS_Success;
}
