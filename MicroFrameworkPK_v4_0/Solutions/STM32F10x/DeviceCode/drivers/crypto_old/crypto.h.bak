/*
 * Copyright (c) 2007, Cameron Rich
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, 
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 * * Neither the name of the axTLS project nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software 
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file crypto.h
 */

#ifndef HEADER_CRYPTO_H
#define HEADER_CRYPTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "bigint_impl.h"
#include "bigint.h"

#ifndef STDCALL
#define STDCALL
#endif
#ifndef EXP_FUNC
#define EXP_FUNC
#endif


/* enable features based on a 'super-set' capbaility. */
#if defined(CONFIG_SSL_FULL_MODE) 
#define CONFIG_SSL_ENABLE_CLIENT
#define CONFIG_SSL_CERT_VERIFICATION
#elif defined(CONFIG_SSL_ENABLE_CLIENT)
#define CONFIG_SSL_CERT_VERIFICATION
#endif

/**************************************************************************
 * AES declarations 
 **************************************************************************/

#define AES_MAXROUNDS			14
#define AES_BLOCKSIZE           16
#define AES_IV_SIZE             16

typedef struct aes_key_st 
{
    UINT16 rounds;
    UINT16 key_size;
    UINT32 ks[(AES_MAXROUNDS+1)*8];
    UINT8 iv[AES_IV_SIZE];
} AES_CTX;

typedef enum
{
    AES_MODE_128,
    AES_MODE_256
} AES_MODE;

void AES_set_key(AES_CTX *ctx, const UINT8 *key, 
        const UINT8 *iv, AES_MODE mode);
void AES_cbc_encrypt(AES_CTX *ctx, const UINT8 *msg, 
        UINT8 *out, int length);
void AES_cbc_decrypt(AES_CTX *ks, const UINT8 *in, UINT8 *out, int length);
void AES_convert_key(AES_CTX *ctx);

/**************************************************************************
 * RC4 declarations 
 **************************************************************************/

typedef struct 
{
    UINT8 x, y, m[256];
} RC4_CTX;

void RC4_setup(RC4_CTX *s, const UINT8 *key, int length);
void RC4_crypt(RC4_CTX *s, const UINT8 *msg, UINT8 *data, int length);

/**************************************************************************
 * SHA1 declarations 
 **************************************************************************/

#define SHA1_SIZE   20

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation
 */
typedef struct 
{
    UINT32 Intermediate_Hash[SHA1_SIZE/4]; /* Message Digest */
    UINT32 Length_Low;            /* Message length in bits */
    UINT32 Length_High;           /* Message length in bits */
    UINT16 Message_Block_Index;   /* Index into message block array   */
    UINT8 Message_Block[64];      /* 512-bit message blocks */
} SHA1_CTX;

void SHA1_Init(SHA1_CTX *);
void SHA1_Update(SHA1_CTX *, const UINT8 * msg, int len);
void SHA1_Final(UINT8 *digest, SHA1_CTX *);

/**************************************************************************
 * MD2 declarations 
 **************************************************************************/

#define MD2_SIZE 16

typedef struct
{
    unsigned char cksum[16];    /* checksum of the data block */
    unsigned char state[48];    /* intermediate digest state */
    unsigned char buffer[16];   /* data block being processed */
    int left;                   /* amount of data in buffer */
} MD2_CTX;

EXP_FUNC void STDCALL MD2_Init(MD2_CTX *ctx);
EXP_FUNC void STDCALL MD2_Update(MD2_CTX *ctx, const UINT8 *input, int ilen);
EXP_FUNC void STDCALL MD2_Final(UINT8 *digest, MD2_CTX *ctx);

/**************************************************************************
 * MD5 declarations 
 **************************************************************************/

#define MD5_SIZE    16

typedef struct 
{
  UINT32 state[4];        /* state (ABCD) */
  UINT32 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  UINT8 buffer[64];       /* input buffer */
} MD5_CTX;

EXP_FUNC void STDCALL MD5_Init(MD5_CTX *);
EXP_FUNC void STDCALL MD5_Update(MD5_CTX *, const UINT8 *msg, int len);
EXP_FUNC void STDCALL MD5_Final(UINT8 *digest, MD5_CTX *);

/**************************************************************************
 * HMAC declarations 
 **************************************************************************/
void hmac_md5(const UINT8 *msg, int length, const UINT8 *key, 
        int key_len, UINT8 *digest);
void hmac_sha1(const UINT8 *msg, int length, const UINT8 *key, 
        int key_len, UINT8 *digest);

/**************************************************************************
 * RSA declarations 
 **************************************************************************/

typedef struct 
{
    bigint *m;              /* modulus */
    bigint *e;              /* public exponent */
    bigint *d;              /* private exponent */
#ifdef CONFIG_BIGINT_CRT
    bigint *p;              /* p as in m = pq */
    bigint *q;              /* q as in m = pq */
    bigint *dP;             /* d mod (p-1) */
    bigint *dQ;             /* d mod (q-1) */
    bigint *qInv;           /* q^-1 mod p */
#endif
    int num_octets;
    BI_CTX *bi_ctx;
} RSA_CTX;

void RSA_priv_key_new(RSA_CTX **rsa_ctx, 
        const UINT8 *modulus, int mod_len,
        const UINT8 *pub_exp, int pub_len,
        const UINT8 *priv_exp, int priv_len
#ifdef CONFIG_BIGINT_CRT
      , const UINT8 *p, int p_len,
        const UINT8 *q, int q_len,
        const UINT8 *dP, int dP_len,
        const UINT8 *dQ, int dQ_len,
        const UINT8 *qInv, int qInv_len
#endif
        );
void RSA_pub_key_new(RSA_CTX **rsa_ctx, 
        const UINT8 *modulus, int mod_len,
        const UINT8 *pub_exp, int pub_len);
void RSA_free(RSA_CTX *ctx);
int RSA_decrypt(const RSA_CTX *ctx, const UINT8 *in_data, UINT8 *out_data,
        int is_decryption);
bigint *RSA_private(const RSA_CTX *c, bigint *bi_msg);
#if defined(CONFIG_SSL_CERT_VERIFICATION) || defined(CONFIG_SSL_GENERATE_X509_CERT)
bigint *RSA_sign_verify(BI_CTX *ctx, const UINT8 *sig, int sig_len,
        bigint *modulus, bigint *pub_exp);
bigint *RSA_public(const RSA_CTX * c, bigint *bi_msg);
int RSA_encrypt(const RSA_CTX *ctx, const UINT8 *in_data, UINT16 in_len, 
        UINT8 *out_data, int is_signing);
void RSA_print(const RSA_CTX *ctx);
#endif

/**************************************************************************
 * RNG declarations 
 **************************************************************************/
EXP_FUNC void STDCALL RNG_initialize(const UINT8 *seed_buf, int size);
EXP_FUNC void STDCALL RNG_terminate(void);
EXP_FUNC void STDCALL get_random(int num_rand_bytes, UINT8 *rand_data);
void get_random_NZ(int num_rand_bytes, UINT8 *rand_data);

#ifdef __cplusplus
}
#endif

#endif 
