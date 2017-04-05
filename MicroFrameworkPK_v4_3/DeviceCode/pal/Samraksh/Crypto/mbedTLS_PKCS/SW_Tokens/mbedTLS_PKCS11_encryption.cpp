#include "mbedTLS_PKCS11.h"
#include <mbedtls/cipher.h>

int MBedTLSEncryptDecryptInit(mbedtls_cipher_context_t *cipher_ctx, const unsigned char *key,
        int key_bitlen, const unsigned char *iv, size_t iv_len, BOOL encrypt)
{
    int ret=-1;
    const mbedtls_cipher_info_t *info = cipher_ctx->cipher_info;
    mbedtls_cipher_init( cipher_ctx );
    if( mbedtls_cipher_setup(cipher_ctx, info) != 0 ) {
    	hal_printf("mbedTLS Cipher Setup failed!\n");
    	return ret;
    }
    //cipher_ctx->cipher_info = info;

    if(encrypt){
		if( mbedtls_cipher_setkey(cipher_ctx, key, key_bitlen, MBEDTLS_ENCRYPT) == 0 ){
			if( mbedtls_cipher_set_iv(cipher_ctx, iv, iv_len)  == 0){
				if( mbedtls_cipher_reset(cipher_ctx ) == 0){
					ret =0;
					hal_printf("mbedTLS Cipher Set Key Success for Encrypt: cipher specific ctx ptr %p !\n", cipher_ctx->cipher_ctx);
				}
			}
		}
    }
    else {

		/*if(mbedtls_cipher_setkey(cipher_ctx, key, key_bitlen, MBEDTLS_DECRYPT) == 0){
			if(mbedtls_cipher_set_iv(cipher_ctx, iv, iv_len) == 0){
				if(mbedtls_cipher_reset(cipher_ctx ) == 0){
					hal_printf("mbedTLS Cipher Set Key Success for DeCrypt!\n");
					ret=0;
				}
			}
		}*/
    }
   return ret;
}

int FreeCyptoData(mbedTLSEncryptData* pEnc){
	mbedtls_cipher_context_t *cipher_ctx = &pEnc->SymmetricCtx;
	mbedtls_cipher_free(cipher_ctx);
	PKCS11_MBEDTLS_FREE(pEnc);
	return 0;
}

