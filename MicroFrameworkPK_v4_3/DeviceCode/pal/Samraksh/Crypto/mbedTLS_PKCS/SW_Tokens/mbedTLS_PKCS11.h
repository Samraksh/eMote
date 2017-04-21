#include <PKCS11\CryptokiPAL.h>
#include <TinyCLR_Runtime.h>
#include <TinyCLR_Runtime__HeapBlock.h>

#include <mbedtls/pkcs11.h>
#include <mbedtls/cipher.h>

#ifndef _MBEDTLS_PKCS11_H_
#define _MBEDTLS_PKCS11_H_ 1


#ifndef PKCS11_MBEDTLS_MAX_HEAP_OBJECT_COUNT
#define PKCS11_MBEDTLS_MAX_HEAP_OBJECT_COUNT 20
#endif

#ifndef PKCS11_MBEDTLS_MAX_KEY_OBJECT_COUNT
#define PKCS11_MBEDTLS_MAX_KEY_OBJECT_COUNT 4
#endif

#ifndef PKCS11_MBEDTLS_MAX_OBJECT_COUNT
#define PKCS11_MBEDTLS_MAX_OBJECT_COUNT PKCS11_MBEDTLS_MAX_HEAP_OBJECT_COUNT+PKCS11_MBEDTLS_MAX_KEY_OBJECT_COUNT
#endif


//Check for the MBEDTLS implementation size in config.h
#ifndef PKCS11_MBEDTLS_MAX_IV_LEN
#define PKCS11_MBEDTLS_MAX_IV_LEN 16
#endif

#define PKCS11_MBEDTLS_MAX_SESSION_COUNT 5


///////////////Macro Definitions
#define PKCS11_MBEDTLS_HEADER() \
    CK_RV retVal = CKR_OK

#define PKCS11_MBEDTLS_CLEANUP() \
    CleanUp:

#define PKCS11_MBEDTLS_RETURN() \
    return retVal

#define PKCS11_MBEDTLS_NOCLEANUP() \
    PKCS11_MBEDTLS_CLEANUP(); \
    PKCS11_MBEDTLS_RETURN()

#define PKCS11_MBEDTLS_LEAVE() \
    goto CleanUp

#define PKCS11_MBEDTLS_SET_AND_LEAVE(x) \
{ \
    retVal = x; \
    PKCS11_MBEDTLS_LEAVE(); \
}

#define PKCS11_MBEDTLS_CHECKRESULT(x) \
    if((x) != 0) PKCS11_MBEDTLS_SET_AND_LEAVE(CKR_FUNCTION_FAILED)

#define PKCS11_MBEDTLS_CHECK_CK_RESULT(x) \
    if(CKR_OK != (retVal = x)) PKCS11_MBEDTLS_LEAVE()

/////////////Malloc and Free////////////////

//#define PKCS11_MBEDTLS_MALLOC(x) \
//	CLR_RT_Memory::Allocate(x,CLR_RT_HeapBlock::HB_Unmovable | CLR_RT_HeapBlock::HB_Alive)

#define PKCS11_MBEDTLS_MALLOC(x) \
	CLR_RT_Memory::Allocate(x)


#define PKCS11_MBEDTLS_FREE(x) \
	CLR_RT_Memory::Release(x)

/////String operations
#define PKCS11_MBEDTLS_STRCAT              strcat
#define PKCS11_MBEDTLS_STRCPY              strcpy
#define PKCS11_MBEDTLS_STRLEN              strlen
#define PKCS11_MBEDTLS_STRNCPY             strncpy
#define PKCS11_MBEDTLS_STRNCMP             strncmp
#define PKCS11_MBEDTLS_STRCMP              strcmp
#define PKCS11_MBEDTLS_STRNCASECMP         strncasecmp
#define PKCS11_MBEDTLS_STRCASECMP          strcasecmp
#define PKCS11_MBEDTLS_FPRINTF             fprintf
#define PKCS11_MBEDTLS_SNPRINTF            _snprintf
#define PKCS11_MBEDTLS_PRINTF              printf
#define PKCS11_MBEDTLS_STAT                stat
#define PKCS11_MBEDTLS_PERROR              perror

extern CK_SLOT_INFO  g_mbedTLS_SlotInfo;
extern CryptokiToken g_mbedTLS_Token;

