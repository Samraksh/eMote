/*
 * DataTransmissionHandler.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: Mukundan Sridharan
 */

//#include <Samraksh/Neighbors.h>
#include <Samraksh/MAC/OMAC/DataTransmissionHandler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/MAC/OMAC/Scheduler.h>
#include <Samraksh/MAC/OMAC/RadioControl.h>

extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;
extern Buffer_15_4_t g_send_buffer;
//extern Buffer_15_4_t g_receive_buffer;
extern NeighborTable g_NeighborTable;
extern RadioControl_t g_omac_RadioControl;


/*
 *
 */
void DataTransmissionHandler::Initialize(){
	m_dsn = 0;
	m_retryCnt = m_dwellCnt=0;
	randVal = 0;
	m_nextDestination=0;
	m_receivedDataBeacon=0;
	m_busy =FALSE;   //indicates if radio is busy.

	//the # of times the current packet has been sent
	m_dataHeartBeats=0;
	m_nextTXCounter=0;
	m_nextTXTicks=0;
	m_lastSlot=0;
	m_collisionCnt = 0;
}

/*
 * This function returns the number of ticks until the transmission time
 */
UINT16 DataTransmissionHandler::NextEvent(UINT32 currentSlotNum){
	m_lastSlot++;
	if (g_send_buffer.Size() > 0) {
		this->ScheduleDataPacket();
	}

	//m_nextTXCounter is initialized to 0
	if (m_nextTXCounter != 0) {
		INT32 remainingSlots = m_nextTXCounter - currentSlotNum;
		if (remainingSlots >= 0 && remainingSlots > (0xffff >> SLOT_PERIOD_BITS)) {
			return 0xffff;
		}
		else {
			//in case the task delay is large and we are already pass
			//tx time, tx immediately
			if(m_nextTXTicks < HAL_Time_CurrentTicks()) {
				return 0xffff;
			}
			else {
				UINT64 ticksInMicroSecs = CPU_TicksToMicroseconds(m_nextTXTicks - HAL_Time_CurrentTicks());
				//return (UINT16)(ticksInMicroSecs/1000);
				return ticksInMicroSecs;
			}
		}
	} else {
		return 0xffff;
	}
}

/*
 *
 */
void DataTransmissionHandler::ExecuteEvent(UINT32 currentSlotNum){
	//BK: At this point there should be some message to be sent in the m_outgoingEntryPtr
	bool rv = Send();
}

/*
 *
 */
UINT8 DataTransmissionHandler::ExecuteEventDone(){
	return 0;
}

/*
 *
 */
void DataTransmissionHandler::PostExecuteEvent(){
	hal_printf("DataTransmissionHandler::PostExecuteEvent\n");
}

/*
 *
 */
void DataTransmissionHandler::SetWakeup(bool shldWakeup){
	hal_printf("DataTransmissionHandler::SetWakeup\n");
}

/*
 *
 */
void DataTransmissionHandler::DataBeaconReceive(UINT8 type, Message_15_4_t *msg, UINT8 size){
	/*
	IEEE802_15_4_Header_t * headerPtr = msg->GetHeader();
	 // For sender, make sure that
	 // 1) we're waiting for a beacon : m_state == S_DATA_PRELOADED
	 // 2) the message is actually a beacon and not an ack:
	 //    type == OMAC_DATA_BEACON_TYPE && dest == BCAST
	 // 3) that it's from the receiver we're expecting
	switch (g_OMAC.m_omac_scheduler.ProtoState.GetState()) {
	  case S_DATA_PRELOADED:
		if (headerPtr->src == m_nextDestination && type == OMAC_DATA_BEACON_TYPE) {
		  if (g_OMAC.m_omac_scheduler.InputState.IsState(I_DWELL_SEND) && headerPtr->dest == RADIO_BROADCAST_ADDRESS) {
			 m_receivedDataBeacon = TRUE;
#ifdef OMAC_DEBUG
			hal_printf("tx1\n");
#endif
			call FIFOPreload.txPacket();
			atomic call PreloadTimer.stop();

			//the second condition is important. ControlBeaconHandlerC directly operates on
			//m_piggybackBeaconCopy. So, do not modify it while it is being processed.
#ifndef ORIGINAL_OMAC
			if (headerPtr->type == AM_TIMESYNCMSG) {
			  call TimeSyncInfo.insertBeacon(msg);
			  hal_printf("piggy1\n");
			}
#endif
		  }
		  //if we find out that some receiver is in the process of receiving,
		  //then cancel the current transmission
		  else if (headerPtr->dest != CPU_Radio_GetAddress(this->radioName)) {
			hal_printf("cancel2\n");
			cancelTX();
			call FIFOPreload.txPacket();
			atomic call PreloadTimer.stop();
		  }
		}
		break;
	  case S_SENDING_DATA:
		// if we receive another data beacon after receiving our data beacon
		// cancel the tx as it might lead to collisions
#ifdef ORIGINAL_OMAC
		if (type == IEEE154_TYPE_DATA && headerPtr->dest == TOS_NODE_ID
			&& headerPtr->type == AM_TIMESYNCMSG) {

		  call TimeSyncInfo.insertBeacon(msg);
		  hal_printf("piggy2\n");
		}
#endif
		if (type == OMAC_DATA_BEACON_TYPE) {
		  //ack for others. cancel our transmission and go to sleep
		  hal_printf("cancel3\n");
		  cancelTX();
		}
		break;
	  default:
		break;
		//for receiver: we are waiting for data packet after our beacon
	}
*/
}

