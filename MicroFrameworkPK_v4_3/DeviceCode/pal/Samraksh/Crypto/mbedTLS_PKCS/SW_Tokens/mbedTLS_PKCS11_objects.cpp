#include "mbedTLS_PKCS11.h"
#include <tinyhal.h>

OBJECT_DATA MBEDTLS_PKCS11_Objects::s_Objects[];
const UINT16 KEY_DATA_SIZE=((256+7)/8+sizeof(KEY_DATA));
//extern const UINT16 KEY_DATA_SIZE;
//extern UINT8 **KeyArray;
//extern bool KeyArrayUse[PKCS11_MBEDTLS_MAX_KEY_OBJECT_COUNT];
extern CK_RV AllocateKeyData(KEY_DATA **pKey);
extern CK_RV FreeKeyData(KEY_DATA *pKey);

int heapUsage =0;

void MBEDTLS_PKCS11_Objects::IntitializeObjects()
{
    memset(MBEDTLS_PKCS11_Objects::s_Objects, 0, sizeof(MBEDTLS_PKCS11_Objects::s_Objects));
}

int MBEDTLS_PKCS11_Objects::FindEmptyObjectHandle()
{
    int index;
    for(index=0; index<ARRAYSIZE(s_Objects); index++)
    {
        if(s_Objects[index].Data == NULL) break;
    }
    return index < ARRAYSIZE(s_Objects) ? index : -1;
}

OBJECT_DATA* MBEDTLS_PKCS11_Objects::GetObjectFromHandle(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject)
{
    OBJECT_DATA* retVal;

    if((int)hObject < 0 || hObject >= PKCS11_MBEDTLS_MAX_OBJECT_COUNT)
    {
        return NULL;
    }
    //if((int)hObject >= PKCS11_MBEDTLS_MAX_HEAP_OBJECT_COUNT && (int)hObject < PKCS11_MBEDTLS_MAX_OBJECT_COUNT){
    	//This is a static Key object.
    	//retVal= (OBJECT_DATA*)KeyArray[(int)hObject - PKCS11_MBEDTLS_MAX_HEAP_OBJECT_COUNT];

    //}else {
    	retVal = &s_Objects[(int)hObject];
    //}
    return (retVal->Data == NULL) ? NULL : retVal;
}


BOOL MBEDTLS_PKCS11_Objects::FreeObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject)
{
    OBJECT_DATA* retVal;
    if((int)hObject < 0 || hObject >= ARRAYSIZE(s_Objects))
    {
        return FALSE;
    }

    retVal = &s_Objects[(int)hObject];
    if(retVal->Data == NULL) return FALSE;

    if(retVal->Type == KeyType){
    	FreeKeyData((KEY_DATA*)retVal->Data);
    }else {
    	heapUsage -= retVal->Size;
    	PKCS11_MBEDTLS_FREE(retVal->Data);

    }
    retVal->Data = NULL;
    retVal->RefCount = 0;

    return TRUE;
}


CK_OBJECT_HANDLE MBEDTLS_PKCS11_Objects::AllocObject(Cryptoki_Session_Context* pSessionCtx, ObjectType type, size_t size, OBJECT_DATA** ppData)
{
    int idx = FindEmptyObjectHandle();
    *ppData = NULL;

    if(idx == -1) return idx;
    *ppData = &s_Objects[idx];

    (*ppData)->Type = type;
    if(type == KeyType){
    	AllocateKeyData((KEY_DATA **) &(*ppData)->Data);
    }else {
    	(*ppData)->Data = PKCS11_MBEDTLS_MALLOC(size);
    	if((*ppData)->Data != NULL) {
    		(*ppData)->Size = size;
    		heapUsage += size;
    	}
    }
    (*ppData)->RefCount = 1;
    if((*ppData)->Data == NULL) return CK_OBJECT_HANDLE_INVALID;

    memset((*ppData)->Data, 0, size);

    return (CK_OBJECT_HANDLE)idx;
}


