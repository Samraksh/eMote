/*
 * DataReceptionHandler.cpp
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


void PublicDataRxCallback(void * param){
	g_omac_scheduler.m_DataReceptionHandler.PostExecuteEvent();
}

/*
 *
 */
void DataReceptionHandler::Initialize(UINT8 radioID, UINT8 macID){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(DATARECEPTION_SLOTPIN, TRUE);
	CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, FALSE );
	CPU_GPIO_EnableOutputPin(OMAC_TX_DATAACK_PIN, FALSE);
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
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_RECEIVER, 0, LISTEN_PERIOD_FOR_RECEPTION_HANDLER , TRUE, FALSE, PublicDataRxCallback); //1 sec Timer in micro seconds

	ASSERT_SP(rm == TimerSupported);

	m_isreceiving = false;
	m_receptionstate = 0;

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
	ASSERT_SP(NextEventTimeinTicks > y);

	UINT64 nextEventsMicroSec = (HAL_Time_TicksToTime(TicksTillNextEvent)) ;
	UINT64 curTicks = HAL_Time_CurrentTicks();

#ifdef def_Neighbor2beFollowed
#ifdef OMAC_DEBUG_PRINTF
	//hal_printf("DataReceptionHandler::NextEvent curTicks: %llu; NextEventTimeinTicks: %llu; m_nextwakeupSlot: %lu; TicksTillNextEvent: %llu; nextEventsMicroSec: %llu\n", curTicks, NextEventTimeinTicks, m_nextwakeupSlot, TicksTillNextEvent, nextEventsMicroSec);
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataReceptionHandler::NextEvent() nextWakeupTimeInMicSec = %llu AbsnextWakeupTimeInMicSec= %llu - %lu \n", HAL_Time_TicksToTime(curTicks), g_omac_scheduler.GetSlotNumberfromTicks(curTicks), HAL_Time_TicksToTime(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), g_omac_scheduler.GetSlotNumberfromTicks(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), nextEventsMicroSec, HAL_Time_TicksToTime(curTicks)+nextEventsMicroSec, (HAL_Time_TicksToTime(curTicks)+nextEventsMicroSec)/SLOT_PERIOD_MILLI/MICSECINMILISEC );
#endif
#endif
	return(nextEventsMicroSec);
}

void DataReceptionHandler::UpdateSeedandCalculateWakeupSlot(UINT64 &wakeupSlot, UINT16 &next_seed, const UINT16 &mask, const UINT32 &seedUpdateIntervalinSlots,  const UINT64 &currentSlotNum ){
	if (currentSlotNum >= wakeupSlot){
		UINT16 randVal;
		UINT64 curFrameStart = wakeupSlot - wakeupSlot % seedUpdateIntervalinSlots;
		while ( currentSlotNum >= wakeupSlot ){
			//TODO: BK: The following does not seem to work. For now we are bypassing this by having a constant.
			randVal = g_omac_scheduler.m_seedGenerator.RandWithMask(&next_seed, mask);
			curFrameStart = curFrameStart + seedUpdateIntervalinSlots;
			wakeupSlot = curFrameStart + randVal % seedUpdateIntervalinSlots;
		}
	}
	lastwakeupSlotUpdateTimeinTicks = HAL_Time_CurrentTicks();
	ASSERT_SP(wakeupSlot  > currentSlotNum);
}
/*
 *
 */
void DataReceptionHandler::ExecuteEvent(){
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, TRUE );
#endif

	VirtualTimerReturnMessage rm;
	m_isreceiving = false;
	m_receptionstate = 0;
	//static int failureCount = 0;
	DeviceStatus e = DS_Fail;
	//hal_printf("\n[LT: %llu NT: %llu] DataReceptionHandler:ExecuteEvent\n",HAL_Time_TicksToTime(HAL_Time_CurrentTicks()), g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTicks()));
	e = g_omac_RadioControl.StartRx();
	if (e == DS_Success){
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, LISTEN_PERIOD_FOR_RECEPTION_HANDLER, TRUE );
		m_lastScheduledOriginTime = HAL_Time_CurrentTicks();
		m_lastScheduledTargetTime = m_lastScheduledOriginTime + 8 * LISTEN_PERIOD_FOR_RECEPTION_HANDLER;
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
	}
	else{
		hal_printf("DataReceptionHandler::ExecuteEvent Could not turn on Rx\n");
		/*failureCount++;
		if(failureCount > 5){
			//ASSERT_SP(0);
		}*/
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, 0, TRUE );
		m_lastScheduledOriginTime = HAL_Time_CurrentTicks();
		m_lastScheduledTargetTime = m_lastScheduledOriginTime + 0;
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
		//PostExecuteEvent();
	}
}

