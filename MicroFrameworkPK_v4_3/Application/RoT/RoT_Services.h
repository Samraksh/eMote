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

bool AttestOS(UINT8* signature, UINT32 length, UINT32 keyIndex);
bool AttestBinary(PBYTE  pData, PBYTE pSig, SigType st, KeyType kt, PBYTE key );
bool ComputeHMAC(PBYTE  pData, UINT32 ulDataLen, PBYTE pDigest, SigType mtype, KeyType kt, PBYTE key, UINT32 keyLen);
bool SecureOS_Boot();


bool CheckFlashSectorPermission( BlockStorageDevice *pDevice, ByteAddress address );
BOOL TinyBooter_GetReleaseInfo(MfReleaseInfo& releaseInfo);
UINT32 Tinybooter_ProgramWordCheck();

#endif /* _ROT_SERVICES_H_ */
