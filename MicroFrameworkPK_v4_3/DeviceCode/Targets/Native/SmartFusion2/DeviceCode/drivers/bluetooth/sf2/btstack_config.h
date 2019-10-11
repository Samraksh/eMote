// Maxim Max32630FTHR Port
//
#ifndef __BTSTACK_CONFIG
#define __BTSTACK_CONFIG
#include <stdint.h>

// Port related features
#define HAVE_INIT_SCRIPT
#define HAVE_EMBEDDED_TIME_MS

// BTstack features that can be enabled
#define ENABLE_BLE
#define ENABLE_CLASSIC
#define ENABLE_LE_PERIPHERAL

// must be uncommented to be a master
//#define ENABLE_LE_CENTRAL

#define ENABLE_LE_DATA_CHANNELS
//#define ENABLE_LOG_ERROR
//#define ENABLE_LOG_INFO
//#define ENABLE_LOG_DEBUG
#define ENABLE_LE_SECURE_CONNECTIONS
#define ENABLE_MICRO_ECC_FOR_LE_SECURE_CONNECTIONS
#define ENABLE_GATT_CLIENT_PAIRING
//#define ENABLE_SCO_OVER_HCI

// BTstack configuration. buffers, sizes, ...
#define HCI_ACL_PAYLOAD_SIZE 550 // max windows can send is 532
#define MAX_NR_GATT_CLIENTS 1
#define MAX_NR_HCI_CONNECTIONS 1
#define MAX_NR_L2CAP_SERVICES  3
#define MAX_NR_L2CAP_CHANNELS  3
#define MAX_NR_RFCOMM_MULTIPLEXERS 1
#define MAX_NR_RFCOMM_SERVICES 1
#define MAX_NR_RFCOMM_CHANNELS 1
#define MAX_NR_BTSTACK_LINK_KEY_DB_MEMORY_ENTRIES  2
//#define MAX_NR_BNEP_SERVICES 1
//#define MAX_NR_BNEP_CHANNELS 1
//#define MAX_NR_HFP_CONNECTIONS 1
#define MAX_NR_WHITELIST_ENTRIES 1
#define MAX_NR_SM_LOOKUP_ENTRIES 3
#define MAX_NR_SERVICE_RECORD_ITEMS 1
#define MAX_NR_AVDTP_STREAM_ENDPOINTS 1
#define MAX_NR_AVDTP_CONNECTIONS 1
#define MAX_NR_AVRCP_CONNECTIONS 1

// Link Key DB and LE Device DB using TLV on top of Flash Sector interface
#define NVM_NUM_LINK_KEYS 8
#define NVM_NUM_DEVICE_DB_ENTRIES 8
#define NVN_NUM_GATT_SERVER_CCC 3

#endif
