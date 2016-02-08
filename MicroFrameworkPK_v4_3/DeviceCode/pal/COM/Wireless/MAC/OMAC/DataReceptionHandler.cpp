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


void PublicDataRxCallback(void * param){
	if(SOFTWARE_ACKS){
		if(g_OMAC.m_omac_scheduler.m_DataReceptionHandler.m_receptionstate==2){
			g_OMAC.m_omac_scheduler.m_DataReceptionHandler.SendDataACK();
		}
		else{
			g_OMAC.m_omac_scheduler.m_DataReceptionHandler.PostExecuteEvent();
		}
	}
	if(!SOFTWARE_ACKS){
		g_OMAC.m_omac_scheduler.m_DataReceptionHandler.PostExecuteEvent();
	}
}
void PublicDataRxSendAckCallback(void * param){
	g_OMAC.m_omac_scheduler.m_DataReceptionHandler.SendDataACK();
}

/*
 *
 */
void DataReceptionHandler::Initialize(UINT8 radioID, UINT8 macID){
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_EnableOutputPin(DATARECEPTION_SLOTPIN, TRUE);
	CPU_GPIO_EnableOutputPin(DATATX_NEXTEVENT, TRUE);
	CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, FALSE );
	CPU_GPIO_SetPinState( DATATX_NEXTEVENT, FALSE );
	CPU_GPIO_EnableOutputPin(OMAC_TX_DATAACK_PIN, FALSE);
	CPU_GPIO_EnableOutputPin(DATA_RX_INTERRUPT_PIN, FALSE);
	CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
	CPU_GPIO_EnableOutputPin(DATA_RX_END_OF_RECEPTION, FALSE);
	CPU_GPIO_SetPinState( DATA_RX_END_OF_RECEPTION, FALSE );
#endif
	RadioID = radioID;
	MacID = macID;
	m_nextwakeupSlot = 0;
	m_seedUpdateIntervalinSlots = SEED_UPDATE_INTERVAL_IN_SLOTS;

	m_mask = 137 * 29 * (g_OMAC.GetMyAddress() + 1);
	m_nextSeed = 119 * 119 * (g_OMAC.GetMyAddress() + 1); // The initial seed
	m_nextwakeupSlot = g_OMAC.m_omac_scheduler.m_seedGenerator.RandWithMask(&m_nextSeed, m_mask) % m_seedUpdateIntervalinSlots;
	UpdateSeedandCalculateWakeupSlot(m_nextwakeupSlot, m_nextSeed, m_mask, m_seedUpdateIntervalinSlots, g_OMAC.m_omac_scheduler.GetSlotNumber() );

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_RECEIVER, 0, LISTEN_PERIOD_FOR_RECEPTION_HANDLER , TRUE, FALSE, PublicDataRxCallback, OMACClockSpecifier); //1 sec Timer in micro seconds
	if(SOFTWARE_ACKS){
		rm = VirtTimer_SetTimer(VIRT_TIMER_OMAC_RECEIVER_ACK, 0, 0 , TRUE, FALSE, PublicDataRxSendAckCallback, OMACClockSpecifier); //1 sec Timer in micro seconds
	}

	//ASSERT_SP(rm == TimerSupported);

	m_isreceiving = false;
	m_receptionstate = 0;

}

