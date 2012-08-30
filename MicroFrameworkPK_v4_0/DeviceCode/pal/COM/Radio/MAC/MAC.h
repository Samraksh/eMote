/*
 * 	Name 		: MAC.h
 *
 *  Author 		: mukundan.sridharan, nived.sivadas
 *
 *  Description : Common MAC Layer Base Class Definition, v 1.0 Aug 28, 2012
 *  			  Modified some of the return types etc, v1.01	Aug 30, 2012
 */


#ifndef _MAC_H_
#define _MAC_H_
#include <tinyhal.h>
#include <Hal_util.h>

// Nived.Sivadas:
// Disabling template for now until i can figure out if its needed
//template <class MessageT>
class MAC
{
private:
    static UINT8 unique_mac_id_;
	UINT8 mac_id_;
	RadioEventHandler radio_event_handler_;
	uint32 radio_interrupt_mask_;

public:
	///Virtual Methods
	virtual bool Send(void* msg, int Size)=0;
	virtual void* ReceiveHandler(void* msg, int Size)=0;
	virtual bool RadioInterruptHandler(RadioInterrupt Interrupt, void* Param)=0;
	virtual bool SendAckHandler(void *msg, int Size)=0;


	uint8 GetUniqueMacID(){
		return unique_mac_id_++;
	}

	MAC(){
		// Define all the interrupt handlers
		radio_interrupt_mask_ = 0;
		radio_event_handler_.set_recieve_handler(ReceiveHandler);
		radio_event_handler_.set_radio_interrupt_handler(RadioInterruptHandler);
		radio_event_handler_.set_send_ack_handler(SendAckHandler);

		mac_id_ = GetUniqueMacID();
	}
};

// Define static member for the template class
template<class MessageT>
MAC<MessageT>::unique_mac_id_ = 0;

#endif
