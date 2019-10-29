#include "ComManager.h"
//#include "SocketOps.h"
#include <Samraksh/VirtualTimer.h>

uint8_t deviceStatus=DS_UnInit;
bool comManagerInitialized=false;

const uint8_t exkey[32] = {0xb8, 0xd4, 0x23, 0x98, 0x20, 0x1b, 0x53, 0xbd,
	0x47, 0xe2, 0x7e, 0x93, 0x3c, 0xb5, 0x91, 0xb6,
	0xac, 0xa8, 0x86, 0x36, 0x51, 0x93, 0xa8, 0x8d,
	0x5f, 0x02, 0x30, 0x6f, 0xb9, 0xea, 0x23, 0xab};
/*
bool InitializeComManagerSockets(){
	if(comManagerInitialized) return true;

	ClientInit(false);
	if ( client_fd < 0 )
		{
			hal_printf("ComManager::Initialize: socket call failed\n");
			return false;
		}

	if(VirtTimer_SetOrChangeTimer(VIRT_TIMER_NET_SOC, 0, 50000, FALSE, TRUE, CheckRecv, ADVTIMER_32BIT) != TimerSupported){
				ASSERT(FALSE);
				return DS_Fail;
	}
	VirtTimer_Start(VIRT_TIMER_NET_SOC);
	comManagerInitialized=true;

	return true;
}
*/
void PrintHex(UINT8* sig, int size){
	for (int j=0;j<size; j++){
		hal_printf("0x%.2X , ",sig[j]);
		if ((j%16)==0) hal_printf("\r\n");
	}
	hal_printf("\r\n");
}


void SecureReceive(void* buffer, UINT16 size){
	hal_printf("\nComManager:: Received secure message of size %d\n", size);
	//UINT8 msg[8]={'a','b', 'c', 'd', 'e', 'f', 'g', 'h'};
	//BTMAC_Manager_Send(msg, 8, ENCRYPTED_DATA_CHANNEL);
	UINT8 *msg = (UINT8*)buffer;
	switch(msg[0]){
		case M_SEC_D_CH: //secure data channel
			//This is secure data message
			break;
		case M_SEC_BIN_RES:
			//This is binary update message
			break;
		case M_SEC_STATUS_RQ:
			UINT8 reply[8];
			reply[0]= M_SEC_STATUS_RES;
			reply[1]= deviceStatus;
			hal_printf("ComManager:: SecureReceive: This is a status request from Cloud, responding \n");
			BTMAC_Manager_Send(reply, 8, ENCRYPTED_DATA_CHANNEL);
			break;
			//should not get this
		case M_SEC_BIN_RQ: //device send this message
		case M_SEC_STATUS_RES:
		case M_UNKNOWN:
		default:
			hal_printf("\nComManager:: Received  unknown open message of size %d\n", size);
	}
}

void RequestNewBinary(){
	UINT8 reply[8];
	reply[0]= M_SEC_BIN_RQ;
	reply[1]= deviceStatus;
	hal_printf("ComManager:: Requestig New Binary from Enterprise \n");
	//BTMAC_Manager_Send(reply, 8, ENCRYPTED_DATA_CHANNEL);
	BTMAC_Manager_Send(reply, 8, CLOUD_CHANNEL);
}

void SendDetectMessage(){
	UINT8 reply[8];
	reply[0]= M_SEC_DETECT;
	reply[1]= deviceStatus;
	hal_printf("Sending detect message to gateway\n");
	//BTMAC_Manager_Send(reply, 8, ENCRYPTED_DATA_CHANNEL);
	BTMAC_Manager_Send(reply, 8, CLOUD_CHANNEL);
}

void OpenCloudReceive(void* buffer, UINT16 size){
	hal_printf("ComManager:: Received open message of size %d\n", size);
	UINT8 *msg = (UINT8*)buffer;
	switch(msg[0]){
		case M_ECDH_REQ:
		case M_ECDH_RES:
		case M_ECDH_FIN:
			EcdhpStateMachine(msg, size);
			break;
		case M_STATUS_RQ:
			UINT8 reply[8];
			reply[0]= M_STATUS_RES;
			reply[1]= deviceStatus;
			hal_printf("ComManager:: OpenReceive: This is a status request from Cloud, responding \n");
			BTMAC_Manager_Send(reply, 8, CLOUD_CHANNEL);

			//RequestNewBinary();
			break;
		case M_UNKNOWN:
		default:
			hal_printf("\nComManager:: Received  unknown open message of size %d\n", size);
	}
}

void COM_Manager_Initialization(uint8_t _param){
	BTMAC_Manager_Initialization(_param, OpenCloudReceive, SecureReceive);
}

void COM_Manager_Uninitialize(){
	BTMAC_Manager_Uninitialize();
}

bool SendToSecurityServer(void *msgStruct,  uint16_t size, MsgTypeE mtype){
	PrintHex((UINT8*)msgStruct, size);
	return BTMAC_Manager_Send(msgStruct, size, CLOUD_CHANNEL);
}

/*
void CheckRecv(void *arg){
	int recv_data = recv(client_fd,rx_buffer,sizeof(rx_buffer),0);
	if(recv_data > 0)
	{
		rx_buffer[recv_data] = '\0';
		hal_printf("received data: %s\n",rx_buffer);
		RecvHandler((uint8_t*)rx_buffer, recv_data);
	}
}
*/

void RecvHandler (uint8_t * msg, uint16_t size){
	switch ((MsgTypeE)msg[0]) {
			case M_ECDH_REQ:
			case M_ECDH_RES:
			case M_ECDH_FIN:
				EcdhpStateMachine(msg,size);
				break;
			//case M_SEC_M_CH: //Secure Management channel
				break;
			case M_SEC_D_CH: //Secure data channel

				break;
			default:
				hal_printf("\nComManager:: Unknown message received\n");
	}
}
