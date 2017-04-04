#include <mbedtls/pkcs11.h>
#include <mbedtls/cipher.h>
#include <mbedtls/md.h>
#include "mbedTLS_PKCS11.h"
mbedtls_cipher_context_t mtls_cipher_cxt;
//mbedtls_pkcs11_context mtls_cxt1;
#define MAX_OPEN_SESSIONS 4
/*
static UINT8 genSessionID=0;
struct SessionMap{
	UINT8 sessionID;
	Cryptoki_Session_Context* sessPtr;
};

//Cryptoki_Session_Context *currentSession =NULL;
//SessionMap OpenSessionMap[MAX_OPEN_SESSIONS];
*/

CK_RV MBEDTLS_PKCS11_Session::InitPin(Cryptoki_Session_Context* pSessionCtx, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
	hal_printf("I am in Session::InitPin, not implemented\n");
	//mbedtls_cipher_init(&mtls_cipher_cxt);
	return CKR_OK;
    //return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Session::SetPin(Cryptoki_Session_Context* pSessionCtx, CK_UTF8CHAR_PTR pOldPin, CK_ULONG ulOldPinLen, CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewPinLen)
{
	hal_printf("I am in Session::SetPin, not implemented\n");
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Session::OpenSession(Cryptoki_Session_Context* pSessionCtx, CK_BBOOL fReadWrite)
{
	hal_printf("I am in Session::OpenSession, not implemented\n");
	//currentSession = pSessionCtx;
	//OpenSessionMap[genSessionID++]= pSessionCtx;
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Session::CloseSession(Cryptoki_Session_Context* pSessionCtx)
{
	hal_printf("I am in Session::CloseSession, not implemented\n");
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Session::Login(Cryptoki_Session_Context* pSessionCtx, CK_USER_TYPE userType, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
	hal_printf("I am in Session::Login, not implemented\n");
	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV MBEDTLS_PKCS11_Session::Logout(Cryptoki_Session_Context* pSessionCtx)
{
	hal_printf("I am in Session::Logout, not implemented\n");
    return CKR_FUNCTION_NOT_SUPPORTED;
}

