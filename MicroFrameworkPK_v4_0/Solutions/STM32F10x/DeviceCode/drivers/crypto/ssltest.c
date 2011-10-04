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

/*
 * The testing of the crypto and ssl stuff goes here. Keeps the individual code
 * modules from being uncluttered with test code.
 *
 * This is test code - I make no apologies for the quality!
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef WIN32
#include <pthread.h>
#endif

//#include "os_port.h"
//#include "ssl.h"
#include "C:\MicroFrameworkPK_v4_0\Solutions\STM32F10x\DeviceCode\drivers\crypto\bigint.c"
#include "C:\MicroFrameworkPK_v4_0\Solutions\STM32F10x\DeviceCode\drivers\crypto\crypto.h"
#include "C:\MicroFrameworkPK_v4_0\Solutions\STM32F10x\DeviceCode\drivers\crypto\aes.c"
#include "C:\MicroFrameworkPK_v4_0\Solutions\STM32F10x\DeviceCode\drivers\crypto\hmac.c"

#define DEFAULT_CERT            "../ssl/test/axTLS.x509_512.cer"
#define DEFAULT_KEY             "../ssl/test/axTLS.key_512"     
//#define DEFAULT_SVR_OPTION      SSL_DISPLAY_BYTES|SSL_DISPLAY_STATES
#define DEFAULT_SVR_OPTION      0
//#define DEFAULT_CLNT_OPTION      SSL_DISPLAY_BYTES|SSL_DISPLAY_STATES
#define DEFAULT_CLNT_OPTION     0

static int g_port = 19001;
extern bigint *bi_str_import(BI_CTX *ctx, const char *data);
extern BI_CTX *bi_initialize(void);

/**************************************************************************
 * AES tests 
 * 
 * Run through a couple of the RFC3602 tests to verify that AES is correct.
 **************************************************************************/
#define TEST1_SIZE  16
#define TEST2_SIZE  32

int AES_test(BI_CTX *bi_ctx)
{
    AES_CTX aes_key;
    int res = 1;
    UINT8 key[TEST1_SIZE];
    UINT8 iv[TEST1_SIZE];

    {
        /*
            Case #1: Encrypting 16 bytes (1 block) using AES-CBC
            Key       : 0x06a9214036b8a15b512e03d534120006
            IV        : 0x3dafba429d9eb430b422da802c9fac41
            Plaintext : "Single block msg"
            Ciphertext: 0xe353779c1079aeb82708942dbe77181a

        */
        char *in_str =  "Single block msg";
        UINT8 ct[TEST1_SIZE];
        UINT8 enc_data[TEST1_SIZE];
        UINT8 dec_data[TEST1_SIZE];

        bigint *key_bi = bi_str_import(
                bi_ctx, "06A9214036B8A15B512E03D534120006");
        bigint *iv_bi = bi_str_import(
                bi_ctx, "3DAFBA429D9EB430B422DA802C9FAC41");
        bigint *ct_bi = bi_str_import(
                bi_ctx, "E353779C1079AEB82708942DBE77181A");
        bi_export(bi_ctx, key_bi, key, TEST1_SIZE);
        bi_export(bi_ctx, iv_bi, iv, TEST1_SIZE);
        bi_export(bi_ctx, ct_bi, ct, TEST1_SIZE);
		
		

        AES_set_key(&aes_key, key, iv, AES_MODE_128);
        AES_cbc_encrypt(&aes_key, (UINT8 *)in_str, 
                enc_data, sizeof(enc_data));
        if (memcmp(enc_data, ct, sizeof(ct)))
        {
			
       //     printf("Error: AES ENCRYPT #1 failed\n");
            goto end;
        }
		char *test = (char *)enc_data;
        AES_set_key(&aes_key, key, iv, AES_MODE_128);
        AES_convert_key(&aes_key);
        AES_cbc_decrypt(&aes_key, enc_data, dec_data, sizeof(enc_data));

        if (memcmp(dec_data, in_str, sizeof(dec_data)))
        {
        //    printf("Error: AES DECRYPT #1 failed\n");
            goto end;
        }
    }

    {
        /*
            Case #2: Encrypting 32 bytes (2 blocks) using AES-CBC 
            Key       : 0xc286696d887c9aa0611bbb3e2025a45a
						
            IV        : 0x562e17996d093d28ddb3ba695a2e6f58
            Plaintext : 0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f
            Ciphertext: 0xd296cd94c2cccf8a3a863028b5e1dc0a
                          7586602d253cfff91b8266bea6d61ab1
        */
        UINT8 in_data[TEST2_SIZE];
        UINT8 ct[TEST2_SIZE];
        UINT8 enc_data[TEST2_SIZE];
        UINT8 dec_data[TEST2_SIZE];

        bigint *in_bi = bi_str_import(bi_ctx,
            "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F");
        bigint *key_bi = bi_str_import(
                bi_ctx, "C286696D887C9AA0611BBB3E2025A45A");
        bigint *iv_bi = bi_str_import(
                bi_ctx, "562E17996D093D28DDB3BA695A2E6F58");
        bigint *ct_bi = bi_str_import(bi_ctx,
            "D296CD94C2CCCF8A3A863028B5E1DC0A7586602D253CFFF91B8266BEA6D61AB1");
        bi_export(bi_ctx, in_bi, in_data, TEST2_SIZE);
        bi_export(bi_ctx, key_bi, key, TEST1_SIZE);
        bi_export(bi_ctx, iv_bi, iv, TEST1_SIZE);
        bi_export(bi_ctx, ct_bi, ct, TEST2_SIZE);

        AES_set_key(&aes_key, key, iv, AES_MODE_128);
        AES_cbc_encrypt(&aes_key, (const UINT8 *)in_data, 
                enc_data, sizeof(enc_data));

        if (memcmp(enc_data, ct, sizeof(ct)))
        {
          //  printf("Error: ENCRYPT #2 failed\n");
            goto end;
        }

        AES_set_key(&aes_key, key, iv, AES_MODE_128);
        AES_convert_key(&aes_key);
        AES_cbc_decrypt(&aes_key, enc_data, dec_data, sizeof(enc_data));
        if (memcmp(dec_data, in_data, sizeof(dec_data)))
        {
           // printf("Error: DECRYPT #2 failed\n");
            goto end;
        }
    } 

    res = 0;
   // printf("All AES tests passed\n");

end:
    return res;
}

