#include "mbedTLS_PKCS11.h"

static ICryptokiEncryption s_Encryption =
{
    MBEDTLS_PKCS11_Encryption::EncryptInit,
    MBEDTLS_PKCS11_Encryption::Encrypt,
    MBEDTLS_PKCS11_Encryption::EncryptUpdate,
    MBEDTLS_PKCS11_Encryption::EncryptFinal,
    MBEDTLS_PKCS11_Encryption::DecryptInit,
    MBEDTLS_PKCS11_Encryption::Decrypt,
    MBEDTLS_PKCS11_Encryption::DecryptUpdate,
    MBEDTLS_PKCS11_Encryption::DecryptFinal,
};

static ICryptokiDigest s_Digest = 
{
    MBEDTLS_PKCS11_Digest::DigestInit,
    MBEDTLS_PKCS11_Digest::Digest,
    MBEDTLS_PKCS11_Digest::Update,
    MBEDTLS_PKCS11_Digest::DigestKey,
    MBEDTLS_PKCS11_Digest::Final,
};

static ICryptokiSignature s_Sign = 
{
    MBEDTLS_PKCS11_Signature::SignInit,
    MBEDTLS_PKCS11_Signature::Sign,
    MBEDTLS_PKCS11_Signature::SignUpdate,
    MBEDTLS_PKCS11_Signature::SignFinal,
    
    MBEDTLS_PKCS11_Signature::VerifyInit,
    MBEDTLS_PKCS11_Signature::Verify,
    MBEDTLS_PKCS11_Signature::VerifyUpdate,
    MBEDTLS_PKCS11_Signature::VerifyFinal,
};

static ICryptokiKey s_Key =
{
    MBEDTLS_PKCS11_Keys::GenerateKey,
    MBEDTLS_PKCS11_Keys::GenerateKeyPair,
    MBEDTLS_PKCS11_Keys::WrapKey,
    MBEDTLS_PKCS11_Keys::UnwrapKey,
    MBEDTLS_PKCS11_Keys::DeriveKey,
    MBEDTLS_PKCS11_Keys::LoadSecretKey,
    MBEDTLS_PKCS11_Keys::LoadRsaKey,
    MBEDTLS_PKCS11_Keys::LoadDsaKey,
    MBEDTLS_PKCS11_Keys::LoadEcKey,
    MBEDTLS_PKCS11_Keys::LoadKeyBlob,
};

static ICryptokiObject s_Object = 
{
    MBEDTLS_PKCS11_Objects::CreateObject,
    MBEDTLS_PKCS11_Objects::CopyObject,
    MBEDTLS_PKCS11_Objects::DestroyObject,
    MBEDTLS_PKCS11_Objects::GetObjectSize,
    MBEDTLS_PKCS11_Objects::GetAttributeValue,
    MBEDTLS_PKCS11_Objects::SetAttributeValue,
    
    MBEDTLS_PKCS11_Objects::FindObjectsInit,
    MBEDTLS_PKCS11_Objects::FindObjects,
    MBEDTLS_PKCS11_Objects::FindObjectsFinal,
};

static ICryptokiRandom s_Random = 
{
    MBEDTLS_PKCS11_Random::SeedRandom,
    MBEDTLS_PKCS11_Random::GenerateRandom,
};

static ICryptokiSession s_Session = 
{
    MBEDTLS_PKCS11_Session::OpenSession,
    MBEDTLS_PKCS11_Session::CloseSession,
    MBEDTLS_PKCS11_Session::InitPin,
    MBEDTLS_PKCS11_Session::SetPin,
    MBEDTLS_PKCS11_Session::Login,
    MBEDTLS_PKCS11_Session::Logout,
};

static ICryptokiToken s_Token =
{
    MBEDTLS_PKCS11_Token::Initialize,
    MBEDTLS_PKCS11_Token::Uninitialize,
    MBEDTLS_PKCS11_Token::InitializeToken,
    MBEDTLS_PKCS11_Token::GetDeviceError,
};

CK_SLOT_INFO g_mbedTLS_SlotInfo =
{
    "mbedTLS_PKCS11_Crypto",
    "<ProviderNameHere>", 
    CKF_TOKEN_PRESENT, 
    { 0, 0 }, 
    { 1, 0 }
};

