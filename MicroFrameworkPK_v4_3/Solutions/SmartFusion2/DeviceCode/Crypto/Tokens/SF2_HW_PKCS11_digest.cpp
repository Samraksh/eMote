#include "SF2_HW_PKCS11.h"


int HMAC_Init(sf2_digest_context_t *ctx, const void *key, int key_bitlen){
	//pDigData->HmacCtx
	ctx->key_bitlen=256; //we support only sha256 based hmac
	memcpy(ctx->key,key,key_bitlen/8);
	return 0;
}

CK_RV SF2_HW_PKCS11_Digest::DigestInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism)
{
	SF2_HW_PKCS11_HEADER();

	SF2_HW_DigestData* pDigData;
	//const EVP_MD*      pDigest;
	CK_OBJECT_HANDLE   hKey   = CK_OBJECT_HANDLE_INVALID;
	bool               isHMAC = false;

	if(pSessionCtx            == NULL) return CKR_SESSION_CLOSED;
	if(pSessionCtx->DigestCtx != NULL) return CKR_SESSION_PARALLEL_NOT_SUPPORTED; // another digest is in progress

	pDigData = (SF2_HW_DigestData*)SF2_HW_PKCS11_MALLOC(sizeof(*pDigData));

	if(pDigData == NULL) return CKR_DEVICE_MEMORY;

	memset(pDigData, 0, sizeof(*pDigData));

	//EVP_MD_CTX_init(&pDigData->CurrentCtx);

	switch(pMechanism->mechanism)
	{
		case CKM_SHA256_HMAC:
			//pDigest = EVP_sha256();
			isHMAC = true;
			break;
		default:
			SF2_HW_PKCS11_SET_AND_LEAVE(CKR_MECHANISM_INVALID);
	}

	if(isHMAC)
	{
		if(pMechanism->pParameter != NULL && pMechanism->ulParameterLen == sizeof(CK_OBJECT_HANDLE))
		{
			hKey = SwapEndianIfBEc32(*(CK_OBJECT_HANDLE*)pMechanism->pParameter);
		}
		else
		{
			SF2_HW_PKCS11_SET_AND_LEAVE(CKR_MECHANISM_PARAM_INVALID);
		}

		pDigData->HmacKey = SF2_HW_PKCS11_Keys::GetKeyFromHandle(pSessionCtx, hKey, TRUE);

		if(pDigData->HmacKey==NULL) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_MECHANISM_PARAM_INVALID);

		//pDigData->HmacCtx.md = pDigest;

		SF2_HW_PKCS11_CHECKRESULT(HMAC_Init(&pDigData->HmacCtx, pDigData->HmacKey->key, pDigData->HmacKey->size));
	}
	else
	{
		SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
	}

	pSessionCtx->DigestCtx = pDigData;

	SF2_HW_PKCS11_CLEANUP();
	if(retVal != CKR_OK && pDigData != NULL)
	{
		SF2_HW_PKCS11_FREE(pDigData);
		pSessionCtx->DigestCtx = NULL;
	}
	SF2_HW_PKCS11_RETURN();

}

CK_RV SF2_HW_PKCS11_Digest::Digest(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
	SF2_HW_PKCS11_HEADER();

	UINT32 digestLen = *pulDigestLen;
	SF2_HW_DigestData* pDigData;

	if(pSessionCtx == NULL || pSessionCtx->DigestCtx == NULL) return CKR_SESSION_CLOSED;

	pDigData = (SF2_HW_DigestData*)pSessionCtx->DigestCtx;
	pDigData->pDigest= pDigest;
	pDigData->pulDigestLen = pulDigestLen;

	if(pDigData->HmacKey != NULL)
	{
		uint32_t digestLen = *pulDigestLen;
		SF2_HW_PKCS11_CHECKRESULT(SF2_Digest(&pDigData->HmacCtx, pData  , ulDataLen, pDigest, &digestLen));
		//SF2_HW_PKCS11_CHECKRESULT(HMAC_Final (&pDigData->HmacCtx, pDigest, &digestLen));
	}
	else
	{
		SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
	}

	*pulDigestLen = digestLen;

	SF2_HW_PKCS11_CLEANUP();

	SF2_HW_PKCS11_FREE(pDigData);
	pSessionCtx->DigestCtx = NULL;

	SF2_HW_PKCS11_RETURN();

}

CK_RV SF2_HW_PKCS11_Digest::Update(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen)
{
	SF2_HW_PKCS11_HEADER();

	SF2_HW_DigestData* pDigData;

	if(pSessionCtx == NULL || pSessionCtx->DigestCtx == NULL) return CKR_SESSION_CLOSED;

	pDigData = (SF2_HW_DigestData*)pSessionCtx->DigestCtx;

	if(pDigData->HmacKey != NULL)
	{
		uint32_t digestLen = *pDigData->pulDigestLen;
		SF2_HW_PKCS11_CHECKRESULT(SF2_Digest(&pDigData->HmacCtx, pData  , ulDataLen, pDigData->pDigest, &digestLen));
	}
	else
	{
		SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
	}

	SF2_HW_PKCS11_CLEANUP();

	if(retVal != CKR_OK)
	{
		SF2_HW_PKCS11_FREE(pDigData);
		pSessionCtx->DigestCtx = NULL;
	}

	SF2_HW_PKCS11_RETURN();
}

CK_RV SF2_HW_PKCS11_Digest::DigestKey(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Digest::Final(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
    SF2_HW_PKCS11_HEADER();

    SF2_HW_DigestData* pDigData;
    UINT32             digestLen = *pulDigestLen;

    if(pSessionCtx == NULL || pSessionCtx->DigestCtx == NULL) return CKR_SESSION_CLOSED;

    pDigData = (SF2_HW_DigestData*)pSessionCtx->DigestCtx;

    if(pDigData->HmacKey != NULL)
    {
        //SF2_HW_PKCS11_CHECKRESULT(HMAC_Final (&pDigData->HmacCtx, pDigest, &digestLen));
    }
    else
    {
    	SF2_HW_PKCS11_SET_AND_LEAVE(CKR_FUNCTION_NOT_SUPPORTED);
    }

    *pulDigestLen = digestLen;

    SF2_HW_PKCS11_CLEANUP();

    SF2_HW_PKCS11_FREE(pDigData);
    pSessionCtx->DigestCtx = NULL;

    SF2_HW_PKCS11_RETURN();

}