/**************************************************************************
 * RC4 tests 
 *
 * ARC4 tests vectors from OpenSSL (crypto/rc4/rc4test.c)
 **************************************************************************/
static const UINT8 keys[7][30]=
{
    {8,0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef},
    {8,0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef},
    {8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {4,0xef,0x01,0x23,0x45},
    {8,0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef},
    {4,0xef,0x01,0x23,0x45},
};

static const UINT8 data_len[7]={8,8,8,20,28,10};
static UINT8 data[7][30]=
{
    {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xff},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0xff},
        {0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,
            0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,
            0x12,0x34,0x56,0x78,0x9A,0xBC,0xDE,0xF0,
            0x12,0x34,0x56,0x78,0xff},
            {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff},
            {0},
};

static const UINT8 output[7][30]=
{
    {0x75,0xb7,0x87,0x80,0x99,0xe0,0xc5,0x96,0x00},
    {0x74,0x94,0xc2,0xe7,0x10,0x4b,0x08,0x79,0x00},
    {0xde,0x18,0x89,0x41,0xa3,0x37,0x5d,0x3a,0x00},
    {0xd6,0xa1,0x41,0xa7,0xec,0x3c,0x38,0xdf,
        0xbd,0x61,0x5a,0x11,0x62,0xe1,0xc7,0xba,
        0x36,0xb6,0x78,0x58,0x00},
        {0x66,0xa0,0x94,0x9f,0x8a,0xf7,0xd6,0x89,
            0x1f,0x7f,0x83,0x2b,0xa8,0x33,0xc0,0x0c,
            0x89,0x2e,0xbe,0x30,0x14,0x3c,0xe2,0x87,
            0x40,0x01,0x1e,0xcf,0x00},
            {0xd6,0xa1,0x41,0xa7,0xec,0x3c,0x38,0xdf,0xbd,0x61,0x00},
            {0},
};

/*static int RC4_test(BI_CTX *bi_ctx)
{
    int i, res = 1;
    RC4_CTX s;

    for (i = 0; i < 6; i++)
    {
        RC4_setup(&s, &keys[i][1], keys[i][0]);
        RC4_crypt(&s, data[i], data[i], data_len[i]);

        if (memcmp(data[i], output[i], data_len[i]))
        {
            printf("Error: RC4 CRYPT #%d failed\n", i);
            goto end;
        }
    }

    res = 0;
    printf("All RC4 tests passed\n");

end:
    return res;
} */

/**************************************************************************
 * SHA1 tests 
 *
 * Run through a couple of the RFC3174 tests to verify that SHA1 is correct.
 **************************************************************************/
