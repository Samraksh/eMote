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

#define __BTSTACK_FILE__ "spp_and_le_counter.c"

// *****************************************************************************
/* EXAMPLE_START(spp_and_le_counter): Dual mode example
 * 
 * @text The SPP and LE Counter example combines the Bluetooth Classic SPP Counter
 * and the Bluetooth LE Counter into a single application.
 *
 * @text In this Section, we only point out the differences to the individual examples
 * and how the stack is configured.
 *
 * @text Note: To test, please run the example, and then: 
 *    - for SPP pair from a remote device, and open the Virtual Serial Port,
 *    - for LE use some GATT Explorer, e.g. LightBlue, BLExplr, to enable notifications.
 */
// *****************************************************************************

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
 
#include "..\btcore\btstack.h"
#include "..\btcore\btstack_debug.h"
#include "..\btcore\classic\rfcomm.h"
#include "..\btcore\classic\sdp_server.h"
#include "..\btcore\classic\sdp_util.h"
#include "..\btcore\ble\le_device_db.h"
#include "..\btcore\ble\att_server.h"
#include "..\btcore\gap.h"
#include "..\btcore\ble\gatt_client.h"
#include "btmain.h"

#define RFCOMM_SERVER_CHANNEL 1
#define HEARTBEAT_PERIOD_MS 1000

#define BLUETOOTH_MASTER 1

#define TEST_MODE 1
#define TEST_MODE_ENABLE_NOTIFICATIONS 1

// prototypes
static void handle_gatt_client_event(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);

typedef enum {
    TC_OFF,
    TC_IDLE,
    TC_W4_SCAN_RESULT,
    TC_W4_CONNECT,
    TC_W4_SERVICE_RESULT,
    TC_W4_CHARACTERISTIC_RX_RESULT,
    TC_W4_CHARACTERISTIC_TX_RESULT,
    TC_W4_ENABLE_NOTIFICATIONS_COMPLETE,
    TC_W4_TEST_DATA
} gc_state_t;

// On the GATT Server, RX Characteristic is used for receive data via Write, and TX Characteristic is used to send data via Notifications
static uint8_t le_streamer_service_uuid[16]           = { 0x00, 0x00, 0xFF, 0x10, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};
static uint8_t le_streamer_characteristic_rx_uuid[16] = { 0x00, 0x00, 0xFF, 0x11, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};
static uint8_t le_streamer_characteristic_tx_uuid[16] = { 0x00, 0x00, 0xFF, 0x11, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};

static gatt_client_service_t le_streamer_service;
static gatt_client_characteristic_t le_streamer_characteristic_rx;
static gatt_client_characteristic_t le_streamer_characteristic_tx;

static gatt_client_notification_t notification_listener;
static int listener_registered = 0;

static gc_state_t state = TC_OFF;
static btstack_packet_callback_registration_t hci_event_callback_registration;

// support for multiple clients
typedef struct {
    char name;
    int le_notification_enabled;
    int  counter;
    char test_data[200];
    int  test_data_len;
    uint32_t test_data_sent;
    uint32_t test_data_start;
} le_streamer_connection_t;

static le_streamer_connection_t le_streamer_connection;

#define MAX_DEVICES 20
enum DEVICE_STATE { REMOTE_NAME_REQUEST, REMOTE_NAME_INQUIRED, REMOTE_NAME_FETCHED };
struct device {
    bd_addr_t          address;
    uint8_t            pageScanRepetitionMode;
    uint16_t           clockOffset;
    enum DEVICE_STATE  state; 
};
#define INQUIRY_INTERVAL 5
struct device devices[MAX_DEVICES];
int deviceCount = 0;

// We're looking for a remote device that lists this service in the advertisement
// LightBlue assigns 0x1111 as the UUID for a Blank service.
#define REMOTE_SERVICE 0x1111

static uint16_t  rfcomm_channel_id;
static uint8_t   spp_service_buffer[150];
static int       le_notification_enabled;
static hci_con_handle_t att_con_handle;

