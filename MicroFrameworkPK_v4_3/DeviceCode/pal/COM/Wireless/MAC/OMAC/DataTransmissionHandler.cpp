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


//#define DATATXPIN 29 //2

extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;
extern Buffer_15_4_t g_send_buffer;
//extern Buffer_15_4_t g_receive_buffer;
extern NeighborTable g_NeighborTable;
extern RadioControl_t g_omac_RadioControl;
DataTransmissionHandler g_DataTransmissionHandler;


UINT64 DataTransmissionHandler::GetTxTicks()
{
	return m_nextTXTicks;
}

UINT32 DataTransmissionHandler::GetTxCounter()
{
	return m_nextTXCounter;
}

void DataTransmissionHandler::SetTxTicks(UINT64 tmp_nextTXTicks)
{
	m_nextTXTicks = tmp_nextTXTicks;
}

void DataTransmissionHandler::SetTxCounter(UINT32 tmp_nextTXCounter)
{
	m_nextTXCounter = tmp_nextTXCounter;
}

INT64 DataTransmissionHandler::GetNeighborGlobalTime()
{
	return m_nbrGlobalTime;
}

UINT32 DataTransmissionHandler::GetNeighborWakeupSlot()
{
	return m_nbrWakeupSlot;
}

void DataTransmissionHandler::SetNeighborGlobalTime(INT64 tmp_nbrGlobalTime)
{
	m_nbrGlobalTime = tmp_nbrGlobalTime;
}

void DataTransmissionHandler::SetNeighborWakeupSlot(UINT32 tmp_nbrWakeupSlot)
{
	m_nbrWakeupSlot = tmp_nbrWakeupSlot;
}


/*
 *
 */
void DataTransmissionHandler::Initialize(){
	CPU_GPIO_EnableOutputPin(DATATX_PIN, TRUE);
	m_dsn = 0;
	m_retryCnt = m_dwellCnt=0;
	randVal = 0;
	m_nextDestination=0;
	m_receivedDataBeacon=0;
	m_busy =FALSE;   //indicates if radio is busy.

	//the # of times the current packet has been sent
	m_dataHeartBeats=0;
	//m_nextTXCounter=0;
	//m_nextTXTicks=0;
	m_lastSlot=0;
	m_collisionCnt = 0;
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
}


/*
 * This function returns the number of ticks until the transmission time
 */
UINT64 DataTransmissionHandler::NextEvent(UINT32 currentSlotNum){
	m_lastSlot++;
	//hal_printf("DataTransmissionHandler::NextEvent currentSlotNum: %u\n", currentSlotNum);
	//nextTXCounter - used to indicate if a tx happened or not.

	//if (g_send_buffer.Size() > 0 && nextTXCounter == 0) {
	/*if(m_lastSlot % 100 == 0){
		hal_printf("----------->g_send_buffer size: %u<--------------\n", g_send_buffer.Size());
	}*/

	if (g_send_buffer.Size() > 0) {
		/*if(m_lastSlot % 100 == 0){
			hal_printf("----------->CALLING ScheduleDataPacket<--------------\n");
		}*/
		this->ScheduleDataPacket();
	}

	UINT32 nextTXCounter = GetTxCounter();
	UINT64 nextTXTicks = GetTxTicks();

	/*if (nextTXCounter != 0) {
		if(nextTXTicks < HAL_Time_CurrentTicks()) {
			//We have already gone past the scheduled time, so don't send
			return MAX_UINT16;
		}
		else {
			UINT64 ticksInMicroSecs = CPU_TicksToMicroseconds(nextTXTicks - HAL_Time_CurrentTicks());
			//return (UINT16)(ticksInMicroSecs/1000);
			////hal_printf("\n[%llu%llu] DataTransmissionHandler:NextEvent nextEventsMicroSec=%llu nexxEventTime = %llu\n",HAL_Time_CurrentTime(), g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTime()), ticksInMicroSecs, ticksInMicroSecs +HAL_Time_CurrentTime() );
			return ticksInMicroSecs;
		}
	}
	else {
		//No data packet is scheduled, so wait for a long period
		return MAX_UINT64;
	}*/

	//m_nextTXCounter is initialized to 0
	if (nextTXCounter != 0) {
		INT32 remainingSlots = nextTXCounter - currentSlotNum;
		//if (remainingSlots >= 0 && remainingSlots > (MAX_UINT16 >> SLOT_PERIOD_BITS)) {
		if (remainingSlots >= 0) {
			//return MAX_UINT16;
			//return remainingSlots;
			return CPU_TicksToMicroseconds((UINT32)remainingSlots * SLOT_PERIOD_TICKS);
		}
		else {
			//in case the task delay is large and we are already pass
			//tx time, tx immediately
			if(nextTXTicks < HAL_Time_CurrentTicks()) {
				//We have already gone past the scheduled time, so don't send
				return MAX_UINT16;
				//return 0;
			}
			else {
				UINT64 ticksInMicroSecs = CPU_TicksToMicroseconds(nextTXTicks - HAL_Time_CurrentTicks());
				//return (UINT16)(ticksInMicroSecs/1000);
				////hal_printf("\n[%llu%llu] DataTransmissionHandler:NextEvent nextEventsMicroSec=%llu nexxEventTime = %llu\n",HAL_Time_CurrentTime(), g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTime()), ticksInMicroSecs, ticksInMicroSecs +HAL_Time_CurrentTime() );
				return ticksInMicroSecs;
			}
		}

	} else {
		////hal_printf("nextTXCounter: %u\n", nextTXCounter);
		////hal_printf("nextTXTicks: %llu\n", nextTXTicks);
		//No data packet is scheduled, so wait for a long period
		return MAX_UINT64;
	}
}

