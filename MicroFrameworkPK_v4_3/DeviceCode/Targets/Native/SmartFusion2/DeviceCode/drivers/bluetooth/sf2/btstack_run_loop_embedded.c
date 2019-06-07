/*
 * Copyright (C) 2014 BlueKitchen GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 4. Any redistribution, use, or modification is done solely for
 *    personal benefit and not for any commercial purpose or for
 *    monetary gain.
 *
 * THIS SOFTWARE IS PROVIDED BY BLUEKITCHEN GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MATTHIAS
 * RINGWALD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Please inquire about commercial licensing options at 
 * contact@bluekitchen-gmbh.com
 *
 */

#define __BTSTACK_FILE__ "btstack_run_loop_embedded.c"

/*
 *  btstack_run_loop_embedded.c
 *
 *  For this run loop, we assume that there's no global way to wait for a list
 *  of data sources to get ready. Instead, each data source has to queried
 *  individually. Calling ds->isReady() before calling ds->process() doesn't 
 *  make sense, so we just poll each data source round robin.
 *
 *  To support an idle state, where an MCU could go to sleep, the process function
 *  has to return if it has to called again as soon as possible
 *
 *  After calling process() on every data source and evaluating the pending timers,
 *  the idle hook gets called if no data source did indicate that it needs to be
 *  called right away.
 *
 */


#include <stddef.h> // NULL

#include "..\btcore\btstack_debug.h"
#include "..\btcore\btstack_run_loop.h"
#include "btstack_run_loop_embedded.h"
#include "..\btcore\btstack_linked_list.h"
#include "hal_tick.h"
#include "hal_cpu.h"

#include "..\btcore\btstack_debug.h"

#if defined(HAVE_EMBEDDED_TICK) && defined(HAVE_EMBEDDED_TIME_MS)
#error "Please specify either HAVE_EMBEDDED_TICK or HAVE_EMBEDDED_TIME_MS"
#endif

#if defined(HAVE_EMBEDDED_TICK) || defined(HAVE_EMBEDDED_TIME_MS)
#define TIMER_SUPPORT
#endif

static const btstack_run_loop_t btstack_run_loop_embedded;

// the run loop
static btstack_linked_list_t data_sources;

static btstack_linked_list_t timers;

static int trigger_event_received = 0;
//static uint64_t commTest;
/**
 * Add data_source to run_loop
 */
static void btstack_run_loop_embedded_add_data_source(btstack_data_source_t *ds){
	log_info("add data source\r\n");
    btstack_linked_list_add(&data_sources, (btstack_linked_item_t *) ds);
}

/**
 * Remove data_source from run loop
 */
static int btstack_run_loop_embedded_remove_data_source(btstack_data_source_t *ds){
	log_info("remove data source\r\n");
    return btstack_linked_list_remove(&data_sources, (btstack_linked_item_t *) ds);
}

// set timer
static void btstack_run_loop_embedded_set_timer(btstack_timer_source_t *ts, uint32_t timeout_in_ms){
    ts->timeout = hal_time_ms() + timeout_in_ms + 1;
}

// under the assumption that a tick value is +/- 2^30 away from now, calculate the upper bits of the tick value
static int btstack_run_loop_embedded_reconstruct_higher_bits(uint64_t now, uint64_t ticks){
    uint64_t delta = ticks - now;
    if (delta >= 0){
        if (ticks >= now) {
            return 0;
        } else {
            return 1;
        }
    } else {
        if (ticks < now) {
            return 0;
        } else {
            return -1;
        }
    }
}

/**
 * Add timer to run_loop (keep list sorted)
 */
static void btstack_run_loop_embedded_add_timer(btstack_timer_source_t *ts){

    uint64_t now = hal_time_ms();

    uint64_t new_low = ts->timeout;
    uint64_t     new_high = btstack_run_loop_embedded_reconstruct_higher_bits(now, new_low);
	log_info("btstack_run_loop_embedded_add_timer %llu %llu\r\n", now, ts->timeout);

    btstack_linked_item_t *it;
    for (it = (btstack_linked_item_t *) &timers; it->next ; it = it->next){
        // don't add timer that's already in there
        if ((btstack_timer_source_t *) it->next == ts){
            log_error( "btstack_run_loop_timer_add error: timer to add already in list!");
            return;
        }
        uint64_t next_low  = ((btstack_timer_source_t *) it->next)->timeout;
        uint64_t      next_high = btstack_run_loop_embedded_reconstruct_higher_bits(now, next_low);
        if (new_high < next_high || ((new_high == next_high) && (new_low < next_low))){
            break;
        }
    }
    ts->item.next = it->next;
    it->next = (btstack_linked_item_t *) ts;
}

