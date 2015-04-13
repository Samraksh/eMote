/*
 * TimesyncTest.cpp
 */
#include <tinyhal.h>
//#include <Samraksh/HALTimer.h>
#include <Samraksh/VirtualTimer.h>
#include "SimpleTimesyncTest.h"



#define DEBUG_TIMESYNCTEST 1
#define DEBUG_TSYNC 1

//#define ACTIVITYPIN 24
#define TIMESYNCSENDPIN 2
#define TIMESYNCRECEIVEPIN 23
#define NBRCLOCKMONITORPIN 31
#define NBCLOCKMONITORPERIOD 100000
#define LOCALCLOCKMONITORPIN 29
#define LocalClockMonitor_TIMER 32
#define NbrClockMonitor_TIMER 33
#define SimpleTimesyncTest_Send_TIMER 34
#define NbrClockMonitorStart_TIMER 35


//extern HALTimerManager gHalTimerManagerObject;
//extern VirtualTimer gVirtualTimerObject;
//extern VirtualTimerMapper gVirtualTimerMapperObject;
extern SimpleTimesyncTest gSimpleTimesyncTest;
extern UINT16 MF_NODE_ID;
//extern Buffer_15_4_t m_receive_buffer;

void SimpleTimesyncTest_Send_Timer_Handler(void * arg){
	gSimpleTimesyncTest.Send();
}

void SimpleTimesyncTest_ReceiveHandler (UINT16 NumOfPendingPackets){
	//Message_15_4_t** temp = m_receive_buffer.GetOldestPtr();

	UINT8* ptr_msg = &(gSimpleTimesyncTest.msg_in_buffer[0]);
	UINT8** ptr_ptr_msg = &(ptr_msg);

	Mac_GetNextPacket(ptr_ptr_msg);

	SimpleTimeSyncMsg* temp = (SimpleTimeSyncMsg*) (&(gSimpleTimesyncTest.msg_in_buffer[16]));

	UINT16 Size = ((*ptr_ptr_msg)[1]<<8) + ((*ptr_ptr_msg)[0]);

	//Message_15_4_t * msg_15_4_ptr = &(gSimpleTimesyncTest.msg_15_4);
	//IEEE802_15_4_Header_t * header = msg_15_4_ptr->GetHeader();
	//SimpleTimeSyncMsg* m_timeSyncMsg = (SimpleTimeSyncMsg *) msg_15_4_ptr->GetPayload();
	//memcpy(m_timeSyncMsg, &((*ptr_ptr_msg)[2]), Size);

	UINT16 src = ((*ptr_ptr_msg)[5 + Size]<<8) + ((*ptr_ptr_msg)[4 + Size]);
	if (gSimpleTimesyncTest.Nbr2beFollowed==0){ gSimpleTimesyncTest.Nbr2beFollowed = src; }
	if (src == gSimpleTimesyncTest.Nbr2beFollowed ){

	UINT64 EventTime;
	UINT8 a,b,c,d,e,f,g,h;
	a	= ((*ptr_ptr_msg)[8 + Size] );
	b  =	 ((*ptr_ptr_msg)[9 + Size]);
	c  = 	((*ptr_ptr_msg)[10 + Size] );
	d		  = ((*ptr_ptr_msg)[11 + Size]);
	e		  = ((*ptr_ptr_msg)[12 + Size] );
	f		  = ((*ptr_ptr_msg)[13 + Size] );
	g		  = ((*ptr_ptr_msg)[14 + Size] );
	h		  = ((*ptr_ptr_msg)[15 + Size] );
	EventTime = a;
	EventTime = EventTime	+ (((UINT64)b) << 8);
	EventTime = EventTime 	+ (((UINT64)c) << 16);
	EventTime = EventTime	+ (((UINT64)d) << 24);
	EventTime = EventTime	+ (((UINT64)e) << 32);
	EventTime = EventTime	+ (((UINT64)f) << 40);
	EventTime = EventTime	+ (((UINT64)g) << 48);
	EventTime = EventTime	+ (((UINT64)h) << 56);


	//return gSimpleTimesyncTest.Receive(msg_ptr,Size);

	gSimpleTimesyncTest.ReceiveSyncMessage(src,EventTime, temp);
	}
}

void SimpleTimesyncTest_SendAckHandler (void* msg, UINT16 size, NetOpStatus status){
	gSimpleTimesyncTest.SendAck(msg,size,status);
}

