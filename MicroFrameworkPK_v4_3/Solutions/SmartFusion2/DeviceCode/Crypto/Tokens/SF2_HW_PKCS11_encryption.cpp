#include "SF2_HW_pkcs11.h"
//

int EncryptDecryptInit(sf2_cipher_context_t *cipher_ctx, const unsigned char *key,
        int key_bitlen, const unsigned char *iv, size_t iv_len, BOOL _encrypt,
		SF2_CipherType type, uint8_t _mode)
{
    int ret=0;
    //const sf2_cipher_info_t *info = cipher_ctx->cipher_info;
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
	//sf2_cipher_free(cipher_ctx);
	//SF2_HW_PKCS11_FREE()
	SF2_HW_PKCS11_FREE(pEnc);
	return 0;
}

CK_RV  SF2_HW_PKCS11_Encryption::InitHelper(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pEncryptMech, CK_OBJECT_HANDLE hKey, BOOL isEncrypt)
{
    SF2_HW_PKCS11_HEADER();

    SF2_HW_EncryptData* pEnc;
    SF2_CipherType cipherType;
    uint8_t mode=0;
   // const sf2_cipher_info_t* pCipherInfo;
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
            mode = mode| CBC_MASK;
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
            mode = mode| ECB_MASK;
            if(pEncryptMech->mechanism == CKM_AES_ECB_PAD)
            {
                padding = 1;
            }
            break;
        default:
            SF2_HW_PKCS11_SET_AND_LEAVE(CKR_MECHANISM_INVALID);
    }

    pEnc->SymmetricCtx.type = cipherType;

    if(!isEncrypt){mode = mode| DECRYPT_MASK;}

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
		int blockSize;
		int mod;

		if(pSessionCtx == NULL || pSessionCtx->EncryptionCtx == NULL) return CKR_SESSION_CLOSED;

		pEnc = (SF2_HW_EncryptData*)pSessionCtx->EncryptionCtx;

		if(pEnc->IsSymmetric)
		{
			blockSize = SF2_GetBlockSize(&pEnc->SymmetricCtx);
		}
		else
		{
			blockSize = SF2_GetBlockSize(&pEnc->SymmetricCtx);

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

	SF2_HW_PKCS11_CHECK_CK_RESULT(SF2_HW_PKCS11_Encryption::EncryptUpdate(pSessionCtx, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen));

	tmp -= *pulEncryptedDataLen;

	SF2_HW_PKCS11_CHECK_CK_RESULT(SF2_HW_PKCS11_Encryption::EncryptFinal(pSessionCtx, &pEncryptedData[*pulEncryptedDataLen], &tmp));

	*pulEncryptedDataLen += tmp;

	SF2_HW_PKCS11_NOCLEANUP();
}

