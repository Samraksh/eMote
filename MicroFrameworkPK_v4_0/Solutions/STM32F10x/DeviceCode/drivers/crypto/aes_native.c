// #include <stdio.h>
// #include <stdlib.h>
// #include <signal.h>
// #include <string.h>
// #include <errno.h>
// #include <sys/stat.h>
// #include <fcntl.h>
//#include <tinyhal.h>


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

#define TEST1_SIZE  16
#define TEST2_SIZE  32

/*
            Encrypting 16 bytes (1 block) using AES-CBC
            Key       : 0x06a9214036b8a15b512e03d534120006
            IV        : 0x3dafba429d9eb430b422da802c9fac41
            Plaintext : Parameter

        */
		
void AES_Encrypt(char *str, UINT8* enc_data, int length) 
{
	BI_CTX *bi_ctx;
	bi_ctx = bi_initialize();
	AES_CTX aes_key;
    UINT8 key[TEST1_SIZE];
    UINT8 iv[TEST1_SIZE];
	UINT8 ct[TEST1_SIZE];
	char *in_str = str;	
//	UINT8 ct[TEST1_SIZE];
	int i = 0;

	//debug_printf("ckp1");
	bigint *key_bi = bi_str_import(
                bi_ctx, "06A9214036B8A15B512E03D534120006");
    bigint *iv_bi = bi_str_import(
                bi_ctx, "3DAFBA429D9EB430B422DA802C9FAC41");
	//bigint *ct_bi = bi_str_import(
     //           bi_ctx, "E353779C1079AEB82708942DBE77181A");
	//debug_printf("ckp2");			
	bi_export(bi_ctx, key_bi, key, TEST1_SIZE);
	//debug_printf("ckp3");
    bi_export(bi_ctx, iv_bi, iv, TEST1_SIZE);
	//debug_printf("ckp4");
	//bi_export(bi_ctx, ct_bi, ct, TEST1_SIZE);
	//debug_printf("ckp5");	
	AES_set_key(&aes_key, key, iv, AES_MODE_128);
	//debug_printf("ckp6");
    AES_cbc_encrypt(&aes_key, (UINT8 *)in_str, enc_data,length);
	//debug_printf("ckp7");
	bi_terminate(bi_ctx);			
	//debug_printf("ckp8");
	/*
	debug_printf("%s", in_str);
	
	for(i =0; i < 16; i++)
	  {
		  //debug_printf("%d", enc_data[i]);
		  //debug_printf("%c",(char *)encVal[i]);
		  debug_printf("%02x",(char *)enc_data[i]);
	  }
	*/
	
}
void AES_CTEdit(UINT8* ct)
{
BI_CTX *bi_ctx;
	bi_ctx = bi_initialize();
	bigint *ct_bi = bi_str_import(
                bi_ctx, "E353779C1079AEB82708942DBE77181A");
bi_export(bi_ctx, ct_bi, ct, TEST1_SIZE);
bi_terminate(bi_ctx);	
}


void AES_Decrypt(char *enc_data, UINT8 *dec_data)
{	
	BI_CTX *bi_ctx;
	bi_ctx = bi_initialize();
	AES_CTX aes_key;
    UINT8 key[TEST1_SIZE];
    UINT8 iv[TEST1_SIZE];
	UINT8 ct[TEST1_SIZE];
//	UINT8 dec_data[TEST1_SIZE];
	
	bigint *key_bi = bi_str_import(
                bi_ctx, "06A9214036B8A15B512E03D534120006");
    bigint *iv_bi = bi_str_import(
                bi_ctx, "3DAFBA429D9EB430B422DA802C9FAC41");
				
	bi_export(bi_ctx, key_bi, key, TEST1_SIZE);
    bi_export(bi_ctx, iv_bi, iv, TEST1_SIZE);
	
	AES_set_key(&aes_key, key, iv, AES_MODE_128);
	AES_convert_key(&aes_key);
	AES_cbc_decrypt(&aes_key, (UINT8*)enc_data, dec_data, sizeof(enc_data));
	bi_terminate(bi_ctx);
	//return dec_data;
}