/*
 *
 */
bool DataTransmissionHandler::Send(){
	ASSERT(m_outgoingEntryPtr != NULL);

	//Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
	//m_outgoingEntryPtr->GetHeader()->dest = sn->MacAddress;
	IEEE802_15_4_Header_t * header = m_outgoingEntryPtr->GetHeader();
	header->type = MFM_DATA;

	if (m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp() == 0) {
		hal_printf("DataTransmissionHandler::Send calling Send\n");
		DeviceStatus rs = g_omac_RadioControl.Send(m_outgoingEntryPtr->GetHeader()->dest, m_outgoingEntryPtr, m_outgoingEntryPtr->GetMessageSize()  );
	}
	else {
		hal_printf("DataTransmissionHandler::Send calling Send_TimeStamped\n");
		DeviceStatus rs = g_omac_RadioControl.Send_TimeStamped(m_outgoingEntryPtr->GetHeader()->dest, m_outgoingEntryPtr , m_outgoingEntryPtr->GetMessageSize(), \
																												m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp()  );
	}
	return true;
}

/*
 *
 */
void DataTransmissionHandler::ScheduleDataPacket()
{
	// do not schedule a new tx if
	// 1) we've already scheduled an tx task (m_nextTXTicks >= now), or
	// 2) we have nothing to send
	//3) when the radio is idle.

	static bool allNeighborFlag = false;

	if ((m_nextTXTicks + SLOT_PERIOD_MILLI * TICKS_PER_MILLI < HAL_Time_CurrentTicks()) && g_send_buffer.Size() > 0){
		UINT16 dest;
		Neighbor_t* neighborEntry;

		// Don't reschedule another tx for packets that are being sent
		if (g_omac_scheduler.InputState.IsState(I_DATA_SEND_PENDING)) {
			//hal_printf("DataTransmissionHandler::ScheduleDataPacket input state is I_DATA_SEND_PENDING. returning...\n");
			return;
		}

		//TODO: BK: THis returns m_outgoingEntryPtr is NULL. FindFirstSyncedNbrMessage should be reimplemented //DONE
		if (m_outgoingEntryPtr == NULL) {
			//m_outgoingEntryPtr = g_OMAC.FindFirstSyncedNbrMessage();
			m_outgoingEntryPtr = g_send_buffer.GetOldest();
			//m_outgoingEntryPtr = g_send_buffer.GetNextFreeBuffer();
			if (m_outgoingEntryPtr == NULL) {
				//hal_printf("m_outgoingEntryPtr is null\n");
				return;
			}
		}

		Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
		m_outgoingEntryPtr->GetHeader()->dest = sn->MacAddress;
		dest = m_outgoingEntryPtr->GetHeader()->dest;
		//hal_printf("DataTransmissionHandler::ScheduleDataPacket dest is: %d\n", dest);
		neighborEntry =  g_NeighborTable.GetNeighborPtr(dest);

		if (neighborEntry != NULL) {
			//next wake up slot determined neighbor's dataInterval
			UINT32 counter, nextFrameAfterSeedUpdate;
			INT64 nbrGlobalTime;

			UINT32 p = neighborEntry->dataInterval, seedUpdateInterval = (p << 3);
			INT16 delayDiff;
			//UINT16 myRadioDelay = call SlotScheduler.getRadioDelay();
			UINT16 myRadioDelay = 0;
			UINT16 framesPassed = 0, seedUpdates = 0, mask, tmpRand, tmpSeed, lastSeed;
			UINT32 nextWakeup;
			UINT8 i;

			if (neighborEntry->MacAddress != dest) {
				hal_printf("incorrect neighbor returned\n");
			}
			// 1st,  compute the current localTime of the neighbor
			nbrGlobalTime = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, HAL_Time_CurrentTicks());
			// 2nd, compute neighbor's current counter value. The start of the counter
			// is later than the start of the clock. So, a counter offset should be used
			counter = (nbrGlobalTime - neighborEntry->counterOffset) >> SLOT_PERIOD_BITS;
			mask = 137 * 29 * (dest + 1);
			// seed consistency is critical. Because the following operations involve
			// changing the seed based on separate blocks of read and write,
			// we need to have mutual exclusions during the whole update process. This may
			// prevent the insertion of a newly received seed, which is fine because
			// new seeds are pseudo-random and can be locally generated
			////GLOBAL_LOCK(irq);
			lastSeed = neighborEntry->lastSeed;
			nextFrameAfterSeedUpdate = neighborEntry->nextFrameAfterSeedUpdate;

			// A new frame-seed, generated from the last frame-seed, is used to generate the wakeup
			// slot to wakeup for every 8 frames. Frame length is defined by dataInterval.
			//
			// If the current frame-seed is still valid, i.e. if there has been a seed update within the
			// last 8 frames, then we simply use this frame-seed to generate the wakeup slot.
			// Although a node periodically updates its <seed, nextFrameAfterSeedUpdate>, it never
			// updates it beyond the current time. In other words, nextFrameAfterSeedUpdate will not be
			// updated beyond counter. THis shows that the nextFrameAfterSeedUpdate in this case
			// must have been updated by the neighbor itself
			if (counter <= nextFrameAfterSeedUpdate) {
				tmpSeed = lastSeed;
				tmpRand = g_omac_scheduler.m_seedGenerator.RandWithMask(&tmpSeed, mask);
				////hal_printf("DataTransmissionHandler::ScheduleDataPacket1 -- tmpRand is %u\n", tmpRand);
				tmpSeed = tmpRand;
				nextWakeup = nextFrameAfterSeedUpdate - p + tmpRand % p;
				if (counter < nextWakeup) {
					counter = nextWakeup;
				} else {
					counter = nextWakeup + p;
					//printf("1 cannot use slot %u, using counter=%u\n"
					//  , (UINT16)nextWakeup, (UINT16)counter);
				}
			}else {
				framesPassed = (counter - nextFrameAfterSeedUpdate) / p;
				seedUpdates = (counter + p - nextFrameAfterSeedUpdate) / seedUpdateInterval;

				for (i = 0; i < seedUpdates; i++) {
					lastSeed= g_omac_scheduler.m_seedGenerator.RandWithMask(&lastSeed, mask);
				}

				tmpSeed = lastSeed;
				tmpRand = g_omac_scheduler.m_seedGenerator.RandWithMask(&tmpSeed, mask);
				////hal_printf("DataTransmissionHandler::ScheduleDataPacket2 -- tmpRand is %u\n", tmpRand);
				nextWakeup = nextFrameAfterSeedUpdate + framesPassed * p + tmpRand % p;
				if (counter < nextWakeup) {
					counter = nextWakeup;
				} else {
					  //printf("now=%u\n", (UINT16)counter);
					if (nextWakeup + p -(nextFrameAfterSeedUpdate - p + seedUpdateInterval * seedUpdates) >= seedUpdateInterval) {
						lastSeed = tmpSeed;
						seedUpdates++;
						framesPassed++;
						tmpRand = g_omac_scheduler.m_seedGenerator.RandWithMask(&tmpSeed, mask);
						counter = nextFrameAfterSeedUpdate + framesPassed * p + tmpRand % p;
					} else {
						counter = nextWakeup + p;
					}
				}
				//hal_printf("using seed %u\n", lastSeed);
				m_nextTXTicks = (counter << SLOT_PERIOD_BITS) + neighborEntry->counterOffset;
				nbrGlobalTime = m_nextTXTicks;

				// 5th, compute my local time of the neighbor's listen slot
				m_nextTXTicks = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(dest, m_nextTXTicks);
				m_nextTXTicks = m_nextTXTicks + SENDER_MARGIN;

				neighborEntry->nextFrameAfterSeedUpdate = nextFrameAfterSeedUpdate + seedUpdates * seedUpdateInterval;
				neighborEntry->lastSeed = lastSeed;
			}
			  //hal_printf("receiver wakes up at %lu, %lu\n", m_nextTXTicks, globalTime);

			  m_nextTXCounter = (m_nextTXTicks - g_omac_scheduler.GetCounterOffset()) >> SLOT_PERIOD_BITS;
			  //hal_printf("m_nextTick=%lu, glbl=%lu\n", m_nextTXTicks, globalTime);
			  //hal_printf("sslot %lu time %lu\n", counter, globalTime);
			  hal_printf("ScheduleDataPacket CurTicks: %llu counter: %d neighborEntry->counterOffset: %d m_nextTXCounter: %d \n",HAL_Time_CurrentTicks(), counter, neighborEntry->counterOffset, m_nextTXCounter);

		}
		else if(dest == 0xFFFF || dest == 0xC000){	//TODO (Ananth): where does c000 come from?
			if(!allNeighborFlag){
				hal_printf("Cannot find nbr %u\n", dest);
				allNeighborFlag = true;
			}
		}
		else {
			hal_printf("Cannot find nbr %u\n", dest);
		}
	}
}

