/*
 * SysServices.h
 *
 *  Created on: Mar 7, 2017
 *      Author: Mukundan
 */
#ifndef _SF2_SYS_SERVICES_H_
#define _SF2_SYS_SERVICES_H_

#include <cmsis/m2sxxx.h>
#include <drivers/mss_sys_services/mss_sys_services.h>
#include <drivers/mss_sys_services/mss_comblk.h>

void MSS_COMBLK_init (comblk_async_event_handler_t async_event_handler,uint8_t* p_response);
static void asynchronous_event_handler(uint8_t event_opcode);

struct SF2_Services {

	void InitializeSysServices();
};

#endif //_SF2_SYS_SERVICES_H_



