#ifndef _SECUREBOOT_ATTESTATION_H_
#define _SECUREBOOT_ATTESTATION_H_

#include <PKCS11\CryptokiPAL.h>

enum KeyType{Symetric, PK };
enum SigType{HMAC, SHA256};

bool VerifyIntegrity(CK_BYTE_PTR  pData, CK_BYTE_PTR pSig, SigType st, KeyType kt, CK_BYTE_PTR key );
bool ComputeHMAC(CK_BYTE_PTR  pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_MECHANISM_TYPE mtype, CK_KEY_TYPE kt, CK_BYTE_PTR key, CK_ULONG keyLen);



#endif /* _SECUREBOOT_ATTESTATION_H_ */