CK_RV  MBEDTLS_PKCS11_Encryption::InitHelper(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pEncryptMech, CK_OBJECT_HANDLE hKey, BOOL isEncrypt)
{
    PKCS11_MBEDTLS_HEADER();

    mbedTLSEncryptData* pEnc;
    const mbedtls_cipher_info_t* pCipherInfo;
    int padding = 0;

    if(             pSessionCtx                 == NULL) return CKR_SESSION_CLOSED;
    if( isEncrypt && pSessionCtx->EncryptionCtx != NULL) return CKR_SESSION_PARALLEL_NOT_SUPPORTED;
    if(!isEncrypt && pSessionCtx->DecryptionCtx != NULL) return CKR_SESSION_PARALLEL_NOT_SUPPORTED;

    pEnc = (mbedTLSEncryptData*)PKCS11_MBEDTLS_MALLOC(sizeof(mbedTLSEncryptData));

    if(pEnc == NULL) return CKR_DEVICE_MEMORY;

    memset(pEnc, 0, sizeof(mbedTLSEncryptData));

    pEnc->Key = MBEDTLS_PKCS11_Keys::GetKeyFromHandle(pSessionCtx, hKey, !isEncrypt);

    pEnc->IsSymmetric = TRUE;

    switch(pEncryptMech->mechanism)
    {
        case CKM_AES_CBC:
        case CKM_AES_CBC_PAD:
            switch(pEnc->Key->size)
            {
                case 128:
                    pCipherInfo = mbedtls_cipher_info_from_string("AES-128-CBC");
                    break;
                case 192:
                    pCipherInfo = mbedtls_cipher_info_from_string("AES-192-CBC");
                    break;
                case 256:
                	pCipherInfo = mbedtls_cipher_info_from_string("AES-256-CBC");
                    break;
                default:
                    PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_MECHANISM_INVALID);
            }
            if(pEncryptMech->mechanism == CKM_AES_CBC_PAD)
            {
                padding = 1;
            }
            break;

        case CKM_AES_ECB:
        case CKM_AES_ECB_PAD:
            switch(pEnc->Key->size)
            {
                case 128:
                    pCipherInfo = mbedtls_cipher_info_from_string("AES-128-ECB");
                    break;
                case 192:
                    pCipherInfo = mbedtls_cipher_info_from_string("AES-192-ECB");
                    break;
                case 256:
                    pCipherInfo = mbedtls_cipher_info_from_string("AES-256-ECB");
                    break;
                default:
                    PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_MECHANISM_INVALID);
            }
            if(pEncryptMech->mechanism == CKM_AES_ECB_PAD)
            {
                padding = 1;
            }
            break;

        case CKM_DES3_CBC:
            pCipherInfo = mbedtls_cipher_info_from_string("DES-EDE3-CBC");
            break;

        case CKM_DES3_CBC_PAD:
            pCipherInfo = mbedtls_cipher_info_from_string("DES-EDE3-CBC");
            padding = 1;
            break;

        case CKM_RSA_PKCS:
        	PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_MECHANISM_INVALID);
            break;

        default:
            PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_MECHANISM_INVALID);
    }


    pEnc->SymmetricCtx.cipher_info = pCipherInfo;

    if(pEnc->IsSymmetric)
    {
    	size_t ivSize;
        if(pEncryptMech->ulParameterLen > 0 && pEncryptMech->ulParameterLen > 0)
        {
            memcpy(pEnc->IV, pEncryptMech->pParameter, pEncryptMech->ulParameterLen);
            ivSize=pEncryptMech->ulParameterLen;
        }

        if(ivSize > PKCS11_MBEDTLS_MAX_IV_LEN) {
        	PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
        }

        pEnc->Key->ctx = &(pEnc->SymmetricCtx);

        KEY_DATA tmpKey;
        memcpy(&tmpKey,pEnc->Key, sizeof(KEY_DATA));

        //Initialize Encryption or Decryption
        PKCS11_MBEDTLS_CHECKRESULT( MBedTLSEncryptDecryptInit(&pEnc->SymmetricCtx, (const UINT8*)pEnc->Key->key,
        		pEnc->Key->size, pEnc->IV, ivSize, isEncrypt ) );

        memcpy(pEnc->Key,&tmpKey, sizeof(KEY_DATA));
        //TODO: Mukundan: Not sure how to do padding
        //PKCS11_MBEDTLS_CHECKRESULT(EVP_CIPHER_CTX_set_padding(&pEnc->SymmetricCtx, padding));
    }
    else
    {
        //This is for asymettric encryption, we will do this a bit later
    	/*
    	pEnc->Key->ctx = EVP_PKEY_CTX_new((EVP_PKEY*)pEnc->Key->key, NULL);
        if(isEncrypt)
        {
            PKCS11_MBEDTLS_CHECKRESULT(EVP_PKEY_encrypt_init       ((EVP_PKEY_CTX*)pEnc->Key->ctx         ));
            PKCS11_MBEDTLS_CHECKRESULT(EVP_PKEY_CTX_set_rsa_padding((EVP_PKEY_CTX*)pEnc->Key->ctx, padding));
        }
        else
        {
            PKCS11_MBEDTLS_CHECKRESULT(EVP_PKEY_decrypt_init       ((EVP_PKEY_CTX*)pEnc->Key->ctx         ));
            PKCS11_MBEDTLS_CHECKRESULT(EVP_PKEY_CTX_set_rsa_padding((EVP_PKEY_CTX*)pEnc->Key->ctx, padding));
        }
        */
    	PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);

    }

    if(isEncrypt) pSessionCtx->EncryptionCtx = pEnc;
    else          pSessionCtx->DecryptionCtx = pEnc;

    PKCS11_MBEDTLS_CLEANUP();
    if(retVal != CKR_OK && pEnc != NULL)
    {
    	FreeCyptoData(pEnc);
    }
    PKCS11_MBEDTLS_RETURN();
}

CK_RV MBEDTLS_PKCS11_Encryption::EncryptInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pEncryptMech, CK_OBJECT_HANDLE hKey)
{
	return InitHelper(pSessionCtx, pEncryptMech, hKey, TRUE);
}

