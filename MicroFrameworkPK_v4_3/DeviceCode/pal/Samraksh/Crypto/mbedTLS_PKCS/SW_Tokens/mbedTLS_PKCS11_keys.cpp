#include "mbedTLS_PKCS11.h"
#include "../../RandomUtils.h"

const UINT16 KEY_DATA_SIZE=((256+7)/8+sizeof(KEY_DATA));
#define MAX_KEYS 4
UINT8 KeyArray[MAX_KEYS][KEY_DATA_SIZE];
BOOL KeyArrayUse[MAX_KEYS];

int FindEmptyKeyIndex(){
	int ret= -1;
	for (int i=0 ; i< MAX_KEYS; i++)
	{
		if(KeyArrayUse[i]==0){
			ret = i;
			break;
		}
	}
	return ret;
}

CK_RV MBEDTLS_PKCS11_Keys::DeleteKey(Cryptoki_Session_Context* pSessionCtx, KEY_DATA* pKey)
{
    if(pKey == NULL) return CKR_OBJECT_HANDLE_INVALID;
    switch(pKey->attrib)
    {
        case Secret:
        default:
            private_free(pKey->key);
            break;
    }
    return CKR_OK;
}

KEY_DATA* MBEDTLS_PKCS11_Keys::GetKeyFromHandle(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hKey, BOOL getPrivate)
{
    OBJECT_DATA* pObj = MBEDTLS_PKCS11_Objects::GetObjectFromHandle(pSessionCtx, hKey);

    if(pObj == NULL) return NULL;

    if(pObj->Type == KeyType)
    {
        return (KEY_DATA*)pObj->Data;
    }
    else if(pObj->Type == CertificateType)
    {
        CERT_DATA* pCert = (CERT_DATA*)pObj->Data;

        return getPrivate ? &pCert->privKeyData : &pCert->pubKeyData;
    }

    return NULL;
}


CK_RV MBEDTLS_PKCS11_Keys::GenerateKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phKey)
{
	 switch(pMechanism->mechanism)
	    {
	        case CKM_DES3_KEY_GEN:
	        case CKM_AES_KEY_GEN:
	        case CKM_GENERIC_SECRET_KEY_GEN:
	            if(pTemplate != NULL && pTemplate->type == CKA_VALUE_LEN)
	            {
	                int len = SwapEndianIfBEc32(*(int*)pTemplate->pValue);
	                OBJECT_DATA* pData = NULL;
	                KEY_DATA* pKey = NULL;

	                *phKey = MBEDTLS_PKCS11_Objects::AllocObject(pSessionCtx, KeyType, (len+7)/8 + sizeof(KEY_DATA), &pData);
	                /*
	                //Todo::Mukundan:: The following a hack to fix a problem with vanish Key data.
	                //This needs to be relooked
	                int x=FindEmptyKeyIndex();
	                if(x<0){
	                	return CKR_DEVICE_MEMORY;
	                }
	                else {
	                	*phKey = (CK_OBJECT_HANDLE) KeyArray[x];
	                }
					*/
	                if(*phKey == CK_OBJECT_HANDLE_INVALID) return CKR_DEVICE_MEMORY;

	                pKey = (KEY_DATA*)pData->Data;

	                //pKey->ArrayIndex = x;

	                GetRandomBytes((UINT8*)&pKey[1], len/8);

	                switch(pMechanism->mechanism)
	                {
	                    case CKM_AES_KEY_GEN:
	                        pKey->type = CKK_AES;
	                        break;
	                    case CKM_DES3_KEY_GEN:
	                        pKey->type = CKK_DES3;
	                        break;
	                    case CKM_GENERIC_SECRET_KEY_GEN:
	                        pKey->type = CKK_GENERIC_SECRET;
	                        break;
	                }
	                pKey->size = len;
	                pKey->key = (void*)&pKey[1];
	                pKey->ctx = NULL;
	                pKey->attrib = Secret;
	            }
	            else
	            {
	                return CKR_TEMPLATE_INCOMPLETE;
	            }
	            break;

	        default:
	            return CKR_MECHANISM_INVALID;
	    }

	    return CKR_OK;
}

CK_RV MBEDTLS_PKCS11_Keys::GenerateKeyPair(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism,
                         CK_ATTRIBUTE_PTR pPublicKeyTemplate , CK_ULONG ulPublicCount, 
                         CK_ATTRIBUTE_PTR pPrivateKeyTemplate, CK_ULONG ulPrivateCount, 
                         CK_OBJECT_HANDLE_PTR phPublicKey    , CK_OBJECT_HANDLE_PTR phPrivateKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Keys::WrapKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hWrappingKey, CK_OBJECT_HANDLE hKey, CK_BYTE_PTR pWrappedKey, CK_ULONG_PTR pulWrappedKeyLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Keys::UnwrapKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hUnwrappingKey, CK_BYTE_PTR pWrappedKey, CK_ULONG ulWrappedKeyLen, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulAttributeCount, CK_OBJECT_HANDLE_PTR phKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Keys::DeriveKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hBaseKey, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulAttributeCount, CK_OBJECT_HANDLE_PTR phKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Keys::LoadKeyBlob(Cryptoki_Session_Context* pSessionCtx, const PBYTE pKey, CK_ULONG keyLen, CK_KEY_TYPE keyType, KEY_ATTRIB keyAttrib, CK_OBJECT_HANDLE_PTR phKey )
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Keys::LoadSecretKey(Cryptoki_Session_Context* pSessionCtx, CK_KEY_TYPE keyType, const UINT8* pKey, CK_ULONG ulKeyLength, CK_OBJECT_HANDLE_PTR phKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Keys::LoadRsaKey(Cryptoki_Session_Context* pSessionCtx, const RsaKeyData& keyData, CK_BBOOL isPrivate, CK_OBJECT_HANDLE_PTR phKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Keys::LoadDsaKey(Cryptoki_Session_Context* pSessionCtx, const DsaKeyData& keyData, CK_BBOOL isPrivate, CK_OBJECT_HANDLE_PTR phKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Keys::LoadEcKey(Cryptoki_Session_Context* pSessionCtx, const EcKeyData&  keyData, CK_BBOOL isPrivate, CK_OBJECT_HANDLE_PTR phKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_OBJECT_HANDLE MBEDTLS_PKCS11_Keys::LoadKey(Cryptoki_Session_Context* pSessionCtx, void* pKey, CK_KEY_TYPE type, KEY_ATTRIB attrib, size_t keySize)
{
    CK_OBJECT_HANDLE retVal;
    OBJECT_DATA* pData = NULL;
    KEY_DATA* pKeyData = NULL;
    BOOL fCopyKey = false;

    switch(attrib)
    {
        case Secret:
            retVal = MBEDTLS_PKCS11_Objects::AllocObject(pSessionCtx, KeyType, (keySize+7)/8 + sizeof(KEY_DATA), &pData);
            fCopyKey = true;
            break;

        default:
            retVal = MBEDTLS_PKCS11_Objects::AllocObject(pSessionCtx, KeyType, sizeof(KEY_DATA), &pData);
            break;
    }

    if(retVal != CK_OBJECT_HANDLE_INVALID)
    {
        pKeyData = (KEY_DATA*)pData->Data;

        if(fCopyKey)
        {
            memcpy( &pKeyData[1], pKey, (keySize+7)/8 );
            pKeyData->key = &pKeyData[1];
        }
        else
        {
            pKeyData->key = pKey;
        }

        pKeyData->type   = type;
        pKeyData->size   = keySize;
        pKeyData->attrib = attrib;
        pKeyData->ctx    = NULL;
    }

    return retVal;
}