// THE Couner
static btstack_timer_source_t heartbeat;
static int  counter = 0;
static char counter_string[30];
static int  counter_string_len;
static hci_con_handle_t connection_handle;

static btstack_packet_callback_registration_t hci_event_callback_registration;

/*
 * @section Advertisements 
 *
 * @text The Flags attribute in the Advertisement Data indicates if a device is in dual-mode or not.
 * Flag 0x06 indicates LE General Discoverable, BR/EDR not supported although we're actually using BR/EDR.
 * In the past, there have been problems with Anrdoid devices when the flag was not set.
 * Setting it should prevent the remote implementation to try to use GATT over LE/EDR, which is not 
 * implemented by BTstack. So, setting the flag seems like the safer choice (while it's technically incorrect).
 */
/* LISTING_START(advertisements): Advertisement data: Flag 0x06 indicates LE-only device */
const uint8_t adv_data[] = {
    // Flags general discoverable, BR/EDR not supported
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, 0x06, 
    // Name
    0x0b, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'L', 'E', ' ', 'C', 'o', 'u', 'n', 't', 'e', 'r', 
    // Incomplete List of 16-bit Service Class UUIDs -- FF10 - only valid for testing!
    0x03, BLUETOOTH_DATA_TYPE_INCOMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, 0x10, 0xff,
};
/* LISTING_END */
uint8_t adv_data_len = sizeof(adv_data);

static void start_scan(void){
    log_always("Starting inquiry scan..\n");
    gap_inquiry_start(INQUIRY_INTERVAL);
}

static int getDeviceIndexForAddress( bd_addr_t addr){
    int j;
    for (j=0; j< deviceCount; j++){
        if (bd_addr_cmp(addr, devices[j].address) == 0){
            return j;
        }
    }
    return -1;
}

static int has_more_remote_name_requests(void){
    int i;
    for (i=0;i<deviceCount;i++) {
        if (devices[i].state == REMOTE_NAME_REQUEST) return 1;
    }
    return 0;
}

static void do_next_remote_name_request(void){
    int i;
    for (i=0;i<deviceCount;i++) {
        // remote name request
        if (devices[i].state == REMOTE_NAME_REQUEST){
            devices[i].state = REMOTE_NAME_INQUIRED;
            printf("Get remote name of %s...\n", bd_addr_to_str(devices[i].address));
            gap_remote_name_request( devices[i].address, devices[i].pageScanRepetitionMode,  devices[i].clockOffset | 0x8000);
            return;
        }
    }
}

static void continue_remote_names(void){
    if (has_more_remote_name_requests()){
        do_next_remote_name_request();
        return;
    } 
    start_scan();
}