/*
CK_RV MBEDTLS_PKCS11_Objects::LoadX509Cert(Cryptoki_Session_Context* pSessionCtx, X509* x, OBJECT_DATA** ppObject, EVP_PKEY* privateKey, CK_OBJECT_HANDLE_PTR phObject)
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

CK_RV MBEDTLS_PKCS11_Objects::CreateObject(Cryptoki_Session_Context* pSessionCtx, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phObject)
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
				//hal_printf("I am in CreateObjects:: Secret key, not implemented\n");
				break;

			case CKO_DATA:
				//hal_printf("I am in CreateObjects:: Data, not implemented\n");
				break;

			case CKO_PUBLIC_KEY:
				//hal_printf("I am in CreateObjects:: Public key, not implemented\n");
				break;

			case CKO_PRIVATE_KEY:
				//hal_printf("I am in CreateObjects:: Pivate key, not implemented\n");
				break;

			case CKO_HW_FEATURE:
				//hal_printf("I am in CreateObjects:: HW_Feature, not implemented\n");
				break;

			case CKO_DOMAIN_PARAMETERS:
				//hal_printf("I am in CreateObjects:: Domain Parameters, not implemented\n");
				break;

			case CKO_MECHANISM:
				//hal_printf("I am in CreateObjects:: Mechanism, not implemented\n");
				break;
		}
	}

	return CKR_FUNCTION_NOT_SUPPORTED;

}

CK_RV MBEDTLS_PKCS11_Objects::CopyObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phNewObject)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Objects::DestroyObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject)
{
	 /*if((int)hObject >= PKCS11_MBEDTLS_MAX_HEAP_OBJECT_COUNT && (int)hObject < PKCS11_MBEDTLS_MAX_KEY_OBJECT_COUNT){
	    	//This is a static Key object.
	    	//memset(KeyArray[(int)hObject - PKCS11_MBEDTLS_MAX_HEAP_OBJECT_COUNT], 0, KEY_DATA_SIZE);
	    	//KeyArrayUse[(int)hObject - PKCS11_MBEDTLS_MAX_HEAP_OBJECT_COUNT]=0;

	 }*/

	OBJECT_DATA* pData = GetObjectFromHandle(pSessionCtx, hObject);

	if(pData == NULL) return CKR_OBJECT_HANDLE_INVALID;

	if(--pData->RefCount <= 0)
	{
		if(pData->Type == KeyType)
		{
			MBEDTLS_PKCS11_Keys::DeleteKey(pSessionCtx, (KEY_DATA *)pData->Data);
		}
		else if(pData->Type == CertificateType)
		{
			//CERT_DATA* pCert = (CERT_DATA*)pData->Data;
			//X509_free(pCert->cert);
			return CKR_FUNCTION_NOT_SUPPORTED;
		}

		return FreeObject(pSessionCtx, hObject) == NULL ? CKR_OBJECT_HANDLE_INVALID : CKR_OK;
	}

	return CKR_OK;
}

