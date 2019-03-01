#include "SF2_HW_pkcs11.h"
#include "../SF2_HW_Crypto.h"

const UINT16 KEY_DATA_SIZE=((256+7)/8+sizeof(KEY_DATA));

UINT8 DummyKeyArray[MAX_KEY_OBJECT_COUNT][KEY_DATA_SIZE];
UINT8 KeyArray[MAX_KEY_OBJECT_COUNT][KEY_DATA_SIZE];
bool KeyArrayUse[MAX_KEY_OBJECT_COUNT];

void InitKeyArray(){
	memset(KeyArray, 0, KEY_DATA_SIZE*MAX_KEY_OBJECT_COUNT);
	memset(KeyArrayUse, 0, MAX_KEY_OBJECT_COUNT);
}

int FindEmptyKeyIndex(){
	int ret= -1;
	for (int i=0 ; i< MAX_KEY_OBJECT_COUNT; i++)
	{
		if(KeyArrayUse[i]==0){
			KeyArrayUse[i]=1;
			ret = i;
			break;
		}
	}
	return ret;
}

CK_RV AllocateKeyData(KEY_DATA **pKey){
	int x=FindEmptyKeyIndex();
	if(x<0){
		return CKR_DEVICE_MEMORY;
	}
	else {
		//*phKey = (CK_OBJECT_HANDLE) KeyArray[x];
		//*phKey = (CK_OBJECT_HANDLE) x+PKCS11_MBEDTLS_MAX_HEAP_OBJECT_COUNT;
		//pData = (OBJECT_DATA*)KeyArray[x];
		*pKey = (KEY_DATA*)&(KeyArray[x][0]);
		//(*pKey)->arrayIndex =x;
		return CKR_OK;
	}
}

CK_RV FreeKeyData(KEY_DATA *pKey){
	UINT8 x= MAX_KEY_OBJECT_COUNT;
	for (int i=0 ; i< MAX_KEY_OBJECT_COUNT; i++)
	{
		if(pKey == (KEY_DATA *)KeyArray[i]){
			x = i;
			break;
		}
	}

	if (x >= MAX_KEY_OBJECT_COUNT){
		return CKR_ARGUMENTS_BAD;
	}
	KeyArrayUse[x]=0;
	memset(KeyArray[x], 0, KEY_DATA_SIZE);
	return CKR_OK;
}

CK_RV SF2_HW_PKCS11_Keys::DeleteKey(Cryptoki_Session_Context* pSessionCtx, KEY_DATA* pKey)
{
    if(pKey == NULL) return CKR_OBJECT_HANDLE_INVALID;

    switch(pKey->attrib)
    {
        case Secret:
        	break;
        default:
        	SF2_HW_PKCS11_FREE((sf2_ec_key_t*)pKey->key);
            break;
    }
    //FreeKeyData(pKey);
    return CKR_OK;
}

KEY_DATA* SF2_HW_PKCS11_Keys::GetKeyFromHandle(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hKey, BOOL getPrivate)
{
    OBJECT_DATA* pObj = SF2_HW_PKCS11_Objects::GetObjectFromHandle(pSessionCtx, hKey);

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


CK_RV SF2_HW_PKCS11_Keys::GenerateKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phKey)
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

				*phKey = SF2_HW_PKCS11_Objects::AllocObject(pSessionCtx, KeyType, (len+7)/8 + sizeof(KEY_DATA), &pData);

				/*Todo::Mukundan:: The following a hack to fix a problem with vanish Key data.
				//This needs to be relooked
				int x=FindEmptyKeyIndex();
				if(x<0){
					return CKR_DEVICE_MEMORY;
				}
				else {
					//*phKey = (CK_OBJECT_HANDLE) KeyArray[x];
					*phKey = (CK_OBJECT_HANDLE) x+PKCS11_MBEDTLS_MAX_HEAP_OBJECT_COUNT;
					//pData = (OBJECT_DATA*)KeyArray[x];
					pKey = (KEY_DATA*)KeyArray[x];
				}*/

				if(*phKey == CK_OBJECT_HANDLE_INVALID) return CKR_DEVICE_MEMORY;

				pKey = (KEY_DATA*)pData->Data;

				//pKey->arrayIndex = x;

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