UINT64 DataReceptionHandler::NextEvent(){
	UINT64 y = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
	UINT64 currentSlotNum = g_OMAC.m_omac_scheduler.GetSlotNumber();
	if ( currentSlotNum >= m_nextwakeupSlot ){ //Check for seed update
		UpdateSeedandCalculateWakeupSlot(m_nextwakeupSlot, m_nextSeed, m_mask, m_seedUpdateIntervalinSlots,  currentSlotNum );
	}
	UINT64 NextEventTimeinTicks = m_nextwakeupSlot * SLOT_PERIOD_TICKS - RADIO_TURN_ON_DELAY_MICRO * TICKS_PER_MICRO;
	/*if(HARDWARE_ACKS){
		NextEventTimeinTicks -= (EXTENDED_MODE_TX_DELAY_MICRO * TICKS_PER_MICRO);
		ASSERT_SP(NextEventTimeinTicks > 0);
	}*/

	while(NextEventTimeinTicks  <= y + OMAC_SCHEDULER_MIN_REACTION_TIME_IN_TICKS){
		currentSlotNum = m_nextwakeupSlot;
		UpdateSeedandCalculateWakeupSlot(m_nextwakeupSlot, m_nextSeed, m_mask, m_seedUpdateIntervalinSlots,  currentSlotNum );
		NextEventTimeinTicks = m_nextwakeupSlot * SLOT_PERIOD_TICKS - RADIO_TURN_ON_DELAY_MICRO * TICKS_PER_MICRO;
	}
	UINT64 TicksTillNextEvent = NextEventTimeinTicks - y;
	//ASSERT_SP(NextEventTimeinTicks > y);

	UINT64 nextEventsMicroSec = (g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(TicksTillNextEvent)) ;
	UINT64 curTicks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();

#ifdef def_Neighbor2beFollowed
#ifdef OMAC_DEBUG_PRINTF
	//hal_printf("DataReceptionHandler::NextEvent curTicks: %llu; NextEventTimeinTicks: %llu; m_nextwakeupSlot: %lu; TicksTillNextEvent: %llu; nextEventsMicroSec: %llu\n", curTicks, NextEventTimeinTicks, m_nextwakeupSlot, TicksTillNextEvent, nextEventsMicroSec);
	hal_printf("\n[LT: %llu - %lu NT: %llu - %lu] DataReceptionHandler::NextEvent() nextWakeupTimeInMicSec = %llu AbsnextWakeupTimeInMicSec= %llu - %lu \n", g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(curTicks), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(curTicks), g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), g_OMAC.m_omac_scheduler.GetSlotNumberfromTicks(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_OMAC.Neighbor2beFollowed, curTicks)), nextEventsMicroSec, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(curTicks)+nextEventsMicroSec, (g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ConvertTickstoMicroSecs(curTicks)+nextEventsMicroSec)/SLOT_PERIOD_MILLI/MICSECINMILISEC );
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
			randVal = g_OMAC.m_omac_scheduler.m_seedGenerator.RandWithMask(&next_seed, mask);
			curFrameStart = curFrameStart + seedUpdateIntervalinSlots;
			wakeupSlot = curFrameStart + randVal % seedUpdateIntervalinSlots;
		}
	}
	lastwakeupSlotUpdateTimeinTicks = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
	//ASSERT_SP(wakeupSlot  > currentSlotNum);
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
	e = g_OMAC.m_omac_RadioControl.StartRx();
	if (e == DS_Success){
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, LISTEN_PERIOD_FOR_RECEPTION_HANDLER, TRUE, OMACClockSpecifier );
		m_lastScheduledOriginTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
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
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, 100, TRUE ,OMACClockSpecifier);
		m_lastScheduledOriginTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
		m_lastScheduledTargetTime = m_lastScheduledOriginTime + 0;
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
		if(rm != TimerSupported){ //Could not start the timer to turn the radio off. Turn-off immediately
			PostExecuteEvent();
		}
		//PostExecuteEvent();
	}
}

void DataReceptionHandler::HandleRadioInterrupt(){ // This is the beginning of a reception
	VirtualTimerReturnMessage rm;
	m_isreceiving = true;
	//ASSERT_SP(m_receptionstate == 0);
	m_receptionstate = 1;
	rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
	CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
	if(rm != TimerSupported){
		/*CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );*/
	}
	rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, PACKET_PERIOD_FOR_RECEPTION_HANDLER, TRUE, OMACClockSpecifier );
	m_lastScheduledOriginTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
	m_lastScheduledTargetTime = m_lastScheduledOriginTime + 8 * PACKET_PERIOD_FOR_RECEPTION_HANDLER;
	if(rm != TimerSupported){
		/*CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );*/
	}
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
	if(rm != TimerSupported){
		/*CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, TRUE );*/
	}
}

void DataReceptionHandler::SendACKHandler(){ // Handler for end of tranmission interupt from the radio
	if(SOFTWARE_ACKS) { //Only happens in SOFTWARE_ACKS case
		VirtualTimerReturnMessage rm;
		m_isreceiving = false;
		ASSERT_SP(m_receptionstate == 3);
		m_receptionstate = 4;
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, 0, TRUE, OMACClockSpecifier );
		m_lastScheduledOriginTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();//HAL_Time_CurrentTicks();
		m_lastScheduledTargetTime = m_lastScheduledOriginTime + 0;
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
	}
}