static void handle_gatt_client_event(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    uint16_t mtu;
    switch(state){
        case TC_W4_SERVICE_RESULT:
            switch(hci_event_packet_get_type(packet)){
                case GATT_EVENT_SERVICE_QUERY_RESULT:
                    // store service (we expect only one)
                    gatt_event_service_query_result_get_service(packet, &le_streamer_service);
                    break;
                case GATT_EVENT_QUERY_COMPLETE:
                    if (packet[4] != 0){
                        log_always("SERVICE_QUERY_RESULT - Error status %x.\n", packet[4]);
                        gap_disconnect(connection_handle);
                        break;  
                    } 
                    // service query complete, look for characteristic
                    state = TC_W4_CHARACTERISTIC_RX_RESULT;
                    log_always("Search for LE Streamer RX characteristic.\n");
                    gatt_client_discover_characteristics_for_service_by_uuid128(handle_gatt_client_event, connection_handle, &le_streamer_service, le_streamer_characteristic_rx_uuid);
                    break;
                default:
                    break;
            }
            break;
            
        case TC_W4_CHARACTERISTIC_RX_RESULT:
            switch(hci_event_packet_get_type(packet)){
                case GATT_EVENT_CHARACTERISTIC_QUERY_RESULT:
                    gatt_event_characteristic_query_result_get_characteristic(packet, &le_streamer_characteristic_rx);
                    break;
                case GATT_EVENT_QUERY_COMPLETE:
                    if (packet[4] != 0){
                        log_always("CHARACTERISTIC_QUERY_RESULT - Error status %x.\n", packet[4]);
                        gap_disconnect(connection_handle);
                        break;  
                    } 
                    // rx characteristiic found, look for tx characteristic
                    state = TC_W4_CHARACTERISTIC_TX_RESULT;
                    log_always("Search for LE Streamer TX characteristic.\n");
                    gatt_client_discover_characteristics_for_service_by_uuid128(handle_gatt_client_event, connection_handle, &le_streamer_service, le_streamer_characteristic_tx_uuid);
                    break;
                default:
                    break;
            }
            break;

        case TC_W4_CHARACTERISTIC_TX_RESULT:
            switch(hci_event_packet_get_type(packet)){
                case GATT_EVENT_CHARACTERISTIC_QUERY_RESULT:
                    gatt_event_characteristic_query_result_get_characteristic(packet, &le_streamer_characteristic_tx);
                    break;
                case GATT_EVENT_QUERY_COMPLETE:
                    if (packet[4] != 0){
                        log_always("CHARACTERISTIC_QUERY_RESULT - Error status %x.\n", packet[4]);
                        gap_disconnect(connection_handle);
                        break;  
                    } 
                    // register handler for notifications
                    listener_registered = 1;
                    gatt_client_listen_for_characteristic_value_updates(&notification_listener, handle_gatt_client_event, connection_handle, &le_streamer_characteristic_tx);
                    // setup tracking
                    le_streamer_connection.name = 'A';
                    le_streamer_connection.test_data_len = ATT_DEFAULT_MTU - 3;
                    //test_reset(&le_streamer_connection);
                    gatt_client_get_mtu(connection_handle, &mtu);
                    le_streamer_connection.test_data_len = btstack_min(mtu - 3, sizeof(le_streamer_connection.test_data));
                    log_always("%c: ATT MTU = %u => use test data of len %u\n", le_streamer_connection.name, mtu, le_streamer_connection.test_data_len);
                    // enable notifications
#if (TEST_MODE & TEST_MODE_ENABLE_NOTIFICATIONS)
                    log_always("Start streaming - enable notify on test characteristic.\n");
                    state = TC_W4_ENABLE_NOTIFICATIONS_COMPLETE;
                    gatt_client_write_client_characteristic_configuration(handle_gatt_client_event, connection_handle,
                        &le_streamer_characteristic_tx, GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION);
                    break;
#endif
                    state = TC_W4_TEST_DATA;
#if (TEST_MODE & TEST_MODE_WRITE_WITHOUT_RESPONSE)
                    log_always("Start streaming - request can send now.\n");
                    gatt_client_request_can_write_without_response_event(handle_gatt_client_event, connection_handle);
#endif
                    break;
                default:
                    break;
            }
            break;
			
        case TC_W4_ENABLE_NOTIFICATIONS_COMPLETE:
            switch(hci_event_packet_get_type(packet)){
                case GATT_EVENT_QUERY_COMPLETE:
                    log_always("Notifications enabled, status %02x\n", gatt_event_query_complete_get_status(packet));
                    if ( gatt_event_query_complete_get_status(packet)) break;
                    state = TC_W4_TEST_DATA;
					listener_registered = 1;

					/*log_always("*** sending abc 3 *****");
					counter_string[0] = 'a';
					counter_string[0] = 'b';
					counter_string[0] = 'c';
					counter_string_len = 3;
                    //att_server_notify(att_con_handle, ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE, (uint8_t*) counter_string, counter_string_len);
					
					// send
    				uint8_t status = gatt_client_write_value_of_characteristic_without_response(connection_handle, le_streamer_characteristic_rx.value_handle, 3, (uint8_t*) counter_string);
    				if (status){
        				log_always("error %02x for write without response!\n", status);
        				return;
    				}

    				// request again
				    gatt_client_request_can_write_without_response_event(handle_gatt_client_event, connection_handle);
					*/
					
#if (TEST_MODE & TEST_MODE_WRITE_WITHOUT_RESPONSE)
                    log_always("Start streaming - request can send now.\n");
                    gatt_client_request_can_write_without_response_event(handle_gatt_client_event, connection_handle);
#endif
                    break;
                default:
                    break;
            }
            break;

        case TC_W4_TEST_DATA:
            switch(hci_event_packet_get_type(packet)){
                case GATT_EVENT_NOTIFICATION:
                    //test_track_data(&le_streamer_connection, gatt_event_notification_get_value_length(packet));
					/*log_always("*** sending abc 1 *****");
					counter_string[0] = 'a';
					counter_string[0] = 'b';
					counter_string[0] = 'c';
					counter_string_len = 3;
                    att_server_notify(att_con_handle, ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE, (uint8_t*) counter_string, counter_string_len);*/
                    break;
                case GATT_EVENT_QUERY_COMPLETE:
                    break;
                case GATT_EVENT_CAN_WRITE_WITHOUT_RESPONSE:

					/*log_always("*** sending abc 3 *****");
					counter_string[0] = 'a';
					counter_string[1] = 'b';
					counter_string[2] = 'c';
					counter_string_len = 3;
                    //att_server_notify(att_con_handle, ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE, (uint8_t*) counter_string, counter_string_len);
					
					// send
    				uint8_t status = gatt_client_write_value_of_characteristic_without_response(connection_handle, le_streamer_characteristic_rx.value_handle, 3, (uint8_t*) counter_string);
    				if (status){
        				log_always("error %02x for write without response!\n", status);
        				return;
    				}

    				// request again
				    gatt_client_request_can_write_without_response_event(handle_gatt_client_event, connection_handle);*/
                    //streamer(&le_streamer_connection);
					/*log_always("*** sending abc 2 *****");
					counter_string[0] = 'a';
					counter_string[0] = 'b';
					counter_string[0] = 'c';
					counter_string_len = 3;
                    att_server_notify(att_con_handle, ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE, (uint8_t*) counter_string, counter_string_len);*/
                    break;
                default:
                    log_always("Unknown packet type %x\n", hci_event_packet_get_type(packet));
                    break;
            }
            break;

        default:
            log_always("error\n");
            break;
    }
    
}
/* 
 * @section Packet Handler
 * 
 * @text The packet handler of the combined example is just the combination of the individual packet handlers.
 */

