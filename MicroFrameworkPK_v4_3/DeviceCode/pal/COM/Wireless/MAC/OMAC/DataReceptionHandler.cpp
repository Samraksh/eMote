/*
 * DataReceptionHandler.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan Sridharan
 *
 *  Modified on: Oct 30, 2015
 *  	Authors: Bora Karaoglu; Ananth Muralidharan
 *
 *  Copyright The Samraksh Company
 */

#include <Samraksh/MAC/OMAC/DataReceptionHandler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>


extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;
extern RadioControl_t g_omac_RadioControl;
DataReceptionHandler g_DataReceptionHandler;

MacReceiveFuncPtrType g_rxAckHandler;
MacEventHandler_t* g_appHandler;


void PublicReceiveHCallback(void * param){
	g_omac_scheduler.m_DataReceptionHandler.PostExecuteEvent();
}

/*
 *
 */
void DataReceptionHandler::Initialize(UINT8 radioID, UINT8 macID){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(DATARECEPTION_SLOTPIN, TRUE);
	CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, FALSE );
#endif
	RadioID = radioID;
	MacID = macID;
	m_nextwakeupSlot = 0;
	m_seedUpdateIntervalinSlots = SEED_UPDATE_INTERVAL_IN_SLOTS;

	m_mask = 137 * 29 * (CPU_Radio_GetAddress(radioID) + 1);
	m_nextSeed = 119 * 119 * (CPU_Radio_GetAddress(radioID) + 1); // The initial seed
	m_nextwakeupSlot = g_omac_scheduler.m_seedGenerator.RandWithMask(&m_nextSeed, m_mask) % m_seedUpdateIntervalinSlots;
	UpdateSeedandCalculateWakeupSlot(m_nextwakeupSlot, m_nextSeed, m_mask, m_seedUpdateIntervalinSlots, g_omac_scheduler.GetSlotNumber() );

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(HAL_RECEPTION_TIMER, 0, SLOT_PERIOD_MILLI * 1 * MICSECINMILISEC, TRUE, FALSE, PublicReceiveHCallback); //1 sec Timer in micro seconds
}

UINT64 DataReceptionHandler::NextEvent(){
	UINT64 y = HAL_Time_CurrentTicks();
	UINT64 currentSlotNum = g_omac_scheduler.GetSlotNumber();
	if ( currentSlotNum >= m_nextwakeupSlot ){ //Check for seed update
		UpdateSeedandCalculateWakeupSlot(m_nextwakeupSlot, m_nextSeed, m_mask, m_seedUpdateIntervalinSlots,  currentSlotNum );
	}
	UINT64 NextEventTimeinTicks = m_nextwakeupSlot * SLOT_PERIOD_TICKS;
	//UINT64 NextEventTime = (( y/ (UINT64) WAKEUPPERIODINTICKS)  + 1) * ((UINT64)WAKEUPPERIODINTICKS);
	UINT64 TicksTillNextEvent = NextEventTimeinTicks - y;
	ASSERT(NextEventTimeinTicks > y);

	UINT64 nextEventsMicroSec = (HAL_Time_TicksToTime(TicksTillNextEvent)) ;
	UINT64 curTicks = HAL_Time_CurrentTicks();
#ifdef def_Neighbor2beFollowed
	//hal_printf("DataReceptionHandler::NextEvent curTicks: %llu; NextEventTimeinTicks: %llu; m_nextwakeupSlot: %lu; TicksTillNextEvent: %llu; nextEventsMicroSec: %llu\n", curTicks, NextEventTimeinTicks, m_nextwakeupSlot, TicksTillNextEvent, nextEventsMicroSec);
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataReceptionHandler::NextEvent() nextWakeupTimeInMicSec = %llu AbsnextWakeupTimeInMicSec= %llu - %lu \n", HAL_Time_TicksToTime(curTicks), g_omac_scheduler.GetSlotNumberfromTicks(curTicks), HAL_Time_TicksToTime(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), g_omac_scheduler.GetSlotNumberfromTicks(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), nextEventsMicroSec, HAL_Time_TicksToTime(curTicks)+nextEventsMicroSec, (HAL_Time_TicksToTime(curTicks)+nextEventsMicroSec)/SLOT_PERIOD_MILLI/MICSECINMILISEC );
#endif
	return(nextEventsMicroSec);
}

void DataReceptionHandler::UpdateSeedandCalculateWakeupSlot(UINT64 &wakeupSlot, UINT16 &next_seed, const UINT16 &mask, const UINT32 &seedUpdateIntervalinSlots,  const UINT64 &currentSlotNum ){
#ifdef def_Neighbor2beFollowed
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataReceptionHandler:UpdateSeedandCalculateWakeupSlot\n"
			, HAL_Time_TicksToTime(HAL_Time_CurrentTicks()), g_omac_scheduler.GetSlotNumber(), HAL_Time_TicksToTime(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, HAL_Time_CurrentTicks())),g_omac_scheduler.GetSlotNumberfromTicks(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, HAL_Time_CurrentTicks())) );
#endif
	if (currentSlotNum >= wakeupSlot){
		UINT16 randVal;
		UINT64 curFrameStart = wakeupSlot - wakeupSlot % seedUpdateIntervalinSlots;
		while ( currentSlotNum >= wakeupSlot ){
			randVal = g_omac_scheduler.m_seedGenerator.RandWithMask(&next_seed, mask);
			curFrameStart = curFrameStart + seedUpdateIntervalinSlots;
			wakeupSlot = curFrameStart + randVal % seedUpdateIntervalinSlots;
		}
	}
	ASSERT(wakeupSlot  > currentSlotNum);
}
/*
 *
 */
void DataReceptionHandler::ExecuteEvent(){
	DeviceStatus e = DS_Fail;
	//hal_printf("\n[LT: %llu NT: %llu] DataReceptionHandler:ExecuteEvent\n",HAL_Time_TicksToTime(HAL_Time_CurrentTicks()), g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTicks()));
	e = g_omac_RadioControl.StartRx();
	if (e == DS_Success){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, TRUE );
#endif
		VirtualTimerReturnMessage rm;
		rm = VirtTimer_Start(HAL_RECEPTION_TIMER);
	}
	else{
		hal_printf("DataReceptionHandler::ExecuteEvent Could not turn on Rx\n");
	}
}

/*
 *
 */
void DataReceptionHandler::PostExecuteEvent(){
	/* Two types of symptons if collision occurs:
	 * 1. Cannot receive packet, but there is energy in the channel
	 * 2. Received packet but with incorrect CRC
	 */
	//if (m_wakeupCnt % m_reportPeriod == 0) {
	//	hal_printf("wakeupCnt=%u,rxCnt=%u,collision=%u,idle=%u,overhear=%u\n",
	//		m_wakeupCnt, m_receivedSlotCnt,
	//		m_collisionCnt, m_idleListenCnt, m_overhearCnt);
	//}
	UpdateSeedandCalculateWakeupSlot(m_nextwakeupSlot, m_nextSeed, m_mask, m_seedUpdateIntervalinSlots,  g_omac_scheduler.GetSlotNumber() );
	if( g_omac_scheduler.InputState.IsState(I_DATA_RCV_PENDING) ) {
		//Stop the radio
		g_omac_RadioControl.Stop();
	}
	else {
		hal_printf("DataReceptionHandler::PostExecuteEvent():: Missed the turnoff opportunity");
		g_omac_RadioControl.Stop();
	}
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, FALSE );
#endif
	g_omac_scheduler.PostExecution();
}