CK_RV MBEDTLS_PKCS11_Encryption::Encrypt(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen)
{
    PKCS11_MBEDTLS_HEADER();

    if(pEncryptedData == NULL)
    {
        mbedTLSEncryptData *pEnc;
        int blockSize;
        int mod;

        if(pSessionCtx == NULL || pSessionCtx->EncryptionCtx == NULL) return CKR_SESSION_CLOSED;

        pEnc = (mbedTLSEncryptData*)pSessionCtx->EncryptionCtx;

        if(pEnc->IsSymmetric)
        {
            blockSize = mbedtls_cipher_get_block_size(&pEnc->SymmetricCtx);
        }
        else
        {
        	//Mukundan: I think the mbedtls cipher wrapper handles asymmetric crypto too
        	blockSize = mbedtls_cipher_get_block_size(&pEnc->SymmetricCtx);
            //blockSize = EVP_PKEY_size((EVP_PKEY*)pEnc->Key->key);
        }

        mod = ulDataLen % blockSize;
        if(0 != mod)
        {
            *pulEncryptedDataLen = ulDataLen + (blockSize - mod);
        }
        else
        {
            *pulEncryptedDataLen = ulDataLen + blockSize;
        }

        return CKR_OK;
    }

    CK_ULONG tmp = *pulEncryptedDataLen;

    PKCS11_MBEDTLS_CHECK_CK_RESULT(MBEDTLS_PKCS11_Encryption::EncryptUpdate(pSessionCtx, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen));

    tmp -= *pulEncryptedDataLen;

    PKCS11_MBEDTLS_CHECK_CK_RESULT(MBEDTLS_PKCS11_Encryption::EncryptFinal(pSessionCtx, &pEncryptedData[*pulEncryptedDataLen], &tmp));

    *pulEncryptedDataLen += tmp;

    PKCS11_MBEDTLS_NOCLEANUP();

}

CK_RV MBEDTLS_PKCS11_Encryption::EncryptUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen)
{
    PKCS11_MBEDTLS_HEADER();
    mbedTLSEncryptData *pEnc;

    if(pSessionCtx == NULL || pSessionCtx->EncryptionCtx == NULL) return CKR_SESSION_CLOSED;

    pEnc = (mbedTLSEncryptData*)pSessionCtx->EncryptionCtx;
    //mbedtls_cipher_context_t * cipher_ctx = (mbedtls_cipher_context_t *)pEnc->Key->ctx;
    mbedtls_cipher_context_t * cipher_ctx = (mbedtls_cipher_context_t *) &pEnc->SymmetricCtx;

    hal_printf("Crypto specific ctx ptr: %p \n", cipher_ctx->cipher_ctx );

    if(pEnc->IsSymmetric)
    {
        size_t outLen = *pulEncryptedPartLen;
        PKCS11_MBEDTLS_CHECKRESULT( mbedtls_cipher_update (cipher_ctx,  pPart, ulPartLen, pEncryptedPart, &outLen));
        *pulEncryptedPartLen = outLen;
    }
    else
    {
    	//size_t encLen = *pulEncryptedPartLen;
        //PKCS11_MBEDTLS_CHECKRESULT(EVP_PKEY_encrypt((EVP_PKEY_CTX*)pEnc->Key->ctx, pEncryptedPart, &encLen, pPart, ulPartLen));
        //*pulEncryptedPartLen = encLen;
    	//We dont support asymetric at this time.
    	PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
    }

    PKCS11_MBEDTLS_CLEANUP();

    if(retVal != CKR_OK)
    {
    	FreeCyptoData(pEnc);
        pSessionCtx->EncryptionCtx = NULL;
    }

    PKCS11_MBEDTLS_RETURN();

}

CK_RV MBEDTLS_PKCS11_Encryption::EncryptFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pLastEncryptedPart, CK_ULONG_PTR pulLastEncryptedPartLen)
{
    PKCS11_MBEDTLS_HEADER();
    mbedTLSEncryptData *pEnc;

    if(pSessionCtx == NULL || pSessionCtx->EncryptionCtx == NULL) return CKR_SESSION_CLOSED;

    pEnc = (mbedTLSEncryptData*)pSessionCtx->EncryptionCtx;

    if(pEnc->IsSymmetric)
    {
        size_t outLen = *pulLastEncryptedPartLen;
        PKCS11_MBEDTLS_CHECKRESULT(mbedtls_cipher_finish((mbedtls_cipher_context_t *)pEnc->Key->ctx, pLastEncryptedPart, &outLen));
        *pulLastEncryptedPartLen = outLen;

       // PKCS11_MBEDTLS_CHECKRESULT(mbedtls_cipher_reset((mbedtls_cipher_context_t *)pEnc->Key->ctx));
    }
    else
    {
        //EVP_PKEY_CTX_free((EVP_PKEY_CTX*)pEnc->Key->ctx);
        //*pulLastEncryptedPartLen = 0;
        //We dont support asymetric at this time.
        PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
    }

    PKCS11_MBEDTLS_CLEANUP();

    FreeCyptoData(pEnc);
    pSessionCtx->EncryptionCtx = NULL;

    PKCS11_MBEDTLS_RETURN();

}    

CK_RV MBEDTLS_PKCS11_Encryption::DecryptInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pDecryptMech, CK_OBJECT_HANDLE hKey)
{
    return InitHelper(pSessionCtx, pDecryptMech, hKey, FALSE);
}

