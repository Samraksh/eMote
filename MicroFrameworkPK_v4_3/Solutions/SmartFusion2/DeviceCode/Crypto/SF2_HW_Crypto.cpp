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

/*
//Ecc apis
uint8_t MSS_SYS_puf_fetch_ecc_public_key(uint8_t* p_puf_public_key);
//d*P=Q
uint8_t MSS_SYS_ecc_point_multiplication(
    uint8_t* p_scalar_d,
    uint8_t* p_point_p,
    uint8_t* p_point_q
);
//P+Q=R
uint8_t MSS_SYS_ecc_point_addition
(
    uint8_t* p_point_p,
    uint8_t* p_point_q,
    uint8_t* p_point_r
);
//Get base point G, which can then be used to generate public key
void MSS_SYS_ecc_get_base_point
(
    uint8_t* p_point_g
);
*/

CRYPTO_RESULT M2S_STATUS_TO_CRYPTO (uint8_t status){
	switch (status){
		case MSS_SYS_SUCCESS:
			return CRYPTO_SUCCESS;
		case MSS_SYS_UNEXPECTED_ERROR:
			return CRYPTO_UNKNOWNERROR;
		case MSS_SYS_MEM_ACCESS_ERROR:
			return CRYPTO_NOMEMORY;
		case MSS_SYS_SERVICE_NOT_LICENSED:
		case MSS_SYS_SERVICE_DISABLED_BY_FACTORY:
		case MSS_SYS_SERVICE_DISABLED_BY_USER:
		default:
			return CRYPTO_FAILURE;
	}
}

uint8_t nrbgInit(){
	if(!nrbg_init){
		SF2_MYSYS_CHECKRESULT(MSS_SYS_nrbg_reset());
		SF2_MYSYS_CHECKRESULT(MSS_SYS_nrbg_instantiate(personalStr, 16, &nrbg_handle));
		nrbg_init=true;
	}
}

uint8_t GetRandomSeed(uint8_t *buf, uint16_t length){
	return GetRandomBytes(buf, length);
}

uint8_t GetRandomBytes(uint8_t *buf, uint16_t length){

	//Initialize nrbg if not initailized
	if(!nrbg_init)	nrbgInit();
	do {
		uint16_t dataSize=0;
		if(length > 128) { dataSize=128; length -=128;} else { dataSize = length;}

		uint8_t *result = (uint8_t*) buf;

		//first generate a new nouce,
		uint8_t status = MSS_SYS_nrbg_instantiate(personalStr, 0 /*sizeof(personalization_str)*/, nrbg_handle);
		if(status != MSS_SYS_SUCCESS) return M2S_STATUS_TO_CRYPTO(status);

		//now generate random bytes using nonce
		status = MSS_SYS_nrbg_generate( result, additionalStr, dataSize, 0, 0, nrbg_handle);
		if(status != MSS_SYS_SUCCESS) return M2S_STATUS_TO_CRYPTO(status);

		result+=dataSize; ret+=dataSize;
	} while (length < 128);
	return MSS_SYS_SuCCESS;
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

//Generate ECC Public-Private Key-Pair
CRYPTO_RESULT SF2_ECC384_PKEY(sf2_ec_key_t *key){

	//SF2 uses NIST P-384 curve, which means the private key is 384 bits/48 bytes long.
	//public key is 96 bytes long

	//generate a 48 byte, random number first
	GetRandomBytes(key->privateKey, 48);


	/* ECC Key generation */
	status = MSS_SYS_ecc_point_multiplication(key->privateKey, 0, key->publicKey);
	if(MSS_SYS_SUCCESS != status)
	{
		return M2S_STATUS_TO_CRYPTO(status);
	}
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