void DataReceptionHandler::HandleRadioInterrupt(){
	VirtualTimerReturnMessage rm;
	m_isreceiving = true;
	ASSERT_SP(m_receptionstate == 0);
	m_receptionstate = 1;
	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
	CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
	CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
	CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
	CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
	if(rm != TimerSupported){
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
	}
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, PACKET_PERIOD_FOR_RECEPTION_HANDLER, TRUE );
	m_lastScheduledOriginTime = HAL_Time_CurrentTicks();
	m_lastScheduledTargetTime = m_lastScheduledOriginTime + 8 * PACKET_PERIOD_FOR_RECEPTION_HANDLER;
	if(rm != TimerSupported){
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );

	}
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
	if(rm != TimerSupported){
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
	}

}

void DataReceptionHandler::SendACKHandler(){
	VirtualTimerReturnMessage rm;
	m_isreceiving = false;
	ASSERT_SP(m_receptionstate == 3);
	m_receptionstate = 4;
	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, 0, TRUE );
	m_lastScheduledOriginTime = HAL_Time_CurrentTicks();
	m_lastScheduledTargetTime = m_lastScheduledOriginTime + 0;
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
}

void DataReceptionHandler::HandleEndofReception(UINT16 address){
#ifdef SOFTWARE_ACKS_ENABLED
	VirtualTimerReturnMessage rm;
	m_isreceiving = false;
	ASSERT_SP(m_receptionstate == 1);
	m_receptionstate = 2;
	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, ACK_TX_MAX_DURATION_MICRO, TRUE );
	m_lastScheduledOriginTime = HAL_Time_CurrentTicks();
	m_lastScheduledTargetTime = m_lastScheduledOriginTime + ACK_TX_MAX_DURATION_MICRO * 8;
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
	if(rm == TimerSupported){
		this->SendDataACK(address);
	}
#endif
#ifndef SOFTWARE_ACKS_ENABLED
	VirtualTimerReturnMessage rm;
	m_isreceiving = false;
	ASSERT_SP(m_receptionstate == 1);
	m_receptionstate = 2;
	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, 0, TRUE );
	m_lastScheduledOriginTime = HAL_Time_CurrentTicks();
	m_lastScheduledTargetTime = m_lastScheduledOriginTime + 0;
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
#endif
}

void DataReceptionHandler::SendDataACK(UINT16 address){
	ASSERT_SP(m_receptionstate == 2);
	m_receptionstate = 3;
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_TX_DATAACK_PIN, TRUE);
#endif
	IEEE802_15_4_Header_t* header = m_ACKmsg.GetHeader();
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest = address;
	header->src = CPU_Radio_GetAddress(g_OMAC.radioName);
	//header->network = MyConfig.Network;
	header->mac_id = g_OMAC.macName;
	header->type = MFM_DATA_ACK;
	header->SetFlags(0);
	m_ACKmsg.GetMetaData()->SetReceiveTimeStamp(0);

	UINT8* payload = m_ACKmsg.GetPayload();
	*payload = 66;

	header->length = sizeof(IEEE802_15_4_Header_t) + 1;
	g_omac_RadioControl.Send(address, &m_ACKmsg, header->length);
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_TX_DATAACK_PIN, FALSE);
#endif
}

void DataReceptionHandler::PostExecuteEvent(){
	m_currtime = HAL_Time_CurrentTicks();

	if(!(m_receptionstate == 0 || m_receptionstate == 4)){
		m_isreceiving = false;
	}

	g_omac_RadioControl.Stop();
	#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, FALSE );
	#endif
	g_omac_scheduler.PostExecution();
}


