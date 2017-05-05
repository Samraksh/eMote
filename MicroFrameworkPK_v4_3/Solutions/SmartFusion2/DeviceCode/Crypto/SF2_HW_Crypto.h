#ifndef _SF2_HW_CRYPTO_H_
#define _SF2_HW_CRYPTO_H_

#include <drivers/mss_sys_services/mss_sys_services.h>

#define SF2_HW_MAX_IV_LENGTH 16
#define SF2_HW_MAX_KEY_LENGTH 32 // This is in bytes

#define SF2_MYSYS_SET_AND_LEAVE(x) \
	return x

#define SF2_MYSYS_CHECKRESULT(x) \
    if((x) != MSS_SYS_SUCCESS) SF2_MYSYS_SET_AND_LEAVE(x)



enum SF2_CipherType{
	AES_128,
	AES_256
};

enum SF2_CipherOperation {
	ENCRYPT,
	DECRYPT
};

#define DECRYPT_MASK 0x80
#define ECB_MASK 0x00
#define CBC_MASK 0x01
#define OFB_MASK 0x02
#define CTR_MASK 0x03
/*enum SF2_CipherMode {
	ECB, CBC, OFB, CTR
};
*/

///Please note this is defined by Samraksh. Should really use sf2 defintion if available.
typedef struct {
	SF2_CipherType type;
	SF2_CipherOperation operation;
	uint8_t mode;
    /** Buffer for data that hasn't been encrypted yet */
    //unsigned char unprocessed_data[MBEDTLS_MAX_BLOCK_LENGTH];

    /** Number of bytes that still need processing */
    //size_t unprocessed_len;

    /** Current IV or NONCE_COUNTER for CTR-mode */
    unsigned char iv[SF2_HW_MAX_IV_LENGTH];

    int key_bitlen; // Key length to use
    unsigned char key[SF2_HW_MAX_KEY_LENGTH];

    // IV size in bytes (for ciphers with variable-length IVs)
    int iv_size;
    // Cipher-specific context
    //void *cipher_ctx;

} sf2_cipher_context_t;


//function prototypes

///returns a random seed of size length, using FPGA random functions
int GetRandomSeed(uint8_t *buf, uint16_t length);

///returns a random byte stream of size length, using FPGA random functions
int GetRandomBytes(uint8_t *buf, uint16_t length);

/// Reset the context of the cipher
int SF2_CipherReset(sf2_cipher_context_t* ctx);

///main wrapper function for all symmetric cipher methods. Ctx all relevant information for cipher,
///data points to array to be encrypted/decrpted
///dataSize is size of data in bytes
///result points to the array where the output should be stored
///The output is of the same size as that of the input.
int SF2_Cipher(sf2_cipher_context_t* ctx, uint8_t* data, int dataSize, uint8_t* result );

///returns the block size used for a given cipher as specified by ctx. Currently we support only
/// aes128 and aes256, which both use 128bit/16-byte block size
int SF2_GetBlockSize(sf2_cipher_context_t* ctx){
	return 16;
}

#endif //_SF2_HW_CRYPTO_H_