typedef struct _KEY_DATA
{
	UINT8 		arrayIndex;
    CK_KEY_TYPE       type;
    CK_ULONG          size;
    KEY_ATTRIB        attrib;
    CK_VOID_PTR       key;
    CK_VOID_PTR       ctx;
} KEY_DATA;

typedef struct _CERT_DATA
{
    //X509* cert;
    KEY_DATA pubKeyData;
    KEY_DATA privKeyData;
} CERT_DATA;

typedef enum _ObjectType
{
    KeyType  = 1,
    DataType = 2,
    CertificateType = 3
} ObjectType;

typedef struct _OBJECT_DATA
{
    ObjectType        Type;
    CHAR              FileName[20];
    CHAR              GroupName[20];
    int               RefCount;
    CK_VOID_PTR       Data;
} OBJECT_DATA;

struct FIND_OBJECT_DATA
{
    UINT32  ObjectType;
    CHAR    FileName[20];
    CHAR    GroupName[20];
};

typedef enum _mbedTLSCryptoState
{
    Uninitialized,
    Initialized,
    InProgress,
} mbedTLSCryptoState;

typedef struct _mbedTLSEncryptData
{
    UINT8            IV[PKCS11_MBEDTLS_MAX_IV_LEN];
    mbedtls_cipher_context_t   SymmetricCtx;
    BOOL             IsSymmetric;
    KEY_DATA*        Key;
    BOOL             IsUpdateInProgress;
} mbedTLSEncryptData;


struct MBEDTLS_PKCS11_Token
{
    static CK_RV Initialize();
    static CK_RV Uninitialize();
    static CK_RV InitializeToken(CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel, CK_ULONG ulLabelLen);
    static CK_RV GetDeviceError(CK_ULONG_PTR pErrorCode);
};

struct MBEDTLS_PKCS11_Encryption
{
	static CK_RV InitHelper(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pEncryptMech, CK_OBJECT_HANDLE hKey, BOOL isEncrypt);

    static CK_RV EncryptInit     (Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pEncryptMech, CK_OBJECT_HANDLE hKey);
    static CK_RV Encrypt           (Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen);
    static CK_RV EncryptUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen);
    static CK_RV EncryptFinal    (Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pLastEncryptedPart, CK_ULONG_PTR pulLastEncryptedPartLen);

    static CK_RV DecryptInit     (Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pEncryptMech, CK_OBJECT_HANDLE hKey);
    static CK_RV Decrypt     (Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen);
    static CK_RV DecryptUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen);
    static CK_RV DecryptFinal  (Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen);
};

struct MBEDTLS_PKCS11_Session
{
    static CK_RV InitPin(Cryptoki_Session_Context* pSessionCtx, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen);
    static CK_RV SetPin(Cryptoki_Session_Context* pSessionCtx, CK_UTF8CHAR_PTR pOldPin, CK_ULONG ulOldPinLen, CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewPinLen);

    static CK_RV OpenSession(Cryptoki_Session_Context* pSessionCtx, CK_BBOOL fReadWrite);
    static CK_RV CloseSession(Cryptoki_Session_Context* pSessionCtx);

    static CK_RV Login(Cryptoki_Session_Context* pSessionCtx, CK_USER_TYPE userType, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen);
    static CK_RV Logout(Cryptoki_Session_Context* pSessionCtx);
};

struct MBEDTLS_PKCS11_Objects
{
    static CK_RV CreateObject(Cryptoki_Session_Context* pSessionCtx, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phObject);
    static CK_RV CopyObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phNewObject);
    static CK_RV DestroyObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject);
    static CK_RV GetObjectSize(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ULONG_PTR pulSize);
    static CK_RV GetAttributeValue(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount);
    static CK_RV SetAttributeValue(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount);    

    static CK_RV FindObjectsInit(Cryptoki_Session_Context* pSessionCtx, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount);
    static CK_RV FindObjects(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE_PTR phObjects, CK_ULONG ulMaxCount, CK_ULONG_PTR pulObjectCount);
    static CK_RV FindObjectsFinal(Cryptoki_Session_Context* pSessionCtx);

    //custom member variables
    static OBJECT_DATA*     GetObjectFromHandle(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject);
    static BOOL             FreeObject(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hObject);
    static CK_OBJECT_HANDLE AllocObject(Cryptoki_Session_Context* pSessionCtx, ObjectType type, size_t size, OBJECT_DATA** ppData);

    static void IntitializeObjects();
