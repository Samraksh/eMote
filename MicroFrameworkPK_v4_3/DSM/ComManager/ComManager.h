/*
 * ComManager.h
 *
 *  Created on: May 12, 2019
 *      Author: Komudi
 */

#ifndef _COM_MANAGER_H_
#define _COM_MANAGER_H_

#include <tinyhal.h>

enum MsgTypeE {
	M_UNKNOWN=100,
	M_ECDH_REQ,
	//M_ECDH_ACK
	M_ECDH_RES,
	M_ECDH_FIN,
	M_SEC_M_CH, //Secure Management channel
	M_SEC_D_CH, //secure data channel
	M_STATUS_RQ,
	M_STATUS_RES,
	M_SEC_STATUS_RQ,
	M_SEC_STATUS_RES
	//M_ECDH_TER
};

enum DeviceStatusE {
	DS_Unknown=64,
	DS_UnInit,
	DS_SecComm,
	DS_Good
};


//typedef void *RecvFnPtr (uint8_t * msg, uint16_t size);

//void CheckRecv(void *arg);

bool InitializeComManager();

bool SendToSecurityServer(void *msgStruct, uint16_t size, MsgTypeE mtype);

void RecvHandler (uint8_t * msg, uint16_t size);

void EcdhpStateMachine(UINT8 *msg, UINT8 size);




#endif //_COM_MANAGER_H_
