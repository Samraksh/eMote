#include "SF2_HW_pkcs11.h"
#include "../SF2_HW_Crypto.h"

CK_RV SF2_HW_PKCS11_Random::SeedRandom(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen)
{
	GetRandomSeed(pSeed, (int)ulSeedLen);
	return CKR_OK;
}

CK_RV SF2_HW_PKCS11_Random::GenerateRandom(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pRandomData, CK_ULONG ulRandomLen)
{
	GetRandomBytes(pRandomData, (UINT16)ulRandomLen);
	return CKR_OK;
}

