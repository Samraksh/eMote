#include "mbedTLS_PKCS11.h"
#include "../../RandomUtils.h"

CK_RV MBEDTLS_PKCS11_Random::SeedRandom(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen)
{
	GetRandomSeed(pSeed, (int)ulSeedLen);
    return CKR_OK;
}

CK_RV MBEDTLS_PKCS11_Random::GenerateRandom(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pRandomData, CK_ULONG ulRandomLen)
{
	GetRandomBytes(pRandomData, (UINT16)ulRandomLen);
    return CKR_OK;
}

