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



UINT32 DataReceptionHandler::GetWakeupSlot()
{
	return m_nextWakeupSlot;
}

void DataReceptionHandler::SetWakeupSlot(UINT32 tmp_nextWakeupSlot)
{
	m_nextWakeupSlot = tmp_nextWakeupSlot;
}


/*
 *
 */
void DataReceptionHandler::Initialize(UINT8 radioID, UINT8 macID){
	CPU_GPIO_EnableOutputPin(DATARECEPTION_SLOTPIN, TRUE);
	varCounter = FALSE;
	RadioID = radioID;
	MacID = macID;

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
	//m_nextWakeupSlot = 0;

	wakeupSlot = wakeupTime = scheduledWakeupTime = 0;

	dataBeaconBufferPtr = &dataBeaconBuffer;
	//m_dataInterval determines the duty-cycle. WAKEUP_INTERVAL determines how long radio is awake and
	//m_dataInterval is the number of slots within that period.
	m_dataInterval = WAKEUP_INTERVAL / SLOT_PERIOD_MILLI;
	if (m_dataInterval < 1) {
		m_dataInterval = 10;
	}

	lastSeed = m_nextSeed = 119 * 119 * (CPU_Radio_GetAddress(radioID) + 1);
	m_mask = 137 * 29 * (CPU_Radio_GetAddress(radioID) + 1);
	//m_nextSeed is updated with its passed pointer. It will be the next seed to use
	//after 8 frames pass
	UINT64 tmp_nextWakeupSlot = g_omac_scheduler.m_seedGenerator.RandWithMask(&m_nextSeed, m_mask) % m_dataInterval;
	SetWakeupSlot(tmp_nextWakeupSlot);
	//seed is updated every 8 frames. return the seed to its initial value
	g_omac_scheduler.m_DiscoveryHandler.SetSeed(lastSeed, m_dataInterval);
	m_seedUpdateInterval = (m_dataInterval << 3);

	m_receivedDataPacket = FALSE;
	m_efdDetected = FALSE;
	m_dwellSource = INVALID_ADDRESS;

	if (m_dataInterval <= 1) {
		hal_printf("ERROR: data interval is less or equal to 1\n");
	}

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(HAL_RECEPTION_TIMER, 0, SLOT_PERIOD_MILLI * 1 * MICSECINMILISEC, TRUE, FALSE, PublicReceiveHCallback); //1 sec Timer in micro seconds

	/*if(!varCounter){
		g_rxAckHandler = g_OMAC.GetAppHandler(g_OMAC.GetAppIdIndex())->GetReceiveHandler();
		g_appHandler = g_OMAC.GetAppHandler(g_OMAC.GetAppIdIndex());
		varCounter = TRUE;
	}*/
	CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, FALSE );
}

UINT64 DataReceptionHandler::NextEvent(UINT32 currentSlotNum){
	UINT16 nextEventsSlot = 0;
	UINT64 nextEventsMicroSec = 0;
	nextEventsSlot = NextEventinSlots(currentSlotNum);
	if(nextEventsSlot == 0) return(nextEventsMicroSec-1);//BK: Current slot is already too late. Hence return a large number back
	nextEventsMicroSec = nextEventsSlot * SLOT_PERIOD_MILLI * MICSECINMILISEC;
	nextEventsMicroSec = nextEventsMicroSec + g_omac_scheduler.GetTimeTillTheEndofSlot();

	hal_printf("\n[LT: %llu NT: %llu] DataReceptionHandler:NextEvent nextEventsMicroSec=%llu nexxEventTime = %llu \n",HAL_Time_CurrentTime(), g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTime()), nextEventsMicroSec, nextEventsMicroSec +HAL_Time_CurrentTime() );
	return(nextEventsMicroSec);
}
/*
 * Takes current slot number as input and returns slot number in future when event is supposed to happen
 * Input value:		32 bit current slot number
 * Return value:	16 bit slot value of event in future
 */
