/*
 * DataReceptionHandler.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan Sridharan
 */

#include "DataReceptionHandler.h"
#include "OMAC.h"

extern OMAC g_OMAC;
extern Buffer_15_4_t g_send_buffer;
extern Buffer_15_4_t g_receive_buffer;
extern RadioControl_t g_omac_RadioControl;


//extern LCD_PCF85162_Driver g_LCD_PCF85162_Driver;
/*
void PublicReceiveCallback(void * param){
	g_scheduler->m_DiscoveryHandler.BeaconNTimerHandler(param);
}
*/


void DataReceptionHandler::Initialize(){
	UINT16 lastSeed;
	m_busy, m_shldWakeup = TRUE;
	UINT8 m_dwellCnt = 0;
	m_receivedDataPacket = m_efdDetected=FALSE;
	m_wakeupCnt =  m_idleListenCnt = m_collisionCnt = m_overhearCnt = 0;
	m_sfdCnt = 0;

	//wakeup slots that receive at least one packet
	m_receivedSlotCnt = 0;
	m_reportPeriod = 10000 / WAKEUP_INTERVAL;

	m_lastBeaconRequestSlot = 0;
	m_nextWakeupSlot = 0;

	wakeupSlot = wakeupTime = scheduledWakeupTime = 0;

	dataBeaconBufferPtr = &dataBeaconBuffer;
	m_dataInterval = WAKEUP_INTERVAL / SLOT_PERIOD_MILLI;
	if (m_dataInterval < 1) {
		m_dataInterval = 10;
	}

	lastSeed = m_nextSeed = 119 * 119 * (MF_NODE_ID + 1);
	m_mask = 137 * 29 * (MF_NODE_ID + 1);
	//m_nextSeed is updated with its passed pointer. It will be the next seed to use
	//after 8 frames pass
	m_nextWakeupSlot =
	g_OMAC.m_omac_scheduler.m_seedGenerator.RandWithMask(&m_nextSeed, m_mask) % m_dataInterval;
	//seed is updated every 8 frames. return the seed to its initial value
	g_OMAC.m_omac_scheduler.m_DiscoveryHandler.SetSeed(lastSeed, m_dataInterval);
	m_seedUpdateInterval = (m_dataInterval << 3);

	m_receivedDataPacket = FALSE;
	m_efdDetected = FALSE;
	m_dwellSource = INVALID_ADDRESS;

	if (m_dataInterval <= 1) {
		hal_printf("ERROR: data interval is less or equal to 1\n");
	}

}


void DataReceptionHandler::ExecuteSlot(UINT32 slotNum){
#ifdef OMAC_DEBUG
	CPU_GPIO_SetPinState((GPIO_PIN) 23, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 23, TRUE);
#endif
	//call ChannelMonitor.monitorChannel();
	//SendDataBeacon(FALSE);
	m_wakeupCnt++;
}

bool DataReceptionHandler::SendDataBeacon(bool sendPiggyBacked){
	g_OMAC.m_omac_scheduler.ProtoState.ForceState(S_WAITING_DATA);
	m_receivedDataPacket = FALSE;
	m_efdDetected = FALSE;
	DataBeacon_t * sndMsg = (DataBeacon_t *) dataBeaconBufferPtr->GetPayload();
	sndMsg->nodeID = MF_NODE_ID;
	dataBeaconBufferPtr->GetHeader()->type = OMAC_DATA_BEACON_TYPE;

	g_omac_RadioControl.Send(RADIO_BROADCAST_ADDRESS, dataBeaconBufferPtr, sizeof(DataBeacon_t) );
	//call DataReceptionTimer.startOneShot(WAIT_TIME_AFTER_DATA_BEACON);
}

UINT8 DataReceptionHandler::ExecuteSlotDone(){

}

void DataReceptionHandler::PostExecuteSlot(){
	/* Two types of symptons if collision occurs:
	 * 1. Cannot receive packet, but there is energy in the channel
	 * 2. Received packet but with incorrect CRC
	 */
#ifdef OMAC_DEBUG
	CPU_GPIO_SetPinState((GPIO_PIN) 23, FALSE);
	//hal_printf("[Lcl %lu] Radio stopped\n", call GlobalTime.getLocalTime());
#endif
	if (m_wakeupCnt % m_reportPeriod == 0) {
		hal_printf("wkCnt=%u,rxCnt=%u,collision=%u,idle=%u,overhear=%u\n",
			m_wakeupCnt, m_receivedSlotCnt,
			m_collisionCnt, m_idleListenCnt, m_overhearCnt);
	}
}

UINT16 DataReceptionHandler::NextSlot(UINT32 slotNum){
	UINT16 remainingSlots, randVal;

	//Sanity check: I am executing when I am not supposed to.
	if (!m_shldWakeup) {
		return 0xffff;
	}

	// If we havent woken up yet in the current frame, skip this if-block and
	// simply update the remainingSlot .
	if (m_nextWakeupSlot < slotNum) {

		// first, find the slot denoting the start of the frame immediately after the current one.
		// we have woken up already in the current frame b/c m_nextWakeupSlot < slotNum < nextFrame.
		UINT32 nextFrame = m_nextWakeupSlot + m_dataInterval -	m_nextWakeupSlot % m_dataInterval;
		//update the seed every 8 frames to reduce computation overhead
		if (nextFrame % m_seedUpdateInterval == 0 ) {
			//use the new/next seed for the next 8 frames
			UINT16 lastSeed = m_nextSeed;
			//hal_printf("using seed %u\n", lastSeed);
			//update next seed. we wont use it until 8 frames later
			randVal = g_OMAC.m_omac_scheduler.m_seedGenerator.RandWithMask(&m_nextSeed, m_mask);
			m_nextWakeupSlot = nextFrame + randVal % m_dataInterval;

			//we have computed the wakeup slot for the frame denoted by nextFrame
			//seed info contains the frame that we next need to compute the wakeup slot for
			g_OMAC.m_omac_scheduler.m_DiscoveryHandler.SetSeed(lastSeed, nextFrame + m_dataInterval);
		}

		// If we are less than 8 frame since the last seed update, simply compute
		// the next wakeup by advancing one frame from the last wakeup slot
		else {
			m_nextWakeupSlot += m_dataInterval;
		}
	}

	//then compute the remaining slots before our next wakeup based on the computed schedule
	remainingSlots = m_nextWakeupSlot - slotNum;
	//Should never happen
	if (m_nextWakeupSlot < slotNum) {
		hal_printf("ERROR: nxtSlot %lu curSlot %lu\n", m_nextWakeupSlot, slotNum);
	}

	if (remainingSlots == 0) {
			wakeupSlot = slotNum;
			return 0;
		} else if (remainingSlots >= 0xffff) {
			return 0xffff;
		} else {
			return remainingSlots;
	}

	/* Returns value in ticks left to wake up, we might do this latter,
	 * for the timebeing lets stick to the simpler login of slots
	 if (remainingSlots == 0) {
		wakeupSlot = slotNum;
		return 0;
	} else if (remainingSlots >= (0xffff >> SLOT_PERIOD_BITS)) {
		return 0xffff;
	} else {
		return (remainingSlots << SLOT_PERIOD_BITS);
	}*/
}



void DataReceptionHandler::SetWakeup(bool shldWakeup){
	m_shldWakeup = shldWakeup;
}