static void packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(channel);

	bd_addr_t addr;
    bd_addr_t event_addr;
    uint8_t   rfcomm_channel_nr;
    uint16_t  mtu;
    int i;
	int index;


	switch (packet_type) {
		case HCI_EVENT_PACKET:
			switch (hci_event_packet_get_type(packet)) {
#ifdef BLUETOOTH_MASTER
				case BTSTACK_EVENT_STATE:
            		// BTstack activated, get started
		            if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING){
        		        log_always("Start scaning!\n");
            		    gap_set_scan_parameters(1,0x0030, 0x0030);
                		gap_start_scan(); 
					    
						//inquiry
						//start_scan();
            		}
            	break;
        		case GAP_EVENT_ADVERTISING_REPORT:{
            		bd_addr_t address;
            		gap_event_advertising_report_get_address(packet, address);
            		uint8_t address_type = gap_event_advertising_report_get_address_type(packet);
            		uint8_t length = gap_event_advertising_report_get_data_length(packet);
            		const uint8_t * data = gap_event_advertising_report_get_data(packet);
            		log_always("Advertisement event: addr-type %u, addr %s, data[%u] ", address_type, bd_addr_to_str(address), length);
            		log_info_hexdump(data, length);
            		//if (!ad_data_contains_uuid16(length, (uint8_t *) data, REMOTE_SERVICE)) break;
            		//log_always("Found remote with UUID %04x, connecting...\n", REMOTE_SERVICE);
					gap_remote_name_request( address, gap_event_inquiry_result_get_page_scan_repetition_mode(packet),  gap_event_inquiry_result_get_clock_offset(packet) | 0x8000);
            		if (address[0] != 0x98 || address[1] != 0x07) break;
            		log_always("Found device\r\n");
            		gap_stop_scan();
            		gap_connect(address,address_type);
					state = TC_W4_CONNECT;
            		break;
				}

// 98:07:2d:38:F3:80												
				/*case GAP_EVENT_INQUIRY_RESULT:
                    if (deviceCount >= MAX_DEVICES) break;  // already full
                    gap_event_inquiry_result_get_bd_addr(packet, addr);
                    index = getDeviceIndexForAddress(addr);
                    if (index >= 0) break;   // already in our list

                    memcpy(devices[deviceCount].address, addr, 6);
                    devices[deviceCount].pageScanRepetitionMode = gap_event_inquiry_result_get_page_scan_repetition_mode(packet);
                    devices[deviceCount].clockOffset = gap_event_inquiry_result_get_clock_offset(packet);
                    // print info
                    log_always("Device found: %s ",  bd_addr_to_str(addr));
                    log_always("with COD: 0x%06x, ", (unsigned int) gap_event_inquiry_result_get_class_of_device(packet));
                    log_always("pageScan %d, ",      devices[deviceCount].pageScanRepetitionMode);
                    log_always("clock offset 0x%04x",devices[deviceCount].clockOffset);
                    if (gap_event_inquiry_result_get_rssi_available(packet)){
                        log_always(", rssi %d dBm", (int8_t) gap_event_inquiry_result_get_rssi(packet));
                    }
                    if (gap_event_inquiry_result_get_name_available(packet)){
                        char name_buffer[240];
                        int name_len = gap_event_inquiry_result_get_name_len(packet);
                        memcpy(name_buffer, gap_event_inquiry_result_get_name(packet), name_len);
                        name_buffer[name_len] = 0;
                        log_always(", name '%s'", name_buffer);
                        devices[deviceCount].state = REMOTE_NAME_FETCHED;;
                    } else {
                        devices[deviceCount].state = REMOTE_NAME_REQUEST;
                    }
                    deviceCount++;
                    break;

                case GAP_EVENT_INQUIRY_COMPLETE:
                    for (i=0;i<deviceCount;i++) {
                        // retry remote name request
                        if (devices[i].state == REMOTE_NAME_INQUIRED)
                            devices[i].state = REMOTE_NAME_REQUEST;
                    }
                    continue_remote_names();
                    break;*/
#endif
                case HCI_EVENT_PIN_CODE_REQUEST:
                    // inform about pin code request
                    log_always("Pin code request - using '0000'\n");
                    hci_event_pin_code_request_get_bd_addr(packet, event_addr);
                    gap_pin_code_response(event_addr, "0000");
                    break;

                case HCI_EVENT_USER_CONFIRMATION_REQUEST:
                    // inform about user confirmation request
                    log_always("SSP User Confirmation Request with numeric value %d\n", little_endian_read_32(packet, 8));
                    log_always("SSP User Confirmation Auto accept\n");
                    break;

				case HCI_EVENT_LE_META:
            		// wait for connection complete
            		if (hci_event_le_meta_get_subevent_code(packet) !=  HCI_SUBEVENT_LE_CONNECTION_COMPLETE) break;
					log_always("------------------------ Connected ------------------------------");
					if (state != TC_W4_CONNECT) return;
					state = TC_W4_SERVICE_RESULT;
            		connection_handle = hci_subevent_le_connection_complete_get_connection_handle(packet);
					log_always("discovering handle %d", connection_handle);
		            gatt_client_discover_primary_services_by_uuid128(handle_gatt_client_event, connection_handle, le_streamer_service_uuid);
           	 		break;

                case HCI_EVENT_DISCONNECTION_COMPLETE:
					log_always("------------------------ Disconnected ------------------------");
					if (listener_registered){
                		listener_registered = 0;
                		gatt_client_stop_listening_for_characteristic_value_updates(&notification_listener);
            		}
					// re-enable page/inquiry scan again
                    gap_discoverable_control(1);
                    gap_connectable_control(1);
                    // re-enable advertisements
                    gap_advertisements_enable(1);
                    le_notification_enabled = 0;
                    break;

                case ATT_EVENT_CAN_SEND_NOW:
					log_always("------------------------ Connected ------------------------");
					/*log_always("sending abc 3");
					counter_string[0] = 'a';
					counter_string[0] = 'b';
					counter_string[0] = 'c';
					counter_string_len = 3;
                    att_server_notify(att_con_handle, ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE, (uint8_t*) counter_string, counter_string_len);*/
                    break;

                case RFCOMM_EVENT_INCOMING_CONNECTION:
					// data: event (8), len(8), address(48), channel (8), rfcomm_cid (16)
                    rfcomm_event_incoming_connection_get_bd_addr(packet, event_addr); 
                    rfcomm_channel_nr = rfcomm_event_incoming_connection_get_server_channel(packet);
                    rfcomm_channel_id = rfcomm_event_incoming_connection_get_rfcomm_cid(packet);
                    log_always("RFCOMM channel %u requested for %s\n", rfcomm_channel_nr, bd_addr_to_str(event_addr));
                    rfcomm_accept_connection(rfcomm_channel_id);
					break;
					
				case RFCOMM_EVENT_CHANNEL_OPENED:
					// data: event(8), len(8), status (8), address (48), server channel(8), rfcomm_cid(16), max frame size(16)
					if (rfcomm_event_channel_opened_get_status(packet)) {
                        log_always("RFCOMM channel open failed, status %u\n", rfcomm_event_channel_opened_get_status(packet));
                    } else {
                        rfcomm_channel_id = rfcomm_event_channel_opened_get_rfcomm_cid(packet);
                        mtu = rfcomm_event_channel_opened_get_max_frame_size(packet);
                        log_always("RFCOMM channel open succeeded. New RFCOMM Channel ID %u, max frame size %u\n", rfcomm_channel_id, mtu);
						// disable page/inquiry scan to get max performance
                        gap_discoverable_control(0);
                        gap_connectable_control(0);
                        // disable advertisements
                        gap_advertisements_enable(0);
                    }
					break;

                case RFCOMM_EVENT_CAN_SEND_NOW:
                    rfcomm_send(rfcomm_channel_id, (uint8_t*) counter_string, counter_string_len);
                    break;

                case RFCOMM_EVENT_CHANNEL_CLOSED:
                    log_always("RFCOMM channel closed\n");
                    rfcomm_channel_id = 0;
                    break;
                
                default:
                    break;
			}
            break;
                        
        case RFCOMM_DATA_PACKET:
            log_info("RCV: '");
            //for (i=0;i<size;i++){
            //    putchar(packet[i]);
            //}
            log_info("\n");
            break;

        default:
            break;
	}
}

