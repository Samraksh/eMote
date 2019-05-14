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

bool InitializeServerCom();

bool SendToSecurityServer(void *msgStruct, MsgTypeE mtype);


#endif //_COM_MANAGER_H_