/*static int SHA1_test(BI_CTX *bi_ctx)
{
    SHA1_CTX ctx;
    uint8_t ct[SHA1_SIZE];
    uint8_t digest[SHA1_SIZE];
    int res = 1;

    {
        const char *in_str = "abc";
        bigint *ct_bi = bi_str_import(bi_ctx,
                "A9993E364706816ABA3E25717850C26C9CD0D89D");
        bi_export(bi_ctx, ct_bi, ct, SHA1_SIZE);

        SHA1_Init(&ctx);
        SHA1_Update(&ctx, (const uint8_t *)in_str, strlen(in_str));
        SHA1_Final(digest, &ctx);

        if (memcmp(digest, ct, sizeof(ct)))
        {
            printf("Error: SHA1 #1 failed\n");
            goto end;
        }
    }

    {
        const char *in_str =
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
        bigint *ct_bi = bi_str_import(bi_ctx,
                "84983E441C3BD26EBAAE4AA1F95129E5E54670F1");
        bi_export(bi_ctx, ct_bi, ct, SHA1_SIZE);

        SHA1_Init(&ctx);
        SHA1_Update(&ctx, (const uint8_t *)in_str, strlen(in_str));
        SHA1_Final(digest, &ctx);

        if (memcmp(digest, ct, sizeof(ct)))
        {
            printf("Error: SHA1 #2 failed\n");
            goto end;
        }
    }

    res = 0;
    printf("All SHA1 tests passed\n");

end:
    return res;
}

/**************************************************************************
 * MD5 tests 
 *
 * Run through a couple of the RFC1321 tests to verify that MD5 is correct.
 **************************************************************************/
/*static int MD5_test(BI_CTX *bi_ctx)
{
    MD5_CTX ctx;
    uint8_t ct[MD5_SIZE];
    uint8_t digest[MD5_SIZE];
    int res = 1;

    {
        const char *in_str =  "abc";
        bigint *ct_bi = bi_str_import(bi_ctx, 
                "900150983CD24FB0D6963F7D28E17F72");
        bi_export(bi_ctx, ct_bi, ct, MD5_SIZE);

        MD5_Init(&ctx);
        MD5_Update(&ctx, (const uint8_t *)in_str, strlen(in_str));
        MD5_Final(digest, &ctx);

        if (memcmp(digest, ct, sizeof(ct)))
        {
            printf("Error: MD5 #1 failed\n");
            goto end;
        }
    }

    {
        const char *in_str =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        bigint *ct_bi = bi_str_import(
                bi_ctx, "D174AB98D277D9F5A5611C2C9F419D9F");
        bi_export(bi_ctx, ct_bi, ct, MD5_SIZE);

        MD5_Init(&ctx);
        MD5_Update(&ctx, (const uint8_t *)in_str, strlen(in_str));
        MD5_Final(digest, &ctx);

        if (memcmp(digest, ct, sizeof(ct)))
        {
            printf("Error: MD5 #2 failed\n");
            goto end;
        }
    }
    res = 0;
    printf("All MD5 tests passed\n");

end:
    return res;
}

/**************************************************************************
 * HMAC tests 
 *
 * Run through a couple of the RFC2202 tests to verify that HMAC is correct.
 **************************************************************************/
int HMAC_test(BI_CTX *bi_ctx)
{
    UINT8 key[SHA1_SIZE];
    UINT8 ct[SHA1_SIZE];
    UINT8 dgst[SHA1_SIZE];
    int res = 1;
    char *key_str;

    char *data_str = "Hi There";
   // bigint *key_bi = bi_str_import(bi_ctx, "0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B");
   // bigint *ct_bi = bi_str_import(bi_ctx, "9294727A3638BB1C13F48EF8158BFC9D");
  /*  bi_export(bi_ctx, key_bi, key, MD5_SIZE);
    bi_export(bi_ctx, ct_bi, ct, MD5_SIZE);
    hmac_md5((const uint8_t *)data_str, 8, key, MD5_SIZE, dgst);
    if (memcmp(dgst, ct, MD5_SIZE))
    {
        printf("HMAC MD5 #1 failed\n");
        goto end;
    }

    data_str = "what do ya want for nothing?";
    key_str = "Jefe";
    ct_bi = bi_str_import(bi_ctx, "750C783E6AB0B503EAA86E310A5DB738");
    bi_export(bi_ctx, ct_bi, ct, MD5_SIZE);
    hmac_md5((const uint8_t *)data_str, 28, (const uint8_t *)key_str, 4, dgst);
    if (memcmp(dgst, ct, MD5_SIZE))
    {
        printf("HMAC MD5 #2 failed\n");
        goto end;
    }
   */
   // data_str = "Hi There";
    bigint *key_bi = bi_str_import(bi_ctx, "0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B");
    bi_export(bi_ctx, key_bi, key, SHA1_SIZE);
    bigint *ct_bi = bi_str_import(bi_ctx, "B617318655057264E28BC0B6FB378C8EF146BE00");
    bi_export(bi_ctx, ct_bi, ct, SHA1_SIZE);

    hmac_sha1((UINT8 *)data_str, 8, 
            (UINT8 *)key, SHA1_SIZE, dgst);
    if (memcmp(dgst, ct, SHA1_SIZE))
    {
      //  printf("HMAC SHA1 #1 failed\n");
        goto end;
    }

    data_str = "what do ya want for nothing?";
    key_str = "Jefe";
    ct_bi = bi_str_import(bi_ctx, "EFFCDF6AE5EB2FA2D27416D5F184DF9C259A7C79");
    bi_export(bi_ctx, ct_bi, ct, SHA1_SIZE);

    hmac_sha1((UINT8 *)data_str, 28, (UINT8 *)key_str, 5, dgst);
    if (memcmp(dgst, ct, SHA1_SIZE))
    {
      // printf("HMAC SHA1 failed\n");
        exit(1);
    }

    res = 0;
 //   printf("All HMAC tests passed\n");

end:
    return res;
}




