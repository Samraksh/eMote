#include "SF2_HW_pkcs11.h"
//

int EncryptDecryptInit(sf2_cipher_context_t *cipher_ctx, const unsigned char *key,
        int key_bitlen, const unsigned char *iv, size_t iv_len, BOOL _encrypt,
		SF2_CipherType type, SF2_CipherMode _mode)
{
    int ret=0;
    //const mbedtls_cipher_info_t *info = cipher_ctx->cipher_info;
    cipher_ctx->type = type;
    cipher_ctx->iv_size = iv_len;
    cipher_ctx->key_bitlen=key_bitlen;
    cipher_ctx->mode = _mode;

    memcpy(cipher_ctx->iv, iv, iv_len);
    memcpy(cipher_ctx->key, key, key_bitlen/8); //Make sure keybitlength is a whole number in bytes.

    _encrypt ? cipher_ctx->operation = ENCRYPT : cipher_ctx->operation = DECRYPT;

    return ret;
}

int FreeCyptoData(SF2_HW_EncryptData* pEnc){
	sf2_cipher_context_t *cipher_ctx = &pEnc->SymmetricCtx;
	//mbedtls_cipher_free(cipher_ctx);
	//SF2_HW_PKCS11_FREE()
	SF2_HW_PKCS11_FREE(pEnc);
	return 0;
}

CK_RV  SF2_HW_PKCS11_Encryption::InitHelper(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pEncryptMech, CK_OBJECT_HANDLE hKey, BOOL isEncrypt)
{
    SF2_HW_PKCS11_HEADER();

    SF2_HW_EncryptData* pEnc;
    SF2_CipherType cipherType;
    SF2_CipherMode mode;
   // const mbedtls_cipher_info_t* pCipherInfo;
    int padding = 0;

    if(             pSessionCtx                 == NULL) return CKR_SESSION_CLOSED;
    if( isEncrypt && pSessionCtx->EncryptionCtx != NULL) return CKR_SESSION_PARALLEL_NOT_SUPPORTED;
    if(!isEncrypt && pSessionCtx->DecryptionCtx != NULL) return CKR_SESSION_PARALLEL_NOT_SUPPORTED;

    pEnc = (SF2_HW_EncryptData*)SF2_HW_PKCS11_MALLOC(sizeof(SF2_HW_EncryptData));

    if(pEnc == NULL) return CKR_DEVICE_MEMORY;

    memset(pEnc, 0, sizeof(SF2_HW_EncryptData));

    pEnc->Key = SF2_HW_PKCS11_Keys::GetKeyFromHandle(pSessionCtx, hKey, !isEncrypt);

    pEnc->IsSymmetric = TRUE;

    switch(pEncryptMech->mechanism)
    {
        case CKM_AES_CBC:
        case CKM_AES_CBC_PAD:
            switch(pEnc->Key->size)
            {
                case 128:
                	cipherType = AES_128;
                    break;
                case 256:
                	cipherType = AES_256;
                    break;
                default:
                    SF2_HW_PKCS11_SET_AND_LEAVE(CKR_MECHANISM_INVALID);
            }
            mode = CBC;
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
            		cipherType = AES_128;
            		break;
				case 256:
					cipherType = AES_256;
					break;
				default:
					SF2_HW_PKCS11_SET_AND_LEAVE(CKR_MECHANISM_INVALID);
            }
            mode = ECB;
            if(pEncryptMech->mechanism == CKM_AES_ECB_PAD)
            {
                padding = 1;
            }
            break;
        default:
            SF2_HW_PKCS11_SET_AND_LEAVE(CKR_MECHANISM_INVALID);
    }

    pEnc->SymmetricCtx.type = cipherType;

    if(pEnc->IsSymmetric)
    {
    	size_t ivSize;
        if(pEncryptMech->ulParameterLen > 0 && pEncryptMech->ulParameterLen > 0)
        {
            memcpy(pEnc->IV, pEncryptMech->pParameter, pEncryptMech->ulParameterLen);
            ivSize=pEncryptMech->ulParameterLen;
        }

        if(ivSize > SF2_HW_PKCS11_MAX_IV_LEN) {
        	SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
        }

        pEnc->Key->ctx = &(pEnc->SymmetricCtx);

        KEY_DATA tmpKey;
        memcpy(&tmpKey,pEnc->Key, sizeof(KEY_DATA));

        //Initialize Encryption or Decryption
        SF2_HW_PKCS11_CHECKRESULT( EncryptDecryptInit(&pEnc->SymmetricCtx, (const UINT8*)pEnc->Key->key,
        		pEnc->Key->size, pEnc->IV, ivSize, isEncrypt, cipherType, mode ) );

        memcpy(pEnc->Key,&tmpKey, sizeof(KEY_DATA));
        //TODO: Mukundan: Not sure how to do padding
        //SF2_HW_PKCS11_CHECKRESULT(EVP_CIPHER_CTX_set_padding(&pEnc->SymmetricCtx, padding));
    }
    else
    {
        //This is for asymettric encryption, we will do this a bit later
    	/*
    	pEnc->Key->ctx = EVP_PKEY_CTX_new((EVP_PKEY*)pEnc->Key->key, NULL);
        if(isEncrypt)
        {
            SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_encrypt_init       ((EVP_PKEY_CTX*)pEnc->Key->ctx         ));
            SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_CTX_set_rsa_padding((EVP_PKEY_CTX*)pEnc->Key->ctx, padding));
        }
        else
        {
            SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_decrypt_init       ((EVP_PKEY_CTX*)pEnc->Key->ctx         ));
            SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_CTX_set_rsa_padding((EVP_PKEY_CTX*)pEnc->Key->ctx, padding));
        }
        */
    	SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);

    }

    if(isEncrypt) pSessionCtx->EncryptionCtx = pEnc;
    else          pSessionCtx->DecryptionCtx = pEnc;

    SF2_HW_PKCS11_CLEANUP();
    if(retVal != CKR_OK && pEnc != NULL)
    {
    	FreeCyptoData(pEnc);
    }
    SF2_HW_PKCS11_RETURN();
}