CK_RV MBEDTLS_PKCS11_Encryption::Decrypt(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen)
{
    PKCS11_MBEDTLS_HEADER();

    if(pData == NULL)
    {
        mbedTLSEncryptData *pDecr;
        int blockSize;
        int mod;

        if(pSessionCtx == NULL || pSessionCtx->DecryptionCtx == NULL) return CKR_SESSION_CLOSED;

        pDecr = (mbedTLSEncryptData*)pSessionCtx->DecryptionCtx;

        if(pDecr->IsSymmetric)
        {
            blockSize =  mbedtls_cipher_get_block_size(&pDecr->SymmetricCtx);
        }
        else
        {
        	//Not sure if this is right. But just keep it same
        	blockSize =  mbedtls_cipher_get_block_size(&pDecr->SymmetricCtx);
        	//blockSize = EVP_PKEY_size((EVP_PKEY*)pDecr->Key->key);
        }

        mod = ulEncryptedDataLen % blockSize;
        if(0 != mod)
        {
            *pulDataLen = ulEncryptedDataLen + (blockSize - mod);
        }
        else
        {
            *pulDataLen = ulEncryptedDataLen + blockSize;
        }

        return CKR_OK;
    }

    CK_ULONG tmp = *pulDataLen;

    PKCS11_MBEDTLS_CHECK_CK_RESULT(MBEDTLS_PKCS11_Encryption::DecryptUpdate(pSessionCtx, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen));

    tmp -= *pulDataLen;

    PKCS11_MBEDTLS_CHECK_CK_RESULT(MBEDTLS_PKCS11_Encryption::DecryptFinal(pSessionCtx, &pData[*pulDataLen], &tmp));

    *pulDataLen += tmp;

    PKCS11_MBEDTLS_NOCLEANUP();

}    

CK_RV MBEDTLS_PKCS11_Encryption::DecryptUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen)
{
    PKCS11_MBEDTLS_HEADER();

    mbedTLSEncryptData *pDec;

    if(pSessionCtx == NULL || pSessionCtx->DecryptionCtx == NULL) return CKR_SESSION_CLOSED;

    pDec = (mbedTLSEncryptData*)pSessionCtx->DecryptionCtx;

    if(pDec->IsSymmetric)
    {
        size_t outLen = *pulPartLen;
        PKCS11_MBEDTLS_CHECKRESULT( mbedtls_cipher_update ((mbedtls_cipher_context_t *)pDec->Key->ctx, pEncryptedPart, ulEncryptedPartLen, pPart, &outLen));
        *pulPartLen = outLen;
    }
    else
    {
        //size_t outLen = *pulPartLen;
        //PKCS11_MBEDTLS_CHECKRESULT(EVP_PKEY_decrypt((EVP_PKEY_CTX*)pDec->Key->ctx, pPart, &outLen, pEncryptedPart, ulEncryptedPartLen));
        //*pulPartLen = outLen;
    	 PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
    }

    PKCS11_MBEDTLS_CLEANUP();

    if(retVal != CKR_OK)
    {
    	FreeCyptoData(pDec);
        pSessionCtx->DecryptionCtx = NULL;
    }

    PKCS11_MBEDTLS_RETURN();

}    

CK_RV MBEDTLS_PKCS11_Encryption::DecryptFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen)
{
    PKCS11_MBEDTLS_HEADER();

    mbedTLSEncryptData *pDec;

    if(pSessionCtx == NULL || pSessionCtx->DecryptionCtx == NULL) return CKR_SESSION_CLOSED;

    pDec = (mbedTLSEncryptData*)pSessionCtx->DecryptionCtx;

    if(pDec->IsSymmetric)
    {
        size_t outLen = *pulLastPartLen;
        PKCS11_MBEDTLS_CHECKRESULT(mbedtls_cipher_finish((mbedtls_cipher_context_t *)pDec->Key->ctx, pLastPart, &outLen));
        *pulLastPartLen = outLen;
        PKCS11_MBEDTLS_CHECKRESULT(mbedtls_cipher_reset((mbedtls_cipher_context_t *)pDec->Key->ctx));
    }
    else
    {
        //EVP_PKEY_CTX_free((EVP_PKEY_CTX*)pDec->Key->ctx);
        //*pulLastPartLen = 0;
    	PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
    }

    PKCS11_MBEDTLS_CLEANUP();

    FreeCyptoData(pDec);
    pSessionCtx->DecryptionCtx = NULL;

    PKCS11_MBEDTLS_RETURN();
}    


