#include "mbedTLS_PKCS11.h"
#include <mbedtls/cipher.h>

CK_RV  MBEDTLS_PKCS11_Encryption::InitHelper(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pEncryptMech, CK_OBJECT_HANDLE hKey, BOOL isEncrypt)
{
    PKCS11_MBEDTLS_HEADER();

    mbedTLSEncryptData* pEnc;
    const mbedtls_cipher_info_t* pCipherInfo;
    int padding = 0;

    if(             pSessionCtx                 == NULL) return CKR_SESSION_CLOSED;
    if( isEncrypt && pSessionCtx->EncryptionCtx != NULL) return CKR_SESSION_PARALLEL_NOT_SUPPORTED;
    if(!isEncrypt && pSessionCtx->DecryptionCtx != NULL) return CKR_SESSION_PARALLEL_NOT_SUPPORTED;

    pEnc = (mbedTLSEncryptData*)private_malloc(sizeof(*pEnc));

    if(pEnc == NULL) return CKR_DEVICE_MEMORY;

    memset(pEnc, 0, sizeof(*pEnc));

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

    /*if(pEnc->IsSymmetric)
    {
        if(pEncryptMech->ulParameterLen > 0 && pEncryptMech->ulParameterLen > 0)
        {
            memcpy(pEnc->IV, pEncryptMech->pParameter, pEncryptMech->ulParameterLen);
        }

        pEnc->Key->ctx = &pEnc->SymmetricCtx;

        if(isEncrypt)
        {
            PKCS11_MBEDTLS_CHECKRESULT(EVP_EncryptInit(&pEnc->SymmetricCtx, pCipherInfo, (const UINT8*)pEnc->Key->key, pEnc->IV));
        }
        else
        {
            PKCS11_MBEDTLS_CHECKRESULT(EVP_DecryptInit(&pEnc->SymmetricCtx, pCipher, (const UINT8*)pEnc->Key->key, pEnc->IV));
        }

        PKCS11_MBEDTLS_CHECKRESULT(EVP_CIPHER_CTX_set_padding(&pEnc->SymmetricCtx, padding));
    }
    else
    {
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
    }

    if(isEncrypt) pSessionCtx->EncryptionCtx = pEnc;
    else          pSessionCtx->DecryptionCtx = pEnc;
*/

    PKCS11_MBEDTLS_CLEANUP();
    if(retVal != CKR_OK && pEnc != NULL)
    {
        private_free(pEnc);
    }

    PKCS11_MBEDTLS_RETURN();

}

CK_RV MBEDTLS_PKCS11_Encryption::EncryptInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pEncryptMech, CK_OBJECT_HANDLE hKey)
{

	//mbedtls_cipher_init( &cipher_ctx );
	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Encryption::Encrypt(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Encryption::EncryptUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Encryption::EncryptFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pLastEncryptedPart, CK_ULONG_PTR pulLastEncryptedPartLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}    

CK_RV MBEDTLS_PKCS11_Encryption::DecryptInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pDecryptMech, CK_OBJECT_HANDLE hKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Encryption::Decrypt(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}    

CK_RV MBEDTLS_PKCS11_Encryption::DecryptUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}    

CK_RV MBEDTLS_PKCS11_Encryption::DecryptFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}    