CK_RV SF2_HW_PKCS11_Encryption::EncryptUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen)
{
    SF2_HW_PKCS11_HEADER();
    SF2_HW_EncryptData *pEnc;

    if(pSessionCtx == NULL || pSessionCtx->EncryptionCtx == NULL) return CKR_SESSION_CLOSED;

    pEnc = (SF2_HW_EncryptData*)pSessionCtx->EncryptionCtx;
    //mbedtls_cipher_context_t * cipher_ctx = (sf2_cipher_context_t *)pEnc->Key->ctx;
    sf2_cipher_context_t * cipher_ctx = (sf2_cipher_context_t *) &pEnc->SymmetricCtx;

    //hal_printf("Crypto specific ctx ptr: %p \n", cipher_ctx );

    if(pEnc->IsSymmetric)
    {
        uint32_t outLen = *pulEncryptedPartLen;
        SF2_HW_PKCS11_CHECKRESULT( SF2_Cipher(cipher_ctx,  pPart, ulPartLen, pEncryptedPart, &outLen ));
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

    //We dont have the concept of EncryptFinal in SF2 aes. Hence, this is just a dummy call, no actual encryption

    if(pSessionCtx == NULL || pSessionCtx->EncryptionCtx == NULL) return CKR_SESSION_CLOSED;

    pEnc = (SF2_HW_EncryptData*)pSessionCtx->EncryptionCtx;

    if(pEnc->IsSymmetric)
    {
        size_t outLen = *pulLastEncryptedPartLen;
        //SF2_HW_PKCS11_CHECKRESULT(SF2_Cipher((sf2_cipher_context_t *)pEnc->Key->ctx, pLastEncryptedPart));
        //*pulLastEncryptedPartLen = outLen;
    	if(outLen > 0){
    		hal_printf("This shouldnt the case, something seems wrong\n");
    	}

    	//We dont have the concept of EncryptFinal in SF2 aes. Hence This should never be called.
        *pulLastEncryptedPartLen = 0;
       // SF2_HW_PKCS11_CHECKRESULT(sf2_cipher_reset((sf2_cipher_context_t *)pEnc->Key->ctx));
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
	SF2_HW_PKCS11_HEADER();

	if(pData == NULL)
	{
		SF2_HW_EncryptData *pDecr;
		int blockSize;
		int mod;

		if(pSessionCtx == NULL || pSessionCtx->DecryptionCtx == NULL) return CKR_SESSION_CLOSED;

		pDecr = (SF2_HW_EncryptData*)pSessionCtx->DecryptionCtx;

		if(pDecr->IsSymmetric)
		{
			blockSize =  SF2_GetBlockSize(&pDecr->SymmetricCtx);
		}
		else
		{
			//Not sure if this is right. But just keep it same
			blockSize =  SF2_GetBlockSize(&pDecr->SymmetricCtx);
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

	SF2_HW_PKCS11_CHECK_CK_RESULT(SF2_HW_PKCS11_Encryption::DecryptUpdate(pSessionCtx, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen));

	tmp -= *pulDataLen;

	SF2_HW_PKCS11_CHECK_CK_RESULT(SF2_HW_PKCS11_Encryption::DecryptFinal(pSessionCtx, &pData[*pulDataLen], &tmp));

	*pulDataLen += tmp;

	SF2_HW_PKCS11_NOCLEANUP();

}    

CK_RV SF2_HW_PKCS11_Encryption::DecryptUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen)
{
    SF2_HW_PKCS11_HEADER();

    SF2_HW_EncryptData *pDec;

    if(pSessionCtx == NULL || pSessionCtx->DecryptionCtx == NULL) return CKR_SESSION_CLOSED;

    pDec = (SF2_HW_EncryptData*)pSessionCtx->DecryptionCtx;
    sf2_cipher_context_t *cipher_ctx = &(pDec->SymmetricCtx);

    if(pDec->IsSymmetric)
    {
        uint32_t outLen = *pulPartLen;
        SF2_HW_PKCS11_CHECKRESULT( SF2_Cipher(cipher_ctx, pEncryptedPart, ulEncryptedPartLen, pPart, &outLen));
        *pulPartLen = outLen;
    }
    else
    {
        //size_t outLen = *pulPartLen;
        //SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_decrypt((EVP_PKEY_CTX*)pDec->Key->ctx, pPart, &outLen, pEncryptedPart, ulEncryptedPartLen));
        //*pulPartLen = outLen;
    	 SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
    }

    SF2_HW_PKCS11_CLEANUP();

    if(retVal != CKR_OK)
    {
    	FreeCyptoData(pDec);
        pSessionCtx->DecryptionCtx = NULL;
    }

    SF2_HW_PKCS11_RETURN();
}    

CK_RV SF2_HW_PKCS11_Encryption::DecryptFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen)
{
    SF2_HW_PKCS11_HEADER();

    SF2_HW_EncryptData *pDec;

    if(pSessionCtx == NULL || pSessionCtx->DecryptionCtx == NULL) return CKR_SESSION_CLOSED;

    pDec = (SF2_HW_EncryptData*)pSessionCtx->DecryptionCtx;
    sf2_cipher_context_t *cipher_ctx = &(pDec->SymmetricCtx);

    if(pDec->IsSymmetric)
    {
    	*pulLastPartLen = 0;
        size_t outLen = *pulLastPartLen;
        if(outLen > 0){
        	hal_printf("Something wrong: SF2 crypto does not have the concept of DecryptFinal \n");
        }
        //SF2_HW_PKCS11_CHECKRESULT(SF2_Cipher((sf2_cipher_context_t *)pDec->Key->ctx, pLastPart, &outLen));
        //*pulLastPartLen = outLen;
        //SF2_HW_PKCS11_CHECKRESULT(SF2_CipherReset( cipher_ctx));
    }
    else
    {
        //EVP_PKEY_CTX_free((EVP_PKEY_CTX*)pDec->Key->ctx);
        //*pulLastPartLen = 0;
    	SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
    }

    SF2_HW_PKCS11_CLEANUP();

    FreeCyptoData(pDec);
    pSessionCtx->DecryptionCtx = NULL;

    SF2_HW_PKCS11_RETURN();
}    


