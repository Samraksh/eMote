#ifndef _ROT_SERVICES_H_
#define _ROT_SERVICES_H_

#include <tinyhal_types.h>
#include "Crypto.h"
#include "CryptoInterface.h"
#include "Keys.h"
#include <tinyhal.h>
#include <BlockStorage_decl.h>

enum KeyType{Symetric, PK };
enum SigType{HMAC, SHA256};
enum OSModule{RoT, TB, RT, App};


bool SecureOS_Boot(OSModule mod, UINT32 modLength, UINT8* pSig, UINT32 sigLength, UINT8* pKey, UINT32 keyLength);

bool AttestOS(OSModule mod, UINT32 modLength,  UINT8* pSig, UINT32 sigLength, UINT8* pkey, UINT32 keyLength);
bool AttestBinary(PBYTE  pData, UINT32 dataLength, PBYTE pSig, UINT32 sigLength, PBYTE key, UINT32 keyLength );
bool ComputeHMAC(PBYTE  pData, UINT32 ulDataLen, PBYTE pDigest, SigType mtype, KeyType kt, PBYTE key, UINT32 keyLen);


bool CheckFlashSectorPermission( BlockStorageDevice *pDevice, ByteAddress address );
BOOL TinyBooter_GetReleaseInfo(MfReleaseInfo& releaseInfo);
UINT32 Tinybooter_ProgramWordCheck();

void PrintHex(UINT8* sig, int size);

#endif /* _ROT_SERVICES_H_ */