private:
   // static CK_RV LoadX509Cert(Cryptoki_Session_Context* pSessionCtx, X509* x, OBJECT_DATA** ppObject, EVP_PKEY* privateKey, CK_OBJECT_HANDLE_PTR phObject);
    static int FindEmptyObjectHandle();
    static OBJECT_DATA s_Objects[PKCS11_MBEDTLS_MAX_OBJECT_COUNT];
};

struct MBEDTLS_PKCS11_Digest
{
    static CK_RV DigestInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism);
    static CK_RV Digest(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen);
    static CK_RV Update(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen);
    static CK_RV DigestKey(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hKey);
    static CK_RV Final(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen);
};

struct MBEDTLS_PKCS11_Signature
{
    static CK_RV SignInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey);
    static CK_RV Sign(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen);
    static CK_RV SignUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pPart, CK_ULONG ulPartLen);
    static CK_RV SignFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen);

    static CK_RV VerifyInit(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey);
    static CK_RV Verify(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen);
    static CK_RV VerifyUpdate(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pPart, CK_ULONG ulPartLen);
    static CK_RV VerifyFinal(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen);
};


struct MBEDTLS_PKCS11_Keys
{
	static CK_RV DeleteKey(Cryptoki_Session_Context* pSessionCtx, KEY_DATA* pKey);
	static KEY_DATA* GetKeyFromHandle(Cryptoki_Session_Context* pSessionCtx, CK_OBJECT_HANDLE hKey, BOOL getPrivate);

    static CK_RV GenerateKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phKey);
    static CK_RV GenerateKeyPair(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, 
                             CK_ATTRIBUTE_PTR pPublicKeyTemplate , CK_ULONG ulPublicCount, 
                             CK_ATTRIBUTE_PTR pPrivateKeyTemplate, CK_ULONG ulPrivateCount, 
                             CK_OBJECT_HANDLE_PTR phPublicKey    , CK_OBJECT_HANDLE_PTR phPrivateKey);
    static CK_RV WrapKey  (Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hWrappingKey, CK_OBJECT_HANDLE hKey, CK_BYTE_PTR pWrappedKey, CK_ULONG_PTR pulWrappedKeyLen);
    static CK_RV UnwrapKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hUnwrappingKey, CK_BYTE_PTR pWrappedKey, CK_ULONG ulWrappedKeyLen, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulAttributeCount, CK_OBJECT_HANDLE_PTR phKey);
    static CK_RV DeriveKey(Cryptoki_Session_Context* pSessionCtx, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hBaseKey, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulAttributeCount, CK_OBJECT_HANDLE_PTR phKey);

    static CK_RV LoadSecretKey(Cryptoki_Session_Context* pSessionCtx, CK_KEY_TYPE keyType, const UINT8* pKey, CK_ULONG ulKeyLength, CK_OBJECT_HANDLE_PTR phKey);
    static CK_RV LoadRsaKey  (Cryptoki_Session_Context* pSessionCtx, const RsaKeyData& keyData, CK_BBOOL isPrivate, CK_OBJECT_HANDLE_PTR phKey);
    static CK_RV LoadDsaKey  (Cryptoki_Session_Context* pSessionCtx, const DsaKeyData& keyData, CK_BBOOL isPrivate, CK_OBJECT_HANDLE_PTR phKey);
    static CK_RV LoadEcKey   (Cryptoki_Session_Context* pSessionCtx, const EcKeyData&  keyData, CK_BBOOL isPrivate, CK_OBJECT_HANDLE_PTR phKey);
    static CK_RV LoadKeyBlob(Cryptoki_Session_Context* pSessionCtx, const PBYTE pKey, CK_ULONG keyLen, CK_KEY_TYPE keyType, KEY_ATTRIB keyAttrib, CK_OBJECT_HANDLE_PTR phKey);

private:
    static CK_OBJECT_HANDLE LoadKey(Cryptoki_Session_Context* pSessionCtx, void* pKey, CK_KEY_TYPE type, KEY_ATTRIB attrib, size_t keySize);
};

struct MBEDTLS_PKCS11_Random
{
    static CK_RV SeedRandom(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen);
    static CK_RV GenerateRandom(Cryptoki_Session_Context* pSessionCtx, CK_BYTE_PTR pRandomData, CK_ULONG ulRandomLen);    
};

#endif //_MBEDTLS_PKCS11_H_

