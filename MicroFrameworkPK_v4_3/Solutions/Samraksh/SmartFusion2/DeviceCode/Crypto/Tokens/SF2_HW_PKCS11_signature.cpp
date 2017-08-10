#include "SF2_HW_pkcs11.h"

CK_RV SF2_HW_PKCS11_Signature::SignInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Signature::Sign(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Signature::SignUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Signature::SignFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Signature::VerifyInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Signature::Verify(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Signature::VerifyUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV SF2_HW_PKCS11_Signature::VerifyFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}


