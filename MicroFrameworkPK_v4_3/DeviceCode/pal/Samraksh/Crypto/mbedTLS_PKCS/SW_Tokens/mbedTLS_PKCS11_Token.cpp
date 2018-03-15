#include "mbedTLS_PKCS11.h"

extern void InitKeyArray();

CK_RV MBEDTLS_PKCS11_Token::Initialize()
{
    //MBEDTLS_add_all_algorithms();
	InitKeyArray();
    return CKR_OK;
}
CK_RV MBEDTLS_PKCS11_Token::Uninitialize()
{
    //CRYPTO_cleanup_all_ex_data();
    //EVP_cleanup();
    
    return CKR_OK;
}

CK_RV MBEDTLS_PKCS11_Token::InitializeToken(CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel, CK_ULONG ulLabelLen)
{
    
    //MBEDTLS_PKCS11_Objects::IntitializeObjects();
    return CKR_OK;
}

CK_RV MBEDTLS_PKCS11_Token::GetDeviceError(CK_ULONG_PTR pErrorCode)
{
    *pErrorCode = 0;
    return CKR_OK;
}
    

