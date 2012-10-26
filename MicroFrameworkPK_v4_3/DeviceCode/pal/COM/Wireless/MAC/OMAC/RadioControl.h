/*
 * RadioControl.h
 *
 *  Created on: Sep 10, 2012
 *      Author: Mukundan
 */

#ifndef RADIOCONTROL_H_
#define RADIOCONTROL_H_

#include <Samraksh/Message.h>

//Radio Wrapper Class
typedef class RadioControl{
	UINT8 RadioID;
  public:
	DeviceStatus Initialize(UINT8 RadioID);
	DeviceStatus Start();
	DeviceStatus Send(RadioAddress_t address, Message_15_4_t * msg, UINT16 size);
	DeviceStatus Send_TimeStamped(RadioAddress_t address, Message_15_4_t * msg, UINT16 size, UINT32 eventTime);
	DeviceStatus Receive(Message_15_4_t * msg, UINT16 size);
	DeviceStatus Stop();
}RadioControl_t;


#endif /* RADIOCONTROL_H_ */