/**
 * Remove timer from run loop
 */
static int btstack_run_loop_embedded_remove_timer(btstack_timer_source_t *ts){
	log_info("remove timer\r\n");
    return btstack_linked_list_remove(&timers, (btstack_linked_item_t *) ts);
}

static void btstack_run_loop_embedded_dump_timer(void){
#ifdef ENABLE_LOG_INFO 
    btstack_linked_item_t *it;
    int i = 0;
    for (it = (btstack_linked_item_t *) timers; it ; it = it->next){
        btstack_timer_source_t *ts = (btstack_timer_source_t*) it;
        log_info("(%llu) timer %d, timeout %llu\n", hal_time_ms(), i, ts->timeout);
    }
#endif
}

static void btstack_run_loop_embedded_enable_data_source_callbacks(btstack_data_source_t * ds, uint16_t callback_types){
	log_info("btstack_run_loop_embedded_enable_data_source_callbacks\r\n");
    ds->flags |= callback_types;
}

static void btstack_run_loop_embedded_disable_data_source_callbacks(btstack_data_source_t * ds, uint16_t callback_types){
    ds->flags &= ~callback_types;
}

/**
 * Execute run_loop once
 */
void btstack_run_loop_embedded_execute(void){}

void btstack_run_loop_embedded_execute_once(void) {
    btstack_data_source_t *ds;

    // process data sources
    btstack_data_source_t *next;
    for (ds = (btstack_data_source_t *) data_sources; ds != NULL ; ds = next){
        next = (btstack_data_source_t *) ds->item.next; // cache pointer to next data_source to allow data source to remove itself
        if (ds->flags & DATA_SOURCE_CALLBACK_POLL){
            ds->process(ds, DATA_SOURCE_CALLBACK_POLL);
        }
    }

    uint64_t now = hal_time_ms();
	/*if (now > commTest){
		commTest = hal_time_ms() + 5000;
		log_info("<comm test>");
		hci_test_chip_comm();
	}*/


    // process timers
    while (timers) {
		//btstack_run_loop_embedded_dump_timer();
        btstack_timer_source_t *ts = (btstack_timer_source_t *) timers;
        uint64_t timeout_low = ts->timeout;
        uint64_t      timeout_high = btstack_run_loop_embedded_reconstruct_higher_bits(now, timeout_low);
        if (timeout_high > 0 || ((timeout_high == 0) && (timeout_low > now))) break;
        btstack_run_loop_embedded_remove_timer(ts);
		log_info("expired timer: %llu %llu %llu\r\n", timeout_high, timeout_low, now);
        ts->process(ts);
    }
    
    // disable IRQs and check if run loop iteration has been requested. if not, go to sleep
    hal_cpu_disable_irqs();
    if (trigger_event_received){
        trigger_event_received = 0;
        hal_cpu_enable_irqs();
    } else {
        hal_cpu_enable_irqs_and_sleep();
    }
}

/**
 * Execute run_loop
 */

static uint64_t btstack_run_loop_embedded_get_time_ms(void){
    return hal_time_ms();
}


/**
 * trigger run loop iteration
 */
void btstack_run_loop_embedded_trigger(void){
    trigger_event_received = 1;
	btstack_run_loop_embedded_execute_once();
}

static void btstack_run_loop_embedded_init(void){
	log_info("btstack_run_loop_embedded_init\r\n");
    data_sources = NULL;

    timers = NULL;

	//commTest = hal_time_ms() + 5000;
}

/**
 * Provide btstack_run_loop_embedded instance 
 */
const btstack_run_loop_t * btstack_run_loop_embedded_get_instance(void){
    return &btstack_run_loop_embedded;
}

static const btstack_run_loop_t btstack_run_loop_embedded = {
    &btstack_run_loop_embedded_init,
    &btstack_run_loop_embedded_add_data_source,
    &btstack_run_loop_embedded_remove_data_source,
    &btstack_run_loop_embedded_enable_data_source_callbacks,
    &btstack_run_loop_embedded_disable_data_source_callbacks,
    &btstack_run_loop_embedded_set_timer,
    &btstack_run_loop_embedded_add_timer,
    &btstack_run_loop_embedded_remove_timer,
    &btstack_run_loop_embedded_execute,
    &btstack_run_loop_embedded_dump_timer,
    &btstack_run_loop_embedded_get_time_ms,
};
