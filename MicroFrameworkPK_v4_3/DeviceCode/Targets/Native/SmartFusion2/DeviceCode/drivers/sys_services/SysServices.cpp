/*
 * SysServices.c
 *
 *  Created on: Mar 7, 2017
 *      Author: Mukundan
 */

#include <tinyhal.h>
#include "SysServices.h"
#include <intc/VectorIndex.h>

#define PORDIGEST_CHECK_SERV_RESP_LENGTH                2u
static volatile uint8_t g_request_in_progress = 0u;
static volatile uint16_t g_last_response_length = 0u;
static sys_serv_async_event_handler_t g_event_handler = 0;
static uint8_t g_response[PORDIGEST_CHECK_SERV_RESP_LENGTH] = {0u};
static uint32_t g_initial_mssddr_facc1_cr = 0U;
static uint8_t g_opcode=0u;

void ISR_ComBlk( void* Param ){
	uint8_t* intParam = (uint8_t*)Param;
	asynchronous_event_handler(*intParam);
}

void SF2_Services::InitializeSysServices() {
	//MSS_SYS_init(MSS_SYS_NO_EVENT_HANDLER);
	g_last_response_length = 0u;
	g_request_in_progress = 0u;

	/*
	 * Set a default good value for g_initial_mssddr_facc1_cr used to control
	 * the clock dividers coming in and out of Flash*Freeze.
	 */
	g_initial_mssddr_facc1_cr = SYSREG->MSSDDR_FACC1_CR;

	//Initialize emote intc driver with the same interrupt handler to avoid conflict.
	CPU_INTC_ActivateInterrupt(VectorIndex::c_IRQ_INDEX_ComBlk, ISR_ComBlk, (void*)&g_opcode);

	/*
	 * Initialize the COMBLK used to communicate with the System Controller.
	 */

	MSS_COMBLK_init(asynchronous_event_handler, g_response);
}