CK_RV SF2_HW_PKCS11_Keys::GenerateKeyPair(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism,
                         CK_ATTRIBUTE_PTR pPublicKeyTemplate , CK_ULONG ulPublicCount, 
                         CK_ATTRIBUTE_PTR pPrivateKeyTemplate, CK_ULONG ulPrivateCount, 
                         CK_OBJECT_HANDLE_PTR phPublicKey    , CK_OBJECT_HANDLE_PTR phPrivateKey)
{
    SF2_HW_PKCS11_HEADER();
    //EVP_PKEY_CTX* ctx   = NULL;
    //EC_GROUP*     group = NULL;
    OBJECT_DATA*  pData = NULL;
    KEY_DATA*     pKey  = NULL;

    Watchdog_GetSetEnabled( FALSE, TRUE );

    switch(pMechanism->mechanism)
    {
        case CKM_EC_KEY_PAIR_GEN:
        case CKM_ECDH_KEY_PAIR_GEN:
            if(pPublicKeyTemplate != NULL && pPublicKeyTemplate->type == CKA_VALUE_LEN)
            {
                int len = SwapEndianIfBEc32(*(int*)pPublicKeyTemplate->pValue);

                *phPublicKey = SF2_HW_PKCS11_Objects::AllocObject(pSessionCtx, KeyType, sizeof(KEY_DATA), &pData);

                if(*phPublicKey == CK_OBJECT_HANDLE_INVALID) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_DEVICE_MEMORY);

                pKey = (KEY_DATA*)pData->Data;

                *phPrivateKey = *phPublicKey;


                if(len!=384)
                {
                    return CKR_KEY_SIZE_RANGE;
                }


                //if(NULL == (ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL))) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_FAILED);

                pKey->key = (sf2_ec_key_t*)SF2_HW_PKCS11_MALLOC(sizeof(sf2_ec_key_t));

                //SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_keygen_init(ctx));

                //SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_set1_EC_KEY(ctx->pkey, EC_KEY_new()));

                //SF2_HW_PKCS11_CHECKRESULT(EC_KEY_set_group(ctx->pkey->pkey.ec, group));

                SF2_HW_PKCS11_CHECKRESULT(SF2_ECC384_PKEY((sf2_ec_key_t*)pKey->key));

                pKey->type = CKK_EC;
                pKey->size = len;
                pKey->attrib = PrivatePublic;
                pKey->ctx = NULL;
            }
            break;

        default:
            return CKR_MECHANISM_INVALID;
    }

    SF2_HW_PKCS11_CLEANUP();

    Watchdog_GetSetEnabled( TRUE, TRUE );

    //if(ctx != NULL) EVP_PKEY_CTX_free(ctx);

    //if(group != NULL) EC_GROUP_free(group);

    if(retVal != CKR_OK)
    {
        if(pData != NULL)
        {
            SF2_HW_PKCS11_Objects::FreeObject(pSessionCtx, *phPublicKey);
            *phPublicKey = CK_OBJECT_HANDLE_INVALID;
            *phPrivateKey = CK_OBJECT_HANDLE_INVALID;
        }
    }

    SF2_HW_PKCS11_RETURN();
}

CK_RV SF2_HW_PKCS11_Keys::WrapKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hWrappingKey, CK_OBJECT_HANDLE hKey, CK_BYTE_PTR pWrappedKey, CK_ULONG_PTR pulWrappedKeyLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Keys::UnwrapKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hUnwrappingKey, CK_BYTE_PTR pWrappedKey, CK_ULONG ulWrappedKeyLen, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulAttributeCount, CK_OBJECT_HANDLE_PTR phKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