/**************************************************************************
 * main()
 *
 **************************************************************************/
 
 int AESTestCall()
 {
 int ret =0;
 BI_CTX *bi_ctx;
   bi_ctx = bi_initialize();

    if (AES_test(bi_ctx))
    {
      //  printf("AES tests failed\n");
		ret = 1;
	}
	bi_terminate(bi_ctx);
	return ret;
 }
 
 int HMACTestCall()
 {
  int ret =0;
 BI_CTX *bi_ctx;
   bi_ctx = bi_initialize();
   
    if (HMAC_test(bi_ctx))
    {
    //    printf("HMAC tests failed\n");
        ret =1;
    }
	bi_terminate(bi_ctx);
	return ret;
 }
/*int main(int argc, char *argv[])
{
    int ret = 1;
    BI_CTX *bi_ctx;
    int fd;

// #ifdef WIN32
    // WSADATA wsaData;
    // WORD wVersionRequested = MAKEWORD(2, 2);
    // WSAStartup(wVersionRequested, &wsaData);
    // fd = _open("test_result.txt", O_WRONLY|O_TEMPORARY|O_CREAT, _S_IWRITE);
    // dup2(fd, 2);                        /* write stderr to this file */
// #else
    // fd = open("/dev/null", O_WRONLY);   /* write stderr to /dev/null */
    // signal(SIGPIPE, SIG_IGN);           /* ignore pipe errors */
    // dup2(fd, 2);
// #endif

    // /* can't do testing in this mode */
// #if defined CONFIG_SSL_GENERATE_X509_CERT
    // printf("Error: Must compile with default key/certificates\n");
    // exit(1);
// #endif

    // bi_ctx = bi_initialize();

    // if (AES_test(bi_ctx))
    // {
        // printf("AES tests failed\n");
        // goto cleanup;
    // }
    // TTY_FLUSH();

    // if (RC4_test(bi_ctx))
    // {
        // printf("RC4 tests failed\n");
        // goto cleanup;
    // }
    // TTY_FLUSH();

    // if (MD5_test(bi_ctx))
    // {
        // printf("MD5 tests failed\n");
        // goto cleanup;
    // }
    // TTY_FLUSH();

    // if (SHA1_test(bi_ctx))
    // {
        // printf("SHA1 tests failed\n");
        // goto cleanup;
    // }
    // TTY_FLUSH();

    // if (HMAC_test(bi_ctx))
    // {
        // printf("HMAC tests failed\n");
        // goto cleanup;
    // }
    // TTY_FLUSH();

    // if (BIGINT_test(bi_ctx))
    // {
        // printf("BigInt tests failed!\n");
        // goto cleanup;
    // }
    // TTY_FLUSH();

    

    // if (RSA_test())
    // {
        // printf("RSA tests failed\n");
        // goto cleanup;
    // }
    // TTY_FLUSH();

    // if (cert_tests())
    // {
        // printf("CERT tests failed\n");
        // goto cleanup;
    // }
    // TTY_FLUSH();

// #if !defined(WIN32) && defined(CONFIG_SSL_CTX_MUTEXING)
    // if (multi_thread_test())
        // goto cleanup;
// #endif

    // if (SSL_basic_test())
        // goto cleanup;

    // system("sh ../ssl/test/killopenssl.sh");

    // if (SSL_unblocked_test())
        // goto cleanup;

    // system("sh ../ssl/test/killopenssl.sh");

    // if (SSL_client_tests())
        // goto cleanup;

    // system("sh ../ssl/test/killopenssl.sh");

    // if (SSL_server_tests())
        // goto cleanup;

    // system("sh ../ssl/test/killopenssl.sh");

    // if (header_issue())
    // {
        // printf("Header tests failed\n"); TTY_FLUSH();
        // goto cleanup;
    // }

    // ret = 0;        /* all ok */
    // printf("**** ALL TESTS PASSED ****\n"); TTY_FLUSH();
// cleanup:

    // if (ret)
        // printf("Error: Some tests failed!\n");

 //  close(fd);
    // return ret;
// }
