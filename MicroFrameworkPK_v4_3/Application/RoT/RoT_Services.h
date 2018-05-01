#ifndef _ROT_SERVICES_H_
#define _ROT_SERVICES_H_

#include <tinyhal_types.h>

enum KeyType{Symetric, PK };
enum SigType{HMAC, SHA256};

bool AttestBinary(PBYTE  pData, PBYTE pSig, SigType st, KeyType kt, PBYTE key );
bool ComputeHMAC(PBYTE  pData, UINT32 ulDataLen, PBYTE pDigest, SigType mtype, KeyType kt, PBYTE key, UINT32 keyLen);



#endif /* _ROT_SERVICES_H_ */
