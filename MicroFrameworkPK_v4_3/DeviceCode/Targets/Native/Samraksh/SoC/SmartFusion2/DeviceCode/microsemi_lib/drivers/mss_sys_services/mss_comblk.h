/*******************************************************************************
 * (c) Copyright 2012-2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * SmartFusion2 COMBLK access functions.
 *
 * SVN $Revision: 7294 $
 * SVN $Date: 2015-04-14 14:33:48 +0530 (Tue, 14 Apr 2015) $
 */
#ifndef __MSS_COMBLK_H_
#define __MSS_COMBLK_H_ 1

#include "../../../microsemi_lib/CMSIS/m2sxxx.h"
#include "../../../microsemi_lib/drivers/mss_sys_services/mss_comblk_page_handler.h"

#ifdef __cplusplus
extern "C" {
#endif
  
/*------------------------------------------------------------------------------
 *
 */
typedef void(*comblk_completion_handler_t)(uint8_t * p_response, uint16_t response_size);

typedef void (*comblk_async_event_handler_t)(uint8_t event_opcode);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_init
(
    comblk_async_event_handler_t async_event_handler, 
    uint8_t* p_response
);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_send_cmd_with_ptr
(
    uint8_t cmd_opcode,
    uint32_t cmd_params_ptr,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_send_cmd
(
    const uint8_t * p_cmd,
    uint16_t cmd_size,
    const uint8_t * p_data,
    uint32_t data_size,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_read
(
    const uint8_t * p_data,
    uint16_t cmd_size,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_send_paged_cmd
(
    const uint8_t * p_cmd,
    uint16_t cmd_size,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_page_handler_t page_read_handler,
    comblk_completion_handler_t completion_handler
);

#ifdef __cplusplus
}
#endif

#endif /* __MSS_COMBLK_H_ */