static BOOL ParseECParam(CK_VOID_PTR pParameter, CK_ULONG ulParamLen, CK_ECDH1_DERIVE_PARAMS& params)
{
    UINT8* pParam = (UINT8*)pParameter;

    memset(&params, 0, sizeof(params));

    params.kdf             = SwapEndianIfBEc32(*(UINT32*)pParam); pParam+=4;
    params.ulSharedDataLen = SwapEndianIfBEc32(*(UINT32*)pParam); pParam+=4;

    if(params.ulSharedDataLen > 0)
    {
        params.pSharedData = pParam; pParam += params.ulSharedDataLen;
    }
    params.ulPublicDataLen = SwapEndianIfBEc32(*(UINT32*)pParam); pParam+=4;

    if(params.ulPublicDataLen > 0)
    {
        params.pPublicData = pParam;
    }

    return (params.ulPublicDataLen + params.ulSharedDataLen + 3 * sizeof(UINT32)) <= ulParamLen;
}


CK_RV SF2_HW_PKCS11_Keys::DeriveKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hBaseKey, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulAttributeCount, CK_OBJECT_HANDLE_PTR phKey)
{
	SF2_HW_PKCS11_HEADER();
	//EVP_PKEY_CTX* ctx   = NULL;
	KEY_DATA* pKey = NULL;
	//EVP_PKEY* pPubKey;
	//EC_POINT* pPoint;

	UINT8  resultKey[1024/8];
	size_t keyBytes = 0;

	memset(resultKey, 0, sizeof(resultKey));

	switch(pMechanism->mechanism)
	{
		case CKM_ECDH1_DERIVE:
			{
				CK_ECDH1_DERIVE_PARAMS  params;
				EVP_PKEY*               pBaseKey;
				const EC_GROUP*         pGroup;
				const EVP_MD*           pDigest = NULL;
				UINT8 pubKey[66*2+1];

				if(!ParseECParam(pMechanism->pParameter, pMechanism->ulParameterLen, params)) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_MECHANISM_PARAM_INVALID);

				//ctx  = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL       ); if(ctx  == NULL) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_FAILED);
				pKey = GetKeyFromHandle(pSessionCtx, hBaseKey, TRUE); if(pKey == NULL) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_FAILED);

				if(pKey->type != CKK_EC) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_KEY_TYPE_INCONSISTENT);

				pBaseKey = (EVP_PKEY*)pKey->key;                    if(pBaseKey== NULL) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_FAILED);
				pGroup   = EC_KEY_get0_group(pBaseKey->pkey.ec);    if(pGroup  == NULL) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_FAILED);
				pPoint   = EC_POINT_new(pGroup);                    if(pPoint  == NULL) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_FAILED);
				pPubKey  = EVP_PKEY_new();                          if(pPubKey == NULL) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_FAILED);

				keyBytes = (EVP_PKEY_bits(pBaseKey) + 7) / 8;

				ctx->pkey = pBaseKey;
				SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_set1_EC_KEY(pPubKey, EC_KEY_new()));

				pubKey[0] = POINT_CONVERSION_UNCOMPRESSED;
				memcpy(&pubKey[1], params.pPublicData, params.ulPublicDataLen);

				SF2_HW_PKCS11_CHECKRESULT(EC_POINT_oct2point(pGroup, pPoint, (UINT8*)pubKey, params.ulPublicDataLen+1, NULL));

				SF2_HW_PKCS11_CHECKRESULT(EC_KEY_set_group(pPubKey->pkey.ec, pGroup));
				SF2_HW_PKCS11_CHECKRESULT(EC_KEY_set_public_key(pPubKey->pkey.ec, pPoint));

				SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_derive_init(ctx));
				SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_derive_set_peer(ctx, pPubKey));
				SF2_HW_PKCS11_CHECKRESULT(EVP_PKEY_derive(ctx, resultKey, &keyBytes));


				switch(params.kdf)
				{
					case CKM_SHA1_KEY_DERIVATION:
						pDigest = EVP_sha1();
						break;

					case CKM_SHA256_KEY_DERIVATION:
						pDigest = EVP_sha256();
						break;

					case CKM_SHA512_KEY_DERIVATION:
						pDigest = EVP_sha512();
						break;

					case CKM_MD5_KEY_DERIVATION:
						pDigest = EVP_md5();
						break;

					case CKM_NULL_KEY_DERIVATION:
						break;

					case CKM_SHA256_HMAC:
					case CKM_TLS_MASTER_KEY_DERIVE_DH:
					default:
						SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
				}

				if(pDigest != NULL)
				{
					UINT8 tmp[1024/8];
					EVP_MD_CTX pDigestCtx;

					SF2_HW_PKCS11_CHECKRESULT(EVP_DigestInit(&pDigestCtx, pDigest));
					EVP_MD_CTX_set_flags(&pDigestCtx,EVP_MD_CTX_FLAG_ONESHOT);
					SF2_HW_PKCS11_CHECKRESULT(EVP_DigestUpdate(&pDigestCtx, resultKey, keyBytes));
					SF2_HW_PKCS11_CHECKRESULT(EVP_DigestFinal(&pDigestCtx, tmp, (UINT32*)&keyBytes));
					SF2_HW_PKCS11_CHECKRESULT(EVP_MD_CTX_cleanup(&pDigestCtx));

					memcpy(resultKey, tmp, keyBytes);
				}

				*phKey = LoadKey(pSessionCtx, (void*)resultKey, CKK_GENERIC_SECRET, Secret, keyBytes * 8);
			}
			break;

		default:
			SF2_HW_PKCS11_SET_AND_LEAVE(CKR_MECHANISM_INVALID);
	}

	SF2_HW_PKCS11_CLEANUP();
	if(ctx != NULL)
	{
		ctx->pkey = NULL;
		EVP_PKEY_CTX_free(ctx);
	}
	if(pPubKey != NULL)
	{
		EVP_PKEY_free(pPubKey);
	}
	if(pPoint != NULL)
	{
		EC_POINT_free(pPoint);
	}
	SF2_HW_PKCS11_RETURN();
}