/*
 *
 */
void DataTransmissionHandler::ExecuteEvent(UINT32 currentSlotNum){
	//BK: At this point there should be some message to be sent in the m_outgoingEntryPtr
	////hal_printf("\n[%llu%llu] DataTransmissionHandler:ExecuteEvent\n",HAL_Time_CurrentTime(), g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTime()));
	CPU_GPIO_SetPinState( DATATX_PIN, TRUE );
	bool rv = Send();
	CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
	SetTxCounter(0);
	PostExecuteEvent();
}

/*
 *
 */
/*UINT8 DataTransmissionHandler::ExecuteEventDone(){
	return 0;
}*/

/*
 *
 */
void DataTransmissionHandler::PostExecuteEvent(){
	////hal_printf("DataTransmissionHandler::PostExecuteEvent\n");
	//stop the radio
	g_omac_scheduler.PostExecution();
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
	if(m_outgoingEntryPtr == NULL){
		ASSERT(1);
	}

	DeviceStatus rs;

	if(g_send_buffer.GetNumberMessagesInBuffer() == 0){
		return false;
	}

	//Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
	//m_outgoingEntryPtr->GetHeader()->dest = sn->MacAddress;
	IEEE802_15_4_Header_t * header = m_outgoingEntryPtr->GetHeader();
	header->type = MFM_DATA;

	//for(int i = 0; i < 50; i++){
		//HAL_Time_Sleep_MicroSeconds(500);

	//CPU_GPIO_SetPinState( DATATX_PIN, TRUE );

	if (m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp() == 0) {
		////hal_printf("DataTransmissionHandler::Send calling Send\n");
		rs = g_omac_RadioControl.Send(m_outgoingEntryPtr->GetHeader()->dest, m_outgoingEntryPtr, m_outgoingEntryPtr->GetMessageSize(), 0 );
	}
	else {
		//hal_printf("DataTransmissionHandler::Send msg size %u\n", m_outgoingEntryPtr->GetMessageSize());
		rs = g_omac_RadioControl.Send(m_outgoingEntryPtr->GetHeader()->dest, m_outgoingEntryPtr, m_outgoingEntryPtr->GetMessageSize(), m_outgoingEntryPtr->GetMetaData()->GetReceiveTimeStamp()  );
	}
	//}

	if(rs != DS_Success)
		return false;
	else
		return true;

	//hal_printf("send status: %d\n", rs);
	//CPU_GPIO_SetPinState( DATATX_PIN, FALSE );
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
	UINT64 nextTXTicks = GetTxTicks();

	if ( ( (nextTXTicks + (SLOT_PERIOD_MILLI * TICKS_PER_MILLI)) < HAL_Time_CurrentTicks() ) && g_send_buffer.GetNumberMessagesInBuffer() > 0){
		UINT16 dest;
		Neighbor_t* neighborEntry;

		// Don't reschedule another tx for packets that are being sent
		if (g_omac_scheduler.InputState.IsState(I_DATA_SEND_PENDING)) {
			////hal_printf("**********DataTransmissionHandler::ScheduleDataPacket input state is I_DATA_SEND_PENDING. returning...**********\n");
			return;
		}

		//TODO: BK: THis returns m_outgoingEntryPtr is NULL. FindFirstSyncedNbrMessage should be reimplemented //DONE
		if (m_outgoingEntryPtr == NULL) {
			//m_outgoingEntryPtr = g_OMAC.FindFirstSyncedNbrMessage();
			m_outgoingEntryPtr = g_send_buffer.GetOldest();
			//m_outgoingEntryPtr = g_send_buffer.GetNextFreeBuffer();
			if (m_outgoingEntryPtr == NULL) {
				////hal_printf("**********m_outgoingEntryPtr is null**********\n");
				return;
			}
		}

		//Neighbor_t* sn = g_NeighborTable.GetMostObsoleteTimeSyncNeighborPtr();
		//m_outgoingEntryPtr->GetHeader()->dest = sn->MacAddress;
		dest = m_outgoingEntryPtr->GetHeader()->dest;
		//hal_printf("DataTransmissionHandler::ScheduleDataPacket dest is: %d\n", dest);
		neighborEntry = g_NeighborTable.GetNeighborPtr(dest);
		/*hal_printf("-----------------\n");
		hal_printf("neighborEntry->LastHeardTime %llu\n", neighborEntry->LastHeardTime);
		hal_printf("neighborEntry->MacAddress %u\n", neighborEntry->MacAddress);
		hal_printf("-----------------\n");*/

		if (neighborEntry != NULL) {
			////hal_printf("------------>NEIGHBORENTRY IS NOT NULL<----------------\n");
			//next wake up slot determined neighbor's dataInterval
			UINT32 neighborSlotNumber, neighborNextFrameAfterSeedUpdate;
			INT64 neighborGlobalTime;

			UINT32 dataInterval = neighborEntry->dataInterval;
			UINT32 seedUpdateInterval = (dataInterval << 3);
			INT16 delayDiff;
			//UINT16 myRadioDelay = call SlotScheduler.getRadioDelay();
			UINT16 myRadioDelay = 0;
			UINT16 framesPassed = 0, seedUpdates = 0, mask, randVal, tmpSeed, lastSeed;
			UINT32 neighborNextWakeup;
			UINT8 i;

			if (neighborEntry->MacAddress != dest) {
				hal_printf("incorrect neighbor returned\n");
			}
			// 1st,  compute the current localTime of the neighbor
			neighborGlobalTime = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(dest, HAL_Time_CurrentTicks());
			// 2nd, compute neighbor's current slotNumber value. The start of the slotNumber
			// is later than the start of the clock. So, a slotNumber offset should be used
			//slotNumber = (neighborGlobalTime - neighborEntry->counterOffset) >> SLOT_PERIOD_BITS;
			// BK: The slot number now depends only on the CurrentTicks. This design reduces data exchange and also deals with cases where data slot updates are missed due to some reason.
			neighborSlotNumber = (UINT32)(neighborGlobalTime / SLOT_PERIOD_TICKS);

			mask = 137 * 29 * (dest + 1);
			// seed consistency is critical. Because the following operations involve
			// changing the seed based on separate blocks of read and write,
			// we need to have mutual exclusions during the whole update process. This may
			// prevent the insertion of a newly received seed, which is fine because
			// new seeds are pseudo-random and can be locally generated
			GLOBAL_LOCK(irq);
			lastSeed = neighborEntry->lastSeed;
			neighborNextFrameAfterSeedUpdate = neighborEntry->nextFrameAfterSeedUpdate;

			// A new frame-seed, generated from the last frame-seed, is used to generate the wakeup
			// slot to wakeup for every 8 frames. Frame length is defined by dataInterval.
			//
			// If the current frame-seed is still valid, i.e. if there has been a seed update within the
			// last 8 frames, then we simply use this frame-seed to generate the wakeup slot.
			// Although a node periodically updates its <seed, nextFrameAfterSeedUpdate>, it never
			// updates it beyond the current time. In other words, nextFrameAfterSeedUpdate will not be
			// updated beyond slotNumber. THis shows that the nextFrameAfterSeedUpdate in this case
			// must have been updated by the neighbor itself
			if (neighborSlotNumber <= neighborNextFrameAfterSeedUpdate) {
				tmpSeed = lastSeed;
				randVal = g_omac_scheduler.m_seedGenerator.RandWithMask(&tmpSeed, mask);
				tmpSeed = randVal;
				neighborNextWakeup = neighborNextFrameAfterSeedUpdate - dataInterval + (randVal % dataInterval);
				if (neighborSlotNumber < neighborNextWakeup) {
					neighborSlotNumber = neighborNextWakeup;
				} else {
					neighborSlotNumber = neighborNextWakeup + dataInterval;
					//printf("1 cannot use slot %u, using slotNumber=%u\n"
					//  , (UINT16)nextWakeup, (UINT16)slotNumber);
				}
			}else {
				framesPassed = (neighborSlotNumber - neighborNextFrameAfterSeedUpdate) / dataInterval;
				seedUpdates = (neighborSlotNumber + dataInterval - neighborNextFrameAfterSeedUpdate) / seedUpdateInterval;

				for (i = 0; i < seedUpdates; i++) {
					lastSeed= g_omac_scheduler.m_seedGenerator.RandWithMask(&lastSeed, mask);
				}

				tmpSeed = lastSeed;
				randVal = g_omac_scheduler.m_seedGenerator.RandWithMask(&tmpSeed, mask);
				neighborNextWakeup = neighborNextFrameAfterSeedUpdate + (framesPassed * dataInterval) + (randVal % dataInterval);
				if (neighborSlotNumber < neighborNextWakeup) {
					neighborSlotNumber = neighborNextWakeup;
				} else {
					  //printf("now=%u\n", (UINT16)slotNumber);
					if (neighborNextWakeup + dataInterval -(neighborNextFrameAfterSeedUpdate - dataInterval + seedUpdateInterval * seedUpdates) >= seedUpdateInterval) {
						lastSeed = tmpSeed;
						seedUpdates++;
						framesPassed++;
						randVal = g_omac_scheduler.m_seedGenerator.RandWithMask(&tmpSeed, mask);
						neighborSlotNumber = neighborNextFrameAfterSeedUpdate + (framesPassed * dataInterval) + (randVal % dataInterval);
					} else {
						neighborSlotNumber = neighborNextWakeup + dataInterval;
					}
				}
			}

			////hal_printf("DataTransmissionHandler::ScheduleDataPacket -- lastSeed is %u\n", lastSeed);
			////hal_printf("DataTransmissionHandler::ScheduleDataPacket -- currentRandomValue is %u\n", randVal);
			nextTXTicks = (neighborSlotNumber << SLOT_PERIOD_BITS) + neighborEntry->counterOffset;
			neighborGlobalTime = nextTXTicks;
			SetCurrentRandomValue(randVal);
			SetNeighborNextWakeup(neighborNextWakeup);
			SetNeighborWakeupSlot(neighborSlotNumber);
			SetNeighborGlobalTime(neighborGlobalTime);
			////hal_printf("DataTransmissionHandler::ScheduleDataPacket -- currentRandomValue is %u\n", GetCurrentRandomValue());

			// 5th, compute my local time of the neighbor's listen slot
			nextTXTicks = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(dest, nextTXTicks);
			nextTXTicks = nextTXTicks + SENDER_MARGIN;
			//hal_printf("--------->My local time of neighbor's listen slot %llu<----------\n", nextTXTicks);
			SetTxTicks(nextTXTicks);

			neighborEntry->nextFrameAfterSeedUpdate = neighborNextFrameAfterSeedUpdate + seedUpdates * seedUpdateInterval;
			neighborEntry->lastSeed = lastSeed;
		//}
		  //hal_printf("receiver wakes up at %lu, %lu\n", m_nextTXTicks, globalTime);

		  //m_nextTXCounter = (m_nextTXTicks - g_omac_scheduler.GetCounterOffset()) >> SLOT_PERIOD_BITS;
		  UINT32 nextTXCounter = (nextTXTicks - g_omac_scheduler.GetCounterOffset()) >> SLOT_PERIOD_BITS;
		  SetTxCounter(nextTXCounter);
		  //hal_printf("m_nextTick=%lu, glbl=%lu\n", m_nextTXTicks, globalTime);
		  //hal_printf("sslot %lu time %lu\n", slotNumber, globalTime);
		  //hal_printf("\n ScheduleDataPacket CurTicks: %llu slotNumber: %d neighborEntry->counterOffset: %d m_nextTXCounter: %d \n",HAL_Time_CurrentTicks(), slotNumber, neighborEntry->counterOffset, m_nextTXCounter);

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

