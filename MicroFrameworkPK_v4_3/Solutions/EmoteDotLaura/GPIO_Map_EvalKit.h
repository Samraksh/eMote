#ifndef _EMOTE_DOT_LAURA_GPIOMAP_H_
#define _EMOTE_DOT_LAURA_GPIOMAP_H_

//Assign GPIO Pins to various hardware modules that are used.
//Leave unused pins as such, so that they can be used in future.
//All GPIO pins must be defined here to avoid conflict





//Port0


//Port1
#define EXT_TIMER0_INPUT_PIN (GPIO_PIN)12 //P1.4

//Port2


#define LED0 (GPIO_PIN)20
#define LED1 (GPIO_PIN)21
#define LED2 (GPIO_PIN)22

//Port3

//Port4
//Port5
//Port6

//Port7
//IMPORTANT: The general purpose test pins must be used only in tests from TestSuite.
//i.e., they can be reused in tests. If you need dedicated tests for debuging module dont use these pins
//Test Suite test pins
#define TEST_PIN0 (GPIO_PIN)56
#define TEST_PIN1 (GPIO_PIN)57
#define TEST_PIN2 (GPIO_PIN)58
#define TEST_PIN3 (GPIO_PIN)59
#define TEST_PIN4 (GPIO_PIN)60
#define TEST_PIN5 (GPIO_PIN)61
#define TEST_PIN6 (GPIO_PIN)62
#define TEST_PIN7 (GPIO_PIN)63


#define DATATX_NEXT_EVENT					(GPIO_PIN)120//22
#define DATARX_NEXT_EVENT					(GPIO_PIN)120//23
#define DATATX_DATA_PIN						(GPIO_PIN)120//24//22
#define DATARX_HANDLE_END_OF_RX				(GPIO_PIN)120
#define OMAC_CONTINUATION					(GPIO_PIN)120//23
#define DATARX_SEND_SW_ACK					(GPIO_PIN)120//23//22
#define OMAC_DRIVING_RADIO_SEND				(GPIO_PIN)120//22
#define DATARX_EXEC_EVENT					(GPIO_PIN)120//22//23
#define SI4468_HANDLE_SLEEP					(GPIO_PIN)120//0//23
//#define SCHED_RX_EXEC_PIN 					(GPIO_PIN)06171015//22 //THis is a duplicate definition
#define OMAC_DISCO_EXEC_EVENT				(GPIO_PIN)120//23//0
#define EMOTE_NET_MAC_GETNEXTPACKET			(GPIO_PIN)120
#define EMOTE_NET_MANAGED_CALLBACK			(GPIO_PIN)120//0
#define OMAC_DATARXPIN						(GPIO_PIN)120//0
#define OMAC_DRIVING_RADIO_RECV				(GPIO_PIN)120//22//23
#define OMAC_DRIVING_RADIO_SLEEP			(GPIO_PIN)120//22//0
#define OMAC_SCHED_POST_POST_EXEC			(GPIO_PIN)120//22//0
#define OMAC_DISCO_POST_EXEC				(GPIO_PIN)120//23//0
#define DATATX_RECV_SW_ACK					(GPIO_PIN)120//23
#define OMAC_TIMESYNC_NEXT_EVENT			(GPIO_PIN)120//24
#define DISCO_NEXT_EVENT					(GPIO_PIN)120
#define DISCO_BEACON_N						(GPIO_PIN)120//25
#define SI4468_HANDLE_CCA					(GPIO_PIN)122//22
#define SI4468_HANDLE_INTERRUPT_TX			(GPIO_PIN)120//0//24
#define SI4468_MEASURE_RX_TIME				(GPIO_PIN)120//24
#define SI4468_HANDLE_INTERRUPT_RX			(GPIO_PIN)120//0
#define SCHED_NEXT_EVENT					(GPIO_PIN)120//0


#define OMAC_TESTING_SCHEDULER_PIN 			(GPIO_PIN)120//24
#define OMAC_TESTING_VTIMER_PIN 			(GPIO_PIN)120


#define VT_CALLBACK 						(GPIO_PIN)120					//J11_PIN7

#define NEIGHBORCLOCKMONITORPIN 			(GPIO_PIN)120 // 120 //31 //2
#define LOCALCLOCKMONITORPIN 				(GPIO_PIN)120 //120 //22 //25

//RF231 related
#define RF231_TX_INTERRUPT 					(GPIO_PIN)120					//J11_PIN7
#define RF231_RADIO_STATEPIN2 				(GPIO_PIN)120					//J11_PIN7
#define RF231_RX 							(GPIO_PIN)120								//J11_PIN8
#define RF231_TX_TIMESTAMP 					(GPIO_PIN)120								//J11_PIN9
#define RF231_TX 							(GPIO_PIN)120								//J11_PIN10
#define RF231_FRAME_BUFF_ACTIVE 			(GPIO_PIN)120
#define RF231_SEND_FAIL						(GPIO_PIN)120
#define RF231_START_OF_RX_MODE 				(GPIO_PIN)120 //24 (<--31)
#define RF231_GENERATE_HW_ACK				(GPIO_PIN)120 //(<--30)
#define RF231_HW_ACK_RESP_TIME				(GPIO_PIN)120
#define CCA_PIN								(GPIO_PIN)120
#define RF231_RX_START						(GPIO_PIN)120
#define RF231_AMI							(GPIO_PIN)120
#define RF231_TRX_RX_END 					(GPIO_PIN)120
#define RF231_TRX_TX_END 					(GPIO_PIN)120 //(<--4)

