#include "../../mbedTLS_PKCS/SW_Tokens/AES_SW_PKCS11.h"

OBJECT_DATA AES_SW_PKCS11_Objects::s_Objects[];


void AES_SW_PKCS11_Objects::IntitializeObjects()
{
    memset(AES_SW_PKCS11_Objects::s_Objects, 0, sizeof(AES_SW_PKCS11_Objects::s_Objects));
}

int AES_SW_PKCS11_Objects::FindEmptyObjectHandle()
{
    int index;
    for(index=0; index<ARRAYSIZE(s_Objects); index++)
    {
        if(s_Objects[index].Data == NULL) break;
    }
    return index < ARRAYSIZE(s_Objects) ? index : -1;
}

OBJECT_DATA* AES_SW_PKCS11_Objects::GetObjectFromHandle(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject)
{
    OBJECT_DATA* retVal;

    if((int)hObject < 0 || hObject >= ARRAYSIZE(s_Objects))
    {
        return NULL;
    }

    retVal = &s_Objects[(int)hObject];
    return (retVal->Data == NULL) ? NULL : retVal;
}


BOOL AES_SW_PKCS11_Objects::FreeObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject)
{
    OBJECT_DATA* retVal;

    if((int)hObject < 0 || hObject >= ARRAYSIZE(s_Objects))
    {
        return FALSE;
    }

    retVal = &s_Objects[(int)hObject];

    if(retVal->Data == NULL) return FALSE;

    free(retVal->Data);

    retVal->Data = NULL;
    retVal->RefCount = 0;

    return TRUE;
}


CK_OBJECT_HANDLE AES_SW_PKCS11_Objects::AllocObject(Cryptoki_Session_Context* pSessionCtx, ObjectType type, size_t size, OBJECT_DATA** ppData)
{
    int idx = FindEmptyObjectHandle();

    *ppData = NULL;

    if(idx == -1) return idx;

    *ppData = &s_Objects[idx];

    (*ppData)->Type = type;
    (*ppData)->Data = malloc(size);
    (*ppData)->RefCount = 1;

    if((*ppData)->Data == NULL) return CK_OBJECT_HANDLE_INVALID;

    memset((*ppData)->Data, 0, size);

    return (CK_OBJECT_HANDLE)idx;
}


/*
CK_RV AES_SW_PKCS11_Objects::LoadX509Cert(Cryptoki_Session_Context* pSessionCtx, X509* x, OBJECT_DATA** ppObject, EVP_PKEY* privateKey, CK_OBJECT_HANDLE_PTR phObject)
{
    CERT_DATA* pCert;
    EVP_PKEY*  pubKey  = X509_get_pubkey(x);
    UINT32     keyType = CKK_VENDOR_DEFINED;

    if(phObject == NULL) return CKR_ARGUMENTS_BAD;

    *phObject = AllocObject(pSessionCtx, CertificateType, sizeof(CERT_DATA), ppObject);

    if(*phObject == CK_OBJECT_HANDLE_INVALID)
    {
        return CKR_FUNCTION_FAILED;
    }

    if(pubKey != NULL)

    {
        switch(pubKey->type)
        {
            case EVP_PKEY_RSA:
            case EVP_PKEY_RSA2:
                keyType = CKK_RSA;
                break;

            case EVP_PKEY_DSA:
            case EVP_PKEY_DSA1:
            case EVP_PKEY_DSA2:
            case EVP_PKEY_DSA3:
            case EVP_PKEY_DSA4:
                keyType = CKK_DSA;
                break;

            case EVP_PKEY_DH:
                keyType = CKK_DH;
                break;

            case EVP_PKEY_EC:
                keyType = CKK_DH;
                break;
        }
    }


    pCert = (CERT_DATA*)(*ppObject)->Data;

    pCert->cert = x;
    pCert->privKeyData.key = privateKey;
    if(privateKey != NULL)
    {
        pCert->privKeyData.attrib = Private;
        pCert->privKeyData.type   = keyType;
        pCert->privKeyData.size   = EVP_PKEY_bits(privateKey);
    }

    pCert->pubKeyData.key    = pubKey;
    pCert->pubKeyData.attrib = Public;
    pCert->pubKeyData.type   = keyType;
    pCert->pubKeyData.size   = EVP_PKEY_bits(pubKey);

    return CKR_OK;
}
*/

CK_RV AES_SW_PKCS11_Objects::CreateObject(Cryptoki_Session_Context* pSessionCtx, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phObject)
{
	UINT32 attribIndex = 0;
/*
	if(pTemplate[attribIndex].type == CKA_CLASS && pTemplate[attribIndex].ulValueLen == sizeof(CK_OBJECT_CLASS))
	{
		CK_OBJECT_CLASS cls = SwapEndianIfBEc32(*(CK_OBJECT_CLASS*)pTemplate[attribIndex].pValue);
		CHAR pwd[64];
		int len;

		switch(cls)
		{
			case CKO_CERTIFICATE:
				for(++attribIndex; attribIndex < ulCount; attribIndex++)
				{
					switch(pTemplate[attribIndex].type)
					{
						case CKA_PASSWORD:
							len = pTemplate[attribIndex].ulValueLen;

							if(len >= ARRAYSIZE(pwd))
							{
								len = ARRAYSIZE(pwd) - 1;
							}

							memcpy(pwd, pTemplate[attribIndex].pValue, len);

							pwd[len] = 0;

							break;

						case CKA_VALUE:
						{
							OBJECT_DATA* pObject = NULL;
							EVP_PKEY* privateKey = NULL;
							X509 *x =  ssl_parse_certificate(pTemplate[attribIndex].pValue, pTemplate[attribIndex].ulValueLen, pwd, &privateKey);

							if (x == NULL)
							{
								hal_printf("Unable to load certificate\n");
								return CKR_FUNCTION_FAILED;
							}
							else
							{
								CK_RV retVal = LoadX509Cert(pSessionCtx, x, &pObject, privateKey, phObject);

								if(retVal != CKR_OK) X509_free(x);

								return retVal;
							}

						}
					}
				}
				break;

		}
	}
	*/
	return CKR_FUNCTION_NOT_SUPPORTED;

}

CK_RV AES_SW_PKCS11_Objects::CopyObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phNewObject)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Objects::DestroyObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Objects::GetObjectSize(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ULONG_PTR pulSize)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Objects::GetAttributeValue(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Objects::SetAttributeValue(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Objects::FindObjectsInit(Cryptoki_Session_Context* pSessionCtx, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Objects::FindObjects(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE_PTR phObjects, CK_ULONG ulMaxCount, CK_ULONG_PTR pulObjectCount)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Objects::FindObjectsFinal(Cryptoki_Session_Context* pSessionCtx)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