static CryptokiMechanism s_Mechanisms[] = 
{
    // Add your supported mechanisms here
	//{ CKM_EC_KEY_PAIR_GEN      , { 256,  521, CKF_GENERATE_KEY_PAIR                            } },
	//{ CKM_ECDSA                , { 256,  521,                             CKF_SIGN | CKF_VERIFY} },
	//{ CKM_ECDH1_DERIVE         , { 256,  521, CKF_DERIVE                                       } },
	{ CKM_GENERIC_SECRET_KEY_GEN,{   1, 3072, CKF_GENERATE                                     } },
	//{ CKM_DES3_KEY_GEN         , {  56,  168, CKF_GENERATE                                     } },
	//{ CKM_DES3_CBC             , {  56,  168, CKF_ENCRYPT | CKF_DECRYPT                        } },
	//{ CKM_DES3_CBC_PAD         , {  56,  168, CKF_ENCRYPT | CKF_DECRYPT                        } },
	{ CKM_MD5_HMAC             , { 128,  128, CKF_DIGEST                | CKF_SIGN | CKF_VERIFY} },
	{ CKM_MD5                  , { 128,  128, CKF_DIGEST                                       } },
	//{ CKM_SHA_1_HMAC           , { 160,  160, CKF_DIGEST                | CKF_SIGN | CKF_VERIFY} },
	{ CKM_SHA256_HMAC          , { 256,  256, CKF_DIGEST                | CKF_SIGN | CKF_VERIFY} },
	//{ CKM_SHA384_HMAC          , { 384,  384, CKF_DIGEST                | CKF_SIGN | CKF_VERIFY} },
	//{ CKM_SHA512_HMAC          , { 512,  512, CKF_DIGEST                | CKF_SIGN | CKF_VERIFY} },
	{ CKM_RIPEMD160_HMAC       , { 160,  160, CKF_DIGEST                | CKF_SIGN | CKF_VERIFY} },
	{ CKM_SHA_1                , { 160,  160, CKF_DIGEST                                       } },
	//{ CKM_SHA224               , { 224,  224, CKF_DIGEST                                       } },
	{ CKM_SHA256               , { 256,  256, CKF_DIGEST                                       } },
	//{ CKM_SHA384               , { 384,  384, CKF_DIGEST                                       } },
	//{ CKM_SHA512               , { 512,  512, CKF_DIGEST                                       } },
	{ CKM_RIPEMD160            , { 160,  160, CKF_DIGEST                                       } },
	//{ CKM_DSA_KEY_PAIR_GEN     , {1024, 2048, CKF_GENERATE_KEY_PAIR                            } },
	//{ CKM_DSA                  , {1024, 3072,                             CKF_SIGN | CKF_VERIFY} },
	//{ CKM_RSA_PKCS_KEY_PAIR_GEN, {1024, 2048, CKF_GENERATE_KEY_PAIR                            } },
	//{ CKM_RSA_PKCS             , {1024, 2048, CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY} },
	{ CKM_AES_CBC              , { 128,  256, CKF_ENCRYPT | CKF_DECRYPT                        } },
	{ CKM_AES_CBC_PAD          , { 128,  256, CKF_ENCRYPT | CKF_DECRYPT                        } },
	//{ CKM_AES_ECB              , { 128,  256, CKF_ENCRYPT | CKF_DECRYPT                        } },
	//{ CKM_AES_ECB_PAD          , { 128,  256, CKF_ENCRYPT | CKF_DECRYPT                        } },
	{ CKM_AES_KEY_GEN          , { 128,  256, CKF_GENERATE                                     } },
};

CryptokiToken g_mbedTLS_Token =
{
    { // TOKEN INFO
        "<TokenLable>",
        "<TokenManufacturer>",
        "<model>",
        "<serialNumber>",
        CKF_RNG | CKF_PROTECTED_AUTHENTICATION_PATH | CKF_TOKEN_INITIALIZED,
        10, // max session count
        0, // session count
        10, // max rw session count
        0, // rw session count
        0, // max pin len
        0, // min pin len
        1024, // Total public mem
        1024, // free public mem
        0, // total private mem
        0, // free private mem

        { 1, 0 }, // hardware version
        { 1, 0 }, // firmware version
        "201001010101000", // utc time
    },

    ROPublic,  // token wide session state
    {   // key wrapping mechanism
        CKM_AES_CBC,
        NULL,
        0
    },
    L"PinWrappingKey",
    4*1024,
    ARRAYSIZE(s_Mechanisms),
    s_Mechanisms,

    &s_Token,
    &s_Encryption,
    &s_Digest,
    &s_Sign,
    &s_Key,
    &s_Object,
    &s_Random,    
    &s_Session,
    NULL,  // persistent storage 
};

