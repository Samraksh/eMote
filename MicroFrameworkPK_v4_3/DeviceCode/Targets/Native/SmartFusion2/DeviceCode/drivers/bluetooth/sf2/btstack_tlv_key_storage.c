/*
 * Copyright (C) 2017 BlueKitchen GmbH
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
 *
 * THIS SOFTWARE IS PROVIDED BY MATTHIAS RINGWALD AND CONTRIBUTORS
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
 */

#define __BTSTACK_FILE__ "btstack_tlv_flash_bank.c"

#include "..\btcore\btstack_tlv.h"
#include "..\btcore\btstack_util.h"
#include "..\btcore\btstack_debug.h"
#include "btstack_tlv_key_storage.h"

#include <string.h>

#define BTSTACK_TLV_HEADER_LEN 8

#ifndef BTSTACK_FLASH_ALIGNMENT_MAX
#define BTSTACK_FLASH_ALIGNMENT_MAX 8
#endif


/**
 * Get Value for Tag
 * @param tag
 * @param buffer
 * @param buffer_size
 * @returns size of value
 */
static int btstack_tlv_flash_bank_get_tag(void * context, uint32_t tag, uint8_t * buffer, uint32_t buffer_size){
	log_info("^^^ get tag");
/*
	btstack_tlv_flash_bank_t * self = (btstack_tlv_flash_bank_t *) context;

	uint32_t tag_index = 0;
	uint32_t tag_len   = 0;
	tlv_iterator_t it;
	btstack_tlv_flash_bank_iterator_init(self, &it, self->current_bank);
	while (btstack_tlv_flash_bank_iterator_has_next(self, &it)){
		if (it.tag == tag){
			log_info("Found tag '%x' at position %u", tag, it.offset);
			tag_index = it.offset;
			tag_len   = it.len;
			break;
		}
		tlv_iterator_fetch_next(self, &it);
	}
	if (tag_index == 0) return 0;
	if (!buffer) return tag_len;
	int copy_size = btstack_min(buffer_size, tag_len);
	btstack_tlv_flash_bank_read(self, self->current_bank, tag_index + 8, buffer, copy_size);
	return copy_size;*/
	return 0;
}

/**
 * Store Tag 
 * @param tag
 * @param data
 * @param data_size
 */
static int btstack_tlv_flash_bank_store_tag(void * context, uint32_t tag, const uint8_t * data, uint32_t data_size){
	log_info("^^^ store tag");
/*
	btstack_tlv_flash_bank_t * self = (btstack_tlv_flash_bank_t *) context;

	// trigger migration if not enough space
	if (self->write_offset + 8 + data_size > self->hal_flash_bank_impl->get_size(self->hal_flash_bank_context)){
		btstack_tlv_flash_bank_migrate(self);
	}

	if (self->write_offset + 8 + data_size > self->hal_flash_bank_impl->get_size(self->hal_flash_bank_context)){
		log_error("couldn't write entry, not enough space left");
		return 2;
	}

	// prepare entry
	uint8_t entry[8];
	big_endian_store_32(entry, 0, tag);
	big_endian_store_32(entry, 4, data_size);

	log_info("write '%x', len %u at %u", tag, data_size, self->write_offset);

	// write value first
	btstack_tlv_flash_bank_write(self, self->current_bank, self->write_offset + 8, data, data_size);

	// then entry
	btstack_tlv_flash_bank_write(self, self->current_bank, self->write_offset, entry, sizeof(entry));

	// overwrite old entries (if exists)
	btstack_tlv_flash_bank_delete_tag_until_offset(self, tag, self->write_offset);

	// done
	self->write_offset += sizeof(entry) + btstack_tlv_flash_bank_align_size(self, data_size);
*/
	return 0;
}

/**
 * Delete Tag
 * @param tag
 */
static void btstack_tlv_flash_bank_delete_tag(void * context, uint32_t tag){
	log_info("^^^ delete tag");
	/*btstack_tlv_flash_bank_t * self = (btstack_tlv_flash_bank_t *) context;
	btstack_tlv_flash_bank_delete_tag_until_offset(self, tag, self->write_offset);*/
}

static const btstack_tlv_t btstack_tlv_flash_bank = {
	/* int  (*get_tag)(..);     */ &btstack_tlv_flash_bank_get_tag,
	/* int (*store_tag)(..);    */ &btstack_tlv_flash_bank_store_tag,
	/* void (*delete_tag)(v..); */ &btstack_tlv_flash_bank_delete_tag,
};

/**
 * Init Tag Length Value Store
 */
const btstack_tlv_t * btstack_tlv_flash_bank_init_instance(){

	log_info("^^^ init bank");
	/*self->hal_flash_bank_impl    = hal_flash_bank_impl;
	self->hal_flash_bank_context = hal_flash_bank_context;

	// try to find current bank
	self->current_bank = btstack_tlv_flash_bank_get_latest_bank(self);
	log_info("found bank %d", self->current_bank);
	if (self->current_bank >= 0){

		// find last entry and write offset
		tlv_iterator_t it;
		uint32_t last_tag = 0;
		uint32_t last_offset = 0;
		btstack_tlv_flash_bank_iterator_init(self, &it, self->current_bank);
		while (btstack_tlv_flash_bank_iterator_has_next(self, &it)){
			last_tag = it.tag;
			last_offset = it.offset;
			tlv_iterator_fetch_next(self, &it);
		}
		self->write_offset = it.offset;

		if (self->write_offset < self->hal_flash_bank_impl->get_size(self->hal_flash_bank_context)){

			// delete older instances of last_tag
			// this handles the unlikely case where MCU did reset after new value + header was written but before delete did complete
			if (last_tag){
				btstack_tlv_flash_bank_delete_tag_until_offset(self, last_tag, last_offset);
			}

			// verify that rest of bank is empty
			// this handles the unlikely case where MCU did reset after new value was written, but not the tag
			if (!btstack_tlv_flash_bank_test_erased(self, self->current_bank, self->write_offset)){
				log_info("Flash not empty after last found tag -> migrate");
				btstack_tlv_flash_bank_migrate(self);
			} else {
				log_info("Flash clean after last found tag");
			}
		} else {
			// failure!
			self->current_bank = -1;
		}
	} 

	if (self->current_bank < 0) {
		btstack_tlv_flash_bank_erase_bank(self, 0);
		self->current_bank = 0;
		btstack_tlv_flash_bank_write_header(self, self->current_bank, 0);	// epoch = 0;
		self->write_offset = 8;
	}

	log_info("write offset %u", self->write_offset);*/
	return &btstack_tlv_flash_bank;
}