void SimpleLocalClockMonitorTimerHandler(void * arg) {
	//Toggle Pin State for monitoring with Logic Analyzer

	if(gSimpleTimesyncTest.LocalClkMonitorPinState) {
		gSimpleTimesyncTest.LocalClkMonitorPinState = false;
		CPU_GPIO_SetPinState((GPIO_PIN) LOCALCLOCKMONITORPIN, false);
	}
	else {
		gSimpleTimesyncTest.LocalClkMonitorPinState = true;
		CPU_GPIO_SetPinState((GPIO_PIN) LOCALCLOCKMONITORPIN, true);
	}
	//g_OMAC.m_omac_scheduler.m_TimeSyncHandler.ISLocalCLKScheduled = FALSE;
	//LocalClockMonitorTimerScheduler();

}

void SimpleNeighborClockMonitorTimerHandler(void * arg) {
	//Toggle Pin State for monitoring with Logic Analyzer
	if(gSimpleTimesyncTest.NeighClkMonitorPinState) {
		gSimpleTimesyncTest.NeighClkMonitorPinState = false;
		CPU_GPIO_SetPinState((GPIO_PIN) NBRCLOCKMONITORPIN, false);
	}
	else {
		gSimpleTimesyncTest.NeighClkMonitorPinState = true;
		CPU_GPIO_SetPinState((GPIO_PIN) NBRCLOCKMONITORPIN, true);
	}
}


// TIMESYNCTEST

void SimpleTimesyncTest::ReceiveSyncMessage( UINT16 msg_src, UINT64 EventTime, SimpleTimeSyncMsg* rcv_msg){

#ifdef DEBUG_TSYNC
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, TRUE );
	bool TimerReturn = VirtTimer_Stop(NbrClockMonitor_TIMER);
#endif

	UINT64 rcv_ltime;
	INT64 l_offset;
	rcv_ltime=  (((UINT64)rcv_msg->localTime1) <<32) + rcv_msg->localTime0;
	l_offset = rcv_ltime - EventTime;

	m_globalTime.regressgt2.Insert(msg_src, rcv_ltime, l_offset, (INT64) EventTime);

#ifdef DEBUG_TSYNC
	if (Nbr2beFollowed==0){ Nbr2beFollowed = msg_src; }
	if (msg_src == Nbr2beFollowed ){
		++RcvCount;
		if (RcvCount>=2  ){

			// RcvCount = 30; //This is to ensure preventing overflow on the RcvCount
			float relfreq = m_globalTime.regressgt2.FindRelativeFreq(msg_src);
			UINT32 NeighborsPeriodLength = (UINT32) (((float) NBCLOCKMONITORPERIOD)/relfreq); ///m_globalTime.regressgt.samples[nbrIndex].relativeFreq;
			INT64 y = HAL_Time_CurrentTime();
			UINT32 start_delay = (UINT32)(y - (INT64) EventTime); // Attempt to compansate for the difference
			//start_delay = 3200;

			VirtTimer_Change(NbrClockMonitor_TIMER, start_delay, NeighborsPeriodLength, TRUE);

			TimerReturn = VirtTimer_Start(NbrClockMonitor_TIMER);
		}
	}
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, FALSE );
#endif

}

BOOL SimpleTimesyncTest::Send(){

#ifdef DEBUG_TSYNC
	VirtTimer_Stop(LocalClockMonitor_TIMER);
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCSENDPIN, TRUE );
#endif
	Message_15_4_t * msg_15_4_ptr = &msg_15_4; //Create a new message
	IEEE802_15_4_Header_t * header = msg_15_4_ptr->GetHeader();
	SimpleTimeSyncMsg* m_timeSyncMsg = (SimpleTimeSyncMsg *) msg_15_4_ptr->GetPayload();

	INT64 y,d;
		y = HAL_Time_CurrentTime();
		m_timeSyncMsg->testnumber00 = 1;
		m_timeSyncMsg->testnumber01 = 2;
		m_timeSyncMsg->testnumber02 = 3;
		m_timeSyncMsg->testnumber03 = 4;
		m_timeSyncMsg->testnumberlong = 323;

		m_timeSyncMsg->localTime0 = (UINT32)(y & 0x0000FFFF);
		m_timeSyncMsg->localTime1 = (UINT32) (y>>32);

		m_timeSyncMsg->testnumber10 = 5;
		m_timeSyncMsg->testnumber11 = 6;
		m_timeSyncMsg->testnumber12 = 7;
		m_timeSyncMsg->testnumber13 = 8;

		m_timeSyncMsg->nodeID = 0;//BK: WILDHACK
		m_timeSyncMsg->seqNo = ++SendCount;
		m_timeSyncMsg->testnumber20 = 9;
		m_timeSyncMsg->testnumber21 = 10;
		m_timeSyncMsg->testnumber22 = 11;
		m_timeSyncMsg->testnumber23 = 12;
		header->dest = RADIO_BROADCAST_ADDRESS;
		header->type = MFM_TIMESYNC;
		Mac_SendTimeStamped(MacId, MAC_BROADCAST_ADDRESS, MFM_TIMESYNC, (void*) msg_15_4_ptr, sizeof(SimpleTimeSyncMsg), y );