// ATT Client Read Callback for Dynamic Data
// - if buffer == NULL, don't copy data, just return size of value
// - if buffer != NULL, copy data and return number bytes copied
// @param offset defines start of attribute value
static uint16_t att_read_callback(hci_con_handle_t con_handle, uint16_t att_handle, uint16_t offset, uint8_t * buffer, uint16_t buffer_size){
	log_always("btmain: att read cb");
    UNUSED(con_handle);

    if (att_handle == ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE){
        return att_read_callback_handle_blob((const uint8_t *)counter_string, buffer_size, offset, buffer, buffer_size);
    }
    return 0;
}

// write requests
static int att_write_callback(hci_con_handle_t con_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size){
    // ignore cancel sent for new connections
    if (transaction_mode == ATT_TRANSACTION_MODE_CANCEL) return 0;
    // find characteristic for handle
    switch (att_handle){
        case ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_CLIENT_CONFIGURATION_HANDLE:
            le_notification_enabled = little_endian_read_16(buffer, 0) == GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;
            att_con_handle = con_handle;
            return 0;
        case ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
            log_always("Write on test characteristic: ");
            log_hexdump(HCI_DUMP_LOG_LEVEL_ALWAYS, buffer, buffer_size);
            return 0;
        default:
            log_always("WRITE Callback, handle %04x, mode %u, offset %u, data: ", con_handle, transaction_mode, offset);
            log_hexdump(HCI_DUMP_LOG_LEVEL_ALWAYS, buffer, buffer_size);
            return 0;
    }
}