CK_RV MBEDTLS_PKCS11_Objects::GetObjectSize(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ULONG_PTR pulSize)
{
	//hal_printf("I am in Objects::GetObjectSize , not implemented\n");
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Objects::GetAttributeValue(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
	OBJECT_DATA* pObj = MBEDTLS_PKCS11_Objects::GetObjectFromHandle(pSessionCtx, hObject);

	if(pObj == NULL) return CKR_OBJECT_HANDLE_INVALID;

	if(pObj->Type == CertificateType)
	{
		/* We dont support certificates at this time
		CERT_DATA* pCertData = (CERT_DATA*)pObj->Data;
		X509* pCert = pCertData->cert;
		INT32 valLen = 0;

		for(int i=0; i<(int)ulCount; i++)
		{
			switch(pTemplate[i].type)
			{
				case CKA_CLASS:
					*(INT32*)pTemplate[i].pValue = SwapEndianIfBEc32(CKO_CERTIFICATE);
					break;
				case CKA_PRIVATE:
					*(INT32*)pTemplate[i].pValue = SwapEndianIfBEc32(pCertData->privKeyData.key == NULL ? 0 : 1);
					valLen = sizeof(INT32);
					break;

				case CKA_VALUE_BITS:
					{
						*(INT32*)pTemplate[i].pValue = SwapEndianIfBEc32(pCertData->pubKeyData.size);
						valLen = sizeof(INT32);
					}
					break;

				case CKA_KEY_TYPE:
					{
						*(UINT32*)pTemplate[i].pValue = SwapEndianIfBEc32(pCertData->pubKeyData.type);
						valLen = sizeof(UINT32);
					}
					break;

				case CKA_ISSUER:
					{
						char* name=X509_NAME_oneline(X509_get_issuer_name(pCert),NULL,0);
						valLen = PKCS11_MBEDTLS_STRLEN(name) + 1;

						if(valLen > pTemplate[i].ulValueLen) valLen = pTemplate[i].ulValueLen;

						hal_strcpy_s((char*)pTemplate[i].pValue, valLen, name);
						PKCS11_MBEDTLS_FREE(name);
					}
					break;

				case CKA_SUBJECT:
					{
						char* subject=X509_NAME_oneline(X509_get_subject_name(pCert),NULL,0);
						valLen = PKCS11_MBEDTLS_STRLEN(subject) + 1;

						if(valLen > pTemplate[i].ulValueLen) valLen = pTemplate[i].ulValueLen;

						hal_strcpy_s((char*)pTemplate[i].pValue, valLen, subject);
						PKCS11_MBEDTLS_FREE(subject);
					}
					break;

				case CKA_SERIAL_NUMBER:
					{
						ASN1_INTEGER* asn = X509_get_serialNumber(pCert);

						valLen = asn->length;

						if(valLen > pTemplate[i].ulValueLen) valLen = pTemplate[i].ulValueLen;

						PKCS11_MBEDTLS_MEMCPY(pTemplate[i].pValue, asn->data, valLen);
					}
					break;

				case CKA_MECHANISM_TYPE:
					if(pCert->sig_alg != NULL)
					{
						int signature_nid;
						CK_MECHANISM_TYPE type = CKM_VENDOR_DEFINED;

						signature_nid = OBJ_obj2nid(pCert->sig_alg->algorithm);

						switch(signature_nid)
						{
							case NID_sha1WithRSA:
							case NID_sha1WithRSAEncryption:
								//szNid = "1.2.840.113549.1.1.5";
								type = CKM_SHA1_RSA_PKCS;
								break;

							case NID_md5WithRSA:
							case NID_md5WithRSAEncryption:
								//szNid = "1.2.840.113549.1.1.4";
								type = CKM_MD5_RSA_PKCS;
								break;

							case NID_sha256WithRSAEncryption:
								//szNid = "1.2.840.113549.1.1.11";
								type = CKM_SHA256_RSA_PKCS;
								break;

							case NID_sha384WithRSAEncryption:
								//szNid = "1.2.840.113549.1.1.12";
								type = CKM_SHA384_RSA_PKCS;
								break;

							case NID_sha512WithRSAEncryption:
								//szNid = "1.2.840.113549.1.1.13";
								type = CKM_SHA512_RSA_PKCS;
								break;
						}

						valLen = sizeof(CK_MECHANISM_TYPE);
						*(CK_MECHANISM_TYPE*)pTemplate[i].pValue = SwapEndianIfBEc32(type);
					}
					break;

				case CKA_START_DATE:
					{
						DATE_TIME_INFO dti;

						ssl_get_ASN1_UTCTIME(X509_get_notBefore(pCert), &dti);

						valLen = (INT32)pTemplate[i].ulValueLen;

						if(valLen > sizeof(dti)) valLen = sizeof(dti);

						PKCS11_MBEDTLS_MEMCPY(pTemplate[i].pValue, &dti, valLen);
					}
					break;

				case CKA_END_DATE:
					{
						DATE_TIME_INFO dti;

						ssl_get_ASN1_UTCTIME(X509_get_notAfter(pCert), &dti);

						valLen = pTemplate[i].ulValueLen;

						if(valLen > sizeof(dti)) valLen = sizeof(dti);

						PKCS11_MBEDTLS_MEMCPY(pTemplate[i].pValue, &dti, valLen);
					}
					break;

				case CKA_VALUE:
					{
						UINT8* pData = (UINT8*)pTemplate[i].pValue;
						UINT8* pTmp = pData;

						valLen = i2d_X509(pCert, NULL);

						if(valLen > pTemplate[i].ulValueLen) return CKR_DEVICE_MEMORY;

						valLen = i2d_X509(pCert, &pTmp);

						if(valLen < 0) return CKR_FUNCTION_FAILED;
					}
					break;

				case CKA_VALUE_LEN:
					*(UINT32*)pTemplate[i].pValue = SwapEndianIfBEc32(valLen);
					break;

				default:
					return CKR_ATTRIBUTE_TYPE_INVALID;
			}
		}*/
	}
	else if(pObj->Type == KeyType)
	{
		KEY_DATA* pKey = (KEY_DATA*)pObj->Data;
		int valLen = 0;
		bool isPrivate = false;

		for(int i=0; i<(int)ulCount; i++)
		{
			switch(pTemplate[i].type)
			{
				case CKA_CLASS:
					*(INT32*)pTemplate[i].pValue = SwapEndianIfBEc32((0 != (pKey->attrib & Private) ? CKO_PRIVATE_KEY : 0 != (pKey->attrib & Public) ? CKO_PUBLIC_KEY : CKO_SECRET_KEY));
					break;

				case CKA_MODULUS:
					if(pKey->type == CKK_RSA)
					{
						//Mukundan: We dont support rsa at this point
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.rsa->n, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;

				case CKA_EXPONENT_1:
					if(pKey->type == CKK_RSA)
					{
						//Mukundan: We dont support rsa at this point
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.rsa->dmp1, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;

				case CKA_EXPONENT_2:
					if(pKey->type == CKK_RSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.rsa->dmq1, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;

				case CKA_COEFFICIENT:
					if(pKey->type == CKK_RSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.rsa->iqmp, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;

				case CKA_PRIME_1:
					if(pKey->type == CKK_RSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.rsa->p, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;

				case CKA_PRIME_2:
					if(pKey->type == CKK_RSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.rsa->q, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;


				case CKA_PRIVATE_EXPONENT:
					if(pKey->type == CKK_DSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.dsa->priv_key, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					else if(pKey->type == CKK_RSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.rsa->d, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;
				case CKA_PUBLIC_EXPONENT:
					if(pKey->type == CKK_DSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.dsa->pub_key, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					else if(pKey->type == CKK_EC)
					{
						/*UINT8 pTmp[66*2+1];
						EC_KEY* pEC = ((EVP_PKEY*)pKey->key)->pkey.ec;

						const EC_POINT* point = EC_KEY_get0_public_key(pEC);
						valLen = EC_POINT_point2oct(EC_KEY_get0_group(pEC), point, POINT_CONVERSION_UNCOMPRESSED, (UINT8*)pTmp, ARRAYSIZE(pTmp), NULL);

						if(valLen == 0) return CKR_FUNCTION_FAILED;

						memmove(pTemplate[i].pValue, &pTmp[1], valLen-1); // remove POINT_CONVERSION_UNCOMPRESSED header byte
						*/
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					else if(pKey->type == CKK_RSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.rsa->e, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;

				case CKA_PRIME:
					if(pKey->type == CKK_DSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.dsa->p, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;

				case CKA_SUBPRIME:
					if(pKey->type == CKK_DSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.dsa->q, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;

				case CKA_BASE:
					if(pKey->type == CKK_DSA)
					{
						//EVP_PKEY* pRealKey = (EVP_PKEY*)pKey->key;
						//valLen = BN_bn2bin(pRealKey->pkey.dsa->g, (UINT8*)pTemplate[i].pValue);
						return CKR_FUNCTION_NOT_SUPPORTED;
					}
					break;

				case CKA_PRIVATE:
					isPrivate = 0 != *(INT32*)pTemplate[i].pValue;
					break;


				case CKA_KEY_TYPE:
					*(INT32*)pTemplate[i].pValue = SwapEndianIfBEc32(pKey->type);
					break;

				case CKA_VALUE_BITS:
					*(UINT32*)pTemplate[i].pValue = SwapEndianIfBEc32(pKey->size);
					break;

				case CKA_VALUE:
					switch(pKey->type)
					{
						case CKK_AES:
						case CKK_GENERIC_SECRET:
							{
								// TODO: Add permissions to export key
								int len = (pKey->size + 7) / 8;

								if(len > (INT32)pTemplate[i].ulValueLen) len = (int)pTemplate[i].ulValueLen;

								PKCS11_MBEDTLS_MEMCPY(pTemplate[i].pValue, pKey->key, len);
								valLen = len;
							}
							break;

						default:
							return CKR_ATTRIBUTE_TYPE_INVALID;
					}
					break;

				case CKA_VALUE_LEN:
					switch(pKey->type)
					{
						case CKK_EC:
							*(UINT32*)pTemplate[i].pValue = SwapEndianIfBEc32(valLen);
							break;

						default:
							return CKR_ATTRIBUTE_TYPE_INVALID;
					}
					break;

				default:
					return CKR_ATTRIBUTE_TYPE_INVALID;
			}
		}
	}

	return CKR_OK;

	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Objects::SetAttributeValue(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    OBJECT_DATA* pObj = MBEDTLS_PKCS11_Objects::GetObjectFromHandle(pSessionCtx, hObject);

    if(pObj == NULL) return CKR_OBJECT_HANDLE_INVALID;

    /* We dont support certificates at this time
    if(pObj->Type == CertificateType)
    {

    	CERT_DATA* pCertData = (CERT_DATA*)pObj->Data;
        X509* pCert = pCertData->cert;
        CHAR group[20] = {0};
        CHAR fileName[20] = {0};
        INT32 len = 0;

        BOOL fSave = FALSE;
        BOOL fDelete = FALSE;

        for(int i=0; i<(int)ulCount; i++)
        {
            switch(pTemplate[i].type)
            {
                case CKA_PERSIST:
                    fSave = SwapEndianIfBEc32(*(INT32*)pTemplate[i].pValue) != 0;
                    fDelete = !fSave;
                    break;

                case CKA_LABEL:
                    len = pTemplate[i].ulValueLen;

                    if(len >= ARRAYSIZE(group))
                    {
                        len = ARRAYSIZE(group) - 1;
                    }

                    PKCS11_MBEDTLS_MEMCPY(group, pTemplate[i].pValue, len);

                    group[len] = 0;

                    break;

                case CKA_OBJECT_ID:
                    len = pTemplate[i].ulValueLen;

                    if(len >= ARRAYSIZE(fileName))
                    {
                        len = ARRAYSIZE(fileName) - 1;
                    }

                    PKCS11_MBEDTLS_MEMCPY(fileName, pTemplate[i].pValue, len);

                    fileName[len] = 0;

                    break;

                default:
                    return CKR_ATTRIBUTE_TYPE_INVALID;
            }
        }

        if(fDelete)
        {
            hal_strcpy_s(fileName, ARRAYSIZE(pObj->FileName), pObj->FileName);

            pObj->GroupName[0] = 0;

            if(g_OpenSSL_Token.Storage != NULL && g_OpenSSL_Token.Storage->Delete != NULL)
            {
                if(!g_OpenSSL_Token.Storage->Delete(fileName, group)) return CKR_FUNCTION_FAILED;
            }
            else
            {
                return CKR_FUNCTION_NOT_SUPPORTED;
            }
        }
        else if(fSave)
        {
            hal_strcpy_s(pObj->GroupName, ARRAYSIZE(pObj->GroupName), group   );
            hal_strcpy_s(pObj->FileName , ARRAYSIZE(pObj->FileName ), fileName);

            if(g_OpenSSL_Token.Storage != NULL && g_OpenSSL_Token.Storage->Create != NULL)
            {
                BOOL res;
                UINT8* pData, *pTmp;
                INT32 len = i2d_X509( pCert, NULL );

                if(len <= 0) return CKR_FUNCTION_FAILED;

                pData = (UINT8*)PKCS11_MBEDTLS_MALLOC(len);

                if(pData == NULL) return CKR_DEVICE_MEMORY;

                pTmp = pData;

                i2d_X509(pCert, &pTmp);

                // TODO: Save private key as well

                res = g_OpenSSL_Token.Storage->Create( fileName, group, CertificateType, pData, len );

                PKCS11_MBEDTLS_FREE(pData);

                if(!res) return CKR_FUNCTION_FAILED;
            }
            else
            {
                return CKR_FUNCTION_NOT_SUPPORTED;
            }
        }
        else
        {
            return CKR_ATTRIBUTE_TYPE_INVALID;
        }
    }*/

    return CKR_OK;
}

CK_RV MBEDTLS_PKCS11_Objects::FindObjectsInit(Cryptoki_Session_Context* pSessionCtx, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
	//hal_printf("I am in Objects::FindObjectsInit , not implemented\n");
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Objects::FindObjects(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE_PTR phObjects, CK_ULONG ulMaxCount, CK_ULONG_PTR pulObjectCount)
{
	//hal_printf("I am in Objects::FindObjects , not implemented\n");
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Objects::FindObjectsFinal(Cryptoki_Session_Context* pSessionCtx)
{
	//hal_printf("I am in Objects::FindObjectsFinal , not implemented\n");
    return CKR_FUNCTION_NOT_SUPPORTED;
}

