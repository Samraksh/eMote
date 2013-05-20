/*
 * Handlers.h
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan
 */

#ifndef HANDLERS_H_
#define HANDLERS_H_

#include <tinyhal.h>
#include <Samraksh/Hal_Util.h>
#include <Samraksh/Message.h>


class SlotHandler {
  public:
	void Initialize();
	// the scheduler calls this command at the beginning of a slot;
	// @param slotNum the current slot number
	//  @return the handler's offset (in jitters) in the slot that the radio should be started.
	// a return value of 0 is to start the radio immediately
	UINT16 NextSlot(UINT32 slotNum);
	// the scheduler calls this command at the slot winner
	// @param slotNum the current slot number
	void ExecuteSlot(UINT32 slotNum);
	// the handler signals this event once its access to the radio is finished
	UINT8 ExecuteSlotDone();
	// the scheduler calls this command after the radio is stopped
	void PostExecuteSlot();

	void SetWakeup(bool shldWakeup);
};


#endif /* HANDLERS_H_ */