static void beat(void){
    counter++;
	log_info("beat: %d\r\n", counter);
    //counter_string_len = log_info("BTstack counter %04u", counter);
    //puts(counter_string);
}

/*
 * @section Heartbeat Handler
 * 
 * @text Similar to the packet handler, the heartbeat handler is the combination of the individual ones.
 * After updating the counter, it requests an ATT_EVENT_CAN_SEND_NOW and/or RFCOMM_EVENT_CAN_SEND_NOW
 */

 /* LISTING_START(heartbeat): Combined Heartbeat handler */
static void heartbeat_handler(struct btstack_timer_source *ts){

    if (rfcomm_channel_id || le_notification_enabled) {
        beat();
    }

    if (rfcomm_channel_id){
        rfcomm_request_can_send_now_event(rfcomm_channel_id);
    }

    if (le_notification_enabled) {
        att_server_request_can_send_now_event(att_con_handle);
    }

    btstack_run_loop_set_timer(ts, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(ts);
} 
/* LISTING_END */

/*
 * @section Main Application Setup
 *
 * @text As with the packet and the heartbeat handlers, the combined app setup contains the code from the individual example setups.
 */

/* LISTING_START(MainConfiguration): Init L2CAP RFCOMM SDO SM ATT Server and start heartbeat timer */
int btstack_main(void);
int btstack_main(void)
{
    l2cap_init();

    rfcomm_init();
    rfcomm_register_service(packet_handler, RFCOMM_SERVER_CHANNEL, 0xffff);

    // init SDP, create record for SPP and register with SDP
    sdp_init();
    memset(spp_service_buffer, 0, sizeof(spp_service_buffer));
    spp_create_sdp_record(spp_service_buffer, 0x10001, RFCOMM_SERVER_CHANNEL, "SPP Counter");
    sdp_register_service(spp_service_buffer);
//    log_info("SDP service record size: %u\n", de_get_len(spp_service_buffer));

    gap_set_local_name("test and LE Counter 00:00:00:00:00:00");
    gap_ssp_set_io_capability(SSP_IO_CAPABILITY_DISPLAY_YES_NO);
    gap_discoverable_control(1);

    // setup le device db
    le_device_db_init();

    // setup SM: Display only
    sm_init();

	gatt_client_init();

    // setup ATT server
    att_server_init(profile_data, att_read_callback, att_write_callback);    

    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // register for ATT events
    att_server_register_packet_handler(packet_handler);

    // setup advertisements
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
    gap_advertisements_set_data(adv_data_len, (uint8_t*) adv_data);
#ifndef BLUETOOTH_MASTER
    gap_advertisements_enable(1);
#endif

    // set one-shot timer
    heartbeat.process = &heartbeat_handler;
    btstack_run_loop_set_timer(&heartbeat, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(&heartbeat);

    // beat once
    beat();

    // turn on!
	hci_power_control(HCI_POWER_ON);

	log_always("Initialized.");
	    
    return 0;
}

void sendDataPacket(){
	if (listener_registered == 0){
		log_always("X");
	   	return;
	}
	static char last = 'A';
	log_always("*** data packet *****");
	if (last > 'z') last = 'A';
					counter_string[0] = last++;
					counter_string[1] = last++;
					counter_string[2] = last++;
					counter_string_len = 3;
					
					// send
    				uint8_t status = gatt_client_write_value_of_characteristic_without_response(connection_handle, le_streamer_characteristic_rx.value_handle, 3, (uint8_t*) counter_string);
    				if (status){
        				log_always("error %02x for write without response!\n", status);
        				return;
    				}

    				// request again
				    gatt_client_request_can_write_without_response_event(handle_gatt_client_event, connection_handle);
}
/* LISTING_END */
/* EXAMPLE_END */