CK_RV SF2_HW_PKCS11_Keys::LoadKeyBlob(Cryptoki_Session_Context* pSessionCtx, const PBYTE pKey, CK_ULONG keyLen, CK_KEY_TYPE keyType, KEY_ATTRIB keyAttrib, CK_OBJECT_HANDLE_PTR phKey )
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Keys::LoadSecretKey(Cryptoki_Session_Context* pSessionCtx, CK_KEY_TYPE keyType, const UINT8* pKey, CK_ULONG ulKeyLength, CK_OBJECT_HANDLE_PTR phKey)
{
	SF2_HW_PKCS11_HEADER();
	*phKey = LoadKey(pSessionCtx, (void*)pKey, keyType, Secret, ulKeyLength * 8);
	if(*phKey == CK_OBJECT_HANDLE_INVALID) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_FAILED);
	memcpy(DummyKeyArray, KeyArray, KEY_DATA_SIZE*MAX_KEY_OBJECT_COUNT);

	SF2_HW_PKCS11_NOCLEANUP();
}

CK_RV SF2_HW_PKCS11_Keys::LoadRsaKey(Cryptoki_Session_Context* pSessionCtx, const RsaKeyData& keyData, CK_BBOOL isPrivate, CK_OBJECT_HANDLE_PTR phKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Keys::LoadDsaKey(Cryptoki_Session_Context* pSessionCtx, const DsaKeyData& keyData, CK_BBOOL isPrivate, CK_OBJECT_HANDLE_PTR phKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Keys::LoadEcKey(Cryptoki_Session_Context* pSessionCtx, const EcKeyData&  keyData, CK_BBOOL isPrivate, CK_OBJECT_HANDLE_PTR phKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_OBJECT_HANDLE SF2_HW_PKCS11_Keys::LoadKey(Cryptoki_Session_Context* pSessionCtx, void* pKey, CK_KEY_TYPE type, KEY_ATTRIB attrib, size_t keySize)
{
    CK_OBJECT_HANDLE retVal;
    OBJECT_DATA* pData = NULL;
    KEY_DATA* pKeyData = NULL;
    BOOL fCopyKey = false;

    switch(attrib)
    {
        case Secret:
            retVal = SF2_HW_PKCS11_Objects::AllocObject(pSessionCtx, KeyType, (keySize+7)/8 + sizeof(KEY_DATA), &pData);
            fCopyKey = true;
            break;

        default:
            retVal = SF2_HW_PKCS11_Objects::AllocObject(pSessionCtx, KeyType, sizeof(KEY_DATA), &pData);
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