#ifdef DEBUG_TSYNC
		CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCSENDPIN, FALSE );
		VirtTimer_Start(LocalClockMonitor_TIMER);
#endif
		return TRUE;
}

void SimpleTimesyncTest::SendAck(void *msg, UINT16 size, NetOpStatus status){
}


BOOL SimpleTimesyncTest::Initialize(){
	MyAppID = 3; //pick a number less than MAX_APPS currently 4.
	Config.Network = 138;
	myEventHandler.SetRecieveHandler(SimpleTimesyncTest_ReceiveHandler);
	myEventHandler.SetSendAckHandler(SimpleTimesyncTest_SendAckHandler);
	VirtTimer_Initialize();
	//CPU_GPIO_EnableOutputPin((GPIO_PIN) ACTIVITYPIN, FALSE);
#ifdef DEBUG_TIMESYNCTEST
	//CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
	//CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) NBRCLOCKMONITORPIN, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) LOCALCLOCKMONITORPIN, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) TIMESYNCSENDPIN, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) TIMESYNCRECEIVEPIN, FALSE);
#endif
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCRECEIVEPIN, FALSE );
	CPU_GPIO_SetPinState( (GPIO_PIN) TIMESYNCSENDPIN, FALSE );

	LocalClkMonitorPinState = false;
	NeighClkMonitorPinState = false;

	MacId = CSMAMAC;
	Mac_Initialize(&myEventHandler,MacId, MyAppID, Config.RadioID, (void*) &Config);

	Nbr2beFollowed=0;

	//gVirtualTimerObject.CreateTimer(SimpleTimesyncTest_Send_TIMER, 0, 10000000, FALSE, FALSE, SimpleTimesyncTest_Send_Timer_Handler); //10 sec Timer in micro seconds
	VirtTimer_SetTimer(SimpleTimesyncTest_Send_TIMER, 0, 10000000, FALSE, FALSE, SimpleTimesyncTest_Send_Timer_Handler);
	//gVirtualTimerObject.CreateTimer(LocalClockMonitor_TIMER, 0,(UINT32) NBCLOCKMONITORPERIOD, TRUE, FALSE, SimpleLocalClockMonitorTimerHandler);
	VirtTimer_SetTimer(LocalClockMonitor_TIMER, 0, (UINT32) NBCLOCKMONITORPERIOD, TRUE, FALSE, SimpleLocalClockMonitorTimerHandler);
	//gVirtualTimerObject.CreateTimer(NbrClockMonitor_TIMER,0, (UINT32) NBCLOCKMONITORPERIOD, TRUE, FALSE, SimpleNeighborClockMonitorTimerHandler);
	VirtTimer_SetTimer(NbrClockMonitor_TIMER, 0, (UINT32) NBCLOCKMONITORPERIOD, TRUE, FALSE, SimpleNeighborClockMonitorTimerHandler);
	//gVirtualTimerObject.CreateTimer(NbrClockMonitorStart_TIMER,0, (UINT32) NBCLOCKMONITORPERIOD, TRUE, FALSE, StartNeighborClockMonitoyTimerHandler);

	return TRUE;
}

BOOL SimpleTimesyncTest::StartTest(){
	SendCount=0;
	RcvCount=0;
	int a;
	VirtTimer_Start(SimpleTimesyncTest_Send_TIMER);
	while(1){
//		if(CPU_GPIO_GetPinState((GPIO_PIN) ACTIVITYPIN)) {
//			CPU_GPIO_SetPinState((GPIO_PIN) ACTIVITYPIN, FALSE);
//		}
//		else {
//			CPU_GPIO_SetPinState((GPIO_PIN) ACTIVITYPIN, TRUE);
//		}
//		//HAL_Time_Sleep_MicroSeconds(5000000);
	}
	return TRUE;
}

void SimpleTimesyncTest_Initialize(){
	BOOL ret;
	ret = gSimpleTimesyncTest.Initialize();
	ret = gSimpleTimesyncTest.StartTest();
}


