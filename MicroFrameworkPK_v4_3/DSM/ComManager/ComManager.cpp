#include "ComManager.h"
//#include "SocketOps.h"
#include <Samraksh/VirtualTimer.h>


bool comManagerInitialized=false;
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

void SecureReceive(void* buffer, UINT16 size){
	hal_printf("\nComManager:: Received secure message of size %d\n", size);
	UINT8 msg[8]={'a','b', 'c', 'd', 'e', 'f', 'g', 'h'};
	BTMAC_Manager_Send(msg, 8, ENCRYPTED_DATA_CHANNEL);
}

void OpenReceive(void* buffer, UINT16 size){
	hal_printf("\nComManager:: Received open message of size %d\n", size);
}

void COM_Manager_Initialization(uint8_t _param){
	BTMAC_Manager_Initialization(_param, OpenReceive, SecureReceive);
}

void COM_Manager_Uninitialize(){
	BTMAC_Manager_Uninitialize();
}

bool SendToSecurityServer(void *msgStruct, MsgTypeE mtype){
	return TRUE;
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
			case M_SEC_M_CH: //Secure Management channel
				break;
			case M_SEC_D_CH: //Secure data channel

				break;
			default:
				hal_printf("\nComManager:: Unknown message received\n");
	}
}
