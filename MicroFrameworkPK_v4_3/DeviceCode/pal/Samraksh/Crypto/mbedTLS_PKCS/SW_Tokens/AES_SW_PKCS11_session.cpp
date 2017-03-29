#include <mbedtls/pkcs11.h>
#include <mbedtls/cipher.h>
#include <mbedtls/md.h>
#include "../../mbedTLS_PKCS/SW_Tokens/AES_SW_PKCS11.h"
mbedtls_cipher_context_t mtls_cipher_cxt;
//mbedtls_pkcs11_context mtls_cxt1;

CK_RV AES_SW_PKCS11_Session::InitPin(Cryptoki_Session_Context* pSessionCtx, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
	hal_printf("I am in Session::InitPin\n");
	mbedtls_cipher_init(&mtls_cipher_cxt);
	return CKR_OK;
    //return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Session::SetPin(Cryptoki_Session_Context* pSessionCtx, CK_UTF8CHAR_PTR pOldPin, CK_ULONG ulOldPinLen, CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewPinLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Session::OpenSession(Cryptoki_Session_Context* pSessionCtx, CK_BBOOL fReadWrite)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Session::CloseSession(Cryptoki_Session_Context* pSessionCtx)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Session::Login(Cryptoki_Session_Context* pSessionCtx, CK_USER_TYPE userType, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Session::Logout(Cryptoki_Session_Context* pSessionCtx)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

