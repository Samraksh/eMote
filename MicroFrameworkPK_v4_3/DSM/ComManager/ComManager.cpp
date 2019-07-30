#include "ComManager.h"
#include "SocketOps.h"
#include <Samraksh/VirtualTimer.h>


bool comManagerInitialized=false;

bool InitializeComManager(){
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

bool SendToSecurityServer(void *msgStruct, MsgTypeE mtype){
	return TRUE;
}

void CheckRecv(void *arg){
	int recv_data = recv(client_fd,rx_buffer,sizeof(rx_buffer),0);
	if(recv_data > 0)
	{
		rx_buffer[recv_data] = '\0';
		hal_printf("received data: %s\n",rx_buffer);
		RecvHandler((uint8_t*)rx_buffer, recv_data);
	}
}

void RecvHandler (uint8_t * msg, uint16_t size){
	switch ((MsgTypeE)msg[0]) {
			case M_ECDH_REQ:
			case M_ECDH_RES:
			case M_ECDH_FIN:
				EcdhpStateMachine(msg,size);
				break;
			default:
				hal_printf("\nComManager:: Unknown message received\n");
	}
}
