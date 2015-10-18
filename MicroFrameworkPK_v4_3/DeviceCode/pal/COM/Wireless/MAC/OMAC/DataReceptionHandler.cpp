/*
 * DataReceptionHandler.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan Sridharan
 */

#include <Samraksh/MAC/OMAC/DataReceptionHandler.h>
#include <Samraksh/MAC/OMAC/OMAC.h>

//#define DATARECSLOTPIN 30 //2

extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;
extern Buffer_15_4_t g_send_buffer;
extern Buffer_15_4_t g_receive_buffer;
extern RadioControl_t g_omac_RadioControl;
DataReceptionHandler g_DataReceptionHandler;

static BOOL varCounter;
MacReceiveFuncPtrType g_rxAckHandler;
MacEventHandler_t* g_appHandler;


//extern LCD_PCF85162_Driver g_LCD_PCF85162_Driver;

void PublicReceiveHCallback(void * param){
	g_omac_scheduler.m_DataReceptionHandler.PostExecuteEvent();
}

/*
 *
 */
void DataReceptionHandler::Initialize(UINT8 radioID, UINT8 macID){
	CPU_GPIO_EnableOutputPin(DATARECEPTION_SLOTPIN, TRUE);
	RadioID = radioID;
	MacID = macID;
	m_nextwakeupSlot = 0;
	m_seedUpdateIntervalinSlots = 250;

	m_mask = 137 * 29 * (CPU_Radio_GetAddress(radioID) + 1);
	m_nextSeed = 119 * 119 * (CPU_Radio_GetAddress(radioID) + 1); // The initial seed
	m_nextwakeupSlot = g_omac_scheduler.m_seedGenerator.RandWithMask(&m_nextSeed, m_mask) % m_seedUpdateIntervalinSlots;

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(HAL_RECEPTION_TIMER, 0, SLOT_PERIOD_MILLI * 1 * MICSECINMILISEC, TRUE, FALSE, PublicReceiveHCallback); //1 sec Timer in micro seconds

	CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, FALSE );
}

UINT64 DataReceptionHandler::NextEvent(UINT32 currentSlotNum){
	UINT64 y = HAL_Time_CurrentTicks();
	//UINT32 curslot = g_omac_scheduler.GetSlotNumber(y);
	if ( currentSlotNum >= m_nextwakeupSlot ){ //Check for seed update
		UpdateSeedandCalculateWakeupSlot(m_nextwakeupSlot, m_nextSeed, m_mask, m_seedUpdateIntervalinSlots,  currentSlotNum );
	}
	UINT64 NextEventTime = m_nextwakeupSlot * SLOT_PERIOD_TICKS;
	//UINT64 NextEventTime = (( y/ (UINT64) WAKEUPPERIODINTICKS)  + 1) * ((UINT64)WAKEUPPERIODINTICKS);
	UINT64 TicksTillNextEvent = NextEventTime - y;
	UINT64 nextEventsMicroSec = (HAL_Time_TicksToTime(TicksTillNextEvent)) ;
	UINT64 curTime = HAL_Time_CurrentTicks() / (TICKS_PER_MILLI/MICSECINMILISEC);
	hal_printf("\n[LT: %llu NT: %llu] DataReceptionHandler::NextEvent() nextWakeupTimeInMicSec= %llu AbsnextWakeupTimeInMicSec= %llu \n",curTime, g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, curTime), nextEventsMicroSec,curTime+nextEventsMicroSec );
	return(nextEventsMicroSec);
}

void DataReceptionHandler::UpdateSeedandCalculateWakeupSlot(UINT32 &wakeupSlot, UINT16 &next_seed, const UINT16 &mask, const UINT32 &seedUpdateIntervalinSlots,  const UINT32 &currentSlotNum ){
	hal_printf("DataReceptionHandler::ExecuteEvent. I am %u\n", g_OMAC.GetAddress());
	if (currentSlotNum >= wakeupSlot){
		UINT16 randVal;
		UINT32 curFrameStart = wakeupSlot - wakeupSlot % seedUpdateIntervalinSlots;
		while ( currentSlotNum >= wakeupSlot ){
			randVal = g_omac_scheduler.m_seedGenerator.RandWithMask(&next_seed, mask);
			curFrameStart = curFrameStart + seedUpdateIntervalinSlots;
			wakeupSlot = curFrameStart + randVal % seedUpdateIntervalinSlots;
		}
	}
	assert(wakeupSlot  > currentSlotNum);
}
/*
 *
 */
void DataReceptionHandler::ExecuteEvent(UINT32 slotNum){
	DeviceStatus e = DS_Fail;
	//hal_printf("\n[LT: %llu NT: %llu] DataReceptionHandler:ExecuteEvent\n",HAL_Time_CurrentTime(), g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTime()));
	e = g_omac_RadioControl.StartRx();
	if (e == DS_Success){
		CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, TRUE );
		//call ChannelMonitor.monitorChannel();
		//SendDataBeacon(FALSE);
		hal_printf("DataReceptionHandler::ExecuteEvent. I am %u\n", g_OMAC.GetAddress());
		//hal_printf("DataReceptionHandler::ExecuteEvent CurTicks: %llu currentSlotNum: %d m_nextWakeupSlot: %d \n",HAL_Time_CurrentTicks(), slotNum, m_nextWakeupSlot);
	}
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_Start(HAL_RECEPTION_TIMER);
}

/*
 *
 */
void DataReceptionHandler::PostExecuteEvent(){
	/* Two types of symptons if collision occurs:
	 * 1. Cannot receive packet, but there is energy in the channel
	 * 2. Received packet but with incorrect CRC
	 */
#ifdef OMAC_DEBUG
	CPU_GPIO_SetPinState((GPIO_PIN) 23, FALSE);
	//hal_printf("[Lcl %lu] Radio stopped\n", call GlobalTime.getLocalTime());
#endif
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
	CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, FALSE );
	g_omac_scheduler.PostExecution();
}


