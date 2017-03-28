#include "AES_SW_pkcs11.h"

CK_RV AES_SW_PKCS11_Objects::CreateObject(Cryptoki_Session_Context* pSessionCtx, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phObject)
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
	                                TINYCLR_SSL_PRINTF("Unable to load certificate\n");
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

