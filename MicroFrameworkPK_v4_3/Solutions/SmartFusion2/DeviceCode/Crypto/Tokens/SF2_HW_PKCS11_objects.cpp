#include "SF2_HW_pkcs11.h"


OBJECT_DATA SF2_HW_PKCS11_Objects::s_Objects[];

//const UINT16 KEY_DATA_SIZE=((256+7)/8+sizeof(KEY_DATA));

extern CK_RV AllocateKeyData(KEY_DATA **pKey);
int heapUsage =0;

int SF2_HW_PKCS11_Objects::FindEmptyObjectHandle()
{
    int index;
    for(index=0; index<ARRAYSIZE(s_Objects); index++)
    {
        if(s_Objects[index].Data == NULL) break;
    }
    return index < ARRAYSIZE(s_Objects) ? index : -1;
}

OBJECT_DATA* SF2_HW_PKCS11_Objects::GetObjectFromHandle(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject)
{
    OBJECT_DATA* retVal;

    if((int)hObject < 0 || hObject >= SF2_HW_PKCS11_MAX_OBJECT_COUNT)
    {
        return NULL;
    }
   	retVal = &s_Objects[(int)hObject];

    return (retVal->Data == NULL) ? NULL : retVal;
}

CK_OBJECT_HANDLE SF2_HW_PKCS11_Objects::AllocObject(Cryptoki_Session_Context* pSessionCtx, ObjectType type, size_t size, OBJECT_DATA** ppData)
{
    int idx = FindEmptyObjectHandle();
    *ppData = NULL;

    if(idx == -1) return idx;
    *ppData = &s_Objects[idx];

    (*ppData)->Type = type;

    /*if(type == KeyType){
    	AllocateKeyData((KEY_DATA **) &(*ppData)->Data);
    }else {
    	(*ppData)->Data = SF2_HW_PKCS11_MALLOC(size);
    	if((*ppData)->Data != NULL) {
    		(*ppData)->Size = size;
    		heapUsage += size;
    	}
    }*/
    (*ppData)->Data = SF2_HW_PKCS11_MALLOC(size);
    (*ppData)->RefCount = 1;
    if((*ppData)->Data == NULL) return CK_OBJECT_HANDLE_INVALID;

    memset((*ppData)->Data, 0, size);

    return (CK_OBJECT_HANDLE)idx;
}

BOOL SF2_HW_PKCS11_Objects::FreeObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject)
{
    OBJECT_DATA* retVal;
    if((int)hObject < 0 || hObject >= ARRAYSIZE(s_Objects))
    {
        return FALSE;
    }

    retVal = &s_Objects[(int)hObject];
    if(retVal->Data == NULL) return FALSE;
    /*
    if(retVal->Type == KeyType){
    	FreeKeyData((KEY_DATA*)retVal->Data);
    }else {
    	heapUsage -= retVal->Size;
    	SF2_HW_PKCS11_FREE(retVal->Data);

    }*/

    SF2_HW_PKCS11_FREE(retVal->Data);
    retVal->Data = NULL;
    retVal->RefCount = 0;

    return TRUE;
}


void SF2_HW_PKCS11_Objects::IntitializeObjects(){

}

CK_RV SF2_HW_PKCS11_Objects::CreateObject(Cryptoki_Session_Context* pSessionCtx, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phObject)
{
	UINT32 attribIndex = 0;

	if(pTemplate[attribIndex].type == CKA_CLASS && pTemplate[attribIndex].ulValueLen == sizeof(CK_OBJECT_CLASS))
	{
		CK_OBJECT_CLASS cls = SwapEndianIfBEc32(*(CK_OBJECT_CLASS*)pTemplate[attribIndex].pValue);
		CHAR pwd[64];
		int len;

		switch(cls)
		{
			case CKO_CERTIFICATE:
				//Mukundan: We dont support certificates for the time being.
				/*for(++attribIndex; attribIndex < ulCount; attribIndex++)
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
				*/
				break;
			case CKO_SECRET_KEY:
				hal_printf("I am in CreateObjects:: Secret key, not implemented\n");
				break;

			case CKO_DATA:
				hal_printf("I am in CreateObjects:: Data, not implemented\n");
				break;

			case CKO_PUBLIC_KEY:
				hal_printf("I am in CreateObjects:: Public key, not implemented\n");
				break;

			case CKO_PRIVATE_KEY:
				hal_printf("I am in CreateObjects:: Pivate key, not implemented\n");
				break;

			case CKO_HW_FEATURE:
				hal_printf("I am in CreateObjects:: HW_Feature, not implemented\n");
				break;

			case CKO_DOMAIN_PARAMETERS:
				hal_printf("I am in CreateObjects:: Domain Parameters, not implemented\n");
				break;

			case CKO_MECHANISM:
				hal_printf("I am in CreateObjects:: Mechanism, not implemented\n");
				break;
		}
	}

	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Objects::CopyObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phNewObject)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Objects::DestroyObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject)
{
	OBJECT_DATA* pData = GetObjectFromHandle(pSessionCtx, hObject);

	if(pData == NULL) return CKR_OBJECT_HANDLE_INVALID;

	if(--pData->RefCount <= 0)
	{
		if(pData->Type == KeyType)
		{
			SF2_HW_PKCS11_Keys::DeleteKey(pSessionCtx, (KEY_DATA *)pData->Data);
		}
		else if(pData->Type == CertificateType)
		{
			//CERT_DATA* pCert = (CERT_DATA*)pData->Data;
			//X509_free(pCert->cert);
			return CKR_FUNCTION_NOT_SUPPORTED;
		}

		return FreeObject(pSessionCtx, hObject) ? CKR_OK:CKR_OBJECT_HANDLE_INVALID;
	}

	return CKR_OK;
}

CK_RV SF2_HW_PKCS11_Objects::GetObjectSize(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ULONG_PTR pulSize)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Objects::GetAttributeValue(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Objects::SetAttributeValue(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Objects::FindObjectsInit(Cryptoki_Session_Context* pSessionCtx, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Objects::FindObjects(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE_PTR phObjects, CK_ULONG ulMaxCount, CK_ULONG_PTR pulObjectCount)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Objects::FindObjectsFinal(Cryptoki_Session_Context* pSessionCtx)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

