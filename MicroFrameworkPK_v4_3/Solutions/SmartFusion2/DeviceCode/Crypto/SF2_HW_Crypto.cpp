#include "SF2_HW_Crypto.h"
#include "tinyhal.h"

//nbrg related variables
static bool nrbg_init =0;
static uint8_t nrbg_handle=0;
static uint8_t additionalStr[8];
const uint8_t personalStr[16]= "H e y !$*)0@n/?";

//encryption related variables.
//static uint8_t g_key_256[64];
//static uint8_t g_plain_text[64];
//static uint8_t g_encrypted_text[32];
//static uint8_t g_decrypted_text[32];
//static uint8_t g_dec_input_text[64];
//static uint8_t g_hash_input_text[128];
//static uint8_t g_hmac_input_text[128];
//static uint8_t g_nb_blocks;
//static uint8_t g_hmac_code_stored[32];
//static uint8_t g_checksum_hash[32];
//static uint8_t g_aes_iv[32];



uint8_t nrbgInit(){
	if(!nrbg_init){
		SF2_MYSYS_CHECKRESULT(MSS_SYS_nrbg_reset());
		SF2_MYSYS_CHECKRESULT(MSS_SYS_nrbg_instantiate(personalStr, 16, &nrbg_handle));
		nrbg_init=true;
	}
}

int GetRandomSeed(uint8_t *buf, uint16_t length){
	return GetRandomBytes(buf, length);
}

int GetRandomBytes(uint8_t *buf, uint16_t length){
	nrbgInit();
	do {
		uint16_t dataSize=0;
		if(length > 128) { dataSize=128; length -=128;} else { dataSize = length;}

		uint8_t *result = (uint8_t*) buf;
		uint8_t status = MSS_SYS_nrbg_generate( result, additionalStr, dataSize, 0, 0, nrbg_handle);

		if(status != MSS_SYS_SUCCESS) return -1;

		result+=dataSize;
	} while (length < 128);

}


int SF2_CipherReset(sf2_cipher_context_t* ctx) {
	memset(ctx, 0, sizeof(sf2_cipher_context_t));
	return MSS_SYS_SUCCESS;
}


//https://gist.github.com/waldyrfelix/3908162

int SF2_CipherPad(sf2_cipher_context_t* ctx, uint8_t* data, int dataSize, uint8_t* result ){

}

int SF2_Cipher(sf2_cipher_context_t* ctx, uint8_t* data, int dataSize, uint8_t* result ){
	int status=0;
	uint16_t nb_blocks; //specifies the size of data as 128-bit/16-byte blocks

	// if data size is not multiple of 128-bits return error.
	//We dont do padding at this level, padding should have been already done.
	if(dataSize % 16) { return -1;}

	nb_blocks = dataSize/16;
	switch (ctx->type) {
	case AES_128:
	    // Perform 128-bit encryption.
	    status = MSS_SYS_128bit_aes(ctx->key,
	                                ctx->iv,
	                                nb_blocks,
									ctx->mode,
	                                result,
	                                data);

		break;
	case AES_256:
		status = MSS_SYS_256bit_aes(ctx->key,
			                        ctx->iv,
									nb_blocks,
									ctx->mode,
									result,
									data);

		break;
	default:
		break;
	}
	return status;
}
