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
	M_UNKNOWN=0,
	M_ECDH_REQ,
	//M_ECDH_ACK
	M_ECDH_RES,
	M_ECDH_FIN
	//M_ECDH_TER
};


//typedef void *RecvFnPtr (uint8_t * msg, uint16_t size);

void CheckRecv(void *arg);

bool InitializeComManager();

bool SendToSecurityServer(void *msgStruct, MsgTypeE mtype);

void RecvHandler (uint8_t * msg, uint16_t size);

void EcdhpStateMachine(UINT8 *msg, UINT8 size);




#endif //_COM_MANAGER_H_