UINT16 DataReceptionHandler::NextEventinSlots(UINT32 currentSlotNum){
	UINT16 remainingSlots, randVal;
	//hal_printf("DataReceptionHandler::NextEvent currentSlotNum: %u\n", currentSlotNum);

	//Sanity check: I am executing when I am not supposed to.
	if (!m_shldWakeup) {
		return 0xffff;
	}

	UINT32 tmp_nextWakeupSlot = GetWakeupSlot();
	// If we haven't woken up yet in the current frame, skip this if-block and
	// simply update the remainingSlot .
	if (tmp_nextWakeupSlot < currentSlotNum) {
		while (tmp_nextWakeupSlot < currentSlotNum) {
			////hal_printf("DataReceptionHandler::NextEvent - step 1\n");
			// first, find the slot denoting the start of the frame immediately after the current one.
			// we have woken up already in the current frame b/c m_nextWakeupSlot < slotNum < nextFrame.
			UINT32 nextFrame = tmp_nextWakeupSlot + m_dataInterval - (tmp_nextWakeupSlot % m_dataInterval);
			//update the seed every 8 frames to reduce computation overhead
			if (nextFrame % m_seedUpdateInterval == 0 ) {
				//use the new/next seed for the next 8 frames
				UINT16 lastSeed = m_nextSeed;
				//hal_printf("using seed %u\n", lastSeed);
				//update next seed. we wont use it until 8 frames later
				randVal = g_omac_scheduler.m_seedGenerator.RandWithMask(&m_nextSeed, m_mask);
				////hal_printf("DataReceptionHandler::NextEvent -- m_nextSeed is %u\n", m_nextSeed);
				////hal_printf("DataReceptionHandler::NextEvent -- randVal is %u\n", randVal);
				tmp_nextWakeupSlot = nextFrame + randVal % m_dataInterval;
				SetWakeupSlot(tmp_nextWakeupSlot);

				//we have computed the wakeup slot for the frame denoted by nextFrame
				//seed info contains the frame that we next need to compute the wakeup slot for
				g_omac_scheduler.m_DiscoveryHandler.SetSeed(lastSeed, nextFrame + m_dataInterval);
			}

			// If we are less than 8 frame since the last seed update, simply compute
			// the next wakeup by advancing one frame from the last wakeup slot
			else {
				tmp_nextWakeupSlot += m_dataInterval;
				SetWakeupSlot(tmp_nextWakeupSlot);
			}
			//hal_printf("CurTicks: %llu currentSlotNum: %d m_nextWakeupSlot: %d \n",HAL_Time_CurrentTicks(), currentSlotNum, m_nextWakeupSlot);
			//hal_printf("DataReceptionHandler::NextEvent m_nextWakeupSlot: %d\n", tmp_nextWakeupSlot);
		}
	}

	//then compute the remaining slots before our next wakeup based on the computed schedule
	remainingSlots = tmp_nextWakeupSlot - currentSlotNum;


	if (remainingSlots == 0) {
		////hal_printf("DataReceptionHandler::NextEvent - remainingSlots: %u\n", remainingSlots);
		wakeupSlot = currentSlotNum;
		return remainingSlots;
	}
	else if (remainingSlots >= 0xffff) {
		hal_printf("DataReceptionHandler::NextEvent - remainingSlots: %u\n", 0xffff);
		return 0xffff;
	}
	else {
		////hal_printf("DataReceptionHandler::NextEvent - remainingSlots: %u\n", remainingSlots);
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

/*
 *
 */
void DataReceptionHandler::ExecuteEvent(UINT32 slotNum){
	DeviceStatus e = DS_Fail;
	hal_printf("\n[LT: %llu NT: %llu] DataReceptionHandler:ExecuteEvent\n",HAL_Time_CurrentTime(), g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed, HAL_Time_CurrentTime()));
	e = g_omac_RadioControl.StartRx();
	if (e == DS_Success){
		CPU_GPIO_SetPinState( DATARECEPTION_SLOTPIN, TRUE );
		//call ChannelMonitor.monitorChannel();
		//SendDataBeacon(FALSE);
		hal_printf("DataReceptionHandler::ExecuteEvent. I am %u\n", g_OMAC.GetAddress());
		//hal_printf("DataReceptionHandler::ExecuteEvent CurTicks: %llu currentSlotNum: %d m_nextWakeupSlot: %d \n",HAL_Time_CurrentTicks(), slotNum, m_nextWakeupSlot);

		m_wakeupCnt++;
		//BK: Schedule post execute event

		/*Message_15_4_t txMsg;
		Message_15_4_t* txMsgPtr = &txMsg;
		Message_15_4_t** tempPtr = g_send_buffer.GetOldestPtr();
		Message_15_4_t* msgPtr = *tempPtr;
		memset(txMsgPtr, 0, msgPtr->GetMessageSize());
		memcpy(txMsgPtr, msgPtr, msgPtr->GetMessageSize());
		UINT8* snd_payload = txMsgPtr->GetPayload();*/

		//volatile UINT16 rx_length;
		/*MacReceiveFuncPtrType rxAckHandler = g_OMAC.GetAppHandler(g_OMAC.GetAppIdIndex())->GetReceiveHandler();
		MacEventHandler_t* appHandler = g_OMAC.GetAppHandler(g_OMAC.GetAppIdIndex());*/
		//(*g_rxAckHandler)(rx_length);
		//MacEventHandler_t* appHandler1 = MAC<Message_15_4_t, MacConfig>::GetAppHandler( MAC<Message_15_4_t, MacConfig>::GetAppIdIndex() );


		/*for(int i = 0; i < txMsgPtr->GetMessageSize(); i++){
			hal_printf("snd_payload[i]: %u ", snd_payload[i]);
		}
		hal_printf("\n");*/

	}
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_Start(HAL_RECEPTION_TIMER);
}

/*
 *
 */
/*UINT8 DataReceptionHandler::ExecuteEventDone(){
	return 0;
}*/

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

/*
 *
 */
void DataReceptionHandler::SetWakeup(bool shldWakeup){
	m_shldWakeup = shldWakeup;
}

/*
 *
 */
bool DataReceptionHandler::SendDataBeacon(bool sendPiggyBacked){
	hal_printf("start DataReceptionHandler::SendDataBeacon\n");
	g_omac_scheduler.ProtoState.ForceState(S_WAITING_DATA);
	m_receivedDataPacket = FALSE;
	m_efdDetected = FALSE;
	DataBeacon_t * sndMsg = (DataBeacon_t *) dataBeaconBufferPtr->GetPayload();
	sndMsg->nodeID = CPU_Radio_GetAddress(RadioID);
	dataBeaconBufferPtr->GetHeader()->type = OMAC_DATA_BEACON_TYPE;

	g_omac_RadioControl.Send(RADIO_BROADCAST_ADDRESS, dataBeaconBufferPtr, sizeof(DataBeacon_t) );
	//call DataReceptionTimer.startOneShot(WAIT_TIME_AFTER_DATA_BEACON);
	hal_printf("end DataReceptionHandler::SendDataBeacon\n");
	return true;
}

