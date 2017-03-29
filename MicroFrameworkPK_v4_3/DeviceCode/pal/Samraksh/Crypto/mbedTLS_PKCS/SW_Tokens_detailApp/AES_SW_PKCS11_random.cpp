#include "../../mbedTLS_PKCS/SW_Tokens_detailApp/AES_SW_PKCS11.h"

CK_RV AES_SW_PKCS11_Random::SeedRandom(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV AES_SW_PKCS11_Random::GenerateRandom(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pRandomData, CK_ULONG ulRandomLen)
{
    return CKR_FUNCTION_NOT_SUPPORTED;
}