void DataReceptionHandler::HandleEndofReception(UINT16 address){
	if(SOFTWARE_ACKS){
		VirtualTimerReturnMessage rm;
		m_isreceiving = false;
		//ASSERT_SP(m_receptionstate == 1);
		m_receptionstate = 2;
		m_lastRXNodeId = address;
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER_ACK);
		if(rm != TimerSupported){
			this->SendDataACK();
		}
	}
	else if(HARDWARE_ACKS) {
		VirtualTimerReturnMessage rm;
		m_isreceiving = false;
		//ASSERT_SP(m_receptionstate == 1);
		m_receptionstate = 2;
		m_lastRXNodeId = address;
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, ACK_TX_MAX_DURATION_MICRO, TRUE, OMACClockSpecifier );
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
		if(rm != TimerSupported){
			this->SendDataACK();
		}
	}
	else if(!SOFTWARE_ACKS && !HARDWARE_ACKS) {
		CPU_GPIO_SetPinState( DATA_RX_INTERRUPT_PIN, FALSE );
		CPU_GPIO_SetPinState( DATA_RX_END_OF_RECEPTION, TRUE );
		VirtualTimerReturnMessage rm;
		m_isreceiving = false;
		//ASSERT_SP(m_receptionstate == 1);
		m_receptionstate = 2;
		rm = VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
		rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, 0, TRUE, OMACClockSpecifier );
		m_lastScheduledOriginTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
		m_lastScheduledTargetTime = m_lastScheduledOriginTime + 0;
		rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);
		CPU_GPIO_SetPinState( DATA_RX_END_OF_RECEPTION, FALSE );
	}
}



void DataReceptionHandler::SendDataACK(){ // This prepares a software ACK packet and sends it to the radiocontroller
	VirtualTimerReturnMessage rm;
	//ASSERT_SP(m_receptionstate == 2);
	m_receptionstate = 3;
	m_isreceiving = false;

#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_TX_DATAACK_PIN, TRUE);
#endif

	rm = VirtTimer_Change(VIRT_TIMER_OMAC_RECEIVER, 0, ACK_TX_MAX_DURATION_MICRO, TRUE,OMACClockSpecifier );
	m_lastScheduledOriginTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();
	m_lastScheduledTargetTime = m_lastScheduledOriginTime + ACK_TX_MAX_DURATION_MICRO * 8;
	rm = VirtTimer_Start(VIRT_TIMER_OMAC_RECEIVER);

	IEEE802_15_4_Header_t* header = m_ACKmsg.GetHeader();
	/****** Taking the word value of below bits gives 26150 *******/
	/*header->fcf->IEEE802_15_4_Header_FCF_BitValue.frameType = FRAME_TYPE_MAC;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.securityEnabled = 0;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.framePending = 0;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.ackRequired = 1;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.intraPAN = 1;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.reserved = 0;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.destAddrMode = 2;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.frameVersion = 1;
	header->fcf->IEEE802_15_4_Header_FCF_BitValue.srcAddrMode = 2;*/
	/**************************************************************/
	header->fcf.fcfWordValue = 26150;
	header->dsn = 97;
	header->srcpan = 0x0001;
	header->destpan = 0x0001;
	header->dest = m_lastRXNodeId;
	header->src = g_OMAC.GetMyAddress();
	//header->network = MyConfig.Network;
	header->mac_id = g_OMAC.macName;
	header->type = MFM_DATA_ACK;
	header->flags = (0);
	//header->retryAttempt = 0;
	m_ACKmsg.GetMetaData()->SetReceiveTimeStamp((UINT32)0);

	UINT8* payload = m_ACKmsg.GetPayload();
	*payload = 66;

	header->length = sizeof(IEEE802_15_4_Header_t) + 1;
	g_OMAC.m_omac_RadioControl.Send(m_lastRXNodeId, &m_ACKmsg, header->length);
#ifdef OMAC_DEBUG_GPIO
		CPU_GPIO_SetPinState(OMAC_TX_DATAACK_PIN, FALSE);
#endif
}

void DataReceptionHandler::PostExecuteEvent(){
	m_currtime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.GetCurrentTimeinTicks();

	/*if(!(m_receptionstate == 0 || m_receptionstate == 4)){
		m_isreceiving = false;
	}*/

	//Scheduler's PostExecution stops the radio
	g_OMAC.m_omac_RadioControl.Stop();
#ifdef OMAC_DEBUG_GPIO
	CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, FALSE );
#endif

	g_OMAC.m_omac_scheduler.PostExecution();
}

void DataReceptionHandler::FailsafeStop(){
	VirtTimer_Stop(VIRT_TIMER_OMAC_RECEIVER);
}