CK_RV SF2_HW_PKCS11_Encryption::EncryptInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pEncryptMech, CK_OBJECT_HANDLE hKey)
{
	return InitHelper(pSessionCtx, pEncryptMech, hKey, TRUE);
}

CK_RV SF2_HW_PKCS11_Encryption::Encrypt(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen)
{
	SF2_HW_PKCS11_HEADER();

	if(pEncryptedData == NULL)
	{
		SF2_HW_EncryptData *pEnc;
		//int blockSize;
		int mod;

		if(pSessionCtx == NULL || pSessionCtx->EncryptionCtx == NULL) return CKR_SESSION_CLOSED;

		pEnc = (SF2_HW_EncryptData*)pSessionCtx->EncryptionCtx;

		/*if(pEnc->IsSymmetric)
		{
			blockSize = mbedtls_cipher_get_block_size(&pEnc->SymmetricCtx);
		}
		else
		{
			blockSize = mbedtls_cipher_get_block_size(&pEnc->SymmetricCtx);

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
		*/

		return CKR_OK;
	}

	CK_ULONG tmp = *pulEncryptedDataLen;

	SF2_HW_PKCS11_CHECK_CK_RESULT(MBEDTLS_PKCS11_Encryption::EncryptUpdate(pSessionCtx, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen));

	tmp -= *pulEncryptedDataLen;

	SF2_HW_PKCS11_CHECK_CK_RESULT(MBEDTLS_PKCS11_Encryption::EncryptFinal(pSessionCtx, &pEncryptedData[*pulEncryptedDataLen], &tmp));

	*pulEncryptedDataLen += tmp;

	SF2_HW_PKCS11_NOCLEANUP();

}

CK_RV SF2_HW_PKCS11_Encryption::EncryptUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen)
{
    SF2_HW_PKCS11_HEADER();
    SF2_HW_EncryptData *pEnc;

    if(pSessionCtx == NULL || pSessionCtx->EncryptionCtx == NULL) return CKR_SESSION_CLOSED;

    pEnc = (SF2_HW_EncryptData*)pSessionCtx->EncryptionCtx;
    //mbedtls_cipher_context_t * cipher_ctx = (mbedtls_cipher_context_t *)pEnc->Key->ctx;
    mbedtls_cipher_context_t * cipher_ctx = (mbedtls_cipher_context_t *) &pEnc->SymmetricCtx;

    hal_printf("Crypto specific ctx ptr: %p \n", cipher_ctx->cipher_ctx );

    if(pEnc->IsSymmetric)
    {
        size_t outLen = *pulEncryptedPartLen;
        SF2_HW_PKCS11_CHECKRESULT( mbedtls_cipher_update (cipher_ctx,  pPart, ulPartLen, pEncryptedPart, &outLen));
        *pulEncryptedPartLen = outLen;
    }
    else
    {
    	//size_t encLen = *pulEncryptedPartLen;
        //SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_encrypt((EVP_PKEY_CTX*)pEnc->Key->ctx, pEncryptedPart, &encLen, pPart, ulPartLen));
        //*pulEncryptedPartLen = encLen;
    	//We dont support asymetric at this time.
    	SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
    }

    SF2_HW_PKCS11_CLEANUP();

    if(retVal != CKR_OK)
    {
    	FreeCyptoData(pEnc);
        pSessionCtx->EncryptionCtx = NULL;
    }

    SF2_HW_PKCS11_RETURN();
}

CK_RV SF2_HW_PKCS11_Encryption::EncryptFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pLastEncryptedPart, CK_ULONG_PTR pulLastEncryptedPartLen)
{
    SF2_HW_PKCS11_HEADER();
    SF2_HW_EncryptData *pEnc;

    if(pSessionCtx == NULL || pSessionCtx->EncryptionCtx == NULL) return CKR_SESSION_CLOSED;

    pEnc = (SF2_HW_EncryptData*)pSessionCtx->EncryptionCtx;

    if(pEnc->IsSymmetric)
    {
        size_t outLen = *pulLastEncryptedPartLen;
        SF2_HW_PKCS11_CHECKRESULT(mbedtls_cipher_finish((mbedtls_cipher_context_t *)pEnc->Key->ctx, pLastEncryptedPart, &outLen));
        *pulLastEncryptedPartLen = outLen;

       // SF2_HW_PKCS11_CHECKRESULT(mbedtls_cipher_reset((mbedtls_cipher_context_t *)pEnc->Key->ctx));
    }
    else
    {
        //EVP_PKEY_CTX_free((EVP_PKEY_CTX*)pEnc->Key->ctx);
        //*pulLastEncryptedPartLen = 0;
        //We dont support asymetric at this time.
        SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
    }

    SF2_HW_PKCS11_CLEANUP();

    FreeCyptoData(pEnc);
    pSessionCtx->EncryptionCtx = NULL;

    SF2_HW_PKCS11_RETURN();
}    

CK_RV SF2_HW_PKCS11_Encryption::DecryptInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pDecryptMech, CK_OBJECT_HANDLE hKey)
{
	return InitHelper(pSessionCtx, pDecryptMech, hKey, FALSE);
}

CK_RV SF2_HW_PKCS11_Encryption::Decrypt(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}    

CK_RV SF2_HW_PKCS11_Encryption::DecryptUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}    

CK_RV SF2_HW_PKCS11_Encryption::DecryptFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}    


