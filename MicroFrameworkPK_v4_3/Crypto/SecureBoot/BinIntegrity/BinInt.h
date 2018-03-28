#ifndef _SECUREBOOT_BININTEGRITY_BININT_H_
#define _SECUREBOOT_BININTEGRITY_BININT_H_

#include <PKCS11\CryptokiPAL.h>

enum KeyType{Symetric, PK };
enum SigType{HMAC, SHA256};

bool VerifyIntegrity(CK_BYTE_PTR  pData, CK_BYTE_PTR pSig, SigType st, KeyType kt, CK_BYTE_PTR key );




#endif /* _SECUREBOOT_BININTEGRITY_BININT_H_ */
