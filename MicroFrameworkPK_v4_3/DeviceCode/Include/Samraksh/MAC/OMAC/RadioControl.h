/*
 * RadioControl.h
 *
 *  Created on: Sep 10, 2012
 *      Author: Mukundan
 */

#ifndef RADIOCONTROL_H_
#define RADIOCONTROL_H_

#include <Samraksh/Message.h>

/*
 * Radio Wrapper Class
 */
class RadioControl_t{
private:
	bool PiggybackTimeSyncMessage(Message_15_4_t* msg, UINT16 &size);
	bool PiggybackDiscoMessage(Message_15_4_t* msg, UINT16 &size);


	UINT64 GCTIT;
	UINT64 GCTIT2;
	UINT64 GCTIT3;
	UINT64 GCTIT4;


public:
	DeviceStatus Initialize();
	DeviceStatus Uninitialize();
	DeviceStatus StartPLL();
	DeviceStatus StartRx();
	DeviceStatus Preload(RadioAddress_t address, Message_15_4_t * msg, UINT16 size);
	DeviceStatus Send(RadioAddress_t address, Message_15_4_t * msg, UINT16 size);
	DeviceStatus Receive(Message_15_4_t * msg, UINT16 &size);
	DeviceStatus Stop();

	bool PiggybackMessages(Message_15_4_t* msg, UINT16 &size);
	bool stayOn;


};


#endif /* RADIOCONTROL_H_ */
