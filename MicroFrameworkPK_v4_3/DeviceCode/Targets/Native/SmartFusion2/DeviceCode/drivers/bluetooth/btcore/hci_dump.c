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

#define __BTSTACK_FILE__ "hci_dump.c"

/*
 *  hci_dump.c
 *
 *  Dump HCI trace in various formats:
 *
 *  - BlueZ's hcidump format
 *  - Apple's PacketLogger
 *  - stdout hexdump
 *
 */

#include "..\c_code_calling_cpp.h"
#include "..\sf2\btstack_config.h"

// enable POSIX functions (needed for -std=c99)
//#define _POSIX_C_SOURCE 200809

#include "hci_dump.h"
#include "hci.h"
#include "hci_transport.h"
#include "hci_cmd.h"
#include "btstack_run_loop.h"



#ifdef ENABLE_SEGGER_RTT
#include "SEGGER_RTT.h"
static char channel1_out[1024];
#endif

// BLUEZ hcidump - struct not used directly, but left here as documentation
typedef struct {
    uint16_t    len;
    uint8_t     in;
    uint8_t     pad;
    uint32_t    ts_sec;
    uint32_t    ts_usec;
    uint8_t     packet_type;
}
hcidump_hdr;
#define HCIDUMP_HDR_SIZE 13

// APPLE PacketLogger - struct not used directly, but left here as documentation
typedef struct {
    uint32_t    len;
    uint32_t    ts_sec;
    uint32_t    ts_usec;
    uint8_t     type;   // 0xfc for note
}
pktlog_hdr;
#define PKTLOG_HDR_SIZE 13

static int dump_file = -1;
static int dump_format;
static union {
    uint8_t header_bluez[HCIDUMP_HDR_SIZE];
    uint8_t header_packetlogger[PKTLOG_HDR_SIZE];
} header;
#ifdef HAVE_POSIX_FILE_IO
static char time_string[40];
static int  max_nr_packets = -1;
static int  nr_packets = 0;
#endif

#if defined(HAVE_POSIX_FILE_IO) || defined (ENABLE_SEGGER_RTT)
static char log_message_buffer[256];
#endif

// levels: debug, info, error
static int log_level_enabled[3] = { 1, 1, 1};

void hci_dump_open(const char *filename, hci_dump_format_t format){

/*    dump_format = format;

#ifdef HAVE_POSIX_FILE_IO
    if (dump_format == HCI_DUMP_STDOUT) {
        dump_file = fileno(stdout);
    } else {

        int oflags = O_WRONLY | O_CREAT | O_TRUNC;
#ifdef _WIN32
        oflags |= O_BINARY;
#endif
        dump_file = open(filename, oflags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
        if (dump_file < 0){
            printf("hci_dump_open: failed to open file %s\n", filename);
        }
    }
#else

    UNUSED(filename);

#ifdef ENABLE_SEGGER_RTT
    switch (dump_format){
        case HCI_DUMP_PACKETLOGGER:
        case HCI_DUMP_BLUEZ:
            // Configure up channel 1, options:
            // - SEGGER_RTT_MODE_NO_BLOCK_SKIP
            // - SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL
            // Note: with SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL, firwmware will hang if RTT not supported/debug probe not connected
            // Note: with SEGGER_RTT_MODE_NO_BLOCK_SKIP, there's a chance for log file corruption if second write (packet) is skipped
            SEGGER_RTT_ConfigUpBuffer(1, "hci_dump", &channel1_out[0], sizeof(channel1_out), SEGGER_RTT_MODE_NO_BLOCK_SKIP) ;;
            break;
        default:
            break;
    }
#endif
    
    dump_file = 1;
#endif*/
}

#ifdef HAVE_POSIX_FILE_IO
void hci_dump_set_max_packets(int packets){
    max_nr_packets = packets;
}
#endif

static void hci_dump_packetlogger_setup_header(uint8_t * buffer, uint32_t tv_sec, uint32_t tv_us, uint8_t packet_type, uint8_t in, uint16_t len){
    big_endian_store_32( buffer, 0, PKTLOG_HDR_SIZE - 4 + len);
    big_endian_store_32( buffer, 4, tv_sec);
    big_endian_store_32( buffer, 8, tv_us);
    uint8_t packet_logger_type = 0;
    switch (packet_type){
        case HCI_COMMAND_DATA_PACKET:
            packet_logger_type = 0x00;
            break;
        case HCI_ACL_DATA_PACKET:
            packet_logger_type = in ? 0x03 : 0x02;
            break;
        case HCI_SCO_DATA_PACKET:
            packet_logger_type = in ? 0x09 : 0x08;
            break;
        case HCI_EVENT_PACKET:
            packet_logger_type = 0x01;
            break;
        case LOG_MESSAGE_PACKET:
            packet_logger_type = 0xfc;
            break;
        default:
            return;
    }
    buffer[12] = packet_logger_type;
}

static void hci_dump_bluez_setup_header(uint8_t * buffer, uint32_t tv_sec, uint32_t tv_us, uint8_t packet_type, uint8_t in, uint16_t len){
    little_endian_store_16( buffer, 0, 1 + len);
    buffer[2] = in;
    buffer[3] = 0;
    little_endian_store_32( buffer, 4, tv_sec);
    little_endian_store_32( buffer, 8, tv_us);
    buffer[12] = packet_type;
}

static void printf_packet(uint8_t packet_type, uint8_t in, uint8_t * packet, uint16_t len){
 /*   switch (packet_type){
        case HCI_COMMAND_DATA_PACKET:
            printf("CMD => ");
            break;
        case HCI_EVENT_PACKET:
            printf("EVT <= ");
            break;
        case HCI_ACL_DATA_PACKET:
            if (in) {
                printf("ACL <= ");
            } else {
                printf("ACL => ");
            }
            break;
        case HCI_SCO_DATA_PACKET:
            if (in) {
                printf("SCO <= ");
            } else {
                printf("SCO => ");
            }
            break;
        case LOG_MESSAGE_PACKET:
            printf("LOG -- %s\n", (char*) packet);
            return;
        default:
            return;
    }
    printf_hexdump(packet, len);  */
}

static void printf_timestamp(void){

}

void hci_dump_packet(uint8_t packet_type, uint8_t in, uint8_t *packet, uint16_t len) {    

}

static int hci_dump_log_level_active(int log_level){
    if (log_level < HCI_DUMP_LOG_LEVEL_DEBUG) return 0;
    if (log_level > HCI_DUMP_LOG_LEVEL_ERROR) return 0;
    return log_level_enabled[log_level];
}

void hci_dump_log_va_arg(int log_level, const char * format, va_list argptr){

}

void hci_dump_log(int log_level, const char * format, ...){
    va_list argptr;
    va_start(argptr, format);
    debugBT_printf( format, argptr);
    va_end(argptr);
}

#ifdef __AVR__
void hci_dump_log_P(int log_level, PGM_P format, ...){

}
#endif

void hci_dump_close(void){
#ifdef HAVE_POSIX_FILE_IO
    close(dump_file);
#endif
    dump_file = -1;
}

void hci_dump_enable_log_level(int log_level, int enable){
    if (log_level < HCI_DUMP_LOG_LEVEL_DEBUG) return;
    if (log_level > HCI_DUMP_LOG_LEVEL_ERROR) return;
    log_level_enabled[log_level] = enable;
}

