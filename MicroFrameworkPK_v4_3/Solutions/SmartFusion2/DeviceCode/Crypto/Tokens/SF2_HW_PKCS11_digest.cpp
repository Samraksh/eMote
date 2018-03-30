#include "SF2_HW_PKCS11.h"

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
		case CKM_SHA_1:
			pDigest = EVP_sha1();
			break;
		case CKM_SHA224:
			pDigest = EVP_sha224();
			break;
		case CKM_SHA256:
			pDigest = EVP_sha256();
			break;
		case CKM_SHA384:
			pDigest = EVP_sha384();
			break;
		case CKM_SHA512:
			pDigest = EVP_sha512();
			break;

		case CKM_MD5:
			pDigest = EVP_md5();
			break;

		case CKM_RIPEMD160:
			pDigest = EVP_ripemd160();
			break;

		case CKM_MD5_HMAC:
			pDigest = EVP_md5();
			isHMAC = true;
			break;

		case CKM_SHA_1_HMAC:
			pDigest = EVP_sha1();
			isHMAC = true;
			break;

		case CKM_SHA256_HMAC:
			pDigest = EVP_sha256();
			isHMAC = true;
			break;

		case CKM_SHA384_HMAC:
			pDigest = EVP_sha384();
			isHMAC = true;
			break;

		case CKM_SHA512_HMAC:
			pDigest = EVP_sha512();
			isHMAC = true;
			break;

		case CKM_RIPEMD160_HMAC:
			pDigest = EVP_ripemd160();
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

		pDigData->HmacKey = PKCS11_Keys_SF2_HW_PKCS11::GetKeyFromHandle(pSessionCtx, hKey, TRUE);

		if(pDigData->HmacKey==NULL) SF2_HW_PKCS11_SET_AND_LEAVE(CKR_MECHANISM_PARAM_INVALID);

		pDigData->HmacCtx.md = pDigest;

		SF2_HW_PKCS11_CHECKRESULT(HMAC_Init(&pDigData->HmacCtx, pDigData->HmacKey->key, pDigData->HmacKey->size/8, pDigData->HmacCtx.md));
	}
	else
	{
		SF2_HW_PKCS11_CHECKRESULT(EVP_DigestInit_ex(&pDigData->CurrentCtx, pDigest, NULL));
	}

	pSessionCtx->DigestCtx = pDigData;

	SF2_HW_PKCS11_CLEANUP();
	if(retVal != CKR_OK && pDigData != NULL)
	{
		TINYCLR_SSL_FREE(pDigData);
	}
	SF2_HW_PKCS11_RETURN();

}

CK_RV SF2_HW_PKCS11_Digest::Digest(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Digest::Update(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Digest::DigestKey(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Digest::Final(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

