/*
 * RadioControl.h
 *
 *  Created on: Sep 10, 2012
 *      Author: Mukundan
 */

#ifndef RADIOCONTROL_H_
#define RADIOCONTROL_H_

#include <Samraksh/Message.h>
#include "OMAC.h"

/*
 * Radio Wrapper Class
 */
typedef class RadioControl{
private:
	bool PiggbackTimeSyncMessage(Message_15_4_t* msg, UINT16 &size);
	bool PiggbackDiscoMessage(Message_15_4_t* msg, UINT16 &size);
public:
	DeviceStatus Initialize();
	DeviceStatus StartPLL();
	DeviceStatus StartRx();
	DeviceStatus Preload(RadioAddress_t address, Message_15_4_t * msg, UINT16 size);
	DeviceStatus Send(RadioAddress_t address, Message_15_4_t * msg, UINT16 size);
	DeviceStatus Receive(Message_15_4_t * msg, UINT16 &size);
	DeviceStatus Stop();

	bool PiggbackMessages(Message_15_4_t* msg, UINT16 &size);


}RadioControl_t;


#endif /* RADIOCONTROL_H_ */
