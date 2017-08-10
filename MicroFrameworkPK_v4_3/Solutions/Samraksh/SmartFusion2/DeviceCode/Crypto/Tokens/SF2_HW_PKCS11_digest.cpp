#include "SF2_HW_PKCS11.h"

CK_RV SF2_HW_PKCS11_Digest::DigestInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
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