#define SI4468_TX							(GPIO_PIN)120
#define SI4468_TX_TIMESTAMP					(GPIO_PIN)120
#define SI4468_Radio_STATE					(GPIO_PIN)120
#define SI4468_Radio_TX_Instance			(GPIO_PIN)120 //(GPIO_PIN)23 //(GPIO_PIN)22 //(GPIO_PIN)120
#define SI4468_Radio_TX_Instance_NOTS		(GPIO_PIN)120


//Acknowledgements from radio
#define SEND_ACK_PIN						(GPIO_PIN)120 //29//120
#define DATA_RX_INTERRUPT_PIN				(GPIO_PIN)120 //120
#define DATA_TX_ACK_PIN						(GPIO_PIN)120 //120

//Acknowledgements from other node
#define OMAC_TX_DATAACK_PIN 				(GPIO_PIN)120 //120 //120 //23  //120							//J11_pin3 0
#define OMAC_RX_DATAACK_PIN 				(GPIO_PIN)120 //120 // 120 //23  //120							//J11_pin3 0

//Radio Control
#define RADIOCONTROL_STATEPIN 				(GPIO_PIN)120 //120 //23 //120 // 120 //120 				//J11_pin6 //This 	(GPIO_PIN)3  did not work

//TX Related
#define RADIOCONTROL_SEND_PIN 				(GPIO_PIN)120 				//J11_pin4
#define RADIOCONTROL_SENDTS_PIN 			(GPIO_PIN)120 //(<--2) 				//J11_pin5
#define DISCO_SYNCSENDPIN 					(GPIO_PIN)120 // 24  //120						//J12_PIN1
#define DATATX_PIN 							(GPIO_PIN)120 //(<--2) //29 //120 //120 //2							//J12_PIN3
#define RC_TX_TIMESYNCREQ 					(GPIO_PIN)120
#define RC_TX_DATA 							(GPIO_PIN)120	//29 (<--29)
//#define DATATX_DATA_PIN				(GPIO_PIN)120 //25
#define DATARX_TIMESTAMP_PIN				(GPIO_PIN)120 //29
#define DATATX_POSTEXEC						(GPIO_PIN)120
#define DATATX_RECV_HW_ACK					(GPIO_PIN)120 //(<--4)
#define FAST_RECOVERY_SEND					(GPIO_PIN)120 //(<--0)
#define DATATX_SEND_ACK_HANDLER				(GPIO_PIN)120
#define OMAC_RADIOCONTROL_RADIO_SEND_TOGGLER			(GPIO_PIN)120 //SI4468_Radio_TX_Instance //(GPIO_PIN)120
#define DATATX_SendACKHandler_PIN_TOGGLER				(GPIO_PIN)120 //(GPIO_PIN)22 //SCHED_TX_EXEC_PIN //(GPIO_PIN)120
#define DATATX_ReceiveDATAACK_PIN_TOGGLER				(GPIO_PIN)120 //(GPIO_PIN)120
#define DATATX_CCA_PIN_TOGGLER				(GPIO_PIN)120
#define DATATX_TIMING_ERROR_PIN_TOGGLER 	(GPIO_PIN)120
#define DTH_STATE_PIN_TOGGLER				(GPIO_PIN)120 //SCHED_TX_EXEC_PIN

//RX related
#define OMAC_RXPIN 							(GPIO_PIN)120 //(GPIO_PIN)0 //120 //23  //120							//J11_pin3 0
//#define OMAC_DATARXPIN 	(GPIO_PIN)120 //0 //26	//120 //2					//J12_pin5
#define OMAC_TIMESYNCREQRXPIN 				(GPIO_PIN)120 //23 //30
#define DISCO_SYNCRECEIVEPIN 				(GPIO_PIN)120 //25 //120					//J12_PIN2
#define DATARECEPTION_SLOTPIN 				(GPIO_PIN)120 //30 //31 //2				//J12_PIN4
#define DATARECEPTION_RADIOONPIN 			(GPIO_PIN)120 //30 //31 //2				//J12_PIN4
#define DATARX_TIMING_ERROR_PIN_TOGGLER 	(GPIO_PIN)120

//Timesync related
#define TIMESYNC_GENERATE_MESSAGEPIN 		(GPIO_PIN)120 //(<--23) //25 //120 //24 //120// // 3 				//J11_pin6
#define TIMESYNC_RECEIVEPIN 				(GPIO_PIN)120 //24 			//J12_pin5

//Scheduler Related
#define SCHED_START_STOP_PIN 				(GPIO_PIN)120 //4
#define SCHED_RX_EXEC_PIN 					(GPIO_PIN)120 //(GPIO_PIN)25 //25 //4
#define SCHED_TX_EXEC_PIN 					(GPIO_PIN)120 //(GPIO_PIN)24 //24 //4
#define SCHED_DISCO_EXEC_PIN 				(GPIO_PIN)120 //0 //4
#define SCHED_TSREQ_EXEC_PIN 				(GPIO_PIN)120 //23 //4
#define DATARX_NEXTEVENT 					(GPIO_PIN)120
#define DATATX_NEXTEVENT 					(GPIO_PIN)120
#define DATATX_SCHED_DATA_PKT 				(GPIO_PIN)120
#define DATA_RX_END_OF_RECEPTION			(GPIO_PIN)120

//MISC
#define DELAY_BETWEEN_DATATX_SCHED_ACTUAL	(GPIO_PIN)120	//J12_pin5
#define OMAC_DEBUG_PIN 						(GPIO_PIN)120			 			//J11_PIN5
#define VTIMER_CALLBACK_LATENCY_PIN			(GPIO_PIN)120 //(<--31)

#define VIRTUAL_TIMER_EXCEPTION_CHECK_PIN (GPIO_PIN)120 //SCHED_TSREQ_EXEC_PIN


#endif //_EMOTE_DOT_LAURA_GPIOMAP_H_
