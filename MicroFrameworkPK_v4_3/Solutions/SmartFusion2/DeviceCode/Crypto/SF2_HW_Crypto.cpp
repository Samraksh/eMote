#include "SF2_HW_Crypto.h"
#include "tinyhal.h"
#include <crypto.h>

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
	int ret =0;
	do {
		uint16_t dataSize=0;
		if(length > 128) { dataSize=128; length -=128;} else { dataSize = length;}

		uint8_t *result = (uint8_t*) buf;
		uint8_t status = MSS_SYS_nrbg_generate( result, additionalStr, dataSize, 0, 0, nrbg_handle);

		if(status != MSS_SYS_SUCCESS) return -1;

		result+=dataSize; ret+=dataSize;
	} while (length < 128);
	return ret;
}


int SF2_CipherReset(sf2_cipher_context_t* ctx) {
	memset(ctx, 0, sizeof(sf2_cipher_context_t));
	return MSS_SYS_SUCCESS;
}


//https://gist.github.com/waldyrfelix/3908162

int SF2_CipherPad(sf2_cipher_context_t* ctx, uint8_t* data, int dataSize, uint8_t* result ){

}

int SF2_Cipher(sf2_cipher_context_t* ctx, uint8_t* data, uint32_t dataSize, uint8_t* result, uint32_t *resultSize){
	int status=0;
	uint16_t nb_blocks; //specifies the size of data as 128-bit/16-byte blocks

	nb_blocks = dataSize/16;
	// if data size is not multiple of 128-bits return error.
	//We dont do padding at this level, padding should have been already done.
	if(dataSize % 16) {
		nb_blocks++;
	}

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
	*resultSize = nb_blocks*16;
	return status;
}


int SF2_Digest(sf2_digest_context_t* ctx, uint8_t* data, uint32_t dataSize, uint8_t* result, uint32_t *resultSize){
	int status=0;
	/*uint16_t nb_blocks; //specifies the size of data as 128-bit/16-byte blocks

	nb_blocks = dataSize/16;
	// if data size is not multiple of 128-bits return error.
	//We dont do padding at this level, padding should have been already done.
	if(dataSize % 16) {
		nb_blocks++;
	}
*/

	// SF2 supports only HMAC-SHA256 so no switch case

	status = MSS_SYS_hmac
			(   ctx->key,
				data,
				dataSize,
				result
			);

	*resultSize = 32; //SHA256 HMAC has constant 256-bit or 32byte mac
	return status;
}

////////////////////// Native Crypto APIs ////////////////////
// there are maximum BLOCK_SIZE bytes in the signature (32 bytes for AES and XTEA)
CRYPTO_RESULT Crypto_GetFingerprint(BYTE *key, BYTE *Signature, int cbSignatureSize){
	return CRYPTO_ACTIVATIONBADSYNTAX;
}

CRYPTO_RESULT Crypto_GetHMAC(BYTE *pBuffer, DWORD cbBufferSize, BYTE *key, BYTE *Signature, int cbSignatureSize){
	int status=0;

	//SF2 supports only HMAC-SHA256 so no switch case

	status = MSS_SYS_hmac
				(key,
				 pBuffer,
				 cbBufferSize,
				 Signature
			 );

	//*resultSize = 32; //SHA256 HMAC has constant 256-bit or 32byte mac
	return (CRYPTO_RESULT)status;
}

// this function generates a hash using the currently chosen hash algorithm
BOOL Crypto_GetHash(BYTE *pBuffer, DWORD cbBufferSize, BYTE *pHash, DWORD cbHashSize){

	return FALSE;
}

//BOOL Crypto_GetUniqueInfoFromKey(WatchUniqueInfo *pWatchInfo, SymmetricKey *pKey);

// this function returns a string encoding the watch ID; pString must be at least ACTIVATION_KEY_SIZE
// it computes the activation string from a received watch seed
BOOL Crypto_GetActivationStringFromSeed(char *pString, int cbStringSize, KeySeed *Seed, UINT16 region, UINT16 model){
	return FALSE;
}

// Encrypts a buffer using a symmetric algorithm.
BOOL Crypto_Encrypt(BYTE *Key, BYTE *IV, DWORD cbIVSize, BYTE* pPlainText, DWORD cbPlainText, BYTE *pCypherText, DWORD cbCypherText){
	return FALSE;
}

// Decrypts a buffer using a symmetric algorithm
BOOL Crypto_Decrypt(BYTE *Key, BYTE *IV, DWORD cbIVSize, BYTE *pCypherText, DWORD cbCypherText, BYTE* pPlainText, DWORD cbPlainText){
	return FALSE;
}



