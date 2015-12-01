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
class RadioControl_t{
private:
	bool PiggybackTimeSyncMessage(Message_15_4_t* msg, UINT16 &size);
	bool PiggybackDiscoMessage(Message_15_4_t* msg, UINT16 &size);
public:
	DeviceStatus Initialize();
	DeviceStatus StartPLL();
	DeviceStatus StartRx();
	DeviceStatus Preload(RadioAddress_t address, Message_15_4_t * msg, UINT16 size);
	DeviceStatus Send(RadioAddress_t address, Message_15_4_t * msg, UINT16 size);
	DeviceStatus Receive(Message_15_4_t * msg, UINT16 &size);
	DeviceStatus Stop();

	bool PiggybackMessages(Message_15_4_t* msg, UINT16 &size);


};


#endif /* RADIOCONTROL_H_ */
