/*--------- Includes ----------- */

#include <tinyhal.h>
#include <cmsis/m2sxxx.h>
#include <drivers/mss_spi/mss_spi.h>
#include <sys_services/SysServices.h>
#include <crypto.h>
#include "../microsemi_lib/CMSIS/mss_assert.h"

#define KERNEL_SIZE 349428
#define SIGSIZE 32
#define eNVMAddress 0x60000000
#define RoT_Offset 0x0F000
#define STANDARD_SERV_RESP_LENGTH                       6u
#define HMAC_KEY_LENGTH     32u
#define HMAC_REQUEST_CMD                                12u

UINT8 dKey[SIGSIZE] = {
		0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
        0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
        0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
        0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08
};

UINT8 badSig[SIGSIZE] = {
		0x35, 0x75, 0xb3, 0xf7, 0x52, 0xdb, 0xd8, 0x95,
		0x09, 0x2e, 0xfd, 0x90, 0xf5, 0xe6, 0x95, 0x7f,
		0x10, 0x91, 0xe3, 0x42, 0x46, 0x9f, 0xff, 0x5e,
		0xa6, 0xe3, 0xcf, 0xa5, 0xf4, 0x0b, 0xc3, 0x1f
};

UINT8 correctSig[SIGSIZE] = {
		0xf4, 0xf6, 0x66, 0xd0, 0x38, 0x94, 0xee, 0x4e,
		0xf9, 0x32, 0x61, 0xfd, 0x28, 0x97, 0x6a, 0x05,
		0x60, 0xc0, 0xc3, 0x45, 0x79, 0xc6, 0x44, 0x90,
		0x03, 0x46, 0x16, 0xe2, 0x83, 0x64, 0x40, 0xc0
};

UINT8* dSig=correctSig;

void PrintHex(uint8_t* data, int size){
	for (int j=0;j<size; j++){
		hal_printf("0x%.2X , ",data[j]);
	}
	hal_printf("\n");
}

static volatile uint8_t mSig[SIGSIZE];

/*==============================================================================
 *
 */

/*static void request_completion_handler_no_block
(
    uint8_t * p_response,
    uint16_t response_size
)
{
    g_request_in_progress = 0u;
    g_last_response_length = response_size;

	hal_printf("ending comblk %d\r\n", mSig);
    
    if((service_response_length == g_last_response_length) && (service_cmd_opcode == service_response[0]))
    {
        service_status = service_response[1];
    }
    else
    {
        service_status = MSS_SYS_UNEXPECTED_ERROR;
    }    
	hal_printf("\r\n ------------- signature ------------ \r\n");
	PrintHex((uint8_t*)mSig, SIGSIZE);
	hal_printf("\r\n------------------------------------- \r\n");
}

static uint8_t execute_service_no_block
(
    uint8_t cmd_opcode,
    uint8_t * cmd_params_ptr,
    uint8_t * response,
    uint16_t response_length
)
{
	hal_printf("starting comblk\r\n");
    signal_request_start();
    
	service_response_length = response_length;
	service_response = response;
	service_cmd_opcode = cmd_opcode;

    MSS_COMBLK_send_cmd_with_ptr(service_cmd_opcode,                    // cmd_opcode 
                                 (uint32_t)cmd_params_ptr,      // cmd_params_ptr 
                                 service_response,                      // p_response 
                                 service_response_length,               // response_size 
                                 request_completion_handler_no_block);   // completion_handler_no_block 

	wait_for_request_completion();

    return NULL;
}

*/


int codeIntegrityCheck(uint8_t* memory, uint32_t memorySize){
	
	memset((uint8_t*)mSig, 0, SIGSIZE);
	int status=0;
	hal_printf("mSig %d\r\n", mSig);
	//status = generate_hmac(dKey, (uint8_t*)eNVMAddress+RoT_Offset, KERNEL_SIZE, (uint8_t*)mSig);
	status = generate_hmac(dKey, (uint8_t*)eNVMAddress, KERNEL_SIZE, (uint8_t*)mSig);
	//while(1){	
		//PrintHex((uint8_t*)mSig, SIGSIZE);
	//}
	//wait_for_request_completion();

	//CRYPTO_RESULT m_res = Crypto_GetHMAC(,  , dKey, mSig, SIGSIZE);
	return 0;
}

