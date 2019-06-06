#ifndef _ROT_KEYSTORE_
#define _ROT_KEYSTORE_

#include <drivers/mss_sys_services/mss_sys_services.h>
#include <PKCS11\CryptokiPAL.h>

//enum KeyType{Symetric, PK };
//enum SigType{HMAC, SHA256};

enum UserType{UT_RoT, UT_Kernel, UT_App};

enum KeySlot {
	K_DS0=0, //DesignSecurity
	K_DS1, //DesignSecurity
	K_Unused,//May be used by jtag
	K_DeviceKey_PubK,
	K_DeviceKey_PriK,
	K_DeviceKey_SecK,
	K_KeneralStart,
	K_Kernel1,
	K_Kernel2,
	K_Kernel3,
	K_Kernel4,
	K_Kernel5,
	K_Kernel6,
	K_Kernel7,
	K_UserStart,
	K_App1,
	K_App2,
	K_App3,
	K_App4,
	K_App5,
	K_App6,
	K_App7,
	K_App8
};

//bool VerifyIntegrity(CK_BYTE_PTR  pData, CK_BYTE_PTR pSig, SigType st, KeyType kt, CK_BYTE_PTR key );
//bool ComputeHMAC(CK_BYTE_PTR  pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_MECHANISM_TYPE mtype, CK_KEY_TYPE kt, CK_BYTE_PTR key, CK_ULONG keyLen);

const UINT8 totalKeys=57;

UINT8 ValidateUserCall(void* callPtr);

UINT8 KeyStore_KeysRemaining();

UINT8 KeyStore_Initialize();

UINT8 AllocateKey(CK_BYTE_PTR  pData, CK_ULONG ulDataLen, UserType user);

UINT8 StoreKey(CK_BYTE_PTR  pData, CK_ULONG ulDataLen, UserType user);

UINT8 ReadKey(CK_BYTE_PTR  pData, CK_ULONG ulDataLen, UserType user);

#endif // _ROT_KEYSTORE_
